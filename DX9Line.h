#pragma once

#include "DX9Common.h"

class DX9Line
{
private:
	static LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	std::vector<DX9VERTEX_LINE> m_Vert;
	int m_VertCount;

	std::vector<DX9INDEX2> m_Ind;
	int m_IndCount;

protected:
	void DX9Line::CreateVB();
	void DX9Line::CreateIB();
	void DX9Line::CreateVBMax();
	void DX9Line::CreateIBMax();

public:
	DX9Line() {};
	~DX9Line() {};

	void DX9Line::Create(LPDIRECT3DDEVICE9 pD3DDev);
	void DX9Line::CreateMax(LPDIRECT3DDEVICE9 pD3DDev);
	void DX9Line::Clear();
	void DX9Line::Destroy();

	void DX9Line::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color);
	void DX9Line::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color);
	void DX9Line::AddEnd();

	void DX9Line::SetBoxPosition(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size);

	void DX9Line::UpdateVB();
	void DX9Line::UpdateIB();
	void DX9Line::Draw() const;
};