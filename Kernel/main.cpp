
#include "apiserver_km.h"
#include <stdio.h>
#include <stdarg.h>
#include <ntimage.h>

/*
 * Kernel mode ALPC server that is notified of receives through an executive
 * callback that is executed in the context of the sender's thread.
 */



extern "C"
{
	NTSTATUS
	FindNtoskrnl(
		_Out_ PULONG_PTR KernelBase,
		_Out_ PULONG KernelSize
		);

	ULONG_PTR
	FindPattern(
		_In_ ULONG_PTR Base,
		_In_ ULONG Size,
		_In_ const UCHAR *Pattern,
		_In_ ULONG PatternLength
		);
	
	_Function_class_(DRIVER_INITIALIZE)
	_IRQL_requires_(PASSIVE_LEVEL)
	DRIVER_INITIALIZE
	DriverEntry;

	_Function_class_(DRIVER_UNLOAD)
	_IRQL_requires_(PASSIVE_LEVEL)
	DRIVER_UNLOAD
	DriverUnload;

	_Function_class_(CALLBACK_FUNCTION)
	_IRQL_requires_same_
	CALLBACK_FUNCTION
	AlpcCallback;

	_IRQL_requires_max_(APC_LEVEL)
	NTSTATUS
	ExUnregisterCallbackDeferred(
		_Inout_ PVOID CallbackRegistration
		);

	_Function_class_(IO_WORKITEM_ROUTINE_EX)
	_IRQL_requires_(PASSIVE_LEVEL)
	_IRQL_requires_same_
	IO_WORKITEM_ROUTINE_EX
	ExUnregisterCallbackDeferredWorker;
}

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, AlpcCallback)
#pragma alloc_text(PAGE, ExUnregisterCallbackDeferred)
#pragma alloc_text(PAGE, ExUnregisterCallbackDeferredWorker)
#pragma alloc_text(INIT, FindNtoskrnl)
#pragma alloc_text(INIT, FindPattern)
#pragma alloc_text(INIT, DriverEntry)
#endif

HANDLE ApiPort = nullptr;												// API connection port handle
static FAST_MUTEX ClientLock;											// Held while client data is manipulated
static PVOID CallbackRegistration = nullptr;							// Callback registration object
static PALPC_PORT_CALLBACK_INFORMATION CallbackInformation = nullptr;	// ALPC port callback registration information

VOID
Printf(
	_In_ PCCH Format,
	...
	)
{
	CHAR Message[512];
	va_list VaList;
	va_start(VaList, Format);
	const ULONG N = _vsnprintf_s(Message, sizeof(Message) - 1, Format, VaList);
	Message[N] = L'\0';

	vDbgPrintExWithPrefix("[UNMAEWEI SERVER] ", DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, Message, VaList);

	va_end(VaList);
}

NTSTATUS
FindNtoskrnl(
	_Out_ PULONG_PTR KernelBase,
	_Out_ PULONG KernelSize
	)
{
	PAGED_CODE();

	*KernelBase = 0;
	*KernelSize = 0;

	NTSTATUS Status;
	ULONG Size;
	if ((Status = ZwQuerySystemInformation(SystemModuleInformation, nullptr, 0, &Size)) != STATUS_INFO_LENGTH_MISMATCH)
		return Status;
	const PRTL_PROCESS_MODULES Modules =
		static_cast<PRTL_PROCESS_MODULES>(ExAllocatePoolWithTag(NonPagedPoolNx, 2 * Size, 'ldoM'));
	if (Modules == nullptr)
		return STATUS_NO_MEMORY;

	Status = ZwQuerySystemInformation(SystemModuleInformation,
										Modules,
										2 * Size,
										&Size);
	if (NT_SUCCESS(Status))
	{
		*KernelBase = reinterpret_cast<ULONG_PTR>(Modules->Modules[0].ImageBase);
		*KernelSize = Modules->Modules[0].ImageSize;
	}

	ExFreePool(Modules);

	return Status;
}

ULONG_PTR
FindPattern(
	_In_ ULONG_PTR Base,
	_In_ ULONG Size,
	_In_ const UCHAR *Pattern,
	_In_ ULONG PatternLength
	)
{
	PAGED_CODE();

	for (PUCHAR Address = reinterpret_cast<PUCHAR>(Base);
		Address < reinterpret_cast<PUCHAR>(Base + Size - PatternLength);
		++Address)
	{
		ULONG i;
		for (i = 0; i < PatternLength; ++i)
		{
			if (Pattern[i] != 0xCC && (*(Address + i) != Pattern[i]))
				break;
		}

		if (i == PatternLength)
			return reinterpret_cast<ULONG_PTR>(Address);
	}
	return 0;
}

_Use_decl_annotations_
NTSTATUS
ExUnregisterCallbackDeferred(
	_Inout_ PVOID Registration
	)
{
	PAGED_CODE();

	// \Device\RawDisk belongs to \FileSystem\RAW, which is created by ntoskrnl and always exists
	UNICODE_STRING RawDiskDeviceName = RTL_CONSTANT_STRING(L"\\Device\\RawDisk");
	PDEVICE_OBJECT DeviceObject;
	PFILE_OBJECT FileObject;
	const NTSTATUS Status = IoGetDeviceObjectPointer(&RawDiskDeviceName,
													FILE_READ_ATTRIBUTES,
													&FileObject,
													&DeviceObject);
	if (!NT_SUCCESS(Status))
		return Status;
	ObDereferenceObject(FileObject);

	const PIO_WORKITEM WorkItem = IoAllocateWorkItem(DeviceObject);
	if (WorkItem == nullptr)
		return STATUS_NO_MEMORY;

	// Queue a work item to unregister the callback from a different thread
	IoQueueWorkItemEx(WorkItem,
					ExUnregisterCallbackDeferredWorker,
					DelayedWorkQueue,
					Registration);

	return Status;
}

_Use_decl_annotations_
VOID
ExUnregisterCallbackDeferredWorker(
	_In_ PVOID IoObject,
	_In_opt_ PVOID Context,
	_In_ PIO_WORKITEM IoWorkItem
	)
{
	PAGED_CODE();

	UNREFERENCED_PARAMETER(IoObject);

	IoFreeWorkItem(IoWorkItem);

	ExUnregisterCallback(Context);
}

_Use_decl_annotations_
NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
	)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	PAGED_CODE();

	ULONG_PTR KernelBase;
	ULONG KernelSize;
	NTSTATUS Status = FindNtoskrnl(&KernelBase, &KernelSize);
	if (!NT_SUCCESS(Status))
	{
		Printf("Failed to find kernel base: %08X\n", Status);
		return Status;
	}

	const PIMAGE_NT_HEADERS NtHeaders = RtlImageNtHeader(reinterpret_cast<PVOID>(KernelBase));
	if (NtHeaders == nullptr)
		return STATUS_INVALID_IMAGE_FORMAT;
	const PIMAGE_SECTION_HEADER TextSection = IMAGE_FIRST_SECTION(NtHeaders);

	// Find a 'jmp rcx' gadget in ntoskrnl to use as registered callback object function, with our actual callback as the context passed
	const PVOID Rcx = reinterpret_cast<PVOID>(AlpcCallback);
	constexpr UCHAR JmpRcxPattern[] = { 0xFF, 0xE1 };
	const PVOID JmpRcx = reinterpret_cast<PVOID>(FindPattern(KernelBase + TextSection->VirtualAddress,
															TextSection->Misc.VirtualSize,
															JmpRcxPattern,
															sizeof(JmpRcxPattern)));

	if (JmpRcx == nullptr)
	{
		Printf("Failed to find 'jmp rcx' gadget.\n");
		return STATUS_PROCEDURE_NOT_FOUND;
	}
	Printf("Using 'jmp rcx' gadget at 0x%p.\n", JmpRcx);

	OBJECT_ATTRIBUTES ObjectAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(static_cast<PUNICODE_STRING>(nullptr), OBJ_KERNEL_HANDLE);
	ExInitializeFastMutex(&ClientLock);

	// Create the ALPC port
	Status = CreateApiPort();
	if (!NT_SUCCESS(Status))
	{
		Printf("Failed to create port %wZ: ZwAlpcCreatePort error %08X\n", &PortName, Status);
		goto ExitFailure;
	}
	Printf("Created port %wZ.\n", &PortName);

	CallbackInformation = static_cast<PALPC_PORT_CALLBACK_INFORMATION>(ExAllocatePoolWithTag(NonPagedPoolNx, sizeof(ALPC_PORT_CALLBACK_INFORMATION), 'kcbC'));
	if (CallbackInformation == nullptr)
	{
		Status = STATUS_NO_MEMORY;
		goto ExitFailure;
	}

	// Create an executive callback object
	RtlZeroMemory(CallbackInformation, sizeof(*CallbackInformation));
	Status = ExCreateCallback(&CallbackInformation->CallbackObject,
							&ObjectAttributes,
							TRUE,
							FALSE);
	if (!NT_SUCCESS(Status))
	{
		Printf("ExCreateCallback error %08X\n", Status);
		goto ExitFailure;
	}

	// Register our jmp gadget as the callback object's function, and our real callback as the context
	CallbackRegistration = ExRegisterCallback(CallbackInformation->CallbackObject,
											reinterpret_cast<PCALLBACK_FUNCTION>(JmpRcx),
											Rcx);
	if (CallbackRegistration == nullptr)
	{
		ObDereferenceObject(CallbackInformation->CallbackObject);
		Printf("Failed to register callback.\n");
		Status = STATUS_DRIVER_UNABLE_TO_LOAD;
		goto ExitFailure;
	}

	// Register the callback object with the port so that the callback is invoked when a message is received.
	Status = ZwAlpcSetInformation(ApiPort,
								AlpcRegisterCallbackInformation, // Kernel mode only
								CallbackInformation,
								sizeof(*CallbackInformation)); // This value is ignored
	ObDereferenceObject(CallbackInformation->CallbackObject);
	if (!NT_SUCCESS(Status))
	{
		Printf("ZwAlpcSetInformation(AlpcRegisterCallbackInformation) error %08X\n", Status);
		goto ExitFailure;
	}
	Printf("Registered ALPC callback function.\n");

	// If we were not manually mapped, set the driver unload function
	if (DriverObject != nullptr && MmIsAddressValid(DriverObject))
		DriverObject->DriverUnload = DriverUnload;

	Printf("Driver loaded successfully.\n");

	return STATUS_SUCCESS;

ExitFailure:
	if (ApiPort != nullptr)
		ZwClose(ApiPort);
	if (CallbackRegistration != nullptr)
		ExUnregisterCallback(CallbackRegistration);
	if (CallbackInformation != nullptr)
		ExFreePool(CallbackInformation);

	return Status;
}

_Use_decl_annotations_
VOID
DriverUnload(
	_In_ PDRIVER_OBJECT DriverObject
	)
{
	PAGED_CODE();

	if (ApiPort != nullptr)
		ZwClose(ApiPort);
	ApiPort = nullptr;
	
	if (CallbackRegistration != nullptr)
	{
		if (DriverObject != nullptr)
			ExUnregisterCallback(CallbackRegistration);
		else
			ExUnregisterCallbackDeferred(CallbackRegistration);
	}
	CallbackRegistration = nullptr;
	
	if (CallbackInformation != nullptr)
		ExFreePool(CallbackInformation);
	CallbackInformation = nullptr;

	Printf("Driver unloaded.\n");
}

_Use_decl_annotations_
VOID
AlpcCallback(
	_In_opt_ PVOID CallbackContext,
	_In_opt_ PVOID Argument1,
	_In_opt_ PVOID Argument2
	)
{
	UNREFERENCED_PARAMETER(CallbackContext);
	UNREFERENCED_PARAMETER(Argument1);
	UNREFERENCED_PARAMETER(Argument2);

	PAGED_CODE();

	MYAPI_MESSAGE MessageBuffer;
	PMYAPI_MESSAGE ReceiveMessage = &MessageBuffer;

	// Initialize ALPC message attributes
	UCHAR ReceiveMessageAttributesBuffer[sizeof(ALPC_MESSAGE_ATTRIBUTES) +
										sizeof(ALPC_CONTEXT_ATTR) +
										sizeof(ALPC_DATA_VIEW_ATTR)];

	const PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes =
		reinterpret_cast<PALPC_MESSAGE_ATTRIBUTES>(ReceiveMessageAttributesBuffer);

	NT_ASSERTMSG("Receive message attributes size mismatch",
		AlpcGetHeaderSize(ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE) ==
		sizeof(ReceiveMessageAttributesBuffer));

	SIZE_T BufferSize;
	AlpcInitializeMessageAttribute(ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE,
									ReceiveMessageAttributes,
									sizeof(ReceiveMessageAttributesBuffer),
									&BufferSize);

	// Allow context and data view attributes in messages received by us
	ReceiveMessageAttributes->ValidAttributes = ALPC_MESSAGE_CONTEXT_ATTRIBUTE | ALPC_MESSAGE_VIEW_ATTRIBUTE;

	// Call ZwAlpcSendWaitReceivePort to retrieve a message from the queue.
	// If there is no message queued, STATUS_TIMEOUT is returned immediately.
	NTSTATUS Status = ZwAlpcSendWaitReceivePort(ApiPort,
												0,
												nullptr,
												nullptr,
												&ReceiveMessage->Header,
												nullptr,
												ReceiveMessageAttributes,
												nullptr);

	if (!NT_SUCCESS(Status) || Status == STATUS_TIMEOUT)
	{
		if (Status != STATUS_TIMEOUT)
			Printf("ZwAlpcSendWaitReceivePort error %08X\n", Status);
		return;
	}

	// Retrieve the context attribute and get our client identifier from it if there is one
	const PALPC_CONTEXT_ATTR ContextAttributes = static_cast<PALPC_CONTEXT_ATTR>(
		AlpcGetMessageAttribute(ReceiveMessageAttributes, ALPC_MESSAGE_CONTEXT_ATTRIBUTE));

	const PMYAPI_CLIENT ApiClient = static_cast<PMYAPI_CLIENT>(ContextAttributes->PortContext);

	// The high byte of the message type field is only used for flags like LPC_KERNELMODE_MESSAGE.
	const LPC_TYPE MessageType = static_cast<LPC_TYPE>(static_cast<SHORT>(ReceiveMessage->Header.u2.s2.Type) & 0xFF);
	NT_ASSERT(ApiClient != nullptr || MessageType == LPC_TYPE::LPC_CONNECTION_REQUEST || MessageType == LPC_TYPE::LPC_PORT_CLOSED);

	switch (MessageType) {
		case LPC_TYPE::LPC_CONNECTION_REQUEST: {
			// Connection request. Reply with either acceptance or refusal and add the client if accepted
			ExAcquireFastMutexUnsafe(&ClientLock);
			HandleConnectionRequest(ReceiveMessage);
			ExReleaseFastMutexUnsafe(&ClientLock);
			break;
		}
		case LPC_TYPE::LPC_PORT_CLOSED:
		case LPC_TYPE::LPC_CLIENT_DIED: {
			// Client is gone
			Printf("Client disconnected.\n");
			ExAcquireFastMutexUnsafe(&ClientLock);
			DeleteClient(ApiClient);
			ExReleaseFastMutexUnsafe(&ClientLock);
			break;
		}
		case LPC_TYPE::LPC_REQUEST: {
			// API request.
			//Printf("Received request message for API number %u.\n", static_cast<ULONG>(ReceiveMessage->ApiNumber));
			ReceiveMessage->Status = STATUS_SUCCESS;

			if (ReceiveMessage->ApiNumber >= MYAPI_API_NUMBER::MyApiMaximumNumber) {
				Printf("API number is invalid.\n");
				ReceiveMessage->Status = STATUS_ILLEGAL_FUNCTION;
			}
			else {
				// Handle the request
				HandleApiRequest(ReceiveMessage, ReceiveMessageAttributes);
			}
			break;
		}
		default:
		{
			// Some other message type; log it and ignore. LPC_EXCEPTION and LPC_ERROR_EVENT are mostly for CSRSS
			Printf("Received message with unexpected type %u (0x%04X)\n",
				MessageType, ReceiveMessage->Header.u2.s2.Type);
			break;
		}
	}

	if ((static_cast<SHORT>(ReceiveMessage->Header.u2.s2.Type) & LPC_CONTINUATION_REQUIRED) &&
		MessageType != LPC_TYPE::LPC_CONNECTION_REQUEST) {
		// This must be replied to with ALPC_MSGFLG_RELEASE_MESSAGE. See https://www.youtube.com/watch?v=UNpL5csYC1E
		PMYAPI_MESSAGE ReplyMessage = ReceiveMessage;

		// Check if there was a view attached to a message that shouldn't have one, and don't leak it if so
		if ((ReceiveMessageAttributes->ValidAttributes & ALPC_MESSAGE_VIEW_ATTRIBUTE)) {
			const PALPC_DATA_VIEW_ATTR DataView = static_cast<PALPC_DATA_VIEW_ATTR>(
												AlpcGetMessageAttribute(ReceiveMessageAttributes,
																		ALPC_MESSAGE_VIEW_ATTRIBUTE));
			if (DataView != nullptr && DataView->ViewBase != nullptr)
				ZwAlpcDeleteSectionView(ApiPort, 0, DataView->ViewBase);
		}

		// Send the reply.
		Status = ZwAlpcSendWaitReceivePort(ApiPort,
											ALPC_MSGFLG_RELEASE_MESSAGE,
											&ReplyMessage->Header,
											nullptr,
											nullptr,
											nullptr,
											nullptr,
											nullptr);
		if (!NT_SUCCESS(Status)) {
			Printf("ZwAlpcSendWaitReceivePort returned status %08X (replying to a message with type %04X)\n",
				Status, static_cast<SHORT>(ReceiveMessage->Header.u2.s2.Type));
		}
	}

	// Check if this was a request for a driver unload
	if (ReceiveMessage != nullptr && MessageType == LPC_TYPE::LPC_REQUEST &&
		ReceiveMessage->ApiNumber == MYAPI_API_NUMBER::MyApiUnloadDriverNumber)
	{
		Printf("Received unload request.\n");
		DriverUnload(nullptr);
	}
}
