#include "DX9Font.h"

using namespace DX9ENGINE;

// Static member variable declaration
LPDIRECT3DDEVICE9 DX9Font::m_pDevice;

DX9Font::DX9Font()
{
	m_CurrFontInstanceID = 0;
	m_FontColor = 0xFF000000;
}

DX9Common::ReturnValue DX9Font::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	m_pDevice = pDevice;

	return ReturnValue::OK;
}

void DX9Font::Destroy()
{
	m_pDevice = nullptr;

	for (FontInstance& iterator : m_Fonts)
	{
		if (iterator.pFont)
		{
			iterator.pFont->Release();
			iterator.pFont = nullptr;
		}
	}
}

void DX9Font::MakeFont(FontID ID, WSTRING FontName, int FontSize, bool IsBold)
{
	UINT Weight = FW_NORMAL;
	if (IsBold)
		Weight = FW_BOLD;

	LPD3DXFONT tpFont;
	D3DXCreateFont(m_pDevice, FontSize, 0, Weight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, FontName.c_str(), &tpFont);

	m_Fonts.emplace_back(ID, tpFont);
}

DX9Font* DX9Font::SelectFont(FontID ID)
{
	int iterator_n = 0;
	for (FontInstance& iterator : m_Fonts)
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

DX9Font* DX9Font::SetFontColor(DWORD Color)
{
	m_FontColor = Color;
	return this;
}

DX9Font* DX9Font::Draw(int X, int Y, WSTRING String)
{
	RECT Rect_Font;
	SetRect(&Rect_Font, X, Y, m_WindowData.WindowWidth, m_WindowData.WindowHeight);
	m_Fonts[m_CurrFontInstanceID].pFont->DrawText(nullptr, String.c_str(), -1, &Rect_Font, DT_LEFT|DT_NOCLIP, m_FontColor);
	return this;
}