#pragma once

#include "../CoreBase/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
	struct SMouseData;
	struct SMapInfo;
	enum class EMapMode;
	// ***

	class DX9MapTileSelector final
	{
	public:
		DX9MapTileSelector();
		~DX9MapTileSelector() {};

		auto Create(DX9Window* pTileSelectorWindow, DX9Window* pMapWindow, WSTRING BaseDir)->EError;
		void Destroy();

		void UpdateTileSelector(SMouseData* MouseData);
		void UpdateMapSelector(SMouseData* MouseData);
		void UpdateMapMode(EMapMode Mode);
		void Draw();

		auto SetMapInfo(SMapInfo* pInfo)->EError;

		auto GetTileSelectorPositionInCells() const->POINT;
		auto GetMapSelectorPositionInCells() const->POINT;
		auto GetSelectionSizeInCells() const->POINT;

	private:
		auto ConvertPositionToCellXY(POINT Position)->POINT;
		void InitializeSelectorPositionAndSize();

	private:
		static const int SEL_ALPHA = 160;
		static const wchar_t* SEL_FN;

		SMapInfo* m_pMapInfo;
		POINT m_TileSelectorPositionInCells;
		POINT m_SelectionSize;
		UNIQUE_PTR<DX9Image> m_TileSelector;
		UNIQUE_PTR<DX9Image> m_MapSelector;
		POINT m_MapSelectorPositionInCells;
	};
};