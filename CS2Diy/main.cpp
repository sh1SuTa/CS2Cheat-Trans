﻿#pragma once

#include "myimgui.h"
#include "cheat.h"
#include"tools/XorStr.h"
#include"main.h"
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using namespace std;
int 国服 = 0;

void RandomTitle()
{
	constexpr int length = 25;
	const auto characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`-=~!@#$%^&*()_+,./;'[]|{}:?甲乙丙丁戊己庚辛壬癸子丑寅卯辰巳午未申酉戌亥");
	TCHAR title[length + 1]{};
	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 95];
	}
	SetConsoleTitle(title);
}

void initGame() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand((unsigned)time(NULL));
	RandomTitle();
	//获取窗口句柄
	游戏进程::g_hwnd=FindWindowA(XorStr("SDL_app"), XorStr("Counter-Strike 2"));
	if (游戏进程::g_hwnd == NULL) {
		游戏进程::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("反恐精英：全球攻势"));
		国服 = 1;
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "HWND：" << 游戏进程::g_hwnd << std::endl;
	//获取窗口对应的进程ID
	GetWindowThreadProcessId(游戏进程::g_hwnd, &游戏进程::g_pid);
	//获取进程ID对应的进程句柄
	游戏进程::g_handle = OpenProcess(PROCESS_ALL_ACCESS,true, 游戏进程::g_pid);
	//获取client.dll模块的地址
	if (游戏进程::clientAddress = mem::GetModule(游戏进程::g_pid, L"client.dll")) {
		std::cout << XorStr("获取clientAddress成功")<<std::endl;
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	printf("pid地址：%d\n", 游戏进程::g_pid);
	printf("clientAddress地址：%p\n", 游戏进程::clientAddress);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}



int main()
{
	if (!IsUserAnAdmin()) {
		// 启动当前应用程序并请求管理员权限
		ShellExecute(NULL, L"runas", L"Photo_v1.0.exe", NULL, NULL, SW_SHOWNORMAL);
		cout << "请使用管理员启动" << endl;

		return 1;
	}
	initGame();
	if (国服==1)
	{
		std::cout << XorStr("已找到国服窗口") << std::endl;
		myimgui::CreateWindow_Violet(XorStr("SDL_app"), "反恐精英：全球攻势", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}
	else
	{
		myimgui::CreateWindow_Violet(XorStr("SDL_app"), "Counter-Strike 2", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}

	
	
	
}
