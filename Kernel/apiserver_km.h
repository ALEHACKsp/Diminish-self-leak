#pragma once

#include "zwapi.h"
#include "../Common/global.h"
#include "../Common/myapi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MYAPI_CLIENT			// Stored for each connected client
{
	CLIENT_ID ClientId;
	HANDLE PortHandle;
	ALPC_DATA_VIEW_ATTR DataView;
} MYAPI_CLIENT, *PMYAPI_CLIENT;

#define MAX_CLIENT_CONNECTIONS	256

extern HANDLE ApiPort;					// API connection port handle

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
	_In_ PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes
	);

// Client connection support functions
PMYAPI_CLIENT
CreateClient(
	_In_ PCLIENT_ID ClientId
	);

VOID
DeleteClient(
	_Inout_opt_ PMYAPI_CLIENT ApiClient
	);

#ifdef __cplusplus
}
#endif
