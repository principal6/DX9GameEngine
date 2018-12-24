#include "DX9Line.h"

int DX9Line::Create(LPDIRECT3DDEVICE9 pD3DDev) {
	// 멤버 변수 초기화
	m_pDevice = pD3DDev;

	m_pVB = NULL;
	m_pIB = NULL;

	m_Vert.clear();
	m_Ind.clear();

	return 0;
}

int DX9Line::Destroy() {
	m_pDevice = NULL; // DX9Base에서 생성했으므로 여기선 참조만 해제!

	m_Vert.clear();
	m_Ind.clear();

	if (m_pIB != NULL)
		m_pIB->Release();

	if (m_pVB != NULL)
		m_pVB->Release();

	return 0;
}

int DX9Line::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color) {
	// 정점 정보 대입, 버퍼 생성
	m_Vert.push_back(DX9VERTEX_LINE(StartPos.x, StartPos.y, 0.0f, 1.0f, Color));
	m_Vert.push_back(DX9VERTEX_LINE(StartPos.x + Length.x, StartPos.y + Length.y, 0.0f, 1.0f, Color));
	m_nVertCount = (int)m_Vert.size();

	// 색인 정보 대입, 버퍼 생성
	m_Ind.push_back(DX9INDEX2(m_nIndCount * 2, m_nIndCount * 2 + 1));
	m_nIndCount = (int)m_Ind.size();
	return 0;
}

int DX9Line::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color) {
	AddLine(StartPos, D3DXVECTOR2(Size.x, 0), Color);
	AddLine(StartPos, D3DXVECTOR2(0, Size.y), Color);
	AddLine(D3DXVECTOR2(StartPos.x + Size.x, StartPos.y), D3DXVECTOR2(0, Size.y), Color);
	AddLine(D3DXVECTOR2(StartPos.x, StartPos.y + Size.y), D3DXVECTOR2(Size.x, 0), Color);
	return 0;
}

int DX9Line::AddEnd() {
	CreateVB();
	CreateIB();
	UpdateVB();
	m_VertBU = m_Vert;

	return 0;
}

int DX9Line::SetPositionOffset(D3DXVECTOR2 Offset) {
	for (int i = 0; i < m_nVertCount; i++)
	{
		m_Vert[i].x = m_VertBU[i].x + Offset.x;
		m_Vert[i].y = m_VertBU[i].y + Offset.y;
	}
	UpdateVB();
	return 0;
}

int DX9Line::SetBoxPosition(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size) {
	m_Vert[0].x = StartPos.x;
	m_Vert[0].y = StartPos.y;
	m_Vert[1].x = StartPos.x + Size.x;
	m_Vert[1].y = StartPos.y;

	m_Vert[2].x = StartPos.x;
	m_Vert[2].y = StartPos.y;
	m_Vert[3].x = StartPos.x;
	m_Vert[3].y = StartPos.y + Size.y;

	m_Vert[4].x = StartPos.x + Size.x;
	m_Vert[4].y = StartPos.y;
	m_Vert[5].x = StartPos.x + Size.x;
	m_Vert[5].y = StartPos.y + Size.y;

	m_Vert[6].x = StartPos.x;
	m_Vert[6].y = StartPos.y + Size.y;
	m_Vert[7].x = StartPos.x + Size.x;
	m_Vert[7].y = StartPos.y + Size.y;
	
	UpdateVB();
	return 0;
}

int DX9Line::Draw() {
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pDevice->SetTexture(0, NULL);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DX9VERTEX_LINE));
	m_pDevice->SetFVF(D3DFVF_LINE);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_nVertCount, 0, m_nIndCount);

	return 0;
}

int DX9Line::CreateVB() {
	int rVertSize = sizeof(DX9VERTEX_LINE) * m_nVertCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0,
		D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		return -1;
	}

	return 0;
}

int DX9Line::CreateIB() {
	int rIndSize = sizeof(DX9INDEX2) * m_nIndCount;
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return -1;
	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
		return -1;
	memcpy(pIndices, &m_Ind[0], rIndSize);
	m_pIB->Unlock();

	return 0;
}

int DX9Line::UpdateVB() {
	int rVertSize = sizeof(DX9VERTEX_LINE) * m_nVertCount;
	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, rVertSize, (void**)&pVertices, 0)))
		return -1;
	memcpy(pVertices, &m_Vert[0], rVertSize);
	m_pVB->Unlock();

	return 0;
}