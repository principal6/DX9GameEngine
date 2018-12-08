#include "DX9Base.h"

DX9Base::DX9Base(){
	// 멤버 변수 초기화
	mpD3D = NULL;
	mpD3DDevice = NULL;
}

int DX9Base::Create(CINT X, CINT Y, CINT Width, CINT Height){
	DX9COLOR rBGColor = DX9COLOR(255, 0, 255);

	if (CreateWND(L"Game", X, Y, Width, Height, DX9WINDOW_STYLE::OverlappedWindow, rBGColor)
		== NULL)
		return -1;

	if (InitD3D() == -1)
		return -1;

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

int DX9Base::Destroy() {
	if (mpD3DDevice != NULL)
		mpD3DDevice->Release();

	if (mpD3D != NULL)
		mpD3D->Release();

	return 0;
}

HWND DX9Base::CreateWND(const wchar_t* Name,
	CINT X, CINT Y, CINT Width, CINT Height,
	DX9WINDOW_STYLE WindowStyle, DX9COLOR BackColor) {
	// 멤버 변수에 인스턴스 핸들 대입
	mhInstance = GetModuleHandle(NULL);;

	// 윈도우 클래스 등록
	WNDCLASS r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(BackColor.r, BackColor.g, BackColor.b));
	r_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	r_WndClass.hInstance = mhInstance;
	r_WndClass.lpfnWndProc = WndProc;
	r_WndClass.lpszClassName = Name;
	r_WndClass.lpszMenuName = NULL;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&r_WndClass);

	// 윈도우 생성
	mhWnd = CreateWindow(Name, Name, (DWORD)WindowStyle, X, Y, Width, Height,
		NULL, (HMENU)NULL, mhInstance, NULL);

	// 윈도우 표시
	ShowWindow(mhWnd, SW_SHOW);

	UnregisterClass(Name, mhInstance);
	
	return mhWnd;
}

int DX9Base::Run(int(*pMainLoop)()) {

	while (mMSG.message != WM_QUIT)
	{
		if (PeekMessage(&mMSG, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&mMSG);
			DispatchMessage(&mMSG);
		}
		else
		{
			pMainLoop();
		}
	}

	return 0;
}

int DX9Base::Halt(){
	DestroyWindow(mhWnd);

	return 0;
}

int DX9Base::InitD3D() {
	if (NULL == (mpD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return -1;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(mpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &mpD3DDevice)))
	{
		return -1;
	}

	return 0;
}

int DX9Base::BeginRender() {
	mpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(mpD3DDevice->BeginScene()))
		return 0;

	return -1;
}

int DX9Base::EndRender() {
	mpD3DDevice->EndScene();
	mpD3DDevice->Present(NULL, NULL, NULL, NULL);

	return 0;
}