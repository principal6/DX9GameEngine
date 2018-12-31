#include "DX9Common.h"

const int MAX_FONT_NUM = 10;

class DX9Font final
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXFONT	m_pFonts[MAX_FONT_NUM];

	int	m_FontCount;
	int	m_CurrFontID;

	int	m_ScreenW;
	int	m_ScreenH;

	DWORD m_FontColor;

public:
	DX9Font();
	~DX9Font() {};

	void DX9Font::Create(LPDIRECT3DDEVICE9 pDevice, int ScreenW, int ScreenH);
	void DX9Font::Destroy();

	void DX9Font::MakeFont(DX9FONT_ID ID, std::wstring FontName, int FontSize, bool IsBold);
	void DX9Font::SelectFont(DX9FONT_ID ID);
	void DX9Font::SetFontColor(DWORD Color);

	void DX9Font::Draw(int X, int Y, std::wstring String);
};