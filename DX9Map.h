#pragma once

#include <fstream>
#include "Core/DX9Image.h"

namespace DX9ENGINE
{
	struct MapInfo
	{
		WSTRING MapName;
		WSTRING TileName;
		WSTRING MoveName;

		int MapRows;
		int MapCols;
		int MapWidth;
		int MapHeight;
		int TileSize;
		int TileSheetWidth;
		int TileSheetHeight;
		int MoveSheetWidth;
		int MoveSheetHeight;

		MapInfo() : MapRows(0), MapCols(0), TileSize(0) {};
	};

	class DX9Map final : protected DX9Image
	{
	private:
		struct MapData
		{
			int TileID;
			int MoveID;

			MapData() : TileID(0), MoveID(0) {};
			MapData(int TILEID, int MOVEID) : TileID(TILEID), MoveID(MOVEID) {};
		};

		enum class Direction
		{
			Up,
			Down,
			Left,
			Right,
		};

	private:
		static const int MAX_LINE_LEN;
		static const int MAX_TILEID_LEN;
		static const int MAX_MOVEID_LEN;
		static const int MOVE_ALPHA;
		static const int DEF_TILE_SIZE;
		static const int DEPTH_HELL;

		static const wchar_t* MOVE_32;
		static const wchar_t* MOVE_64;

		MapMode m_CurrMapMode; // For Map Editor
		bool m_bMapCreated;

		MapInfo m_MapInfo;
		VECTOR<MapData> m_MapData;

		bool m_bMoveTextureLoaded;
		LPDIRECT3DTEXTURE9 m_pTextureMove;
		LPDIRECT3DVERTEXBUFFER9 m_pVBMove;
		VECTOR<VertexImage> m_VertMove;

		D3DXVECTOR2 m_Offset; // For map movement
		int m_OffsetZeroY; // For map movement (& inversed Y values)

	private:
		static auto DX9Map::ConvertIDtoUV(int ID, int TileSize, int SheetW, int SheetH)->FloatUV;
		static auto DX9Map::ConvertIDToXY(int MapID, int MapCols)->D3DXVECTOR2;
		static auto DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY, int MapCols)->int;
		static auto DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, D3DXVECTOR2 Offset,
			int TileSize, bool YRoundUp = false)->D3DXVECTOR2;

	private:
		void DX9Map::ClearAllData();

		void DX9Map::ParseMapData(WSTRING Str); // For loading maps
		void DX9Map::CreateLoadedMap(WSTRING Data); // For loading maps
		void DX9Map::GetMapDataForSave(WSTRING *pOutputString) const; // For saving maps
		void DX9Map::GetMapDataPartForSave(int DataID, wchar_t *WC, int size) const; // For saving maps

		void DX9Map::CreateMapBase();
		void DX9Map::AddMapFragmentTile(int TileID, int X, int Y);
		void DX9Map::AddMapFragmentMove(int MoveID, int X, int Y);
		void DX9Map::AddEnd();

		void DX9Map::CreateVertexBufferMove(); // IndexBuffer is not needed because they are the same
		void DX9Map::UpdateVertexBufferMove();

		auto DX9Map::IsMovableTile(int MapID, Direction Direction) const->bool;
		auto DX9Map::GetMapTileBoundary(int MapID, Direction Direction) const->float;

	public:
		DX9Map();
		~DX9Map() {};

		auto DX9Map::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData)->Error;
		void DX9Map::Destroy() override;

		void DX9Map::CreateNewMap(MapInfo* Info);
		void DX9Map::LoadMap(WSTRING FileName);
		void DX9Map::SaveMap(WSTRING FileName);
		void DX9Map::SetTileTexture(WSTRING FileName);
		void DX9Map::SetMoveTexture(WSTRING FileName);

		void DX9Map::Draw() override;

		void DX9Map::SetMode(MapMode Mode);
		void DX9Map::SetPosition(D3DXVECTOR2 Offset);
		void DX9Map::SetMapFragmentTile(int TileID, int X, int Y);
		void DX9Map::SetMapFragmentMove(int MoveID, int X, int Y);
		void DX9Map::SetGlobalPosition(D3DXVECTOR2 Offset); // For map movement

		auto DX9Map::IsMapCreated() const->bool;
		auto DX9Map::GetMode() const->MapMode;
		void DX9Map::GetMapInfo(MapInfo *pInfo) const;
		auto DX9Map::GetMapOffset() const->D3DXVECTOR2;
		auto DX9Map::GetMapOffsetZeroY() const->int;
		auto DX9Map::GetVelocityAfterCollision(BoundingBox BB, D3DXVECTOR2 Velocity) const->D3DXVECTOR2;
	};
};