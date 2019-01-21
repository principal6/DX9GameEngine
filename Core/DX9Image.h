#pragma once

#include "DX9Common.h"
#include "DX9Line.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
	// ***

	struct SVertexImage
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		FLOAT u, v;

		SVertexImage() : x(0), y(0), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
		SVertexImage(float _x, float _y, float _u, float _v) : x(_x), y(_y), z(0), rhw(1), color(0xFFFFFFFF), u(_u), v(_v) {};
		SVertexImage(float _x, float _y, float _z, float _rhw, DWORD _color, float _u, float _v) :
			x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {};
	};

	struct SIndex3
	{
		WORD _0, _1, _2;

		SIndex3() : _0(0), _1(0), _2(0) {};
		SIndex3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};
	};

	struct SBoundingBox
	{
		D3DXVECTOR2 PositionOffset;
		D3DXVECTOR2 Size;

		SBoundingBox() : PositionOffset(D3DXVECTOR2(0, 0)), Size(D3DXVECTOR2(0, 0)) {};
		SBoundingBox(D3DXVECTOR2 _POSOFFSET, D3DXVECTOR2 _SIZE) : PositionOffset(_POSOFFSET), Size(_SIZE) {};
	};

	class DX9Image
	{
	public:
		DX9Image();
		virtual ~DX9Image() {};

		virtual auto DX9Image::Create(DX9Window* pDX9Window, WSTRING BaseDir)->EError;
		virtual void DX9Image::Destroy();

		virtual void DX9Image::Draw();
		virtual void DX9Image::DrawBoundingBox();

		virtual void DX9Image::FlipHorizontal();
		virtual void DX9Image::FlipVertical();

#ifdef DX9MAPEDITOR
		// @warning: SetSize() is used publicly only in Map Editor
		virtual void DX9Image::SetSize(int Width, int Height);
#endif

		virtual void DX9Image::SetTexture(WSTRING FileName);
		virtual void DX9Image::SetPosition(D3DXVECTOR2 Position);
		virtual void DX9Image::SetPositionCentered(D3DXVECTOR2 Position);
		virtual auto DX9Image::SetScale(D3DXVECTOR2 Scale)->DX9Image*;
		virtual auto DX9Image::SetVisibleRange(D3DXVECTOR2 Range)->DX9Image*;
		virtual auto DX9Image::SetAtlasUV(D3DXVECTOR2 OffsetInAtlas, D3DXVECTOR2 Size,
			bool bSetSize = true)->DX9Image*;
		virtual auto DX9Image::SetUVRange(float u1, float u2, float v1, float v2)->DX9Image*;
		virtual auto DX9Image::SetAlpha(int Alpha)->DX9Image*;
		virtual auto DX9Image::SetBoundingBox(D3DXVECTOR2 ExtraSize)->DX9Image*;
		virtual auto DX9Image::SetBoundingBoxColor(DWORD Color)->DX9Image*;

		virtual auto DX9Image::GetSize() const->D3DXVECTOR2;
		virtual auto DX9Image::GetScaledSize() const->D3DXVECTOR2;;
		virtual auto DX9Image::GetPosition() const->D3DXVECTOR2;
		virtual auto DX9Image::GetCenterPosition() const->D3DXVECTOR2;
		virtual auto DX9Image::GetBoundingBox() const->SBoundingBox;

		virtual auto DX9Image::IsTextureLoaded() const->bool;

	protected:
		virtual void DX9Image::ClearVertexAndIndexData();
		virtual void DX9Image::CreateVertexBuffer();
		virtual void DX9Image::CreateIndexBuffer();
		virtual void DX9Image::UpdateVertexBuffer();
		virtual void DX9Image::UpdateIndexBuffer();
		virtual void DX9Image::UpdateVertexData();
		virtual void DX9Image::UpdateVertexData(float u1, float v1, float u2, float v2);

#ifndef DX9MAPEDITOR
		virtual void DX9Image::SetSize(D3DXVECTOR2 Size);
#endif

	protected:
		static const int VISIBLE_RANGE_NOT_SET = -1;
		static const DWORD DEF_BOUNDINGBOX_COLOR;

		DX9Window* m_pDX9Window;
		WSTRING m_BaseDir;

		LPDIRECT3DDEVICE9 m_pDevice;

		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
		LPDIRECT3DTEXTURE9 m_pTexture;

		VECTOR<SVertexImage> m_Vertices;
		VECTOR<SIndex3> m_Indices;

		D3DXVECTOR2 m_Size;
		D3DXVECTOR2 m_ScaledSize;
		D3DXVECTOR2 m_VisibleRange;
		D3DXVECTOR2 m_AtlasSize;
		D3DXVECTOR2 m_OffsetInAtlas;

		D3DXVECTOR2 m_Position;
		D3DXVECTOR2 m_Scale;

		SBoundingBox m_BoundingBox;
		D3DXVECTOR2 m_BoundingBoxExtraSize;
		DX9Line m_BoundingBoxLine;
		DWORD m_BoundingBoxColor;
	};
};