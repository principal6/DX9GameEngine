#include "DX9Life.h"

// Static member variables declaration
const D3DXVECTOR2 DX9Life::JUMP_POWER = D3DXVECTOR2(0.0f, -14.0f);
const D3DXVECTOR2 DX9Life::GRAVITY = D3DXVECTOR2(0.0f, 0.5f);
const float DX9Life::STRIDE = 5.0f;

DX9Life::DX9Life()
{
	m_GlobalPos = D3DXVECTOR2(0.0f, 0.0f);
	m_GlobalPosInverse = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
	m_bHitGround = true;
}

DX9Common::ReturnValue DX9Life::Create(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	ReturnValue Result = DX9AnimUnit::Create(pDevice);
	SetGlobalPosition(m_GlobalPos);

	return Result;
}

void DX9Life::Destroy()
{
	DX9AnimUnit::Destroy();
}

void DX9Life::SetMapPointer(DX9Map* pMap)
{
	m_pMap = pMap;
	int deb = 0;
}

void DX9Life::CalculateGlobalPositionInverse()
{
	m_GlobalPosInverse = m_GlobalPos;
	m_GlobalPosInverse.y = m_WindowData.WindowHeight - m_ScaledHeight - m_GlobalPos.y;
}

void DX9Life::CalculateGlobalPosition()
{
	m_GlobalPos = m_GlobalPosInverse;
	m_GlobalPos.y = m_WindowData.WindowHeight - m_ScaledHeight - m_GlobalPosInverse.y;
}

D3DXVECTOR2 DX9Life::GetGlobalPosition() const
{
	return m_GlobalPos;
}

D3DXVECTOR2 DX9Life::GetGlobalPositionInverse() const
{
	return m_GlobalPosInverse;
}

D3DXVECTOR2 DX9Life::GetVelocity() const
{
	return m_Velocity;
}

D3DXVECTOR2 DX9Life::GetOffsetForMapMove() const
{
	D3DXVECTOR2 Result;
	Result.x = m_GlobalPos.x - m_WindowData.WindowHalfWidth;
	Result.y = m_GlobalPosInverse.y - m_WindowData.WindowHalfHeight;

	if (Result.x < 0)
		Result.x = 0;

	if (Result.y > 0)
		Result.y = 0;

	return Result;
}

void DX9Life::Accelerate(D3DXVECTOR2 Accel)
{
	m_Velocity += Accel;
}

void DX9Life::AddVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity += Vel;
}

void DX9Life::SetVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity = Vel;
}

void DX9Life::MoveWithVelocity()
{
	m_GlobalPos.x += m_Velocity.x;
	m_GlobalPos.y -= m_Velocity.y; //@warning: Y value of GlobalPos is in the opposite direction!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_WindowData.WindowHalfWidth)
	{
		m_Position.x = m_GlobalPos.x;
	}
	if (m_GlobalPosInverse.y > m_WindowData.WindowHalfHeight)
	{
		m_Position.y = m_GlobalPosInverse.y;
	}

	SetPosition(m_Position);
}

void DX9Life::MoveConst(D3DXVECTOR2 dXY)
{
	m_GlobalPos.x += dXY.x;
	m_GlobalPos.y -= dXY.y; //@warning: Y value of GlobalPos is in the opposite direction!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_WindowData.WindowHalfWidth)
	{
		m_Position.x = m_GlobalPos.x;
	}
	else
	{
		if (m_Position.x != m_WindowData.WindowHalfWidth)
			m_Position.x = m_WindowData.WindowHalfWidth;
	}

	if (m_GlobalPosInverse.y > m_WindowData.WindowHalfHeight)
	{
		m_Position.y = m_GlobalPosInverse.y;
	}
	else
	{
		if (m_Position.y != m_WindowData.WindowHalfHeight)
			m_Position.y = m_WindowData.WindowHalfHeight;
	}

	SetPosition(m_Position);
}

void DX9Life::Walk(AnimationDir Direction)
{
	D3DXVECTOR2 Velocity;
	switch (Direction)
	{
	case AnimationDir::Left:
		Velocity = D3DXVECTOR2(-STRIDE, 0);
		break;
	case AnimationDir::Right:
		Velocity = D3DXVECTOR2(STRIDE, 0);
		break;
	default:
		break;
	}

	SetAnimation(AnimationID::Walk, true);
	SetDirection(Direction);

	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), Velocity);
	MoveConst(tNewVel);

}
void DX9Life::Jump()
{
	if ((m_bHitGround == false) || (m_Velocity.y > 0)) // Currently the sprite is falling down
		return;

	m_bHitGround = false;

	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), JUMP_POWER);
	SetVelocity(tNewVel);
}

void DX9Life::Gravitate()
{
	Accelerate(GRAVITY);
	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), m_Velocity);

	if (tNewVel.y < m_Velocity.y)
	{
		if (m_bHitGround)
		{
			tNewVel.y = 0;
		}
		else
		{
			m_bHitGround = true;
			//SetAnimation(AnimationID::Landing, true, true); // This is only for main Sprite
			SetAnimation(AnimationID::Idle, true, true);
		}
	}
	else
	{
		if (m_Velocity.y < 0)
			SetAnimation(AnimationID::Jumping, true, true);

		if (m_Velocity.y > 0)
			SetAnimation(AnimationID::Falling, true, true);

		m_bHitGround = false;
	}

	SetVelocity(tNewVel);
	MoveWithVelocity();
}