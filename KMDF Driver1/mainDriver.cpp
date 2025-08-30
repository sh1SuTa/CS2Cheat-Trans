#include"hook.h"
extern "C" NTSTATUS DriverEntry() {
	DbgPrint("[+] DriverEntry Called\n");
	nullhook::call_kernel_function(&nullhook::hoook_handler);
	return STATUS_SUCCESS;

}