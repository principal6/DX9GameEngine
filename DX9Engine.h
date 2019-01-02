#pragma once

#include "DX9Base.h"
#include "DX9Input.h"
#include "DX9Image.h"
#include "DX9Sprite.h"
#include "DX9Monsters.h"
#include "DX9Effect.h"
#include "DX9Map.h"
#include "DX9Font.h"

class DX9Engine final : public DX9Base
{
private:
	static const int WINDOW_X = 50;
	static const int WINDOW_Y = 50;
	static const int KEY_PRESS_INTERVAL = 20;

	bool m_Keys[NUM_KEYS];

	ULONGLONG m_TimerStart;
	ULONGLONG m_TimerSec;
	ULONGLONG m_TimerAnim;
	int m_FPS;
	int m_KeyPressCount;
	bool m_bSpriteWalking;
	bool m_bDrawBoundingBoxes;

	void(*m_pfRender)();
	void(*m_pfKeyboard)(DWORD DIK_KeyCode);
	DX9SHARE_DATA m_ShareData;

	DX9Input* m_Input;
	DX9Image* m_ImageBackGround;
	DX9Map* m_Map;
	DX9Sprite* m_Sprite;
	DX9MonsterManager* m_MonsterManager;
	DX9Effect* m_EffectManager;
	DX9Font* m_Font;

private:
	void DX9Engine::Destroy() override;

	void DX9Engine::DrawBackground();
	void DX9Engine::DrawMap();
	void DX9Engine::DrawMonsters();
	void DX9Engine::DrawSprite();
	void DX9Engine::DrawEffects();

	void DX9Engine::DrawAllBase();

public:
	DX9Engine();
	~DX9Engine() {};

	DX9ERROR_CHECK DX9Engine::Create(int Width, int Height);
	void DX9Engine::SetRenderFunction(void(*Render)());
	void DX9Engine::SetKeyboardFunction(void(*Keyboard)(DWORD DIK_KeyCode));
	void DX9Engine::ToggleBoundingBox();
	DX9ERROR_CHECK DX9Engine::LoadMap(WSTRING FileName);

	void DX9Engine::Run() override;
	int DX9Engine::RunWithAccel(HACCEL hAccel) override;
	void DX9Engine::Halt() override;

	void DX9Engine::MainLoop();
	void DX9Engine::DetectInput();

	void DX9Engine::SetBackground(WSTRING TextureFN);
	DX9Sprite* DX9Engine::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f);
	void DX9Engine::SpriteWalk(DX9ANIMDIR Direction);
	void DX9Engine::SpriteJump();
	void DX9Engine::SpriteSetAnimation(DX9ANIMID AnimationID);

	DX9Monster* DX9Engine::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition);
	DX9Effect* DX9Engine::SpawnEffect(int EffectID, int Damage);

	DX9Font* DX9Engine::GetFontObject();
	DX9Sprite* DX9Engine::GetSpriteObject();
	DX9MonsterManager* DX9Engine::GetMonsterManagerObject();
	DX9Effect* DX9Engine::GetEffectManagerObject();
};