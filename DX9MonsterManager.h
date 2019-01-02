#pragma once

#include "DX9Life.h"

const float OFFSET_Y_HPBAR = 16.0f;

struct DX9MONANIMDATA
{
	DX9ANIMID AnimID;
	int FrameS;
	int FrameE;
	
	DX9MONANIMDATA() : FrameS(0), FrameE(0) {};
	DX9MONANIMDATA(DX9ANIMID _AnimID, int StartFrame, int EndFrame) : AnimID(_AnimID), FrameS(StartFrame), FrameE(EndFrame) {};
};

/*-----------------------------------------------------------------------------
	DX9MonsterType Class
-----------------------------------------------------------------------------*/

class DX9MonsterType
{
public:
	WSTRING m_Name;
	WSTRING m_TextureFileName;
	int m_TextureNumCols;
	int m_TextureNumRows;
	int m_HPMax;
	std::vector<DX9MONANIMDATA> m_AnimData;

public:
	DX9MonsterType() {};
	DX9MonsterType(WSTRING Name, WSTRING TextureFileName, int TextureNumCols, int TextureNumRows, int HP) :
		m_Name(Name), m_TextureFileName(TextureFileName), m_TextureNumCols(TextureNumCols), m_TextureNumRows(TextureNumRows),
		m_HPMax(HP) {};
	~DX9MonsterType() {};

	DX9MonsterType* DX9MonsterType::AddAnimation(DX9MONANIMDATA Value);
};

/*-----------------------------------------------------------------------------
	DX9MonsterInstance Class
-----------------------------------------------------------------------------*/

class DX9MonsterInstance final : public DX9Life
{
private:
	DX9MonsterType m_Type;
	int m_HPCurr;
	bool m_bUILoaded;
	DX9Image *m_HPFrame;
	DX9Image *m_HPBar;

private:
	void DX9MonsterInstance::SetUIPosition(D3DXVECTOR2 Position);
	void DX9MonsterInstance::CalculateHP();
	void DX9MonsterInstance::UpdateGlobalPosition();

public:
	DX9MonsterInstance();
	~DX9MonsterInstance() {};

	void DX9MonsterInstance::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9MonsterInstance::Destroy() override;

	void DX9MonsterInstance::SetMonsterType(DX9MonsterType Type);
	void DX9MonsterInstance::SetGlobalPosition(D3DXVECTOR2 Position) override;
	void DX9MonsterInstance::Damage(int Damage);

	void DX9MonsterInstance::Draw();
};

/*-----------------------------------------------------------------------------
	DX9Monsters Class
-----------------------------------------------------------------------------*/

class DX9MonsterManager
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	DX9Map* m_pMap;

	std::vector<DX9MonsterType> m_Types;

	std::vector<DX9MonsterInstance> m_Instances;

public:
	DX9MonsterManager() {};
	~DX9MonsterManager() {};

	void DX9MonsterManager::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9MonsterManager::Destroy();

	DX9MonsterType* DX9MonsterManager::AddMonsterType(DX9MonsterType Value);
	DX9MonsterInstance* DX9MonsterManager::Spawn(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition);

	void DX9MonsterManager::Animate();
	void DX9MonsterManager::Gravitate();
	void DX9MonsterManager::Draw();
	void DX9MonsterManager::DrawBoundingBox();

	std::vector<DX9MonsterInstance>* DX9MonsterManager::GetInstancePointer();
};