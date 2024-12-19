#include "esp.h"

void ���ƿ��() {

	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	// �����ϱ߿���ɫ�ͷ�ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(0, 153, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1 + (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		ImColor(255, 192, 203) // ��ɫ
	);

	// ������߿���ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(0, 153, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);

	// �����ұ߿򣨷�ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y1 + (cheat::ActorPlayer.y2 - cheat::ActorPlayer.y1) * 0.5f },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);

	// �����±߿򣨰�ɫ����ɫ��
	drawList->AddLine(
		{ cheat::ActorPlayer.x1, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 255, 255) // ��ɫ
	);
	drawList->AddLine(
		{ cheat::ActorPlayer.x2 - (cheat::ActorPlayer.x2 - cheat::ActorPlayer.x1) * 0.5f, cheat::ActorPlayer.y2 },
		{ cheat::ActorPlayer.x2, cheat::ActorPlayer.y2 },
		ImColor(255, 192, 203) // ��ɫ
	);

}
bool ����2D�����С() {
	//�Ų�
	float ��������[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float ��Ļ����[2];
	if (!WorldScreen2d(��������, ��Ļ����))
	{
		return false;
	}
	//ͷ��
	float ��������2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 70.f };
	float ��Ļ����2[2];
	if (!WorldScreen2d(��������2, ��Ļ����2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = ��Ļ����2[0] - (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y1 = ��Ļ����2[1];
	cheat::ActorPlayer.x2 = ��Ļ����[0] + (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y2 = ��Ļ����[1];
	return true;
}

void ��������Ӧ����() {
	͸��::D2D ��߿� = ��ȡ��߿�();
	͸��::D2D �ұ߿� = ��ȡ�ұ߿�();
	͸��::D2D �ϱ߿� = ��ȡ�ϱ߿�();
	͸��::D2D �±߿� = ��ȡ�±߿�();

	ImGui::GetForegroundDrawList()->AddLine({ ��߿�.x,��߿�.y }, { ��߿�.x,�ϱ߿�.y }, ImColor(255, 0, 0));//��߿��е㵽�ϱ߿����
	ImGui::GetForegroundDrawList()->AddLine({ ��߿�.x,�ϱ߿�.y }, { �ұ߿�.x,�ϱ߿�.y }, ImColor(255, 0, 0));//�ϱ߿���㵽�ұ߿򶥵�
	ImGui::GetForegroundDrawList()->AddLine({ �ұ߿�.x,�ϱ߿�.y }, { �ұ߿�.x,�±߿�.y }, ImColor(255, 0, 0));//�ұ߿򶥵㵽�±߿��ҵ�
	ImGui::GetForegroundDrawList()->AddLine({ �ұ߿�.x,�±߿�.y }, { ��߿�.x,�±߿�.y }, ImColor(255, 0, 0));//�±߿��ҵ㵽��߿�׵�
	ImGui::GetForegroundDrawList()->AddLine({ �ұ߿�.x,�±߿�.y }, { ��߿�.x,��߿�.y }, ImColor(255, 0, 0));//��߿�׵㵽��߿��е�


}

͸��::D2D ��ȡ��߿�() {
	char* ���ַ = cheat::ActorPlayer.SkeletonAddress[1] + 14 * 32;
	D3D ������;
	float leftscreen[2];
	mem::Read(��Ϸ����::g_handle, ���ַ, &������.x, 4);
	mem::Read(��Ϸ����::g_handle, ���ַ + 4, &������.y, 4);
	mem::Read(��Ϸ����::g_handle, ���ַ + 8, &������.z, 4);
	float leftworld[3] = { ������.x ,������.y,������.z };
	if (!WorldScreen2d(leftworld, leftscreen))
	{
		return { 0,0 };
	}
	͸��::D2D ��ʱ���� =  { leftscreen[0],leftscreen[1] };
	return ��ʱ����;
}
͸��::D2D ��ȡ�ұ߿�() {
	char* �ҵ�ַ = cheat::ActorPlayer.SkeletonAddress[1] + 9 * 32;
	D3D ������;
	float rightscreen[2];
	mem::Read(��Ϸ����::g_handle, �ҵ�ַ, &������.x, 4);
	mem::Read(��Ϸ����::g_handle, �ҵ�ַ + 4, &������.y, 4);
	mem::Read(��Ϸ����::g_handle, �ҵ�ַ + 8, &������.z, 4);
	float rightworld[3] = { ������.x ,������.y,������.z };
	if (!WorldScreen2d(rightworld, rightscreen))
	{
		return { 0,0 };
	}
	͸��::D2D ��ʱ���� = { rightscreen[0],rightscreen[1] };
	return ��ʱ����;
}
͸��::D2D ��ȡ�ϱ߿�() {
	char* �ϵ�ַ = cheat::ActorPlayer.SkeletonAddress[1] + 6 * 32;
	D3D ������;
	float topscreen[2];
	mem::Read(��Ϸ����::g_handle, �ϵ�ַ, &������.x, 4);
	mem::Read(��Ϸ����::g_handle, �ϵ�ַ + 4, &������.y, 4);
	mem::Read(��Ϸ����::g_handle, �ϵ�ַ + 8, &������.z, 4);
	float topworld[3] = { ������.x ,������.y,������.z };
	if (!WorldScreen2d(topworld, topscreen))
	{
		return { 0,0 };
	}
	͸��::D2D ��ʱ���� = { topscreen[0],topscreen[1] };
	return ��ʱ����;
}
͸��::D2D ��ȡ�±߿�() {
	char* �µ�ַ = cheat::ActorPlayer.SkeletonAddress[1] + 29 * 32;
	D3D ������;
	float bottomscreen[2];
	mem::Read(��Ϸ����::g_handle, �µ�ַ, &������.x, 4);
	mem::Read(��Ϸ����::g_handle, �µ�ַ + 4, &������.y, 4);
	mem::Read(��Ϸ����::g_handle, �µ�ַ + 8, &������.z, 4);
	float bottomworld[3] = { ������.x ,������.y,������.z };
	if (!WorldScreen2d(bottomworld, bottomscreen))
	{
		return { 0,0 };
	}
	͸��::D2D ��ʱ = { bottomscreen[0],bottomscreen[1] };
	return ��ʱ;
}

void ͸�ӻ���() {
	while (true) {
		if (Menu::box == 1) {
			���ƿ��();
		}
		else if (Menu::box == 2)
		{
			��������Ӧ����();
		}
		
	}
}
