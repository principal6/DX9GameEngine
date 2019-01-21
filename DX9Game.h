#pragma once

#include "Core/DX9Window.h"
#include "Core/DX9Input.h"
#include "Game/DX9Background.h"
#include "Game/DX9Life.h"
#include "Game/DX9Monsters.h"
#include "Game/DX9Effect.h"
#include "Game/DX9Map.h"
#include "Game/DX9Font.h"

namespace DX9ENGINE
{
	using PF_RENDER = void(*)();
	using PF_KEYBOARD = void(*)(DWORD DIK_KeyCode);

	class DX9Game final
	{
	public:
		DX9Game();
		~DX9Game() {};

		auto DX9Game::Create(int Width, int Height)->EError;
		void DX9Game::SetRenderFunction(PF_RENDER pfRender);
		void DX9Game::SetKeyboardFunction(PF_KEYBOARD pfKeyboard);
		void DX9Game::ToggleBoundingBox();
		auto DX9Game::LoadMap(WSTRING FileName)->EError;

		void DX9Game::Run();
		void DX9Game::Shutdown();

		void DX9Game::MainLoop();
		void DX9Game::DetectInput();

		void DX9Game::SetBackground(WSTRING TextureFN);

		auto DX9Game::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f)->DX9Life*;
		void DX9Game::SpriteWalk(EAnimationDirection Direction);
		void DX9Game::SpriteJump();
		void DX9Game::SpriteSetAnimation(EAnimationID AnimationID);

		auto DX9Game::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*;
		auto DX9Game::SpawnEffect(int EffectID, int Damage)->DX9Effect*;

		auto DX9Game::GetFontObject()->DX9Font*;
		auto DX9Game::GetSpriteObject()->DX9Life*;
		auto DX9Game::GetMonsterManagerObject()->DX9MonsterManager*;
		auto DX9Game::GetEffectManagerObject()->DX9Effect*;

	private:
		void DX9Game::Destroy();

		void DX9Game::DrawBackground();
		void DX9Game::DrawMap();
		void DX9Game::DrawMonsters();
		void DX9Game::DrawSprite();
		void DX9Game::DrawEffects();

		void DX9Game::DrawAllBase();

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

		UNIQUE_PTR<DX9Window> m_Window;
		UNIQUE_PTR<DX9Input> m_Input;
		UNIQUE_PTR<DX9Background> m_Background;
		UNIQUE_PTR<DX9Map> m_Map;
		UNIQUE_PTR<DX9Life> m_Sprite;
		UNIQUE_PTR<DX9MonsterManager> m_MonsterManager;
		UNIQUE_PTR<DX9Effect> m_EffectManager;
		UNIQUE_PTR<DX9Font> m_FontManager;

		WSTRING m_BaseDir;
		HWND m_hMainWnd;
		MSG m_MSG;
	};
};