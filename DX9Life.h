#pragma once

#include "DX9Anim.h"

class DX9Life : public DX9Anim
{
protected:
	D3DXVECTOR2 m_GlobalPos;
	D3DXVECTOR2 m_GlobalPosInverse;
	D3DXVECTOR2 m_Velocity;

protected:
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
};