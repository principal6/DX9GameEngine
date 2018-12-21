#include "DX9Image.h"

int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev) {
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

	// 정점 정보 대입, 버퍼 생성
	mnVertCount = 4;
	mVert.push_back(DX9VERTEX(mX,  mY, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f));
	mVert.push_back(DX9VERTEX(mX + mWidth, mY, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f));
	mVert.push_back(DX9VERTEX(mX, mY + mHeight, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f));
	mVert.push_back(DX9VERTEX(mX + mWidth, mY + mHeight, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f));
	CreateVB();

	// 색인 정보 대입, 버퍼 생성
	mnIndCount = 2;
	mInd.push_back(DX9INDEX(0, 1, 3));
	mInd.push_back(DX9INDEX(0, 3, 2));
	CreateIB();

	return 0;
}

int DX9Image::Destroy() {
	mpDevice = NULL; // DX9Base에서 생성했으므로 여기선 참조만 해제!
	
	mVert.clear();
	mInd.clear();

	if (mpTexture != NULL)
		mpTexture->Release();

	if (mpIB != NULL)
		mpIB->Release();

	if (mpVB != NULL)
		mpVB->Release();

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

int DX9Image::SetScale(float ScaleX, float ScaleY) {
	mScaleX = ScaleX;
	mScaleY = ScaleY;
	UpdateVertData();
	return 0;
}

int DX9Image::FlipHorizontal() {
	float tempu1 = mVert[0].u;

	mVert[0].u = mVert[1].u;
	mVert[2].u = mVert[3].u;
	mVert[1].u = tempu1;
	mVert[3].u = tempu1;

	UpdateVB();
	return 0;
}

int DX9Image::FlipVertical() {
	float tempv1 = mVert[0].v;

	mVert[0].v = mVert[2].v;
	mVert[1].v = mVert[3].v;
	mVert[2].v = tempv1;
	mVert[3].v = tempv1;

	UpdateVB();
	return 0;
}

int DX9Image::SetTexture(std::wstring FileName) {
	if (mpTexture)
	{
		mpTexture->Release();
		mpTexture = NULL;
	}

	std::wstring NewFileName;
	NewFileName = FileName;
	if (FileName.find(L'\\') == -1)
	{
		NewFileName = L"Data\\";
		NewFileName += FileName;
	}

	// 텍스처 불러오기
	D3DXIMAGE_INFO tImgInfo;
	if (D3DXCreateTextureFromFileEx(mpDevice, NewFileName.c_str(), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		&tImgInfo, NULL, &mpTexture))
		return -1;

	mWidth = (float)tImgInfo.Width;
	mHeight = (float)tImgInfo.Height;
	UpdateVertData();

	return 0;
}

int DX9Image::CreateVB() {
	int rVertSize = sizeof(DX9VERTEX) * mnVertCount;
	if (FAILED(mpDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_DEFAULT, &mpVB, NULL)))
	{
		return -1;
	}

	return 0;
}

int DX9Image::CreateIB() {
	int rIndSize = sizeof(DX9INDEX) * mnIndCount;
	if (FAILED(mpDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mpIB, NULL)))
		return -1;
	VOID* pIndices;
	if (FAILED(mpIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
		return -1;
	memcpy(pIndices, &mInd[0], rIndSize);
	mpIB->Unlock();

	return 0;
}

int DX9Image::UpdateVB() {
	int rVertSize = sizeof(DX9VERTEX) * mnVertCount;
	VOID* pVertices;
	if (FAILED(mpVB->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, &mVert[0], rVertSize);
	mpVB->Unlock();

	return 0;
}

int DX9Image::UpdateVertData() {
	if (mVert.size() < 4)
		return -1;

	mVert[0].x = mX;
	mVert[0].y = mY;
	mVert[1].x = mX + mWidth * mScaleX;
	mVert[1].y = mY;
	mVert[2].x = mX;
	mVert[2].y = mY + mHeight * mScaleY;
	mVert[3].x = mX + mWidth * mScaleX;
	mVert[3].y = mY + mHeight * mScaleY;

	UpdateVB();
	return 0;
}

int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2) {
	if (mVert.size() < 4)
		return -1;

	mVert[0].u = u1;
	mVert[0].v = v1;
	mVert[1].u = u2;
	mVert[1].v = v1;
	mVert[2].u = u1;
	mVert[2].v = v2;
	mVert[3].u = u2;
	mVert[3].v = v2;
	UpdateVertData();
	return 0;
}