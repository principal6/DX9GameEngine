#pragma once

#include "Core/DX9Base.h"
#include "Core/DX9Input.h"
#include "DX9Background.h"
#include "DX9Sprite.h"
#include "DX9Monsters.h"
#include "DX9Effect.h"
#include "DX9Map.h"
#include "DX9Font.h"

namespace DX9ENGINE
{
	using PF_RENDER = void(*)();
	using PF_KEYBOARD = void(*)(DWORD DIK_KeyCode);

	class DX9Engine final
	{
	private:
		static const int ANIM_TICK = 70;
		static const int KEY_TOGGLE_INTERVAL = 15;
		static const int WINDOW_X;
		static const int WINDOW_Y;

		bool m_Keys[NUM_KEYS];

		ULONGLONG m_TimerStart;
		ULONGLONG m_TimerSec;
		ULONGLONG m_TimerAnim;
		int m_FPS;
		int m_KeyPressCount;
		int m_KeyToggleCount;
		bool m_bSpriteWalking;
		bool m_bDrawBoundingBoxes;

		PF_RENDER m_pfRender;
		PF_KEYBOARD m_pfKeyboard;

		DX9Base* m_Base;
		DX9Input* m_Input;
		DX9Background* m_Background;
		DX9Map* m_Map;
		DX9Sprite* m_Sprite;
		DX9MonsterManager* m_MonsterManager;
		DX9Effect* m_EffectManager;
		DX9Font* m_FontManager;

		WSTRING m_BaseDir;
		HWND m_hMainWnd;
		MSG m_MSG;

	private:
		void DX9Engine::Destroy();

		void DX9Engine::DrawBackground();
		void DX9Engine::DrawMap();
		void DX9Engine::DrawMonsters();
		void DX9Engine::DrawSprite();
		void DX9Engine::DrawEffects();

		void DX9Engine::DrawAllBase();

	public:
		DX9Engine();
		~DX9Engine() {};

		auto DX9Engine::Create(int Width, int Height)->Error;
		void DX9Engine::SetRenderFunction(PF_RENDER pfRender);
		void DX9Engine::SetKeyboardFunction(PF_KEYBOARD pfKeyboard);
		void DX9Engine::ToggleBoundingBox();
		auto DX9Engine::LoadMap(WSTRING FileName)->Error;

		void DX9Engine::Run();
		void DX9Engine::Shutdown();

		void DX9Engine::MainLoop();
		void DX9Engine::DetectInput();

		void DX9Engine::SetBackground(WSTRING TextureFN);

		auto DX9Engine::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f)->DX9Sprite*;
		void DX9Engine::SpriteWalk(AnimationDir Direction);
		void DX9Engine::SpriteJump();
		void DX9Engine::SpriteSetAnimation(AnimationID AnimationID);

		auto DX9Engine::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*;
		auto DX9Engine::SpawnEffect(int EffectID, int Damage)->DX9Effect*;

		auto DX9Engine::GetFontObject()->DX9Font*;
		auto DX9Engine::GetSpriteObject()->DX9Sprite*;
		auto DX9Engine::GetMonsterManagerObject()->DX9MonsterManager*;
		auto DX9Engine::GetEffectManagerObject()->DX9Effect*;
	};
};