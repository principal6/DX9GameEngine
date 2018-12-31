#include "DX9Sprite.h"

DX9Sprite::DX9Sprite()
{
	m_GlobalPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
}

void DX9Sprite::Create(int WindowWidth, int WindowHeight)
{
	DX9Anim::Create(WindowWidth, WindowHeight);
	SetGlobalPosition(m_GlobalPos);
}

void DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_WindowHalfW)
		m_GlobalPosInverse.x = m_WindowHalfW;

	if (m_GlobalPosInverse.y < m_WindowHalfH)
		m_GlobalPosInverse.y = m_WindowHalfH;

	SetPosition(m_GlobalPosInverse);
}

D3DXVECTOR2 DX9Sprite::GetGlobalPosition() const
{
	return m_GlobalPos;
}

D3DXVECTOR2 DX9Sprite::GetGlobalPositionInverse() const
{
	return m_GlobalPosInverse;
}

D3DXVECTOR2 DX9Sprite::GetVelocity() const
{
	return m_Velocity;
}

D3DXVECTOR2 DX9Sprite::GetOffsetForMapMove() const
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

void DX9Sprite::Accelerate(D3DXVECTOR2 Accel)
{
	m_Velocity += Accel;
}

void DX9Sprite::AddVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity += Vel;
}

void DX9Sprite::SetVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity = Vel;
}

void DX9Sprite::MoveWithVelocity()
{
	m_GlobalPos.x += m_Velocity.x;
	m_GlobalPos.y -= m_Velocity.y; //GlobalPos의 y는 반대 방향!

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

void DX9Sprite::MoveConst(D3DXVECTOR2 dXY)
{
	m_GlobalPos.x += dXY.x;
	m_GlobalPos.y -= dXY.y; //GlobalPos의 y는 반대 방향!

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