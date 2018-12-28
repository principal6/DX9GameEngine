#include "DX9Effect.h"

DX9Effect::DX9Effect()
{
	m_Type = DX9EFFECTTYPE::Still;
	m_BaseSpawnOffset = D3DXVECTOR2(80.0f, 0.0f);
	m_InstanceCount = 0;
	m_pFisrtInstance = nullptr;
	m_pLastInstance = nullptr;
}

int DX9Effect::Destroy()
{
	if (m_pFisrtInstance)
	{
		DX9EFFECTDATA* curr = m_pFisrtInstance;
		DX9EFFECTDATA* next = m_pFisrtInstance->GetNext();
		delete curr;

		while (next)
		{
			curr = next;
			next = curr->GetNext();
			delete curr;
		}
	}

	DX9Anim::Destroy();
	return 0;
}

int DX9Effect::SetBaseSpawnOffset(D3DXVECTOR2 Offset)
{
	m_BaseSpawnOffset = Offset;

	return 0;
}

int DX9Effect::Spawn(D3DXVECTOR2 Pos, DX9ANIMDIR Direction)
{
	D3DXVECTOR2 offSet = Pos;
	offSet.y += m_BaseSpawnOffset.y;
	if (Direction == DX9ANIMDIR::Left)
	{
		offSet.x -= m_BaseSpawnOffset.x;
	}
	else
	{
		offSet.x += m_BaseSpawnOffset.x;
	}

	m_InstanceCount++;
	if (m_pFisrtInstance == nullptr)
	{
		m_pFisrtInstance = new DX9EFFECTDATA(Pos, offSet);
		m_pLastInstance = m_pFisrtInstance;
	}
	else
	{
		m_pLastInstance->SetNext(new DX9EFFECTDATA(Pos, offSet));
		m_pLastInstance = m_pLastInstance->GetNext();
	}
	
	SetPositionCentered(offSet);
	SetAnimation(DX9ANIMID::Effect, false);

	return 0;
}