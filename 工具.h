#pragma once
#include"cheat.h"
#include"工具类/Mem.h"
#include"output/offsets.hpp"
#include"output/client_dll.hpp"
#include"工具类/XorStr.h"

#include <Windows.h>
#include <stdexcept>
#include <string>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

namespace 工具 {
    // Camera service pointer
    inline char* 指向摄像机服务的指针;
    // FOV pointer
    inline char* fov;
    // Initial rotation value
    inline float 初始值 = 0.f;
    // Local player pointer
    inline char* local_player;
}

namespace 工具 {
    // Type aliases for memory addresses
    using addr32 = unsigned int;
    using addr64 = unsigned long long;
}

namespace 工具 {
    // Process manipulation class
    class Process {
        private:
            unsigned short PID;
            HANDLE process_handle;

        public:
            // Constructor with PID
            Process(unsigned short PID) : PID(PID), process_handle(OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)) {
                if (!process_handle)
                    throw std::runtime_error("Failed to open handle");
            }

            // Constructor with process name
            Process(const TCHAR* proc_name) {
                PID = Process::get_pid(proc_name);
                if (!PID)
                    throw std::runtime_error("Unable to find process ID through process name");
                process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
                if (!process_handle) {
                    throw std::runtime_error("Failed to open process");
                }
            }

            // Get process ID from name
            DWORD static get_pid(const TCHAR* proc_name) {
                PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
                auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (!handle) {
                    throw std::runtime_error("CreateToolhelp32Snapshot func error");
                }
                bool ret = Process32First(handle, &pe);
                while (ret) {
                    auto id = pe.th32ProcessID;
                    HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, id);
                    if (ph) {
                        if (_tcscmp(pe.szExeFile, proc_name) == 0) { 
                            CloseHandle(ph); 
                            return id; 
                        }
                        CloseHandle(ph);
                    }
                    ret = Process32Next(handle, &pe);
                }
                return 0;
            }

            // Get module base address by name
            LONG_PTR get_module_handle(const TCHAR* module_name) const {
                HMODULE handles[1024]{};
                DWORD size;

                EnumProcessModules(process_handle, handles, sizeof(handles), &size);

                for (size_t i = 0; i < size / sizeof(handles[0]); i++) {
                    TCHAR name[1024]{};
                    GetModuleBaseName(process_handle, handles[i], name, sizeof(name));

                    if (_tcscmp(name, module_name) == 0) { 
                        return (LONG_PTR)handles[i]; 
                    }
                }
                throw std::runtime_error("Unable to find module");
            }
            
            // Read memory of type T from address
            template< typename T, typename Ptr>
            T read(const Ptr& ptr) {
                T buffer{};
                SIZE_T t;
                ReadProcessMemory(process_handle, (void*)ptr, &buffer, sizeof(T), &t);
                if (t != sizeof(T))
                    throw std::runtime_error("Memory read error");
                return buffer;
            }

            // Write memory of type T to address
            template< typename T, typename Ptr>
            void write(const Ptr& ptr, const T& value) {
                T v = value;
                SIZE_T write_b;
                WriteProcessMemory(process_handle, (void*)ptr, &v, sizeof(v), &write_b);
                if (write_b != sizeof(v))
                    throw std::runtime_error("Memory write error");
            }

            // Cleanup process handle
            ~Process() { 
                CloseHandle(process_handle); 
            }
    };
}

// Function declarations
void 改fov角度();
void 旋转大陀螺();
void 作弊线程1();
