#pragma once

#include "DX9Common.h"

class DX9Base final
{
// 윈도우 변수
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	MSG m_MSG;

// DX9 변수
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_BGColor;

private:
	HWND DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
		DX9WINDOW_STYLE WindowStyle, COLOR_RGB BackColor);
	int DX9Base::InitD3D();
	friend LRESULT CALLBACK WndProcBase(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

public:
	DX9Base();
	~DX9Base() {};

	int DX9Base::Create(CINT X, CINT Y, CINT Width, CINT Height);
	int DX9Base::CreateOnWindow(HWND hWnd);
	int DX9Base::Run(int(*pMainLoop)());
	int DX9Base::RunWithAccel(int(*pMainLoop)(), HACCEL hAccel);
	void DX9Base::Destroy();
	void DX9Base::Halt();
	
	void DX9Base::SetBackgroundColor(D3DCOLOR color);
	void DX9Base::Resize(HWND hWnd);

	void DX9Base::BeginRender() const;
	void DX9Base::EndRender() const;

	LPDIRECT3DDEVICE9 DX9Base::GetDevice() const;
	HINSTANCE DX9Base::GetInstance() const;
	HWND DX9Base::GetHWND() const;
};