#include "DX9Engine.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace DX9ENGINE;

void Render();
void Keyboard(DWORD Key);
static DX9Engine g_Engine;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (DX_FAILED(g_Engine.Create(800, 600)))
		return -1;

	g_Engine.SetBackground(L"colored_talltrees.png");
	g_Engine.LoadMap(L"map01.jwm");
	g_Engine.SpriteCreate(L"advnt_full.png", 16, 8, 1.5f)
		->AddAnimation(DX9Common::AnimationID::Idle, 0, 0)
		->AddAnimation(DX9Common::AnimationID::Walk, 1, 6)
		->AddAnimation(DX9Common::AnimationID::Jumping, 23, 23)
		->AddAnimation(DX9Common::AnimationID::Falling, 24, 24)
		->AddAnimation(DX9Common::AnimationID::Landing, 25, 25)
		->AddAnimation(DX9Common::AnimationID::Attack1, 39, 40) // Punch
		->AddAnimation(DX9Common::AnimationID::Attack2, 36, 38) // HorzAttack
		->SetGlobalPosition(D3DXVECTOR2(30.0f, 60.0f))
		->SetBoundingBox(D3DXVECTOR2(-24, -24));

	g_Engine.GetMonsterManagerObject()
		->AddMonsterType(DX9MonsterType(L"Mage", L"mage-1-85x94.png", 4, 2, 200))
		->AddAnimation(DX9Common::AnimationData(DX9Common::AnimationID::Idle, 0, 7));

	g_Engine.SpawnMonster(L"Mage", D3DXVECTOR2(560.0f, 60.0f))
		->SetAnimation(DX9Common::AnimationID::Idle);
	g_Engine.SpawnMonster(L"Mage", D3DXVECTOR2(400.0f, 300.0f))
		->SetAnimation(DX9Common::AnimationID::Idle);

	g_Engine.GetEffectManagerObject()
		->SetTextureAtlas(L"particlefx_14.png", 8, 8)
		->AddEffectType(DX9Common::EffectType::Still, DX9Common::AnimationData(DX9Common::AnimationID::Effect, 0, 63),
			D3DXVECTOR2(80.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 15);

	g_Engine.GetFontObject()
		->MakeFont(DX9Common::FontID::Debug, L"굴림", 14, true);

	g_Engine.SetRenderFunction(Render);
	g_Engine.SetKeyboardFunction(Keyboard);

	g_Engine.Run();

	return 0;
}

void Render()
{
	g_Engine.GetFontObject()
		->SelectFont(DX9Common::FontID::Debug)
		->SetFontColor(D3DCOLOR_ARGB(255, 0, 100, 50))
		->Draw(0, 5, L"화살표 키: 이동, 점프 / Ctrl: 물리 공격 / Alt: 마법 공격")
		->Draw(0, 25, L"B: 바운딩 박스 토글");
}

void Keyboard(DWORD Key)
{
	switch (Key)
	{
	case DIK_RIGHTARROW:
		g_Engine.SpriteWalk(DX9Common::AnimationDir::Right);
		break;
	case DIK_LEFTARROW:
		g_Engine.SpriteWalk(DX9Common::AnimationDir::Left);
		break;
	case DIK_LCONTROL:
		g_Engine.SpriteSetAnimation(DX9Common::AnimationID::Attack1);
		break;
	case DIK_LALT:
		g_Engine.SpawnEffect(0, 30);
		break;
	case DIK_UPARROW:
		g_Engine.SpriteJump();
		break;
	case DIK_ESCAPE:
		g_Engine.Shutdown();
		break;
	case DIK_B:
		g_Engine.ToggleBoundingBox();
		break;
	default:
		break;
	}
}
