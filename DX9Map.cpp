#include "DX9Map.h"

// Static member variables declaration
const int DX9Map::MAX_LINE_LEN = 1024;
const int DX9Map::MAX_TILEID_LEN = 3;
const int DX9Map::MAX_MOVEID_LEN = 2;
const int DX9Map::MOVE_ALPHA = 100;
const int DX9Map::DEF_TILE_SIZE = 32;

/*-----------------------------------------------------------------------------
	Static method declaration
-----------------------------------------------------------------------------*/
DX9Common::FloatUV DX9Map::ConvertIDtoUV(int ID, int TileSize, int SheetW, int SheetH)
{
	FloatUV Result;
	int tTileCols, tTileRows;
	DX9Common::GetTileCols(SheetW, TileSize, &tTileCols);
	DX9Common::GetTileRows(SheetH, TileSize, &tTileRows);

	if (ID == -1)
	{
		Result = FloatUV(0, 0, 0, 0);
	}
	else
	{
		DX9Common::ConvertFrameIDIntoUV(ID, tTileCols, tTileRows, &Result);
	}

	return Result;
}

D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID, int MapCols)
{
	D3DXVECTOR2 Result = D3DXVECTOR2(0, 0);

	Result.x = static_cast<float>(MapID % MapCols);
	Result.y = static_cast<float>(MapID / MapCols);

	return Result;
}

int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY, int MapCols)
{
	return static_cast<int>(MapXY.x) + (static_cast<int>(MapXY.y) * MapCols);
}

D3DXVECTOR2 DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, D3DXVECTOR2 Offset, int TileSize, bool YRoundUp)
{
	D3DXVECTOR2 Result;

	float tX = -Offset.x + Position.x;
	float tY = -Offset.y + Position.y;

	int tYR = static_cast<int>(tX) % TileSize;
	int tMapX = static_cast<int>(tX / TileSize);
	int tMapY = static_cast<int>(tY / TileSize);

	if (YRoundUp)
	{
		//@warning: round-up the Y value (If it gets a little bit down, it should be recognized as in the next row)
		if (tYR)
			tMapY++;
	}

	Result.x = static_cast<float>(tMapX);
	Result.y = static_cast<float>(tMapY);

	return Result;
}

/*-----------------------------------------------------------------------------
	Non-static method declaration
-----------------------------------------------------------------------------*/
DX9Map::DX9Map()
{
	m_CurrMapMode = MapMode::TileMode;
	m_bMapCreated = false;
	m_TileSize = DEF_TILE_SIZE;
	m_MapCols = 0;
	m_MapRows = 0;
	m_TileSheetWidth = 0;
	m_TileSheetHeight = 0;
	m_MoveSheetWidth = 0;
	m_MoveSheetHeight = 0;

	m_bMoveTextureLoaded = false;
	m_pTextureMove = nullptr;
	m_pVBMove = nullptr;

	m_Offset = D3DXVECTOR2(0.0f, 0.0f);
	m_OffsetZeroY = 0;
}

DX9Common::ReturnValue DX9Map::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	m_pDevice = pDevice;

	ClearAllData();
	m_Vertices.clear();
	m_Indices.clear();

	return ReturnValue::OK;
}

void DX9Map::ClearAllData()
{
	DX9Image::ClearVertexAndIndexData();

	m_VertMove.clear();
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

void DX9Map::SetTileTexture(WSTRING FileName)
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

void DX9Map::SetMoveTexture(WSTRING FileName)
{
	if (m_pTextureMove)
	{
		m_pTextureMove->Release();
		m_pTextureMove = nullptr;
	}

	WSTRING NewFileName;
	NewFileName = m_WindowData.AppDir;
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

void DX9Map::LoadMap(WSTRING FileName)
{
	WSTRING NewFileName;
	NewFileName = m_WindowData.AppDir;
	NewFileName += ASSET_DIR;
	NewFileName += FileName;

	WIFSTREAM filein;
	filein.open(NewFileName, WIFSTREAM::in);

	if (!filein.is_open())
		return;

	WSTRING fileText;
	wchar_t tempText[MAX_LINE_LEN];
	while (!filein.eof())
	{
		filein.getline(tempText, MAX_LINE_LEN);
		fileText += tempText;
		fileText += '\n';
	}
	fileText = fileText.substr(0, fileText.size() - 1);

	ParseMapData(fileText);
}

void DX9Map::SaveMap(WSTRING FileName)
{
	WOFSTREAM fileout;
	fileout.open(FileName, WOFSTREAM::out);

	if (!fileout.is_open())
		return;

	WSTRING FileText;
	GetMapData(&FileText);

	fileout.write(FileText.c_str(), FileText.size());
}

void DX9Map::CreateMap(WSTRING Name, int MapCols, int MapRows)
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
			m_MapData.push_back(MapData(-1, 0));
		}
	}
	AddEnd();
}

//@warning: this fuction must be called in LoadMap()
void DX9Map::CreateLoadedMap(WSTRING Data)
{
	ClearAllData();

	int tTileID = 0;
	int tMoveID = 0;
	for (int i = 0; i < m_MapRows; i++)
	{
		for (int j = 0; j < m_MapCols; j++)
		{
			tTileID = _wtoi(Data.substr(0, MAX_TILEID_LEN).c_str());
			if (tTileID == 999)
				tTileID = -1;
			
			tMoveID = _wtoi(Data.substr(MAX_TILEID_LEN, MAX_MOVEID_LEN).c_str());

			AddMapFragmentTile(tTileID, j, i);
			AddMapFragmentMove(tMoveID, j, i);
			m_MapData.push_back(MapData(tTileID, tMoveID));

			Data = Data.substr(MAX_TILEID_LEN);
			Data = Data.substr(MAX_MOVEID_LEN);
		}
		Data = Data.substr(1); // Delete '\n' in the string data
	}
	Data.clear();
	AddEnd();
}

void DX9Map::AddMapFragmentTile(int TileID, int X, int Y)
{
	FloatUV tUV = ConvertIDtoUV(TileID, m_TileSize, m_TileSheetWidth, m_TileSheetHeight);

	//@warning: FloatUV offset is done in order to make sure the image borders do not invade contiguous images
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

	float tX = static_cast<float>(X * m_TileSize);
	float tY = static_cast<float>(Y * m_TileSize);

	m_Vertices.push_back(VertexImage(tX, tY, 0, 1, tColor, tUV.u1, tUV.v1));
	m_Vertices.push_back(VertexImage(tX + m_TileSize, tY, 0, 1, tColor, tUV.u2, tUV.v1));
	m_Vertices.push_back(VertexImage(tX, tY + m_TileSize, 0, 1, tColor, tUV.u1, tUV.v2));
	m_Vertices.push_back(VertexImage(tX + m_TileSize, tY + m_TileSize, 0, 1, tColor, tUV.u2, tUV.v2));
	int tVertCount = static_cast<int>(m_Vertices.size());

	m_Indices.push_back(Index3(tVertCount - 4, tVertCount - 3, tVertCount - 1));
	m_Indices.push_back(Index3(tVertCount - 4, tVertCount - 1, tVertCount - 2));
}

void DX9Map::AddMapFragmentMove(int MoveID, int X, int Y)
{
	//@warning: This function should be called only if MoveSheet is loaded first
	if (m_MoveSheetWidth && m_MoveSheetHeight)
	{
		FloatUV tUV = ConvertIDtoUV(MoveID, m_TileSize, m_MoveSheetWidth, m_MoveSheetHeight);

		DWORD Color = D3DCOLOR_ARGB(MOVE_ALPHA, 255, 255, 255);
		float tX = static_cast<float>(X * m_TileSize);
		float tY = static_cast<float>(Y * m_TileSize);

		m_VertMove.push_back(VertexImage(tX, tY, 0, 1, Color, tUV.u1, tUV.v1));
		m_VertMove.push_back(VertexImage(tX + m_TileSize, tY, 0, 1, Color, tUV.u2, tUV.v1));
		m_VertMove.push_back(VertexImage(tX, tY + m_TileSize, 0, 1, Color, tUV.u1, tUV.v2));
		m_VertMove.push_back(VertexImage(tX + m_TileSize, tY + m_TileSize, 0, 1, Color, tUV.u2, tUV.v2));
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
	m_OffsetZeroY = m_WindowData.WindowHeight - (m_MapRows * m_TileSize);

	SetGlobalPosition(D3DXVECTOR2(0, 0));
}

void DX9Map::CreateVertexBufferMove()
{
	int rVertSize = sizeof(VertexImage) * static_cast<int>(m_VertMove.size());
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVBMove, nullptr)))
	{
		return;
	}
}

void DX9Map::UpdateVertexBufferMove()
{
	int rVertSize = sizeof(VertexImage) * static_cast<int>(m_VertMove.size());
	VOID* pVertices;
	if (FAILED(m_pVBMove->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return;
	memcpy(pVertices, &m_VertMove[0], rVertSize);
	m_pVBMove->Unlock();
}

void DX9Map::ParseMapData(WSTRING Str)
{
	size_t tempFind = -1;
	int tempInt = 0;

	tempFind = Str.find_first_of('#');
	if (tempFind)
	{
		m_MapName = Str.substr(0, tempFind);
		Str = Str.substr(tempFind + 1);
	}

	tempFind = Str.find_first_of('#');
	if (tempFind)
	{
		tempInt = _wtoi(Str.substr(0, tempFind).c_str());
		m_TileSize = tempInt;
		Str = Str.substr(tempFind + 1);
	}

	tempFind = Str.find_first_of('#');
	if (tempFind)
	{
		tempInt = _wtoi(Str.substr(0, tempFind).c_str());
		m_MapCols = tempInt;
		Str = Str.substr(tempFind + 1);
	}

	tempFind = Str.find_first_of('#');
	if (tempFind)
	{
		tempInt = _wtoi(Str.substr(0, tempFind).c_str());
		m_MapRows = tempInt;
		Str = Str.substr(tempFind + 1);
	}

	tempFind = Str.find_first_of('#');
	if (tempFind)
	{
		m_TileName = Str.substr(0, tempFind);
		Str = Str.substr(tempFind + 2);
	}

	// Set tile texture of the map
	SetTileTexture(m_TileName);

	// Create map with loaded data
	CreateLoadedMap(Str);
}

float DX9Map::GetMapTileBoundary(int MapID, Direction Dir) const
{
	float Result = 0.0f;

	D3DXVECTOR2 tMapXY = ConvertIDToXY(MapID, m_MapCols);

	float tX = m_Offset.x + tMapXY.x * m_TileSize;
	float tY = m_Offset.y + tMapXY.y * m_TileSize;

	switch (Dir)
	{
	case Direction::Up:
		Result = tY;
		break;
	case Direction::Down:
		Result = tY + m_TileSize;
		break;
	case Direction::Left:
		Result = tX;
		break;
	case Direction::Right:
		Result = tX + m_TileSize;
		break;
	default:
		break;
	}

	return Result;
}

bool DX9Map::IsMovableTile(int MapID, Direction Dir) const
{
	if ((MapID >= (m_MapCols * m_MapRows)) || (MapID < 0))
		return true;

	int tMoveID = m_MapData[MapID].MoveID;
	switch (Dir)
	{
	case Direction::Up:
		if ((tMoveID == 2) || (tMoveID == 7) || (tMoveID == 8) || (tMoveID == 9) ||
			(tMoveID == 12) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case Direction::Down:
		if ((tMoveID == 1) || (tMoveID == 5) || (tMoveID == 6) || (tMoveID == 9) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case Direction::Left:
		if ((tMoveID == 4) || (tMoveID == 5) || (tMoveID == 7) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 13) || (tMoveID == 15))
			return false;
		return true;
	case Direction::Right:
		if ((tMoveID == 3) || (tMoveID == 6) || (tMoveID == 8) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	default:
		return false;
	}
}

void DX9Map::SetMode(MapMode Mode)
{
	switch (Mode)
	{
	case MapMode::TileMode:
		m_CurrMapMode = Mode;
		return;
	case MapMode::MoveMode:
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
			tX = static_cast<float>(j * m_TileSize) + m_Offset.x;
			tY = static_cast<float>(i * m_TileSize) + m_Offset.y;
			m_Vertices[VertID0].x = tX;
			m_Vertices[VertID0].y = tY;
			m_Vertices[VertID0 + 1].x = tX + m_TileSize;
			m_Vertices[VertID0 + 1].y = tY;
			m_Vertices[VertID0 + 2].x = tX;
			m_Vertices[VertID0 + 2].y = tY + m_TileSize;
			m_Vertices[VertID0 + 3].x = tX + m_TileSize;
			m_Vertices[VertID0 + 3].y = tY + m_TileSize;

			if (m_bMoveTextureLoaded)
			{
				m_VertMove[VertID0].x = tX;
				m_VertMove[VertID0].y = tY;
				m_VertMove[VertID0 + 1].x = tX + m_TileSize;
				m_VertMove[VertID0 + 1].y = tY;
				m_VertMove[VertID0 + 2].x = tX;
				m_VertMove[VertID0 + 2].y = tY + m_TileSize;
				m_VertMove[VertID0 + 3].x = tX + m_TileSize;
				m_VertMove[VertID0 + 3].y = tY + m_TileSize;
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
	float MapH = static_cast<float>(m_MapRows * m_TileSize);
	float NewOffsetY = m_WindowData.WindowHeight - MapH + Offset.y;

	SetPosition(D3DXVECTOR2(Offset.x, NewOffsetY));
}

void DX9Map::SetMapFragmentTile(int TileID, int X, int Y)
{
	if ((X < m_MapCols) && (Y < m_MapRows))
	{
		int MapID = X + (Y * m_MapCols);
		int VertID0 = MapID * 4;

		FloatUV tUV = ConvertIDtoUV(TileID, m_TileSize, m_TileSheetWidth, m_TileSheetHeight);

		//@warning: FloatUV offset is done in order to make sure the image borders do not invade contiguous images
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

		m_Vertices[VertID0].u = tUV.u1;
		m_Vertices[VertID0].v = tUV.v1;
		m_Vertices[VertID0].color = tColor;
		m_Vertices[VertID0 + 1].u = tUV.u2;
		m_Vertices[VertID0 + 1].v = tUV.v1;
		m_Vertices[VertID0 + 1].color = tColor;
		m_Vertices[VertID0 + 2].u = tUV.u1;
		m_Vertices[VertID0 + 2].v = tUV.v2;
		m_Vertices[VertID0 + 2].color = tColor;
		m_Vertices[VertID0 + 3].u = tUV.u2;
		m_Vertices[VertID0 + 3].v = tUV.v2;
		m_Vertices[VertID0 + 3].color = tColor;

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

		FloatUV tUV = ConvertIDtoUV(MoveID, m_TileSize, m_MoveSheetWidth, m_MoveSheetHeight);

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

void DX9Map::Draw()
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

		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexImage));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, static_cast<int>(m_Vertices.size()), 0, static_cast<int>(m_Indices.size()));
	}

	if ((m_CurrMapMode == MapMode::MoveMode) && m_pTextureMove)
	{
		m_pDevice->SetTexture(0, m_pTextureMove);

		// Texture alpha * Diffuse color alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetStreamSource(0, m_pVBMove, 0, sizeof(VertexImage));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIndexBuffer);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, static_cast<int>(m_VertMove.size()), 0, static_cast<int>(m_Indices.size()));
	}
}

void DX9Map::GetMapData(WSTRING *pStr) const
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

void DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size) const
{
	WSTRING tempStr;
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
		//@warning: Invalid length
		return;
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
		//@warning: Invalid length
		return;
	}

	wcscpy_s(WC, size, tempStr.c_str());
}

bool DX9Map::IsMapCreated() const
{ 
	return m_bMapCreated;
};

int DX9Map::GetMapName(WSTRING *pStr) const
{
	*pStr = m_MapName;
	return 0;
}

int DX9Map::GetTileName(WSTRING *pStr) const
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
	return (m_MapCols * m_TileSize);
}

int DX9Map::GetHeight() const
{ 
	return (m_MapRows * m_TileSize);
}

D3DXVECTOR2	DX9Map::GetMapOffset() const 
{ 
	return m_Offset;
}

int DX9Map::GetMapOffsetZeroY() const
{ 
	return m_OffsetZeroY;
}

D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(BoundingBox BB, D3DXVECTOR2 Velocity) const
{
	D3DXVECTOR2 NewVelocity = Velocity;

	D3DXVECTOR2 tSprPosS;
	D3DXVECTOR2 tSprPosE;
	D3DXVECTOR2 tMapXYS;
	D3DXVECTOR2 tMapXYE;

	if (Velocity.x > 0)
	{
		// Go Right
		tSprPosS = BB.PositionOffset;
		tSprPosS.x += BB.Size.x; // ¦¤ (Right Up)
		tSprPosE = tSprPosS;
		tSprPosE.x += Velocity.x;
		tSprPosE.y += BB.Size.y;
	}
	else if (Velocity.x < 0)
	{
		// Go Left
		tSprPosS = BB.PositionOffset; // ¦£ (Left Up)
		tSprPosE = tSprPosS;
		tSprPosE.x += Velocity.x;
		tSprPosE.y += BB.Size.y;
	}
	else if (Velocity.y > 0)
	{
		// Go Down
		tSprPosS = BB.PositionOffset;
		tSprPosS.y += BB.Size.y; // ¦¦ (Left Down)
		tSprPosE = tSprPosS;
		tSprPosE.y += Velocity.y;
		tSprPosE.x += BB.Size.x;
	}
	else if (Velocity.y < 0)
	{
		// Go Up
		tSprPosS = BB.PositionOffset; // ¦£ (Left Up)
		tSprPosE = tSprPosS;
		tSprPosE.y += Velocity.y;
		tSprPosE.x += BB.Size.x;
	}

	tMapXYS = ConvertPositionToXY(tSprPosS, m_Offset, m_TileSize);
	tMapXYE = ConvertPositionToXY(tSprPosE, m_Offset, m_TileSize);

	int tXS = static_cast<int>(tMapXYS.x);
	int tYS = static_cast<int>(tMapXYS.y);
	int tXE = static_cast<int>(tMapXYE.x);
	int tYE = static_cast<int>(tMapXYE.y);

	float fWall = 0.0f;
	float fWallCmp = 0.0f;

	if (Velocity.x > 0)
	{
		// Go Right
		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2(static_cast<float>(i), static_cast<float>(j)), m_MapCols);
				if (IsMovableTile(tMapID, Direction::Right) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, Direction::Left);
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
		for (int i = tXS; i >= tXE; i--)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2(static_cast<float>(i), static_cast<float>(j)), m_MapCols);
				if (IsMovableTile(tMapID, Direction::Left) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, Direction::Right);
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
		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j <= tYE; j++)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2(static_cast<float>(i), static_cast<float>(j)), m_MapCols);
				if (IsMovableTile(tMapID, Direction::Down) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, Direction::Up);
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
		for (int i = tXS; i <= tXE; i++)
		{
			for (int j = tYS; j >= tYE; j--)
			{
				int tMapID = ConvertXYToID(D3DXVECTOR2(static_cast<float>(i), static_cast<float>(j)), m_MapCols);
				if (IsMovableTile(tMapID, Direction::Up) == false)
				{
					fWallCmp = GetMapTileBoundary(tMapID, Direction::Down);
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