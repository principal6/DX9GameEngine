#pragma once

#include "DX9Life.h"

class DX9Sprite final : public DX9Life
{
private:

public:
	DX9Sprite() {};
	~DX9Sprite() {};

	void DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice) override;
	void DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position) override;
};