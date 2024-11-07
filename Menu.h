#pragma once

#include"myimgui.h"
#include <chrono>
#pragma once


inline bool showMenu = true;
void ShowImguiMenu();


namespace Menu
{
	//自瞄区
	inline bool Aimbot=true;     //自瞄
	inline float Fov=22.f;       //自瞄范围
	inline bool DrawFov=true;    //绘制自瞄范围
	inline int Aimplace=6;    //自瞄部位
	inline int Aimkey= VK_SHIFT;      //自瞄热键
	//视觉区
	inline bool Team;       //是否绘制队友
	inline int box=1;        //方框
	inline bool Adaptivebox;//自适应方框
	inline bool Health;		//血量
	inline bool Skeleton = false;	//骨骼
	inline int 绘制骨骼类型 = 3;// 0：开发骨骼，3：正常骨骼
	inline bool 视野角度启用 = false;
	inline int 视野角度 = 90;
	inline bool 旋转启用;

}
