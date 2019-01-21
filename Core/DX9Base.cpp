#include "DX9Base.h"

using namespace DX9ENGINE;

// Static member variable
int DX9Base::ms_ChildWindowCount = 0;

// Window procedure for Game Window
LRESULT CALLBACK GameWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
	m_hWnd = nullptr;
	m_pD3DDevice = nullptr;
	m_BGColor = D3DCOLOR_XRGB(0, 0, 255);
}

auto DX9Base::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height)->Error
{
	if (CreateWND(L"Game", X, Y, Width, Height, WindowStyle::OverlappedWindow, m_BGColor, GameWindowProc)
		== nullptr)
		return Error::WINDOW_NOT_CREATED;

	if (InitD3D() == -1)
		return Error::DIRECTX_NOT_CREATED;

	return Error::OK;
}

auto DX9Base::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height, DWORD Color,
	WNDPROC Proc, LPCWSTR MenuName)->Error
{
	if (CreateWND(L"Editor", X, Y, Width, Height, WindowStyle::OverlappedWindow, Color, Proc, MenuName)
		== nullptr)
		return Error::WINDOW_NOT_CREATED;

	return Error::OK;

}

auto DX9Base::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height,
	DWORD Color, WNDPROC Proc)->Error
{
	// Set DirectX clear color
	m_BGColor = Color;

	WSTRING Name = L"EditorChild";
	wchar_t temp[MAX_FILE_LEN] {};
	_itow_s(ms_ChildWindowCount, temp, 10);
	Name += temp;

	ms_ChildWindowCount++;

	if (CreateWND(Name.c_str(), X, Y, Width, Height, WindowStyle::ChildWindow2, Color, Proc, nullptr, hWndParent)
		== nullptr)
		return Error::WINDOW_NOT_CREATED;

	if (InitD3D() == -1)
		return Error::DIRECTX_NOT_CREATED;

	return Error::OK;
}

void DX9Base::Destroy()
{
	DX_RELEASE(m_pD3DDevice);
	DX_RELEASE(m_pD3D);
}

auto DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
	WindowStyle WindowStyle, DWORD BackColor, WNDPROC Proc, LPCWSTR MenuName, HWND hWndParent)->HWND
{
	m_hInstance = GetModuleHandle(nullptr);
	
	// Set window data
	m_WindowData.WindowWidth = Width;
	m_WindowData.WindowHeight = Height;
	m_WindowData.WindowHalfWidth = static_cast<float>(Width / 2.0f);
	m_WindowData.WindowHalfHeight = static_cast<float>(Height / 2.0f);

	WNDCLASS r_WndClass;
	r_WndClass.cbClsExtra = 0;
	r_WndClass.cbWndExtra = 0;
	r_WndClass.hbrBackground = CreateSolidBrush(RGB(GetColorR(BackColor), GetColorG(BackColor), GetColorB(BackColor)));
	r_WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	r_WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	r_WndClass.hInstance = m_hInstance;
	r_WndClass.lpfnWndProc = Proc;
	r_WndClass.lpszClassName = Name;
	r_WndClass.lpszMenuName = MenuName;
	r_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&r_WndClass);

	RECT rWndRect = { X, Y, X + Width, Y + Height };
	AdjustWindowRect(&rWndRect, (DWORD)WindowStyle, false);

	m_hWnd = CreateWindow(Name, Name, (DWORD)WindowStyle, rWndRect.left, rWndRect.top,
		rWndRect.right - rWndRect.left, rWndRect.bottom - rWndRect.top, hWndParent, (HMENU)nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	UnregisterClass(Name, m_hInstance);
	
	return m_hWnd;
}

void DX9Base::SetBackgroundColor(D3DCOLOR color)
{
	m_BGColor = color;
}

auto DX9Base::InitD3D()->int
{
	if (nullptr == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return -1;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	D3DPP.hDeviceWindow = m_hWnd;
	D3DPP.BackBufferWidth = m_WindowData.WindowWidth;
	D3DPP.BackBufferHeight = m_WindowData.WindowHeight;

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &m_pD3DDevice)))
	{
		return -1;
	}

	return 0;
}

void DX9Base::Resize()
{
	if (!m_pD3DDevice)
		return;

	D3DPRESENT_PARAMETERS D3DPP;
	ZeroMemory(&D3DPP, sizeof(D3DPP));
	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	D3DPP.hDeviceWindow = m_hWnd;

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

auto DX9Base::GetDevice()->LPDIRECT3DDEVICE9 const
{
	return m_pD3DDevice;
}

auto DX9Base::GethWnd()->HWND
{
	return m_hWnd;
}

auto DX9Base::GethInstance()->HINSTANCE
{
	return m_hInstance;
}

auto DX9Base::GetWindowData()->WindowData*
{
	return &m_WindowData;
}


//
// Dialog functions
//
void DX9Base::SetDlgBase()
{
	ZeroMemory(&m_OFN, sizeof(m_OFN));
	m_OFN.lStructSize = sizeof(m_OFN);
	m_OFN.hwndOwner = m_hWnd;
	m_OFN.lpstrFile = m_FileName;
	m_OFN.nMaxFile = sizeof(m_FileName);
	m_OFN.lpstrFileTitle = m_FileTitle;
	m_OFN.nMaxFileTitle = sizeof(m_FileTitle);
	m_OFN.nFilterIndex = 1;
	m_OFN.lpstrFileTitle = NULL;
	m_OFN.nMaxFileTitle = 0;
	m_OFN.lpstrInitialDir = NULL;
	m_OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

auto DX9Base::OpenFileDlg(LPCWSTR Filter)->BOOL
{
	SetDlgBase();
	m_OFN.lpstrFilter = Filter;

	return GetOpenFileName(&m_OFN);
}

auto DX9Base::SaveFileDlg(LPCWSTR Filter)->BOOL
{
	SetDlgBase();
	m_OFN.lpstrFilter = Filter;

	return GetSaveFileName(&m_OFN);
}

auto DX9Base::GetDlgFileName()->WSTRING
{
	return m_FileName;
}

auto DX9Base::GetDlgFileTitle()->WSTRING
{
	WSTRING tempString = m_FileName;

	size_t temp = tempString.find_last_of('\\');

	if (temp)
		tempString = tempString.substr(temp + 1);

	return tempString;
}