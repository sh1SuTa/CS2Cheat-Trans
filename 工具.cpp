

#include "工具.h"


void 改fov角度() {
    // Initialize CS2 process handle
    工具::Process cs2(TEXT("cs2.exe"));
    
    // Get client.dll module base address
    auto client = cs2.get_module_handle(TEXT("client.dll"));
    std::cout << XorStr("寻找dll模块完成：") << client << std::endl;
    
    // Get local player pointer (64-bit address)
    auto local_player = cs2.read<工具::addr64>(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
    
    // Get camera services pointer
    auto cam_ser = cs2.read<工具::addr64>(local_player + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices);
    
    // Calculate FOV address and write new value
    auto fov_addr = cam_ser + cs2_dumper::schemas::client_dll::CCSPlayerBase_CameraServices::m_iFOV;
    mem::Write((CHAR*)fov_addr, &Menu::视野角度, sizeof(Menu::视野角度));
}

void 改fov角度2() {
    // Alternative FOV calculation using client address
    cheat::clientAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
}

void 旋转大陀螺() {
    // Spin bot implementation
    float rotation_speed = 1.f;
    mem::Write(cheat::clientAddress + 视角::yam, &工具::初始值, sizeof(工具::初始值));
    工具::初始值 = 工具::初始值 + rotation_speed;
    
    // Reset angle when reaching 180 degrees
    if (工具::初始值 == 180) {
        工具::初始值 = -180;
    }
}

void 作弊线程1() {
    // Main cheat thread loop
    while (true) {
        if (Menu::旋转启用) {
            旋转大陀螺();
        }
    }
}
