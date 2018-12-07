#pragma once

#ifndef DX9IMAGE_H
#define DX9IMAGE_H

#include "DX9Common.h"

#define D3DFVF_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct DX9VERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT u, v;
};

struct DX9INDEX
{
	DX9INDEX() : _0(0), _1(0), _2(0) {};
	DX9INDEX(int A, int B, int C) : _0(A), _1(B), _2(C) {};

	WORD	_0, _1, _2;
};

class DX9Image
{
private:
	LPDIRECT3DDEVICE9       mpDevice; // ÂüÁ¶¿ë¡Ú

private:
	LPDIRECT3DVERTEXBUFFER9 mpVB;
	LPDIRECT3DINDEXBUFFER9	mpIB;
	LPDIRECT3DTEXTURE9		mpTexture;

	DX9VERTEX	*mVert;
	int			mnVertCount;

	DX9INDEX	*mInd;
	int			mnIndCount;

	float	mX;
	float	mY;
	float	mScaleX;
	float	mScaleY;
	bool	mHFlip; // Horizontal Flip
	bool	mVFlip; // Vertical Flip

protected:
	float	mWidth;
	float	mHeight;

private:
	int DX9Image::CreateVB();
	int DX9Image::CreateIB();
	int DX9Image::UpdateVB();

protected:
	int DX9Image::UpdateVertData();
	int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2);

public:
	DX9Image() {};
	~DX9Image() {};

	int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev);
	int DX9Image::Draw();
	int DX9Image::Destroy();

	// Setter
	int DX9Image::SetPosition(float X, float Y);
	int DX9Image::SetSize(float Width, float Height);
	int DX9Image::SetScale(float ScaleX, float ScaleY);
	int DX9Image::SetTexture(wchar_t* FileName);
	int DX9Image::SetHFlip(bool Value);
	int DX9Image::SetVFlip(bool Value);
};

#endif