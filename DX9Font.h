#include "DX9Common.h"

class DX9Font final : protected DX9Common
{
private:
	struct FontInstance
	{
		FontID ID;
		LPD3DXFONT pFont;

		FontInstance() : pFont(nullptr) {};
		FontInstance(FontID _ID, LPD3DXFONT _pFont) : ID(_ID), pFont(_pFont) {};
	};

private:
	static LPDIRECT3DDEVICE9 m_pDevice;

	std::vector<FontInstance> m_Fonts;

	int	m_CurrFontInstanceID;
	DWORD m_FontColor;

public:
	DX9Font();
	~DX9Font() {};

	ReturnValue DX9Font::Create(LPDIRECT3DDEVICE9 pDevice);
	void DX9Font::Destroy();

	void DX9Font::MakeFont(FontID ID, WSTRING FontName, int FontSize, bool IsBold);
	DX9Font* DX9Font::SelectFont(FontID ID);
	DX9Font* DX9Font::SetFontColor(DWORD Color);

	DX9Font* DX9Font::Draw(int X, int Y, WSTRING String);
};