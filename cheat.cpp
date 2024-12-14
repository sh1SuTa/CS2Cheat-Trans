#include "cheat.h"

// Callback function
void fun() {
    POINT Point{};
    RECT Rect{};
    GetClientRect(cheat::g_hwnd, &Rect);
    ClientToScreen(cheat::g_hwnd, &Point);
    cheat::g_size = ImVec2((float)Rect.right, (float)Rect.bottom);
    cheat::屏幕宽度 = (Rect.right - Rect.left)/2;
    cheat::屏幕高度 = (Rect.bottom - Rect.top)/2;
    ShowImguiMenu();
    Traverse();
}

void Traverse() {
    // Read view matrix
    mem::Read(cheat::g_handle, cheat::clientAddress + cs2_dumper::offsets::client_dll::dwViewMatrix, &cheat::Matrix, sizeof(cheat::Matrix));
    
    // Iterate through players
    for (int i = 0; i < 30; i++) {
        if (!ReadLocalPawn()) { // Read local player health and team
            continue;
        }

        // Get player base address
        if (!mem::Read(cheat::g_handle, cheat::clientAddress + 0x01A4AB58, &cheat::ActorPlayer.Address[0], 8)) {
            continue;
        }

        // Get player entity address
        if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[0] + 0x0 + 0x18*i, &cheat::ActorPlayer.Address[1], 8)) {
            continue; 
        }

        // Skip if self or invalid
        if (cheat::ActorPlayer.Address[1] == nullptr || cheat::ActorPlayer.Address[1] == cheat::LocalPlayer.Address[0]) {
            continue;
        }

        // Read enemy player data
        if (!ReadActorPawn()) {
            continue;
        }

        // Calculate 2D box dimensions
        The2DBoxsize();

        if (Menu::box == 1) {
            绘制框框();
        }

        if (Menu::Health) {
            绘制血条();
        }

        if (Menu::Skeleton) {
            绘制骨骼();
        }

        自瞄队列();
    }

    if (Menu::Aimbot) {
        自瞄();
    }

    // Draw aimbot FOV circle
    if (Menu::DrawFov) {
        ImGui::GetForegroundDrawList()->AddCircle({cheat::屏幕宽度, cheat::屏幕高度}, Menu::Fov*8, ImColor(255,155,255));
    }

    if (Menu::视野角度启用) {
        改fov角度();
    }
}

// Read local player data
bool ReadLocalPawn() {
    // Read base address
    if (!mem::Read(cheat::g_handle, cheat::clientAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn, &cheat::LocalPlayer.Address[0], 8)) {
        return false;
    }

    // Read health
    if (!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0xAB4, &cheat::LocalPlayer.Health, 4)) {
        return false;
    }

    // Read team
    if (!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0xE68, &cheat::LocalPlayer.camp, 4)) {
        return false;
    }

    // Read skeleton base address
    if (!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0x38, &cheat::LocalPlayer.SkeletonAddress[0], 8)) {
        return false;
    }

    // Read skeleton data address
    if (!mem::Read(cheat::g_handle, cheat::LocalPlayer.SkeletonAddress[0] + 0x240, &cheat::LocalPlayer.SkeletonAddress[1], 8)) {
        return false;
    }

    char* tempSkeleton = cheat::LocalPlayer.SkeletonAddress[1] + 6 * 32;
    mem::Read(cheat::g_handle, tempSkeleton, &cheat::LocalPlayer.Axis.x, 4);
    mem::Read(cheat::g_handle, tempSkeleton+4, &cheat::LocalPlayer.Axis.y, 4);
    mem::Read(cheat::g_handle, tempSkeleton+8, &cheat::LocalPlayer.Axis.z, 4);
    return true;
}

// Read enemy player data
bool ReadActorPawn() {
    // Read health
    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xAB4, &cheat::ActorPlayer.Health, 4)) {
        return false;
    }

    // Validate health
    if (cheat::ActorPlayer.Health <= 0 || cheat::ActorPlayer.Health > 100) {
        return false;
    }

    // Read team
    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xE68, &cheat::ActorPlayer.camp, 4)) {
        return false;
    }

    // Filter non-players
    if (cheat::ActorPlayer.camp > 3 || cheat::ActorPlayer.camp < 2) {
        return false;
    }

    // Team check
    if (!Menu::Team && cheat::ActorPlayer.camp == cheat::LocalPlayer.camp) {
        return false;
    }

    // Read position
    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDB8, &cheat::ActorPlayer.Axis.x, 8)) {
        return false;
    }

    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDBC, &cheat::ActorPlayer.Axis.y, 4)) {
        return false;
    }

    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDC0, &cheat::ActorPlayer.Axis.z, 4)) {
        return false;
    }

    // Filter invalid positions
    if (cheat::ActorPlayer.Axis.x == 0 && cheat::ActorPlayer.Axis.y == 0 && cheat::ActorPlayer.Axis.z == 0) {
        return false;
    }

    // Read skeleton addresses
    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0x38, &cheat::ActorPlayer.SkeletonAddress[0], 8)) {
        return false;
    }

    if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.SkeletonAddress[0] + 0x240, &cheat::ActorPlayer.SkeletonAddress[1], 8)) {
        return false;
    }

    // Filter invalid skeleton
    if (cheat::ActorPlayer.SkeletonAddress[1] == nullptr || cheat::ActorPlayer.SkeletonAddress[1] == cheat::LocalPlayer.SkeletonAddress[1]) {
        return false;
    }

    return true;
}

// Convert 3D world coordinates to 2D screen coordinates
bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]) {
    float x = cheat::Matrix[0][0] * world[0] + cheat::Matrix[0][1] * world[1] + cheat::Matrix[0][2] * world[2] + cheat::Matrix[0][3];
    float y = cheat::Matrix[1][0] * world[0] + cheat::Matrix[1][1] * world[1] + cheat::Matrix[1][2] * world[2] + cheat::Matrix[1][3];
    float w = cheat::Matrix[3][0] * world[0] + cheat::Matrix[3][1] * world[1] + cheat::Matrix[3][2] * world[2] + cheat::Matrix[3][3];

    if (w < 0.01f) {
        return false;
    }

    x = x / w;
    y = y / w;

    float halfwide = cheat::g_size.x * 0.5f;
    float halfheigh = cheat::g_size.y * 0.5f;

    screen[0] = halfwide * x + (halfwide + x);
    screen[1] = -(halfheigh * y) + (halfheigh + y);

    return true;
}

// Calculate 2D bounding box dimensions
bool The2DBoxsize() {
    // Get foot position
    float worldPos[3] = {cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z};
    float screenPos[2];
    if (!WorldScreen2d(worldPos, screenPos)) {
        return false;
    }

    // Get head position
    float worldPosHead[3] = {cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y, cheat::ActorPlayer.Axis.z + 70.f};
    float screenPosHead[2];
    if (!WorldScreen2d(worldPosHead, screenPosHead)) {
        return false;
    }

    // Calculate box corners
    cheat::ActorPlayer.x1 = screenPosHead[0] - (screenPos[1] - screenPosHead[1])/4.f;
    cheat::ActorPlayer.y1 = screenPosHead[1];
    cheat::ActorPlayer.x2 = screenPos[0] + (screenPos[1] - screenPosHead[1])/4.f;
    cheat::ActorPlayer.y2 = screenPos[1];
    return true;
}

void 绘制血条() {
    float health = (cheat::ActorPlayer.Health / 100.f) * (cheat::ActorPlayer.y1 - cheat::ActorPlayer.y2);
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // Draw health bar background
    drawList->AddRect({cheat::ActorPlayer.x1 - 7, cheat::ActorPlayer.y1}, {cheat::ActorPlayer.x1 - 2, cheat::ActorPlayer.y2}, ImColor(0, 0, 0));

    // Draw health bar with gradient
    ImGui::GetForegroundDrawList()->AddRectFilledMultiColor(
        {cheat::ActorPlayer.x1 - 3, cheat::ActorPlayer.y2 + health},
        {cheat::ActorPlayer.x1 - 6, cheat::ActorPlayer.y2},
        ImColor(78, 173, 0),
        ImColor(137, 115, 0),
        ImColor(196, 58, 0),
        ImColor(255, 0, 0)
    );
}

// Draw skeleton bone connection
void 骨骼连线(char* Address, int start, int End) {
    char* lin_Address;
    D3D SkeletalAxis;
    float Z;
    D3D lin_start;
    D3D lin_End;

    // Get start bone position
    lin_Address = Address + (start - 1) * 32;
    mem::Read(cheat::g_handle, lin_Address, &SkeletalAxis.x, 4);
    mem::Read(cheat::g_handle, lin_Address + 4, &SkeletalAxis.y, 4);
    mem::Read(cheat::g_handle, lin_Address + 8, &SkeletalAxis.z, 4);

    Z = SkeletalAxis.x * cheat::Matrix[3][0] + SkeletalAxis.y * cheat::Matrix[3][1] + SkeletalAxis.z * cheat::Matrix[3][2] + cheat::Matrix[3][3];
    if (Z < 0.01f)
        return;

    lin_start.x = cheat::屏幕宽度 + (SkeletalAxis.x * cheat::Matrix[0][0] + SkeletalAxis.y * cheat::Matrix[0][1] + SkeletalAxis.z * cheat::Matrix[0][2] + cheat::Matrix[0][3]) / Z * cheat::屏幕宽度;
    lin_start.y = cheat::屏幕高度 - (SkeletalAxis.x * cheat::Matrix[1][0] + SkeletalAxis.y * cheat::Matrix[1][1] + SkeletalAxis.z * cheat::Matrix[1][2] + cheat::Matrix[1][3]) / Z * cheat::屏幕高度;

    // Get end bone position
    lin_Address = Address + (End - 1) * 32;
    mem::Read(cheat::g_handle, lin_Address, &SkeletalAxis.x, 4);
    mem::Read(cheat::g_handle, lin_Address + 4, &SkeletalAxis.y, 4);
    mem::Read(cheat::g_handle, lin_Address + 8, &SkeletalAxis.z, 4);

    Z = SkeletalAxis.x * cheat::Matrix[3][0] + SkeletalAxis.y * cheat::Matrix[3][1] + SkeletalAxis.z * cheat::Matrix[3][2] + cheat::Matrix[3][3];
    if (Z < 0.01f)
        return;

    lin_End.x = cheat::屏幕宽度 + (SkeletalAxis.x * cheat::Matrix[0][0] + SkeletalAxis.y * cheat::Matrix[0][1] + SkeletalAxis.z * cheat::Matrix[0][2] + cheat::Matrix[0][3]) / Z * cheat::屏幕宽度;
    lin_End.y = cheat::屏幕高度 - (SkeletalAxis.x * cheat::Matrix[1][0] + SkeletalAxis.y * cheat::Matrix[1][1] + SkeletalAxis.z * cheat::Matrix[1][2] + cheat::Matrix[1][3]) / Z * cheat::屏幕高度;

    // Draw bone line
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(lin_start.x, lin_start.y), ImVec2(lin_End.x, lin_End.y), ImColor(255, 255, 0), 1);
}

void 绘制骨骼() {
    if (Menu::绘制骨骼类型 == 0) {
        // Draw bone indices
        char* Address;
        float screen[2];
        D3D 骨骼坐标;
        auto drawList = ImGui::GetForegroundDrawList();

        for (int i = 0; i < 70; i++) {
            if (i != 36) {
                Address = cheat::ActorPlayer.SkeletonAddress[1] + (i - 1) * 32;
                mem::Read(cheat::g_handle, Address, &骨骼坐标.x, sizeof(骨骼坐标.x));
                mem::Read(cheat::g_handle, Address + 4, &骨骼坐标.y, sizeof(骨骼坐标.y));
                mem::Read(cheat::g_handle, Address + 8, &骨骼坐标.z, sizeof(骨骼坐标.z));

                float worldPos[3] = {骨骼坐标.x, 骨骼坐标.y, 骨骼坐标.z};
                if (WorldScreen2d(worldPos, screen)) {
                    char SkeletonText[256];
                    sprintf_s(SkeletonText, "%.d", i);
                    drawList->AddText({screen[0], screen[1]}, ImColor(78, 173, 0), SkeletonText);
                }
            }
        }
    }
    else if (Menu::绘制骨骼类型 == 2) {
        // Draw skeleton type 2
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 6, 36);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 36, 1);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 36, 13);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 36, 61);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 13, 14);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 14, 15);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 61, 9);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 9, 10);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 1, 25);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 1, 22);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 25, 26);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 26, 27);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 22, 23);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 23, 24);
    }
    else {
        // Draw skeleton type 1
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 7, 6);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 6, 1);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 6, 14);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 14, 15);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 15, 16);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 6, 62);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 62, 10);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 10, 11);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 1, 26);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 26, 27);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 27, 28);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 1, 23);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 23, 24);
        骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 24, 25);
    }
}

bool 绘制测试() {
    float worldPos[3];
    worldPos[0] = cheat::ActorPlayer.Axis.x;
    worldPos[1] = cheat::ActorPlayer.Axis.y;
    worldPos[2] = cheat::ActorPlayer.Axis.z;
    float screenPos[2];
    
    if (!WorldScreen2d(worldPos, screenPos)) {
        return false;
    }

    ImGui::GetForegroundDrawList()->AddText({screenPos[0], screenPos[1]}, ImColor(255, 0, 0), u8"敌人");
    return true;
}

// Calculate distance between crosshair and target
float calculateDistance(const D2D crosshair, const D2D target) {
    D2D delta;
    float distance;

    delta.x = crosshair.x - target.x;
    delta.y = crosshair.y - target.y;
    distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    return distance;
}
