#include "DX9Base.h"

LRESULT CALLBACK WndProcBase(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) // Disable Alt key
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

DX9Base::DX9Base()
{
	m_hInstance = nullptr;
	m_hWnd = nullptr;

	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_BGColor = D3DCOLOR_XRGB(0, 0, 255);
}

bool DX9Base::Create(CINT X, CINT Y, DX9SHARE_DATA* pData)
{
	m_pShareData = pData;

	COLOR_RGB rBGColor = COLOR_RGB(255, 0, 255);

	if (CreateWND(L"Game", X, Y, m_pShareData->WindowWidth, m_pShareData->WindowHeight, DX9WINDOW_STYLE::OverlappedWindow, rBGColor)
		== nullptr)
		return false;

	if (InitD3D() == -1)
		return false;

	return true;
}

void DX9Base::CreateOnWindow(HWND hWnd)
{
	COLOR_RGB rBGColor = COLOR_RGB(255, 0, 255);

	m_hWnd = hWnd;
	m_hInstance = GetModuleHandle(nullptr);

	if (InitD3D() == -1)
		return;
}

void DX9Base::Destroy()
{
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}	

	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

HWND DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
	DX9WINDOW_STYLE WindowStyle, COLOR_RGB BackColor)
{
	m_hInstance = GetModuleHandle(nullptr);

	WNDCLASS r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(BackColor.r, BackColor.g, BackColor.b));
	r_WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = WndProcBase;
	r_WndClass.lpszClassName = Name;
	r_WndClass.lpszMenuName = nullptr;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&r_WndClass);

	RECT rWndRect = { X, Y, X + Width, Y + Height };
	AdjustWindowRect(&rWndRect, (DWORD)WindowStyle, false);

	m_hWnd = CreateWindow(Name, Name, (DWORD)WindowStyle, rWndRect.left, rWndRect.top,
		rWndRect.right - rWndRect.left, rWndRect.bottom - rWndRect.top, nullptr, (HMENU)nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	UnregisterClass(Name, m_hInstance);
	
	return m_hWnd;
}

void DX9Base::SetBackgroundColor(D3DCOLOR color)
{
	m_BGColor = color;
}

void DX9Base::Halt()
{
	DestroyWindow(m_hWnd);
}

int DX9Base::InitD3D()
{
	if (nullptr == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
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

void DX9Base::Resize(HWND hWnd)
{
	if (!m_pD3DDevice)
		return;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	D3DPP.hDeviceWindow = hWnd;

	m_pD3DDevice->Reset(&D3DPP);
}

void DX9Base::BeginRender() const
{
	m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_BGColor, 1.0f, 0);
	m_pD3DDevice->BeginScene();
}

void DX9Base::EndRender() const
{
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

LPDIRECT3DDEVICE9 DX9Base::GetDevice() const
{
	return m_pD3DDevice;
}

HINSTANCE DX9Base::GetInstance() const
{ 
	return m_hInstance;
}

HWND DX9Base::GetHWND() const
{ 
	return m_hWnd;
}