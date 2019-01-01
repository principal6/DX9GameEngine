#include "DX9Life.h"

DX9Life::DX9Life()
{
	m_GlobalPos = D3DXVECTOR2(0.0f, 0.0f);
	m_GlobalPosInverse = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
}

void DX9Life::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DX9AnimUnit::Create(pDevice);
	SetGlobalPosition(m_GlobalPos);
}

void DX9Life::CalculateGlobalPositionInverse()
{
	m_GlobalPosInverse = m_GlobalPos;
	m_GlobalPosInverse.y = m_WindowH - m_ScaledH - m_GlobalPos.y;
}

void DX9Life::CalculateGlobalPosition()
{
	m_GlobalPos = m_GlobalPosInverse;
	m_GlobalPos.y = m_WindowH - m_ScaledH - m_GlobalPosInverse.y;
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
	Result.x = m_GlobalPos.x - m_WindowHalfW;
	Result.y = m_GlobalPosInverse.y - m_WindowHalfH;

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

	if (m_GlobalPosInverse.x < m_WindowHalfW)
	{
		m_Position.x = m_GlobalPos.x;
	}
	if (m_GlobalPosInverse.y > m_WindowHalfH)
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

	if (m_GlobalPosInverse.x < m_WindowHalfW)
	{
		m_Position.x = m_GlobalPos.x;
	}
	else
	{
		if (m_Position.x != m_WindowHalfW)
			m_Position.x = m_WindowHalfW;
	}

	if (m_GlobalPosInverse.y > m_WindowHalfH)
	{
		m_Position.y = m_GlobalPosInverse.y;
	}
	else
	{
		if (m_Position.y != m_WindowHalfH)
			m_Position.y = m_WindowHalfH;
	}

	SetPosition(m_Position);
}