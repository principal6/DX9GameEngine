#pragma once

#ifndef DX9IMAGE_H
#define DX9IMAGE_H

#include "DX9Common.h"
#include <vector>

#define D3DFVF_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct DX9VERTEX {
	DX9VERTEX() : x(0), y(0), z(0), rhw(1), color(0xffffffff), u(0), v(0) {};
	DX9VERTEX(float _x, float _y, float _z, float _rhw,
		DWORD _color, float _u, float _v) : x(_x), y(_y), z(_z), rhw(_rhw),
		color(_color), u(_u), v(_v) {};

	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT u, v;
};

struct DX9INDEX {
	DX9INDEX() : _0(0), _1(0), _2(0) {};
	DX9INDEX(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};

	WORD	_0, _1, _2;
};

class DX9Image {
// 현재 클래스에서 참조만 한 변수들
protected:
	LPDIRECT3DDEVICE9       mpDevice;

// 현재 클래스 내에서 선언한 변수들
protected:
	LPDIRECT3DVERTEXBUFFER9 mpVB;
	LPDIRECT3DINDEXBUFFER9	mpIB;
	LPDIRECT3DTEXTURE9		mpTexture;

	std::vector<DX9VERTEX>	mVert;
	int			mnVertCount;

	std::vector<DX9INDEX>	mInd;
	int			mnIndCount;

	float	mX;
	float	mY;
	float	mScaleX;
	float	mScaleY;
	float	mWidth;
	float	mHeight;

private:
	int DX9Image::CreateVB();
	int DX9Image::CreateIB();
	int DX9Image::UpdateVB();

// 상속 클래스에서 접근할 함수들
protected:
	int DX9Image::UpdateVertData();
	int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2);

public:
	DX9Image() {};
	~DX9Image() {};

	int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Image::Destroy();
	int DX9Image::Draw();

	int DX9Image::FlipHorizontal();
	int DX9Image::FlipVertical();

	// Setter
	int DX9Image::SetPosition(float X, float Y);
	int DX9Image::SetSize(float Width, float Height);
	int DX9Image::SetScale(float ScaleX, float ScaleY);
	int DX9Image::SetTexture(wchar_t* FileName);
};

#endif