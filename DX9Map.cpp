#include "DX9Map.h"

DX9Map::DX9Map() {
	m_nTileCols = 0;
	m_nTileRows = 0;
	m_nTileSheetWidth = 0;
	m_nTileSheetHeight = 0;
}

int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev) {
	// 멤버 변수 초기화
	m_pDevice = pD3DDev;

	m_pVB = NULL;
	m_pIB = NULL;
	m_pTexture = NULL;

	m_Vert.clear();
	m_Ind.clear();

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;

	m_nWidth = 32;
	m_nHeight = 32;

	return 0;
}


int DX9Map::Destroy() {
	DX9Image::Destroy();
	return 0;
}

int DX9Map::SetTexture(std::wstring FileName) {
	DX9Image::SetTexture(FileName);
	
	return 0;
}

int DX9Map::SetTileInfo(int TileW, int TileH) {
	m_nTileSheetWidth = m_nWidth;
	m_nTileSheetHeight = m_nHeight;

	m_nWidth = TileW;
	m_nHeight = TileH;

	m_nTileCols = (int)(m_nTileSheetWidth / m_nWidth);
	m_nTileRows = (int)(m_nTileSheetHeight / m_nHeight);

	return 0;
}

int DX9Map::SetPosition(float OffsetX, float OffsetY) {
	
	int VertID0 = 0;
	float tX = 0.0f;
	float tY = 0.0f;

	for (int i = 0; i < m_nMapRows; i++)
	{
		for (int j = 0; j < m_nMapCols; j++)
		{
			VertID0 = (j + (i * m_nMapCols)) * 4;
			tX = (float)(j * m_nWidth) + OffsetX;
			tY = (float)(i * m_nHeight) + OffsetY;
			m_Vert[VertID0].x = tX;
			m_Vert[VertID0].y = tY;
			m_Vert[VertID0 + 1].x = tX + m_nWidth;
			m_Vert[VertID0 + 1].y = tY;
			m_Vert[VertID0 + 2].x = tX;
			m_Vert[VertID0 + 2].y = tY + m_nHeight;
			m_Vert[VertID0 + 3].x = tX + m_nWidth;
			m_Vert[VertID0 + 3].y = tY + m_nHeight;
		}
	}

	UpdateVB();
	return 0;
}

int DX9Map::CreateMap(int MapCols, int MapRows) {
	if (!m_nTileRows) // 타일이 아직 안 열림
		return -1;

	m_Vert.clear();
	m_Ind.clear();

	m_nMapCols = MapCols;
	m_nMapRows = MapRows;

	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapCols; j++)
		{
			AddMapFragment(-1, j, i);
		}
	}
	AddEnd();

	return 0;
}

int DX9Map::AddMapFragment(int TileID, int X, int Y) {
	float u1;
	float u2;
	float v1;
	float v2;

	if (TileID == -1)
	{
		u1 = 0.0f;
		u2 = 0.0f;
		v1 = 0.0f;
		v2 = 0.0f;
	}
	else
	{
		int TileX = (TileID % m_nTileRows);
		int TileY = (TileID / m_nTileRows);

		u1 = (float)(TileX * m_nWidth) / m_nTileSheetWidth;
		u2 = u1 + (float)m_nWidth / m_nTileSheetWidth;
		v1 = (float)(TileY * m_nHeight) / m_nTileSheetHeight;
		v2 = v1 + (float)m_nHeight / m_nTileSheetHeight;
	}

	float tX = (float)(X * m_nWidth);
	float tY = (float)(Y * m_nHeight);

	m_Vert.push_back(DX9VERTEX(tX, tY, 0, 1, 0xffffffff, u1, v1));
	m_Vert.push_back(DX9VERTEX(tX + m_nWidth, tY, 0, 1, 0xffffffff, u2, v1));
	m_Vert.push_back(DX9VERTEX(tX, tY + m_nHeight, 0, 1, 0xffffffff, u1, v2));
	m_Vert.push_back(DX9VERTEX(tX + m_nWidth, tY + m_nHeight, 0, 1, 0xffffffff, u2, v2));
	m_nVertCount = (int)m_Vert.size();

	m_Ind.push_back(DX9INDEX(m_nVertCount - 4, m_nVertCount - 3, m_nVertCount - 1));
	m_Ind.push_back(DX9INDEX(m_nVertCount - 4, m_nVertCount - 1, m_nVertCount - 2));
	m_nIndCount = (int)m_Ind.size();

	return 0;
}

int DX9Map::AddEnd() {
	DX9Image::CreateVB();
	DX9Image::CreateIB();
	DX9Image::UpdateVB();

	return 0;
}

int DX9Map::SetMapFragment(int TileID, int X, int Y) {
	if ((X < m_nMapCols) && (Y < m_nMapRows))
	{
		int VertID0 = (X + (Y * m_nMapCols)) * 4;

		float u1;
		float u2;
		float v1;
		float v2;

		if (TileID == -1)
		{
			u1 = 0.0f;
			u2 = 0.0f;
			v1 = 0.0f;
			v2 = 0.0f;
		}
		else
		{
			int TileX = (TileID % m_nTileCols);
			int TileY = (TileID / m_nTileCols);

			u1 = (float)(TileX * m_nWidth) / m_nTileSheetWidth;
			u2 = u1 + (float)m_nWidth / m_nTileSheetWidth;
			v1 = (float)(TileY * m_nHeight) / m_nTileSheetHeight;
			v2 = v1 + (float)m_nHeight / m_nTileSheetHeight;
		}

		m_Vert[VertID0].u = u1;
		m_Vert[VertID0].v = v1;
		m_Vert[VertID0 + 1].u = u2;
		m_Vert[VertID0 + 1].v = v1;
		m_Vert[VertID0 + 2].u = u1;
		m_Vert[VertID0 + 2].v = v2;
		m_Vert[VertID0 + 3].u = u2;
		m_Vert[VertID0 + 3].v = v2;

		UpdateVB();

		return 0;
	}

	return -1;
}

int DX9Map::Draw() {
	DX9Image::Draw();
	return 0;
}