#pragma once

#ifndef DX9WINDOW_H
#define DX9WINDOW_H

#include "DX9Common.h"

class DX9Base
{
// 공용 변수
private:
	int (*mpMainLoop);

// 윈도우 변수
private:
	HWND		mhWnd;
	HINSTANCE	mhInstance;
	MSG			mMSG;

// DX9 변수
private:
	LPDIRECT3D9             mpD3D;
	LPDIRECT3DDEVICE9       mpD3DDevice;

// 공용 함수
public:
	DX9Base();
	~DX9Base() {};

	int DX9Base::Create(CINT X, CINT Y, CINT Width, CINT Height);
	int DX9Base::Destroy();

// 윈도우 함수
public:
	HWND DX9Base::CreateWND(const wchar_t* Name,
		CINT X, CINT Y, CINT Width, CINT Height,
		DX9WINDOW_STYLE WindowStyle, DX9COLOR BackColor);
	int DX9Base::Run(int(*pMainLoop)());
	int DX9Base::Halt();

// DX9 함수
public:
	int DX9Base::InitD3D();
	int DX9Base::BeginRender();
	int DX9Base::EndRender();

// Getter
public:
	LPDIRECT3DDEVICE9	DX9Base::GetDevice() { return mpD3DDevice; };
	HINSTANCE			DX9Base::GetInstance() { return mhInstance; };
	HWND				DX9Base::GetHWND() { return mhWnd; };

private:
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

};

#endif