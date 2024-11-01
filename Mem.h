#pragma once
#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
WINUSERAPI SHORT WINAPI GetAsyncKeyState(int vKey);

namespace mem
{
    inline char* GetModule(DWORD PID, const wchar_t* 妯″潡鍚嶇О)
    {
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);
        if (handle == INVALID_HANDLE_VALUE)
        {
            return nullptr;
        }

        MODULEENTRY32 module_info;
        ZeroMemory(&module_info, sizeof(module_info));
        module_info.dwSize = sizeof(module_info);

        bool ret = Module32First(handle, &module_info);
        if (!ret)
        {
            return nullptr;
        }

        do
        {
            //鍒ゆ柇妯″潡鍚嶆槸鍚︾浉绛?            if (wcscmp(妯″潡鍚嶇О, module_info.szModule) == 0)
            {
                //濡傛灉鎵惧埌浜嗗氨鎵撳嵃妯″潡鐨勫湴鍧€ 骞惰繑鍥炶妯″潡鐨勫湴鍧€
                //printf("%ws %llx\n", module_info.szModule, module_info.modBaseAddr);
                CloseHandle(handle);
                return (char*)(module_info.modBaseAddr);
            }
        } while (Module32Next(handle, &module_info));

        CloseHandle(handle);
        return nullptr;
    }

    inline bool Read(HANDLE 杩涚▼鍙ユ焺, char* 鍐呭瓨鍦板潃, void* 瀛樻斁鎸囬拡, int 娆茶鍙栧瓧鑺?
    {
        SIZE_T 缂撳啿鍖?
        if (ReadProcessMemory(杩涚▼鍙ユ焺, 鍐呭瓨鍦板潃, 瀛樻斁鎸囬拡, 娆茶鍙栧瓧鑺? &缂撳啿鍖?)
            return true;
        return false;
    }
    inline bool Write(HANDLE 杩涚▼鍙ユ焺, char* 鍐呭瓨鍦板潃, void* 鍐欏叆鏁版嵁, int 娆插啓鍏ュ瓧鑺?
    {
        SIZE_T 缂撳啿鍖?
        if (WriteProcessMemory(杩涚▼鍙ユ焺, 鍐呭瓨鍦板潃, 鍐欏叆鏁版嵁, 娆插啓鍏ュ瓧鑺? &缂撳啿鍖?)
            return true;
        return false;
    }
}
