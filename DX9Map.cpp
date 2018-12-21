#include "DX9Map.h"

DX9Map::DX9Map() {
	mnRows = 0;
	mnCols = 0;
	mfSheetWidth = 0.0f;
	mfSheetHeight = 0.0f;
}

int DX9Map::Create(LPDIRECT3DDEVICE9 pD3DDev) {
	// 멤버 변수 초기화
	mpDevice = pD3DDev;

	mpVB = NULL;
	mpIB = NULL;
	mpTexture = NULL;

	mVert.clear();
	mInd.clear();

	mX = 0.0f;
	mY = 0.0f;
	mScaleX = 1.0f;
	mScaleY = 1.0f;

	mWidth = 10.0f;
	mHeight = 10.0f;

	return 0;
}


int DX9Map::Destroy() {
	DX9Image::Destroy();
	return 0;
}

int DX9Map::SetTexture(std::wstring FileName) {
	
	std::wstring NewFileName;
	NewFileName = FileName;
	if (FileName.find(L'\\') == -1)
	{
		NewFileName = L"Data\\";
		NewFileName += FileName;
	}

	if (D3DXCreateTextureFromFile(mpDevice, NewFileName.c_str(), &mpTexture))
		return -1;
	
	return 0;
}

int DX9Map::SetTileInfo(float TileW, float TileH, int numRows, int numCols) {
	mWidth = TileW;
	mHeight = TileH;

	mnRows = numRows;
	mnCols = numCols;

	mfSheetWidth = mWidth * mnCols;
	mfSheetHeight = mHeight * mnRows;

	return 0;
}

int DX9Map::AddMapFragment(int TileID, float X, float Y) {
	int TileX = (TileID % mnCols);
	int TileY = (TileID / mnCols);

	float u1 = (float)(TileX * mWidth) / mfSheetWidth;
	float u2 = u1 + (float)mWidth / mfSheetWidth;
	float v1 = (float)(TileY * mHeight) / mfSheetHeight;
	float v2 = v1 + (float)mHeight / mfSheetHeight;

	mVert.push_back(DX9VERTEX(X, Y, 0, 1, 0xffffffff, u1, v1));
	mVert.push_back(DX9VERTEX(X + mWidth, Y, 0, 1, 0xffffffff, u2, v1));
	mVert.push_back(DX9VERTEX(X, Y + mHeight, 0, 1, 0xffffffff, u1, v2));
	mVert.push_back(DX9VERTEX(X + mWidth, Y + mHeight, 0, 1, 0xffffffff, u2, v2));
	mnVertCount = (int)mVert.size();

	mInd.push_back(DX9INDEX(mnVertCount - 4, mnVertCount - 3, mnVertCount - 1));
	mInd.push_back(DX9INDEX(mnVertCount - 4, mnVertCount - 1, mnVertCount - 2));
	mnIndCount = (int)mInd.size();

	return 0;
}

int DX9Map::AddEnd() {
	DX9Image::CreateVB();
	DX9Image::CreateIB();
	DX9Image::UpdateVB();

	return 0;
}

int DX9Map::Draw() {
	DX9Image::Draw();
	return 0;
}