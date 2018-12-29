#pragma once

#ifndef _DX9EFFECT_H_
#define _DX9EFFECT_H_

#include "DX9Anim.h"
#include "DX9Monster.h"

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
	int DX9Effect::DeleteInstance(DX9EFF_INST_DATA* pInstance);

public:
	DX9Effect();
	~DX9Effect() {};

	int DX9Effect::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Effect::Destroy();

	int DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows);

	int DX9Effect::AddEffectType(DX9EFF_TYPE Type, int StartFrame, int EndFrame, D3DXVECTOR2 SpawnOffset,
		D3DXVECTOR2 BBSize, int RepeatCount = 1);
	int DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, DX9ANIMDIR Dir, int Damage);

	int DX9Effect::Update();
	int DX9Effect::Draw();
	int DX9Effect::DrawBoundingBox();

	void DX9Effect::CheckCollisionWithMonsters(DX9Monster* pMonsters);
};

#endif