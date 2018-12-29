#pragma once

#ifndef _DX9MONSTER_H_
#define _DX9MONSTER_H_

#include "DX9Anim.h"

const float OFFSET_Y_HPBAR = 16.0f;

class DX9Monster : public DX9Anim
{
private:
	int m_HPMax;
	int m_HPCurr;
	DX9Image *m_HPFrame;
	DX9Image *m_HPBar;

private:
	int DX9Monster::SetUIPosition(D3DXVECTOR2 Position);
	int DX9Monster::CalculateHP();

public:
	DX9Monster();
	~DX9Monster() {};

	int DX9Monster::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir, int WindowWidth, int WindowHeight);
	int DX9Monster::Destroy();
	
	int DX9Monster::SetMaxHP(int HPMax);
	int DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position);
	int DX9Monster::UpdateGlobalPosition(D3DXVECTOR2 MapOffset, float MapOffsetZeroY);
	D3DXVECTOR2 DX9Monster::GetGlobalPosition() { return m_GlobalPos; };

	int DX9Monster::Damage(int Damage);

	int DX9Monster::Draw();
};

#endif