#pragma once
#include <Windows.h>
#include"../myimgui.h"
namespace 游戏进程 {
	inline HWND g_hwnd;
	//模块地址
	inline char* clientAddress;
	//pid
	inline DWORD g_pid;
	//进程句柄
	inline HANDLE g_handle;
	inline ImVec2 g_size;
}
