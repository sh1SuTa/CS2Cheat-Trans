#include "myimgui.h"
#include "cheat.h"


void initGame() {

	cheat::g_hwnd=FindWindowA("SDL_app", "Counter-Strike 2");


	GetWindowThreadProcessId(cheat::g_hwnd,&cheat::g_pid);
	cheat::g_handle = OpenProcess(PROCESS_ALL_ACCESS,true, cheat::g_pid);
	cheat::clientAddress = mem::GetModule(cheat::g_pid,L"client.dll");
	printf("pid地址：%d", cheat::g_pid);

}

//int a = 1;
//int b = 0;

int main()
{

	initGame();
	myimgui::CreateWindow_Violet("SDL_app", "Counter-Strike 2", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
		18.0f, false, 1);

	/*while (a <= 100) {
		
		b = a+b;
		a++;
	}
	std::cout << b;*/
	
	
}
