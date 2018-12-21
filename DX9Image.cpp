#include "DX9Image.h"

int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev) {
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

	m_nWidth = 10;
	m_nHeight = 10;

	// 정점 정보 대입, 버퍼 생성
	m_nVertCount = 4;
	m_Vert.push_back(DX9VERTEX(m_fX,  m_fY, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f));
	m_Vert.push_back(DX9VERTEX(m_fX + m_nWidth, m_fY, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f));
	m_Vert.push_back(DX9VERTEX(m_fX, m_fY + m_nHeight, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f));
	m_Vert.push_back(DX9VERTEX(m_fX + m_nWidth, m_fY + m_nHeight, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f));
	CreateVB();

	// 색인 정보 대입, 버퍼 생성
	m_nIndCount = 2;
	m_Ind.push_back(DX9INDEX(0, 1, 3));
	m_Ind.push_back(DX9INDEX(0, 3, 2));
	CreateIB();

	return 0;
}

int DX9Image::Destroy() {
	m_pDevice = NULL; // DX9Base에서 생성했으므로 여기선 참조만 해제!
	
	m_Vert.clear();
	m_Ind.clear();

	if (m_pTexture != NULL)
		m_pTexture->Release();

	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pVB != NULL)
		m_pVB->Release();

	return 0;
}

int DX9Image::Draw() {
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	if (m_pTexture)
		m_pDevice->SetTexture(0, m_pTexture);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DX9VERTEX));
	m_pDevice->SetFVF(D3DFVF_TEXTURE);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertCount, 0, m_nIndCount);

	return 0;
}

int DX9Image::SetPosition(float X, float Y) {
	m_fX = X;
	m_fY = Y;
	UpdateVertData();
	return 0;
}

int DX9Image::SetSize(int Width, int Height) {
	m_nWidth = Width;
	m_nHeight = Height;
	UpdateVertData();
	return 0;
}

int DX9Image::SetScale(float ScaleX, float ScaleY) {
	m_fScaleX = ScaleX;
	m_fScaleY = ScaleY;
	UpdateVertData();
	return 0;
}

int DX9Image::FlipHorizontal() {
	float tempu1 = m_Vert[0].u;

	m_Vert[0].u = m_Vert[1].u;
	m_Vert[2].u = m_Vert[3].u;
	m_Vert[1].u = tempu1;
	m_Vert[3].u = tempu1;

	UpdateVB();
	return 0;
}

int DX9Image::FlipVertical() {
	float tempv1 = m_Vert[0].v;

	m_Vert[0].v = m_Vert[2].v;
	m_Vert[1].v = m_Vert[3].v;
	m_Vert[2].v = tempv1;
	m_Vert[3].v = tempv1;

	UpdateVB();
	return 0;
}

int DX9Image::SetTexture(std::wstring FileName) {
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
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
	if (D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		&tImgInfo, NULL, &m_pTexture))
		return -1;

	m_nWidth = tImgInfo.Width;
	m_nHeight = tImgInfo.Height;
	UpdateVertData();

	return 0;
}

int DX9Image::CreateVB() {
	int rVertSize = sizeof(DX9VERTEX) * m_nVertCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		return -1;
	}

	return 0;
}

int DX9Image::CreateIB() {
	int rIndSize = sizeof(DX9INDEX) * m_nIndCount;
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return -1;
	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
		return -1;
	memcpy(pIndices, &m_Ind[0], rIndSize);
	m_pIB->Unlock();

	return 0;
}

int DX9Image::UpdateVB() {
	int rVertSize = sizeof(DX9VERTEX) * m_nVertCount;
	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, &m_Vert[0], rVertSize);
	m_pVB->Unlock();

	return 0;
}

int DX9Image::UpdateVertData() {
	if (m_Vert.size() < 4)
		return -1;

	m_Vert[0].x = m_fX;
	m_Vert[0].y = m_fY;
	m_Vert[1].x = m_fX + m_nWidth * m_fScaleX;
	m_Vert[1].y = m_fY;
	m_Vert[2].x = m_fX;
	m_Vert[2].y = m_fY + m_nHeight * m_fScaleY;
	m_Vert[3].x = m_fX + m_nWidth * m_fScaleX;
	m_Vert[3].y = m_fY + m_nHeight * m_fScaleY;

	UpdateVB();
	return 0;
}

int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2) {
	if (m_Vert.size() < 4)
		return -1;

	m_Vert[0].u = u1;
	m_Vert[0].v = v1;
	m_Vert[1].u = u2;
	m_Vert[1].v = v1;
	m_Vert[2].u = u1;
	m_Vert[2].v = v2;
	m_Vert[3].u = u2;
	m_Vert[3].v = v2;
	UpdateVertData();
	return 0;
}

bool DX9Image::IsTextureLoaded() {
	if (m_pTexture)
		return true;
	return false;
}