#include "Menu.h"




static int Buttonthis = 1;


void ShowImguiMenu()
{
    //显示\隐藏菜单 INS键
    static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
    auto CurTimePoint = std::chrono::steady_clock::now();
    if (GetAsyncKeyState(VK_INSERT) &&
        (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(200)))
    {
        showMenu = !showMenu;
        LastTimePoint = CurTimePoint;
    }


    //绘制菜单
    if (showMenu)
    {
        ImGui::Begin(u8"开挂是违法的");

        if (ImGui::Button(u8"自瞄", { 80,20 }))
        {
            Buttonthis = 1;

        }
        ImGui::SameLine(NULL, 15.f);  //不换行
        if (ImGui::Button(u8"视觉", { 80,20 }))
        {
            Buttonthis = 2;

        }
        ImGui::SameLine(NULL, 15.f);   //不换行
        if (ImGui::Button(u8"杂项", { 80,20 }))
        {
            Buttonthis = 3;
        }
        //绘制菜单
        if (Buttonthis == 1)
        {
            ImGui::TextColored(ImColor(128, 128, 128), u8"自瞄机器人(?)");
            ImGui::Checkbox(u8"自瞄机器人", &Menu::Aimbot);
            ImGui::SliderFloat(u8"FOV", (float*)&Menu::Fov, 1.f, 180.f, "%.0f");
            ImGui::Checkbox(u8"绘制FOV", &Menu::DrawFov);
            ImGui::RadioButton(u8"头", &Menu::Aimplace, 7);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"胸", &Menu::Aimplace, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"肚", &Menu::Aimplace, 2);
            ImGui::RadioButton(u8"M4", &Menu::Aimkey, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"左键", &Menu::Aimkey, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Shift", &Menu::Aimkey, VK_SHIFT);
        }
        if (Buttonthis == 2)
        {
            ImGui::TextColored(ImColor(128, 128, 128), u8"视觉(?)");
            ImGui::Checkbox(u8"队伍", &Menu::Team);
            ImGui::Checkbox(u8"方框", &Menu::box);
            ImGui::Checkbox(u8"自适应方框", &Menu::Adaptivebox);
            ImGui::Checkbox(u8"血量", &Menu::Health);
            ImGui::Checkbox(u8"骨骼", &Menu::Skeleton);
        }
        if (Buttonthis == 3)
        {
            ImGui::TextColored(ImColor(128, 128, 128), u8"杂项(?)");
            ImGui::RadioButton(u8"开发者骨骼", &Menu::绘制骨骼类型,0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"正常骨骼", &Menu::绘制骨骼类型, 3);

        }
        ImGui::End();
    }

}
