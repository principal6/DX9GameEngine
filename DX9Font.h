#include "DX9Common.h"

const int MAX_FONT_NUM = 10;

class DX9Font final
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	DX9SHARE_DATA* m_pShareData;
	LPD3DXFONT	m_pFonts[MAX_FONT_NUM];

	int	m_FontCount;
	int	m_CurrFontID;

	DWORD m_FontColor;

public:
	DX9Font();
	~DX9Font() {};

	void DX9Font::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData);
	void DX9Font::Destroy();

	void DX9Font::MakeFont(DX9FONT_ID ID, WSTRING FontName, int FontSize, bool IsBold);
	DX9Font* DX9Font::SelectFont(DX9FONT_ID ID);
	DX9Font* DX9Font::SetFontColor(DWORD Color);

	DX9Font* DX9Font::Draw(int X, int Y, WSTRING String);
};