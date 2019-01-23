#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "DX9Common.h"
#include <dinput.h>

namespace DX9ENGINE
{
	class DX9Input final
	{
	public:
		DX9Input();
		~DX9Input() {};

		auto DX9Input::Create(HWND hWnd, HINSTANCE hInstance)->EError;
		void DX9Input::Destroy();

		auto DX9Input::OnKeyDown(DWORD DIK_KeyCode)->bool;
		auto DX9Input::OnKeyUp(DWORD DIK_KeyCode)->bool;
		auto DX9Input::OnMouseMove()->DIMOUSESTATE2;
		auto DX9Input::OnMouseButtonDown(int button)->bool;
		auto DX9Input::OnMouseButtonUp(int button)->bool;

		auto DX9Input::GetMouseButtonDown(int button)->bool;
		auto DX9Input::GetKeyState(DWORD DIK_KeyCode) const->bool;
		void DX9Input::GetAllKeyState(bool* Keys);

	private:
		bool DX9Input::CreateMouseDevice(DWORD dwFlags);
		bool DX9Input::CreateKeyboardDevice(DWORD dwFlags);
		bool DX9Input::CheckMouseButton(int button);

	private:
		HWND m_hWnd;

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
	};
};