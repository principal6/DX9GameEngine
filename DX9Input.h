#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "DX9Common.h"
#include <dinput.h>

class DX9Input final : protected DX9Common
{
private:
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

	ReturnValue DX9Input::Create();
	void DX9Input::Destroy() override;

	bool DX9Input::OnKeyDown(DWORD DIK_KeyCode);
	bool DX9Input::OnKeyUp(DWORD DIK_KeyCode);
	DIMOUSESTATE2 DX9Input::OnMouseMove();
	bool DX9Input::OnMouseButtonDown(int button);
	bool DX9Input::OnMouseButtonUp(int button);

	bool DX9Input::GetMouseButtonDown(int button);
	bool DX9Input::GetKeyState(DWORD DIK_KeyCode) const;
	void DX9Input::GetAllKeyState(bool* Keys);
};