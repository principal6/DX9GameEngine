#pragma once

#include "DX9Anim.h"
#include "DX9Monster.h"

class DX9Effect final : protected DX9Image
{
private:
	std::wstring m_TextureAtlasFileName;
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

public:
	DX9Effect();
	~DX9Effect() {};

	void DX9Effect::Create() override;
	void DX9Effect::Destroy() override;

	void DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows);

	void DX9Effect::AddEffectType(DX9EFF_TYPE Type, int StartFrame, int EndFrame, D3DXVECTOR2 SpawnOffset,
		D3DXVECTOR2 BBSize, int RepeatCount = 1);
	void DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, DX9ANIMDIR Dir, int Damage);

	void DX9Effect::Update();
	void DX9Effect::Draw() const override;
	void DX9Effect::DrawBoundingBox() override;

	void DX9Effect::CheckCollisionWithMonsters(DX9Monster* pMonsters);
};