#pragma once

#ifndef _DX9MAP_H_
#define _DX9MAP_H_

#include <fstream>
#include "DX9Image.h"

const int MAX_LINE_LEN = 1024;
const int MAX_TILEID_LEN = 3;
const int MAX_MOVEID_LEN = 2;
const int TILE_W = 32;
const int TILE_H = 32;
const int MOVE_ALPHA = 100;

class DX9Map : protected DX9Image
{
private:
	bool m_bMapCreated;
	DX9MAPMODE m_CurrMapMode;

	int m_nMapCols;
	int m_nMapRows;
	int	m_nTileSheetWidth;
	int	m_nTileSheetHeight;
	int	m_nMoveSheetWidth;
	int	m_nMoveSheetHeight;
	std::wstring m_MapName;
	std::wstring m_TileName;
	std::wstring m_MapDataInString;
	std::vector<DX9MAPDATA> m_MapData;

	bool m_bMoveTextureLoaded;
	LPDIRECT3DTEXTURE9 m_pTextureMove;
	LPDIRECT3DVERTEXBUFFER9 m_pVBMove;
	std::vector<DX9VERTEX_IMAGE> m_VertMove;
	int m_nVertMoveCount;

	float m_fOffsetX;
	float m_fOffsetY;

private:
	int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size);
	DX9UV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH);
	int DX9Map::CreateVBMove();
	int DX9Map::UpdateVBMove();
	int DX9Map::AddMapFragmentTile(int TileID, int X, int Y);
	int DX9Map::AddMapFragmentMove(int MoveID, int X, int Y);
	int DX9Map::AddEnd();
	int DX9Map::SetMapData(std::wstring Str);

	// Sprite Collision
	float DX9Map::GetMapTileBoundary(int MapID, DX9MAPDIR Dir);
	bool DX9Map::IsMovableTile(int MapID, DX9MAPDIR Dir);

public:
	DX9Map();
	~DX9Map() {};

	int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Map::Destroy();
	
	// Graphics
	int DX9Map::SetTileTexture(std::wstring FileName);
	int DX9Map::SetMoveTexture(std::wstring FileName);
	int DX9Map::Draw();

	// Map creation
	int DX9Map::CreateMap(std::wstring Name, int MapCols, int MapRows);
	int DX9Map::CreateMapWithData();
	bool DX9Map::IsMapCreated() { return m_bMapCreated; };

	// Setter
	int DX9Map::SetMapFragmentTile(int TileID, int X, int Y);
	int DX9Map::SetMapFragmentMove(int MoveID, int X, int Y);
	int DX9Map::SetMode(DX9MAPMODE Mode);
	int DX9Map::SetPosition(float OffsetX, float OffsetY);

	// Load & Save
	int DX9Map::LoadMapFromFile(std::wstring FileName);
	int DX9Map::GetMapData(std::wstring *pStr);

	// Getter
	int DX9Map::GetMapName(std::wstring *pStr);
	int DX9Map::GetTileName(std::wstring *pStr);
	int DX9Map::GetMapCols() { return m_nMapCols; };
	int DX9Map::GetMapRows() { return m_nMapRows; };
	int DX9Map::GetWidth() { return (m_nMapCols * TILE_W); };
	int DX9Map::GetHeight() { return (m_nMapRows * TILE_H); };

	// Converter
	D3DXVECTOR2 DX9Map::ConvertScrPosToXY(D3DXVECTOR2 ScreenPos);
	D3DXVECTOR2 DX9Map::ConvertScrPosToXYRoundUp(D3DXVECTOR2 ScreenPos);
	D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID);
	int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY);

	// Sprite Collision
	D3DXVECTOR2 DX9Map::CheckSprCollision(D3DXVECTOR2 SprPos, D3DXVECTOR2 Velocity);
	D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(DX9BOUNDINGBOX BB, D3DXVECTOR2 Velocity);
};


#endif