#pragma once

#include <fstream>
#include "DX9Image.h"

const int MAX_LINE_LEN = 1024;
const int MAX_TILEID_LEN = 3;
const int MAX_MOVEID_LEN = 2;
const int TILE_W = 32;
const int TILE_H = 32;
const int MOVE_ALPHA = 100;

class DX9Map final : protected DX9Image
{
private:
	DX9MAPMODE m_CurrMapMode; // For Map Editor
	bool m_bMapCreated;
	int m_MapCols;
	int m_MapRows;
	int	m_TileSheetWidth;
	int	m_TileSheetHeight;
	int	m_MoveSheetWidth;
	int	m_MoveSheetHeight;
	WSTRING m_MapName;
	WSTRING m_TileName;
	WSTRING m_MapDataInString;
	std::vector<DX9MAPDATA> m_MapData;

	bool m_bMoveTextureLoaded;
	LPDIRECT3DTEXTURE9 m_pTextureMove;
	LPDIRECT3DVERTEXBUFFER9 m_pVBMove;
	std::vector<DX9VERTEX_IMAGE> m_VertMove;
	int m_VertMoveCount;

	D3DXVECTOR2 m_Offset; // For map movement
	int m_OffsetZeroY; // For map movement (& inversed Y values)

private:
	void DX9Map::ClearAllData();

	void DX9Map::CreateLoadedMap();
	void DX9Map::AddMapFragmentTile(int TileID, int X, int Y);
	void DX9Map::AddMapFragmentMove(int MoveID, int X, int Y);
	void DX9Map::AddEnd();
	void DX9Map::CreateVertexBufferMove(); // IndexBuffer is not needed because they are the same
	void DX9Map::UpdateVertexBufferMove();
	void DX9Map::SetMapData(WSTRING Str); // For loading maps

	int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size) const;
	bool DX9Map::IsMovableTile(int MapID, DX9MAPDIR Direction) const;
	float DX9Map::GetMapTileBoundary(int MapID, DX9MAPDIR Direction) const;
	DX9UV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH) const;
	D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID) const;
	int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY) const;
	D3DXVECTOR2 DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, bool YRoundUp = false) const;

public:
	DX9Map();
	virtual ~DX9Map() {};

	void DX9Map::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData);
	void DX9Map::Destroy() override;
	
	void DX9Map::CreateNewMap(WSTRING Name, int MapCols, int MapRows);
	void DX9Map::SetTileTexture(WSTRING FileName);
	void DX9Map::SetMoveTexture(WSTRING FileName);
	void DX9Map::LoadMapFromFile(WSTRING FileName);
	void DX9Map::GetMapData(WSTRING *pOutputString) const; // For saving the map

	void DX9Map::Draw() const override;

	void DX9Map::SetMode(DX9MAPMODE Mode);
	void DX9Map::SetPosition(D3DXVECTOR2 Offset);
	void DX9Map::SetMapFragmentTile(int TileID, int X, int Y);
	void DX9Map::SetMapFragmentMove(int MoveID, int X, int Y);
	void DX9Map::SetGlobalPosition(D3DXVECTOR2 Offset); // For map movement

	bool DX9Map::IsMapCreated() const;
	int DX9Map::GetMapName(WSTRING *pStr) const;
	int DX9Map::GetTileName(WSTRING *pStr) const;
	int DX9Map::GetMapCols() const;
	int DX9Map::GetMapRows() const;
	int DX9Map::GetWidth() const override;
	int DX9Map::GetHeight() const override;
	D3DXVECTOR2	DX9Map::GetMapOffset() const;
	int DX9Map::GetMapOffsetZeroY() const;
	D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(DX9BOUNDINGBOX BB, D3DXVECTOR2 Velocity) const;
};