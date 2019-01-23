#include "DX9MapTileSelector.h"
#include "../CoreBase/DX9Window.h"
#include "../Core/DX9Map.h"

using namespace DX9ENGINE;

// Static member variable
const wchar_t* DX9MapTileSelector::SEL_FN = L"tilesel.png";

PRIVATE auto DX9MapTileSelector::ConvertPositionToCellXY(POINT Position)->POINT
{
	POINT Result{ 0, 0 };
	
	if (m_pMapInfo && m_pMapInfo->TileSize)
	{
		Result.x = (static_cast<int>(Position.x) / m_pMapInfo->TileSize);
		Result.y = (static_cast<int>(Position.y) / m_pMapInfo->TileSize);

		Result.x = max(Result.x, 0);
		Result.y = max(Result.y, 0);
	}

	return Result;
}

DX9MapTileSelector::DX9MapTileSelector()
{
	m_pMapInfo = nullptr;

	m_TileSelectorPositionInCells = { 0, 0 };
	m_SelectionSize = { 0, 0 };
}

auto DX9MapTileSelector::Create(DX9Window* pTileSelectorWindow, DX9Window* pMapWindow, WSTRING BaseDir)->EError
{
	// Create tile selector image
	if (m_TileSelector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_TileSelector->Create(pTileSelectorWindow, BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	m_TileSelector->SetTexture(SEL_FN);
	m_TileSelector->SetAlpha(SEL_ALPHA);
	m_TileSelector->SetSize(D3DXVECTOR2(0, 0));

	// Create map selector image
	// Map selector's texture will be set when SetMapInfo() is called
	if (m_MapSelector = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(m_MapSelector->Create(pMapWindow, BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	m_MapSelector->SetAlpha(SEL_ALPHA);

	return EError::OK;
}

void DX9MapTileSelector::Destroy()
{

}

void DX9MapTileSelector::UpdateTileSelector(SMouseData* MouseData)
{
	if (m_pMapInfo)
	{
		if (m_pMapInfo->TileSize)
		{
			POINT PositionInCells = ConvertPositionToCellXY(MouseData->MousePosition);
			POINT DownPositionInCells = ConvertPositionToCellXY(MouseData->MouseDownPosition);

			// @warning:
			// We do not restrict the tilesheet's max selection range
			// because normally the texture file width and height are bigger than the tilesheet itself
			// fot a better precision in DirectX9 texture.
			// *** The texture size must be 2^n (e.g. 256*512, 1024*128)
			/*
			PositionInCells.x = min(PositionInCells.x, m_pMapInfo->TileSheetCols);
			PositionInCells.y = min(PositionInCells.y, m_pMapInfo->TileSheetRows);
			DownPositionInCells.x = min(DownPositionInCells.x, m_pMapInfo->TileSheetCols);
			DownPositionInCells.y = min(DownPositionInCells.y, m_pMapInfo->TileSheetRows);
			*/

			m_SelectionSize.x = abs(DownPositionInCells.x - PositionInCells.x);
			m_SelectionSize.y = abs(DownPositionInCells.y - PositionInCells.y);

			m_TileSelectorPositionInCells = DownPositionInCells;
			if ((DownPositionInCells.x - PositionInCells.x) > 0)
			{
				// X position flip
				m_TileSelectorPositionInCells.x = DownPositionInCells.x - m_SelectionSize.x;
			}
			if ((DownPositionInCells.y - PositionInCells.y) > 0)
			{
				// Y position flip
				m_TileSelectorPositionInCells.y = DownPositionInCells.y - m_SelectionSize.y;
			}

			D3DXVECTOR2 NewPosition;
			NewPosition.x = static_cast<float>(m_TileSelectorPositionInCells.x * m_pMapInfo->TileSize);
			NewPosition.y = static_cast<float>(m_TileSelectorPositionInCells.y * m_pMapInfo->TileSize);

			D3DXVECTOR2 NewSize{ 0, 0 };
			NewSize.x = m_SelectionSize.x + 1.0f;
			NewSize.y = m_SelectionSize.y + 1.0f;
			NewSize *= static_cast<float>(m_pMapInfo->TileSize);

			m_TileSelector->SetPosition(NewPosition);
			m_TileSelector->SetSize(NewSize);

			m_MapSelector->SetSize(NewSize);
			m_MapSelector->SetAtlasUV(NewPosition, NewSize);
		}
	}
}

void DX9MapTileSelector::UpdateMapSelector(SMouseData* MouseData)
{
	if (m_pMapInfo)
	{
		m_MapSelectorPositionInCells = ConvertPositionToCellXY(MouseData->MousePosition);

		// @warning
		// Restrict position in cells IAW map's max rows and cols
		m_MapSelectorPositionInCells.x = min(m_MapSelectorPositionInCells.x, m_pMapInfo->MapCols - 1);
		m_MapSelectorPositionInCells.y = min(m_MapSelectorPositionInCells.y, m_pMapInfo->MapRows - 1);

		D3DXVECTOR2 NewPosition;
		NewPosition.x = static_cast<float>(m_MapSelectorPositionInCells.x * m_pMapInfo->TileSize);
		NewPosition.y = static_cast<float>(m_MapSelectorPositionInCells.y * m_pMapInfo->TileSize);

		m_MapSelector->SetPosition(NewPosition);
	}
}

void DX9MapTileSelector::Draw()
{
	if (m_TileSelector)
	{
		m_TileSelector->Draw();
	}

	if (m_MapSelector)
	{
		m_MapSelector->Draw();
		m_MapSelector->DrawBoundingBox();
	}
}

auto DX9MapTileSelector::SetMapInfo(SMapInfo* pInfo)->EError
{
	if (nullptr == (m_pMapInfo = pInfo))
		return EError::NULLPTR_MAP_INFO;

	m_TileSelector->SetSize(D3DXVECTOR2(static_cast<float>(m_pMapInfo->TileSize), static_cast<float>(m_pMapInfo->TileSize)));
	m_MapSelector->SetTexture(m_pMapInfo->TileSheetName);
	
	InitializeSelectorPositionAndSize();

	return EError::OK;
}

void DX9MapTileSelector::UpdateMapMode(EMapMode Mode)
{
	switch (Mode)
	{
	case DX9ENGINE::EMapMode::TileMode:
		m_MapSelector->SetTexture(m_pMapInfo->TileSheetName);
		break;
	case DX9ENGINE::EMapMode::MoveMode:
		m_MapSelector->SetTexture(m_pMapInfo->MoveSheetName);
		break;
	default:
		break;
	}

	InitializeSelectorPositionAndSize();
}

PRIVATE void DX9MapTileSelector::InitializeSelectorPositionAndSize()
{
	if (m_pMapInfo)
	{
		m_TileSelector->SetSize(D3DXVECTOR2(static_cast<float>(m_pMapInfo->TileSize),
			static_cast<float>(m_pMapInfo->TileSize)));
		m_TileSelector->SetPosition(D3DXVECTOR2(0, 0));

		m_MapSelector->SetSize(m_TileSelector->GetSize());
		m_MapSelector->SetAtlasUV(D3DXVECTOR2(0, 0), m_TileSelector->GetSize());
	}
}

auto DX9MapTileSelector::GetTileSelectorPositionInCells() const->POINT
{
	return m_TileSelectorPositionInCells;
}

auto DX9MapTileSelector::GetMapSelectorPositionInCells() const->POINT
{
	return m_MapSelectorPositionInCells;
}

auto DX9MapTileSelector::GetSelectionSizeInCells() const->POINT
{
	return m_SelectionSize;
}