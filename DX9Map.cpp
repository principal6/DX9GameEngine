#include "DX9Map.h"

DX9Map::DX9Map()
{
	m_bMapCreated = false;
	m_CurrMapMode = DX9MAPMODE::TileMode;

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
	m_nVertMoveCount = 0;

	m_Offset = D3DXVECTOR2(0.0f, 0.0f);
}

int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir, int WindowHeight)
{
	m_pDevice = pD3DDev;
	m_Vert.clear();
	m_Ind.clear();
	m_BaseDir = BaseDir;
	m_WindowH = WindowHeight;

	return 0;
}

int DX9Map::Destroy()
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

	return DX9Image::Destroy();
}

int DX9Map::SetTileTexture(std::wstring FileName)
{
	assert(DX9Image::SetTexture(FileName) == 0);

	if ((m_Width) && (m_Width))
	{
		m_TileName = FileName;

		m_TileSheetWidth = m_Width; // SetTexture()에서 Sheet정보가 됨!
		m_TileSheetHeight = m_Height;

		return 0;
	}
	else
	{
		assert(m_Width > 0);
		return -1;
	}
}

int DX9Map::SetMoveTexture(std::wstring FileName)
{
	if (m_pTextureMove)
	{
		m_pTextureMove->Release();
		m_pTextureMove = nullptr;
	}

	std::wstring NewFileName;
	NewFileName = m_BaseDir;
	NewFileName += L"\\Data\\";
	NewFileName += FileName;

	// 텍스처 불러오기
	D3DXIMAGE_INFO tImgInfo;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		&tImgInfo, nullptr, &m_pTextureMove)))
		return -1;

	m_MoveSheetWidth = tImgInfo.Width;
	m_MoveSheetHeight = tImgInfo.Height;
	m_bMoveTextureLoaded = true;

	return 0;
}

int DX9Map::CreateMap(std::wstring Name, int MapCols, int MapRows)
{
	m_Vert.clear();
	m_VertMove.clear();
	m_Ind.clear();
	m_MapData.clear();

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

	return 0;
}

int DX9Map::CreateMapWithData()
{
	m_Vert.clear();
	m_VertMove.clear();
	m_Ind.clear();
	m_MapData.clear();

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
		m_MapDataInString = m_MapDataInString.substr(1); // 개행문자 삭제★
	}

	m_MapDataInString.clear();

	AddEnd();

	return 0;
}

int DX9Map::SetGlobalPosition(D3DXVECTOR2 Offset)
{
	float MapH = (float)(m_MapRows * TILE_H);
	float NewOffsetY = m_WindowH - MapH + Offset.y;

	SetPosition(D3DXVECTOR2(Offset.x, NewOffsetY));
	return 0;
}

int DX9Map::SetPosition(D3DXVECTOR2 Offset)
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

	UpdateVB();
	if (m_bMoveTextureLoaded)
	{
		UpdateVBMove();
	}

	return 0;
}

DX9UV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH)
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

int DX9Map::AddMapFragmentTile(int TileID, int X, int Y)
{
	DX9UV tUV = ConvertIDtoUV(TileID, m_TileSheetWidth, m_TileSheetHeight);
	
	// 이걸 해야 주변 텍스처를 침범하지 않음!★
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

	return 0;
}

int DX9Map::AddMapFragmentMove(int MoveID, int X, int Y)
{
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
		m_nVertMoveCount = (int)m_VertMove.size();

		return 0;
	}
	
	return -1;
}

int DX9Map::CreateVBMove()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_nVertMoveCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVBMove, nullptr)))
	{
		return -1;
	}

	return 0;
}

int DX9Map::UpdateVBMove()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_nVertMoveCount;
	VOID* pVertices;
	if (FAILED(m_pVBMove->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, &m_VertMove[0], rVertSize);
	m_pVBMove->Unlock();

	return 0;
}

int DX9Map::AddEnd()
{
	DX9Image::CreateVB();
	DX9Image::CreateIB();
	DX9Image::UpdateVB();

	if (m_bMoveTextureLoaded)
	{
		CreateVBMove();
		UpdateVBMove();
	}
	m_bMapCreated = true;

	m_OffsetZeroY = m_WindowH - (m_MapRows * TILE_H);

	return 0;
}

int DX9Map::SetMode(DX9MAPMODE Mode)
{
	if (m_bMoveTextureLoaded)
	{
		m_CurrMapMode = Mode;
		return 0;
	}
	
	// SetMoveTexture()가 호출된 적이 없습니다.
	assert(0);
	return -1;
}

int DX9Map::SetMapFragmentTile(int TileID, int X, int Y)
{
	if ((X < m_MapCols) && (Y < m_MapRows))
	{
		int MapID = X + (Y * m_MapCols);
		int VertID0 = MapID * 4;

		DX9UV tUV = ConvertIDtoUV(TileID, m_TileSheetWidth, m_TileSheetHeight);
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

		UpdateVB();

		return 0;
	}

	return -1;
}

int DX9Map::SetMapFragmentMove(int MoveID, int X, int Y)
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

		UpdateVBMove();

		return 0;
	}

	return -1;
}

int DX9Map::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (m_pTexture)
	{
		m_pDevice->SetTexture(0, m_pTexture);
		// 텍스처 알파 * 디퓨즈 컬러 알파
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DX9VERTEX_IMAGE));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIB);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertCount, 0, m_IndCount);
	}

	if (m_CurrMapMode == DX9MAPMODE::MoveMode)
	{
		if (m_pTextureMove)
		{
			m_pDevice->SetTexture(0, m_pTextureMove);

			// 텍스처 알파 + 컬러 알파★
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			m_pDevice->SetStreamSource(0, m_pVBMove, 0, sizeof(DX9VERTEX_IMAGE));
			m_pDevice->SetFVF(D3DFVF_TEXTURE);
			m_pDevice->SetIndices(m_pIB);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertMoveCount, 0, m_IndCount);
		}
	}

	return 0;
}

int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size)
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

int DX9Map::GetMapData(std::wstring *pStr)
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
		// 마지막엔 \n 안 하기 위해!
		if (i < m_MapRows)
			*pStr += L'\n';
	}

	return 0;
}

int DX9Map::LoadMapFromFile(std::wstring FileName)
{
	std::wstring NewFileName;
	NewFileName = m_BaseDir;
	NewFileName += L"\\Data\\";
	NewFileName += FileName;

	std::wifstream filein;
	filein.open(NewFileName, std::wifstream::in);
	if (!filein.is_open()) return -1;

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
	CreateMapWithData();

	return 0;
}

int DX9Map::SetMapData(std::wstring Str)
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

	return 0;
}

int DX9Map::GetMapName(std::wstring *pStr)
{
	*pStr = m_MapName;
	return 0;
}

int DX9Map::GetTileName(std::wstring *pStr)
{
	*pStr = m_TileName;
	return 0;
}

D3DXVECTOR2 DX9Map::ConvertIDToXY(int MapID)
{
	D3DXVECTOR2 Result = D3DXVECTOR2(0, 0);
	
	Result.x = (FLOAT)(MapID % m_MapCols);
	Result.y = (FLOAT)(MapID / m_MapCols);
	
	return Result;
}

int DX9Map::ConvertXYToID(D3DXVECTOR2 MapXY)
{
	return (int)MapXY.x + ((int)MapXY.y * m_MapCols);
}

D3DXVECTOR2 DX9Map::ConvertPositionToXY(D3DXVECTOR2 Position, bool YRoundUp)
{
	D3DXVECTOR2 Result;

	float tX = -m_Offset.x + Position.x;
	float tY = -m_Offset.y + Position.y;

	int tYR = (int)tX % TILE_W;
	int tMapX = (int)(tX / TILE_W);
	int tMapY = (int)(tY / TILE_H);

	if (YRoundUp)
	{
		// Y값은 올림 계산!★ (조금이라도 밑으로 내려가면 다음 줄로 인식해야 충돌 편함..)
		if (tYR)
			tMapY++;
	}

	Result.x = (FLOAT)tMapX;
	Result.y = (FLOAT)tMapY;

	return Result;
}

float DX9Map::GetMapTileBoundary(int MapID, DX9MAPDIR Dir)
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

bool DX9Map::IsMovableTile(int MapID, DX9MAPDIR Dir)
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

D3DXVECTOR2 DX9Map::GetVelocityAfterCollision(DX9BOUNDINGBOX BB, D3DXVECTOR2 Velocity)
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
		tSprPosS.x += BB.Size.x; // ┐ (Right Up)
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
		tSprPosS = BB.PosOffset; // ┌ (Left Up)
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
		tSprPosS.y += BB.Size.y; // └ (Left Down)
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
		tSprPosS = BB.PosOffset; // ┌ (Left Up)
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