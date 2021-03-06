#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <WinUser.h>
#include <memoryapi.h>
#include <memory>
#include <Psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "..\..\Common\global.h"
#include "..\..\Common\myapi.h"
#include "..\..\Common\ntdll.h"
#include "..\..\Common\Console\Console.h"
//#include "/Users/madma/OneDrive/Desktop/Diminsh/Diminsh.VIP/Common/global.h"
//#include "/Users/madma/OneDrive/Desktop/Diminsh/Diminsh.VIP/Common/myapi.h"
//#include "/Users/madma/OneDrive/Desktop/Diminsh/Diminsh.VIP/Common/ntdll.h"
//#include "../Console/Console.h"

#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "ntdll.lib")

#define KIX 0

inline HANDLE PortHandle = nullptr;
inline MYAPI_MESSAGE ConnectionMessage = { 0 };
inline PMYAPI_CONNECTINFO ConnectionInfo = &ConnectionMessage.ConnectionInfo;
inline UCHAR MessageAttributesBuffer[sizeof(ALPC_MESSAGE_ATTRIBUTES) + sizeof(ALPC_DATA_VIEW_ATTR)] = { 0 };
inline PALPC_MESSAGE_ATTRIBUTES SendMessageAttributes = reinterpret_cast<PALPC_MESSAGE_ATTRIBUTES>(MessageAttributesBuffer);

#ifdef KIX
inline HANDLE Handle;
#endif
inline DWORD ProcessID;
inline UINT64 GameBase;

static
BOOL
NTAPI
CtrlHandler(
	_In_ ULONG /*fdwCtrlType*/
)
{
	Console::PrintInfo("Received CTRL+C or CTRL+BREAK event. \n");
	if (PortHandle != nullptr)
	{
		NtClose(PortHandle);
		PortHandle = nullptr;
		Console::PrintInfo("Port closed. \n");
	}
	return TRUE; // Do not pass to next handler
}

FORCEINLINE
VOID
InitializeApiMessageLengths(
	_Inout_ PMYAPI_MESSAGE Message,
	_In_ SIZE_T TotalMessageLength
)
{
	// Wow64 uses 64 bit types everywhere but here
	const SHORT DataLength = static_cast<SHORT>(TotalMessageLength - sizeof(PORT_MESSAGE) + (Lpc6432 ? sizeof(LPC_CLIENT_ID) : 0));
	Message->Header.u1.s1.TotalLength = static_cast<SHORT>(TotalMessageLength);
	Message->Header.u1.s1.DataLength = DataLength;
}

NTSTATUS
Connect(
	_In_ PUNICODE_STRING ServerPortName,
	_Out_ PHANDLE ClientPortHandle
)
{
	Console::PrintOk("Connecting to NasaServer server {} \n", ServerPortName);

	SECURITY_QUALITY_OF_SERVICE SecurityQoS;
	SecurityQoS.Length = sizeof(SecurityQoS);
	SecurityQoS.ImpersonationLevel = SecurityImpersonation; // Allow local impersonation
	SecurityQoS.ContextTrackingMode = SECURITY_DYNAMIC_TRACKING; // Lowest overhead
	SecurityQoS.EffectiveOnly = TRUE; // Forbid acquiring of any additional privileges in our security context

	ALPC_PORT_ATTRIBUTES PortAttributes = { 0 };
	PortAttributes.Flags = ALPC_PORFLG_CAN_IMPERSONATE;
	PortAttributes.MaxMessageLength = sizeof(MYAPI_MESSAGE);
	PortAttributes.SecurityQos = SecurityQoS;

	const SIZE_T TotalMessageSize = sizeof(MYAPI_MESSAGE);
	InitializeApiMessageLengths(&ConnectionMessage, TotalMessageSize);

	//auto Status = NtAlpcConnectPort(ClientPortHandle, ServerPortName, NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL);

	const NTSTATUS Status = NtAlpcConnectPort(ClientPortHandle,
		ServerPortName,
		nullptr,
		&PortAttributes,
		ALPC_MSGFLG_SYNC_REQUEST,
		nullptr,
		&ConnectionMessage.Header,
		nullptr,
		nullptr,
		nullptr,
		nullptr);

	if (Status == STATUS_PORT_CONNECTION_REFUSED)
	{
		Console::PrintFail("Connection refused. \n");
	}
	else if (!NT_SUCCESS(Status))
	{
		WCHAR Buffer[512];
		//FormatErrorMessage(0, Status, Buffer, ARRAYSIZE(Buffer));
		Console::PrintFail("CNtAlpcConnectPort failed: {} \n", Status);
	}
	else
	{
		Console::PrintOk("Server PID -> {} \n", static_cast<ULONG>(reinterpret_cast<ULONG_PTR>(ConnectionInfo->ServerProcessId)));
	}
	return Status;
}

NTSTATUS
CheckFulfillment(
	_In_ NTSTATUS Status,
	_In_ _MYAPI_MESSAGE RequestMessage
) {
	if (!NT_SUCCESS(Status))
	{
		if (IS_DISCONNECT_STATUS(Status))
		{
			return PortHandle != nullptr ? Status : STATUS_SUCCESS;
		}
		WCHAR Buffer[512];
		//FormatErrorMessage(0, Status, Buffer, ARRAYSIZE(Buffer));
		wprintf(L"NtAlpcSendWaitReceivePort failed:\n\t%ls", Status);
		return STATUS_UNSUCCESSFUL;
	}
	else if (!NT_SUCCESS(RequestMessage.Status)) // Server responded with an error status
	{
		wprintf(L"Received error status %08X from server.\n", RequestMessage.Status);
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}

NTSTATUS
ReadProcMem(
	_Out_   PVOID Destination,
	_Inout_ PVOID Source,
	_In_    DWORD Size
)
{
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PREAD_PROCESS ReadProcessMemory = &RequestMessage.Data.ReadProcessMemory;

	ReadProcessMemory->Destination = Destination;
	ReadProcessMemory->Source = Source;
	ReadProcessMemory->Size = Size;
	ReadProcessMemory->PID = ProcessID;

	MessageSize = sizeof(*ReadProcessMemory);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiReadMemory;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		return RequestMessage.Status;
	}

	return STATUS_UNSUCCESSFUL;
}

NTSTATUS
WriteProcMem(
	_Out_   PVOID Destination,
	_Inout_ PVOID Source,
	_In_    DWORD Size
)
{
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PWRITE_PROCESS WriteProcessMemory = &RequestMessage.Data.WriteProcessMemory;

	WriteProcessMemory->Destination = Destination;
	WriteProcessMemory->Source = Source;
	WriteProcessMemory->Size = Size;
	WriteProcessMemory->PID = ProcessID;

	MessageSize = sizeof(*WriteProcessMemory);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiWriteMemrory;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		RequestMessage.Data.WriteProcessMemory.Source;
	}

	return STATUS_UNSUCCESSFUL;
}

ULONGLONG
GetBaseAddress(
	_In_ ULONG PID,
	_In_ const char* ModuleName
)
{
#ifdef KIX
	HMODULE h_modules[1024];
	DWORD pcb_needed;

	if (EnumProcessModules(Handle, h_modules, sizeof(h_modules), &pcb_needed))
	{
		for (unsigned int i = 0; i < (pcb_needed / sizeof(HMODULE)); i++)
		{
			TCHAR module_name[MAX_PATH];
			if (GetModuleFileNameEx(Handle, h_modules[i], module_name, sizeof(module_name) / sizeof(TCHAR)))
			{
				std::string str_module_name = module_name;
				if (str_module_name.find(ModuleName) != std::string::npos)
				{
					GameBase = (UINT64)h_modules[i];
					return GameBase;
				}
			}
		}
	}

	return 0;
#endif
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PGET_MODULE_BASE ApiGetBase = &RequestMessage.Data.GetModuleBase;

	ApiGetBase->PID = ProcessID;
	ApiGetBase->ModueBaseName = ModuleName;

	MessageSize = sizeof(*ApiGetBase);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiGetModuleBase;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		return RequestMessage.Data.GetModuleBase.BaseAddress;
	}

	return STATUS_UNSUCCESSFUL;
}

NTSTATUS
ChangeProtection(
	_In_	PVOID Address,
	_In_	DWORD Size,
	_Inout_ DWORD *Protection
)
{
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PWRITE_READ_ONLY WriteReadOnly = &RequestMessage.Data.WriteReadOnly;

	WriteReadOnly->ProcessId = ProcessID;
	WriteReadOnly->Address = Address;
	WriteReadOnly->Size = Size;
	WriteReadOnly->InOutProtect = Protection;

	MessageSize = sizeof(*WriteReadOnly);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiWriteReadOnly;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		return RequestMessage.Status;
	}

	return STATUS_UNSUCCESSFUL;
}

ULONGLONG
GetContext(
	_In_ ULONGLONG WindowHandle,
	_In_ ULONG PID
)
{
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PGET_CONTEXT GetContext = &RequestMessage.Data.GetContext;

	GetContext->PID = PID;
	GetContext->Handle = WindowHandle;

	MessageSize = sizeof(*GetContext);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiGetContext;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		printf("GetContext\n");
		return GetContext->ThreadOwner;
	}

	return STATUS_UNSUCCESSFUL;
}

NTSTATUS
SetContext(
	_In_ ULONGLONG WindowHandle,
	_In_ ULONGLONG WindowOwner,
	_In_ ULONG PID
)
{
	SIZE_T MessageSize;
	MYAPI_MESSAGE RequestMessage;

	RtlZeroMemory(&RequestMessage.Header, sizeof(PORT_MESSAGE));
	PSET_CONTEXT SetContext = &RequestMessage.Data.SetContext;

	SetContext->PID = PID;
	SetContext->Handle = WindowHandle;
	SetContext->ThreadOwner = WindowHandle;

	MessageSize = sizeof(*SetContext);

	RequestMessage.ApiNumber = MYAPI_API_NUMBER::MyApiSetContext;
	RequestMessage.Status = STATUS_UNSUCCESSFUL;

	const SIZE_T TotalMessageSize = FIELD_OFFSET(MYAPI_MESSAGE, Data) + MessageSize;
	InitializeApiMessageLengths(&RequestMessage, TotalMessageSize);

	NTSTATUS Status = NtAlpcSendWaitReceivePort(PortHandle, ALPC_MSGFLG_SYNC_REQUEST, &RequestMessage.Header, nullptr, &RequestMessage.Header, nullptr, nullptr, nullptr);

	if (NT_SUCCESS(CheckFulfillment(Status, RequestMessage))) {
		printf("GetContext\n");
		return SetContext->Result;
	}

	return STATUS_UNSUCCESSFUL;
}

template<typename T>
T Read(
	_Inout_ UINT64 Address
	)
{
	T Buffer{};
#ifdef KIX
	ReadProcessMemory(Handle, (LPCVOID)Address, &Buffer, sizeof(T), nullptr);
#else
	ReadProcMem(reinterpret_cast<void*>(&Buffer), (reinterpret_cast<void*>(Address)), sizeof(T));
#endif
	return Buffer;
}

template<typename T>
void Write(
	_In_	UINT64 Address, 
	_Inout_ T Buffer
	)
{
#ifdef KIX
	WriteProcessMemory(Handle, (LPVOID)Address, &Buffer, sizeof(T), nullptr);
#else
	WriteProcMem(reinterpret_cast<PVOID>(Address), &Buffer, sizeof(T));
#endif
}

template<typename T>
void WriteReadOnly(
	_In_	UINT64 Address,
	_Inout_ T Buffer,
	_Inout_ DWORD Protection
	)
{
#ifdef KIX
	Write<BYTE>(Address, Buffer);
#else
	ChangeProtection(reinterpret_cast<PVOID>(Address), sizeof(Buffer), &Protection);
	Write<BYTE>(Address, Buffer);
	ChangeProtection(reinterpret_cast<PVOID>(Address), sizeof(Buffer), &Protection);
#endif
}

NTSTATUS Setup() {
	NTSTATUS Status;
	if (!SetConsoleCtrlHandler(static_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE))
		return STATUS_UNSUCCESSFUL;

	GetWindowThreadProcessId(FindWindowW(L"R6Game", nullptr), &ProcessID);
	Console::PrintOk("PID -> {} \n", ProcessID);
	if (ProcessID) Status = STATUS_SUCCESS;
	else Status = STATUS_FAIL_CHECK;

#ifdef KIX
	Handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	if (Handle) Status = STATUS_SUCCESS; 
	else Status = STATUS_FAIL_CHECK;
#else
	Status = Connect(&PortName, &PortHandle);
#endif

	GameBase = GetBaseAddress(ProcessID, "RainbowSix.exe");
	Console::PrintOk("BaseAddress -> {:X} \n", GameBase);
	if (GameBase) Status = STATUS_SUCCESS;
	else Status = STATUS_FAIL_CHECK;

	if (!NT_SUCCESS(Status))
		return Status;

#ifdef KIX
	return Status;
#endif

	SIZE_T BufferSize;
	AlpcInitializeMessageAttribute(ALPC_MESSAGE_VIEW_ATTRIBUTE, SendMessageAttributes, sizeof(MessageAttributesBuffer), &BufferSize);
	return STATUS_SUCCESS;
}

NTSTATUS Exit(NTSTATUS Status) {
	// Close up
	if (PortHandle != nullptr)
	{
		NtClose(PortHandle);
		PortHandle = nullptr;
		Console::PrintInfo("Port closed.");
	}

	if (!NT_SUCCESS(Status) && !IS_DISCONNECT_STATUS(Status))
	{
		//RaiseHardErrorForStatus(Status);
		Console::PrintInfo("Press any key to exit.");
	}
	//Console::PrintInfo("Press any key to exit.");
	WaitForKey();
	return Status;
}
