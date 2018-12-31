#include "DX9Effect.h"

DX9Effect::DX9Effect()
{
	m_TextureAtlasCols = 0;
	m_TextureAtlasRows = 0;
	m_TypeCount = 0;
	m_InstanceCount = 0;
	m_pFisrtInstance = nullptr;
	m_pLastInstance = nullptr;
}	

void DX9Effect::Create()
{
	DX9Image::Create();
	DX9Image::ClearVertexAndIndexData();

	CreateVertexBuffer();
	CreateIndexBuffer();

	m_BBLine.CreateMax(m_pDevice);
}

void DX9Effect::CreateVertexBuffer()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * MAX_UNIT_COUNT * 4;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Effect::CreateIndexBuffer()
{
	int rIndSize = sizeof(DX9INDEX3) * MAX_UNIT_COUNT * 2;
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Effect::Destroy()
{
	if (m_pFisrtInstance)
	{
		DX9EFF_INST_DATA* curr = m_pFisrtInstance;
		DX9EFF_INST_DATA* next = m_pFisrtInstance->GetNext();
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

void DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows)
{
	DX9Image::SetTexture(FileName);
	m_TextureAtlasCols = numCols;
	m_TextureAtlasRows = numRows;
	m_UnitW = m_Width / m_TextureAtlasCols;
	m_UnitH = m_Height / m_TextureAtlasRows;
}

void DX9Effect::AddEffectType(DX9EFF_TYPE Type, DX9ANIMDATA AnimData, D3DXVECTOR2 SpawnOffset,
	D3DXVECTOR2 BBSize, int RepeatCount)
{
	m_TypeData.push_back(DX9EFF_TYPE_DATA(Type, AnimData, SpawnOffset, BBSize, RepeatCount));
	m_TypeCount = (int)m_TypeData.size();
}

// Every effect is spawned at global position
void DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, D3DXVECTOR2 MapOffset, DX9ANIMDIR Dir, int Damage)
{
	if (m_InstanceCount >= MAX_UNIT_COUNT)
		return;

	m_InstanceCount++;

	D3DXVECTOR2 NewPos = Pos;
	D3DXVECTOR2 SpawnOffsetBase = m_TypeData[EffectID].GetSpawnOffset();
	NewPos.y += SpawnOffsetBase.y;
	if (Dir == DX9ANIMDIR::Left)
	{
		NewPos.x -= SpawnOffsetBase.x;
	}
	else
	{
		NewPos.x += SpawnOffsetBase.x;
	}
	NewPos.x -= (m_UnitW / 2); // Moving to the image's center
	NewPos.y -= (m_UnitH / 2);

	D3DXVECTOR2 tBBSize = m_TypeData[EffectID].GetBoundingBoxSize();
	DX9BOUNDINGBOX tBB;
	tBB.PosOffset.x = (float)(-tBBSize.x) / 2.0f;
	tBB.PosOffset.y = (float)(-tBBSize.y) / 2.0f;
	tBB.Size.x = m_UnitW + tBBSize.x;
	tBB.Size.y = m_UnitH + tBBSize.y;
	tBB.PosOffset += NewPos;

	int tRepeatCount = m_TypeData[EffectID].GetRepeatCount();

	if (m_pFisrtInstance == nullptr)
	{
		m_pFisrtInstance = new DX9EFF_INST_DATA(EffectID, NewPos, MapOffset, 0, tBB, Damage, tRepeatCount);
		m_pLastInstance = m_pFisrtInstance;
	}
	else
	{
		m_pLastInstance->SetNext(new DX9EFF_INST_DATA(EffectID, NewPos, MapOffset, 0, tBB, Damage, tRepeatCount));
		m_pLastInstance = m_pLastInstance->GetNext();
	}
}

void DX9Effect::Update(D3DXVECTOR2 MapOffset)
{
	DX9Image::ClearVertexAndIndexData();
	m_BBLine.Clear();

	DX9EFF_INST_DATA* iterator = m_pFisrtInstance;
	int iterator_n = 0;

	if (iterator == nullptr)
	{
		m_InstanceCount = iterator_n;
		return;
	}

	while (iterator)
	{
		DX9EFF_TYPE tType = iterator->GetType();
		D3DXVECTOR2 tPos = iterator->GetPos();
		int tTypeDataID = iterator->GetTypeDataID();

		int tStartFrame = m_TypeData[tTypeDataID].GetStartFrame();
		int tEndFrame = m_TypeData[tTypeDataID].GetEndFrame();
		D3DXVECTOR2 tBBSize = m_TypeData[tTypeDataID].GetBoundingBoxSize();

		int tCurrFrame = iterator->GetCurrFrame();

		float u1 = (float)(tCurrFrame % m_TextureAtlasCols) / (float)m_TextureAtlasCols;
		float u2 = u1 + (float)1 / (float)m_TextureAtlasCols;
		float v1 = (float)(tCurrFrame / m_TextureAtlasCols) / (float)m_TextureAtlasRows;
		float v2 = v1 + (float)1 / (float)m_TextureAtlasRows;

		int tCurrRepeatCount = iterator->GetCurrRepeatCount();
		int tMaxRepeatCount = m_TypeData[tTypeDataID].GetRepeatCount();

		// All frames advancing (+1) per every Update()
		tCurrFrame++;
		if (tCurrFrame > tEndFrame)
		{
			if (tCurrRepeatCount < tMaxRepeatCount - 1)
			{
				tCurrRepeatCount++;
				tCurrFrame = tStartFrame;
				iterator->SetCurrRepeatCount(tCurrRepeatCount);
			}
			else
			{
				// DX9Effect's life circle ends here
				m_pFisrtInstance = iterator->GetNext();
				delete iterator;
				iterator = m_pFisrtInstance;
			}
		}

		if (tCurrFrame <= tEndFrame)
		{
			iterator->SetCurrFrame(tCurrFrame);

			D3DXVECTOR2 NewPos = tPos;
			D3DXVECTOR2 SpawnOffset = iterator->GetOffset();
			NewPos.x += -SpawnOffset.x + MapOffset.x;
			NewPos.y -= SpawnOffset.y - MapOffset.y;

			m_Vert.push_back(DX9VERTEX_IMAGE(NewPos.x, NewPos.y, u1, v1));
			m_Vert.push_back(DX9VERTEX_IMAGE(NewPos.x + m_UnitW, NewPos.y, u2, v1));
			m_Vert.push_back(DX9VERTEX_IMAGE(NewPos.x, NewPos.y + m_UnitH, u1, v2));
			m_Vert.push_back(DX9VERTEX_IMAGE(NewPos.x + m_UnitW, NewPos.y + m_UnitH, u2, v2));
			m_VertCount = (int)m_Vert.size();

			m_Ind.push_back(DX9INDEX3((m_IndCount * 2), (m_IndCount * 2) + 1, (m_IndCount * 2) + 3));
			m_Ind.push_back(DX9INDEX3((m_IndCount * 2), (m_IndCount * 2) + 3, (m_IndCount * 2) + 2));
			m_IndCount = (int)m_Ind.size();
			
			DX9BOUNDINGBOX tBB = iterator->GetBoundingBox();
			tBB.PosOffset.x += -SpawnOffset.x + MapOffset.x;
			tBB.PosOffset.y -= SpawnOffset.y - MapOffset.y;

			m_BBLine.AddBox(tBB.PosOffset, tBB.Size, 0xFFFFFFFF);

			iterator = iterator->GetNext();
			iterator_n++;
		}
	}

	m_BBLine.UpdateVB();
	m_BBLine.UpdateIB();

	DX9Image::UpdateVertexBuffer();
	DX9Image::UpdateIndexBuffer();

	m_InstanceCount = iterator_n;
}

void DX9Effect::Draw() const
{
	if (m_pFisrtInstance)
	{
		DX9Image::Draw();
	}
}

void DX9Effect::DrawBoundingBox()
{
	if (m_pFisrtInstance)
	{
		m_BBLine.Draw();
	}
}

void DX9Effect::DeleteInstance(DX9EFF_INST_DATA* pInstance)
{
	DX9EFF_INST_DATA* iterator = m_pFisrtInstance;
	DX9EFF_INST_DATA* next = nullptr;

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

void DX9Effect::CheckCollisionWithMonsters(D3DXVECTOR2 MapOffset, DX9Monster* pMonsters)
{
	DX9EFF_INST_DATA* iterator = m_pFisrtInstance;
	DX9EFF_INST_DATA* iterator_prev = nullptr;
	int iterator_n = 0;

	if (iterator == nullptr)
		return;

	while (iterator)
	{
		// Check collision per each DX9Effect
		D3DXVECTOR2 SpawnOffset = iterator->GetOffset();
		DX9BOUNDINGBOX tBBEfffect = iterator->GetBoundingBox();
		tBBEfffect.PosOffset.x += -SpawnOffset.x + MapOffset.x;
		tBBEfffect.PosOffset.y -= SpawnOffset.y - MapOffset.y;
		D3DXVECTOR2 tBBEFFPS = tBBEfffect.PosOffset;
		D3DXVECTOR2 tBBEFFPE = tBBEfffect.PosOffset + tBBEfffect.Size;
		DX9BOUNDINGBOX tBBMonster = pMonsters->GetBoundingBox();
		D3DXVECTOR2 tBBMONPS = tBBMonster.PosOffset;
		D3DXVECTOR2 tBBMONPE = tBBMonster.PosOffset + tBBMonster.Size;

		bool bCollision = false;
		if ((tBBEFFPS.x <= tBBMONPE.x) && (tBBEFFPE.x >= tBBMONPS.x))
		{
			if ((tBBEFFPS.y <= tBBMONPE.y) && (tBBEFFPE.y >= tBBMONPS.y))
			{
				bCollision = true;
				pMonsters->Damage(iterator->GetDamage());
				iterator->SetDamage(0);

				// @warning: The codes below make disappear the effect immediately
				//DX9EFF_INST_DATA* temp = iterator_prev;
				//DeleteInstance(iterator);
				//iterator = temp;
			}
		}

		iterator_prev = iterator;
		if (iterator)
			iterator = iterator->GetNext();
	}

}