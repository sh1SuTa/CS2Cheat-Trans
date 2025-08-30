#include "memory.h"
#include <ntddk.h>
#include <wdm.h>

PVOID get_system_module_base(LPCSTR module_name)
{
	ULONG bytes = 0;
	//��һ�ε��� ��ȡ����Ļ�������С��NULL��ֱ�ӻ�ȡģ����Ϣ
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInfomation, NULL, bytes, &bytes);
	if (!bytes)
	{
		return NULL;
	}
	// ��ʾ������ڴ治�ᱻ��ҳȷ���ڴ�һֱפ���������ڴ��У������ڴ�������ֽ���Ϊ֮ǰ��ȡģ����Ϣ���ֽڴ�С��������ڴ��ǩΪNULL
	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x4E554C4C);
	status = ZwQuerySystemInformation(SystemModuleInfomation, modules, bytes, &bytes);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("[-] ZwQuerySystemInformation���ص�status��ΪSuccess \n");
		ExFreePoolWithTag(modules, 0x4E554C4C);
		return NULL;
	}
	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	PVOID module_base = 0, module_size = 0;




	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		//strstr����һ���ַ����Ƿ�����ڶ����ַ������Ӵ�
		//strcmp ���� 0��ʾ ���
		if (strcmp((char*)module[i].FullPathName, module_name) == 0) {
			DbgPrint("[+] ģ������%s\n", module[i].FullPathName);
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}
	if (modules)
	{

		ExFreePoolWithTag(modules, NULL);
	}if (module_base <= NULL)
	{
		DbgPrint("[-] ģ��module_base<=NULL \n");
		return NULL;
	}
	return module_base;
}
//��ȡ����������ַ
PVOID get_system_module_export(const char* module_name, LPCSTR routine_name)
{

	PVOID lpModule = get_system_module_base(module_name);
	if (!lpModule)
	{
		return NULL;
	}
	return RtlFindExportedRoutineByName(lpModule, routine_name);
}
PVOID get_system_routine_address(LPCWSTR routine_name) {
	UNICODE_STRING name;
	RtlInitUnicodeString(&name, routine_name);
	return MmGetSystemRoutineAddress(&name);
}
PVOID get_system_module_export(LPCWSTR module_name, LPCSTR routine_name) {
	PLIST_ENTRY modules_list = reinterpret_cast<PLIST_ENTRY>(get_system_routine_address(L"PsLoadedModuleList"));

	if (!modules_list) {
		DbgPrint("[-] PsLoadedModuleList��ȡʧ��\n");
		return NULL;
	}
	for (PLIST_ENTRY link = modules_list; link != modules_list->Blink; link = link->Flink)
	{
		LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);
		UNICODE_STRING name;
		RtlUnicodeStringInit(&name, module_name);
		if (RtlEqualUnicodeString(&name, &entry->BaseDllName, TRUE))
		{
			return (entry->DllBase) ? RtlFindExportedRoutineByName(entry->DllBase, routine_name) : NULL;
		}
	}


}
bool write_memory(void* address, void* buffer, size_t size)
{
	if (RtlCopyMemory(address, buffer, size))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool write_to_read_only_memory(void* Ŀ��λ��, void* д�������, size_t д���С)
{
	//�����ڴ���������MDL��
	PMDL Mdl = IoAllocateMdl(Ŀ��λ��, д���С, FALSE, FALSE, NULL);
	if (!Mdl)
	{
		return false;
	}// �����ڴ�ҳ��
	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	//���������ڴ�ӳ�䵽ϵͳ�ռ�
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	// �����ڴ�Ȩ��Ϊ�ɶ�д
	MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);
	write_memory(Mapping, д�������, д���С);
	//�ͷ�
	MmUnmapLockedPages(Mapping, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);
	return true;
}

ULONG64 get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name)
{
	PPEB pPeb = PsGetProcessPeb(proc);
	if (!pPeb)
	{
		return NULL;
	}

	KAPC_STATE state;
	// ����ǰ�̵߳��������л���Ŀ�����������
	KeStackAttachProcess(proc, &state);

	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;
	if (!pLdr)
	{ // ����޷���ȡ���������ݽṹ��������̵Ķ�ջ����
		KeUnstackDetachProcess(&state);
		return NULL;
	}

	// ��������ģ���б�
	for (PLIST_ENTRY list = (PLIST_ENTRY)pLdr->ModuleListLoadOrder.Flink;
		list != &pLdr->ModuleListLoadOrder;
		list = (PLIST_ENTRY)list->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);

		// �Ƚ�ģ�������Ƿ�ƥ��
		if (RtlCompareUnicodeString(&pEntry->BaseDllName, &module_name, TRUE) == 0)
		{
			ULONG64 baseAddress = (ULONG64)pEntry->DllBase;

			// ��ȡ�����ַ
			//PHYSICAL_ADDRESS physicalAddress = MmGetPhysicalAddress((PVOID)baseAddress);
			//ULONG64 physicalBaseAddress = (ULONG64)physicalAddress.QuadPart;

			// �ָ���ǰ�߳�������
			KeUnstackDetachProcess(&state);
			return baseAddress;  // ���������ַ
		}
	}

	KeUnstackDetachProcess(&state);
	return NULL;
}

bool read_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size)
{
	SIZE_T bytes = 0;
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS process;
	PsLookupProcessByProcessId((HANDLE)pid, &process);
	status = MmCopyVirtualMemory(process, (void*)address, (PEPROCESS)PsGetCurrentProcess(), buffer, size, KernelMode, &bytes);
	if (!NT_SUCCESS(status))
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool write_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size)
{
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS process;
	PsLookupProcessByProcessId((HANDLE)pid, &process);
	KAPC_STATE state;
	KeStackAttachProcess(process, &state);
	MEMORY_BASIC_INFORMATION info;
	status = ZwQueryVirtualMemory(ZwCurrentProcess(), (PVOID)address, MemoryBasicInformation, &info, sizeof(info), NULL);
	if (!NT_SUCCESS(status))
	{
		KeUnstackDetachProcess(&state);
		return false;
	}
	if (((uintptr_t)info.BaseAddress + info.RegionSize) < (address + size))
	{
		KeUnstackDetachProcess(&state); return false;
	}
	if (!(info.State & MEM_COMMIT) || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
	{
		KeUnstackDetachProcess(&state); return false;
	}if ((info.Protect & PAGE_EXECUTE_READWRITE) || (info.Protect & PAGE_EXECUTE_WRITECOPY)
		|| (info.Protect & PAGE_READWRITE) || (info.Protect & PAGE_WRITECOPY))
	{
		RtlCopyMemory((void*)address, buffer, size);
	}
	KeUnstackDetachProcess(&state); return true;
}
