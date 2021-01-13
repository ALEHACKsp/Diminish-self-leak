#pragma once

/*
 * Example dummy API
 */

// Name of the ALPC port
static UNICODE_STRING PortName = RTL_CONSTANT_STRING(L"\\RPC Control\\6C22DBA1-3921-4F37-80BB-C259D59447BB");

// Possible API request message numbers
enum class MYAPI_API_NUMBER : ULONG
{
	MyApiReadMemory,
	MyApiWriteMemrory,
	MyApiGetModuleBase,
	MyApiWriteReadOnly,
	MyApiGetContext,
	MyApiSetContext,
	
	MyApiUnloadDriverNumber, // Request to unload the driver. Not part of the API proper
	MyApiMaximumNumber
};

typedef struct _READ_PROCESS {
	ULONG PID;
	PVOID Destination;
	PVOID Source;
	ULONG Size;
} READ_PROCESS, *PREAD_PROCESS;

typedef struct _WRITE_PROCESS {
	ULONG PID;
	PVOID Destination;
	PVOID Source;
	ULONG Size;
} WRITE_PROCESS, * PWRITE_PROCESS;

typedef struct _WRITE_READ_ONLY {
	ULONG ProcessId;
	PVOID Address;
	ULONG Size;
	ULONG* InOutProtect;
} WRITE_READ_ONLY, * PWRITE_READ_ONLY;

typedef struct _GET_MODULE_BASE
{
	ULONG PID;
	const char* ModueBaseName;
	ULONGLONG BaseAddress;
} GET_MODULE_BASE, *PGET_MODULE_BASE;

typedef struct _GET_CONTEXT
{
	ULONG PID;
	ULONGLONG Handle;
	ULONGLONG ThreadOwner;

} GET_CONTEXT, *PGET_CONTEXT;

typedef struct _SET_CONTEXT
{
	ULONG PID;
	ULONGLONG Handle;
	ULONGLONG ThreadOwner;
	NTSTATUS Result;

} SET_CONTEXT, * PSET_CONTEXT;

// Connection info received after server accepts connection
typedef struct _MYAPI_CONNECTINFO
{
	HANDLE ServerProcessId;
} MYAPI_CONNECTINFO, *PMYAPI_CONNECTINFO;

// Complete API message struct
typedef struct _MYAPI_MESSAGE
{
	PORT_MESSAGE Header;
	union
	{
		MYAPI_CONNECTINFO ConnectionInfo;
		struct
		{
			MYAPI_API_NUMBER ApiNumber;
			NTSTATUS Status;
			union
			{
				_READ_PROCESS ReadProcessMemory;
				_WRITE_PROCESS WriteProcessMemory;
				_GET_MODULE_BASE GetModuleBase;
				_WRITE_READ_ONLY WriteReadOnly;
				_GET_CONTEXT GetContext;
				_SET_CONTEXT SetContext;
			} Data;
		};
	};
} MYAPI_MESSAGE, *PMYAPI_MESSAGE;

static_assert(sizeof(MYAPI_MESSAGE) <= PORT_TOTAL_MAXIMUM_MESSAGE_LENGTH, "Maximum ALPC message size exceeded");
