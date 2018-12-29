#pragma once

#ifndef _DX9IMAGE_H_
#define _DX9IMAGE_H_

#include "DX9Common.h"
#include "DX9Line.h"
#include <vector>

class DX9Image
{
// 현재 클래스에서 참조만 한 변수들
protected:
	LPDIRECT3DDEVICE9 m_pDevice;

// 현재 클래스 내에서 선언한 변수들
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexture;

	std::vector<DX9VERTEX_IMAGE> m_Vert;
	int m_VertCount;

	std::vector<DX9INDEX3> m_Ind;
	int m_IndCount;

	std::wstring m_BaseDir;

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Scale;
	int m_nWidth;
	int m_nHeight;
	int m_nScaledW;
	int m_nScaledH;
	int m_VisibleW;
	int m_VisibleH;

	// Collision
	DX9BOUNDINGBOX m_BB;
	DX9Line m_BBLine;

// 상속 클래스에서 접근할 함수들
protected:
	int DX9Image::CreateVB();
	int DX9Image::CreateIB();
	int DX9Image::UpdateVB();
	int DX9Image::UpdateIB();
	int DX9Image::UpdateVertData();
	int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2);

public:
	DX9Image();
	~DX9Image() {};

	int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Image::Destroy();

	int DX9Image::Draw();
	int DX9Image::DrawBoundingBox();
	int DX9Image::FlipHorizontal();
	int DX9Image::FlipVertical();

	int DX9Image::SetPosition(D3DXVECTOR2 Pos);
	int DX9Image::SetPositionCentered(D3DXVECTOR2 Pos);
	int DX9Image::SetSize(int Width, int Height);
	int DX9Image::SetScale(D3DXVECTOR2 Scale);
	int DX9Image::SetVisibleRange(int Width, int Height);
	int DX9Image::SetUVRange(float u1, float u2, float v1, float v2);
	int DX9Image::SetAlpha(int Alpha);
	int DX9Image::SetTexture(std::wstring FileName);
	int DX9Image::SetBoundingnBox(D3DXVECTOR2 Size);

	D3DXVECTOR2 DX9Image::GetPosition() { return m_Pos; };
	D3DXVECTOR2 DX9Image::GetCenterPosition();
	int DX9Image::GetWidth() { return m_nWidth;	};
	int DX9Image::GetHeight() { return m_nHeight; };
	int DX9Image::GetScaledWidth() { return m_nScaledW; };
	int DX9Image::GetScaledHeight() { return m_nScaledH; };
	DX9BOUNDINGBOX DX9Image::GetBoundingBox();

	bool DX9Image::IsTextureLoaded();
};

#endif