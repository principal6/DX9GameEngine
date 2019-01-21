#include "../Core/DX9Window.h"
#include "DX9Map.h"
#include "DX9Monsters.h"

using namespace DX9ENGINE;

/*-----------------------------------------------------------------------------
	DX9MonsterType Class
-----------------------------------------------------------------------------*/

auto DX9MonsterType::AddAnimation(SAnimationData Value)->DX9MonsterType*
{
	m_AnimData.push_back(Value);
	return this;
}

/*-----------------------------------------------------------------------------
	DX9Monster Class
-----------------------------------------------------------------------------*/

// Static member variable declaration
const float DX9Monster::OFFSET_Y_HPBAR = 16.0f;

DX9Monster::DX9Monster()
{
	m_HPCurr = 0;
	m_bUILoaded = false;
	m_HPFrame = nullptr;
	m_HPBar = nullptr;
}

auto DX9Monster::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError
{
	if (DX_SUCCEEDED(DX9Life::Create(pDX9Window, BaseDir, pMap)))
	{
		m_HPFrame = new DX9Image;
		m_HPFrame->Create(pDX9Window, BaseDir);
		m_HPFrame->SetTexture(L"gamegui.png");
		m_HPFrame->SetAtlasUV(D3DXVECTOR2(0, 0), D3DXVECTOR2(47, 10));

		m_HPBar = new DX9Image;
		m_HPBar->Create(pDX9Window, BaseDir);
		m_HPBar->SetTexture(L"gamegui.png");
		m_HPBar->SetAtlasUV(D3DXVECTOR2(0, 10), D3DXVECTOR2(47, 10));

		m_bUILoaded = true;

		return EError::OK;
	}

	return EError::LIFE_NOT_CREATED;
}

void DX9Monster::Destroy()
{
	DX_DESTROY(m_HPFrame);
	DX_DESTROY(m_HPBar);
}

void DX9Monster::SetUIPosition(D3DXVECTOR2 Position)
{
	D3DXVECTOR2 tPos = GetCenterPosition();
	tPos.y = Position.y;
	tPos.y -= OFFSET_Y_HPBAR + m_BoundingBoxExtraSize.y;
	tPos.x -= m_HPFrame->GetSize().x / 2.0f;

	m_HPFrame->SetPosition(tPos);
	m_HPBar->SetPosition(tPos);
}

void DX9Monster::SetMonsterType(DX9MonsterType Type)
{
	m_Type = Type;
	m_HPCurr = Type.m_HPMax;
}

auto DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Monster*
{
	m_GlobalPos = Position;
	DX9Life::CalculateGlobalPositionInverse();

	DX9Life::SetPosition(m_GlobalPosInverse);

	if (m_bUILoaded)
		SetUIPosition(m_GlobalPosInverse);

	return this;
}

void DX9Monster::UpdateGlobalPosition()
{
	D3DXVECTOR2 MapOffset = m_pMap->GetMapOffset();
	float MapOffsetZeroY = static_cast<float>(m_pMap->GetMapOffsetZeroY());

	D3DXVECTOR2 tGP = m_GlobalPos;
	m_GlobalPos.x += MapOffset.x;
	m_GlobalPos.y -= MapOffset.y - MapOffsetZeroY;

	CalculateGlobalPositionInverse();
	m_GlobalPos = tGP;

	SetPosition(m_GlobalPosInverse);

	if (m_bUILoaded)
		SetUIPosition(m_GlobalPosInverse);
}

void DX9Monster::CalculateHP()
{
	float fPercent = static_cast<float>(DX9Monster::m_HPCurr) / static_cast<float>(m_Type.m_HPMax);
	float tWidth = m_HPBar->GetScaledSize().x;

	m_HPBar->SetVisibleRange(D3DXVECTOR2(tWidth * fPercent, m_HPBar->GetScaledSize().y));
}

void DX9Monster::Damage(int Damage)
{
	m_HPCurr -= Damage;
	if (m_HPCurr < 0)
		m_HPCurr = 0;
}

void DX9Monster::Draw()
{
	CalculateHP();
	UpdateGlobalPosition();

	DX9Life::Draw();
	m_HPFrame->Draw();
	m_HPBar->Draw();
}

/*-----------------------------------------------------------------------------
	DX9MonsterManager Class
-----------------------------------------------------------------------------*/

// Static member variable declaration
LPDIRECT3DDEVICE9 DX9MonsterManager::m_pDevice;

auto DX9MonsterManager::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError
{
	if (pDX9Window == nullptr)
		return EError::NULLPTR_BASE;

	if (pMap == nullptr)
		return EError::NULLPTR_MAP;

	m_pDX9Window = pDX9Window;
	m_pDevice = pDX9Window->GetDevice();
	m_BaseDir = BaseDir;
	m_pMap = pMap;

	return EError::OK;
}

void DX9MonsterManager::Destroy()
{
	m_pDX9Window = nullptr;
	m_pDevice = nullptr;
	m_pMap = nullptr;

	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.Destroy();
	}
}

auto DX9MonsterManager::AddMonsterType(DX9MonsterType Value)->DX9MonsterType*
{
	m_Types.push_back(Value);
	return &m_Types[m_Types.size() - 1];
}

auto DX9MonsterManager::Spawn(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)->DX9Monster*
{
	for (DX9MonsterType& TypeIterator : m_Types)
	{
		if (TypeIterator.m_Name == MonsterName)
		{
			DX9Monster Temp;
			Temp.Create(m_pDX9Window, m_BaseDir, m_pMap);
			Temp.SetMonsterType(TypeIterator);
			Temp.MakeLife(TypeIterator.m_TextureFileName, TypeIterator.m_TextureNumCols, TypeIterator.m_TextureNumRows, 1.0f);

			for (SAnimationData& AnimIterator : TypeIterator.m_AnimData)
			{
				Temp.AddAnimation(AnimIterator.AnimID, AnimIterator.FrameS, AnimIterator.FrameE);
			}

			Temp.SetGlobalPosition(GlobalPosition);
			Temp.SetAnimation(EAnimationID::Idle);
			Temp.SetBoundingBox(TypeIterator.m_BoundingBoxExtraSize);

			m_Instances.push_back(Temp);

			return &m_Instances[m_Instances.size() - 1];
		}
	}
	return nullptr;
}

void DX9MonsterManager::Animate()
{
	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.Animate();
	}
}

void DX9MonsterManager::Gravitate()
{
	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.Gravitate();
	}
}

void DX9MonsterManager::Draw()
{
	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.Draw();
	}
}

void DX9MonsterManager::DrawBoundingBox()
{
	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.DrawBoundingBox();
	}
}

auto DX9MonsterManager::GetInstancePointer()->VECTOR<DX9Monster>*
{
	return &m_Instances;
}