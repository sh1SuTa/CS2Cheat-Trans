#include "Menu.h"

static int currentTab = 1;

void ShowImguiMenu() 
{
    // Toggle menu visibility with INS key
    static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
    auto CurTimePoint = std::chrono::steady_clock::now();
    if (GetAsyncKeyState(VK_INSERT) &&
        (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(200)))
    {
        showMenu = !showMenu;
        LastTimePoint = CurTimePoint;
    }

    // Draw menu if enabled
    if (showMenu)
    {
        ImGui::Begin("Warning: Using cheats may result in server ban");

        // Menu tabs
        if (ImGui::Button("Aimbot", { 80,20 }))
        {
            currentTab = 1;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button("Visuals", { 80,20 }))
        {
            currentTab = 2;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button("Misc", { 80,20 }))
        {
            currentTab = 3;
        }

        // Aimbot settings
        if (currentTab == 1)
        {
            ImGui::TextColored(ImColor(255, 0, 0, 47), "Aimbot Settings (+)");
            ImGui::Checkbox("Enable Aimbot", &Menu::Aimbot);
            ImGui::SliderFloat("FOV", (float*)&Menu::Fov, 1.f, 180.f, "%.0f");
            ImGui::Checkbox("Draw FOV", &Menu::DrawFov);
            
            // Aim target selection
            ImGui::RadioButton("Head", &Menu::Aimplace, 6);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Chest", &Menu::Aimplace, 3);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Stomach", &Menu::Aimplace, 1);
            
            // Aim activation key
            ImGui::RadioButton("Side Button", &Menu::Aimkey, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Left Click", &Menu::Aimkey, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Shift", &Menu::Aimkey, VK_SHIFT);
        }

        // Visual settings
        if (currentTab == 2)
        {
            ImGui::TextColored(ImColor(0, 217, 255), "Visual Settings (O)");
            ImGui::Checkbox("Show Team", &Menu::Team);
            
            // ESP box style
            ImGui::RadioButton("Off", &Menu::box, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Box", &Menu::box, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Adaptive Box", &Menu::box, 2);
            
            ImGui::Checkbox("Health Bar", &Menu::Health);
            ImGui::Checkbox("Skeleton", &Menu::Skeleton);
            ImGui::Checkbox("Enable FOV (In-Game Only)", &Menu::视野角度启用);
            ImGui::SliderInt("View FOV", &Menu::视野角度, 10, 170);
        }

        // Misc settings
        if (currentTab == 3)
        {
            ImGui::TextColored(ImColor(128, 128, 128), "Misc Settings (?)");
            ImGui::RadioButton("Developer Skeleton", &Menu::绘制骨骼类型, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton("Normal Skeleton", &Menu::绘制骨骼类型, 3);
            ImGui::Checkbox("Enable Spin", &Menu::旋转启用);
        }
        ImGui::End();
    }

    // Theme colors (currently unused)
    ImVec4 transBlue = ImVec4(0.52f, 0.78f, 0.91f, 1.0f);
    ImVec4 transPink = ImVec4(0.98f, 0.65f, 0.85f, 1.0f);
    ImVec4 transWhite = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}
