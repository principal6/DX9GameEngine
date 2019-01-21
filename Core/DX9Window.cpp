#include "DX9Window.h"

using namespace DX9ENGINE;

// Static member variable
int DX9Window::ms_ChildWindowCount = 0;

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

DX9Window::DX9Window()
{
	m_hInstance = nullptr;
	m_hWnd = nullptr;
	m_pD3D = nullptr;
	m_pDevice = nullptr;
	m_BGColor = D3DCOLOR_XRGB(255, 0, 0);

	memset(m_FileName, 0, MAX_FILE_LEN);
	memset(m_FileTitle, 0, MAX_FILE_LEN);
}

auto DX9Window::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height)->EError
{
	if (CreateWINAPIWindow(L"Game", X, Y, Width, Height, EWindowStyle::OverlappedWindow, m_BGColor, GameWindowProc)
		== nullptr)
		return EError::WINAPIWINDOW_NOT_CREATED;

	if (InitializeDirectX() == -1)
		return EError::DIRECTX_NOT_CREATED;

	return EError::OK;
}

auto DX9Window::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height, DWORD Color,
	WNDPROC Proc, LPCWSTR MenuName)->EError
{
	if (CreateWINAPIWindow(L"Editor", X, Y, Width, Height, EWindowStyle::OverlappedWindow, Color, Proc, MenuName)
		== nullptr)
		return EError::WINAPIWINDOW_NOT_CREATED;

	return EError::OK;

}

auto DX9Window::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height,
	DWORD Color, WNDPROC Proc)->EError
{
	// Set DirectX clear color
	m_BGColor = Color;

	WSTRING Name = L"EditorChild";
	wchar_t temp[MAX_FILE_LEN] {};
	_itow_s(ms_ChildWindowCount, temp, 10);
	Name += temp;

	ms_ChildWindowCount++;

	if (CreateWINAPIWindow(Name.c_str(), X, Y, Width, Height, EWindowStyle::ChildWindow2, Color, Proc, nullptr, hWndParent)
		== nullptr)
		return EError::WINAPIWINDOW_NOT_CREATED;

	if (InitializeDirectX() == -1)
		return EError::DIRECTX_NOT_CREATED;

	return EError::OK;
}

auto DX9Window::CreateWINAPIWindow(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
	EWindowStyle WindowStyle, DWORD BackColor, WNDPROC Proc, LPCWSTR MenuName, HWND hWndParent)->HWND
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

	m_Rect = { X, Y, X + Width, Y + Height };
	AdjustWindowRect(&m_Rect, (DWORD)WindowStyle, false);

	m_hWnd = CreateWindow(Name, Name, (DWORD)WindowStyle, m_Rect.left, m_Rect.top,
		m_Rect.right - m_Rect.left, m_Rect.bottom - m_Rect.top, hWndParent, (HMENU)nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	UnregisterClass(Name, m_hInstance);
	
	return m_hWnd;
}

auto DX9Window::InitializeDirectX()->int
{
	if (nullptr == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return -1;

	SetDirect3DParameters();

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3DPP, &m_pDevice)))
	{
		return -1;
	}

	return 0;
}

void DX9Window::SetDirect3DParameters()
{
	ZeroMemory(&m_D3DPP, sizeof(m_D3DPP));
	m_D3DPP.Windowed = TRUE;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	m_D3DPP.hDeviceWindow = m_hWnd;

	//m_D3DPP.BackBufferWidth = m_WindowData.WindowWidth;
	//m_D3DPP.BackBufferHeight = m_WindowData.WindowHeight;
}

void DX9Window::Destroy()
{
	DX_RELEASE(m_pDevice);
	DX_RELEASE(m_pD3D);
}

void DX9Window::SetBackgroundColor(D3DCOLOR color)
{
	m_BGColor = color;
}

void DX9Window::Resize()
{
	if (m_pDevice)
	{
		SetDirect3DParameters();
		m_pDevice->Reset(&m_D3DPP);
	}
}

void DX9Window::BeginRender() const
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_BGColor, 1.0f, 0);
	m_pDevice->BeginScene();
}

void DX9Window::EndRender() const
{
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

auto DX9Window::GetDevice()->LPDIRECT3DDEVICE9 const
{
	return m_pDevice;
}

auto DX9Window::GethWnd()->HWND
{
	return m_hWnd;
}

auto DX9Window::GethInstance()->HINSTANCE
{
	return m_hInstance;
}

auto DX9Window::GetWindowData()->SWindowData*
{
	return &m_WindowData;
}


//
// Dialog functions
//
void DX9Window::SetDlgBase()
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

auto DX9Window::OpenFileDlg(LPCWSTR Filter)->BOOL
{
	SetDlgBase();
	m_OFN.lpstrFilter = Filter;

	return GetOpenFileName(&m_OFN);
}

auto DX9Window::SaveFileDlg(LPCWSTR Filter)->BOOL
{
	SetDlgBase();
	m_OFN.lpstrFilter = Filter;

	return GetSaveFileName(&m_OFN);
}

auto DX9Window::GetDlgFileName()->WSTRING
{
	return m_FileName;
}

auto DX9Window::GetDlgFileTitle()->WSTRING
{
	WSTRING tempString = m_FileName;

	size_t temp = tempString.find_last_of('\\');

	if (temp)
		tempString = tempString.substr(temp + 1);

	return tempString;
}