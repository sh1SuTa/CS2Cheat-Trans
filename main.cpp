﻿#pragma once

#include "myimgui.h"
#include "cheat.h"
#include"工具类/XorStr.h"

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

	cheat::g_hwnd=FindWindowA(XorStr("SDL_app"), XorStr("Counter-Strike 2"));
	if (cheat::g_hwnd == NULL) {
		cheat::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("反恐精英：全球攻势"));
		国服 = 1;

	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "HWND：" << cheat::g_hwnd << std::endl;

	GetWindowThreadProcessId(cheat::g_hwnd,&cheat::g_pid);
	cheat::g_handle = OpenProcess(PROCESS_ALL_ACCESS,true, cheat::g_pid);
	if (cheat::clientAddress = mem::GetModule(cheat::g_pid, L"client.dll")) {
		std::cout << XorStr("获取clientAddress成功")<<std::endl;
	}
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	printf("pid地址：%d\n", cheat::g_pid);
	printf("clientAddress地址：%p\n", cheat::clientAddress);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

}



int main()
{
	
	initGame();
	if (国服==1)
	{
		myimgui::CreateWindow_Violet("SDL_app", "反恐精英：全球攻势", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}
	else
	{
		myimgui::CreateWindow_Violet("SDL_app", "Counter-Strike 2", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
			18.0f, false, 1);
	}

	
	
	
}
