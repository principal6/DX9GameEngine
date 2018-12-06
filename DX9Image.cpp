#include "DX9Image.h"

int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev) {
	// 멤버 변수 초기화
	mpDevice = pD3DDev;

	mpVB = NULL;
	mpIB = NULL;
	mpTexture = NULL;
	
	mVert = NULL;
	mInd = NULL;

	mX = 0.0f;
	mY = 0.0f;
	mWidth = 10.0f;
	mHeight = 10.0f;

	mnVertCount = 4;
	mVert = new DX9VERTEX[mnVertCount];
	mVert[0] = { mX,  mY, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f };
	mVert[1] = { mX + mWidth, mY, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f };
	mVert[2] = { mX, mY + mHeight, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f };
	mVert[3] = { mX + mWidth, mY + mHeight, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f };

	CreateVB();
	CreateIB();

	return 0;
}

int DX9Image::Destroy() {
	mpDevice = NULL; // DX9Base에서 생성했으므로 여기선 참조만 해제!
	
	if (mVert != NULL)
		delete[] mVert;

	if (mInd != NULL)
		delete[] mInd;

	if (mpTexture != NULL)
		mpTexture->Release();

	if (mpIB != NULL)
		mpIB->Release();

	if (mpVB != NULL)
		mpVB->Release();

	return 0;
}

int DX9Image::SetPosition(float X, float Y) {
	mX = X;
	mY = Y;
	UpdateVertData();
	return 0;
}

int DX9Image::SetSize(float Width, float Height) {
	mWidth = Width;
	mHeight = Height;
	UpdateVertData();
	return 0;
}

int DX9Image::UpdateVertData() {
	mVert[0].x = mX;
	mVert[0].y = mY;
	mVert[1].x = mX + mWidth;
	mVert[1].y = mY;
	mVert[2].x = mX;
	mVert[2].y = mY + mHeight;
	mVert[3].x = mX + mWidth;
	mVert[3].y = mY + mHeight;
	UpdateVB();
	return 0;
}

int DX9Image::CreateVB()
{
	int rVertSize = sizeof(DX9VERTEX) * mnVertCount;
	if (FAILED(mpDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_DEFAULT, &mpVB, NULL)))
	{
		return -1;
	}

	return 0;
}

int DX9Image::CreateIB()
{
	mnIndCount = 2;
	mInd = new DX9INDEX[mnIndCount];
	mInd[0] = DX9INDEX(0, 1, 3);
	mInd[1] = DX9INDEX(0, 3, 2);

	int rIndSize = sizeof(DX9INDEX) * mnIndCount;
	if (FAILED(mpDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mpIB, NULL)))
		return -1;
	VOID* pIndices;
	if (FAILED(mpIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
		return -1;
	memcpy(pIndices, mInd, rIndSize);
	mpIB->Unlock();

	return 0;
}

int DX9Image::UpdateVB() {
	int rVertSize = sizeof(DX9VERTEX) * mnVertCount;
	VOID* pVertices;
	if (FAILED(mpVB->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, mVert, rVertSize);
	mpVB->Unlock();

	return 0;
}

int DX9Image::SetTexture(wchar_t* FileName) {
	if (mpTexture)
		return -1;

	// 텍스처 불러오기
	wchar_t	NewFileName[255] = { 0 };
	wcscpy_s(NewFileName, L"Data\\");
	wcscat_s(NewFileName, FileName);

	if (FAILED(D3DXCreateTextureFromFile(mpDevice, NewFileName, &mpTexture)))
		return -1;

	return 0;
}

int DX9Image::Draw() {
	mpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	mpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	mpDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (mpTexture)
		mpDevice->SetTexture(0, mpTexture);

	mpDevice->SetStreamSource(0, mpVB, 0, sizeof(DX9VERTEX));
	mpDevice->SetFVF(D3DFVF_TEXTURE);
	mpDevice->SetIndices(mpIB);
	mpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mnVertCount, 0, mnIndCount);

	return 0;
}