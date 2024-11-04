#include"cheat.h"

//进入自瞄队列

void 自瞄队列()
{
	//cheat::FOV = Menu::Fov*8 ;
	//获取对象玩家的三维坐标
	float World[3] = { cheat::ActorPlayer.Axis.x,cheat::ActorPlayer.Axis.y,cheat::ActorPlayer.Axis.z + 69.f };
	float screen[2];
	if (WorldScreen2d(World, screen))
	{
		//计算准星到目标的距离
		cheat::ActorDistance = calculateDistance({ cheat::屏幕宽度,cheat::屏幕高度 }, { screen[0],screen[1] });


		cheat::EnterAimAddress[0] = cheat::ActorPlayer.Address[1];
		//对象骨骼地址给EnterAimAddress[1]
		cheat::EnterAimAddress[1] = cheat::ActorPlayer.SkeletonAddress[1];

	}
}

void 自瞄()
{
	if (cheat::LocalPlayer.Health <= 0) {
		return; // 本地玩家死亡时不进行瞄准
	}
	float 最小距离 = FLT_MAX; // 初始化最小距离

	cheat::AimAddress[0] = cheat::EnterAimAddress[0];
	cheat::AimAddress[1] = cheat::EnterAimAddress[1];
	int lin_hp;
	mem::Read(cheat::g_handle, cheat::AimAddress[0] + 0x344, &lin_hp, 4);

	// 判断对方血量是否 > 0
	if (lin_hp > 0)
	{
		cheat::FOV = Menu::Fov * 8;
		// 判断是否在 FOV 范围内
		if (cheat::FOV > cheat::ActorDistance)
		{
			cheat::Aimmouse = Aiming(cheat::LocalPlayer.Axis, cheat::AimAddress[1]);

			if (GetAsyncKeyState(Menu::Aimkey))
			{
				mem::Write(cheat::g_handle, cheat::clientAddress + 0x1A3DCC4, &cheat::Aimmouse.x, sizeof(cheat::Aimmouse.x));
				mem::Write(cheat::g_handle, cheat::clientAddress + 0x1A3DCC0, &cheat::Aimmouse.y, sizeof(cheat::Aimmouse.y));
			}
		}
	}
}
//自瞄算法
D2D Aiming(D3D LocalAxis, char* AimAddress)
{
	char* Aimindex;
	D3D ActorAxis;
	D3D AimAxis;
	D2D Aimmouse;
	float P_I = 3.1415926535f;

	//7为目标头部，-1则为6
	//Aimindex = AimAddress + (Menu::Aimplace-1)  * 32;
	Aimindex = AimAddress + Menu::Aimplace * 32;


	mem::Read(cheat::g_handle, Aimindex, &ActorAxis.x, sizeof(ActorAxis.x));
	mem::Read(cheat::g_handle, Aimindex + 4, &ActorAxis.y, sizeof(ActorAxis.y));
	mem::Read(cheat::g_handle, Aimindex + 8, &ActorAxis.z, sizeof(ActorAxis.z));

	/*AimAxis.x = LocalAxis.x - ActorAxis.x;
	AimAxis.y = LocalAxis.y - ActorAxis.y;*/
	AimAxis.z = LocalAxis.z - ActorAxis.z;
	AimAxis.x = ActorAxis.x - LocalAxis.x;
	AimAxis.y = ActorAxis.y - LocalAxis.y;



	//第一象限
	//if (AimAxis.x >= 0 && AimAxis.y >= 0)
		//Aimmouse.x = atan(AimAxis.y / AimAxis.x) / P_I * 180 + 180;
	//第二象限
	//if (AimAxis.x <= 0 && AimAxis.y >= 0)
		//Aimmouse.x = atan(AimAxis.y / AimAxis.x) / P_I * 180;
	//第三象限
	//if (AimAxis.x <= 0 && AimAxis.y <= 0)
		//Aimmouse.x = atan(AimAxis.y / AimAxis.x) / P_I * 180;
	//第四象限
	//if (AimAxis.x >= 0 && AimAxis.y <= 0)
		//Aimmouse.x = atan(AimAxis.y / AimAxis.x) / P_I * 180 - 180;
	if (AimAxis.x != 0)
		Aimmouse.x = atan2(AimAxis.y, AimAxis.x) * 180 / P_I; // 使用 atan2 处理所有象限

	if (AimAxis.z != 0)
		Aimmouse.y = atan(AimAxis.z / sqrt(AimAxis.x * AimAxis.x + AimAxis.y * AimAxis.y)) * 180 / P_I;


	Aimmouse.y = atan(AimAxis.z / sqrt(AimAxis.x * AimAxis.x + AimAxis.y * AimAxis.y)) / P_I * 180;
	return Aimmouse;

}
