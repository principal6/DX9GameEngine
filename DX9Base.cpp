#include "DX9Base.h"

LRESULT CALLBACK WndProcBase(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

DX9Base::DX9Base(){
	// 멤버 변수 초기화
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_BGColor = D3DCOLOR_XRGB(0, 0, 255);
}

int DX9Base::Create(CINT X, CINT Y, CINT Width, CINT Height){
	COLORRGB rBGColor = COLORRGB(255, 0, 255);

	if (CreateWND(L"Game", X, Y, Width, Height, DX9WINDOW_STYLE::OverlappedWindow, rBGColor)
		== NULL)
		return -1;

	if (InitD3D() == -1)
		return -1;

	return 0;
}

int DX9Base::CreateOnWindow(HWND hWnd) {
	COLORRGB rBGColor = COLORRGB(255, 0, 255);

	m_hWnd = hWnd;
	m_hInstance = GetModuleHandle(NULL);

	if (InitD3D() == -1)
		return -1;

	return 0;
}

int DX9Base::Destroy() {
	if (m_pD3DDevice != NULL)
		m_pD3DDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();

	return 0;
}

HWND DX9Base::CreateWND(const wchar_t* Name,
	CINT X, CINT Y, CINT Width, CINT Height,
	DX9WINDOW_STYLE WindowStyle, COLORRGB BackColor) {
	// 멤버 변수에 인스턴스 핸들 대입
	m_hInstance = GetModuleHandle(NULL);

	// 윈도우 클래스 등록
	WNDCLASS r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(BackColor.r, BackColor.g, BackColor.b));
	r_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = WndProcBase;
	r_WndClass.lpszClassName = Name;
	r_WndClass.lpszMenuName = NULL;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&r_WndClass);

	// 윈도우 정확한 픽셀 크기로 맞추기
	RECT rWndRect = { X, Y, X + Width, Y + Height };
	AdjustWindowRect(&rWndRect, (DWORD)WindowStyle, false);

	// 윈도우 생성
	m_hWnd = CreateWindow(Name, Name, (DWORD)WindowStyle,
		rWndRect.left, rWndRect.top,
		rWndRect.right - rWndRect.left, rWndRect.bottom - rWndRect.top,
		NULL, (HMENU)NULL, m_hInstance, NULL);

	// 윈도우 표시
	ShowWindow(m_hWnd, SW_SHOW);

	UnregisterClass(Name, m_hInstance);
	
	return m_hWnd;
}

int DX9Base::SetBGColor(D3DCOLOR color) {
	m_BGColor = color;
	return 0;
}

int DX9Base::Run(int(*pMainLoop)()) {

	while (m_MSG.message != WM_QUIT)
	{
		if (PeekMessage(&m_MSG, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
		}
		else
		{
			pMainLoop();
		}
	}

	return 0;
}

int DX9Base::RunWithAccel(int(*pMainLoop)(), HACCEL hAccel) {
	while (GetMessage(&m_MSG, 0, 0, 0)) {
		if (!TranslateAccelerator(m_hWnd, hAccel, &m_MSG)) {
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
			pMainLoop();
		}
	}
	return (int)m_MSG.wParam;
}

int DX9Base::Halt(){
	DestroyWindow(m_hWnd);

	return 0;
}

int DX9Base::InitD3D() {
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return -1;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &m_pD3DDevice)))
	{
		return -1;
	}

	return 0;
}

int DX9Base::Resize(HWND hWnd) {

	if (!m_pD3DDevice)
		return -1;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	D3DPP.hDeviceWindow = hWnd;

	m_pD3DDevice->Reset(&D3DPP);

	return 0;
}

int DX9Base::BeginRender() {
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_BGColor, 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		return 0;

	return -1;
}

int DX9Base::EndRender() {
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	return 0;
}