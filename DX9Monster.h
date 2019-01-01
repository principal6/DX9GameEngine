#pragma once

#include "DX9Life.h"

const float OFFSET_Y_HPBAR = 16.0f;

class DX9Monster final : public DX9Life
{
private:
	int m_HPMax;
	int m_HPCurr;
	DX9Image *m_HPFrame;
	DX9Image *m_HPBar;

private:
	void DX9Monster::SetUIPosition(D3DXVECTOR2 Position);
	void DX9Monster::CalculateHP();

public:
	DX9Monster();
	~DX9Monster() {};

	void DX9Monster::Create(LPDIRECT3DDEVICE9 pDevice) override;
	void DX9Monster::Destroy() override;
	
	void DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position) override;
	void DX9Monster::UpdateGlobalPosition(D3DXVECTOR2 MapOffset, float MapOffsetZeroY);
	void DX9Monster::SetMaxHP(int HPMax);
	void DX9Monster::Damage(int Damage);

	void DX9Monster::Draw();
};