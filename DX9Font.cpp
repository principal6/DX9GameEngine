#include "DX9Font.h"

DX9Font::DX9Font()
{
	for (int i = 0; i < MAX_FONT_NUM; i++)
	{
		m_pFonts[i] = nullptr;
	}

	m_FontCount = 0;
	m_CurrFontID = 0;

	m_FontColor = 0xFF000000;
}

void DX9Font::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData)
{
	m_pDevice = pDevice;
	m_pShareData = pData;
}

void DX9Font::Destroy()
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
}

void DX9Font::MakeFont(DX9FONT_ID ID, WSTRING FontName, int FontSize, bool IsBold)
{
	UINT Weight = FW_NORMAL;
	if (IsBold)
		Weight = FW_BOLD;

	D3DXCreateFont(m_pDevice, FontSize, 0, Weight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, FontName.c_str(), &m_pFonts[(int)ID]);
}

DX9Font* DX9Font::SelectFont(DX9FONT_ID ID)
{
	m_CurrFontID = (int)ID;
	return this;
}

DX9Font* DX9Font::SetFontColor(DWORD Color)
{
	m_FontColor = Color;
	return this;
}

DX9Font* DX9Font::Draw(int X, int Y, WSTRING String)
{
	RECT Rect_Font;
	SetRect(&Rect_Font, X, Y, m_pShareData->WindowWidth, m_pShareData->WindowHeight);
	m_pFonts[m_CurrFontID]->DrawText(nullptr, String.c_str(), -1, &Rect_Font, DT_LEFT|DT_NOCLIP, m_FontColor);
	return this;
}