#include "DX9Sprite.h"

using namespace DX9ENGINE;

DX9Common::ReturnValue DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData, DX9Map* pMap)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	if (pMap == nullptr)
		return ReturnValue::MAP_NULL;

	ReturnValue Result = DX9Life::Create(pDevice, refData);
	DX9Life::SetMapPointer(pMap);
	return Result;
}

DX9Sprite* DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_WindowData.WindowHalfWidth)
		m_GlobalPosInverse.x = m_WindowData.WindowHalfWidth;

	if (m_GlobalPosInverse.y < m_WindowData.WindowHalfHeight)
		m_GlobalPosInverse.y = m_WindowData.WindowHalfHeight;

	SetPosition(m_GlobalPosInverse);

	return this;
}

DX9Sprite* DX9Sprite::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale)
{
	DX9AnimUnit::MakeUnit(TextureFN, numCols, numRows, Scale);
	return this;
}

DX9Sprite* DX9Sprite::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)
{
	DX9AnimUnit::AddAnimation(AnimID, StartFrame, EndFrame);
	return this;
}

DX9Sprite* DX9Sprite::SetBoundingBox(D3DXVECTOR2 Size)
{
	DX9AnimUnit::SetBoundingBox(Size);
	return this;
}