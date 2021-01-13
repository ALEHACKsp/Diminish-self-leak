#include "zwapi.h"
#include <ntstrsafe.h>

#define MM_UNLOADED_DRIVERS_SIZE        50
typedef struct _MM_UNLOADED_DRIVER
{
	UNICODE_STRING     Name;
	PVOID             ModuleStart;
	PVOID             ModuleEnd;
	ULONG64         UnloadTime;
} MM_UNLOADED_DRIVER, * PMM_UNLOADED_DRIVER;
typedef struct _PIDDBCACHE_ENTRY
{
	LIST_ENTRY        List;
	UNICODE_STRING    DriverName;
	ULONG            TimeDateStamp;
	NTSTATUS        LoadStatus;
	char            _0x0028[16];
} PIDDBCACHE_ENTRY, * PPIDDBCACHE_ENTRY;
typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;
typedef struct _PEB
{
	unsigned char Reserved1[2];
	unsigned char BeingDebugged;
	unsigned char Reserved2[1];
	PVOID Reserved3[2];
	PPEB_LDR_DATA Ldr;
	PVOID ProcessParameters;
	PVOID Reserved4[3];
	PVOID AtlThunkSListPtr;
	PVOID Reserved5;
	ULONG Reserved6;
	PVOID Reserved7;
	ULONG Reserved8;
	ULONG AtlThunkSListPtr32;
	PVOID Reserved9[45];
	unsigned char Reserved10[96];
	PVOID PostProcessInitRoutine;
	unsigned char Reserved11[128];
	PVOID Reserved12[1];
	ULONG SessionId;
} PEB, * PPEB;
typedef struct _SYSTEM_SERVICE_DESCRIPTOR_TABLE
{
	PULONG_PTR ServiceTableBase;
	PULONG     ServiceCounterTableBase;
	ULONG_PTR  NumberOfServices;
	PUCHAR     ParamTableBase;
} SYSTEM_SERVICE_DESCRIPTOR_TABLE, * PSYSTEM_SERVICE_DESCRIPTOR_TABLE;
typedef struct _SYSTEM_MODULE
{
	ULONG_PTR Reserved[2];
	PVOID     Base;
	ULONG     Size;
	ULONG     Flags;
	USHORT    Index;
	USHORT    Unknown;
	USHORT    LoadCount;
	USHORT    ModuleNameOffset;
	CHAR      ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;
typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG_PTR     ModuleCount;
	SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;
typedef struct _IMAGE_EXPORT_DIRECTORY
{
	DWORD  Characteristics;
	DWORD  TimeDateStamp;
	USHORT MajorVersion;
	USHORT MinorVersion;
	DWORD  Name;
	DWORD  Base;
	DWORD  NumberOfFunctions;
	DWORD  NumberOfNames;
	DWORD  AddressOfFunctions;
	DWORD  AddressOfNames;
	DWORD  AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;
typedef struct _IMAGE_DATA_DIRECTORY
{
	ULONG VirtualAddress;
	ULONG Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;
typedef struct _IMAGE_OPTIONAL_HEADER64
{
	USHORT    Magic;
	UCHAR     MajorLinkerVersion;
	UCHAR     MinorLinkerVersion;
	ULONG     SizeOfCode;
	ULONG     SizeOfInitializedData;
	ULONG     SizeOfUninitializedData;
	ULONG     AddressOfEntryPoint;
	ULONG     BaseOfCode;
	ULONGLONG ImageBase;
	ULONG     SectionAlignment;
	ULONG     FileAlignment;
	USHORT    MajorOperatingSystemVersion;
	USHORT    MinorOperatingSystemVersion;
	USHORT    MajorImageVersion;
	USHORT    MinorImageVersion;
	USHORT    MajorSubsystemVersion;
	USHORT    MinorSubsystemVersion;
	ULONG     Win32VersionValue;
	ULONG     SizeOfImage;
	ULONG     SizeOfHeaders;
	ULONG     CheckSum;
	USHORT    Subsystem;
	USHORT    DllCharacteristics;
	ULONGLONG SizeOfStackReserve;
	ULONGLONG SizeOfStackCommit;
	ULONGLONG SizeOfHeapReserve;
	ULONGLONG SizeOfHeapCommit;
	ULONG     LoaderFlags;
	ULONG     NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;
typedef struct _IMAGE_FILE_HEADER
{
	USHORT Machine;
	USHORT NumberOfSections;
	ULONG  TimeDateStamp;
	ULONG  PointerToSymbolTable;
	ULONG  NumberOfSymbols;
	USHORT SizeOfOptionalHeader;
	USHORT Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;
typedef struct _IMAGE_NT_HEADERS64
{
	ULONG                   Signature;
	IMAGE_FILE_HEADER       FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;
typedef struct _IMAGE_DOS_HEADER
{
	USHORT e_magic;
	USHORT e_cblp;
	USHORT e_cp;
	USHORT e_crlc;
	USHORT e_cparhdr;
	USHORT e_minalloc;
	USHORT e_maxalloc;
	USHORT e_ss;
	USHORT e_sp;
	USHORT e_csum;
	USHORT e_ip;
	USHORT e_cs;
	USHORT e_lfarlc;
	USHORT e_ovno;
	USHORT e_res[4];
	USHORT e_oemid;
	USHORT e_oeminfo;
	USHORT e_res2[10];
	LONG   e_lfanew;
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;
typedef struct _IMAGE_SECTION_HEADER
{
	unsigned char Name[8];
	union
	{
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	unsigned short  NumberOfRelocations;
	unsigned short  NumberOfLinenumbers;
	DWORD Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;
typedef struct _IMAGE_BASE_RELOCATION
{
	DWORD VirtualAddress;
	DWORD SizeOfBlock;
} IMAGE_BASE_RELOCATION, * PIMAGE_BASE_RELOCATION;
typedef struct _IMAGE_IMPORT_BY_NAME
{
	unsigned short Hint;
	CHAR Name[1];
} IMAGE_IMPORT_BY_NAME, * PIMAGE_IMPORT_BY_NAME;
typedef struct _IMAGE_THUNK_DATA64
{
	union
	{
		ULONGLONG ForwarderString;
		ULONGLONG Function;
		ULONGLONG Ordinal;
		ULONGLONG AddressOfData;
	} u1;
} IMAGE_THUNK_DATA64, * PIMAGE_THUNK_DATA64;
typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
	union
	{
		DWORD Characteristics;
		DWORD OriginalFirstThunk;
	};
	DWORD TimeDateStamp;
	DWORD ForwarderChain;
	DWORD Name;
	DWORD FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR, * PIMAGE_IMPORT_DESCRIPTOR;
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	union
	{
		LIST_ENTRY InInitializationOrderLinks;
		LIST_ENTRY InProgressLinks;
	};
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

inline auto dereference(uintptr_t address, unsigned int offset) -> uintptr_t
{
	if (address == 0)
		return 0;

	return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));
}
inline auto relative(uintptr_t address, unsigned int size) -> PVOID
{
	if (address == 0)
		return 0;

	return ((PVOID)((PBYTE)(address)+*(int*)((PBYTE)(address)+((size)-(INT)sizeof(INT))) + (size)));
}
inline auto compare_data(const unsigned char* pData, const unsigned char* bMask, const char* szMask) -> bool
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;

	return (*szMask) == 0;
}
inline auto find_pattern2(UINT64 dwAddress, UINT64 dwLen, unsigned char* bMask, const char* szMask) -> ULONGLONG
{
	for (ULONGLONG i = 0; i < dwLen; i++)
		if (compare_data((unsigned char*)(dwAddress + i), bMask, szMask))
			return (ULONGLONG)(dwAddress + i);

	return 0;
}
template <typename t = void*>
inline auto find_pattern(void* start, size_t length, const char* pattern, const char* mask) -> t
{
	const auto data = static_cast<const char*>(start);
	const auto pattern_length = strlen(mask);

	for (size_t i = 0; i <= length - pattern_length; i++)
	{
		bool accumulative_found = true;

		for (size_t j = 0; j < pattern_length; j++)
		{
			if (!MmIsAddressValid(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(data) + i + j)))
			{
				accumulative_found = false;
				break;
			}

			if (data[i + j] != pattern[j] && mask[j] != '?')
			{
				accumulative_found = false;
				break;
			}
		}

		if (accumulative_found)
		{
			return (t)(reinterpret_cast<uintptr_t>(data) + i);
		}
	}

	return (t)nullptr;
}
inline auto get_kernel_address(const char* name, size_t& size) -> uintptr_t
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG neededSize = 0;

	ZwQuerySystemInformation(
		SystemModuleInformation,
		&neededSize,
		0,
		&neededSize
	);

	PSYSTEM_MODULE_INFORMATION pModuleList;

	pModuleList = (PSYSTEM_MODULE_INFORMATION)ExAllocatePool(NonPagedPool, neededSize);

	if (!pModuleList) {
		return 0;
	}

	status = ZwQuerySystemInformation(SystemModuleInformation,
		pModuleList,
		neededSize,
		0
	);

	ULONG i = 0;
	uintptr_t address = 0;

	for (i = 0; i < pModuleList->ModuleCount; i++)
	{
		SYSTEM_MODULE mod = pModuleList->Modules[i];

		address = uintptr_t(pModuleList->Modules[i].Base);
		size = uintptr_t(pModuleList->Modules[i].Size);
		if (strstr(mod.ImageName, name) != NULL)
			break;
	}

	ExFreePool(pModuleList);

	return address;
}
inline auto resolve_relative_address(PVOID Instruction, ULONG OffsetOffset, ULONG InstructionSize) -> PVOID
{
	ULONG_PTR Instr = (ULONG_PTR)Instruction;
	LONG RipOffset = *(PLONG)(Instr + OffsetOffset);
	PVOID ResolvedAddr = (PVOID)(Instr + InstructionSize + RipOffset);

	return ResolvedAddr;
}
inline auto random_number() -> unsigned char
{
	size_t size;
	auto mod = get_kernel_address("ntoskrnl.exe", size);

	auto cMmGetSystemRoutineAddress = reinterpret_cast<decltype(&MmGetSystemRoutineAddress)>(get_exported_function((uintptr_t)mod, "MmGetSystemRoutineAddress"));

	UNICODE_STRING routineName = RTL_CONSTANT_STRING(L"RtlRandom");
	auto cRtlRandom = reinterpret_cast<decltype(&RtlRandom)>(cMmGetSystemRoutineAddress(&routineName));

	ULONG seed = 1234765;
	ULONG rand = cRtlRandom(&seed) % 100;

	unsigned char randint = 0;

	if (rand >= 101 || rand <= -1)
		randint = 72;

	return (unsigned char)(rand);
}
inline auto get_exported_function(const ULONGLONG mod, const char* name) -> ULONGLONG
{
	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(mod);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<ULONGLONG>(dos_header) + dos_header->e_lfanew);

	const auto data_directory = nt_headers->OptionalHeader.DataDirectory[0];
	const auto export_directory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(mod + data_directory.VirtualAddress);

	const auto address_of_names = reinterpret_cast<ULONG*>(mod + export_directory->AddressOfNames);

	for (size_t i = 0; i < export_directory->NumberOfNames; i++)
	{
		const auto function_name = reinterpret_cast<const char*>(mod + address_of_names[i]);

		if (!stricmp(function_name, name))
		{
			const auto name_ordinal = reinterpret_cast<unsigned short*>(mod + export_directory->AddressOfNameOrdinals)[i];

			const auto function_rva = mod + reinterpret_cast<ULONG*>(mod + export_directory->AddressOfFunctions)[name_ordinal];
			return function_rva;
		}
	}

	return 0;
}
inline auto get_ps_loaded() -> PERESOURCE
{
	size_t size;
	auto mod = get_kernel_address("ntoskrnl.exe", size);

	auto cMmGetSystemRoutineAddress = reinterpret_cast<decltype(&MmGetSystemRoutineAddress)>(get_exported_function((uintptr_t)mod, "MmGetSystemRoutineAddress"));

	ERESOURCE PsLoadedModuleResource;
	UNICODE_STRING routineName = RTL_CONSTANT_STRING(L"PsLoadedModuleResource");
	auto cPsLoadedModuleResource = reinterpret_cast<decltype(&PsLoadedModuleResource)>(cMmGetSystemRoutineAddress(&routineName));

	return cPsLoadedModuleResource;
}
inline auto get_piddb_table() -> PRTL_AVL_TABLE
{
	size_t size;
	uintptr_t ntos_base = get_kernel_address("ntoskrnl.exe", size);

	RTL_OSVERSIONINFOW osVersion = { 0 };
	osVersion.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&osVersion);

	PRTL_AVL_TABLE PiDDBCacheTable = nullptr;

	if (osVersion.dwBuildNumber >= 18362) {
		PiDDBCacheTable = (PRTL_AVL_TABLE)dereference(find_pattern<uintptr_t>((void*)ntos_base, size, "\x48\x8d\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x3d\x00\x00\x00\x00\x0f\x83", "xxx????x????x????xx"), 3);
	}
	else if (osVersion.dwBuildNumber >= 17134) {
		PiDDBCacheTable = (PRTL_AVL_TABLE)dereference(find_pattern<uintptr_t>((void*)ntos_base, size, "\x48\x8D\x0D\x00\x00\x00\x00\x4C\x89\x35\x00\x00\x00\x00\x49", "xxx????xxx????x"), 3);
	}

	if (!PiDDBCacheTable)
		return 0;

	return PiDDBCacheTable;
}
inline auto get_piddb_lock() -> PERESOURCE
{
	size_t size;
	uintptr_t ntos_base = get_kernel_address("ntoskrnl.exe", size);

	PERESOURCE PiDDBLock = (PERESOURCE)dereference(find_pattern<uintptr_t>((void*)ntos_base, size, "\x48\x8d\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x48\x8b\x0d\x00\x00\x00\x00\x33\xdb", "xxx????x????xxx????xx"), 3);

	if (!PiDDBLock)
		return 0;

	return PiDDBLock;
}
inline auto verify_piddb() -> bool
{
	return (get_piddb_lock() != 0 && get_piddb_table() != 0);
}
inline auto clean_piddb() -> bool
{
	PERESOURCE PiDDBLock = get_piddb_lock();
	PRTL_AVL_TABLE PiDDBCacheTable = get_piddb_table();

	PIDDBCACHE_ENTRY lookupEntry = { };
	lookupEntry.DriverName = RTL_CONSTANT_STRING(L"gdrv.sys");
	lookupEntry.TimeDateStamp = 0x51D4F9CB;

	ExAcquireResourceExclusiveLite(PiDDBLock, TRUE);

	auto pFoundEntry = (PPIDDBCACHE_ENTRY)RtlLookupElementGenericTableAvl(PiDDBCacheTable, &lookupEntry);
	if (pFoundEntry == nullptr)
	{
		ExReleaseResourceLite(PiDDBLock);
		return false;
	}

	RemoveEntryList(&pFoundEntry->List);
	RtlDeleteElementGenericTableAvl(PiDDBCacheTable, pFoundEntry);

	ExReleaseResourceLite(PiDDBLock);

	return true;
}

inline auto is_unload_empty(PMM_UNLOADED_DRIVER Entry) -> bool
{
	if (Entry->Name.MaximumLength == 0 ||
		Entry->Name.Length == 0 ||
		Entry->Name.Buffer == NULL)
	{
		return TRUE;
	}

	return FALSE;
}
inline auto get_mmu_address() -> PMM_UNLOADED_DRIVER
{
	size_t size;
	uintptr_t ntos_base = get_kernel_address("ntoskrnl.exe", size);

	/*
	*	nt!MmLocateUnloadedDriver:
	*	fffff801`51c70394 4c8b15a57e1500  mov     r10,qword ptr [nt!MmUnloadedDrivers (fffff801`51dc8240)]
	*	fffff801`51c7039b 4c8bc9          mov     r9 ,rcx
	*/
	PVOID MmUnloadedDriversInstr = (PVOID)find_pattern2((UINT64)ntos_base, size,
		(unsigned char*)"\x4C\x8B\x15\x00\x00\x00\x00\x4C\x8B\xC9",
		"xxx????xxx"
	);

	if (MmUnloadedDriversInstr == NULL)
	{
		return 0x0;
	}

	auto MmUnloadedDrivers = *(PMM_UNLOADED_DRIVER*)resolve_relative_address(MmUnloadedDriversInstr, 3, 7);

	return MmUnloadedDrivers;
}
inline auto get_mml_address() -> PULONG
{
	size_t size;
	uintptr_t ntos_base = get_kernel_address("ntoskrnl.exe", size);

	/*
		*	nt!MiRememberUnloadedDriver+0x59:
		*	fffff801`5201a4c5 8b057ddddaff    mov     eax,dword ptr [nt!MmLastUnloadedDriver (fffff801`51dc8248)]
		*	fffff801`5201a4cb 83f832          cmp     eax,32h
	*/
	PVOID MmLastUnloadedDriverInstr = (PVOID)find_pattern2((UINT64)ntos_base, size,
		(unsigned char*)"\x8B\x05\x00\x00\x00\x00\x83\xF8\x32",
		"xx????xxx"
	);

	if (MmLastUnloadedDriverInstr == NULL)
	{
		return 0x0;
	}

	auto MmLastUnloadedDriver = (PULONG)resolve_relative_address(MmLastUnloadedDriverInstr, 2, 6);

	return MmLastUnloadedDriver;
}
inline auto verify_mmu() -> bool
{
	return (get_mmu_address() != 0 && get_mml_address() != 0);
}
inline auto is_mmu_filled() -> bool
{
	for (ULONG Index = 0; Index < MM_UNLOADED_DRIVERS_SIZE; ++Index)
	{
		PMM_UNLOADED_DRIVER Entry = &get_mmu_address()[Index];
		if (is_unload_empty(Entry))
		{
			return FALSE;
		}
	}

	return TRUE;
}
inline auto clean_mmu() -> bool
{
	auto ps_loaded = get_ps_loaded();

	ExAcquireResourceExclusiveLite(ps_loaded, TRUE);

	BOOLEAN Modified = FALSE;
	BOOLEAN Filled = is_mmu_filled();

	UNICODE_STRING DriverName = RTL_CONSTANT_STRING(L"gdrv.sys");

	for (ULONG Index = 0; Index < MM_UNLOADED_DRIVERS_SIZE; ++Index)
	{
		PMM_UNLOADED_DRIVER Entry = &get_mmu_address()[Index];

		if (Modified)
		{
			//
			// Shift back all entries after modified one.
			//
			PMM_UNLOADED_DRIVER PrevEntry = &get_mmu_address()[Index - 1];
			RtlCopyMemory(PrevEntry, Entry, sizeof(MM_UNLOADED_DRIVER));

			//
			// Zero last entry.
			//
			if (Index == MM_UNLOADED_DRIVERS_SIZE - 1)
			{
				RtlFillMemory(Entry, sizeof(MM_UNLOADED_DRIVER), 0);
			}
		}
		else if (RtlEqualUnicodeString(&DriverName, &Entry->Name, TRUE))
		{
			//
			// Erase driver entry.
			//
			PVOID BufferPool = Entry->Name.Buffer;
			RtlFillMemory(Entry, sizeof(MM_UNLOADED_DRIVER), 0);
			ExFreePoolWithTag(BufferPool, 'TDmM');

			//
			// Because we are erasing last entry we want to set MmLastUnloadedDriver to 49
			// if list have been already filled.
			//
			*get_mml_address() = (Filled ? MM_UNLOADED_DRIVERS_SIZE : *get_mml_address()) - 1;
			Modified = TRUE;
		}
	}

	if (Modified)
	{
		ULONG64 PreviousTime = 0;

		//
		// Make UnloadTime look right.
		//
		for (LONG Index = MM_UNLOADED_DRIVERS_SIZE - 2; Index >= 0; --Index)
		{
			PMM_UNLOADED_DRIVER Entry = &get_mmu_address()[Index];
			if (is_unload_empty(Entry))
			{
				continue;
			}

			if (PreviousTime != 0 && Entry->UnloadTime > PreviousTime)
			{
				//
				// Decrease by random value here maybe.
				//
				Entry->UnloadTime = PreviousTime - random_number();
			}

			PreviousTime = Entry->UnloadTime;
		}

		//
		// Clear remaining entries.
		//
		clean_mmu();
	}

	ExReleaseResourceLite(ps_loaded);

	return true;
}