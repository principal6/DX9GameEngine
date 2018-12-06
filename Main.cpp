#include "DX9Base.h"
#include "DX9Image.h"
#include "DX9Input.h"

int MainLoop();

DX9Base* gDXBase;
DX9Input* gDXInput;
DX9Image* gDXImage;

float gSprX = 0.0f;
float gSprY = 0.0f;

int main(){
	gDXBase = new DX9Base;
	gDXBase->Create(0, 0, 800, 600);

	gDXInput = new DX9Input;
	gDXInput->Create(gDXBase->GetInstance(), gDXBase->GetHWND());

	gDXImage = new DX9Image;
	gDXImage->Create(gDXBase->GetDevice());
	gDXImage->SetSize(150.0f, 150.0f);
	gDXImage->SetTexture(L"Wony.png");

	// 메인 루프 실행
	gDXBase->Run(MainLoop);
	
	// 종료 전 객체 파괴
	gDXImage->Destroy();
	gDXInput->Destroy();
	gDXBase->Destroy();

	delete gDXImage;
	delete gDXInput;
	delete gDXBase;

	return 0;
}

int MainLoop() {
	gDXBase->BeginRender();

	if (gDXInput->OnKeyDown(DIK_RIGHTARROW))
		gSprX += 1.0f;
	if (gDXInput->OnKeyDown(DIK_ESCAPE))
		gDXBase->Halt();

	gDXImage->SetPosition(gSprX, gSprY);
	gDXImage->Draw();

	gDXBase->EndRender();

	return 0;
}