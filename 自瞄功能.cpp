#include"cheat.h"

void 自瞄队列()
{
    // Calculate FOV based on menu setting
    //cheat::FOV = Menu::Fov*8;
    
    // Get target player's 3D coordinates with head offset
    float World[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z + 69.f };
    float screen[2];
    if (WorldScreen2d(World, screen))
    {
        // Calculate distance from crosshair to target
        cheat::ActorDistance[0] = calculateDistance({cheat::屏幕宽度,cheat::屏幕高度}, {screen[0],screen[1]});
        
        // Update closest target info
        if (cheat::ActorDistance[0] <= cheat::ActorDistance[1])
        {
            cheat::ActorDistance[1] = cheat::ActorDistance[0];
            cheat::EnterAimAddress[0] = cheat::ActorPlayer.Address[1];
            cheat::EnterAimAddress[1] = cheat::ActorPlayer.SkeletonAddress[1];
        }
    }
}

void 自瞄()
{
    // Don't aim if local player is dead
    if (cheat::LocalPlayer.Health <= 0) {
        return;
    }

    cheat::AimAddress[0] = cheat::EnterAimAddress[0];
    cheat::AimAddress[1] = cheat::EnterAimAddress[1];
    int lin_hp;
    mem::Read(cheat::g_handle, cheat::AimAddress[0] + 0x344, &lin_hp, 4);
    
    // Only aim at living targets
    if (lin_hp > 0)
    {
        cheat::FOV = Menu::Fov * 8;
        // Check if target is within FOV
        if (cheat::FOV > cheat::ActorDistance[1])
        {
            cheat::Aimmouse = Aiming(cheat::LocalPlayer.Axis, cheat::AimAddress[1]);

            // Apply aim when key is pressed
            if (GetAsyncKeyState(Menu::Aimkey))
            {
                mem::Write(cheat::clientAddress + 视角::yam, &cheat::Aimmouse.x, sizeof(cheat::Aimmouse.x));
                mem::Write(cheat::clientAddress + 视角::pitch, &cheat::Aimmouse.y, sizeof(cheat::Aimmouse.y));
            }
        }
    }
    cheat::ActorDistance[1] = FLT_MAX;
}

// Calculate aim angles based on local and target positions
D2D Aiming(D3D LocalAxis, char* AimAddress)
{
    char* Aimindex;
    D3D ActorAxis;
    D3D AimAxis;
    D2D Aimmouse;
    const float PI = 3.1415926535f;

    // Get target bone position based on aim location setting
    Aimindex = AimAddress + Menu::Aimplace * 32;
    mem::Read(cheat::g_handle, Aimindex, &ActorAxis.x, sizeof(ActorAxis.x));
    mem::Read(cheat::g_handle, Aimindex + 4, &ActorAxis.y, sizeof(ActorAxis.y));
    mem::Read(cheat::g_handle, Aimindex + 8, &ActorAxis.z, sizeof(ActorAxis.z));

    // Calculate relative position
    AimAxis.z = LocalAxis.z - ActorAxis.z;
    AimAxis.x = ActorAxis.x - LocalAxis.x;
    AimAxis.y = ActorAxis.y - LocalAxis.y;

    // Calculate yaw angle
    if (AimAxis.x != 0)
        Aimmouse.x = atan2(AimAxis.y, AimAxis.x) * 180 / PI;

    // Calculate pitch angle
    if (AimAxis.z != 0)
        Aimmouse.y = atan(AimAxis.z / sqrt(AimAxis.x * AimAxis.x + AimAxis.y * AimAxis.y)) * 180 / PI;

    Aimmouse.y = atan(AimAxis.z / sqrt(AimAxis.x * AimAxis.x + AimAxis.y * AimAxis.y)) / PI * 180;
    return Aimmouse;
}

// Find closest target from list of potential targets
cheat::ActorInfo 选择最近目标(const std::vector<cheat::ActorInfo>& targets) {
    cheat::ActorInfo bestTarget{};
    float minDistance = FLT_MAX;

    for (const auto& target : targets) {
        if (target.距离 < minDistance) {
            minDistance = target.距离;
            bestTarget = target;
        }
    }
    return bestTarget;
}
