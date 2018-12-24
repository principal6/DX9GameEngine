#pragma once

#ifndef DX9WINDOW_H
#define DX9WINDOW_H

#include "DX9Common.h"

class DX9Base
{
// 윈도우 변수
private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	MSG			m_MSG;

// DX9 변수
private:
	LPDIRECT3D9             m_pD3D;
	LPDIRECT3DDEVICE9       m_pD3DDevice;
	D3DCOLOR				m_BGColor;

private:
	HWND DX9Base::CreateWND(const wchar_t* Name,
		CINT X, CINT Y, CINT Width, CINT Height,
		DX9WINDOW_STYLE WindowStyle, COLORRGB BackColor);
	int DX9Base::InitD3D();

public:
	DX9Base();
	~DX9Base() {};

	int DX9Base::Create(CINT X, CINT Y, CINT Width, CINT Height);
	int DX9Base::CreateOnWindow(HWND hWnd);
	int DX9Base::Destroy();
	int DX9Base::Run(int(*pMainLoop)());
	int DX9Base::RunWithAccel(int(*pMainLoop)(), HACCEL hAccel);
	int DX9Base::Halt();
	int DX9Base::Resize(HWND hWnd);
	int DX9Base::BeginRender();
	int DX9Base::EndRender();

// Setter
public:
	int DX9Base::SetBGColor(D3DCOLOR color);

// Getter
public:
	LPDIRECT3DDEVICE9	DX9Base::GetDevice() { return m_pD3DDevice; };
	HINSTANCE			DX9Base::GetInstance() { return m_hInstance; };
	HWND				DX9Base::GetHWND() { return m_hWnd; };

private:
	friend LRESULT CALLBACK WndProcBase(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

};

#endif