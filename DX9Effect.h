#pragma once

#ifndef _DX9EFFECT_H_
#define _DX9EFFECT_H_

#include "DX9Anim.h"

const int MAX_EFFECT_COUNT = 100;

class DX9Effect : protected DX9Image
{
private:
	std::wstring m_TextureAtlasFN;
	int m_TACols;
	int m_TARows;
	int m_UnitW;
	int m_UnitH;

	std::vector<DX9EFF_TYPE_DATA> m_TypeData;
	int m_TypeCount;

	DX9EFF_INST_DATA* m_pFisrtInstance;
	DX9EFF_INST_DATA* m_pLastInstance;
	int m_InstanceCount;

private:
	int DX9Effect::CreateVB();
	int DX9Effect::CreateIB();
	int DX9Effect::UpdateVB();
	int DX9Effect::UpdateIB();	

public:
	DX9Effect();
	~DX9Effect() {};

	int DX9Effect::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Effect::Destroy();

	int DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows);

	int DX9Effect::AddEffectType(DX9EFF_TYPE Type, int StartFrame, int EndFrame, D3DXVECTOR2 SpawnOffset, int RepeatCount = 1);
	int DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, DX9ANIMDIR Dir);

	int DX9Effect::Update();
	int DX9Effect::Draw();
};

#endif