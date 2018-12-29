#include "DX9Effect.h"

DX9Effect::DX9Effect()
{
	m_TACols = 0;
	m_TARows = 0;
	m_TypeCount = 0;
	m_InstanceCount = 0;
	m_pFisrtInstance = nullptr;
	m_pLastInstance = nullptr;
}	

int DX9Effect::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir)
{
	m_pDevice = pD3DDev;
	m_Vert.clear();
	m_Ind.clear();
	m_BaseDir = BaseDir;

	CreateVB();
	CreateIB();

	m_BBLine.CreateMax(pD3DDev);

	return 0;
}

int DX9Effect::CreateVB()
{
	int rVertSize = sizeof(DX9VERTEX_IMAGE) * MAX_VB_COUNT;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return -1;
	}

	return 0;
}

int DX9Effect::CreateIB()
{
	int rIndSize = sizeof(DX9INDEX3) * (MAX_VB_COUNT / 2);
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return -1;
	}

	return 0;
}

int DX9Effect::Destroy()
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
	return 0;
}

int DX9Effect::SetTextureAtlas(std::wstring FileName, int numCols, int numRows)
{
	DX9Image::SetTexture(FileName);
	m_TACols = numCols;
	m_TARows = numRows;
	m_UnitW = m_nWidth / m_TACols;
	m_UnitH = m_nHeight / m_TARows;
	return 0;
}

int DX9Effect::AddEffectType(DX9EFF_TYPE Type, int StartFrame, int EndFrame, D3DXVECTOR2 SpawnOffset,
	D3DXVECTOR2 BBSize, int RepeatCount)
{
	m_TypeData.push_back(DX9EFF_TYPE_DATA(Type, StartFrame, EndFrame, SpawnOffset, BBSize, RepeatCount));
	m_TypeCount = (int)m_TypeData.size();
	return 0;
}

int DX9Effect::Spawn(int EffectID, D3DXVECTOR2 Pos, DX9ANIMDIR Dir)
{
	if (m_InstanceCount >= MAX_VB_COUNT)
		return -1;

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
	NewPos.x -= (m_UnitW / 2);
	NewPos.y -= (m_UnitH / 2);


	D3DXVECTOR2 tBBSize = m_TypeData[EffectID].GetBoundingBoxSize();
	DX9BOUNDINGBOX tBB;
	tBB.PosOffset.x = (float)(-tBBSize.x) / 2.0f;
	tBB.PosOffset.y = (float)(-tBBSize.y) / 2.0f;
	tBB.Size.x = m_UnitW + tBBSize.x;
	tBB.Size.y = m_UnitH + tBBSize.y;

	if (m_pFisrtInstance == nullptr)
	{
		m_pFisrtInstance = new DX9EFF_INST_DATA(EffectID, NewPos, 0, tBB);
		m_pLastInstance = m_pFisrtInstance;
	}
	else
	{
		m_pLastInstance->SetNext(new DX9EFF_INST_DATA(EffectID, NewPos, 0, tBB));
		m_pLastInstance = m_pLastInstance->GetNext();
	}
	
	return 0;
}

int DX9Effect::Update()
{
	m_Vert.clear();
	m_Ind.clear();
	m_VertCount = 0;
	m_IndCount = 0;
	m_BBLine.Clear();

	DX9EFF_INST_DATA* iterator = m_pFisrtInstance;
	int iterator_n = 0;

	if (iterator == nullptr)
		return -1;

	while (iterator)
	{
		DX9EFF_TYPE tType = iterator->GetType();
		D3DXVECTOR2 tPos = iterator->GetPos();
		int tTypeDataID = iterator->GetTypeDataID();

		int tStartFrame = m_TypeData[tTypeDataID].GetStartFrame();
		int tEndFrame = m_TypeData[tTypeDataID].GetEndFrame();
		D3DXVECTOR2 tBBSize = m_TypeData[tTypeDataID].GetBoundingBoxSize();

		int tCurrFrame = iterator->GetCurrFrame();

		float u1 = (float)(tCurrFrame % m_TACols) / (float)m_TACols;
		float u2 = u1 + (float)1 / (float)m_TACols;
		float v1 = (float)(tCurrFrame / m_TACols) / (float)m_TARows;
		float v2 = v1 + (float)1 / (float)m_TARows;

		// All frames advancing (+1) per every Update()
		tCurrFrame++;
		if (tCurrFrame > tEndFrame)
		{
			m_pFisrtInstance = iterator->GetNext();
			delete iterator;
			iterator = m_pFisrtInstance;
		}
		else
		{
			iterator->SetCurrFrame(tCurrFrame);

			m_Vert.push_back(DX9VERTEX_IMAGE(tPos.x, tPos.y, u1, v1));
			m_Vert.push_back(DX9VERTEX_IMAGE(tPos.x + m_UnitW, tPos.y, u2, v1));
			m_Vert.push_back(DX9VERTEX_IMAGE(tPos.x, tPos.y + m_UnitH, u1, v2));
			m_Vert.push_back(DX9VERTEX_IMAGE(tPos.x + m_UnitW, tPos.y + m_UnitH, u2, v2));
			m_VertCount = (int)m_Vert.size();

			m_Ind.push_back(DX9INDEX3((m_IndCount * 2), (m_IndCount * 2) + 1, (m_IndCount * 2) + 3));
			m_Ind.push_back(DX9INDEX3((m_IndCount * 2), (m_IndCount * 2) + 3, (m_IndCount * 2) + 2));
			m_IndCount = (int)m_Ind.size();
			
			DX9BOUNDINGBOX tBB = iterator->GetBoundingBox();
			m_BBLine.AddBox(tPos + tBB.PosOffset, tBB.Size, 0xFFFFFFFF);

			iterator = iterator->GetNext();
			iterator_n++;
		}
	}

	m_BBLine.UpdateVB();
	m_BBLine.UpdateIB();

	DX9Image::UpdateVB();
	DX9Image::UpdateIB();
	return 0;
}

int DX9Effect::Draw()
{
	if (m_pFisrtInstance)
	{
		return DX9Image::Draw();
	}

	return -1;
}

int DX9Effect::DrawBoundingBox()
{
	if (m_pFisrtInstance)
	{
		return m_BBLine.Draw();
	}

	return -1;
}