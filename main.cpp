#pragma once

#include "myimgui.h"
#include "cheat.h"
#include "工具类/XorStr.h"
#include "main.h"

// Flag to indicate if running on Chinese server
int isChineseServer = 0;

// Generates random window title to avoid detection
void GenerateRandomTitle() {
    constexpr int TITLE_LENGTH = 25;
    const auto VALID_CHARS = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`-=~!@#$%^&*()_+,./;'[]|{}:?甲乙丙丁戊己庚辛壬癸子丑寅卯辰巳午未申酉戌亥");
    TCHAR title[TITLE_LENGTH + 1]{};
    
    for (int i = 0; i < TITLE_LENGTH; i++) {
        title[i] += VALID_CHARS[rand() % 95];
    }
    SetConsoleTitle(title);
}

// Initialize game-related handles and addresses
void InitializeGame() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(static_cast<unsigned>(time(nullptr)));
    GenerateRandomTitle();

    // Get game window handle
    cheat::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("Counter-Strike 2"));
    if (!cheat::g_hwnd) {
        cheat::g_hwnd = FindWindowA(XorStr("SDL_app"), XorStr("反恐精英：全球攻势"));
        isChineseServer = 1;
    }

    // Log window handle
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN);
    std::cout << "HWND: " << cheat::g_hwnd << std::endl;

    // Get process ID and handle
    GetWindowThreadProcessId(cheat::g_hwnd, &cheat::g_pid);
    GetWindowThreadProcessId(cheat::g_hwnd, &游戏进程::g_pid);
    
    cheat::g_handle = OpenProcess(PROCESS_ALL_ACCESS, true, cheat::g_pid);
    游戏进程::g_handle = OpenProcess(PROCESS_ALL_ACCESS, true, cheat::g_pid);

    // Get client.dll module address
    if ((cheat::clientAddress = mem::GetModule(cheat::g_pid, L"client.dll"))) {
        游戏进程::clientAddress = mem::GetModule(cheat::g_pid, L"client.dll");
        std::cout << XorStr("Successfully got clientAddress") << std::endl;
    }

    // Log process info
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
    printf("PID: %d\n", cheat::g_pid);
    printf("clientAddress: %p\n", cheat::clientAddress);
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

int main() {
    InitializeGame();

    // Create appropriate window based on server type
    if (isChineseServer) {
        std::cout << XorStr("Chinese server window found") << std::endl;
        myimgui::CreateWindow_Violet(XorStr("SDL_app"), "反恐精英：全球攻势", &fun, 
            "C:\\Windows\\Fonts\\simkai.ttf", 18.0f, false, 1);
    } else {
        myimgui::CreateWindow_Violet(XorStr("SDL_app"), "Counter-Strike 2", &fun,
            "C:\\Windows\\Fonts\\simkai.ttf", 18.0f, false, 1);
    }
}
