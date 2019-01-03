#pragma once

#include "DX9Life.h"

class DX9Sprite final : public DX9Life
{
public:
	DX9Sprite() {};
	~DX9Sprite() {};

	void DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	DX9Sprite* DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position) override;

	DX9Sprite* DX9Sprite::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f) override;
	DX9Sprite* DX9Sprite::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame) override;
	DX9Sprite* DX9Sprite::SetBoundingnBox(D3DXVECTOR2 Size) override;
};