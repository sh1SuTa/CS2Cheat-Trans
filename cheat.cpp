#include "cheat.h"



//回调
void fun() {
	POINT Point{};
	RECT Rect{};
	GetClientRect(cheat::g_hwnd, &Rect);
	ClientToScreen(cheat::g_hwnd, &Point);
	cheat::g_size = ImVec2((float)Rect.right,(float) Rect.bottom);
	cheat::屏幕宽度 = (Rect.right - Rect.left)/2;
	cheat::屏幕高度 =  (Rect.bottom - Rect.top)/2;
	ShowImguiMenu();
	Traverse();
}

bool 绘制测试();


void Traverse() {
	//本地矩阵
	mem::Read(cheat::g_handle, cheat::clientAddress + cs2_dumper::offsets::client_dll::dwViewMatrix, &cheat::Matrix, sizeof(cheat::Matrix));
	
	//遍历对象玩家地址
	for (int i = 0; i < 30; i++)
	{
		if ( !ReadLocalPawn()) {//包含了血量，队伍
			continue;
		}
		//对象玩家地址[0]获取处
		if (!mem::Read(cheat::g_handle, cheat::clientAddress + 0x01A36118, &cheat::ActorPlayer.Address[0],8)) {
		
			continue;
		}
		//对象玩家地址[1]获取处
		if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[0] + 0x0+ 0x18*i , &cheat::ActorPlayer.Address[1], 8)) {
			continue;
		}
		if (cheat::ActorPlayer.Address[1] == nullptr || cheat::ActorPlayer.Address[1] == cheat::LocalPlayer.Address[0])
		{//判断是不是自己
			continue;
		}
		if ( !ReadActorPawn())//读取其他人坐标，每一次for循环就是一个实体
		{
			continue;
		}
		
		//计算2D方框大小
		The2DBoxsize();
		if (Menu::box == 1) {
			绘制框框();
		}
		
		
		if (Menu::Health)
		{
			绘制血条();
		}
		if (Menu::Skeleton)
		{
			绘制骨骼();
		}
		自瞄队列();
	}

	if (Menu::Aimbot)
	{
		自瞄();
	}
	//绘制自瞄范围
	if (Menu::DrawFov)
	{
		ImGui::GetForegroundDrawList()->AddCircle({ cheat::屏幕宽度,cheat::屏幕高度 },Menu::Fov*8,ImColor(255,155,255));
	}
	if (Menu::视野角度启用)
	{
		改fov角度();
	}
}


//读取本地玩家
bool ReadLocalPawn() {
	//基础地址
	if ( 
		! mem::Read(cheat::g_handle, cheat::clientAddress + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn, &cheat::LocalPlayer.Address[0], 8)
	) {
		return false;
	}
	//血量
	if (
		!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0xAB4, &cheat::LocalPlayer.Health, 4)
	){
		return false;
	}
	//阵营
	if (
		!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0xE68, &cheat::LocalPlayer.camp, 4)
		) {
		return false;
	}
	//骨骼地址 基础地址+38 -> +240 
	if (
		!mem::Read(cheat::g_handle, cheat::LocalPlayer.Address[0] + 0x38, &cheat::LocalPlayer.SkeletonAddress[0], 8)
		) {
		return false;
	}
	if (
		!mem::Read(cheat::g_handle, cheat::LocalPlayer.SkeletonAddress[0] + 0x240+0x00, &cheat::LocalPlayer.SkeletonAddress[1], 8)
		) {
		return false;
	}
	char* 临时骨骼;
	临时骨骼 = cheat::LocalPlayer.SkeletonAddress[1] + 6 * 32;
	mem::Read(cheat::g_handle, 临时骨骼, &cheat::LocalPlayer.Axis.x, 4);
	mem::Read(cheat::g_handle, 临时骨骼+4, &cheat::LocalPlayer.Axis.y, 4);
	mem::Read(cheat::g_handle, 临时骨骼+8, &cheat::LocalPlayer.Axis.z, 4);
	return true;
}

//读取对象玩家Pawn
bool ReadActorPawn() {
	//血量
	if (!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xAB4, &cheat::ActorPlayer.Health, 4)) {
		return false;
	}
	if (cheat::ActorPlayer.Health <= 0 || cheat::ActorPlayer.Health>100)
	{
		return false;
	}
	//阵营
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xE68, &cheat::ActorPlayer.camp, 4)
		) {
		return false;
	}
	//过滤非人
	if (cheat::ActorPlayer.camp > 3 || cheat::ActorPlayer.camp < 2 ) {
		return false;
	}
	if (Menu::Team == false) {
		if (cheat::ActorPlayer.camp == cheat::LocalPlayer.camp) {
			return false;
		}
	}
	
	//X
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDB8, &cheat::ActorPlayer.Axis.x, 8)
		) {
		return false;
	}
	//std::cout << "X坐标：" << cheat::ActorPlayer.Axis.x << std::endl;
	//Y
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDBC, &cheat::ActorPlayer.Axis.y, 4)
		) {
		return false;
	}
	//Z
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0xDC0, &cheat::ActorPlayer.Axis.z, 4)
		) {
		return false;
	}
	//过滤坐标
	if (cheat::ActorPlayer.Axis.x==0&& cheat::ActorPlayer.Axis.y==0&& cheat::ActorPlayer.Axis.z==0)
	{
		return false;
	}
	//骨骼地址 基础地址+38 -> +240 
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.Address[1] + 0x38, &cheat::ActorPlayer.SkeletonAddress[0], 8)
		) {
		return false;
	}
	if (
		!mem::Read(cheat::g_handle, cheat::ActorPlayer.SkeletonAddress[0] + 0x240 + 0x00, &cheat::ActorPlayer.SkeletonAddress[1], 8)
		) {
		return false;
	}
	//过滤骨骼
	if (cheat::ActorPlayer.SkeletonAddress[1]==nullptr|| cheat::ActorPlayer.SkeletonAddress[1]==cheat::LocalPlayer.SkeletonAddress[1])
	{
		return false;
	}
	//printf("Skeleton地址：%p \n", cheat::LocalPlayer.SkeletonAddress[1]);
	return true;

}

//三维转二维
bool WorldScreen2d(_In_ float world[3],_Out_ float screen[2]) {

	float x = cheat::Matrix[0][0] * world[0] + cheat::Matrix[0][1] * world[1] + cheat::Matrix[0][2] * world[2] + cheat::Matrix[0][3] * 1;
	float y = cheat::Matrix[1][0] * world[0] + cheat::Matrix[1][1] * world[1] + cheat::Matrix[1][2] * world[2] + cheat::Matrix[1][3] * 1;

	float w = cheat::Matrix[3][0] * world[0] + cheat::Matrix[3][1] * world[1] + cheat::Matrix[3][2] * world[2] + cheat::Matrix[3][3] * 1;

	if (w<0.01f)
	{
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

//计算2D方框大小
bool The2DBoxsize() {
	//脚部
	float 世界坐标[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float 屏幕坐标[2];
	if (!WorldScreen2d(世界坐标, 屏幕坐标))
	{
		return false;
	}
	//头部
	float 世界坐标2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z+70.f };
	float 屏幕坐标2[2];
	if (!WorldScreen2d(世界坐标2, 屏幕坐标2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = 屏幕坐标2[0] - (屏幕坐标[1]- 屏幕坐标2[1])/4.f;
	cheat::ActorPlayer.y1 = 屏幕坐标2[1];
	cheat::ActorPlayer.x2 = 屏幕坐标[0]+(屏幕坐标[1] - 屏幕坐标2[1])/4.f;
	cheat::ActorPlayer.y2 = 屏幕坐标[1];
	return true;
}



void 绘制血条() {

	float health = (cheat::ActorPlayer.Health / 100.f)* (cheat::ActorPlayer.y1- cheat::ActorPlayer.y2);
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	drawList->AddRect({ cheat::ActorPlayer.x1 - 7,cheat::ActorPlayer.y1 }, { cheat::ActorPlayer.x1 - 2,cheat::ActorPlayer.y2 }, ImColor(0, 0, 0));
	
	ImGui::GetForegroundDrawList()->AddRectFilledMultiColor(
		{ cheat::ActorPlayer.x1 - 3,cheat::ActorPlayer.y2+ health }, 
		{ cheat::ActorPlayer.x1 - 6,cheat::ActorPlayer.y2  }, ImColor(78, 173, 0), ImColor(137, 115, 0),ImColor(196, 58, 0), ImColor(255, 0, 0)
	);
}

void 骨骼连线(char* Address, int start, int End)
{

	char* lin_Address;
	D3D SkeletalAxis;
	float Z;
	D3D lin_start;
	D3D lin_End;

	lin_Address = Address + (start - 1) * 32;
	mem::Read(cheat::g_handle, lin_Address, &SkeletalAxis.x, 4);
	mem::Read(cheat::g_handle, lin_Address + 4, &SkeletalAxis.y, 4);
	mem::Read(cheat::g_handle, lin_Address + 8, &SkeletalAxis.z, 4);
	Z = SkeletalAxis.x * cheat::Matrix[3][0] + SkeletalAxis.y * cheat::Matrix[3][1] + SkeletalAxis.z * cheat::Matrix[3][2] + cheat::Matrix[3][3];
	if (Z < 0.01f)
		return;

	lin_start.x = cheat::屏幕宽度 + (SkeletalAxis.x * cheat::Matrix[0][0] + SkeletalAxis.y * cheat::Matrix[0][1] + SkeletalAxis.z * cheat::Matrix[0][2] + cheat::Matrix[0][3]) / Z * cheat::屏幕宽度;
	lin_start.y = cheat::屏幕高度 - (SkeletalAxis.x * cheat::Matrix[1][0] + SkeletalAxis.y * cheat::Matrix[1][1] + SkeletalAxis.z * cheat::Matrix[1][2] + cheat::Matrix[1][3]) / Z * cheat::屏幕高度;


	lin_Address = Address + (End - 1) * 32;
	mem::Read(cheat::g_handle, lin_Address, &SkeletalAxis.x, 4);
	mem::Read(cheat::g_handle, lin_Address + 4, &SkeletalAxis.y, 4);
	mem::Read(cheat::g_handle, lin_Address + 8, &SkeletalAxis.z, 4);
	Z = SkeletalAxis.x * cheat::Matrix[3][0] + SkeletalAxis.y * cheat::Matrix[3][1] + SkeletalAxis.z * cheat::Matrix[3][2] + cheat::Matrix[3][3];
	if (Z < 0.01f)
		return;

	lin_End.x = cheat::屏幕宽度 + (SkeletalAxis.x * cheat::Matrix[0][0] + SkeletalAxis.y * cheat::Matrix[0][1] + SkeletalAxis.z * cheat::Matrix[0][2] + cheat::Matrix[0][3]) / Z * cheat::屏幕宽度;
	lin_End.y = cheat::屏幕高度 - (SkeletalAxis.x * cheat::Matrix[1][0] + SkeletalAxis.y * cheat::Matrix[1][1] + SkeletalAxis.z * cheat::Matrix[1][2] + cheat::Matrix[1][3]) / Z * cheat::屏幕高度;

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(lin_start.x, lin_start.y), ImVec2(lin_End.x, lin_End.y), ImColor(255, 255, 0), 1);
}

void 绘制骨骼() {

	
	
	if (Menu::绘制骨骼类型 == 0) {
		char* Address;
		float screen[2];
		D3D 骨骼坐标;
		auto drawList = ImGui::GetForegroundDrawList(); // 提前获取画笔

		for (int i = 0; i < 70; i++) {
			if (i != 36) {
				Address = cheat::ActorPlayer.SkeletonAddress[1] + (i - 1) * 32;
				// 读取骨骼坐标
				mem::Read(cheat::g_handle, Address, &骨骼坐标.x, sizeof(骨骼坐标.x));
				mem::Read(cheat::g_handle, Address + 4, &骨骼坐标.y, sizeof(骨骼坐标.y));
				mem::Read(cheat::g_handle, Address + 8, &骨骼坐标.z, sizeof(骨骼坐标.z));

				float 世界坐标[3] = { 骨骼坐标.x, 骨骼坐标.y, 骨骼坐标.z };
				if (WorldScreen2d(世界坐标, screen)) {
					char SkeletonText[256];
					sprintf_s(SkeletonText, "%.d", i);
					drawList->AddText({ screen[0], screen[1] }, ImColor(78, 173, 0), SkeletonText);
				}
			}
		}
	}

	else if(Menu::绘制骨骼类型 == 2) {
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 6, 36);//1
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],36, 1);//2
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 36,13);//3
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],36,61);//4
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 13, 14);//5
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],14,15);//6
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 61, 9);//7
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 9, 10);//8
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1], 1, 25);//9
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],1, 22);//10
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],25,26);//11
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],26,27);//12
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],22,23);//13
		骨骼连线(cheat::ActorPlayer.SkeletonAddress[1],23,24);//14
		
	}
	else
	{
		//绘制骨骼线条
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

	float 世界坐标[3];
	世界坐标[0] = cheat::ActorPlayer.Axis.x;
	世界坐标[1] = cheat::ActorPlayer.Axis.y;
	世界坐标[2] = cheat::ActorPlayer.Axis.z;
	float 屏幕坐标[2];
	
	if ( !WorldScreen2d(世界坐标, 屏幕坐标) )
	{
		return false;
	}

	ImGui::GetForegroundDrawList()->AddText({ 屏幕坐标[0],屏幕坐标[1] }, ImColor(255, 0, 0), u8"敌人");
	return true;

}

//取准星距离
float calculateDistance(const D2D 准星, const D2D 对象)
{
	D2D 距离_;
	float 距离;

	距离_.x = 准星.x - 对象.x;
	距离_.y = 准星.y - 对象.y;
	距离 = sqrt(距离_.x * 距离_.x + 距离_.y * 距离_.y);
	return 距离;
}

