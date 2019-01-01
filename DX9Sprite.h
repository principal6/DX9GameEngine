#pragma once

#include "DX9Life.h"

class DX9Sprite final : public DX9Life
{
public:
	DX9Sprite() {};
	~DX9Sprite() {};

	void DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position) override;
};