#include "DX9Base.h"
#include "DX9Image.h"
#include "DX9Input.h"
#include "DX9Sprite.h"
#include "DX9Map.h"

int MainLoop();
int DetectInput();

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

int main() {
	gDXBase = new DX9Base;
	gDXBase->Create(50, 50, 800, 600);

	gDXInput = new DX9Input;
	gDXInput->Create(gDXBase->GetInstance(), gDXBase->GetHWND());

	gDXImage = new DX9Image;
	gDXImage->Create(gDXBase->GetDevice());
	gDXImage->SetSize(960.0f, 672.0f);
	gDXImage->SetTexture(L"bg.png");

	gDXSprite = new DX9Sprite;
	gDXSprite->Create(gDXBase->GetDevice());
	gDXSprite->SetSize(32.0f, 64.0f);
	gDXSprite->SetScale(2.0f, 2.0f);
	gDXSprite->SetTexture(L"advnt_full.png");
	gDXSprite->SetNumRowsAndCols(10, 10);
	gDXSprite->AddAnimation(0, 0, 0);
	gDXSprite->AddAnimation(1, 0, 0, true);
	gDXSprite->AddAnimation(2, 1, 5);
	gDXSprite->AddAnimation(3, 1, 5, true);

	gDXMap = new DX9Map;
	gDXMap->Create(gDXBase->GetDevice());
	gDXMap->SetTexture(L"maptile32x32.png");
	gDXMap->SetTileInfo(32.0f, 32.0f, 17, 6);
	gDXMap->AddMapFragment(2, 0.0f, 568.0f);
	gDXMap->AddMapFragment(2, 32.0f, 568.0f);
	gDXMap->AddMapFragment(2, 64.0f, 568.0f);
	gDXMap->AddMapFragment(2, 96.0f, 568.0f);
	gDXMap->AddEnd();

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