#pragma once

#include <global.h>
#include <myapi.h>

typedef struct _MYAPI_CLIENT			// Stored for each connected client
{
	LPC_CLIENT_ID ClientId;
	HANDLE PortHandle;
	ALPC_DATA_VIEW_ATTR DataView;
} MYAPI_CLIENT, *PMYAPI_CLIENT;

#define NUM_SERVER_THREADS		8
#define MAX_CLIENT_CONNECTIONS	256

extern HANDLE ApiPort;					// API connection port handle
extern ULONG NumWorkerThreads;			// Number of worker threads

// Main API port functions
NTSTATUS
CreateApiPort(
	);

NTSTATUS
HandleConnectionRequest(
	_In_ PMYAPI_MESSAGE ReceiveMessage
	);

NTSTATUS
HandleApiRequest(
	_Inout_ PMYAPI_MESSAGE ApiMessage,
	_In_opt_ PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes
	);

// Client connection support functions
PMYAPI_CLIENT
CreateClient(
	_In_ PLPC_CLIENT_ID ClientId
	);

VOID
DeleteClient(
	_Inout_opt_ PMYAPI_CLIENT ApiClient
	);

// Debug functions
VOID
UpdateConsoleTitle(
	);
