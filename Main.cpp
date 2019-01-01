#include "DX9Base.h"
#include "DX9Input.h"
#include "DX9Image.h"
#include "DX9Sprite.h"
#include "DX9Monsters.h"
#include "DX9Effect.h"
#include "DX9Map.h"
#include "DX9Font.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

// Function headers
int MainLoop();
void DetectInput();

// Constants
const int WINDOW_X = 50;
const int WINDOW_Y = 50;
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const int KEY_PRESS_INTERVAL = 20;

// Variables
bool gbWalking = false;
bool gbDrawBB = false;
ULONGLONG gTimerStart = 0;
ULONGLONG gTimerSec = 0;
ULONGLONG gTimerAnim = 0;
int gFPS = 0;
int gKeyPressCount = 0;

// Classes
DX9Base* gDXBase;
DX9Input* gDXInput;
DX9Image* gDXImage;
DX9Map* gDXMap;
DX9Sprite* gDXSprite;
DX9Monsters* gDXMonsters;
DX9Effect* gDXEffect;
DX9Font* gDXFont;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	wchar_t wszBaseDir[255] = { 0 };
	GetCurrentDirectoryW(255, wszBaseDir);

	gDXBase = new DX9Base;
	gDXBase->Create(WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H);

	gDXInput = new DX9Input;
	gDXInput->Create(gDXBase->GetInstance(), gDXBase->GetHWND());

	gDXImage = new DX9Image;
	gDXImage->SetStaticMembers(wszBaseDir, WINDOW_W, WINDOW_H);
	gDXImage->Create(gDXBase->GetDevice());
	gDXImage->SetTexture(L"bg_forest_evening.png");

	gDXMap = new DX9Map;
	gDXMap->Create(gDXBase->GetDevice(), WINDOW_H);
	gDXMap->LoadMapFromFile(L"map01.jwm");

	gDXSprite = new DX9Sprite();
	gDXSprite->Create(gDXBase->GetDevice(), gDXMap);
	gDXSprite->MakeUnit(L"advnt_full.png", 10, 10, 1.5f);
	gDXSprite->AddAnimation(DX9ANIMID::Idle, 0, 0);
	gDXSprite->AddAnimation(DX9ANIMID::Walk, 1, 5);
	gDXSprite->AddAnimation(DX9ANIMID::Jumping, 17, 17);
	gDXSprite->AddAnimation(DX9ANIMID::Falling, 18, 18);
	gDXSprite->AddAnimation(DX9ANIMID::Landing, 19, 19);
	gDXSprite->AddAnimation(DX9ANIMID::Attack1, 27, 28); // Punch
	gDXSprite->AddAnimation(DX9ANIMID::Attack2, 24, 26); // HorzAttack
	gDXSprite->SetGlobalPosition(D3DXVECTOR2(30.0f, 60.0f));
	gDXSprite->SetBoundingnBox(D3DXVECTOR2(-24, -18));
	
	gDXMonsters = new DX9Monsters();
	gDXMonsters->Create(gDXBase->GetDevice(), gDXMap);
	gDXMonsters->AddMonsterType(DX9MonsterType(L"Mage", L"mage-1-85x94.png", 4, 2, 200))
		->AddAnimation(DX9MONANIMDATA(DX9ANIMID::Idle, 0, 3))
		->AddAnimation(DX9MONANIMDATA(DX9ANIMID::Walk, 4, 7));
	gDXMonsters->Spawn(L"Mage", D3DXVECTOR2(560.0f, 60.0f))
		->SetAnimation(DX9ANIMID::Idle);
	gDXMonsters->Spawn(L"Mage", D3DXVECTOR2(400.0f, 300.0f))
		->SetAnimation(DX9ANIMID::Walk);

	gDXEffect = new DX9Effect;
	gDXEffect->Create(gDXBase->GetDevice(), gDXMap);
	gDXEffect->SetTextureAtlas(L"particlefx_14.png", 8, 8);
	gDXEffect->AddEffectType(DX9EFF_TYPE::Still, DX9ANIMDATA(0, 63), D3DXVECTOR2(80.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	gDXFont = new DX9Font;
	gDXFont->Create(gDXBase->GetDevice(), WINDOW_W, WINDOW_H);
	gDXFont->MakeFont(DX9FONT_ID::Debug, L"굴림", 14, true);

	gTimerStart = GetTickCount64();

	// Enter into the main loop
	gDXBase->Run(MainLoop);
	
	// Destroy all objects before the program ends
	gDXFont->Destroy();
	gDXEffect->Destroy();
	gDXMonsters->Destroy();
	gDXSprite->Destroy();
	gDXMap->Destroy();
	gDXImage->Destroy();
	gDXInput->Destroy();
	gDXBase->Destroy();

	delete gDXFont;
	delete gDXEffect;
	delete gDXMonsters;
	delete gDXSprite;
	delete gDXMap;
	delete gDXImage;
	delete gDXInput;
	delete gDXBase;

	return 0;
}

int MainLoop()
{
	DetectInput();

	// Timer for calculating FPS
	if (GetTickCount64() >= gTimerSec + 1000)
	{
		gTimerSec = GetTickCount64();
		ULONGLONG tTime = (gTimerSec - gTimerStart) / 1000;
		std::cout << "Time: " << tTime << " // FPS: " << gFPS << std::endl;
		gFPS = 0;
	}

	// Timer for steady animations
	if (GetTickCount64() >= gTimerAnim + 80)
	{
		gTimerAnim = GetTickCount64();
		gDXMonsters->Animate();
		gDXSprite->Animate();
	}

	if (gKeyPressCount > 0)
	{
		gKeyPressCount++;

		if (gKeyPressCount > KEY_PRESS_INTERVAL)
			gKeyPressCount = 0;
	}
	
	// Apply gravity
	gDXMonsters->Gravitate();
	gDXSprite->Gravitate();

	gDXBase->BeginRender();

		gDXImage->Draw();

		D3DXVECTOR2 tOffset = gDXSprite->GetOffsetForMapMove();
		gDXMap->SetGlobalPosition(-tOffset);
		gDXMap->Draw();

		gDXMonsters->Draw();
		gDXSprite->Draw();
		
		gDXEffect->CheckCollisionWithMonsters(gDXMonsters);
		gDXEffect->Draw();
		
		if (gbDrawBB)
		{
			gDXMonsters->DrawBoundingBox();
			gDXSprite->DrawBoundingBox();
			gDXEffect->DrawBoundingBox();
		}

		gDXFont->SelectFont(DX9FONT_ID::Debug);
		gDXFont->SetFontColor(D3DCOLOR_ARGB(255, 0, 255, 120));
		gDXFont->Draw(0, 5, L"화살표 키: 이동, 점프 / Ctrl: 물리 공격 / Alt: 마법 공격");
		gDXFont->Draw(0, 25, L"B: 바운딩 박스 토글");

	gDXBase->EndRender();

	gFPS++;

	return 0;
}

void DetectInput()
{
	gbWalking = false;

	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
	{
		gDXSprite->Walk(DX9ANIMDIR::Right);
		gbWalking = true;
	}
	if (gDXInput->OnKeyDown(DIK_LEFTARROW))
	{
		gDXSprite->Walk(DX9ANIMDIR::Left);
		gbWalking = true;
	}
	if (gDXInput->OnKeyDown(DIK_LCONTROL))
	{
		gDXSprite->SetAnimation(DX9ANIMID::Attack1);
	}
	if (gDXInput->OnKeyDown(DIK_LALT))
	{
		if (gKeyPressCount == 0)
		{
			gKeyPressCount++;
			gDXEffect->Spawn(0, gDXSprite->GetCenterPosition(), gDXSprite->GetDirection(), 20);
		}
	}
	if (gDXInput->OnKeyDown(DIK_UPARROW))
	{
		gDXSprite->Jump();
		//MoveSprite(D3DXVECTOR2(0, -kStride)); // Exists for debugging
	}
	if (gDXInput->OnKeyDown(DIK_DOWNARROW))
	{
		//MoveSprite(D3DXVECTOR2(0, kStride)); // Exists for debugging
	}
	if (gDXInput->OnKeyDown(DIK_ESCAPE))
		gDXBase->Halt();
	if (gDXInput->OnKeyDown(DIK_B))
	{
		if (gKeyPressCount == 0)
		{
			gKeyPressCount++;
			gbDrawBB = !gbDrawBB;
		}
	}

	if ((!gbWalking) && (!gDXSprite->IsBeingAnimated()))
	{
		gDXSprite->SetAnimation(DX9ANIMID::Idle);
	}
}