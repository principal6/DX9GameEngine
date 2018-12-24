#include "DX9Base.h"
#include "DX9Image.h"
#include "DX9Input.h"
#include "DX9Sprite.h"
#include "DX9Map.h"

int MainLoop();
int DetectInput();

const int gWndW = 800;
const int gWndH = 600;
wchar_t gBaseDir[255] = { 0 };
ULONGLONG	gTimerSec = 0;
ULONGLONG	gTimerAnim = 0;
int			gFPS = 0;

DX9Base* gDXBase;
DX9Input* gDXInput;
DX9Image* gDXImage;
DX9Sprite* gDXSprite;
DX9Map* gDXMap;

float gSprX = 0.0f;
float gSprY = 0.0f;

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
	gDXSprite->SetTexture(L"advnt_full.png");
	gDXSprite->SetNumRowsAndCols(10, 10);
	gDXSprite->SetScale(2.0f, 2.0f);
	gDXSprite->AddAnimation(0, 0, 0);
	gDXSprite->AddAnimation(1, 0, 0, true);
	gDXSprite->AddAnimation(2, 1, 5);
	gDXSprite->AddAnimation(3, 1, 5, true);
	gSprGroundOffsetY = (float)(gWndH - gDXSprite->GetSpriteH() - TILE_H);
	gSprY = gSprGroundOffsetY;

	gDXMap = new DX9Map;
	gDXMap->Create(gDXBase->GetDevice(), gBaseDir);
	gDXMap->LoadMapFromFile(L"test.jwm");
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
		wchar_t tempstr[20];
		_itow_s(gFPS, tempstr, 10);
		OutputDebugString(tempstr);
		gFPS = 0;
	}

	// 애니메이션용 타이머
	if (GetTickCount64() >= gTimerAnim + 50)
	{
		gTimerAnim = GetTickCount64();

		gDXSprite->Animate();
	}

	DetectInput();

	gDXBase->BeginRender();

		gDXImage->Draw();

		gDXMap->Draw();

		gDXSprite->SetPosition(gSprX, gSprY);
		gDXSprite->Draw();

	gDXBase->EndRender();

	gFPS++;

	return 0;
}

int DetectInput() {
	bool bSpriteIdle = true;

	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
	{
		bSpriteIdle = false;
		gSprX += 3.0f;
		gDXSprite->SetAnimation(2);
	}
	if (gDXInput->OnKeyDown(DIK_LEFTARROW))
	{
		bSpriteIdle = false;
		gSprX -= 3.0f;
		gDXSprite->SetAnimation(3);
	}
	if (gDXInput->OnKeyDown(DIK_LALT))
		gSprY += 2.0f;
	if (gDXInput->OnKeyDown(DIK_UPARROW))
		gSprY -= 2.0f;
	if (gDXInput->OnKeyDown(DIK_DOWNARROW))
		gSprY += 2.0f;
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