#include <global.h>

/*
 * User mode error handling support
 */

VOID
FormatErrorMessage(
	_In_opt_ ULONG Win32Error,
	_In_opt_ NTSTATUS NtStatus,
	_Out_ PWCHAR Buffer,
	_In_ ULONG BufferLength
	)
{
	// Check for 32/64 bit mismatches first
#ifndef _WIN64
	if (NtStatus == STATUS_INVALID_MESSAGE)
	{
		PPEB Peb32 = nullptr;
		const BOOLEAN IsWow64 = NT_SUCCESS(NtQueryInformationProcess(NtCurrentProcess,
																	ProcessWow64Information,
																	&Peb32,
																	sizeof(Peb32),
																	nullptr)) && Peb32 != nullptr;
		if (IsWow64 && !Lpc6432)
		{
			_snwprintf(Buffer, BufferLength - 1, L"%ls", L"USE_LPC6432 should be defined for WOW64 processes.\n");
			return;
		}
		else if (!IsWow64 && Lpc6432)
		{
			_snwprintf(Buffer, BufferLength - 1, L"%ls", L"USE_LPC6432 should not be defined when running on a native x86 OS.\n");
			return;
		}
	}
	else if (NtStatus == STATUS_BUFFER_TOO_SMALL && Lpc6432)
	{
		_snwprintf(Buffer, BufferLength - 1, L"%ls", L"USE_LPC6432 should not be defined when talking to 64 bit servers.\n");
		return;
	}
#endif

	// Standard error messages
	DWORD_PTR Args[3] = { 0 };
	if (!NT_SUCCESS(NtStatus))
	{
		static UNICODE_STRING NtdllName = RTL_CONSTANT_STRING(L"ntdll.dll");
		PVOID Ntdll;
		PMESSAGE_RESOURCE_ENTRY MsgResource;
		if (NT_SUCCESS(RtlFindMessage((LdrGetDllHandle(nullptr, nullptr, &NtdllName, &Ntdll), Ntdll),
									LDR_FORMAT_MESSAGE_FROM_SYSTEM_MESSAGE_TABLE,
									MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
									NtStatus,
									&MsgResource)))
		{
			Args[0] = NtStatus;
			Args[1] = reinterpret_cast<DWORD_PTR>(MsgResource->Text);
		}
	}

	if (Win32Error != 0)
	{
		static UNICODE_STRING Kernel32Name = RTL_CONSTANT_STRING(L"kernel32.dll");
		PVOID Kernel32;
		PMESSAGE_RESOURCE_ENTRY MsgResource;
		if (NT_SUCCESS(RtlFindMessage((LdrGetDllHandle(nullptr, nullptr, &Kernel32Name, &Kernel32), Kernel32),
									LDR_FORMAT_MESSAGE_FROM_SYSTEM_MESSAGE_TABLE,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									Win32Error,
									&MsgResource)))
		{
			const int Index = NT_SUCCESS(NtStatus) || Args[0] == 0 ? 0 : 2;
			Args[Index] = reinterpret_cast<DWORD_PTR>(MsgResource->Text);
		}
	}

	PCWSTR Format;
	if (Win32Error != 0 && NT_SUCCESS(NtStatus))
		Format = L"Win32 error: %1!s!";
	else if (Win32Error == 0 && !NT_SUCCESS(NtStatus))
		Format = L"[NTSTATUS 0x%1!08X!]: %2!s!";
	else
		Format = L"[NTSTATUS 0x%1!08X!]: %2!s! %3!s!";

	RtlFormatMessage(const_cast<PWSTR>(Format),
					BufferLength,
					FALSE,
					FALSE,
					TRUE,
					reinterpret_cast<va_list*>(Args),
					Buffer,
					BufferLength,
					nullptr);
}

NTSTATUS
RaiseHardErrorForStatus(
	_In_ NTSTATUS Status
	)
{
	WCHAR Buffer[512];
	FormatErrorMessage(0, Status, Buffer, sizeof(Buffer));

	UNICODE_STRING HardErrorMessage = RTL_CONSTANT_STRING(Buffer);
	ULONG_PTR ErrorParameters[] = { reinterpret_cast<ULONG_PTR>(&HardErrorMessage) };
	HARDERROR_RESPONSE Response;
	const NTSTATUS StatusCode = STATUS_FATAL_APP_EXIT | HARDERROR_OVERRIDE_ERRORMODE;

	return NtRaiseHardError(StatusCode,
							1,
							1,
							ErrorParameters,
							OptionOk,
							&Response);
}
