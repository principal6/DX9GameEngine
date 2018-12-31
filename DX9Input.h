#pragma once

#define DIRECTINPUT_VERSION 0x0800

const int NUM_KEYS = 256;

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "DX9Common.h"
#include <dinput.h>

class DX9Input final
{
private:
	HINSTANCE m_hInstance;
	HWND  m_hWnd;

	LPDIRECTINPUT8 m_DI8;
	LPDIRECTINPUTDEVICE8 m_DIDevKeyboard;
	LPDIRECTINPUTDEVICE8 m_DIDevMouse;

	char m_BufferKeyState[NUM_KEYS];
	bool m_KeyDown[NUM_KEYS];
	bool m_KeyUp[NUM_KEYS];
	DIMOUSESTATE2 m_MouseState;

	int m_MouseX;
	int m_MouseY;
	bool m_MouseBtnDown[3];
	bool m_MouseBtnUp[3];
	bool m_MouseBtnIdle[3];

private:
	bool DX9Input::CreateMouseDevice(DWORD dwFlags);
	bool DX9Input::CreateKeyboardDevice(DWORD dwFlags);
	bool DX9Input::CheckMouseButton(int button);

public:
	DX9Input();
	~DX9Input() {};

	bool DX9Input::Create(HINSTANCE hInstance, HWND hWnd);
	bool DX9Input::Destroy();

	bool DX9Input::OnKeyDown(DWORD DIK_KeyCode);
	bool DX9Input::OnKeyUp(DWORD DIK_KeyCode);
	DIMOUSESTATE2 DX9Input::OnMouseMove();
	bool DX9Input::OnMouseButtonDown(int button);
	bool DX9Input::OnMouseButtonUp(int button);

	bool DX9Input::GetMouseButtonDown(int button);
	bool DX9Input::GetKeyState(DWORD DIK_KeyCode) const;
};