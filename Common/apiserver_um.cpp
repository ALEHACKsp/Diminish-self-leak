#include <apiserver_um.h>
#include <stdio.h>

/*
 * API functionality implemented by the user mode server
 */

static ULONG NumConnectedClients = 0;	// Number of clients currently connected

NTSTATUS
CreateApiPort(
	)
{
	OBJECT_ATTRIBUTES ObjectAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&PortName, OBJ_CASE_INSENSITIVE);

	// Apparently this is fine. Taken from rpcrt4.dll
	ALPC_PORT_ATTRIBUTES PortAttributes = { 0 };
	PortAttributes.Flags = 0;
	PortAttributes.MaxMessageLength = sizeof(MYAPI_MESSAGE);
	PortAttributes.MemoryBandwidth = 0;
	PortAttributes.MaxPoolUsage = MAXUINT32;
	PortAttributes.MaxSectionSize = MAXUINT32;
	PortAttributes.MaxViewSize = MAXUINT32;
	PortAttributes.MaxTotalSectionSize = MAXUINT32;

	return NtAlpcCreatePort(&ApiPort,
							&ObjectAttributes,
							&PortAttributes);
}

NTSTATUS
HandleConnectionRequest(
	_In_ PMYAPI_MESSAGE ReceiveMessage
	)
{
	wprintf(L"Received connection request from PID %u.\n",
		static_cast<ULONG>(reinterpret_cast<ULONG64>(ReceiveMessage->Header.ClientId.UniqueProcess)));

	// Note: the connection info is only actually returned if the connection is accepted
	ReceiveMessage->ConnectionInfo.ServerProcessId = NtCurrentTeb()->ClientId.UniqueProcess;

	PMYAPI_CLIENT ApiClient = nullptr;
	HANDLE PortHandle = nullptr;
	const BOOLEAN AcceptConnection = NumConnectedClients < MAX_CLIENT_CONNECTIONS;

	if (AcceptConnection)
	{
		// Create a new client that we will pass as port context
		ApiClient = CreateClient(&ReceiveMessage->Header.ClientId);
	}

	// Accept/reject the request
	NTSTATUS Status = NtAlpcAcceptConnectPort(&PortHandle,
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
		wprintf(L"NtAlpcAcceptConnectPort error %08X.\n", Status);
		return Status;
	}

	wprintf(L"Successfully %ls connection.\n",
		AcceptConnection ? L"accepted" : L"refused");

	if (AcceptConnection)
	{
		// Set the port handle on the new API client
		ApiClient->PortHandle = PortHandle;
	}

	return Status;
}

NTSTATUS
HandleApiRequest(
	_Inout_ PMYAPI_MESSAGE ApiMessage,
	_In_opt_ PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes
	)
{
	UNREFERENCED_PARAMETER(ReceiveMessageAttributes);
	
	// Normally you'd index into a dispatch table and call some function pointer for the API number here
	switch (ApiMessage->ApiNumber)
	{
		case MYAPI_API_NUMBER::MyApiUpcaseStringNumber:
		{
			const PUPCASE_STRING_MESSAGE UpcaseStringMessage = &ApiMessage->Data.UpcaseStringMessage;
			UNICODE_STRING ReceivedValue;
			RtlInitUnicodeString(&ReceivedValue, UpcaseStringMessage->Value);
			DECLARE_UNICODE_STRING_SIZE(ReplyValue, sizeof(UPCASE_STRING_MESSAGE::Value) / sizeof(WCHAR));

			const NTSTATUS Status = RtlUpcaseUnicodeString(&ReplyValue, &ReceivedValue, FALSE);
			if (NT_SUCCESS(Status))
			{
				RtlZeroMemory(UpcaseStringMessage->Value, sizeof(UpcaseStringMessage->Value));
				RtlCopyMemory(UpcaseStringMessage->Value, ReplyValue.Buffer, ReplyValue.Length);
			}
			break;
		}
		case MYAPI_API_NUMBER::MyApiMultiplyNumbersNumber:
		{
			const PMULTIPLY_NUMBERS_MESSAGE MultiplyNumbersMessage = &ApiMessage->Data.MultiplyNumbersMessage;
			MultiplyNumbersMessage->Result = MultiplyNumbersMessage->X * MultiplyNumbersMessage->Y;
			break;
		}
		case MYAPI_API_NUMBER::MyApiLongRunningComputationNumber:
		{
			const PLONG_RUNNING_COMPUTATION_MESSAGE LongRunningComputationMessage = &ApiMessage->Data.LongRunningComputationMessage;
			RtlSleep(LongRunningComputationMessage->AmountOfTimeToWork);
			break;
		}
		default:
			break;
	}
	return STATUS_SUCCESS;
}

PMYAPI_CLIENT
CreateClient(
	_In_ PLPC_CLIENT_ID ClientId
	)
{
	PMYAPI_CLIENT ApiClient = static_cast<PMYAPI_CLIENT>(
		RtlAllocateHeap(RtlProcessHeap(),
						HEAP_ZERO_MEMORY,
						sizeof(MYAPI_CLIENT)));
	ApiClient->ClientId = *ClientId;
	NumConnectedClients++;

	UpdateConsoleTitle();

	return ApiClient;
}

VOID
DeleteClient(
	_Inout_opt_ PMYAPI_CLIENT ApiClient
	)
{
	if (NumConnectedClients > 0 && ApiClient != nullptr)
	{
		if (ApiClient->PortHandle != nullptr)
		{
			NtAlpcDisconnectPort(ApiClient->PortHandle,
								ALPC_CANCELFLG_TRY_CANCEL);
			NtClose(ApiClient->PortHandle);
		}

		RtlFreeHeap(RtlProcessHeap(), 0, ApiClient);
		NumConnectedClients--;
	}

	UpdateConsoleTitle();
}

VOID
UpdateConsoleTitle(
	)
{
#ifdef _DEBUG
	wchar_t Buffer[64] = { L'\0' };
	_snwprintf(Buffer, 64, L"PID %u: %u threads, %u clients",
		HandleToULong(NtCurrentTeb()->ClientId.UniqueProcess), NUM_SERVER_THREADS, NumConnectedClients);
	SetConsoleTitleW(Buffer);
#endif
}
