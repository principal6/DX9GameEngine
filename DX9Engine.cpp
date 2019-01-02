#include "DX9Engine.h"

DX9Engine::DX9Engine()
{
	m_TimerStart = 0;
	m_TimerSec = 0;
	m_TimerAnim = 0;
	m_FPS = 0;
	m_KeyPressCount = 0;
	m_bSpriteWalking = false;
	m_bDrawBoundingBoxes = false;
}

DX9ERROR_CHECK DX9Engine::Create(int Width, int Height)
{
	// Set data that will be shared in many sub-classes
	m_ShareData.WindowWidth = Width;
	m_ShareData.WindowHeight = Height;
	m_ShareData.m_WindowHalfW = static_cast<float>(Width / 2.0f);
	m_ShareData.m_WindowHalfH = static_cast<float>(Height / 2.0f);
	GetCurrentDirectoryW(255, m_ShareData.AppDir);
	
	// Create window and initialize Direct3D9
	if (!DX9Base::Create(WINDOW_X, WINDOW_Y, &m_ShareData))
		return DX9ERROR_CHECK::CREATION_FAILED;

	// Create input device
	if (m_Input = new DX9Input)
	{
		if (!m_Input->Create(GetInstance(), GetHWND()))
			return DX9ERROR_CHECK::CREATION_FAILED;
	}

	// Create image object
	if (m_ImageBackGround = new DX9Image)
	{
		m_ImageBackGround->Create(GetDevice(), &m_ShareData);
	}
	
	// Create map object
	if (m_Map = new DX9Map)
	{
		m_Map->Create(GetDevice(), &m_ShareData);
	}
	
	// Create sprite object
	if (m_Sprite = new DX9Sprite())
	{
		m_Sprite->Create(GetDevice(), &m_ShareData, m_Map);
	}

	// Create monster manager object
	if (m_MonsterManager = new DX9MonsterManager())
	{
		m_MonsterManager->Create(GetDevice(), &m_ShareData, m_Map);
	}

	// Create effect object
	if (m_EffectManager = new DX9Effect)
	{
		m_EffectManager->Create(GetDevice(), &m_ShareData, m_Map);
	}

	if (m_Font = new DX9Font)
	{
		m_Font->Create(GetDevice(), &m_ShareData);
	}

	return DX9ERROR_CHECK::OK;
}

void DX9Engine::SetRenderFunction(void(*Render)())
{
	m_pfRender = Render;
}

void DX9Engine::SetKeyboardFunction(void(*Keyboard)(DWORD DIK_KeyCode))
{
	m_pfKeyboard = Keyboard;
}

void DX9Engine::ToggleBoundingBox()
{
	m_bDrawBoundingBoxes = !m_bDrawBoundingBoxes;
}

DX9ERROR_CHECK DX9Engine::LoadMap(WSTRING FileName)
{
	if (m_Map)
	{
		m_Map->LoadMapFromFile(FileName);
		return DX9ERROR_CHECK::OK;
	}
	return DX9ERROR_CHECK::OBJECT_NOT_CREATED;
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

int DX9Engine::RunWithAccel(HACCEL hAccel)
{
	m_TimerStart = GetTickCount64();

	while (GetMessage(&m_MSG, 0, 0, 0)) {
		if (!TranslateAccelerator(m_hWnd, hAccel, &m_MSG)) {
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);

			MainLoop();
		}
	}

	return (int)m_MSG.wParam;
}

void DX9Engine::Halt()
{
	DX9Base::Halt();
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
	if (GetTickCount64() >= m_TimerAnim + 80)
	{
		m_TimerAnim = GetTickCount64();
		m_MonsterManager->Animate();
		m_Sprite->Animate();
	}

	if (m_KeyPressCount > 0)
	{
		m_KeyPressCount++;

		if (m_KeyPressCount > KEY_PRESS_INTERVAL)
			m_KeyPressCount = 0;
	}

	DX9Base::BeginRender();

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

	DX9Base::EndRender();
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
		m_Sprite->SetAnimation(DX9ANIMID::Idle);
	}
}

void DX9Engine::Destroy()
{
	m_Font->Destroy();
	m_EffectManager->Destroy();
	m_MonsterManager->Destroy();
	m_Sprite->Destroy();
	m_Map->Destroy();
	m_ImageBackGround->Destroy();
	m_Input->Destroy();

	delete m_Font;
	delete m_EffectManager;
	delete m_MonsterManager;
	delete m_Sprite;
	delete m_Map;
	delete m_ImageBackGround;
	delete m_Input;

	m_Font = nullptr;
	m_EffectManager = nullptr;
	m_MonsterManager = nullptr;
	m_Sprite = nullptr;
	m_Map = nullptr;
	m_ImageBackGround = nullptr;
	m_Input = nullptr;

	DX9Base::Destroy();
}

void DX9Engine::SetBackground(WSTRING TextureFN)
{
	assert(m_ImageBackGround);
	m_ImageBackGround->SetTexture(TextureFN);
}

DX9Sprite* DX9Engine::SpriteCreate(WSTRING TextureFN, int numCols, int numRows, float Scale)
{
	assert(m_Sprite);
	return m_Sprite->MakeUnit(TextureFN, numCols, numRows, Scale);
}

void DX9Engine::SpriteWalk(DX9ANIMDIR Direction)
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

void DX9Engine::SpriteSetAnimation(DX9ANIMID AnimationID)
{
	assert(m_Sprite);
	m_Sprite->SetAnimation(AnimationID);
}

DX9Effect* DX9Engine::SpawnEffect(int EffectID, int Damage)
{
	assert(m_EffectManager);
	return m_EffectManager->Spawn(EffectID, m_Sprite->GetCenterPosition(), m_Sprite->GetDirection(), Damage);
}

DX9Monster* DX9Engine::SpawnMonster(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)
{
	assert(m_MonsterManager);
	return m_MonsterManager->Spawn(MonsterName, GlobalPosition);
}

DX9Font* DX9Engine::GetFontObject()
{
	assert(m_Font);
	return m_Font;
}

DX9Sprite* DX9Engine::GetSpriteObject()
{
	assert(m_Sprite);
	return m_Sprite;
}

DX9MonsterManager* DX9Engine::GetMonsterManagerObject()
{
	assert(m_MonsterManager);
	return m_MonsterManager;
}

DX9Effect* DX9Engine::GetEffectManagerObject()
{
	assert(m_EffectManager);
	return m_EffectManager;
}

void DX9Engine::DrawBackground()
{
	assert(m_ImageBackGround);
	m_ImageBackGround->Draw();
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