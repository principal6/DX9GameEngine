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

	int m_WindowH;
	int m_MapCols;
	int m_MapRows;
	int	m_TileSheetWidth;
	int	m_TileSheetHeight;
	int	m_MoveSheetWidth;
	int	m_MoveSheetHeight;
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

	int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir, int WindowHeight);
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
	int DX9Map::SetGlobalPosition(float OffsetX, float OffsetY);

	// Load & Save
	int DX9Map::LoadMapFromFile(std::wstring FileName);
	int DX9Map::GetMapData(std::wstring *pStr);

	// Getter
	int DX9Map::GetMapName(std::wstring *pStr);
	int DX9Map::GetTileName(std::wstring *pStr);
	int DX9Map::GetMapCols() { return m_MapCols; };
	int DX9Map::GetMapRows() { return m_MapRows; };
	int DX9Map::GetWidth() { return (m_MapCols * TILE_W); };
	int DX9Map::GetHeight() { return (m_MapRows * TILE_H); };

	// Converter
	D3DXVECTOR2 DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, bool YRoundUp = false);
	D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID);
	int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY);

	// Sprite Collision
	D3DXVECTOR2 DX9Map::CheckSprCollision(D3DXVECTOR2 SprPos, D3DXVECTOR2 Velocity);
	D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(DX9BOUNDINGBOX BB, D3DXVECTOR2 Velocity);
};


#endif