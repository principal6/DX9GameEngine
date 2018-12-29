#pragma once

#ifndef _DX9SPRITE_H_
#define _DX9SPRITE_H_

#include "DX9Anim.h"

class DX9Sprite : public DX9Anim
{
private:
	D3DXVECTOR2 m_Velocity;

public:
	DX9Sprite();
	~DX9Sprite() {};

	int DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position);

	D3DXVECTOR2 DX9Sprite::GetGlobalPosition() { return m_GlobalPos; };
	D3DXVECTOR2 DX9Sprite::GetGlobalPositionInverse();
	D3DXVECTOR2 DX9Sprite::GetVelocity() { return m_Velocity; };
	D3DXVECTOR2 DX9Sprite::GetOffsetForMapMove();

	int DX9Sprite::MoveWithVelocity();
	int DX9Sprite::MoveConst(D3DXVECTOR2 dXY);
	int DX9Sprite::Accelerate(D3DXVECTOR2 Accel);
	int DX9Sprite::AddVelocity(D3DXVECTOR2 Vel);
	int DX9Sprite::SetVelocity(D3DXVECTOR2 Vel);
};

#endif