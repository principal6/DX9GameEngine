#include "DX9Engine.h"

using namespace DX9ENGINE;

// Static member variables declaration
const int DX9Engine::WINDOW_X = 50;
const int DX9Engine::WINDOW_Y = 50;

DX9Engine::DX9Engine()
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

auto DX9Engine::Create(int Width, int Height)->Error
{
	// Set base directory
	wchar_t tempDir[MAX_FILE_LEN]{};
	GetCurrentDirectoryW(255, tempDir);
	m_BaseDir = tempDir;

	// Create base (window and initialize Direct3D9)
	if (m_Base = new DX9Base)
	{
		if (DX_FAILED(m_Base->CreateGameWindow(WINDOW_X, WINDOW_Y, Width, Height)))
			return Error::BASE_NOT_CREATED;

		// Set main window handle
		m_hMainWnd = m_Base->GethWnd();
	}

	// Create input device
	if (m_Input = new DX9Input)
	{
		// Pass main window's handle to DX9Input
		if (DX_FAILED(m_Input->Create(m_Base->GethWnd(), m_Base->GethInstance())))
			return Error::INPUT_NOT_CREATED;
	}

	// Create font manager object
	if (m_FontManager = new DX9Font)
	{
		if (DX_FAILED(m_FontManager->Create(m_Base)))
			return Error::FONTMANAGER_NOT_CREATED;
	}

	// Create image object
	if (m_Background = new DX9Background)
	{
		if (DX_FAILED(m_Background->Create(m_Base, m_BaseDir)))
			return Error::IMAGE_NOT_CREATED;
	}
	
	// Create map object
	if (m_Map = new DX9Map)
	{
		if (DX_FAILED(m_Map->Create(m_Base, m_BaseDir)))
			return Error::MAP_NOT_CREATED;
	}
	
	// Create sprite object
	if (m_Sprite = new DX9Life())
	{
		if (DX_FAILED(m_Sprite->Create(m_Base, m_BaseDir, m_Map)))
			return Error::SPRITE_NOT_CREATED;
	}

	// Create monster manager object
	if (m_MonsterManager = new DX9MonsterManager())
	{
		if (DX_FAILED(m_MonsterManager->Create(m_Base, m_BaseDir, m_Map)))
			return Error::MONSTERMANAGER_NOT_CREATED;
	}

	// Create effect manager object
	if (m_EffectManager = new DX9Effect)
	{
		if (DX_FAILED(m_EffectManager->Create(m_Base, m_BaseDir, m_Map)))
			return Error::EFFECTMANAGER_NOT_CREATED;
	}

	return Error::OK;
}

void DX9Engine::SetRenderFunction(PF_RENDER pfRender)
{
	m_pfRender = pfRender;
}

void DX9Engine::SetKeyboardFunction(PF_KEYBOARD pfKeyboard)
{
	m_pfKeyboard = pfKeyboard;
}

void DX9Engine::ToggleBoundingBox()
{
	if (m_KeyToggleCount == 0)
	{
		m_bDrawBoundingBoxes = !m_bDrawBoundingBoxes;
		m_KeyToggleCount++;
	}
}

auto DX9Engine::LoadMap(WSTRING FileName)->Error
{
	if (m_Map)
	{
		m_Map->LoadMap(FileName);
		return Error::OK;
	}
	return Error::OBJECT_NOT_CREATED;
}

void DX9Engine::Run()
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

void DX9Engine::Shutdown()
{
	if (m_hMainWnd)
	{
		DestroyWindow(m_hMainWnd);
	}
}

void DX9Engine::MainLoop()
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

	m_Base->BeginRender();

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
		m_EffectManager->CheckCollisionWithMonsters(m_MonsterManager);
	}

	DrawAllBase();

	if (m_pfRender)
		m_pfRender();

	m_Base->EndRender();
	m_FPS++;
}

void DX9Engine::DetectInput()
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
		m_Sprite->SetAnimation(AnimationID::Idle);
	}
}

void DX9Engine::Destroy()
{
	DX_DESTROY(m_FontManager);
	DX_DESTROY(m_EffectManager);
	DX_DESTROY(m_MonsterManager);
	DX_DESTROY(m_Sprite);
	DX_DESTROY(m_Map);
	DX_DESTROY(m_Background);
	DX_DESTROY(m_Input);
	DX_DESTROY(m_Base);
}

void DX9Engine::SetBackground(WSTRING TextureFN)
{
	assert(m_Background);
	m_Background->SetTexture(TextureFN);
}

auto DX9Engine::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale)->DX9Life*
{
	assert(m_Sprite);
	return m_Sprite->MakeLife(TextureFN, numCols, numRows, Scale);
}

void DX9Engine::SpriteWalk(AnimationDir Direction)
{
	assert(m_Sprite);

	m_bSpriteWalking = true;
	m_Sprite->Walk(Direction);
}

void DX9Engine::SpriteJump()
{
	assert(m_Sprite);
	m_Sprite->Jump();
}

void DX9Engine::SpriteSetAnimation(AnimationID AnimationID)
{
	assert(m_Sprite);
	m_Sprite->SetAnimation(AnimationID);
}

auto DX9Engine::SpawnEffect(int EffectID, int Damage)->DX9Effect*
{
	assert(m_EffectManager);
	return m_EffectManager->Spawn(EffectID, m_Sprite->GetCenterPosition(), m_Sprite->GetDirection(), Damage);
}

auto DX9Engine::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*
{
	assert(m_MonsterManager);
	return m_MonsterManager->Spawn(MonsterName, GlobalPosition);
}

auto DX9Engine::GetFontObject()->DX9Font*
{
	assert(m_FontManager);
	return m_FontManager;
}

auto DX9Engine::GetSpriteObject()->DX9Life*
{
	assert(m_Sprite);
	return m_Sprite;
}

auto DX9Engine::GetMonsterManagerObject()->DX9MonsterManager*
{
	assert(m_MonsterManager);
	return m_MonsterManager;
}

auto DX9Engine::GetEffectManagerObject()->DX9Effect*
{
	assert(m_EffectManager);
	return m_EffectManager;
}

void DX9Engine::DrawBackground()
{
	assert(m_Background);
	m_Background->Draw();
}

void DX9Engine::DrawMap()
{
	assert(m_Map);
	m_Map->Draw();
}

void DX9Engine::DrawMonsters()
{
	assert(m_MonsterManager);
	m_MonsterManager->Draw();
	if (m_bDrawBoundingBoxes)
		m_MonsterManager->DrawBoundingBox();
}

void DX9Engine::DrawSprite()
{
	assert(m_Sprite);
	m_Sprite->Draw();
	if (m_bDrawBoundingBoxes)
		m_Sprite->DrawBoundingBox();
}

void DX9Engine::DrawEffects()
{
	assert(m_EffectManager);
	m_EffectManager->Draw();
	if (m_bDrawBoundingBoxes)
		m_EffectManager->DrawBoundingBox();
}

void DX9Engine::DrawAllBase()
{
	DrawBackground();
	DrawMap();
	DrawMonsters();
	DrawSprite();
	DrawEffects();
}