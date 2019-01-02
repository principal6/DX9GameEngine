#include "DX9Sprite.h"

void DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData, DX9Map* pMap)
{
	DX9Life::Create(pDevice, pData);
	DX9Life::SetMapPointer(pMap);
}

DX9Sprite* DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_pShareData->m_WindowHalfW)
		m_GlobalPosInverse.x = m_pShareData->m_WindowHalfW;

	if (m_GlobalPosInverse.y < m_pShareData->m_WindowHalfH)
		m_GlobalPosInverse.y = m_pShareData->m_WindowHalfH;

	SetPosition(m_GlobalPosInverse);

	return this;
}

DX9Sprite* DX9Sprite::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale)
{
	DX9AnimUnit::MakeUnit(TextureFN, numCols, numRows, Scale);
	return this;
}

DX9Sprite* DX9Sprite::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip)
{
	DX9AnimUnit::AddAnimation(AnimID, StartFrame, EndFrame, HFlip);
	return this;
}

DX9Sprite* DX9Sprite::SetBoundingnBox(D3DXVECTOR2 Size)
{
	DX9AnimUnit::SetBoundingnBox(Size);
	return this;
}