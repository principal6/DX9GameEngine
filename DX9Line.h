#pragma once

#ifndef _DX9LINE_H_
#define _DX9LINE_H_

#include "DX9Common.h"
#include <vector>

class DX9Line
{
// 현재 클래스에서 참조만 한 변수들
private:
	LPDIRECT3DDEVICE9 m_pDevice;

// 현재 클래스 내에서 선언한 변수들
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	std::vector<DX9VERTEX_LINE> m_Vert;
	int m_VertCount;
	std::vector<DX9VERTEX_LINE> m_VertBackup;

	std::vector<DX9INDEX2> m_Ind;
	int m_IndCount;

protected:
	int DX9Line::CreateVB();
	int DX9Line::CreateIB();
	int DX9Line::CreateVBMax();
	int DX9Line::CreateIBMax();

public:
	DX9Line() {};
	~DX9Line() {};

	int DX9Line::Create(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Line::CreateMax(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Line::Destroy();

	int DX9Line::Clear();
	int DX9Line::AddLine(D3DXVECTOR2 StartPos, D3DXVECTOR2 Length, DWORD Color);
	int DX9Line::AddBox(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size, DWORD Color);
	int DX9Line::AddEnd();

	int DX9Line::SetPositionOffset(D3DXVECTOR2 Offset);
	int DX9Line::SetBoxPosition(D3DXVECTOR2 StartPos, D3DXVECTOR2 Size);

	int DX9Line::UpdateVB();
	int DX9Line::UpdateIB();
	int DX9Line::Draw();
};

#endif