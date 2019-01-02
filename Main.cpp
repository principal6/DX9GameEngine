#include "DX9Engine.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

void Render();
void Keyboard(DWORD Key);
static DX9Engine m_Engine;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (m_Engine.Create(800, 600) != DX9ERROR_CHECK::OK)
		return -1;

	m_Engine.SetRenderFunction(Render);
	m_Engine.SetKeyboardFunction(Keyboard);

	m_Engine.SetBackground(L"bg_forest_evening.png");
	m_Engine.LoadMap(L"map01.jwm");
	m_Engine.SpriteCreate(L"advnt_full.png", 10, 10, 1.5f)
		->AddAnimation(DX9ANIMID::Idle, 0, 0)
		->AddAnimation(DX9ANIMID::Walk, 1, 5)
		->AddAnimation(DX9ANIMID::Jumping, 17, 17)
		->AddAnimation(DX9ANIMID::Falling, 18, 18)
		->AddAnimation(DX9ANIMID::Landing, 19, 19)
		->AddAnimation(DX9ANIMID::Attack1, 27, 28) // Punch
		->AddAnimation(DX9ANIMID::Attack2, 24, 26) // HorzAttack
		->SetGlobalPosition(D3DXVECTOR2(30.0f, 60.0f))
		->SetBoundingnBox(D3DXVECTOR2(-24, -18));

	m_Engine.GetMonsterManagerObject()
		->AddMonsterType(DX9MonsterType(L"Mage", L"mage-1-85x94.png", 4, 2, 200))
		->AddAnimation(DX9MONANIMDATA(DX9ANIMID::Idle, 0, 7));

	m_Engine.SpawnMonster(L"Mage", D3DXVECTOR2(560.0f, 60.0f))
		->SetAnimation(DX9ANIMID::Idle);
	m_Engine.SpawnMonster(L"Mage", D3DXVECTOR2(400.0f, 300.0f))
		->SetAnimation(DX9ANIMID::Idle);

	m_Engine.GetEffectManagerObject()
		->SetTextureAtlas(L"particlefx_14.png", 8, 8)
		->AddEffectType(DX9EFF_TYPE::Still, DX9ANIMDATA(0, 63), D3DXVECTOR2(80.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	m_Engine.GetFontObject()
		->MakeFont(DX9FONT_ID::Debug, L"굴림", 14, true);

	m_Engine.Run();

	return 0;
}

void Render()
{
	m_Engine.GetFontObject()
		->SelectFont(DX9FONT_ID::Debug)
		->SetFontColor(D3DCOLOR_ARGB(255, 0, 255, 120))
		->Draw(0, 5, L"화살표 키: 이동, 점프 / Ctrl: 물리 공격 / Alt: 마법 공격")
		->Draw(0, 25, L"B: 바운딩 박스 토글");
}

void Keyboard(DWORD Key)
{
	switch (Key)
	{
	case DIK_RIGHTARROW:
		m_Engine.SpriteWalk(DX9ANIMDIR::Right);
		break;
	case DIK_LEFTARROW:
		m_Engine.SpriteWalk(DX9ANIMDIR::Left);
		break;
	case DIK_LCONTROL:
		m_Engine.SpriteSetAnimation(DX9ANIMID::Attack1);
		break;
	case DIK_LALT:
		m_Engine.SpawnEffect(0, 30);
		break;
	case DIK_UPARROW:
		m_Engine.SpriteJump();
		break;
	case DIK_ESCAPE:
		m_Engine.Halt();
		break;
	case DIK_B:
		m_Engine.ToggleBoundingBox();
		break;
	default:
		break;
	}
}
