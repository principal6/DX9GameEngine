#pragma once

#include "DX9Life.h"

namespace DX9ENGINE
{
	/*-----------------------------------------------------------------------------
		DX9MonsterType Class
	-----------------------------------------------------------------------------*/
	class DX9MonsterType final
	{
	public:
		WSTRING m_Name;
		WSTRING m_TextureFileName;
		int m_TextureNumCols;
		int m_TextureNumRows;
		int m_HPMax;
		VECTOR<SAnimationData> m_AnimData;

	public:
		DX9MonsterType() {};
		DX9MonsterType(WSTRING Name, WSTRING TextureFileName, int TextureNumCols, int TextureNumRows, int HP) :
			m_Name(Name), m_TextureFileName(TextureFileName), m_TextureNumCols(TextureNumCols),
			m_TextureNumRows(TextureNumRows), m_HPMax(HP) {};
		~DX9MonsterType() {};

		auto DX9MonsterType::AddAnimation(SAnimationData Value)->DX9MonsterType*;
	};

	/*-----------------------------------------------------------------------------
		DX9Monster Class
	-----------------------------------------------------------------------------*/
	class DX9Monster final : public DX9Life
	{
	public:
		DX9Monster();
		~DX9Monster() {};

		auto DX9Monster::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError;
		void DX9Monster::Destroy() override;

		void DX9Monster::SetMonsterType(DX9MonsterType Type);
		auto DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Monster* override;
		void DX9Monster::Damage(int Damage);

		void DX9Monster::Draw();

	private:
		void DX9Monster::SetUIPosition(D3DXVECTOR2 Position);
		void DX9Monster::CalculateHP();
		void DX9Monster::UpdateGlobalPosition();

	private:
		static const float OFFSET_Y_HPBAR;

		DX9MonsterType m_Type;
		int m_HPCurr;
		bool m_bUILoaded;
		DX9Image *m_HPFrame;
		DX9Image *m_HPBar;
	};

	/*-----------------------------------------------------------------------------
		DX9MonsterManager Class
	-----------------------------------------------------------------------------*/
	class DX9MonsterManager final
	{
	public:
		DX9MonsterManager() {};
		~DX9MonsterManager() {};

		auto DX9MonsterManager::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError;
		void DX9MonsterManager::Destroy();

		auto DX9MonsterManager::AddMonsterType(DX9MonsterType Value)->DX9MonsterType*;
		auto DX9MonsterManager::Spawn(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*;

		void DX9MonsterManager::Animate();
		void DX9MonsterManager::Gravitate();
		void DX9MonsterManager::Draw();
		void DX9MonsterManager::DrawBoundingBox();

		auto DX9MonsterManager::GetInstancePointer()->VECTOR<DX9Monster>*;

	private:
		static LPDIRECT3DDEVICE9 m_pDevice;
		DX9Window* m_pDX9Window;
		DX9Map* m_pMap;
		WSTRING m_BaseDir;

		VECTOR<DX9MonsterType> m_Types;
		VECTOR<DX9Monster> m_Instances;
	};
};