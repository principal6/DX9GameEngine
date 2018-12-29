#include "DX9Font.h"

DX9Font::DX9Font()
{
	for (int i = 0; i < MAX_FONT_NUM; i++)
	{
		m_pFonts[i] = nullptr;
	}

	m_FontCount = 0;
	m_CurrFontID = 0;

	m_ScreenW = 0;
	m_ScreenH = 0;

	m_FontColor = 0xFF000000;
}

int DX9Font::Create(LPDIRECT3DDEVICE9 pDevice, int ScreenW, int ScreenH)
{
	m_pDevice = pDevice;
	m_ScreenW = ScreenW;
	m_ScreenH = ScreenH;

	return 0;
}

int DX9Font::Destroy()
{
	m_pDevice = nullptr;

	for (int i = 0; i < MAX_FONT_NUM; i++)
	{
		if (m_pFonts[i])
		{
			m_pFonts[i]->Release();
			m_pFonts[i] = nullptr;
		}
	}

	return 0;
}

int DX9Font::MakeFont(DX9FONT_ID ID, std::wstring FontName, int FontSize, bool IsBold)
{
	UINT Weight = FW_NORMAL;
	if (IsBold)
		Weight = FW_BOLD;

	D3DXCreateFont(m_pDevice, FontSize, 0, Weight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, FontName.c_str(), &m_pFonts[(int)ID]);

	return 0;
}

int DX9Font::SelectFont(DX9FONT_ID ID)
{
	m_CurrFontID = (int)ID;

	return 0;
}

int DX9Font::SetFontColor(DWORD Color)
{
	m_FontColor = Color;

	return 0;
}

int DX9Font::Draw(int X, int Y, std::wstring String)
{
	RECT Rect_Font;
	SetRect(&Rect_Font, X, Y, m_ScreenW, m_ScreenH);
	m_pFonts[m_CurrFontID]->DrawText(nullptr, String.c_str(), -1, &Rect_Font, DT_LEFT|DT_NOCLIP, m_FontColor);

	return 0;
}