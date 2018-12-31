#include "DX9Map.h"

DX9Map::DX9Map()
{
	m_CurrMapMode = DX9MAPMODE::TileMode;
	m_bMapCreated = false;
	m_WindowH = 0;
	m_MapCols = 0;
	m_MapRows = 0;
	m_TileSheetWidth = 0;
	m_TileSheetHeight = 0;
	m_MoveSheetWidth = 0;
	m_MoveSheetHeight = 0;

	m_bMoveTextureLoaded = false;
	m_pTextureMove = nullptr;
	m_pVBMove = nullptr;
	m_VertMoveCount = 0;

	m_Offset = D3DXVECTOR2(0.0f, 0.0f);
	m_OffsetZeroY = 0;
}

void DX9Map::Create(LPDIRECT3DDEVICE9 pDevice, int WindowHeight)
{
	m_pDevice = pDevice;

	ClearAllData();
	m_Vert.clear();
	m_Ind.clear();
	m_WindowH = WindowHeight;
}

void DX9Map::ClearAllData()
{
	DX9Image::ClearVertexAndIndexData();

	m_VertMove.clear();
	m_VertMoveCount = 0;
	m_MapData.clear();
}

void DX9Map::Destroy()
{
	if (m_pTextureMove)
	{
		m_pTextureMove->Release();
		m_pTextureMove = nullptr;
	}
	if (m_pVBMove)
	{
		m_pVBMove->Release();
		m_pVBMove = nullptr;
	}

	DX9Image::Destroy();
}

void DX9Map::SetTileTexture(std::wstring FileName)
{
	DX9Image::SetTexture(FileName);

	assert(m_Width > 0);

	if ((m_Width) && (m_Width))
	{
		m_TileName = FileName;

		m_TileSheetWidth = m_Width; // 'm_Width = TileSheetWidth' after SetTexture() being called
		m_TileSheetHeight = m_Height;
	}
}

void DX9Map::SetMoveTexture(std::wstring FileName)
{
	if (m_pTextureMove)
	{
		m_pTextureMove->Release();
		m_pTextureMove = nullptr;
	}

	std::wstring NewFileName;
	NewFileName = m_BaseDir;
	NewFileName += ASSET_DIR;
	NewFileName += FileName;

	D3DXIMAGE_INFO tImageInfo;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &tImageInfo, nullptr, &m_pTextureMove)))
		return;

	m_MoveSheetWidth = tImageInfo.Width;
	m_MoveSheetHeight = tImageInfo.Height;
	m_bMoveTextureLoaded = true;
}

void DX9Map::GetMapData(std::wstring *pStr) const
{
	wchar_t tempWC[255] = { 0 };
	*pStr = m_MapName;
	*pStr += L'#';
	_itow_s(m_MapCols, tempWC, 10);
	*pStr += tempWC;
	*pStr += L'#';
	_itow_s(m_MapRows, tempWC, 10);
	*pStr += tempWC;
	*pStr += L'#';
	*pStr += m_TileName;
	*pStr += L'#';
	*pStr += L'\n';

	int tDataID = 0;
	for (int i = 0; i < m_MapRows; i++)
	{
		for (int j = 0; j < m_MapCols; j++)
		{
			tDataID = j + (i * m_MapCols);
			GetMapDataPart(tDataID, tempWC, 255);
			*pStr += tempWC;
		}

		if (i < m_MapRows) // To avoid '\n' at the end
			*pStr += L'\n';
	}
}

void DX9Map::LoadMapFromFile(std::wstring FileName)
{
	std::wstring NewFileName;
	NewFileName = m_BaseDir;
	NewFileName += ASSET_DIR;
	NewFileName += FileName;

	std::wifstream filein;
	filein.open(NewFileName, std::wifstream::in);
	if (!filein.is_open())
		return;

	std::wstring fileText;

	wchar_t tempText[MAX_LINE_LEN];
	fileText.clear();
	while (!filein.eof()) {
		filein.getline(tempText, MAX_LINE_LEN);
		fileText += tempText;
		fileText += '\n';
	}
	fileText = fileText.substr(0, fileText.size() - 1);

	SetMapData(fileText);
	SetTileTexture(m_TileName);
	CreateLoadedMap();
}

void DX9Map::CreateNewMap(std::wstring Name, int MapCols, int MapRows)
{
	ClearAllData();

	m_MapName = Name;
	m_MapCols = MapCols;
	m_MapRows = MapRows;

	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapCols; j++)
		{
			AddMapFragmentTile(-1, j, i);
			AddMapFragmentMove(0, j, i);
			m_MapData.push_back(DX9MAPDATA(-1, 0));
		}
	}
	AddEnd();
}

//@warning: this fuction must be called in LoadMapFromFile()
void DX9Map::CreateLoadedMap()
{
	ClearAllData();

	int tTileID = 0;
	int tMoveID = 0;
	for (int i = 0; i < m_MapRows; i++)
	{
		for (int j = 0; j < m_MapCols; j++)
		{
			tTileID = _wtoi(m_MapDataInString.substr(0, MAX_TILEID_LEN).c_str());
			if (tTileID == 999)
				tTileID = -1;
			
			tMoveID = _wtoi(m_MapDataInString.substr(MAX_TILEID_LEN, MAX_MOVEID_LEN).c_str());

			AddMapFragmentTile(tTileID, j, i);
			AddMapFragmentMove(tMoveID, j, i);
			m_MapData.push_back(DX9MAPDATA(tTileID, tMoveID));

			m_MapDataInString = m_MapDataInString.substr(MAX_TILEID_LEN);
			m_MapDataInString = m_MapDataInString.substr(MAX_MOVEID_LEN);
		}
		m_MapDataInString = m_MapDataInString.substr(1); // Delete '\n' in the string data
	}
	m_MapDataInString.clear();
	AddEnd();
}

void DX9Map::AddMapFragmentTile(int TileID, int X, int Y)
{
	DX9UV tUV = ConvertIDtoUV(TileID, m_TileSheetWidth, m_TileSheetHeight);

	//@warning: UV offset is done in order to make sure the image borders do not invade contiguous images
	tUV.u1 += UV_OFFSET;
	tUV.v1 += UV_OFFSET;
	tUV.u2 -= UV_OFFSET;
	tUV.v2 -= UV_OFFSET;

	DWORD tColor;
	if (TileID == -1)
	{
		tColor = D3DCOLOR_ARGB(0, 255, 255, 255); // The tile is transparent (Alpha = 0)
	}
	else
	{
		tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	}

	float tX = (float)(X * TILE_W);
	float tY = (float)(Y * TILE_H);

	m_Vert.push_back(DX9VERTEX_IMAGE(tX, tY, 0, 1, tColor, tUV.u1, tUV.v1));
	m_Vert.push_back(DX9VERTEX_IMAGE(tX + TILE_W, tY, 0, 1, tColor, tUV.u2, tUV.v1));
	m_Vert.push_back(DX9VERTEX_IMAGE(tX, tY + TILE_H, 0, 1, tColor, tUV.u1, tUV.v2));
	m_Vert.push_back(DX9VERTEX_IMAGE(tX + TILE_W, tY + TILE_H, 0, 1, tColor, tUV.u2, tUV.v2));
	m_VertCount = (int)m_Vert.size();

	m_Ind.push_back(DX9INDEX3(m_VertCount - 4, m_VertCount - 3, m_VertCount - 1));
	m_Ind.push_back(DX9INDEX3(m_VertCount - 4, m_VertCount - 1, m_VertCount - 2));
	m_IndCount = (int)m_Ind.size();
}

void DX9Map::AddMapFragmentMove(int MoveID, int X, int Y)
{
	//@warning: This function should be called only if MoveSheet is loaded first
	if (m_MoveSheetWidth && m_MoveSheetHeight)
	{
		DX9UV tUV = ConvertIDtoUV(MoveID, m_MoveSheetWidth, m_MoveSheetHeight);

		DWORD Color = D3DCOLOR_ARGB(MOVE_ALPHA, 255, 255, 255);
		float tX = (float)(X * TILE_W);
		float tY = (float)(Y * TILE_H);

		m_VertMove.push_back(DX9VERTEX_IMAGE(tX, tY, 0, 1, Color, tUV.u1, tUV.v1));
		m_VertMove.push_back(DX9VERTEX_IMAGE(tX + TILE_W, tY, 0, 1, Color, tUV.u2, tUV.v1));
		m_VertMove.push_back(DX9VERTEX_IMAGE(tX, tY + TILE_H, 0, 1, Color, tUV.u1, tUV.v2));
		m_VertMove.push_back(DX9VERTEX_IMAGE(tX + TILE_W, tY + TILE_H, 0, 1, Color, tUV.u2, tUV.v2));
		m_VertMoveCount = (int)m_VertMove.size();
	}
}

// AddEnd() is always called after creating any kind of maps
void DX9Map::AddEnd()
{
	DX9Image::CreateVertexBuffer();
	DX9Image::CreateIndexBuffer();
	DX9Image::UpdateVertexBuffer();
	DX9Image::UpdateIndexBuffer();

	if (m_bMoveTextureLoaded)
	{
		CreateVertexBufferMove();
		UpdateVertexBufferMove();
	}

	m_bMapCreated = true;
	m_OffsetZeroY = m_WindowH - (m_MapRows * TILE_H);

	SetGlobalPosition(D3DXVECTOR2(0, 0));
}

void DX9Map::CreateVertexBufferMove()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_VertMoveCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVBMove, nullptr)))
	{
		return;
	}
}

void DX9Map::UpdateVertexBufferMove()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_VertMoveCount;
	VOID* pVertices;
	if (FAILED(m_pVBMove->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, &m_VertMove[0], rVertSize);
	m_pVBMove->Unlock();
}

void DX9Map::SetMapData(std::wstring Str)
{
	size_t tFind = -1;
	int tInt = 0;

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		m_MapName = Str.substr(0, tFind);
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		tInt = _wtoi(Str.substr(0, tFind).c_str());
		m_MapCols = tInt;
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		tInt = _wtoi(Str.substr(0, tFind).c_str());
		m_MapRows = tInt;
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		m_TileName = Str.substr(0, tFind);
		Str = Str.substr(tFind + 2);
	}

	m_MapDataInString = Str;
}

float DX9Map::GetMapTileBoundary(int MapID, DX9MAPDIR Dir) const
{
	float Result = 0.0f;

	D3DXVECTOR2 tMapXY = ConvertIDToXY(MapID);

	float tX = m_Offset.x + tMapXY.x * TILE_W;
	float tY = m_Offset.y + tMapXY.y * TILE_H;

	switch (Dir)
	{
	case DX9MAPDIR::Up:
		Result = tY;
		break;
	case DX9MAPDIR::Down:
		Result = tY + TILE_H;
		break;
	case DX9MAPDIR::Left:
		Result = tX;
		break;
	case DX9MAPDIR::Right:
		Result = tX + TILE_W;
		break;
	default:
		break;
	}

	return Result;
}

bool DX9Map::IsMovableTile(int MapID, DX9MAPDIR Dir) const
{
	if ((MapID >= (m_MapCols * m_MapRows)) || (MapID < 0))
		return true;

	int tMoveID = m_MapData[MapID].MoveID;
	switch (Dir)
	{
	case DX9MAPDIR::Up:
		if ((tMoveID == 2) || (tMoveID == 7) || (tMoveID == 8) || (tMoveID == 9) ||
			(tMoveID == 12) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case DX9MAPDIR::Down:
		if ((tMoveID == 1) || (tMoveID == 5) || (tMoveID == 6) || (tMoveID == 9) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case DX9MAPDIR::Left:
		if ((tMoveID == 4) || (tMoveID == 5) || (tMoveID == 7) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 13) || (tMoveID == 15))
			return false;
		return true;
	case DX9MAPDIR::Right:
		if ((tMoveID == 3) || (tMoveID == 6) || (tMoveID == 8) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	default:
		return false;
	}
}

DX9UV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH) const
{
	DX9UV Result;
	int tX = 0;
	int tY = 0;
	int tTileCols = (int)(SheetW / TILE_W);

	if (ID == -1)
	{
		Result = DX9UV(0, 0, 0, 0);
	}
	else
	{
		tX = (ID % tTileCols);
		tY = (ID / tTileCols);

		Result.u1 = (float)(tX * TILE_W) / (float)SheetW;
		Result.u2 = Result.u1 + (float)TILE_W / (float)SheetW;
		Result.v1 = (float)(tY * TILE_H) / (float)SheetH;
		Result.v2 = Result.v1 + (float)TILE_H / (float)SheetH;
	}

	return Result;
}

D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID) const
{
	D3DXVECTOR2 Result = D3DXVECTOR2(0, 0);

	Result.x = (FLOAT)(MapID % m_MapCols);
	Result.y = (FLOAT)(MapID / m_MapCols);

	return Result;
}

int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY) const
{
	return (int)MapXY.x + ((int)MapXY.y * m_MapCols);
}

D3DXVECTOR2 DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, bool YRoundUp) const
{
	D3DXVECTOR2 Result;

	float tX = -m_Offset.x + Position.x;
	float tY = -m_Offset.y + Position.y;

	int tYR = (int)tX % TILE_W;
	int tMapX = (int)(tX / TILE_W);
	int tMapY = (int)(tY / TILE_H);

	if (YRoundUp)
	{
		//@warning: round-up the Y value (If it gets a little bit down, it should be recognized as in the next row)
		if (tYR)
			tMapY++;
	}

	Result.x = (FLOAT)tMapX;
	Result.y = (FLOAT)tMapY;

	return Result;
}

void DX9Map::SetMode(DX9MAPMODE Mode)
{
	switch (Mode)
	{
	case DX9MAPMODE::TileMode:
		m_CurrMapMode = Mode;
		return;
	case DX9MAPMODE::MoveMode:
		if (m_bMoveTextureLoaded)
		{
			m_CurrMapMode = Mode;
			return;
		}
	default:
		break;
	}

	//@warning SetMoveTexture() should have been called first
	assert(0);
}

void DX9Map::SetPosition(D3DXVECTOR2 Offset)
{
	int VertID0 = 0;
	float tX = 0.0f;
	float tY = 0.0f;

	m_Offset = Offset;

	for (int i = 0; i < m_MapRows; i++)
	{
		for (int j = 0; j < m_MapCols; j++)
		{
			VertID0 = (j + (i * m_MapCols)) * 4;
			tX = (float)(j * TILE_W) + m_Offset.x;
			tY = (float)(i * TILE_H) + m_Offset.y;
			m_Vert[VertID0].x = tX;
			m_Vert[VertID0].y = tY;
			m_Vert[VertID0 + 1].x = tX + TILE_W;
			m_Vert[VertID0 + 1].y = tY;
			m_Vert[VertID0 + 2].x = tX;
			m_Vert[VertID0 + 2].y = tY + TILE_H;
			m_Vert[VertID0 + 3].x = tX + TILE_W;
			m_Vert[VertID0 + 3].y = tY + TILE_H;

			if (m_bMoveTextureLoaded)
			{
				m_VertMove[VertID0].x = tX;
				m_VertMove[VertID0].y = tY;
				m_VertMove[VertID0 + 1].x = tX + TILE_W;
				m_VertMove[VertID0 + 1].y = tY;
				m_VertMove[VertID0 + 2].x = tX;
				m_VertMove[VertID0 + 2].y = tY + TILE_H;
				m_VertMove[VertID0 + 3].x = tX + TILE_W;
				m_VertMove[VertID0 + 3].y = tY + TILE_H;
			}
		}
	}

	UpdateVertexBuffer();
	if (m_bMoveTextureLoaded)
	{
		UpdateVertexBufferMove();
	}
}

void DX9Map::SetGlobalPosition(D3DXVECTOR2 Offset)
{
	float MapH = (float)(m_MapRows * TILE_H);
	float NewOffsetY = m_WindowH - MapH + Offset.y;

	SetPosition(D3DXVECTOR2(Offset.x, NewOffsetY));
}

void DX9Map::SetMapFragmentTile(int TileID, int X, int Y)
{
	if ((X < m_MapCols) && (Y < m_MapRows))
	{
		int MapID = X + (Y * m_MapCols);
		int VertID0 = MapID * 4;

		DX9UV tUV = ConvertIDtoUV(TileID, m_TileSheetWidth, m_TileSheetHeight);

		//@warning: UV offset is done in order to make sure the image borders do not invade contiguous images
		tUV.u1 += UV_OFFSET;
		tUV.v1 += UV_OFFSET;
		tUV.u2 -= UV_OFFSET;
		tUV.v2 -= UV_OFFSET;

		DWORD tColor;
		if (TileID == -1)
		{
			tColor = D3DCOLOR_ARGB(0, 255, 255, 255);
		}
		else
		{
			tColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		}

		m_Vert[VertID0].u = tUV.u1;
		m_Vert[VertID0].v = tUV.v1;
		m_Vert[VertID0].color = tColor;
		m_Vert[VertID0 + 1].u = tUV.u2;
		m_Vert[VertID0 + 1].v = tUV.v1;
		m_Vert[VertID0 + 1].color = tColor;
		m_Vert[VertID0 + 2].u = tUV.u1;
		m_Vert[VertID0 + 2].v = tUV.v2;
		m_Vert[VertID0 + 2].color = tColor;
		m_Vert[VertID0 + 3].u = tUV.u2;
		m_Vert[VertID0 + 3].v = tUV.v2;
		m_Vert[VertID0 + 3].color = tColor;

		m_MapData[MapID].TileID = TileID;

		UpdateVertexBuffer();
	}
}

void DX9Map::SetMapFragmentMove(int MoveID, int X, int Y)
{
	if ((X < m_MapCols) && (Y < m_MapRows))
	{
		int MapID = X + (Y * m_MapCols);
		int VertID0 = MapID * 4;

		DX9UV tUV = ConvertIDtoUV(MoveID, m_MoveSheetWidth, m_MoveSheetHeight);

		m_VertMove[VertID0].u = tUV.u1;
		m_VertMove[VertID0].v = tUV.v1;
		m_VertMove[VertID0 + 1].u = tUV.u2;
		m_VertMove[VertID0 + 1].v = tUV.v1;
		m_VertMove[VertID0 + 2].u = tUV.u1;
		m_VertMove[VertID0 + 2].v = tUV.v2;
		m_VertMove[VertID0 + 3].u = tUV.u2;
		m_VertMove[VertID0 + 3].v = tUV.v2;

		m_MapData[MapID].MoveID = MoveID;

		UpdateVertexBufferMove();
	}
}

void DX9Map::Draw() const
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (m_pTexture)
	{
		m_pDevice->SetTexture(0, m_pTexture);

		// Texture alpha * Diffuse color alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(DX9VERTEX_IMAGE));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertCount, 0, m_IndCount);
	}

	if ((m_CurrMapMode == DX9MAPMODE::MoveMode) && m_pTextureMove)
	{
		m_pDevice->SetTexture(0, m_pTextureMove);

		// Texture alpha * Diffuse color alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetStreamSource(0, m_pVBMove, 0, sizeof(DX9VERTEX_IMAGE));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertMoveCount, 0, m_IndCount);
	}
}

int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size) const
{
	std::wstring tempStr;
	wchar_t tempWC[255] = { 0 };

	int tTileID = m_MapData[DataID].TileID;
	if (tTileID == -1) tTileID = 999;
	_itow_s(tTileID, tempWC, 10);
	size_t tempLen = wcslen(tempWC);

	switch (tempLen)
	{
	case 1:
		tempStr = L"00";
		tempStr += tempWC;
		break;
	case 2:
		tempStr = L"0";
		tempStr += tempWC;
		break;
	case 3:
		tempStr = tempWC;
		break;
	default:
		return -1;
	}

	int tMoveID = m_MapData[DataID].MoveID;
	_itow_s(tMoveID, tempWC, 10);
	tempLen = wcslen(tempWC);

	switch (tempLen)
	{
	case 1:
		tempStr += L"0";
		tempStr += tempWC;
		break;
	case 2:
		tempStr += tempWC;
		break;
	default:
		return -1;
	}

	wcscpy_s(WC, size, tempStr.c_str());
	return 0;
}

bool DX9Map::IsMapCreated() const
{ 
	return m_bMapCreated;
};

int DX9Map::GetMapName(std::wstring *pStr) const
{
	*pStr = m_MapName;
	return 0;
}

int DX9Map::GetTileName(std::wstring *pStr) const
{
	*pStr = m_TileName;
	return 0;
}

int DX9Map::GetMapCols() const 
{ 
	return m_MapCols;
}

int DX9Map::GetMapRows() const
{
	return m_MapRows;
}

int DX9Map::GetWidth() const 
{ 
	return (m_MapCols * TILE_W);
}

int DX9Map::GetHeight() const
{ 
	return (m_MapRows * TILE_H);
}

D3DXVECTOR2	DX9Map::GetMapOffset() const 
{ 
	return m_Offset;
}

int DX9Map::GetMapOffsetZeroY() const
{ 
	return m_OffsetZeroY;
}

D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(DX9BOUNDINGBOX BB, D3DXVECTOR2 Velocity) const
{
	D3DXVECTOR2 NewVelocity = Velocity;

	D3DXVECTOR2 tSprPosS;
	D3DXVECTOR2 tSprPosE;
	D3DXVECTOR2 tMapXYS;
	D3DXVECTOR2 tMapXYE;

	if (Velocity.x > 0)
	{
		// Go Right
		tSprPosS = BB.PosOffset;
		tSprPosS.x += BB.Size.x; // ¦¤ (Right Up)
		tSprPosE = tSprPosS;
		tSprPosE.x += Velocity.x;
		tSprPosE.y += BB.Size.y;

		tMapXYS = ConvertPositionToXY(tSprPosS);
		tMapXYE = ConvertPositionToXY(tSprPosE);

		int tXS = (int)tMapXYS.x;
		int tYS = (int)tMapXYS.y;
		int tXE = (int)tMapXYE.x;
		int tYE = (int)tMapXYE.y;

		float fWall = 0.0f;
		float fWallCmp = 0.0f;

		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2((FLOAT)i, (FLOAT)j));
				if (IsMovableTile(tMapID, DX9MAPDIR::Right) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, DX9MAPDIR::Left);
					if (fWall == 0)
					{
						fWall = fWallCmp;
					}
					else if (fWall && fWallCmp)
					{
						fWall = min(fWall, fWallCmp);
					}
				}
			}
		}

		if (fWall)
		{
			float fCurr = tSprPosS.x + Velocity.x;
			float fDist = fWall - tSprPosS.x - 0.1f;
			NewVelocity.x = fDist;
		}
	}
	else if (Velocity.x < 0)
	{
		// Go Left
		tSprPosS = BB.PosOffset; // ¦£ (Left Up)
		tSprPosE = tSprPosS;
		tSprPosE.x += Velocity.x;
		tSprPosE.y += BB.Size.y;

		tMapXYS = ConvertPositionToXY(tSprPosS);
		tMapXYE = ConvertPositionToXY(tSprPosE);

		int tXS = (int)tMapXYS.x;
		int tYS = (int)tMapXYS.y;
		int tXE = (int)tMapXYE.x;
		int tYE = (int)tMapXYE.y;

		float fWall = 0.0f;
		float fWallCmp = 0.0f;

		for (int i = tXS; i >= tXE; i--)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2((FLOAT)i, (FLOAT)j));
				if (IsMovableTile(tMapID, DX9MAPDIR::Left) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, DX9MAPDIR::Right);
					if (fWall == 0)
					{
						fWall = fWallCmp;
					}
					else if (fWall && fWallCmp)
					{
						fWall = max(fWall, fWallCmp);
					}
				}
			}
		}

		if (fWall)
		{
			float fCurr = tSprPosS.x + Velocity.x;
			float fDist = fWall - tSprPosS.x;
			NewVelocity.x = fDist;
		}
	}
	else if (Velocity.y > 0)
	{
		// Go Down
		tSprPosS = BB.PosOffset;
		tSprPosS.y += BB.Size.y; // ¦¦ (Left Down)
		tSprPosE = tSprPosS;
		tSprPosE.y += Velocity.y;
		tSprPosE.x += BB.Size.x;

		tMapXYS = ConvertPositionToXY(tSprPosS, true);
		tMapXYE = ConvertPositionToXY(tSprPosE, true);

		int tXS = (int)tMapXYS.x;
		int tYS = (int)tMapXYS.y;
		int tXE = (int)tMapXYE.x;
		int tYE = (int)tMapXYE.y;

		float fWall = 0.0f;
		float fWallCmp = 0.0f;

		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2((FLOAT)i, (FLOAT)j));
				if (IsMovableTile(tMapID, DX9MAPDIR::Down) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, DX9MAPDIR::Up);
					if (fWall == 0)
					{
						fWall = fWallCmp;
					}
					else if (fWall && fWallCmp)
					{
						fWall = min(fWall, fWallCmp);
					}
				}
			}

			if (fWall)
			{
				float fCurr = tSprPosS.y + Velocity.y;
				float fDist = fWall - tSprPosS.y - 0.1f;
				NewVelocity.y = min(NewVelocity.y, fDist);
			}
		}
	}
	else if (Velocity.y < 0)
	{
		// Go Up
		tSprPosS = BB.PosOffset; // ¦£ (Left Up)
		tSprPosE = tSprPosS;
		tSprPosE.y += Velocity.y;
		tSprPosE.x += BB.Size.x;

		tMapXYS = ConvertPositionToXY(tSprPosS);
		tMapXYE = ConvertPositionToXY(tSprPosE);

		int tXS = (int)tMapXYS.x;
		int tYS = (int)tMapXYS.y;
		int tXE = (int)tMapXYE.x;
		int tYE = (int)tMapXYE.y;

		float fWall = 0.0f;
		float fWallCmp = 0.0f;

		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j >= tYE; j--)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2((FLOAT)i, (FLOAT)j));
				if (IsMovableTile(tMapID, DX9MAPDIR::Up) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, DX9MAPDIR::Down);
					if (fWall == 0)
					{
						fWall = fWallCmp;
					}
					else if (fWall && fWallCmp)
					{
						fWall = max(fWall, fWallCmp);
					}
				}
			}
		}

		if (fWall)
		{
			float fCurr = tSprPosS.y + Velocity.y;
			float fDist = fWall - tSprPosS.y;
			NewVelocity.y = max(NewVelocity.y, fDist);
		}
	}
	return NewVelocity;
}