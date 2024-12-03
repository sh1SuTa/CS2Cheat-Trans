#pragma once
#include"工具类/Mem.h"
#include"myimgui.h"
#include "Menu.h"
#include"工具.h"
#include"output/offsets.hpp"
#include"自瞄功能.h"
#include"透视.h"
#include"视角.h"

#include<cmath>
#include <iostream>
#include<vector>



struct D2D
{
	float x, y;
};
struct D3D
{
	float x, y, z;
};


//取准星距离
float calculateDistance(const D2D 准星, const D2D 对象);


namespace cheat {

	inline char* EnterAimAddress[2];
	inline float ActorDistance[2] = { 99999.f,999999.f};


	inline float 屏幕宽度;
	inline float 屏幕高度;

	inline HWND g_hwnd;
	//模块地址
	inline char* clientAddress;
	//pid
	inline DWORD g_pid;
	//进程句柄
	inline HANDLE g_handle;
	inline ImVec2 g_size;

	inline float Matrix[4][4]; //矩阵
	inline char* AimAddress[2];
	inline float FOV;//自瞄范围
	inline D2D Aimmouse;
	

	struct ActorInfo
	{
		char* Address[2]; 	//基础地址
		char* SkeletonAddress[2]; //骨骼地址
		int Health; //血量
		int camp;	 //阵营
		D3D Axis; //坐标
		float x1;
		float y1;
		float x2;
		float y2;
		float 距离;
	};
	inline ActorInfo LocalPlayer; //本地
	inline ActorInfo ActorPlayer; //对象玩家
	
	inline std::vector<ActorInfo> targets; // 存储当前可以瞄准的目标
	
}

void fun();
void Traverse();
//读取本地玩家Pawn
bool ReadLocalPawn();
//读取对象玩家Pawn
bool ReadActorPawn();
//三维转二维
bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]);

bool 绘制测试();
void 绘制血条();
void 绘制骨骼();
void 骨骼连线(char* Address, int start, int End);
bool The2DBoxsize();
void 自瞄();
D2D Aiming(D3D LocalAxis, char* AimAddress);
