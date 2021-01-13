#include "apiserver_km.h"
#include "zwapi.h"
#include <ntstrsafe.h>


/*
 * API functionality implemented by the kernel mode server
 */

#pragma alloc_text(PAGE, CreateApiPort)
#pragma alloc_text(PAGE, HandleConnectionRequest)
#pragma alloc_text(PAGE, HandleApiRequest)
#pragma alloc_text(PAGE, CreateClient)
#pragma alloc_text(PAGE, DeleteClient)


static ULONG NumConnectedClients = 0;	// Number of clients currently connected
ULONG PIDOfClient;

NTSTATUS
CreateApiPort(
	)
{
	PAGED_CODE();

	OBJECT_ATTRIBUTES ObjectAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&PortName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE);

	// Apparently this is fine. Taken from rpcrt4.dll
	ALPC_PORT_ATTRIBUTES PortAttributes = { 0 };
	PortAttributes.Flags = ALPC_PORFLG_SYSTEM_PROCESS; // This flag is required, at least when talking to user mode.
	PortAttributes.MaxMessageLength = sizeof(MYAPI_MESSAGE);
	PortAttributes.MemoryBandwidth = 0;
	PortAttributes.MaxPoolUsage = MAXUINT32;
	PortAttributes.MaxSectionSize = MAXUINT32;
	PortAttributes.MaxViewSize = MAXUINT32;
	PortAttributes.MaxTotalSectionSize = MAXUINT32;

	return ZwAlpcCreatePort(&ApiPort,
							&ObjectAttributes,
							&PortAttributes);
}

NTSTATUS
HandleConnectionRequest(
	_In_ PMYAPI_MESSAGE ReceiveMessage
	)
{
	PAGED_CODE();

	PIDOfClient = static_cast<ULONG>(reinterpret_cast<ULONG_PTR>(ReceiveMessage->Header.ClientId.UniqueProcess));

	Printf("Received connection request from PID %u.\n", PIDOfClient);

	// Note: the connection info is only actually returned if the connection is accepted
	ReceiveMessage->ConnectionInfo.ServerProcessId = PsGetCurrentProcessId();

	PMYAPI_CLIENT ApiClient = nullptr;
	HANDLE PortHandle = nullptr;
	const BOOLEAN AcceptConnection = NumConnectedClients < MAX_CLIENT_CONNECTIONS; // Most secure driver award goes to...

	if (AcceptConnection)
	{
		// Create a new client that we will pass as port context
		ApiClient = CreateClient(&ReceiveMessage->Header.ClientId);
		if (ApiClient == nullptr)
		{
			return STATUS_INSUFFICIENT_RESOURCES;
		}
	}

	// Accept/reject the request
	const NTSTATUS Status = ZwAlpcAcceptConnectPort(&PortHandle,
													ApiPort,
													0,
													nullptr,
													nullptr,
													ApiClient,
													&ReceiveMessage->Header,
													nullptr,
													AcceptConnection);
													
	if (!NT_SUCCESS(Status))
	{
		if (AcceptConnection)
			DeleteClient(ApiClient);
		Printf("ZwAlpcAcceptConnectPort error %08X\n", Status);
		return Status;
	}

	Printf("Successfully %hs connection.\n",
		AcceptConnection ? "accepted" : "refused");

	if (AcceptConnection)
	{
		// Set the port handle on the new API client
		ApiClient->PortHandle = PortHandle;
	}

	return Status;
}

bool SafeCopy(
	PVOID Destination, 
	PVOID Source,
	SIZE_T Size
	) 
{
	SIZE_T returnSize = 0;
	if (NT_SUCCESS(MmCopyVirtualMemory(PsGetCurrentProcess(), Source, PsGetCurrentProcess(), Destination, Size, KernelMode, &returnSize)) && returnSize == Size) {
		return TRUE;
	}
	return FALSE;
}

ULONGLONG GetUserModule(
	_In_ PEPROCESS pProcess, 
	_In_ UNICODE_STRING* ModuleName, 
	_In_ BOOLEAN isWow64
	)
{
	ASSERT(pProcess != NULL);
	if (pProcess == NULL)
	{
		return NULL;
	}

	// Protect from UserMode AV
	__try
	{
		LARGE_INTEGER time = { 0 };
		time.QuadPart = -250ll * 10 * 1000;     // 250 msec.

		// Wow64 process
		if (isWow64)
		{
			PPEB32 pPeb32 = (PPEB32)PsGetProcessWow64Process(pProcess);
			if (pPeb32 == NULL)
			{
				return NULL;
			}

			// Wait for loader a bit
			for (INT i = 0; !pPeb32->Ldr && i < 10; i++)
			{
				KeDelayExecutionThread(KernelMode, TRUE, &time);
			}

			// Still no loader
			if (!pPeb32->Ldr)
			{
				return NULL;
			}

			// Search in InLoadOrderModuleList
			for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList.Flink;
				pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
				pListEntry = (PLIST_ENTRY32)pListEntry->Flink)
			{
				UNICODE_STRING ustr;
				PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

				RtlUnicodeStringInit(&ustr, (PWCH)pEntry->BaseDllName.Buffer);

				if (RtlCompareUnicodeString(&ustr, ModuleName, TRUE) == 0)
				{
					return (ULONGLONG)pEntry->DllBase;
				}
			}
		}
		// Native process
		else
		{
			PPEB pPeb = PsGetProcessPeb(pProcess);
			if (!pPeb)
			{
				return NULL;
			}


			for (INT i = 0; !pPeb->Ldr && i < 10; i++)
			{
				KeDelayExecutionThread(KernelMode, TRUE, &time);
			}

			if (!pPeb->Ldr)
			{
				return NULL;
			}

			// Search in InLoadOrderModuleList
			for (PLIST_ENTRY pListEntry = pPeb->Ldr->InLoadOrderModuleList.Flink;
				pListEntry != &pPeb->Ldr->InLoadOrderModuleList;
				pListEntry = pListEntry->Flink)
			{
				PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
				if (RtlCompareUnicodeString(&pEntry->BaseDllName, ModuleName, TRUE) == 0)
				{
					return (ULONGLONG)pEntry->DllBase;
				}
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return NULL;
}

struct TagThreadInfo
{
	PETHREAD OwningThread; // 0x0000
};
struct TagHwnd
{
	TagThreadInfo* ThreadInfo; // 0x0000
};
template <class Ty1, class Ty2> struct pair
{
	__forceinline pair(Ty1 first, Ty2 second) : first(first), second(second) {}
	__forceinline pair() : first(), second() {}

	Ty1 first;
	Ty2 second;
};
inline pair<UINT64, UINT32> GetDriverBase(const char* driver_name)
{

	UINT32 pool_size = 0;

	ZwQuerySystemInformation
	(
		SystemModuleInformation,
		&pool_size,
		0,
		reinterpret_cast<PULONG>(&pool_size)
	);

	const auto allocated_pool = ExAllocatePoolWithTag(NonPagedPool, pool_size, 'xutl');

	if (!allocated_pool)
		return {};

	const auto status = ZwQuerySystemInformation
	(
		SystemModuleInformation,
		allocated_pool,
		pool_size,
		0
	);

	if (!NT_SUCCESS(status))
	{
		ExFreePoolWithTag(allocated_pool, 'xutl');
		return {};
	}

	const auto module_list = reinterpret_cast<PSYSTEM_MODULE_INFORMATION>(allocated_pool);

	for (auto i = 0u; i < module_list->ModuleCount; i++)
	{
		const auto curr_module = module_list->Modules[i];

		if (strstr(reinterpret_cast<const char*>(curr_module.FullPathName), driver_name) == 0)
			continue;

		ExFreePoolWithTag(allocated_pool, 'xutl');
		return { reinterpret_cast<uintptr_t>(curr_module.ImageBase), curr_module.ImageSize };
	}

	ExFreePoolWithTag(allocated_pool, 'xutl');
	return {};
}
extern "C" __declspec(dllexport) PVOID __stdcall RtlFindExportedRoutineByName(PVOID, PCCH);
inline UINT8* GetDriverExport(UINT64 driver_address, const char* export_name) {
	return reinterpret_cast<UINT8*>(RtlFindExportedRoutineByName(reinterpret_cast<void*>(driver_address), export_name));
}

NTSTATUS
HandleApiRequest(
	_Inout_ PMYAPI_MESSAGE ApiMessage,
	_In_ PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes
	)
{
	PAGED_CODE();

	UNREFERENCED_PARAMETER(ReceiveMessageAttributes);

	switch (ApiMessage->ApiNumber)
	{
		case MYAPI_API_NUMBER::MyApiReadMemory:
		{
			PEPROCESS Process = NULL;
			PEPROCESS UMProcess = NULL;
			const PREAD_PROCESS ApiReadRequest = &ApiMessage->Data.ReadProcessMemory;

			NTSTATUS Status = PsLookupProcessByProcessId((HANDLE)ApiReadRequest->PID, &Process);
			Status = PsLookupProcessByProcessId((HANDLE)PIDOfClient, &UMProcess);

			if (NT_SUCCESS(Status)) {
				SIZE_T OutSize = 0;
				Status = MmCopyVirtualMemory(Process, ApiReadRequest->Source, UMProcess, ApiReadRequest->Destination, ApiReadRequest->Size, UserMode, &OutSize);
				ObDereferenceObject(Process);
			}

			break;
		}
		case MYAPI_API_NUMBER::MyApiWriteMemrory:
		{
			const PWRITE_PROCESS ApiWroteRequest = &ApiMessage->Data.WriteProcessMemory;


			PEPROCESS Process = NULL;
			PEPROCESS UMProcess = NULL;
			NTSTATUS Status = PsLookupProcessByProcessId((HANDLE)ApiWroteRequest->PID, &Process);
			Status = PsLookupProcessByProcessId((HANDLE)PIDOfClient, &UMProcess);

			if (NT_SUCCESS(Status)) {
				SIZE_T OutSize = 0;
				Status = MmCopyVirtualMemory(UMProcess, ApiWroteRequest->Source, Process, ApiWroteRequest->Destination, (SIZE_T)ApiWroteRequest->Size, UserMode, &OutSize);
				ObDereferenceObject(Process);
			}
			break;
		}
		case MYAPI_API_NUMBER::MyApiGetModuleBase:
		{
			const PGET_MODULE_BASE ApiGetBaseAddress = &ApiMessage->Data.GetModuleBase;
			ANSI_STRING AS;
			UNICODE_STRING ModuleName;
			ULONGLONG BaseAddress;
			KAPC_STATE APCState;

			RtlInitAnsiString(&AS, ApiGetBaseAddress->ModueBaseName);
			RtlAnsiStringToUnicodeString(&ModuleName, &AS, TRUE);

			PEPROCESS Process;
			PsLookupProcessByProcessId((HANDLE)ApiGetBaseAddress->PID, &Process);
			BOOLEAN isWow64;
			isWow64 = (PsGetProcessWow64Process(Process) != NULL) ? TRUE : FALSE;

			KeStackAttachProcess(Process, &APCState);
			BaseAddress = GetUserModule(Process, &ModuleName, isWow64);
			KeUnstackDetachProcess(&APCState);
			ApiGetBaseAddress->BaseAddress = BaseAddress;
			RtlFreeUnicodeString(&ModuleName);
			break;
		}
		case MYAPI_API_NUMBER::MyApiWriteReadOnly: 
		{
			Printf("called man");
			const PWRITE_READ_ONLY ApiWriteReadOnly = &ApiMessage->Data.WriteReadOnly;

			DWORD Protect;
			PEPROCESS Process;
			NTSTATUS Status = PsLookupProcessByProcessId((HANDLE)ApiWriteReadOnly->ProcessId, &Process);

			if (NT_SUCCESS(Status)) {
				if (SafeCopy(&Protect, reinterpret_cast<PVOID>(ApiWriteReadOnly->InOutProtect), sizeof(Protect))) {
					SIZE_T size = ApiWriteReadOnly->Size;

					KeAttachProcess(Process);
					Status = ZwProtectVirtualMemory(NtCurrentProcess(), &ApiWriteReadOnly->Address, &size, Protect, &Protect);
					KeDetachProcess();

					SafeCopy(reinterpret_cast<PVOID>(ApiWriteReadOnly->InOutProtect), &Protect, sizeof(Protect));
				}
				else {
					Status = STATUS_ACCESS_VIOLATION;
				}

				ObDereferenceObject(Process);
				break;
			}
		}
		case MYAPI_API_NUMBER::MyApiGetContext:
		{
			ULONGLONG TempResult = 0x0;
			const PGET_CONTEXT ApiGetGetContext = &ApiMessage->Data.GetContext;

			PEPROCESS TargeProcess;
			const auto [Win32kBase, Win32kSize] = GetDriverBase("win32kbase.sys");

			PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(ApiGetGetContext->PID), &TargeProcess);
			KeAttachProcess(TargeProcess);

			UINT64 HandleTable = *reinterpret_cast<UINT64*>(GetDriverExport(Win32kBase, "gpKernelHandleTable"));
			UINT16 WordChange = (UINT16)ApiGetGetContext->Handle;
			auto TagStructure = *reinterpret_cast<TagThreadInfo**>(HandleTable + ((WordChange + (WordChange * 2)) * 8) + 0x8);
			TempResult = reinterpret_cast<ULONGLONG>(TagStructure->OwningThread);
			KeDetachProcess();
			ObDereferenceObject(TargeProcess);
			ApiGetGetContext->ThreadOwner = TempResult;
			break;
		}
		case MYAPI_API_NUMBER::MyApiSetContext:
		{
			const PSET_CONTEXT ApiGetGetContext = &ApiMessage->Data.SetContext;

			PEPROCESS TargetProcess;

			const auto [Win32kBase, Win32kSize] = GetDriverBase("win32kbase.sys");
			PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(ApiGetGetContext->PID), &TargetProcess);
			KeAttachProcess(TargetProcess);

			UINT64 HandleTable = *reinterpret_cast<UINT64*>(GetDriverExport(Win32kBase, "gpKernelHandleTable"));
			UINT16 WordChange = (UINT16)ApiGetGetContext->Handle;

			auto TagStructure = *reinterpret_cast<TagThreadInfo**>(HandleTable + ((WordChange + (WordChange * 2)) * 8) + 0x8);

			TagStructure->OwningThread = reinterpret_cast<PETHREAD>(ApiGetGetContext->ThreadOwner);

			KeDetachProcess();
			ObDereferenceObject(TargetProcess);

			ApiGetGetContext->Result = true;
			break;
		}
		default:
			break;
	}
	return STATUS_SUCCESS;
}

PMYAPI_CLIENT
CreateClient(
	_In_ PCLIENT_ID ClientId
	)
{
	PAGED_CODE();

	PMYAPI_CLIENT ApiClient = static_cast<PMYAPI_CLIENT>(
		ExAllocatePoolWithTag(NonPagedPoolNx,
							sizeof(MYAPI_CLIENT),
							'tnlC'));

	if (ApiClient == nullptr)
		return nullptr;

	ApiClient->ClientId = *ClientId;
	NumConnectedClients++;

	return ApiClient;
}


VOID
DeleteClient(
	_Inout_opt_ PMYAPI_CLIENT ApiClient
	)
{
	PAGED_CODE();

	if (NumConnectedClients > 0 && ApiClient != nullptr)
	{
		if (ApiClient->PortHandle != nullptr)
		{
			ZwAlpcDisconnectPort(ApiClient->PortHandle,
								ALPC_CANCELFLG_TRY_CANCEL);
			ZwClose(ApiClient->PortHandle);
		}

		ExFreePoolWithTag(ApiClient, 'tnlC');
		NumConnectedClients--;
	}
}
