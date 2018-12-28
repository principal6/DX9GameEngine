#include "DX9Base.h"
#include "DX9Input.h"
#include "DX9Image.h"
#include "DX9Sprite.h"
#include "DX9Monster.h"
#include "DX9Effect.h"
#include "DX9Map.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

// 함수 헤더
int MainLoop();
int DetectInput();
int Gravitate();
int MoveSprite(D3DXVECTOR2 Velocity);

// 상수
const int WINDOW_X = 50;
const int WINDOW_Y = 50;
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const D3DXVECTOR2 kGravity = D3DXVECTOR2(0.0f, 0.6f);
const D3DXVECTOR2 kJumpPow = D3DXVECTOR2(0.0f, -15.0f);
const float kStride = 5.0f;

// 변수
float gSprGroundOffsetY = 0.0f;
float gMapGroundOffsetY = 0.0f;
bool gbHitGround = false;
bool gbWalking = false;
bool gbEffect = false;
ULONGLONG gTimerStart = 0;
ULONGLONG gTimerSec = 0;
ULONGLONG gTimerAnim = 0;
int gFPS = 0;

// 클래스
DX9Base* gDXBase;
DX9Input* gDXInput;
DX9Image* gDXImage;
DX9Sprite* gDXSprite;
DX9Monster* gDXMonster;
DX9Effect* gDXEffect;
DX9Map* gDXMap;

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
	gDXImage->Create(gDXBase->GetDevice(), wszBaseDir);
	gDXImage->SetTexture(L"bg.png");

	gDXSprite = new DX9Sprite;
	gDXSprite->Create(gDXBase->GetDevice(), wszBaseDir);
	gDXSprite->MakeSprite(L"advnt_full.png", 10, 10, 1.5f);
	gDXSprite->AddAnimation(DX9ANIMID::Idle, 0, 0);
	gDXSprite->AddAnimation(DX9ANIMID::Walk, 1, 5);
	gDXSprite->AddAnimation(DX9ANIMID::Attack1, 27, 28); // Punch
	gDXSprite->AddAnimation(DX9ANIMID::Attack2, 24, 26); // HorzAttack
	gSprGroundOffsetY = (float)(WINDOW_H - gDXSprite->GetSpriteScaledH() - TILE_H);
	gDXSprite->SetPosition(D3DXVECTOR2(400.0f, gSprGroundOffsetY - 32.0f));
	gDXSprite->SetBoundingnBox(D3DXVECTOR2(-24, -18));

	gDXMonster = new DX9Monster;
	gDXMonster->Create(gDXBase->GetDevice(), wszBaseDir);
	gDXMonster->MakeSprite(L"mage-1-85x94.png", 4, 2);
	gDXMonster->AddAnimation(DX9ANIMID::Idle, 0, 7);
	gDXMonster->SetPosition(D3DXVECTOR2(10.0f, (float)(WINDOW_H - gDXMonster->GetSpriteScaledH() - TILE_H)));

	gDXEffect = new DX9Effect;
	gDXEffect->Create(gDXBase->GetDevice(), wszBaseDir);
	gDXEffect->MakeSprite(L"particlefx_14.png", 8, 8);
	gDXEffect->AddAnimation(DX9ANIMID::Effect, 0, 63);
	gDXEffect->SetBaseSpawnOffset(D3DXVECTOR2(80.0f, 0.0f));

	gDXMap = new DX9Map;
	gDXMap->Create(gDXBase->GetDevice(), wszBaseDir);
	gDXMap->LoadMapFromFile(L"map01.jwm");
	gMapGroundOffsetY = (float)(-gDXMap->GetHeight() + WINDOW_H);
	gDXMap->SetPosition(0, gMapGroundOffsetY);

	gTimerStart = GetTickCount64();

	// 메인 루프 실행
	gDXBase->Run(MainLoop);
	
	// 종료 전 객체 파괴
	gDXMap->Destroy();
	gDXEffect->Destroy();
	gDXMonster->Destroy();
	gDXSprite->Destroy();
	gDXImage->Destroy();
	gDXInput->Destroy();
	gDXBase->Destroy();

	delete gDXMap;
	delete gDXEffect;
	delete gDXMonster;
	delete gDXSprite;
	delete gDXImage;
	delete gDXInput;
	delete gDXBase;

	return 0;
}

int MainLoop()
{
	// FPS용 타이머
	if (GetTickCount64() >= gTimerSec + 1000)
	{
		gTimerSec = GetTickCount64();
		ULONGLONG tTime = (gTimerSec - gTimerStart) / 1000;
		std::cout << "Time: " << tTime << " // FPS: " << gFPS << std::endl;
		gFPS = 0;
	}

	// 애니메이션용 타이머
	if (GetTickCount64() >= gTimerAnim + 80)
	{
		gTimerAnim = GetTickCount64();
		gDXMonster->Animate();
		gDXSprite->Animate();
	}

	gDXEffect->Animate();
	if (!gDXEffect->IsBeingAnimated())
		gbEffect = false;

	DetectInput();
	Gravitate();

	gDXSprite->MoveWithVelocity();

	gDXBase->BeginRender();

		gDXImage->Draw();

		gDXMap->Draw();

		gDXMonster->Draw();
		gDXMonster->DrawBoundingBox();

		gDXSprite->Draw();
		gDXSprite->DrawBoundingBox();

		if (gbEffect)
		{
			gDXEffect->Draw();
			gDXEffect->DrawBoundingBox();
		}

	gDXBase->EndRender();

	gFPS++;

	return 0;
}

int Gravitate()
{
	gDXSprite->Accelerate(kGravity);
	D3DXVECTOR2 tCurrSprVel = gDXSprite->GetVelocity();
	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(gDXSprite->GetBoundingBox(), tCurrSprVel);

	if (tNewVel.y < tCurrSprVel.y)
	{
		if (gbHitGround)
		{
			tNewVel.y = 0;
		}
		else
		{
			gbHitGround = true;
			gDXSprite->SetFrame(19); // 착지
		}	
	}
	else
	{
		if (tCurrSprVel.y < 0)
			gDXSprite->SetFrame(17); // 점프 중

		if (tCurrSprVel.y > 0)
			gDXSprite->SetFrame(18); // 낙하 중

		gbHitGround = false;
	}
	
	gDXSprite->SetVelocity(tNewVel);

	return 0;
}

int Jump()
{
	D3DXVECTOR2 tCurrSprVel = gDXSprite->GetVelocity();
	if ((gbHitGround == false) || (tCurrSprVel.y > 0)) // 현재 낙하 중
		return -1;

	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(gDXSprite->GetBoundingBox(), kJumpPow);
	gDXSprite->SetVelocity(tNewVel);
	gbHitGround = false;
	return 0;
}

int MoveSprite(D3DXVECTOR2 Velocity)
{
	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(gDXSprite->GetBoundingBox(), Velocity);
	gDXSprite->MoveConst(tNewVel);
	std::cout << gDXSprite->GetBoundingBox().PosOffset.x << " / "
		<< gDXSprite->GetBoundingBox().PosOffset.x + gDXSprite->GetBoundingBox().Size.x
		<< std::endl;

	return 0;
}

int DetectInput()
{
	gbWalking = false;

	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
	{
		MoveSprite(D3DXVECTOR2(kStride, 0));
		gDXSprite->SetAnimation(DX9ANIMID::Walk, true);
		gDXSprite->SetAnimDir(DX9ANIMDIR::Right);
		gbWalking = true;
	}
	if (gDXInput->OnKeyDown(DIK_LEFTARROW))
	{
		MoveSprite(D3DXVECTOR2(-kStride, 0));
		gDXSprite->SetAnimation(DX9ANIMID::Walk, true);
		gDXSprite->SetAnimDir(DX9ANIMDIR::Left);
		gbWalking = true;
	}
	if (gDXInput->OnKeyDown(DIK_LCONTROL))
	{
		gDXSprite->SetAnimation(DX9ANIMID::Attack1);
	}
	if (gDXInput->OnKeyDown(DIK_LALT))
	{
		gDXEffect->Spawn(gDXSprite->GetCenterPosition(), gDXSprite->GetAnimDir());
		gbEffect = true;
	}
	if (gDXInput->OnKeyDown(DIK_UPARROW))
	{
		Jump();
	}
	if (gDXInput->OnKeyDown(DIK_DOWNARROW))
	{

	}
	if (gDXInput->OnKeyDown(DIK_ESCAPE))
		gDXBase->Halt();

	if ((!gbWalking) && (!gDXSprite->IsBeingAnimated()))
	{
		gDXSprite->SetAnimation(DX9ANIMID::Idle);
	}

	return 0;
}