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
        ImGui::Begin(u8"开挂会被服务器ben掉的。");

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
            ImGui::TextColored(ImColor(255, 0, 0,47), u8"自瞄机器人(+)");
            ImGui::Checkbox(u8"自瞄机器人", &Menu::Aimbot);
            ImGui::SliderFloat(u8"FOV", (float*)&Menu::Fov, 1.f, 180.f, "%.0f");
            ImGui::Checkbox(u8"绘制FOV", &Menu::DrawFov);
            ImGui::RadioButton(u8"头", &Menu::Aimplace, 6);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"胸", &Menu::Aimplace, 3);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"肚", &Menu::Aimplace, 1);
            ImGui::RadioButton(u8"侧键", &Menu::Aimkey, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"左键", &Menu::Aimkey, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Shift", &Menu::Aimkey, VK_SHIFT);
        }
        if (Buttonthis == 2)
        {
            ImGui::TextColored(ImColor(0, 217, 255), u8"视觉(O)");
            ImGui::Checkbox(u8"队伍", &Menu::Team);
            ImGui::RadioButton(u8"关闭", &Menu::box, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"方框", &Menu::box, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"自适应方框", &Menu::box, 2);
            ImGui::Checkbox(u8"血量", &Menu::Health);
            ImGui::Checkbox(u8"骨骼", &Menu::Skeleton);
            ImGui::Checkbox(u8"启用FOV(请务必在对局中)", &Menu::视野角度启用);
            ImGui::SliderInt(u8"视野FOV角度", &Menu::视野角度, 10, 170);
        }
        if (Buttonthis == 3)
        {
            ImGui::TextColored(ImColor(128, 128, 128), u8"杂项(?)");
            ImGui::RadioButton(u8"开发者骨骼", &Menu::绘制骨骼类型,0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"正常骨骼", &Menu::绘制骨骼类型, 3);
            ImGui::Checkbox(u8"让自己转起来", &Menu::旋转启用);

        }
        ImGui::End();
    }
    


    // 蓝粉白主题颜色
    ImVec4 transBlue = ImVec4(0.52f, 0.78f, 0.91f, 1.0f);
    ImVec4 transPink = ImVec4(0.98f, 0.65f, 0.85f, 1.0f); 
    ImVec4 transWhite = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 

    // 绘制菜单
    /*
    if (showMenu)
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, transWhite); // 背景设为白色
        ImGui::PushStyleColor(ImGuiCol_Text, transPink);      // 文字设为浅蓝色
        ImGui::Begin(u8"开挂会被服务器ben掉的，知道吗。");

        if (ImGui::Button(u8"自瞄", { 80,20 }))
        {
            Buttonthis = 1;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button(u8"视觉", { 80,20 }))
        {
            Buttonthis = 2;
        }
        ImGui::SameLine(NULL, 15.f);
        if (ImGui::Button(u8"杂项", { 80,20 }))
        {
            Buttonthis = 3;
        }

        // 绘制自瞄选项
        if (Buttonthis == 1)
        {
            ImGui::TextColored(transPink, u8"自瞄机器人(?)");
            ImGui::Checkbox(u8"自瞄机器人", &Menu::Aimbot);
            ImGui::SliderFloat(u8"FOV", (float*)&Menu::Fov, 1.f, 180.f, "%.0f");
            ImGui::Checkbox(u8"绘制FOV", &Menu::DrawFov);
            ImGui::RadioButton(u8"头", &Menu::Aimplace, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"胸", &Menu::Aimplace, 3);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"肚", &Menu::Aimplace, 1);
            ImGui::RadioButton(u8"M4", &Menu::Aimkey, 5);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"左键", &Menu::Aimkey, 1);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"Shift", &Menu::Aimkey, VK_SHIFT);
        }

        // 绘制视觉选项
        if (Buttonthis == 2)
        {
            ImGui::TextColored(transPink, u8"视觉(?)");
            ImGui::Checkbox(u8"队伍", &Menu::Team);
            ImGui::Checkbox(u8"方框", &Menu::box);
            ImGui::Checkbox(u8"自适应方框", &Menu::Adaptivebox);
            ImGui::Checkbox(u8"血量", &Menu::Health);
            ImGui::Checkbox(u8"骨骼", &Menu::Skeleton);
        }

        // 绘制杂项选项
        if (Buttonthis == 3)
        {
            ImGui::TextColored(transPink, u8"杂项(?)");
            ImGui::RadioButton(u8"开发者骨骼", &Menu::绘制骨骼类型, 0);
            ImGui::SameLine(NULL, 15.f);
            ImGui::RadioButton(u8"正常骨骼", &Menu::绘制骨骼类型, 3);
        }

        ImGui::End();
        ImGui::PopStyleColor(2); // 恢复原始颜色
    }*/


}
