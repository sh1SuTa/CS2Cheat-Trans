#pragma once

#include "myimgui.h"
#include "cheat.h"

int 国服 = 0;
void initGame() {

	cheat::g_hwnd=FindWindowA("SDL_app", "Counter-Strike 2");
	if (cheat::g_hwnd == NULL) {
		cheat::g_hwnd = FindWindowA("SDL_app", "反恐精英：全球攻势");
		国服 = 1;

	}
	std::cout << "HWND：" << cheat::g_hwnd << std::endl;

	GetWindowThreadProcessId(cheat::g_hwnd,&cheat::g_pid);
	cheat::g_handle = OpenProcess(PROCESS_ALL_ACCESS,true, cheat::g_pid);
	if (cheat::clientAddress = mem::GetModule(cheat::g_pid, L"client.dll")) {
		std::cout << "获取clientAddress成功"<<std::endl;
	}
	printf("pid地址：%d\n", cheat::g_pid);
	
	
	printf("clientAddress地址：%p\n", cheat::clientAddress);

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
