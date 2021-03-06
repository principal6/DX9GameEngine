#include "DX9Map.h"
#include "DX9Monsters.h"
#include "DX9Effect.h"

using namespace DX9ENGINE;

DX9Effect::DX9Effect()
{
	m_TextureAtlasCols = 0;
	m_TextureAtlasRows = 0;
	m_InstanceCount = 0;
	m_pFisrtInstance = nullptr;
	m_pLastInstance = nullptr;
}	

auto DX9Effect::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError
{
	if (pDX9Window == nullptr)
		return EError::NULLPTR_WINDOW;

	if (pMap == nullptr)
		return EError::NULLPTR_MAP;

	EError Result = DX9Image::Create(pDX9Window, BaseDir);
	m_pMap = pMap;

	DX9Image::ClearVertexAndIndexData();

	CreateVertexBuffer();
	CreateIndexBuffer();

	m_BoundingBoxLine.CreateMax(m_pDevice);

	return Result;
}

void DX9Effect::CreateVertexBuffer()
{
	int rVertSize = sizeof(SVertexImage) * MAX_UNIT_COUNT * 4;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Effect::CreateIndexBuffer()
{
	int rIndSize = sizeof(SIndex3) * MAX_UNIT_COUNT * 2;
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Effect::Destroy()
{
	if (m_pFisrtInstance)
	{
		EffectInstanceData* curr = m_pFisrtInstance;
		EffectInstanceData* next = m_pFisrtInstance->GetNext();
		delete curr;

		while (next)
		{
			curr = next;
			next = curr->GetNext();
			delete curr;
		}
	}

	DX9Image::Destroy();
}

auto DX9Effect::SetTextureAtlas(WSTRING FileName, int numCols, int numRows)->DX9Effect*
{
	DX9Image::SetTexture(FileName);
	m_TextureAtlasCols = numCols;
	m_TextureAtlasRows = numRows;
	m_UnitSize.x = m_Size.x / static_cast<float>(m_TextureAtlasCols);
	m_UnitSize.y = m_Size.y / static_cast<float>(m_TextureAtlasRows);

	return this;
}

auto DX9Effect::AddEffectType(EEffectType Type, SAnimationData Data, D3DXVECTOR2 SpawnOffset, D3DXVECTOR2 BBSize,
	int Delay, int RepeatCount)->DX9Effect*
{
	// Add this new effect type to the vector array
	m_TypeData.push_back(EffectTypeData(Type, Data, SpawnOffset, BBSize, Delay, RepeatCount));

	// Set the delay count of this new effect type to zero
	m_DelayCounts.push_back(0);

	return this;
}

// Every effect is spawned at global position
auto DX9Effect::Spawn(int EffectTypeID, D3DXVECTOR2 Pos, EAnimationDirection Dir, int Damage)->DX9Effect*
{
	// No more space for a new effect
	if (m_InstanceCount >= MAX_UNIT_COUNT)
		return nullptr;

	// Delay count is not set zero yet
	if (m_DelayCounts[EffectTypeID] > 0)
		return nullptr;

	m_InstanceCount++;

	D3DXVECTOR2 MapOffset = m_pMap->GetMapOffset();
	D3DXVECTOR2 NewPos = Pos;
	D3DXVECTOR2 SpawnOffsetBase = m_TypeData[EffectTypeID].GetSpawnOffset();
	NewPos.y += SpawnOffsetBase.y;
	if (Dir == EAnimationDirection::Left)
	{
		NewPos.x -= SpawnOffsetBase.x;
	}
	else
	{
		NewPos.x += SpawnOffsetBase.x;
	}
	NewPos.x -= (m_UnitSize.x / 2.0f); // Moving to the image's center
	NewPos.y -= (m_UnitSize.y / 2.0f);

	D3DXVECTOR2 tBBSize = m_TypeData[EffectTypeID].GetBoundingBoxSize();
	SBoundingBox tBB;
	tBB.PositionOffset.x = static_cast<float>(-tBBSize.x) / 2.0f;
	tBB.PositionOffset.y = static_cast<float>(-tBBSize.y) / 2.0f;
	tBB.Size.x = m_UnitSize.x + tBBSize.x;
	tBB.Size.y = m_UnitSize.y + tBBSize.y;
	tBB.PositionOffset += NewPos;

	int tRepeatCount = m_TypeData[EffectTypeID].GetRepeatCount();

	if (m_pFisrtInstance == nullptr)
	{
		m_pFisrtInstance = new EffectInstanceData(EffectTypeID, NewPos, MapOffset, 0, tBB, Damage, tRepeatCount);
		m_pLastInstance = m_pFisrtInstance;
	}
	else
	{
		m_pLastInstance->SetNext(new EffectInstanceData(EffectTypeID, NewPos, MapOffset, 0, tBB, Damage, tRepeatCount));
		m_pLastInstance = m_pLastInstance->GetNext();
	}

	m_DelayCounts[EffectTypeID] = m_TypeData[EffectTypeID].GetDelay();

	return this;
}

void DX9Effect::Update()
{
	D3DXVECTOR2 MapOffset = m_pMap->GetMapOffset();
	DX9Image::ClearVertexAndIndexData();
	m_BoundingBoxLine.Clear();

	EffectInstanceData* iterator = m_pFisrtInstance;
	int iterator_n = 0;

	if (iterator == nullptr)
	{
		// No instance to update
		m_InstanceCount = iterator_n;
		return;
	}

	// There are instances
	while (iterator)
	{
		EEffectType tType = iterator->GetType();
		D3DXVECTOR2 tPos = iterator->GetPos();
		int tTypeDataID = iterator->GetTypeDataID();

		int tStartFrame = m_TypeData[tTypeDataID].GetStartFrame();
		int tEndFrame = m_TypeData[tTypeDataID].GetEndFrame();
		D3DXVECTOR2 tBBSize = m_TypeData[tTypeDataID].GetBoundingBoxSize();

		int tCurrFrame = iterator->GetCurrFrame();

		STextureUV tUV;
		ConvertFrameIDIntoUV(tCurrFrame, m_UnitSize, m_AtlasSize, m_TextureAtlasCols, m_TextureAtlasRows, &tUV);

		int tCurrRepeatCount = iterator->GetCurrRepeatCount();
		int tMaxRepeatCount = m_TypeData[tTypeDataID].GetRepeatCount();

		// All frames advancing (+1) per every Update()
		tCurrFrame++;
		if (tCurrFrame > tEndFrame)
		{
			// One animation cycle has ended
			if (tCurrRepeatCount < tMaxRepeatCount - 1)
			{
				// It has to repeat the animation
				tCurrRepeatCount++;
				tCurrFrame = tStartFrame;
				iterator->SetCurrRepeatCount(tCurrRepeatCount);
			}
			else
			{
				// It repeated all the animation cycles.
				// DX9Effect's life circle ends here
				m_pFisrtInstance = iterator->GetNext();
				delete iterator;
				iterator = m_pFisrtInstance;
			}
		}

		if (tCurrFrame <= tEndFrame)
		{
			// Still being animated
			iterator->SetCurrFrame(tCurrFrame);

			D3DXVECTOR2 NewPos = tPos;
			D3DXVECTOR2 SpawnOffset = iterator->GetOffset();
			NewPos.x += -SpawnOffset.x + MapOffset.x;
			NewPos.y -= SpawnOffset.y - MapOffset.y;

			m_Vertices.push_back(SVertexImage(NewPos.x, NewPos.y, tUV.u1, tUV.v1));
			m_Vertices.push_back(SVertexImage(NewPos.x + m_UnitSize.x, NewPos.y, tUV.u2, tUV.v1));
			m_Vertices.push_back(SVertexImage(NewPos.x, NewPos.y + m_UnitSize.y, tUV.u1, tUV.v2));
			m_Vertices.push_back(SVertexImage(NewPos.x + m_UnitSize.x, NewPos.y + m_UnitSize.y, tUV.u2, tUV.v2));

			int tIndicesCount = static_cast<int>(m_Indices.size());
			m_Indices.push_back(SIndex3((tIndicesCount * 2), (tIndicesCount * 2) + 1, (tIndicesCount * 2) + 3));
			m_Indices.push_back(SIndex3((tIndicesCount * 2), (tIndicesCount * 2) + 3, (tIndicesCount * 2) + 2));
			
			
			SBoundingBox tBB = iterator->GetBoundingBox();
			tBB.PositionOffset.x += -SpawnOffset.x + MapOffset.x;
			tBB.PositionOffset.y -= SpawnOffset.y - MapOffset.y;

			m_BoundingBoxLine.AddBox(tBB.PositionOffset, tBB.Size, m_BoundingBoxColor);

			iterator = iterator->GetNext();
			iterator_n++;
		}
	}

	m_BoundingBoxLine.UpdateVB();
	m_BoundingBoxLine.UpdateIB();

	DX9Image::UpdateVertexBuffer();
	DX9Image::UpdateIndexBuffer();

	m_InstanceCount = iterator_n;

	// On each update, the delay counts -= 1
	for (int& iterator : m_DelayCounts)
	{
		if (iterator > 0)
			iterator--;
	}
}

void DX9Effect::Draw()
{
	if (m_pFisrtInstance)
	{
		Update();
		DX9Image::Draw();
	}
}

void DX9Effect::DrawBoundingBox()
{
	if (m_pFisrtInstance)
	{
		m_BoundingBoxLine.Draw();
	}
}

void DX9Effect::DeleteInstance(EffectInstanceData* pInstance)
{
	EffectInstanceData* iterator = m_pFisrtInstance;
	EffectInstanceData* next = nullptr;

	if (iterator == pInstance)
	{
		next = pInstance->GetNext();
		delete pInstance;
		m_pFisrtInstance = next;
		iterator = next;
		if (next == nullptr)
			m_pLastInstance = iterator;
	}

	while (iterator)
	{
		if (iterator->GetNext() == pInstance)
		{
			next = pInstance->GetNext();
			iterator->SetNext(next);
			delete pInstance;
			if (next == nullptr)
				m_pLastInstance = iterator;
		}
		iterator = iterator->GetNext();
	}
}

void DX9Effect::CheckCollisionWithMonsters(DX9MonsterManager* pMonsters)
{
	D3DXVECTOR2 MapOffset = m_pMap->GetMapOffset();
	EffectInstanceData* iterator = m_pFisrtInstance;
	EffectInstanceData* iterator_prev = nullptr;
	int iterator_n = 0;

	if (iterator == nullptr)
		return;

	while (iterator)
	{
		VECTOR<DX9Monster>* pInstances = pMonsters->GetInstancePointer();

		// We are gonna do hit test for all monster instances
		for (int i = 0; i < pInstances->size(); i++)
		{
			// Check collision per each DX9Effect
			D3DXVECTOR2 SpawnOffset = iterator->GetOffset();
			SBoundingBox tBBEfffect = iterator->GetBoundingBox();
			tBBEfffect.PositionOffset.x += -SpawnOffset.x + MapOffset.x;
			tBBEfffect.PositionOffset.y -= SpawnOffset.y - MapOffset.y;
			D3DXVECTOR2 tBBEFFPS = tBBEfffect.PositionOffset;
			D3DXVECTOR2 tBBEFFPE = tBBEfffect.PositionOffset + tBBEfffect.Size;
			
			SBoundingBox tBBMonster = (*pInstances)[i].GetBoundingBox();
			D3DXVECTOR2 tBBMONPS = tBBMonster.PositionOffset;
			D3DXVECTOR2 tBBMONPE = tBBMonster.PositionOffset + tBBMonster.Size;

			bool bCollision = false;
			if ((tBBEFFPS.x <= tBBMONPE.x) && (tBBEFFPE.x >= tBBMONPS.x))
			{
				if ((tBBEFFPS.y <= tBBMONPE.y) && (tBBEFFPE.y >= tBBMONPS.y))
				{
					bCollision = true;
					(*pInstances)[i].Damage(iterator->GetDamage());
				}
			}
		}

		// We set the damage of the effect to zero after the hit test,
		// because we need to damgage all the monsters that are in the damage zone of the effect!
		// and if we set the damage to zero inside the loop, we just hit one monster per effect,
		// and that's not what we want :)
		iterator->SetDamage(0);

		// @warning: The codes below make disappear the effect immediately when it touches monster
		//EffectInstanceData* temp = iterator_prev;
		//DeleteInstance(iterator);
		//iterator = temp;

		// Now iterate.
		iterator_prev = iterator;
		if (iterator)
			iterator = iterator->GetNext();
	}

}