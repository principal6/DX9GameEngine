#include "../CoreBase/DX9Common.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
	// ***

	enum class EFontID
	{
		Title,
		Subtitle,
		Text,
		Debug,
	};

	struct SFontInstance
	{
		EFontID ID;
		LPD3DXFONT pFont;

		SFontInstance() : pFont(nullptr) {};
		SFontInstance(EFontID _ID, LPD3DXFONT _pFont) : ID(_ID), pFont(_pFont) {};
	};

	class DX9Font final
	{
	public:
		DX9Font();
		~DX9Font() {};

		// Set parent DX9Window
		auto DX9Font::Create(DX9Window* pDX9Window)->EError;

		// Release all the instances
		void DX9Font::Destroy();

		// Make a font
		void DX9Font::MakeFont(EFontID ID, WSTRING FontName, int FontSize, bool IsBold);

		// Select a font
		auto DX9Font::SelectFontByID(EFontID ID)->DX9Font*;

		// Set font color
		auto DX9Font::SetFontColor(DWORD Color)->DX9Font*;

		// Draw the text
		auto DX9Font::Draw(int X, int Y, WSTRING String)->DX9Font*;

	private:
		
		// Pointer to the parent DX9Window
		DX9Window* m_pDX9Window;

		// Font instances
		VECTOR<SFontInstance> m_Fonts;

		// Current font instance ID
		int m_CurrFontInstanceID;

		// Current font color
		DWORD m_FontColor;
	};
};