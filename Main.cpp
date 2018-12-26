#include "DX9Base.h"
#include "DX9Image.h"
#include "DX9Input.h"
#include "DX9Sprite.h"
#include "DX9Map.h"
#include "DX9Line.h"

int MainLoop();
int DetectInput();
int Gravitate();
int MoveSprite(D3DXVECTOR2 Velocity);

const int gWndW = 800;
const int gWndH = 600;
const D3DXVECTOR2 gGravity = D3DXVECTOR2(0.0f, 0.3f);
const float gStride = 4.0f;
wchar_t gBaseDir[255] = { 0 };
bool gbHitGround = false;

ULONGLONG	gTimerSec = 0;
ULONGLONG	gTimerAnim = 0;
int			gFPS = 0;

DX9Base*	gDXBase;
DX9Input*	gDXInput;
DX9Image*	gDXImage;
DX9Sprite*	gDXSprite;
DX9Map*		gDXMap;

float gSprGroundOffsetY = 0.0f;
float gMapGroundOffsetY = 0.0f;

int main() {
	GetCurrentDirectory(255, gBaseDir);

	gDXBase = new DX9Base;
	gDXBase->Create(50, 50, gWndW, gWndH);

	gDXInput = new DX9Input;
	gDXInput->Create(gDXBase->GetInstance(), gDXBase->GetHWND());

	gDXImage = new DX9Image;
	gDXImage->Create(gDXBase->GetDevice(), gBaseDir);
	gDXImage->SetTexture(L"bg.png");

	gDXSprite = new DX9Sprite;
	gDXSprite->Create(gDXBase->GetDevice(), gBaseDir);
	gDXSprite->MakeSprite(L"advnt_full.png", 10, 10, 2.0f);
	gDXSprite->AddAnimation(0, 0, 0);
	gDXSprite->AddAnimation(1, 0, 0, true);
	gDXSprite->AddAnimation(2, 1, 5);
	gDXSprite->AddAnimation(3, 1, 5, true);
	gSprGroundOffsetY = (float)(gWndH - gDXSprite->GetSpriteH() - TILE_H);
	gDXSprite->SetPosition(D3DXVECTOR2(400.0f, gSprGroundOffsetY - 32.0f));
	gDXSprite->SetBoundingnBox(D3DXVECTOR2(-34, -30));

	gDXMap = new DX9Map;
	gDXMap->Create(gDXBase->GetDevice(), gBaseDir);
	gDXMap->LoadMapFromFile(L"map01.jwm");
	gMapGroundOffsetY = (float)(-gDXMap->GetHeight() + gWndH);
	gDXMap->SetPosition(0, gMapGroundOffsetY);

	// 메인 루프 실행
	gDXBase->Run(MainLoop);
	
	// 종료 전 객체 파괴
	gDXMap->Destroy();
	gDXSprite->Destroy();
	gDXImage->Destroy();
	gDXInput->Destroy();
	gDXBase->Destroy();

	delete gDXMap;
	delete gDXSprite;
	delete gDXImage;
	delete gDXInput;
	delete gDXBase;

	return 0;
}

int MainLoop() {
	// FPS용 타이머
	if (GetTickCount64() >= gTimerSec + 1000)
	{
		gTimerSec = GetTickCount64();
		//std::cout << gFPS << std::endl;
		gFPS = 0;
	}

	// 애니메이션용 타이머
	if (GetTickCount64() >= gTimerAnim + 50)
	{
		gTimerAnim = GetTickCount64();
		gDXSprite->Animate();
	}

	DetectInput();
	Gravitate();

	gDXSprite->MoveWithVelocity();

	gDXBase->BeginRender();

		gDXImage->Draw();

		gDXMap->Draw();

		gDXSprite->Draw();
		gDXSprite->DrawBoundingBox();

	gDXBase->EndRender();

	gFPS++;

	return 0;
}

int Gravitate() {
	gDXSprite->Accelerate(gGravity);
	D3DXVECTOR2 SprVel = gDXSprite->GetVelocity();
	DX9BOUNDINGBOX tSprBB = gDXSprite->GetBoundingBox();

	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(tSprBB, SprVel);
	
	if (tNewVel.y < SprVel.y)
	{
		if (gbHitGround)
			tNewVel.y = 0;
		gbHitGround = true;
	}
	else
	{
		gbHitGround = false;
	}
	
	gDXSprite->SetVelocity(tNewVel);

	std::cout << "Gravitate: " << tNewVel.x << " / " << tNewVel.y << std::endl;

	return 0;
}

int Jump() {
	D3DXVECTOR2 CurrSprVel = gDXSprite->GetVelocity();

	if (gbHitGround == false)
		return -1;

	if (CurrSprVel.y > 0)
		return -1;

	DX9BOUNDINGBOX tSprBB = gDXSprite->GetBoundingBox();
	D3DXVECTOR2 JumpVel = D3DXVECTOR2(0.0f, -10.0f);
	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(tSprBB, JumpVel);
	gDXSprite->SetVelocity(tNewVel);
	gbHitGround = false;

	return 0;
}

int MoveSprite(D3DXVECTOR2 Velocity) {
	DX9BOUNDINGBOX tSprBB = gDXSprite->GetBoundingBox();
	D3DXVECTOR2 tNewVel = gDXMap->GetVelocityAfterCollision(tSprBB, Velocity);
	gDXSprite->MoveOnce(tNewVel);

	std::cout << "MoveSprite: " << tNewVel.x << " / "<< tNewVel.y << std::endl;
	return 0;
}

int DetectInput() {
	bool bSpriteIdle = true;

	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
	{
		bSpriteIdle = false;
		MoveSprite(D3DXVECTOR2(gStride, 0));
		gDXSprite->SetAnimation(2);
	}
	if (gDXInput->OnKeyDown(DIK_LEFTARROW))
	{
		bSpriteIdle = false;
		MoveSprite(D3DXVECTOR2(-gStride, 0));
		gDXSprite->SetAnimation(3);
	}
	if (gDXInput->OnKeyDown(DIK_LALT))
	{

	}
	if (gDXInput->OnKeyDown(DIK_UPARROW))
	{
		Jump();
		//MoveSprite(D3DXVECTOR2(0, -gStride));
	}
	if (gDXInput->OnKeyDown(DIK_DOWNARROW))
	{
		//MoveSprite(D3DXVECTOR2(0, gStride));
	}
	if (gDXInput->OnKeyDown(DIK_ESCAPE))
		gDXBase->Halt();

	if (bSpriteIdle)
	{
		if (gDXSprite->GetSpriteDir())
		{
			gDXSprite->SetAnimation(1);
		}
		else
		{
			gDXSprite->SetAnimation(0);
		}
	}

	return 0;
}