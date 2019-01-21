#include "Core/DX9Common.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Base;
	// ***

	class DX9Font final
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
		DX9Base* m_pBase;

		VECTOR<FontInstance> m_Fonts;

		int m_CurrFontInstanceID;
		DWORD m_FontColor;

	public:
		DX9Font();
		~DX9Font() {};

		auto DX9Font::Create(DX9Base* pBase)->Error;
		void DX9Font::Destroy();

		void DX9Font::MakeFont(FontID ID, WSTRING FontName, int FontSize, bool IsBold);
		auto DX9Font::SelectFont(FontID ID)->DX9Font*;
		auto DX9Font::SetFontColor(DWORD Color)->DX9Font*;

		auto DX9Font::Draw(int X, int Y, WSTRING String)->DX9Font*;
	};
};