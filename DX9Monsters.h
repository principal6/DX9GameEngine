#pragma once

#include "DX9Life.h"

/*-----------------------------------------------------------------------------
	DX9MonsterType Class
-----------------------------------------------------------------------------*/

class DX9MonsterType : protected DX9Common
{
public:
	WSTRING m_Name;
	WSTRING m_TextureFileName;
	int m_TextureNumCols;
	int m_TextureNumRows;
	int m_HPMax;
	std::vector<AnimationData> m_AnimData;

public:
	DX9MonsterType() {};
	DX9MonsterType(WSTRING Name, WSTRING TextureFileName, int TextureNumCols, int TextureNumRows, int HP) :
		m_Name(Name), m_TextureFileName(TextureFileName), m_TextureNumCols(TextureNumCols), m_TextureNumRows(TextureNumRows),
		m_HPMax(HP) {};
	~DX9MonsterType() {};

	DX9MonsterType* DX9MonsterType::AddAnimation(AnimationData Value);
	void DX9MonsterType::Destroy() override;
};

/*-----------------------------------------------------------------------------
	DX9Monster Class
-----------------------------------------------------------------------------*/

class DX9Monster final : public DX9Life
{
private:
	static const float OFFSET_Y_HPBAR;

	DX9MonsterType m_Type;
	int m_HPCurr;
	bool m_bUILoaded;
	DX9Image *m_HPFrame;
	DX9Image *m_HPBar;

private:
	void DX9Monster::SetUIPosition(D3DXVECTOR2 Position);
	void DX9Monster::CalculateHP();
	void DX9Monster::UpdateGlobalPosition();

public:
	DX9Monster();
	~DX9Monster() {};

	ReturnValue DX9Monster::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9Monster::Destroy() override;

	void DX9Monster::SetMonsterType(DX9MonsterType Type);
	DX9Monster* DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position) override;
	void DX9Monster::Damage(int Damage);

	void DX9Monster::Draw();
};

/*-----------------------------------------------------------------------------
	DX9MonsterManager Class
-----------------------------------------------------------------------------*/

class DX9MonsterManager : protected DX9Common
{
private:
	static LPDIRECT3DDEVICE9 m_pDevice;
	DX9Map* m_pMap;

	std::vector<DX9MonsterType> m_Types;
	std::vector<DX9Monster> m_Instances;

public:
	DX9MonsterManager() {};
	~DX9MonsterManager() {};

	ReturnValue DX9MonsterManager::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9MonsterManager::Destroy();

	DX9MonsterType* DX9MonsterManager::AddMonsterType(DX9MonsterType Value);
	DX9Monster* DX9MonsterManager::Spawn(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition);

	void DX9MonsterManager::Animate();
	void DX9MonsterManager::Gravitate();
	void DX9MonsterManager::Draw();
	void DX9MonsterManager::DrawBoundingBox();

	std::vector<DX9Monster>* DX9MonsterManager::GetInstancePointer();
};