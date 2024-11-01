

#include"工具.h"


void 改fov角度() {
	
	
	
	/*mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices, &工具::指向摄像机服务的指针,sizeof(工具::指向摄像机服务的指针));
	mem::Read(cheat::g_handle, 工具::指向摄像机服务的指针 + cs2_dumper::schemas::client_dll::CCSPlayerBase_CameraServices::m_iFOV, &工具::fov,sizeof(工具::fov));
	printf("fov地址：%p\n", 工具::fov);
	int a = 120;
	mem::Write(cheat::g_handle, 工具::fov, &a, sizeof(a));*/
    工具::Process cs2(TEXT("cs2.exe"));
    
    //寻找dll模块
    auto client = cs2.get_module_handle(TEXT("client.dll"));
    std::cout << "寻找dll模块完成" << std::endl;
    //addr64 8字节的无符号long long
    auto local_player = cs2.read<工具::addr64>(client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
    std::cout << "addr64 8字节的无符号long long" << std::endl;
    //指向摄像机服务的指针
    auto cam_ser = cs2.read<工具::addr64>(local_player + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_pCameraServices);
    std::cout << "指向摄像机服务的指针" << std::endl;
    //fov的地址
    auto fov_addr = cam_ser + cs2_dumper::schemas::client_dll::CCSPlayerBase_CameraServices::m_iFOV;
    std::cout << "fov的地址" << std::endl;
    
    cs2.write(fov_addr, 120);
    std::cout << "写入完成" << std::endl;
    

}
