#pragma once

#include "DX9AnimUnit.h"
#include "DX9Map.h"

const D3DXVECTOR2 JUMP_POWER = D3DXVECTOR2(0.0f, -14.0f);
const D3DXVECTOR2 GRAVITY = D3DXVECTOR2(0.0f, 0.5f);
const float STRIDE = 5.0f;

class DX9Life : public DX9AnimUnit
{
protected:
	const DX9Map* m_pMap;
	D3DXVECTOR2 m_GlobalPos;
	D3DXVECTOR2 m_GlobalPosInverse;
	D3DXVECTOR2 m_Velocity;
	bool m_bHitGround;

protected:
	void DX9Life::SetMapPointer(DX9Map* pMap);
	void DX9Life::CalculateGlobalPositionInverse();
	void DX9Life::CalculateGlobalPosition();

public:
	DX9Life();
	virtual ~DX9Life() {};

	void DX9Life::Create(LPDIRECT3DDEVICE9 pDevice) override;
	virtual void DX9Life::SetGlobalPosition(D3DXVECTOR2 Position) = 0;

	D3DXVECTOR2 DX9Life::GetGlobalPosition() const;
	D3DXVECTOR2 DX9Life::GetGlobalPositionInverse() const;
	D3DXVECTOR2 DX9Life::GetVelocity() const;
	D3DXVECTOR2 DX9Life::GetOffsetForMapMove() const;

	void DX9Life::Accelerate(D3DXVECTOR2 Accel);
	void DX9Life::AddVelocity(D3DXVECTOR2 Vel);
	void DX9Life::SetVelocity(D3DXVECTOR2 Vel);
	void DX9Life::MoveWithVelocity();
	void DX9Life::MoveConst(D3DXVECTOR2 dXY);

	void DX9Life::Walk(DX9ANIMDIR Direction);
	void DX9Life::Jump();
	void DX9Life::Gravitate();
};