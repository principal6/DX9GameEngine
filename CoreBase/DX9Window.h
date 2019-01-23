#pragma once

#include "DX9Common.h"

namespace DX9ENGINE
{
	enum class EWindowStyle : DWORD
	{
		Overlapped = WS_OVERLAPPED,
		Popup = WS_POPUP,
		Child = WS_CHILD,
		Minimize = WS_MINIMIZE,
		Visible = WS_VISIBLE,
		DIsabled = WS_DISABLED,
		ClipSiblings = WS_CLIPSIBLINGS,
		ClipChildren = WS_CLIPCHILDREN,
		Maximize = WS_MAXIMIZE,
		Caption = WS_CAPTION, // = WS_BORDER | WS_DLGFRAME
		Border = WS_BORDER,
		DlgFrame = WS_DLGFRAME,
		VScroll = WS_VSCROLL,
		HScroll = WS_HSCROLL,
		SysMenu = WS_SYSMENU,
		ThickFrame = WS_THICKFRAME,
		Group = WS_GROUP,
		TabStop = WS_TABSTOP,
		MinimizeBox = WS_MINIMIZEBOX,
		MaximizeBox = WS_MAXIMIZEBOX,
		OverlappedWindow = WS_OVERLAPPEDWINDOW, // = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		PopupWindow = WS_POPUPWINDOW, // = WS_POPUP | WS_BORDER | WS_SYSMENU
		ChildWindow = WS_CHILDWINDOW, // = WS_CHILD
		ChildWindow2 = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
	};

	struct SWindowData
	{
		int WindowWidth;
		int WindowHeight;
		float WindowHalfWidth;
		float WindowHalfHeight;

		SWindowData() : WindowWidth(0), WindowHeight(0), WindowHalfWidth(0), WindowHalfHeight(0) {};
	};

	struct SMouseData
	{
		POINT MousePosition;
		POINT MouseDownPosition;
		bool bMouseLeftButtonPressed;
		bool bMouseRightButtonPressed;
		bool bOnMouseMove;

		SMouseData() : bMouseLeftButtonPressed(false), bOnMouseMove(false) {};
	};

	class DX9Window final
	{
	public:
		DX9Window();
		~DX9Window() {};

		auto DX9Window::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height)->EError;
		auto DX9Window::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height, DWORD Color,
			WNDPROC Proc, LPCWSTR MenuName)->EError;
		auto DX9Window::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height,
			DWORD Color, WNDPROC Proc)->EError;
		void DX9Window::Destroy();

		void DX9Window::SetWindowCaption(WSTRING Caption);
		void DX9Window::SetBackgroundColor(D3DCOLOR color);
		void DX9Window::Resize();

		void DX9Window::BeginRender() const;
		void DX9Window::EndRender() const;

		auto DX9Window::GetDevice()->LPDIRECT3DDEVICE9 const;
		auto DX9Window::GethWnd()->HWND;
		auto DX9Window::GethInstance()->HINSTANCE;
		auto DX9Window::GetWindowData()->SWindowData*;
		auto DX9Window::GetMouseData()->SMouseData*;

		// Dialog
		void DX9Window::SetDlgBase();
		auto DX9Window::OpenFileDlg(LPCWSTR Filter)->BOOL;
		auto DX9Window::SaveFileDlg(LPCWSTR Filter)->BOOL;
		auto DX9Window::GetDlgFileName()->WSTRING;
		auto DX9Window::GetDlgFileTitle()->WSTRING;

		// Editor message handler
		void DX9Window::EditorChildWindowMessageHandler(UINT Message, WPARAM wParam, LPARAM lParam);
		auto DX9Window::IsMouseLeftButtonPressed() const->bool;
		auto DX9Window::IsMouseRightButtonPressed() const->bool;
		auto DX9Window::OnMouseMove()->bool;

	private:
		auto DX9Window::CreateWINAPIWindow(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
			EWindowStyle WindowStyle, DWORD BackColor, WNDPROC Proc, LPCWSTR MenuName = nullptr, HWND hWndParent = nullptr)->HWND;
		auto DX9Window::InitializeDirectX()->int;
		void DX9Window::SetDirect3DParameters();

	private:
		static int ms_ChildWindowCount;
		
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		RECT m_Rect;
		SWindowData m_WindowData;

		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pDevice;
		D3DPRESENT_PARAMETERS m_D3DPP;
		D3DCOLOR m_BGColor;

		OPENFILENAME m_OFN;
		TCHAR m_FileName[MAX_FILE_LEN];
		TCHAR m_FileTitle[MAX_FILE_LEN];

		SMouseData m_MouseData;
	};
};