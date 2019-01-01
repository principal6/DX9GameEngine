#include "DX9Sprite.h"

void DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DX9Life::Create(pDevice);
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