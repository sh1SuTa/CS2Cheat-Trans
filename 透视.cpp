#include "透视.h"

void 绘制框框() {

	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	// 绘制上边框（蓝色和粉色）
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(0, 153, 255) // 蓝色
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1 + (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(255, 192, 203) // 粉色
	);

	// 绘制左边框（蓝色到白色）
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(0, 153, 255) // 蓝色
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // 白色
	);

	// 绘制右边框（粉色到白色）
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // 粉色
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // 白色
	);

	// 绘制下边框（白色到粉色）
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // 白色
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2 - (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // 粉色
	);

}
bool 计算2D方框大小() {
	//脚部
	float 世界坐标[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float 屏幕坐标[2];
	if (!WorldScreen2d(世界坐标, 屏幕坐标))
	{
		return false;
	}
	//头部
	float 世界坐标2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 70.f };
	float 屏幕坐标2[2];
	if (!WorldScreen2d(世界坐标2, 屏幕坐标2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = 屏幕坐标2[0] - (屏幕坐标[1] - 屏幕坐标2[1]) / 4.f;
	cheat::ActorPlayer.y1 = 屏幕坐标2[1];
	cheat::ActorPlayer.x2 = 屏幕坐标[0] + (屏幕坐标[1] - 屏幕坐标2[1]) / 4.f;
	cheat::ActorPlayer.y2 = 屏幕坐标[1];
	return true;
}

void 绘制自适应方框() {
	透视::D2D 左边框 = 获取左边框();
	透视::D2D 右边框 = 获取右边框();
	透视::D2D 上边框 = 获取上边框();
	透视::D2D 下边框 = 获取下边框();

	ImGui::GetForegroundDrawList()->AddLine({ 左边框.x,左边框.y }, { 左边框.x,上边框.y }, ImColor(255, 0, 0));//左边框中点到上边框左点
	ImGui::GetForegroundDrawList()->AddLine({ 左边框.x,上边框.y }, { 右边框.x,上边框.y }, ImColor(255, 0, 0));//上边框左点到右边框顶点
	ImGui::GetForegroundDrawList()->AddLine({ 右边框.x,上边框.y }, { 右边框.x,下边框.y }, ImColor(255, 0, 0));//右边框顶点到下边框右点
	ImGui::GetForegroundDrawList()->AddLine({ 右边框.x,下边框.y }, { 左边框.x,下边框.y }, ImColor(255, 0, 0));//下边框右点到左边框底点
	ImGui::GetForegroundDrawList()->AddLine({ 右边框.x,下边框.y }, { 左边框.x,左边框.y }, ImColor(255, 0, 0));//左边框底点到左边框中点


}

透视::D2D 获取左边框() {
	char* 左地址 = cheat::ActorPlayer.SkeletonAddress[1] + 14 * 32;
	D3D 左坐标;
	float leftscreen[2];
	mem::Read(cheat::g_handle, 左地址, &左坐标.x, 4);
	mem::Read(cheat::g_handle, 左地址 + 4, &左坐标.y, 4);
	mem::Read(cheat::g_handle, 左地址 + 8, &左坐标.z, 4);
	float leftworld[3] = { 左坐标.x ,左坐标.y,左坐标.z };
	if (!WorldScreen2d(leftworld, leftscreen))
	{
		return { 0,0 };
	}
	透视::D2D 临时坐标 =  { leftscreen[0],leftscreen[1] };
	return 临时坐标;
}
透视::D2D 获取右边框() {
	char* 右地址 = cheat::ActorPlayer.SkeletonAddress[1] + 9 * 32;
	D3D 右坐标;
	float rightscreen[2];
	mem::Read(cheat::g_handle, 右地址, &右坐标.x, 4);
	mem::Read(cheat::g_handle, 右地址 + 4, &右坐标.y, 4);
	mem::Read(cheat::g_handle, 右地址 + 8, &右坐标.z, 4);
	float rightworld[3] = { 右坐标.x ,右坐标.y,右坐标.z };
	if (!WorldScreen2d(rightworld, rightscreen))
	{
		return { 0,0 };
	}
	透视::D2D 临时坐标 = { rightscreen[0],rightscreen[1] };
	return 临时坐标;
}
透视::D2D 获取上边框() {
	char* 上地址 = cheat::ActorPlayer.SkeletonAddress[1] + 6 * 32;
	D3D 上坐标;
	float topscreen[2];
	mem::Read(cheat::g_handle, 上地址, &上坐标.x, 4);
	mem::Read(cheat::g_handle, 上地址 + 4, &上坐标.y, 4);
	mem::Read(cheat::g_handle, 上地址 + 8, &上坐标.z, 4);
	float topworld[3] = { 上坐标.x ,上坐标.y,上坐标.z };
	if (!WorldScreen2d(topworld, topscreen))
	{
		return { 0,0 };
	}
	透视::D2D 临时坐标 = { topscreen[0],topscreen[1] };
	return 临时坐标;
}
透视::D2D 获取下边框() {
	char* 下地址 = cheat::ActorPlayer.SkeletonAddress[1] + 29 * 32;
	D3D 下坐标;
	float bottomscreen[2];
	mem::Read(cheat::g_handle, 下地址, &下坐标.x, 4);
	mem::Read(cheat::g_handle, 下地址 + 4, &下坐标.y, 4);
	mem::Read(cheat::g_handle, 下地址 + 8, &下坐标.z, 4);
	float bottomworld[3] = { 下坐标.x ,下坐标.y,下坐标.z };
	if (!WorldScreen2d(bottomworld, bottomscreen))
	{
		return { 0,0 };
	}
	透视::D2D 临时 = { bottomscreen[0],bottomscreen[1] };
	return 临时;
}

void 透视绘制() {
	if (Menu::box == 1) {
		绘制框框();
	}
	else if(Menu::box == 2)
	{
		绘制自适应方框();
	}
	else
	{
		return;
	}

}