#pragma once

#ifndef DX9INPUT_H
#define DX9INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#define NUM_KEYS 256

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "DX9Common.h"
#include <dinput.h>

class DX9Input
{
public:

private:
	HWND            m_hWnd;
	HINSTANCE		m_hInstance;

	LPDIRECTINPUT8			m_diObj;
	LPDIRECTINPUTDEVICE8	m_diKeyboard;
	LPDIRECTINPUTDEVICE8	m_diMouse;

	char					m_BufferKeyState[NUM_KEYS];
	bool					m_KeyDown[NUM_KEYS];
	bool					m_KeyUp[NUM_KEYS];
	DIMOUSESTATE2			m_MouseState;

	int		m_MouseX, m_MouseY;	// 마우스 위치
	bool	m_MouseBtnDown[3];
	bool	m_MouseBtnUp[3];
	bool	m_MouseBtnIdle[3];

public:
	DX9Input();
	~DX9Input();

	// Create
	bool			DX9Input::Create(HINSTANCE hInstance, HWND hWnd);

	// Event
	bool			DX9Input::OnKeyDown(DWORD DIK_KeyCode);
	bool			DX9Input::OnKeyUp(DWORD DIK_KeyCode);
	DIMOUSESTATE2	DX9Input::OnMouseMove(void);
	bool			DX9Input::OnMouseButtonDown(int button);
	bool			DX9Input::OnMouseButtonUp(int button);

	// Getter
	bool			DX9Input::GetKeyState(DWORD DIK_KeyCode);
	bool			DX9Input::GetMouseButtonDown(int button);

	// Destroy
	bool			DX9Input::Destroy();

private:
	bool DX9Input::CreateMouseDevice(DWORD dwFlags);
	bool DX9Input::CreateKeyboardDevice(DWORD dwFlags);
	bool DX9Input::CheckMouseButton(int button);
};

#endif