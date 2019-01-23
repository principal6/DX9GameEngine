#include "DX9Game.h"

using namespace DX9ENGINE;

// Static member variables declaration
const int DX9Game::WINDOW_X = 50;
const int DX9Game::WINDOW_Y = 50;

DX9Game::DX9Game()
{
	m_TimerStart = 0;
	m_TimerSec = 0;
	m_TimerAnim = 0;
	m_FPS = 0;
	m_KeyPressCount = 0;
	m_KeyToggleCount = 0;
	m_bSpriteWalking = false;
	m_bDrawBoundingBoxes = false;
}

auto DX9Game::Create(int Width, int Height)->EError
{
	// Set base directory
	wchar_t tempDir[MAX_FILE_LEN]{};
	GetCurrentDirectoryW(MAX_FILE_LEN, tempDir);
	m_BaseDir = tempDir;
	m_BaseDir = m_BaseDir.substr(0, m_BaseDir.find_last_not_of(PROJECT_FOLDER));

	// Create base (window and initialize Direct3D9)
	if (m_Window = MAKE_UNIQUE(DX9Window)())
	{
		if (DX_FAILED(m_Window->CreateGameWindow(WINDOW_X, WINDOW_Y, Width, Height)))
			return EError::WINDOW_NOT_CREATED;

		// Set main window handle
		m_hMainWnd = m_Window->GethWnd();
	}

	// Create input device
	if (m_Input = MAKE_UNIQUE(DX9Input)())
	{
		// Pass main window's handle to DX9Input
		if (DX_FAILED(m_Input->Create(m_Window->GethWnd(), m_Window->GethInstance())))
			return EError::INPUT_NOT_CREATED;
	}

	// Create font manager object
	if (m_FontManager = MAKE_UNIQUE(DX9Font)())
	{
		if (DX_FAILED(m_FontManager->Create(m_Window.get())))
			return EError::FONTMANAGER_NOT_CREATED;
	}

	// Create image object
	if (m_Background = MAKE_UNIQUE(DX9Background)())
	{
		if (DX_FAILED(m_Background->Create(m_Window.get(), m_BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	
	// Create map object
	if (m_Map = MAKE_UNIQUE(DX9Map)())
	{
		if (DX_FAILED(m_Map->Create(m_Window.get(), m_BaseDir)))
			return EError::MAP_NOT_CREATED;
	}
	
	// Create sprite object
	if (m_Sprite = MAKE_UNIQUE(DX9Life)())
	{
		if (DX_FAILED(m_Sprite->Create(m_Window.get(), m_BaseDir, m_Map.get())))
			return EError::SPRITE_NOT_CREATED;
	}

	// Create monster manager object
	if (m_MonsterManager = MAKE_UNIQUE(DX9MonsterManager)())
	{
		if (DX_FAILED(m_MonsterManager->Create(m_Window.get(), m_BaseDir, m_Map.get())))
			return EError::MONSTERMANAGER_NOT_CREATED;
	}

	// Create effect manager object
	if (m_EffectManager = MAKE_UNIQUE(DX9Effect)())
	{
		if (DX_FAILED(m_EffectManager->Create(m_Window.get(), m_BaseDir, m_Map.get())))
			return EError::EFFECTMANAGER_NOT_CREATED;
	}

	return EError::OK;
}

void DX9Game::SetRenderFunction(PF_RENDER pfRender)
{
	m_pfRender = pfRender;
}

void DX9Game::SetKeyboardFunction(PF_KEYBOARD pfKeyboard)
{
	m_pfKeyboard = pfKeyboard;
}

void DX9Game::ToggleBoundingBox()
{
	if (m_KeyToggleCount == 0)
	{
		m_bDrawBoundingBoxes = !m_bDrawBoundingBoxes;
		m_KeyToggleCount++;
	}
}

auto DX9Game::LoadMap(WSTRING FileName)->EError
{
	if (m_Map)
	{
		m_Map->LoadMap(FileName);
		return EError::OK;
	}
	return EError::OBJECT_NOT_CREATED;
}

void DX9Game::Run()
{
	m_TimerStart = GetTickCount64();

	while (m_MSG.message != WM_QUIT)
	{
		if (PeekMessage(&m_MSG, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
		}
		else
		{
			MainLoop();
		}
	}

	Destroy();
}

void DX9Game::Shutdown()
{
	if (m_hMainWnd)
	{
		DestroyWindow(m_hMainWnd);
	}
}

void DX9Game::MainLoop()
{
	DetectInput();

	// Timer for calculating FPS
	if (GetTickCount64() >= m_TimerSec + 1000)
	{
		m_TimerSec = GetTickCount64();
		ULONGLONG tTime = (m_TimerSec - m_TimerStart) / 1000;
		std::cout << "Time: " << tTime << " // FPS: " << m_FPS << std::endl;
		m_FPS = 0;
	}

	// Timer for steady animations
	if (GetTickCount64() >= m_TimerAnim + ANIM_TICK)
	{
		m_TimerAnim = GetTickCount64();
		m_MonsterManager->Animate();
		m_Sprite->Animate();
	}

	if (m_KeyToggleCount > 0)
	{
		m_KeyToggleCount++;

		if (m_KeyToggleCount > KEY_TOGGLE_INTERVAL)
			m_KeyToggleCount = 0;
	}

	m_Window->BeginRender();

	// Apply gravity
	m_MonsterManager->Gravitate();
	m_Sprite->Gravitate();

	if (m_Map)
	{
		D3DXVECTOR2 mapOffset = m_Sprite->GetOffsetForMapMove();
		m_Map->SetGlobalPosition(-mapOffset);
	}

	if ((m_EffectManager) && (m_MonsterManager))
	{
		m_EffectManager->CheckCollisionWithMonsters(m_MonsterManager.get());
	}

	DrawAllBase();

	if (m_pfRender)
		m_pfRender();

	m_Window->EndRender();
	m_FPS++;
}

void DX9Game::DetectInput()
{
	m_bSpriteWalking = false;

	m_Input->GetAllKeyState(m_Keys);

	for (int i = 0; i < NUM_KEYS; i++)
	{
		if (m_Keys[i] == true)
		{
			if (m_pfKeyboard)
				m_pfKeyboard(i);
		}
	}

	if ((!m_bSpriteWalking) && (!m_Sprite->IsBeingAnimated()))
	{
		m_Sprite->SetAnimation(EAnimationID::Idle);
	}
}

void DX9Game::Destroy()
{
	DX_DESTROY_SMART(m_FontManager);
	DX_DESTROY_SMART(m_EffectManager);
	DX_DESTROY_SMART(m_MonsterManager);
	DX_DESTROY_SMART(m_Sprite);
	DX_DESTROY_SMART(m_Map);
	DX_DESTROY_SMART(m_Background);
	DX_DESTROY_SMART(m_Input);
	DX_DESTROY_SMART(m_Window);
}

void DX9Game::SetBackground(WSTRING TextureFN)
{
	assert(m_Background);
	m_Background->SetTexture(TextureFN);
}

auto DX9Game::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale)->DX9Life*
{
	assert(m_Sprite);
	return m_Sprite->MakeLife(TextureFN, numCols, numRows, Scale);
}

void DX9Game::SpriteWalk(EAnimationDirection Direction)
{
	assert(m_Sprite);

	m_bSpriteWalking = true;
	m_Sprite->Walk(Direction);
}

void DX9Game::SpriteJump()
{
	assert(m_Sprite);
	m_Sprite->Jump();
}

void DX9Game::SpriteSetAnimation(EAnimationID AnimationID)
{
	assert(m_Sprite);
	m_Sprite->SetAnimation(AnimationID);
}

auto DX9Game::SpawnEffect(int EffectID, int Damage)->DX9Effect*
{
	assert(m_EffectManager);
	return m_EffectManager->Spawn(EffectID, m_Sprite->GetCenterPosition(), m_Sprite->GetDirection(), Damage);
}

auto DX9Game::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*
{
	assert(m_MonsterManager);
	return m_MonsterManager->Spawn(MonsterName, GlobalPosition);
}

auto DX9Game::GetFontObject()->DX9Font*
{
	assert(m_FontManager);
	return m_FontManager.get();
}

auto DX9Game::GetSpriteObject()->DX9Life*
{
	assert(m_Sprite);
	return m_Sprite.get();
}

auto DX9Game::GetMonsterManagerObject()->DX9MonsterManager*
{
	assert(m_MonsterManager);
	return m_MonsterManager.get();
}

auto DX9Game::GetEffectManagerObject()->DX9Effect*
{
	assert(m_EffectManager);
	return m_EffectManager.get();
}

void DX9Game::DrawBackground()
{
	assert(m_Background);
	m_Background->Draw();
}

void DX9Game::DrawMap()
{
	assert(m_Map);
	m_Map->Draw();
}

void DX9Game::DrawMonsters()
{
	assert(m_MonsterManager);
	m_MonsterManager->Draw();
	if (m_bDrawBoundingBoxes)
		m_MonsterManager->DrawBoundingBox();
}

void DX9Game::DrawSprite()
{
	assert(m_Sprite);
	m_Sprite->Draw();
	if (m_bDrawBoundingBoxes)
		m_Sprite->DrawBoundingBox();
}

void DX9Game::DrawEffects()
{
	assert(m_EffectManager);
	m_EffectManager->Draw();
	if (m_bDrawBoundingBoxes)
		m_EffectManager->DrawBoundingBox();
}

void DX9Game::DrawAllBase()
{
	DrawBackground();
	DrawMap();
	DrawMonsters();
	DrawSprite();
	DrawEffects();
}