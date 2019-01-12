#pragma once

#include "DX9Common.h"

namespace DX9ENGINE
{
	class DX9Line final : public DX9Common
	{
	private:
		struct VertexLine
		{
			FLOAT x, y, z, rhw;
			DWORD color;

			VertexLine() : x(0), y(0), z(0), rhw(1), color(0xffffffff) {};
			VertexLine(float _x, float _y, DWORD _color) : x(_x), y(_y), z(0), rhw(1), color(_color) {};
			VertexLine(float _x, float _y, float _z, float _rhw, DWORD _color) :
				x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {};
		};

		struct Index2
		{
			WORD _0, _1;

			Index2() : _0(0), _1(0) {};
			Index2(int ID0, int ID1) : _0(ID0), _1(ID1) {};
		};

	private:
		static LPDIRECT3DDEVICE9 m_pDevice;
		LPDIRECT3DVERTEXBUFFER9 m_pVB;
		LPDIRECT3DINDEXBUFFER9 m_pIB;

		std::vector<VertexLine> m_Vertices;
		std::vector<Index2> m_Indices;

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
};