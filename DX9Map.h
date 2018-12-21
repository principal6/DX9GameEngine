#pragma once

#ifndef DX9MAP_H
#define DX9MAP_H

#include "DX9Image.h"

class DX9Map : protected DX9Image {
private:
	int	m_nTileCols;
	int	m_nTileRows;
	int	m_nTileSheetWidth;
	int	m_nTileSheetHeight;
	int m_nMapCols;
	int m_nMapRows;

public:
	DX9Map();
	~DX9Map() {};

	int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Map::Destroy();
	int DX9Map::SetTexture(std::wstring FileName);
	int DX9Map::SetTileInfo(int TileW, int TileH);
	int DX9Map::SetPosition(float OffsetX, float OffsetY);
	int DX9Map::CreateMap(int MapCols, int MapRows);
	int DX9Map::AddMapFragment(int TileID, int X, int Y);
	int DX9Map::AddEnd();
	int DX9Map::SetMapFragment(int TileID, int X, int Y);
	int DX9Map::Draw();
	// 나중에 LoadTileMap, DrawTileMap 추가하자!
};


#endif