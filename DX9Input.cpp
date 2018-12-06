#include "DX9Input.h"

DX9Input::DX9Input()
{
	// 모든 멤버 변수 초기화
	m_diObj = 0;
	m_diKeyboard = 0;
	m_diMouse = 0;
	memset(&m_KeyDown, false, sizeof(m_KeyDown));
	memset(&m_KeyUp, false, sizeof(m_KeyUp));
	m_MouseX = 0;
	m_MouseY = 0;
	m_hWnd = NULL;
	m_hInstance = NULL;
}

DX9Input::~DX9Input()
{

}

bool DX9Input::Create(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	memset(m_MouseBtnDown, false, sizeof(m_MouseBtnDown));
	memset(m_MouseBtnUp, false, sizeof(m_MouseBtnUp));
	memset(m_MouseBtnIdle, true, sizeof(m_MouseBtnIdle));

	// DirectInput8 생성
	if(FAILED(DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &m_diObj, NULL)))
		return false;

	// 마우스 장치 생성
	if (FAILED(CreateMouseDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	// 키보드 장치 생성
	if (FAILED(CreateKeyboardDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	return true;
}

bool DX9Input::CreateMouseDevice(DWORD dwFlags)
{
	if(FAILED(m_diObj->CreateDevice(GUID_SysMouse, &m_diMouse, NULL)))
		return false;

	if(FAILED(m_diMouse->SetDataFormat(&c_dfDIMouse2)))
		return false;

	if(FAILED(m_diMouse->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if(FAILED(m_diMouse->Acquire()))
		return false;

	return true;
}

bool DX9Input::CreateKeyboardDevice(DWORD dwFlags)
{
	if(FAILED(m_diObj->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, NULL)))
		return false;

	if(FAILED(m_diKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if(FAILED(m_diKeyboard->SetCooperativeLevel(m_hWnd, dwFlags)))
		return false;

	if(FAILED(m_diKeyboard->Acquire()))
		return false;
	
	return true;
}

bool DX9Input::Destroy()
{
	if (m_diMouse)
	{
		m_diMouse->Unacquire();
		m_diMouse->Release();
	}

	if (m_diKeyboard)
	{
		m_diKeyboard->Unacquire();
		m_diKeyboard->Release();
	}
	if (m_diObj)
	{
		m_diObj->Release();
	}
	
	return true;
}

bool DX9Input::OnKeyDown(DWORD DIK_KeyCode)
{
	HRESULT hr;

	if(FAILED(hr = m_diKeyboard->GetDeviceState(sizeof(m_BufferKeyState),(LPVOID) &m_BufferKeyState)))
	{
		if(hr == DIERR_INPUTLOST)
			m_diKeyboard->Acquire();
	}

	if (m_BufferKeyState[DIK_KeyCode] & 0x80)	// 키가 눌리면 true를 반환
	{
		m_KeyUp[DIK_KeyCode] = false;
		m_KeyDown[DIK_KeyCode] = true;
		return true;
	}
	else
	{
		if (m_KeyDown[DIK_KeyCode] == true) // 이전에 키가 눌려있었다면?
		{
			m_KeyUp[DIK_KeyCode] = true;
		}
		else
		{
			m_KeyUp[DIK_KeyCode] = false;
		}
		
		m_KeyDown[DIK_KeyCode] = false;
		return false;
	}
}

bool DX9Input::OnKeyUp(DWORD DIK_KeyCode)
{
	return m_KeyUp[DIK_KeyCode];
}

DIMOUSESTATE2 DX9Input::OnMouseMove(void)
{
	DIMOUSESTATE2 p;
	HRESULT hr;
	
	memset(&m_MouseState, 0, sizeof(m_MouseState));
	if(FAILED(hr = m_diMouse->GetDeviceState(sizeof(m_MouseState),(LPVOID) &m_MouseState)))
	{
		if(hr == DIERR_INPUTLOST)
			m_diMouse->Acquire();
	}

	p.lX = (float)m_MouseState.lX;
	p.lY = (float)m_MouseState.lY;
	p.lZ = (float)m_MouseState.lZ;

	return p;
}

bool DX9Input::CheckMouseButton(int button)
{
	if(m_MouseState.rgbButtons[button] & 0x80) // 버튼이 눌림
	{
		if ( (m_MouseBtnDown[button] == false) && (m_MouseBtnIdle[button] == true) )
		{
			m_MouseBtnUp[button] = false;
			m_MouseBtnDown[button] = true;
		}
	}
	else if(!m_MouseState.rgbButtons[button]) // 버튼이 눌리지 않음
	{
		if ( (m_MouseBtnDown[button] == false) && (m_MouseBtnIdle[button] == false) )
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnIdle[button] = true;
			m_MouseBtnDown[button] = false;
		}
		else if ( (m_MouseBtnDown[button] == true) && (m_MouseBtnIdle[button] == true) )
		{
			m_MouseBtnUp[button] = true;
			m_MouseBtnDown[button] = false;
		}
	}

	return true;
}

bool DX9Input::OnMouseButtonDown(int button)
{
	CheckMouseButton(button);

	if (m_MouseBtnDown[button] == true)
	{
		m_MouseBtnDown[button] = false;
		m_MouseBtnIdle[button] = false;
		return true;
	}

	return false;
}

bool DX9Input::OnMouseButtonUp(int button)
{
	CheckMouseButton(button);

	if (m_MouseBtnUp[button] == true)
	{
		m_MouseBtnUp[button] = false;
		m_MouseBtnIdle[button] = true;
		return true;
	}

	return false;
}

bool DX9Input::GetKeyState(DWORD DIK_KeyCode)
{
	if (m_BufferKeyState[DIK_KeyCode] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DX9Input::GetMouseButtonDown(int button)
{
	CheckMouseButton(button);
	return m_MouseBtnDown[button];
}