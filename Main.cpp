#include "DX9Base.h"
#include "DX9Image.h"
#include "DX9Input.h"
#include "DX9Sprite.h"

int MainLoop();
int DetectInput();

DX9Base* gDXBase;
DX9Input* gDXInput;
DX9Image* gDXImage;
DX9Sprite* gDXSprite;

float gSprX = 0.0f;
float gSprY = 0.0f;

int main() {
	gDXBase = new DX9Base;
	gDXBase->Create(0, 0, 800, 600);

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

	// 메인 루프 실행
	gDXBase->Run(MainLoop);
	
	// 종료 전 객체 파괴
	gDXSprite->Destroy();
	gDXImage->Destroy();
	gDXInput->Destroy();
	gDXBase->Destroy();

	delete gDXSprite;
	delete gDXImage;
	delete gDXInput;
	delete gDXBase;

	return 0;
}

int MainLoop() {
	DetectInput();

	gDXBase->BeginRender();

		gDXImage->Draw();

		gDXSprite->SetPosition(gSprX, gSprY);
		gDXSprite->Draw();

	gDXBase->EndRender();

	return 0;
}

int DetectInput() {
	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
		gSprX += 2.0f;
	if (gDXInput->OnKeyDown(DIK_LEFTARROW))
		gSprX -= 2.0f;
	if (gDXInput->OnKeyDown(DIK_UPARROW))
		gSprY -= 2.0f;
	if (gDXInput->OnKeyDown(DIK_DOWNARROW))
		gSprY += 2.0f;
	if (gDXInput->OnKeyDown(DIK_ESCAPE))
		gDXBase->Halt();

	return 0;
}