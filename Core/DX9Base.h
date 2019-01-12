#pragma once

#include "DX9Common.h"

namespace DX9ENGINE
{
	class DX9Base final : public DX9Common
	{
	private:
		enum class WindowStyle : DWORD
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

	private:
		static int ms_ChildWindowCount;
		static LPDIRECT3D9 ms_pD3D;

		LPDIRECT3DDEVICE9 m_pD3DDevice;
		D3DCOLOR m_BGColor;

		int m_CurrWindowWidth;
		int m_CurrWindowHeight;

	private:
		auto DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
			WindowStyle WindowStyle, RGBInt BackColor, WNDPROC Proc, LPCWSTR MenuName = nullptr, HWND hWndParent = nullptr)->HWND;
		auto DX9Base::InitD3D()->int;

	public:
		DX9Base();
		~DX9Base() {};

		auto DX9Base::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height)->Error;
		auto DX9Base::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height, RGBInt Color,
			WNDPROC Proc, LPCWSTR MenuName)->Error;
		auto DX9Base::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height,
			RGBInt Color, WNDPROC Proc)->Error;
		void DX9Base::Destroy() override;

		void DX9Base::SetBackgroundColor(D3DCOLOR color);
		void DX9Base::Resize();

		void DX9Base::BeginRender() const;
		void DX9Base::EndRender() const;

		auto DX9Base::GetDevice()->LPDIRECT3DDEVICE9 const;
	};
};