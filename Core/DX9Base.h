#pragma once

#include "DX9Common.h"

class DX9Base final : public DX9Common
{
protected:
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
		ChildWindow2 =  WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
	};

private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_BGColor;

private:
	HWND DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
		WindowStyle WindowStyle, RGBInt BackColor, HWND hWndParent = nullptr);
	int DX9Base::InitD3D();
	friend LRESULT CALLBACK WndProcBase(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

public:
	DX9Base();
	~DX9Base() {};

	ReturnValue DX9Base::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height);
	ReturnValue DX9Base::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height);
	ReturnValue DX9Base::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height, RGBInt Color);
	virtual void DX9Base::Destroy() override;
	
	void DX9Base::SetBackgroundColor(D3DCOLOR color);
	void DX9Base::Resize(HWND hWnd);

	void DX9Base::BeginRender() const;
	void DX9Base::EndRender() const;

	LPDIRECT3DDEVICE9 DX9Base::GetDevice() const;
};