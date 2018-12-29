#include "DX9Sprite.h"

DX9Sprite::DX9Sprite()
{
	m_GlobalPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
}

int DX9Sprite::CalculateGlobalPositionInverse()
{
	m_GlobalPosInverse = m_GlobalPos;
	m_GlobalPosInverse.y = m_WindowH - m_ScaledH - m_GlobalPos.y;

	return 0;
}

int DX9Sprite::CalculateGlobalPosition()
{
	m_GlobalPos = m_GlobalPosInverse;
	m_GlobalPos.y = m_WindowH - m_ScaledH - m_GlobalPosInverse.y;

	return 0;
}

int DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_WindowHalfW)
		m_GlobalPosInverse.x = m_WindowHalfW;

	if (m_GlobalPosInverse.y < m_WindowHalfH)
		m_GlobalPosInverse.y = m_WindowHalfH;

	SetPosition(m_GlobalPosInverse);

	return 0;
}

D3DXVECTOR2 DX9Sprite::GetGlobalPositionInverse()
{
	return m_GlobalPosInverse;
}

int DX9Sprite::Accelerate(D3DXVECTOR2 Accel)
{
	m_Velocity += Accel;

	return 0;
}

int DX9Sprite::AddVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity += Vel;

	return 0;
}

int DX9Sprite::SetVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity = Vel;

	return 0;
}

int DX9Sprite::MoveWithVelocity()
{
	m_GlobalPos.x += m_Velocity.x;
	m_GlobalPos.y -= m_Velocity.y; //GlobalPos의 y는 반대 방향!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_WindowHalfW)
	{
		m_Pos.x = m_GlobalPos.x;
	}
	if (m_GlobalPosInverse.y > m_WindowHalfH)
	{
		m_Pos.y = m_GlobalPosInverse.y;
	}
	else
		int dd = 0;

	SetPosition(m_Pos);

	return 0;
}

int DX9Sprite::MoveConst(D3DXVECTOR2 dXY)
{
	m_GlobalPos.x += dXY.x;
	m_GlobalPos.y -= dXY.y; //GlobalPos의 y는 반대 방향!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_WindowHalfW)
	{
		m_Pos.x = m_GlobalPos.x;
	}
	else
	{
		if (m_Pos.x != m_WindowHalfW)
			m_Pos.x = m_WindowHalfW;
	}

	if (m_GlobalPosInverse.y > m_WindowHalfH)
	{
		m_Pos.y = m_GlobalPosInverse.y;
	}
	else
	{	
		if (m_Pos.y != m_WindowHalfH)
			m_Pos.y = m_WindowHalfH;
	}		

	SetPosition(m_Pos);

	return 0;
}

D3DXVECTOR2 DX9Sprite::GetOffsetForMapMove()
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