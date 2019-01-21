#include "Core/DX9Base.h"
#include "DX9Sprite.h"

using namespace DX9ENGINE;

auto DX9Sprite::Create(DX9Base* pBase, WSTRING BaseDir, DX9Map* pMap)->Error
{
	if (pBase == nullptr)
		return Error::BASE_NULL;

	if (pMap == nullptr)
		return Error::MAP_NULL;

	Error Result = DX9Life::Create(pBase, BaseDir);
	m_pMap = pMap;

	return Result;
}

auto DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Sprite*
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_pBase->GetWindowData()->WindowHalfWidth)
		m_GlobalPosInverse.x = m_pBase->GetWindowData()->WindowHalfWidth;

	if (m_GlobalPosInverse.y < m_pBase->GetWindowData()->WindowHalfHeight)
		m_GlobalPosInverse.y = m_pBase->GetWindowData()->WindowHalfHeight;

	SetPosition(m_GlobalPosInverse);

	return this;
}

auto DX9Sprite::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale)->DX9Sprite*
{
	DX9AnimUnit::MakeUnit(TextureFN, numCols, numRows, Scale);
	return this;
}

auto DX9Sprite::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)->DX9Sprite*
{
	DX9AnimUnit::AddAnimation(AnimID, StartFrame, EndFrame);
	return this;
}

auto DX9Sprite::SetBoundingBox(D3DXVECTOR2 Size)->DX9Sprite*
{
	DX9AnimUnit::SetBoundingBox(Size);
	return this;
}