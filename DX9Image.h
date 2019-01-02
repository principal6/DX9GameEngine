#pragma once

#include "DX9Common.h"
#include "DX9Line.h"

class DX9Image
{
protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	DX9SHARE_DATA* m_pShareData;

	std::vector<DX9VERTEX_IMAGE> m_Vert;
	std::vector<DX9INDEX3> m_Ind;
	int m_VertCount;
	int m_IndCount;

	int m_Width;
	int m_Height;
	int m_ScaledW;
	int m_ScaledH;
	int m_VisibleW;
	int m_VisibleH;
	D3DXVECTOR2 m_Position;
	D3DXVECTOR2 m_Scale;
	DX9BOUNDINGBOX m_BB;
	DX9Line m_BBLine;

protected:
	void DX9Image::ClearVertexAndIndexData();
	virtual void DX9Image::CreateVertexBuffer();
	virtual void DX9Image::CreateIndexBuffer();
	void DX9Image::UpdateVertexBuffer();
	void DX9Image::UpdateIndexBuffer();
	void DX9Image::UpdateVertexData();
	void DX9Image::UpdateVertexData(float u1, float v1, float u2, float v2);

public:
	DX9Image();
	virtual ~DX9Image() {};

	virtual void DX9Image::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData);
	virtual void DX9Image::Destroy();

	virtual void DX9Image::Draw() const;
	virtual void DX9Image::DrawBoundingBox();
	void DX9Image::FlipHorizontal();
	void DX9Image::FlipVertical();

	void DX9Image::SetSize(int Width, int Height); //@warning: only used in Map Editor
	virtual void DX9Image::SetTexture(WSTRING FileName);
	virtual void DX9Image::SetPosition(D3DXVECTOR2 Position);
	virtual void DX9Image::SetPositionCentered(D3DXVECTOR2 Position);
	virtual void DX9Image::SetScale(D3DXVECTOR2 Scale);
	void DX9Image::SetVisibleRange(int Width, int Height);
	void DX9Image::SetUVRange(float u1, float u2, float v1, float v2);
	virtual void DX9Image::SetAlpha(int Alpha);
	virtual DX9Image* DX9Image::SetBoundingnBox(D3DXVECTOR2 Size);

	virtual int DX9Image::GetWidth() const;
	virtual int DX9Image::GetHeight() const;
	int DX9Image::GetScaledWidth() const;
	int DX9Image::GetScaledHeight() const;
	D3DXVECTOR2 DX9Image::GetPosition() const;
	virtual D3DXVECTOR2 DX9Image::GetCenterPosition() const;
	virtual DX9BOUNDINGBOX DX9Image::GetBoundingBox() const;
	bool DX9Image::IsTextureLoaded() const;
};