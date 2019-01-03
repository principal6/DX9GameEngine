#pragma once

#include "DX9Image.h"
#include "DX9Monsters.h"

class DX9Effect final : protected DX9Image
{
private:
	class EffectTypeData
	{
	private:
		EffectType m_Type;
		AnimationData m_AnimData;
		D3DXVECTOR2 m_SpawnOffset;
		D3DXVECTOR2 m_BBSize;
		int m_Delay;
		int m_RepeatCount;

	public:
		EffectTypeData() : m_Delay(0), m_RepeatCount(1) {};
		EffectTypeData(EffectType Type, AnimationData Data, D3DXVECTOR2 SpawnOffset, D3DXVECTOR2 BBSize, int Delay,
			int RepeatCount) :
			m_Type(Type), m_AnimData(Data), m_SpawnOffset(SpawnOffset), m_BBSize(BBSize), m_Delay(Delay),
			m_RepeatCount(RepeatCount) {};

		auto GetSpawnOffset() { return m_SpawnOffset; };
		auto GetBoundingBoxSize() { return m_BBSize; };
		auto GetStartFrame() { return m_AnimData.FrameS; };
		auto GetEndFrame() { return m_AnimData.FrameE; };
		auto GetRepeatCount() { return m_RepeatCount; };
		auto GetDelay() { return m_Delay; };
	};

	class EffectInstanceData
	{
	private:
		EffectInstanceData* m_pNext;
		EffectType m_Type;
		int m_TypeDataID;
		D3DXVECTOR2 m_Position;
		D3DXVECTOR2 m_Offset;
		int m_CurrFrame;
		int m_MaxRepeatCount;
		int m_CurrRepeatCount;
		BoundingBox m_BB;
		int m_Damage;

	public:
		EffectInstanceData() : m_pNext(nullptr), m_Position(D3DXVECTOR2(0, 0)) {};
		EffectInstanceData(int TypeDataID, D3DXVECTOR2 Position, D3DXVECTOR2 Offset, int CurrFrame, BoundingBox BB,
			int Damage, int RepeatCount) :
			m_pNext(nullptr), m_TypeDataID(TypeDataID), m_Position(Position), m_Offset(Offset), m_CurrFrame(CurrFrame), m_BB(BB),
			m_Damage(Damage), m_MaxRepeatCount(RepeatCount) {};

		void SetNext(EffectInstanceData* Next) { m_pNext = Next; };
		void SetCurrFrame(int FrameID) { m_CurrFrame = FrameID; };
		void SetCurrRepeatCount(int Value) { m_CurrRepeatCount = Value; };
		void SetDamage(int Damage) { m_Damage = Damage; };
		void SetBoundingBox(BoundingBox NewBB) { m_BB = NewBB; };

		auto GetBoundingBox() const { return m_BB; };
		auto GetNext() const { return m_pNext; };
		auto GetType() const { return m_Type; };
		auto GetPos() const { return m_Position; };
		auto GetOffset() const { return m_Offset; };
		auto GetCurrRepeatCount() const { return m_CurrRepeatCount; };
		auto GetCurrFrame() const { return m_CurrFrame; };
		auto GetTypeDataID() const { return m_TypeDataID; };
		auto GetDamage() const { return m_Damage; };
	};

private:
	const DX9Map* m_pMap;
	int m_TextureAtlasCols;
	int m_TextureAtlasRows;
	int m_UnitWidth;
	int m_UnitHeight;

	std::vector<EffectTypeData> m_TypeData;

	EffectInstanceData* m_pFisrtInstance;
	EffectInstanceData* m_pLastInstance;
	int m_InstanceCount;
	std::vector<int> m_DelayCounts;

private:
	void DX9Effect::CreateVertexBuffer() override;
	void DX9Effect::CreateIndexBuffer() override;
	void DX9Effect::DeleteInstance(EffectInstanceData* pInstance);
	void DX9Effect::Update();

public:
	DX9Effect();
	~DX9Effect() {};

	ReturnValue DX9Effect::Create(LPDIRECT3DDEVICE9 pDevice, DX9Map* pMap);
	void DX9Effect::Destroy() override;

	DX9Effect* DX9Effect::SetTextureAtlas(WSTRING FileName, int numCols, int numRows);
	DX9Effect* DX9Effect::AddEffectType(EffectType Type, AnimationData Data, D3DXVECTOR2 SpawnOffset, D3DXVECTOR2 BBSize,
		int Delay, int RepeatCount = 1);
	DX9Effect* DX9Effect::Spawn(int EffectTypeID, D3DXVECTOR2 Pos, AnimationDir Dir, int Damage);

	void DX9Effect::Draw() override;
	void DX9Effect::DrawBoundingBox() override;

	void DX9Effect::CheckCollisionWithMonsters(DX9MonsterManager* pMonsters);
};