#pragma once

#ifndef DX9LINE_H
#define DX9LINE_H

#include "DX9Common.h"
#include <vector>

class DX9Line {
private:
	LPDIRECT3DDEVICE9       m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	std::vector<DX9VERTEX_LINE>	m_Vert;
	int							m_nVertCount;
	std::vector<DX9VERTEX_LINE>	m_VertBU;

	std::vector<DX9INDEX2>	m_Ind;
	int						m_nIndCount;

	// 상속 클래스에서 접근할 함수들
protected:
	int DX9Line::CreateVB();
	int DX9Line::CreateIB();
	int DX9Line::UpdateVB();

public:
	DX9Line() {};
	~DX9Line() {};

	int DX9Line::Create(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Line::Destroy();

	int DX9Line::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color);
	int DX9Line::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color);
	int DX9Line::AddEnd();

	int DX9Line::SetPositionOffset(D3DXVECTOR2 Offset);
	int DX9Line::SetBoxPosition(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size);

	int DX9Line::Draw();
};

#endif