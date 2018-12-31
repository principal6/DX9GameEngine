#pragma once

#include "DX9Anim.h"

class DX9Sprite final : public DX9Anim
{
private:
	D3DXVECTOR2 m_Velocity;

public:
	DX9Sprite();
	~DX9Sprite() {};

	void DX9Sprite::Create() override;
	void DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position);

	D3DXVECTOR2 DX9Sprite::GetGlobalPosition() const;
	D3DXVECTOR2 DX9Sprite::GetGlobalPositionInverse() const;
	D3DXVECTOR2 DX9Sprite::GetVelocity() const;
	D3DXVECTOR2 DX9Sprite::GetOffsetForMapMove() const;

	void DX9Sprite::Accelerate(D3DXVECTOR2 Accel);
	void DX9Sprite::AddVelocity(D3DXVECTOR2 Vel);
	void DX9Sprite::SetVelocity(D3DXVECTOR2 Vel);
	void DX9Sprite::MoveWithVelocity();
	void DX9Sprite::MoveConst(D3DXVECTOR2 dXY);
};