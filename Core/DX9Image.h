#pragma once

#include "DX9Common.h"
#include "DX9Line.h"

class DX9Image : protected DX9Common
{
protected:
	struct VertexImage
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		FLOAT u, v;

		VertexImage() : x(0), y(0), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
		VertexImage(float _x, float _y, float _u, float _v) : x(_x), y(_y), z(0), rhw(1), color(0xFFFFFFFF), u(_u), v(_v) {};
		VertexImage(float _x, float _y, float _z, float _rhw, DWORD _color, float _u, float _v) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {};
	};

	struct Index3
	{
		WORD _0, _1, _2;

		Index3() : _0(0), _1(0), _2(0) {};
		Index3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};
	};

	struct BoundingBox
	{
		D3DXVECTOR2 PositionOffset;
		D3DXVECTOR2 Size;

		BoundingBox() : PositionOffset(D3DXVECTOR2(0, 0)), Size(D3DXVECTOR2(0, 0)) {};
		BoundingBox(D3DXVECTOR2 _POSOFFSET, D3DXVECTOR2 _SIZE) : PositionOffset(_POSOFFSET), Size(_SIZE) {};
	};

protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;

	std::vector<VertexImage> m_Vertices;
	std::vector<Index3> m_Indices;

	int m_Width;
	int m_Height;
	int m_ScaledWidth;
	int m_ScaledHeight;
	int m_VisibleWidth;
	int m_VisibleHeight;
	D3DXVECTOR2 m_Position;
	D3DXVECTOR2 m_Scale;
	BoundingBox m_BB;
	DX9Line m_BBLine;

protected:
	virtual void DX9Image::ClearVertexAndIndexData();
	virtual void DX9Image::CreateVertexBuffer();
	virtual void DX9Image::CreateIndexBuffer();
	virtual void DX9Image::UpdateVertexBuffer();
	virtual void DX9Image::UpdateIndexBuffer();
	virtual void DX9Image::UpdateVertexData();
	virtual void DX9Image::UpdateVertexData(float u1, float v1, float u2, float v2);

public:
	DX9Image();
	virtual ~DX9Image() {};

	virtual ReturnValue DX9Image::Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void DX9Image::Destroy() override;

	virtual void DX9Image::Draw();
	virtual void DX9Image::DrawBoundingBox();
	virtual void DX9Image::FlipHorizontal();
	virtual void DX9Image::FlipVertical();

	//@warning: SetSize() is only used in Map Editor
	virtual void DX9Image::SetSize(int Width, int Height);
	virtual void DX9Image::SetTexture(WSTRING FileName);
	virtual void DX9Image::SetPosition(D3DXVECTOR2 Position);
	virtual void DX9Image::SetPositionCentered(D3DXVECTOR2 Position);
	virtual void DX9Image::SetScale(D3DXVECTOR2 Scale);
	virtual void DX9Image::SetVisibleRange(int Width, int Height);
	virtual void DX9Image::SetUVRange(float u1, float u2, float v1, float v2);
	virtual void DX9Image::SetAlpha(int Alpha);
	virtual DX9Image* DX9Image::SetBoundingnBox(D3DXVECTOR2 Size);

	virtual int DX9Image::GetWidth() const;
	virtual int DX9Image::GetHeight() const;
	virtual int DX9Image::GetScaledWidth() const;
	virtual int DX9Image::GetScaledHeight() const;
	virtual D3DXVECTOR2 DX9Image::GetPosition() const;
	virtual D3DXVECTOR2 DX9Image::GetCenterPosition() const;
	virtual DX9Image::BoundingBox DX9Image::GetBoundingBox() const;
	virtual bool DX9Image::IsTextureLoaded() const;
};