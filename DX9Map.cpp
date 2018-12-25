#include "DX9Map.h"

DX9Map::DX9Map() {
	m_nMapCols = 0;
	m_nMapRows = 0;
	m_nTileSheetWidth = 0;
	m_nTileSheetHeight = 0;
}

int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir) {
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

	m_nWidth = 0;
	m_nHeight = 0;

	m_bMapCreated = false;
	m_bMoveTextureLoaded = false;
	m_strBaseDir = BaseDir;

	return 0;
}

int DX9Map::Destroy() {
	return DX9Image::Destroy();
}

int DX9Map::SetTileTexture(std::wstring FileName) {
	assert(DX9Image::SetTexture(FileName) == 0);

	if ((m_nWidth) && (m_nWidth))
	{
		m_strTileName = FileName;

		m_nTileSheetWidth = m_nWidth; // SetTexture()에서 Sheet정보가 됨!
		m_nTileSheetHeight = m_nHeight;

		return 0;
	}
	else
	{
		assert(m_nWidth > 0);
		return -1;
	}
}

int DX9Map::SetMoveTexture(std::wstring FileName) {
	if (m_pTextureMove)
	{
		m_pTextureMove->Release();
		m_pTextureMove = NULL;
	}

	std::wstring NewFileName;
	NewFileName = m_strBaseDir;
	NewFileName += L"\\Data\\";
	NewFileName += FileName;

	// 텍스처 불러오기
	D3DXIMAGE_INFO tImgInfo;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		&tImgInfo, NULL, &m_pTextureMove)))
		return -1;

	m_nMoveSheetWidth = tImgInfo.Width;
	m_nMoveSheetHeight = tImgInfo.Height;

	m_bMoveTextureLoaded = true;

	return 0;
}

int DX9Map::CreateMap(std::wstring Name, int MapCols, int MapRows) {
	m_Vert.clear();
	m_VertMove.clear();
	m_Ind.clear();
	m_MapData.clear();

	m_strMapName = Name;
	m_nMapCols = MapCols;
	m_nMapRows = MapRows;

	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapCols; j++)
		{
			AddMapFragmentTile(-1, j, i);
			AddMapFragmentMove(0, j, i);
			m_MapData.push_back(DXMAPDATA(-1, 0));
		}
	}
	AddEnd();

	return 0;
}

int DX9Map::CreateMapWithData() {
	m_Vert.clear();
	m_VertMove.clear();
	m_Ind.clear();
	m_MapData.clear();

	int tTileID = 0;
	int tMoveID = 0;

	for (int i = 0; i < m_nMapRows; i++)
	{
		for (int j = 0; j < m_nMapCols; j++)
		{
			tTileID = _wtoi(m_strLoadedMapTiles.substr(0, MAX_TILEID_LEN).c_str());
			if (tTileID == 999)
				tTileID = -1;
			
			// 수정 필!★
			tMoveID = _wtoi(m_strLoadedMapTiles.substr(MAX_TILEID_LEN, MAX_MOVEID_LEN).c_str());

			AddMapFragmentTile(tTileID, j, i);
			AddMapFragmentMove(tMoveID, j, i);
			m_MapData.push_back(DXMAPDATA(tTileID, tMoveID));

			m_strLoadedMapTiles = m_strLoadedMapTiles.substr(MAX_TILEID_LEN);
			m_strLoadedMapTiles = m_strLoadedMapTiles.substr(MAX_MOVEID_LEN);
		}
		m_strLoadedMapTiles = m_strLoadedMapTiles.substr(1); // 개행문자 삭제★
	}

	m_strLoadedMapTiles.clear();

	AddEnd();

	return 0;
}

int DX9Map::SetPosition(float OffsetX, float OffsetY) {
	int VertID0 = 0;
	float tX = 0.0f;
	float tY = 0.0f;

	m_fOffsetX = OffsetX;
	m_fOffsetY = OffsetY;

	for (int i = 0; i < m_nMapRows; i++)
	{
		for (int j = 0; j < m_nMapCols; j++)
		{
			VertID0 = (j + (i * m_nMapCols)) * 4;
			tX = (float)(j * TILE_W) + m_fOffsetX;
			tY = (float)(i * TILE_H) + m_fOffsetY;
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

DXUV DX9Map::ConvertIDtoUV(int ID, int SheetW, int SheetH) {
	DXUV Result;
	int tX = 0;
	int tY = 0;
	int tTileCols = (int)(SheetW / TILE_W);

	if (ID == -1)
	{
		Result = DXUV(0, 0, 0, 0);
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

int DX9Map::AddMapFragmentTile(int TileID, int X, int Y) {
	DXUV tUV = ConvertIDtoUV(TileID, m_nTileSheetWidth, m_nTileSheetHeight);
	
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
	m_nVertCount = (int)m_Vert.size();

	m_Ind.push_back(DX9INDEX3(m_nVertCount - 4, m_nVertCount - 3, m_nVertCount - 1));
	m_Ind.push_back(DX9INDEX3(m_nVertCount - 4, m_nVertCount - 1, m_nVertCount - 2));
	m_nIndCount = (int)m_Ind.size();

	return 0;
}

int DX9Map::AddMapFragmentMove(int MoveID, int X, int Y) {
	if (m_nMoveSheetWidth && m_nMoveSheetHeight)
	{
		DXUV tUV = ConvertIDtoUV(MoveID, m_nMoveSheetWidth, m_nMoveSheetHeight);

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

int DX9Map::CreateVBMove() {
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_nVertMoveCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVBMove, NULL)))
	{
		return -1;
	}

	return 0;
}

int DX9Map::UpdateVBMove() {
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_nVertMoveCount;
	VOID* pVertices;
	if (FAILED(m_pVBMove->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, &m_VertMove[0], rVertSize);
	m_pVBMove->Unlock();

	return 0;
}

int DX9Map::AddEnd() {
	DX9Image::CreateVB();
	DX9Image::CreateIB();
	DX9Image::UpdateVB();

	if (m_bMoveTextureLoaded)
	{
		CreateVBMove();
		UpdateVBMove();
	}
	m_bMapCreated = true;

	return 0;
}

int DX9Map::SetMode(DX9MAPMODE Mode) {
	if (m_bMoveTextureLoaded)
	{
		m_CurrMapMode = Mode;
		return 0;
	}
	
	// SetMoveTexture()가 호출된 적이 없습니다.
	assert(0);
	return -1;
}

int DX9Map::SetMapFragmentTile(int TileID, int X, int Y) {
	if ((X < m_nMapCols) && (Y < m_nMapRows))
	{
		int MapID = X + (Y * m_nMapCols);
		int VertID0 = MapID * 4;

		DXUV tUV = ConvertIDtoUV(TileID, m_nTileSheetWidth, m_nTileSheetHeight);
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

int DX9Map::SetMapFragmentMove(int MoveID, int X, int Y) {
	if ((X < m_nMapCols) && (Y < m_nMapRows))
	{
		int MapID = X + (Y * m_nMapCols);
		int VertID0 = MapID * 4;

		DXUV tUV = ConvertIDtoUV(MoveID, m_nMoveSheetWidth, m_nMoveSheetHeight);

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

int DX9Map::Draw() {
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (m_pTexture)
	{
		m_pDevice->SetTexture(0, m_pTexture);

		// 텍스처 알파 + 컬러 알파★
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DX9VERTEX_IMAGE));
		m_pDevice->SetFVF(D3DFVF_TEXTURE);
		m_pDevice->SetIndices(m_pIB);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertCount, 0, m_nIndCount);
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
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertMoveCount, 0, m_nIndCount);
		}
	}

	return 0;
}

int DX9Map::GetMapDataPart(int DataID, wchar_t *WC, int size) {
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

int DX9Map::GetMapData(std::wstring *pStr) {
	wchar_t tempWC[255] = { 0 };
	*pStr = m_strMapName;
	*pStr += L'#';
	_itow_s(m_nMapCols, tempWC, 10);
	*pStr += tempWC;
	*pStr += L'#';
	_itow_s(m_nMapRows, tempWC, 10);
	*pStr += tempWC;
	*pStr += L'#';
	*pStr += m_strTileName;
	*pStr += L'#';
	*pStr += L'\n';

	int tDataID = 0;
	for (int i = 0; i < m_nMapRows; i++)
	{
		for (int j = 0; j < m_nMapCols; j++)
		{
			tDataID = j + (i * m_nMapCols);
			GetMapDataPart(tDataID, tempWC, 255);
			*pStr += tempWC;
		}
		// 마지막엔 \n 안 하기 위해!
		if (i < m_nMapRows)
			*pStr += L'\n';
	}

	return 0;
}

int DX9Map::LoadMapFromFile(std::wstring FileName) {
	std::wstring NewFileName;
	NewFileName = m_strBaseDir;
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
	SetTileTexture(m_strTileName);
	CreateMapWithData();

	return 0;
}

int DX9Map::SetMapData(std::wstring Str) {

	size_t tFind = -1;
	int tInt = 0;

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		m_strMapName = Str.substr(0, tFind);
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		tInt = _wtoi(Str.substr(0, tFind).c_str());
		m_nMapCols = tInt;
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		tInt = _wtoi(Str.substr(0, tFind).c_str());
		m_nMapRows = tInt;
		Str = Str.substr(tFind + 1);
	}

	tFind = Str.find_first_of('#');
	if (tFind)
	{
		m_strTileName = Str.substr(0, tFind);
		Str = Str.substr(tFind + 2);
	}

	m_strLoadedMapTiles = Str;

	return 0;
}

int DX9Map::GetMapName(std::wstring *pStr) {
	*pStr = m_strMapName;
	return 0;
}

int DX9Map::GetTileName(std::wstring *pStr) {
	*pStr = m_strTileName;
	return 0;
}

D3DXVECTOR2 DX9Map::ConvertIDtoXY(int MapID) {
	D3DXVECTOR2 Result = D3DXVECTOR2(0, 0);
	
	Result.x = (FLOAT)(MapID % m_nMapCols);
	Result.y = (FLOAT)(MapID / m_nMapCols);
	
	return Result;
}

int DX9Map::ConvertXYtoID(D3DXVECTOR2 MapXY) {
	return (int)MapXY.x + ((int)MapXY.y * m_nMapCols);
}

D3DXVECTOR2 DX9Map::ConvertScrPostoXY(D3DXVECTOR2 ScreenPos) {
	D3DXVECTOR2 Result;

	float tX = -m_fOffsetX + ScreenPos.x;
	float tY = -m_fOffsetY + ScreenPos.y;

	int tMapX = (int)(tX / TILE_W);
	int tMapY = (int)(tY / TILE_H);

	Result.x = (FLOAT)tMapX;
	Result.y = (FLOAT)tMapY;

	return Result;
}

float DX9Map::GetMapTileBoundary(int MapID, DXMAPDIR Dir) {
	float Result = 0.0f;

	D3DXVECTOR2 tMapXY = ConvertIDtoXY(MapID);

	float tX = m_fOffsetX + tMapXY.x * TILE_W;
	float tY = m_fOffsetY + tMapXY.y * TILE_H;

	switch (Dir)
	{
	case DXMAPDIR::Up:
		Result = tY;
		break;
	case DXMAPDIR::Down:
		Result = tY + TILE_H;
		break;
	case DXMAPDIR::Left:
		Result = tX;
		break;
	case DXMAPDIR::Right:
		Result = tX + TILE_W;
		break;
	default:
		break;
	}

	return Result;
}

bool DX9Map::IsMovableTile(int MapID, DXMAPDIR Dir) {
	if ((MapID > (m_nMapCols * m_nMapRows)) || (MapID < 0))
		return true;

	int tMoveID = m_MapData[MapID].MoveID;
	switch (Dir)
	{
	case DXMAPDIR::Up:
		if ((tMoveID == 2) || (tMoveID == 7) || (tMoveID == 8) || (tMoveID == 9) ||
			(tMoveID == 12) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case DXMAPDIR::Down:
		if ((tMoveID == 1) || (tMoveID == 5) || (tMoveID == 6) || (tMoveID == 9) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	case DXMAPDIR::Left:
		if ((tMoveID == 4) || (tMoveID == 5) || (tMoveID == 7) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 12) || (tMoveID == 13) || (tMoveID == 15))
			return false;
		return true;
	case DXMAPDIR::Right:
		if ((tMoveID == 3) || (tMoveID == 6) || (tMoveID == 8) || (tMoveID == 10) ||
			(tMoveID == 11) || (tMoveID == 13) || (tMoveID == 14) || (tMoveID == 15))
			return false;
		return true;
	default:
		return false;
	}
}

D3DXVECTOR2 DX9Map::CheckSprCollision(D3DXVECTOR2 SprPos, D3DXVECTOR2 Velocity) {
	D3DXVECTOR2 NewVelocity = Velocity;

	D3DXVECTOR2 tMapXYSrc = ConvertScrPostoXY(SprPos);

	D3DXVECTOR2 SprPosTrg = SprPos;
	D3DXVECTOR2 tMapXYTrg = D3DXVECTOR2(0, 0);
	
	if (Velocity.x != 0)
	{
		// Left & Right
		SprPosTrg.x += Velocity.x;
		tMapXYTrg = ConvertScrPostoXY(SprPosTrg);

		if (tMapXYSrc != tMapXYTrg)
		{
			int StartX = (int)tMapXYSrc.x;
			int EndX = (int)tMapXYTrg.x;
			int Y = (int)tMapXYSrc.y;

			float fWall = 0.0f;
			if (Velocity.x > 0)
			{
				// Right
				for (int i = StartX; i <= EndX; i++)
				{
					int tMapID = ConvertXYtoID(D3DXVECTOR2((FLOAT)i, (FLOAT)Y));
					if (IsMovableTile(tMapID, DXMAPDIR::Right) == false)
						fWall = GetMapTileBoundary(tMapID, DXMAPDIR::Left);
				}

				if (fWall)
				{
					float fCurr = SprPos.x + Velocity.x;
					float fDist = fWall - SprPos.x - 0.1f;
					NewVelocity.x = fDist;
				}

			}
			if (Velocity.x < 0)
			{
				// Left
				for (int i = StartX; i >= EndX; i--)
				{
					int tMapID = ConvertXYtoID(D3DXVECTOR2((FLOAT)i, (FLOAT)Y));
					if (IsMovableTile(tMapID, DXMAPDIR::Left) == false)
						fWall = GetMapTileBoundary(tMapID, DXMAPDIR::Right);
				}

				if (fWall)
				{
					float fCurr = SprPos.x + Velocity.x;
					float fDist = fWall - SprPos.x;
					NewVelocity.x = fDist;
				}
			}
		}
	}

	if (Velocity.y != 0)
	{
		// Up & Down
		SprPosTrg.y += Velocity.y;
		tMapXYTrg = ConvertScrPostoXY(SprPosTrg);

		if (tMapXYSrc != tMapXYTrg)
		{
			int StartY = (int)tMapXYSrc.y;
			int EndY = (int)tMapXYTrg.y;
			int X = (int)tMapXYSrc.x;

			float fWall = 0.0f;
			if (Velocity.y > 0)
			{
				// Down
				for (int i = StartY; i <= EndY; i++)
				{
					int tMapID = ConvertXYtoID(D3DXVECTOR2((FLOAT)X, (FLOAT)i));
					if (IsMovableTile(tMapID, DXMAPDIR::Down) == false)
						fWall = GetMapTileBoundary(tMapID, DXMAPDIR::Up);
				}

				if (fWall)
				{
					float fCurr = SprPos.y + Velocity.y;
					float fDist = fWall - SprPos.y - 0.1f;
					NewVelocity.y = fDist;
				}
			}
			if (Velocity.y < 0)
			{
				// Up
				for (int i = StartY; i >= EndY; i--)
				{
					int tMapID = ConvertXYtoID(D3DXVECTOR2((FLOAT)X, (FLOAT)i));
					if (IsMovableTile(tMapID, DXMAPDIR::Up) == false)
						fWall = GetMapTileBoundary(tMapID, DXMAPDIR::Down);
				}

				if (fWall)
				{
					float fCurr = SprPos.y + Velocity.y;
					float fDist = fWall - SprPos.y;
					NewVelocity.y = fDist;
				}
			}
		}
	}

	return NewVelocity;
}