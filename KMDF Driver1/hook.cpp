#include "hook.h"
#include <wingdi.h>
GdiSelectBrush_t GdiSelectBrush = NULL;

NtGdiCreateSolidBrush_t NtGdiCreateSolidBrush = NULL;
PatBlt_t NtGdiPatBlt = NULL;
NTUserGetDC_t NTUserGetDC = NULL;
ReleaseDC_t NtUserReleaseDC = NULL;

DeleteObjectApp_t NtGdiDeleteObjectApp = NULL;
//hook����
bool nullhook::call_kernel_function(void* kernel_function_address)
{
	if (!kernel_function_address)
	{
		return false;
	}
	//PVOID *function = reinterpret_cast<PVOID*>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys","NtQueryCompositionSurfaceStatistics"));
	PVOID* function = reinterpret_cast<PVOID*>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtDxgkGetTrackedWorkloadStatistics"));

	DbgPrint("[+] NtDxgkGetTrackedWorkloadStatistics�����ĵ�ַ:%p\n", function);
	if (!function)
	{
		return false;
	}
	BYTE orig[] = { 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BYTE shell_code[] = { 0x48,0xb8 };//mov rax,xxx
	BYTE shell_code_end[] = { 0xff,0xe0 };//jmp rax
	BYTE shell_code_cmp[] = { 0x3D, 0x3412, 0x0000 };//cmp eax, 0x1234

	//�ڴ��������������
	RtlSecureZeroMemory(&orig, sizeof(orig));
	memcpy((PVOID)(ULONG_PTR)orig, &shell_code, sizeof(shell_code));
	uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_function_address);
	memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code)), &hook_address, sizeof(void*));
	memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));
	write_to_read_only_memory(function, &orig, sizeof(orig));

	GdiSelectBrush = (GdiSelectBrush_t)get_system_module_export(L"win32kfull.sys", "NtGdiSelectBrush");
	NtGdiCreateSolidBrush = (NtGdiCreateSolidBrush_t)get_system_module_export(L"win32kfull.sys", "NtGdiCreateSolidBrush");
	NtGdiPatBlt = (PatBlt_t)get_system_module_export(L"win32kfull.sys", "NtGdiPatBlt");
	NTUserGetDC = (NTUserGetDC_t)get_system_module_export(L"win32kbase.sys", "NtUserGetDC");
	NtUserReleaseDC = (ReleaseDC_t)get_system_module_export(L"win32kbase.sys", "NtUserReleaseDC");
	NtGdiDeleteObjectApp = (DeleteObjectApp_t)get_system_module_export(L"win32kbase.sys", "NtGdiDeleteObjectApp");

	DbgPrint("[+] NTUserGetDC�����ĵ�ַ:%p\n", NTUserGetDC);

	return true;
}

NTSTATUS nullhook::hoook_handler(PVOID called_parrm)
{
	NULL_MEMORY* instructions = (NULL_MEMORY*)called_parrm;
	//���ӣ�������
	if (instructions->req_base == true)
	{
		DbgPrint("[+] ����ģ�����ַ\n");
		ANSI_STRING AS;
		UNICODE_STRING ModuleName;
		//��һ�� C ����ַ�����const char*����ʼ��Ϊ ANSI_STRING �ṹ��
		RtlInitAnsiString(&AS, instructions->module_name);
		DbgPrint("[+] ����ģ������֣�%s\n", instructions->module_name);
		//�����û��ռ��ȡ�� ASCII �ַ���ת��Ϊ�ں�ģʽ��ʹ�õ� Unicode �ַ���
		RtlAnsiStringToUnicodeString(&ModuleName, &AS, true);
		//DbgPrint("[+] ����ģ������֣�%s\n", ModuleName);
		PEPROCESS process;
		//���ݽ��� ID ���Ҳ����ض�Ӧ�� PEPROCESS ���̶���
		DbgPrint("[+] ����ģ���PID��%d\n", instructions->pid);
		PsLookupProcessByProcessId((HANDLE)instructions->pid, &process);
		ULONG64 base_address64 = NULL;
		base_address64 = get_module_base_x64(process, ModuleName);

		if (base_address64 == NULL)
		{
			DbgPrint("[-] ����ģ��Ļ���ַΪ��\n");
		}
		DbgPrint("[+] ����ģ��Ļ���ַ��%llu\n", base_address64);
		instructions->base_address = base_address64;
		RtlFreeUnicodeString(&ModuleName);
	}
	else if (instructions->write == true)
	{
		//����Ƿ�����Ч�ڴ棬��ֹ����
		if (instructions->address < 0x7fffffffffff && instructions->address >0) {
			PVOID kernelBuff = ExAllocatePool(NonPagedPool, instructions->size);
			if (!memcpy(kernelBuff, instructions->buffer_address, instructions->size))
			{
				return STATUS_UNSUCCESSFUL;
			}
			PEPROCESS process;
			PsLookupProcessByProcessId((HANDLE)instructions->pid, &process);
			write_kernel_memory((HANDLE)instructions->pid, instructions->address, kernelBuff, instructions->size);
			ExFreePool(kernelBuff);
		}

	}
	//���ڴ�
	else if (instructions->read == true)
	{
		if (instructions->address < 0x7fffffffffff && instructions->address >0)
		{
			read_kernel_memory((HANDLE)instructions->pid, instructions->address, instructions->output, instructions->size);
		}
	}
	else if (instructions->draw_box == TRUE)
	{
		HDC hdc = NTUserGetDC(NULL);
		if (!hdc)
		{
			return STATUS_UNSUCCESSFUL;
		}
		HBRUSH brush = NtGdiCreateSolidBrush(RGB(instructions->r, instructions->g, instructions->b), NULL);
		if (!brush)
		{
			return STATUS_UNSUCCESSFUL;
		}
		RECT rect = { instructions->x, instructions->y, instructions->x + instructions->w, instructions->y + instructions->h };
		FrameRect(hdc, &rect, brush, instructions->t);
		NtUserReleaseDC(hdc);
		NtGdiDeleteObjectApp(brush);
		DbgPrint("[+] �������\n");

	}


	return STATUS_SUCCESS;
}

INT  nullhook::FrameRect(HDC hdc, const RECT* lprc, HBRUSH hbr, int thickness)
{
	HBRUSH old_brush = NULL;
	RECT rc = *lprc;
	if (!(old_brush = GdiSelectBrush(hdc, hbr)))
	{
		return 0;
	}

	NtGdiPatBlt(hdc, rc.left, rc.top, 1, rc.bottom - rc.top, PATCOPY);
	NtGdiPatBlt(hdc, rc.right - 1, rc.top, 1, rc.bottom - rc.top, PATCOPY);
	NtGdiPatBlt(hdc, rc.left, rc.top, rc.right - rc.left, 1, PATCOPY);
	NtGdiPatBlt(hdc, rc.left, rc.bottom - 1, rc.right - rc.left, 1, PATCOPY);


	return TRUE;
}
