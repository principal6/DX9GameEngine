#pragma once

#ifndef _DX9EFFECT_H_
#define _DX9EFFECT_H_

#include "DX9Anim.h"

class DX9Effect : public DX9Anim
{
private:
	DX9EFFECTTYPE m_Type;
	D3DXVECTOR2 m_BaseSpawnOffset;
	DX9EFFECTDATA* m_pFisrtInstance;
	DX9EFFECTDATA* m_pLastInstance;
	int m_InstanceCount;

public:
	DX9Effect();
	~DX9Effect() {};

	int DX9Effect::Destroy();
	int DX9Effect::SetBaseSpawnOffset(D3DXVECTOR2 Offset);
	int DX9Effect::Spawn(D3DXVECTOR2 Pos, DX9ANIMDIR Direction);
};

#endif