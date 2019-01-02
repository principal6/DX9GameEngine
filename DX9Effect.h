#pragma once

#include "DX9Image.h"
#include "DX9MonsterManager.h"

class DX9Effect final : protected DX9Image
{
private:
	const DX9Map* m_pMap;
	int m_TextureAtlasCols;
	int m_TextureAtlasRows;
	int m_UnitW;
	int m_UnitH;

	std::vector<DX9EFF_TYPE_DATA> m_TypeData;
	int m_TypeCount;

	DX9EFF_INST_DATA* m_pFisrtInstance;
	DX9EFF_INST_DATA* m_pLastInstance;
	int m_InstanceCount;

private:
	void DX9Effect::CreateVertexBuffer() override;
	void DX9Effect::CreateIndexBuffer() override;
	void DX9Effect::DeleteInstance(DX9EFF_INST_DATA* pInstance);
	void DX9Effect::Update();

public:
	DX9Effect();
	~DX9Effect() {};

	void DX9Effect::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9Effect::Destroy() override;

	void DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows);

	void DX9Effect::AddEffectType(DX9EFF_TYPE Type, DX9ANIMDATA AnimData, D3DXVECTOR2 SpawnOffset,
		D3DXVECTOR2 BBSize, int RepeatCount = 1);
	void DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, DX9ANIMDIR Dir, int Damage);

	void DX9Effect::Draw();
	void DX9Effect::DrawBoundingBox() override;

	void DX9Effect::CheckCollisionWithMonsters(DX9MonsterManager* pMonsterManager);
};