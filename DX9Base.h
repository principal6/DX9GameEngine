#pragma once

#include "DX9Common.h"

class DX9Base : protected DX9Common
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
	};

	struct RGBInt
	{
		int Red, Green, Blue;

		RGBInt() : Red(0), Green(0), Blue(0) {};
		RGBInt(int _Red, int _Green, int _Blue) : Red(_Red), Green(_Green), Blue(_Blue) {};
	};

private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_BGColor;

protected:
	
	MSG m_MSG;

private:
	HWND DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
		WindowStyle WindowStyle, RGBInt BackColor);
	int DX9Base::InitD3D();
	friend LRESULT CALLBACK WndProcBase(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

public:
	DX9Base();
	~DX9Base() {};

	bool DX9Base::Create(CINT X, CINT Y);
	void DX9Base::CreateOnWindow(HWND hWnd);
	virtual void DX9Base::Run() = 0;
	virtual int DX9Base::RunWithAccel(HACCEL hAccel) = 0;
	virtual void DX9Base::Destroy() override;
	virtual void DX9Base::Shutdown();
	
	void DX9Base::SetBackgroundColor(D3DCOLOR color);
	void DX9Base::Resize(HWND hWnd);

	void DX9Base::BeginRender() const;
	void DX9Base::EndRender() const;

	LPDIRECT3DDEVICE9 DX9Base::GetDevice() const;
};