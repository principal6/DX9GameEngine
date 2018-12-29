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
	int DX9Monster::CalculateHP();

public:
	DX9Monster();
	~DX9Monster() {};

	int DX9Monster::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Monster::Destroy();
	
	int DX9Monster::SetMaxHP(int HPMax);
	int DX9Monster::SetPosition(D3DXVECTOR2 Pos);

	int DX9Monster::Draw();
};

#endif