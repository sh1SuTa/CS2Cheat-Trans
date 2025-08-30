#pragma once

#include"memory.h"
typedef HBRUSH(*GdiSelectBrush_t)(_In_ HDC hdc, _In_ HBRUSH hbr);
typedef BOOL(*PatBlt_t)(_In_ HDC hdc, _In_ int x, _In_ int y, _In_ int w, _In_ int h, _In_ DWORD);
typedef HDC(*NTUserGetDC_t)(HWND hwnd);
typedef HBRUSH(*NtGdiCreateSolidBrush_t)(_In_ COLORREF crColor, _In_ HBRUSH hbr);
typedef BOOL(*ReleaseDC_t)(HDC hDC);
typedef BOOL(*DeleteObjectApp_t)(HGDIOBJ hObject);

namespace nullhook {
	bool call_kernel_function(void* kernel_function_address);
	NTSTATUS hoook_handler(PVOID called_parrm);
	INT WINAPI FrameRect(HDC hdc, CONST RECT* lprc, HBRUSH hbr, int thickness);
}