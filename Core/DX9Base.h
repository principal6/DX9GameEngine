#pragma once

#include "DX9Common.h"

namespace DX9ENGINE
{
	struct WindowData
	{
		int WindowWidth;
		int WindowHeight;
		float WindowHalfWidth;
		float WindowHalfHeight;
	};

	class DX9Base final
	{
	private:
		static int ms_ChildWindowCount;
		
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		WindowData m_WindowData;

		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		D3DCOLOR m_BGColor;

		OPENFILENAME m_OFN;
		TCHAR m_FileName[260] = { 0 };
		TCHAR m_FileTitle[260] = { 0 };

	private:
		auto DX9Base::CreateWND(const wchar_t* Name, CINT X, CINT Y, CINT Width, CINT Height,
			WindowStyle WindowStyle, DWORD BackColor, WNDPROC Proc, LPCWSTR MenuName = nullptr, HWND hWndParent = nullptr)->HWND;
		auto DX9Base::InitD3D()->int;

	public:
		DX9Base();
		~DX9Base() {};

		auto DX9Base::CreateGameWindow(CINT X, CINT Y, CINT Width, CINT Height)->Error;
		auto DX9Base::CreateParentWindow(CINT X, CINT Y, CINT Width, CINT Height, DWORD Color,
			WNDPROC Proc, LPCWSTR MenuName)->Error;
		auto DX9Base::CreateChildWindow(HWND hWndParent, CINT X, CINT Y, CINT Width, CINT Height,
			DWORD Color, WNDPROC Proc)->Error;
		void DX9Base::Destroy();

		void DX9Base::SetBackgroundColor(D3DCOLOR color);
		void DX9Base::Resize();

		void DX9Base::BeginRender() const;
		void DX9Base::EndRender() const;

		auto DX9Base::GetDevice()->LPDIRECT3DDEVICE9 const;
		auto DX9Base::GethWnd()->HWND;
		auto DX9Base::GethInstance()->HINSTANCE;
		auto DX9Base::GetWindowData()->WindowData*;

		// Dialog
		void DX9Base::SetDlgBase();
		auto DX9Base::OpenFileDlg(LPCWSTR Filter)->BOOL;
		auto DX9Base::SaveFileDlg(LPCWSTR Filter)->BOOL;
		auto DX9Base::GetDlgFileName()->WSTRING;
		auto DX9Base::GetDlgFileTitle()->WSTRING;
	};
};