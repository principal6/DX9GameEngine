#pragma once

#ifndef DX9MAP_H
#define DX9MAP_H

#include <fstream>
#include "DX9Image.h"

const int MAX_LINE_LEN = 1024;

const int MAX_TILEID_LEN = 3;
const int MAX_MOVEID_LEN = 2;

const int TILE_W = 32;
const int TILE_H = 32;

const int MOVE_ALPHA = 100;

enum class DXMAPDIR {
	Up,
	Down,
	Left,
	Right,
};

struct DXUV {
	float u1;
	float u2;
	float v1;
	float v2;
	DXUV() : u1(0), u2(0), v1(0), v2(0) {};
	DXUV(float U1, float U2, float V1, float V2) : u1(U1), u2(U2), v1(V1), v2(V2) {};
};

struct DXMAPDATA {
	int TileID;
	int MoveID;
	DXMAPDATA(int TILEID, int MOVEID) : TileID(TILEID), MoveID(MOVEID) {};
};

class DX9Map : protected DX9Image {
private:
	bool		m_bMapCreated;
	DX9MAPMODE	m_CurrMapMode;

	int m_nMapCols;
	int m_nMapRows;
	int	m_nTileSheetWidth;
	int	m_nTileSheetHeight;
	int	m_nMoveSheetWidth;
	int	m_nMoveSheetHeight;
	std::wstring m_strMapName;
	std::wstring m_strTileName;
	std::wstring m_strLoadedMapTiles;
	std::vector<DXMAPDATA> m_MapData;

	bool m_bMoveTextureLoaded;
	LPDIRECT3DTEXTURE9		m_pTextureMove;
	LPDIRECT3DVERTEXBUFFER9 m_pVBMove;
	std::vector<DX9VERTEX_IMAGE>	m_VertMove;
	int						m_nVertMoveCount;

	float m_fOffsetX;
	float m_fOffsetY;

private:
	int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size);
	DXUV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH);
	int DX9Map::CreateVBMove();
	int DX9Map::UpdateVBMove();
	int DX9Map::AddMapFragmentTile(int TileID, int X, int Y);
	int DX9Map::AddMapFragmentMove(int MoveID, int X, int Y);
	int DX9Map::AddEnd();
	int DX9Map::SetMapData(std::wstring Str);

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
	D3DXVECTOR2 DX9Map::GetMapXYFromPosition(float ScreenX, float ScreenY);

	// Asker
	bool DX9Map::IsMovableTile(int MapID, DXMAPDIR Dir);
	bool DX9Map::IsAbleToMove(DXMAPDIR Dir, float ScreenX, float ScreenY, float Stride,
		float *dX, float *dY);
};


#endif