#include "../Core/DX9Window.h"
#include "DX9Font.h"

using namespace DX9ENGINE;

DX9Font::DX9Font()
{
	m_CurrFontInstanceID = 0;
	m_FontColor = 0xFF000000;
}

auto DX9Font::Create(DX9Window* pDX9Window)->EError
{
	if (pDX9Window == nullptr)
		return EError::NULLPTR_BASE;

	m_pDX9Window = pDX9Window;

	return EError::OK;
}

void DX9Font::Destroy()
{
	m_pDX9Window = nullptr;

	for (SFontInstance& iterator : m_Fonts)
	{
		if (iterator.pFont)
		{
			iterator.pFont->Release();
			iterator.pFont = nullptr;
		}
	}
}

void DX9Font::MakeFont(EFontID ID, WSTRING FontName, int FontSize, bool IsBold)
{
	UINT Weight = FW_NORMAL;
	if (IsBold)
		Weight = FW_BOLD;

	LPD3DXFONT tpFont;
	D3DXCreateFont(m_pDX9Window->GetDevice(), FontSize, 0, Weight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, FontName.c_str(), &tpFont);

	m_Fonts.emplace_back(ID, tpFont);
}

auto DX9Font::SelectFont(EFontID ID)->DX9Font*
{
	int iterator_n = 0;
	for (SFontInstance& iterator : m_Fonts)
	{
		if (iterator.ID == ID)
		{
			m_CurrFontInstanceID = iterator_n;
			return this;
		}
		iterator_n++;
	}

	return nullptr;
}

auto DX9Font::SetFontColor(DWORD Color)->DX9Font*
{
	m_FontColor = Color;
	return this;
}

auto DX9Font::Draw(int X, int Y, WSTRING String)->DX9Font*
{
	RECT Rect_Font;
	SetRect(&Rect_Font, X, Y, m_pDX9Window->GetWindowData()->WindowWidth, m_pDX9Window->GetWindowData()->WindowHeight);
	m_Fonts[m_CurrFontInstanceID].pFont->DrawText(nullptr, String.c_str(), -1, &Rect_Font, DT_LEFT|DT_NOCLIP, m_FontColor);
	return this;
}