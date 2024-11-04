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
