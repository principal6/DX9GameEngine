#include "DX9Game.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace DX9ENGINE;

void Render();
void Keyboard(DWORD Key);
static DX9Game g_MyGame;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (DX_FAILED(g_MyGame.Create(800, 600)))
		return -1;

	g_MyGame.SetBackground(L"colored_talltrees.png");

	g_MyGame.LoadMap(L"map01.jwm");
	g_MyGame.SpriteCreate(L"firefox_56x80_creek23.png", POINT{ 56, 80 }, 3, 9, 1.0f)
		->AddAnimation(EAnimationID::Idle, 0, 2)
		->AddAnimation(EAnimationID::Walk, 3, 5)
		->AddAnimation(EAnimationID::Jumping, 7, 7)
		->AddAnimation(EAnimationID::Falling, 8, 8)
		->AddAnimation(EAnimationID::Landing, 6, 6)
		->AddAnimation(EAnimationID::Attack1, 12, 14) // Punch
		->AddAnimation(EAnimationID::Attack2, 15, 17) // Kick
		->SetGlobalPosition(D3DXVECTOR2(30.0f, 250.0f))
		->SetBoundingBox(D3DXVECTOR2(-12, -12));

	g_MyGame.GetMonsterManagerObject()
		->AddMonsterType(DX9MonsterType(L"Mage", L"mage.png", POINT{ 128, 128 }, 4, 2, 200, D3DXVECTOR2(-40, -30)))
		->AddAnimation(SAnimationData(EAnimationID::Idle, 0, 7));

	g_MyGame.SpawnMonster(L"Mage", D3DXVECTOR2(560.0f, 80.0f))
		->SetAnimation(EAnimationID::Idle);
	g_MyGame.SpawnMonster(L"Mage", D3DXVECTOR2(400.0f, 300.0f))
		->SetAnimation(EAnimationID::Idle);

	g_MyGame.GetEffectManagerObject()
		->SetTextureAtlas(L"particlefx_14.png", 8, 8)
		->AddEffectType(EEffectType::Still, SAnimationData(EAnimationID::Effect, 0, 63),
			D3DXVECTOR2(80.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 15);

	g_MyGame.GetFontObject()
		->MakeFont(EFontID::Debug, L"굴림", 14, true);

	g_MyGame.SetRenderFunction(Render);
	g_MyGame.SetKeyboardFunction(Keyboard);

	g_MyGame.Run();

	return 0;
}

void Render()
{
	g_MyGame.GetFontObject()
		->SelectFontByID(EFontID::Debug)
		->SetFontColor(D3DCOLOR_ARGB(255, 0, 100, 50))
		->Draw(0, 5, L"화살표 키: 이동, 점프 / Ctrl: 물리 공격 / Alt: 마법 공격")
		->Draw(0, 25, L"B: 바운딩 박스 토글");
}

void Keyboard(DWORD Key)
{
	switch (Key)
	{
	case DIK_RIGHTARROW:
		g_MyGame.SpriteWalk(EAnimationDirection::Right);
		break;
	case DIK_LEFTARROW:
		g_MyGame.SpriteWalk(EAnimationDirection::Left);
		break;
	case DIK_LCONTROL:
		g_MyGame.SpriteSetAnimation(EAnimationID::Attack2);
		break;
	case DIK_LALT:
		g_MyGame.SpriteSetAnimation(EAnimationID::Attack1);
		g_MyGame.SpawnEffect(0, 30);
		break;
	case DIK_UPARROW:
		g_MyGame.SpriteJump();
		break;
	case DIK_ESCAPE:
		g_MyGame.Shutdown();
		break;
	case DIK_B:
		g_MyGame.ToggleBoundingBox();
		break;
	default:
		break;
	}
}
