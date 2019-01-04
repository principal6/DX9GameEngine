#include "DX9Monsters.h"

/*-----------------------------------------------------------------------------
	DX9MonsterType Class
-----------------------------------------------------------------------------*/

DX9MonsterType* DX9MonsterType::AddAnimation(AnimationData Value)
{
	m_AnimData.push_back(Value);
	return this;
}

void DX9MonsterType::Destroy()
{
	m_AnimData.clear();
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

DX9Common::ReturnValue DX9Monster::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData, DX9Map* pMap)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	if (pMap == nullptr)
		return ReturnValue::MAP_NULL;

	ReturnValue Result = DX9Life::Create(pDevice, refData);
	DX9Life::SetMapPointer(pMap);

	m_HPFrame = new DX9Image;
	m_HPFrame->Create(pDevice, refData);
	m_HPFrame->SetTexture(L"hpbarbg.png");

	m_HPBar = new DX9Image;
	m_HPBar->Create(pDevice, refData);
	m_HPBar->SetTexture(L"hpbar.png");

	m_bUILoaded = true;

	return Result;
}

void DX9Monster::Destroy()
{
	if (m_HPFrame)
	{
		m_HPFrame->Destroy();
		delete m_HPFrame;
	}

	if (m_HPBar)
	{
		m_HPBar->Destroy();
		delete m_HPBar;
	}

	DX9Life::Destroy();
}

void DX9Monster::SetUIPosition(D3DXVECTOR2 Position)
{
	D3DXVECTOR2 tPos = GetCenterPosition();
	tPos.y = Position.y;
	tPos.y -= OFFSET_Y_HPBAR;
	tPos.x -= static_cast<float>(m_HPFrame->GetWidth() / 2);
	m_HPFrame->SetPosition(tPos);
	m_HPBar->SetPosition(tPos);
}

void DX9Monster::SetMonsterType(DX9MonsterType Type)
{
	m_Type = Type;
	m_HPCurr = Type.m_HPMax;
}

DX9Monster* DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	SetPosition(m_GlobalPosInverse);

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
	float tW = static_cast<float>(m_HPBar->GetScaledWidth());

	m_HPBar->SetVisibleRange(static_cast<int>(tW * fPercent), m_HPBar->GetScaledHeight());
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

DX9Common::ReturnValue DX9MonsterManager::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData, DX9Map* pMap)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	if (pMap == nullptr)
		return ReturnValue::MAP_NULL;

	m_pDevice = pDevice;
	m_WindowData = refData;
	m_pMap = pMap;

	return ReturnValue::OK;
}

void DX9MonsterManager::Destroy()
{
	m_pDevice = nullptr;
	m_pMap = nullptr;

	for (DX9Monster& InstanceIterator : m_Instances)
	{
		InstanceIterator.Destroy();
	}
}

DX9MonsterType* DX9MonsterManager::AddMonsterType(DX9MonsterType Value)
{
	m_Types.push_back(Value);
	return &m_Types[m_Types.size() - 1];
}

DX9Monster* DX9MonsterManager::Spawn(WSTRING MonsterName, D3DXVECTOR2 GlobalPosition)
{
	for (DX9MonsterType& TypeIterator : m_Types)
	{
		if (TypeIterator.m_Name == MonsterName)
		{
			DX9Monster Temp;
			Temp.Create(m_pDevice, m_WindowData, m_pMap);
			Temp.SetMonsterType(TypeIterator);
			Temp.MakeUnit(TypeIterator.m_TextureFileName, TypeIterator.m_TextureNumCols, TypeIterator.m_TextureNumRows, 1.0f);

			for (DX9Common::AnimationData& AnimIterator : TypeIterator.m_AnimData)
			{
				Temp.AddAnimation(AnimIterator.AnimID, AnimIterator.FrameS, AnimIterator.FrameE);
			}

			Temp.SetGlobalPosition(GlobalPosition);
			Temp.SetAnimation(AnimationID::Idle);

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

std::vector<DX9Monster>* DX9MonsterManager::GetInstancePointer()
{
	return &m_Instances;
}