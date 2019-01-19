#include "DX9AnimUnit.h"

using namespace DX9ENGINE;

DX9AnimUnit::DX9AnimUnit()
{
	m_NumRows = 0;
	m_NumCols = 0;

	m_AnimDir = AnimationDir::Right;
	m_CurrAnimID = AnimationID::Idle;
	m_CurrFrameID = 0;
	m_AnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_UnitWidth = 0;
	m_UnitHeight = 0;
}

auto DX9AnimUnit::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData)->Error
{
	if (pDevice == nullptr)
		return Error::DEVICE_NULL;

	return DX9Image::Create(pDevice, refData);
}

void DX9AnimUnit::Destroy()
{
	DX9Image::Destroy();
}

auto DX9AnimUnit::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale)->DX9AnimUnit*
{
	DX9Image::SetTexture(TextureFN);
	DX9Image::SetScale(D3DXVECTOR2(Scale, Scale));
	SetNumRowsAndCols(numCols, numRows); // m_UnitWidth & m_UnitHeight are set here
	m_ScaledWidth = static_cast<int>(m_UnitWidth * Scale);
	m_ScaledHeight = static_cast<int>(m_UnitHeight * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	SetBoundingBox(D3DXVECTOR2(0.0f, 0.0f));

	return this;
}

void DX9AnimUnit::SetNumRowsAndCols(int numCols, int numRows)
{
	m_NumCols = numCols;
	m_NumRows = numRows;

	m_UnitWidth = static_cast<int>(m_Width / numCols);
	m_UnitHeight = static_cast<int>(m_Height / numRows);
	
	SetSize(m_UnitWidth, m_UnitHeight);
	SetFrame(0);
}

void DX9AnimUnit::SetTexture(WSTRING FileName)
{
	DX9Image::SetTexture(FileName);
}

void DX9AnimUnit::SetScale(D3DXVECTOR2 Scale)
{
	DX9Image::SetScale(Scale);
}

void DX9AnimUnit::SetAlpha(int Alpha)
{
	DX9Image::SetAlpha(Alpha);
}

void DX9AnimUnit::SetFrame(int FrameID)
{
	if ((m_NumRows == 0) || (m_NumCols == 0))
		return;
	
	FloatUV tUV;
	DX9Common::ConvertFrameIDIntoUV(FrameID, m_NumCols, m_NumRows, &tUV);

	switch (m_AnimDir)
	{
	case AnimationDir::Left:
		UpdateVertexData(tUV.u2, tUV.v1, tUV.u1, tUV.v2);
		break;
	case AnimationDir::Right:
		UpdateVertexData(tUV.u1, tUV.v1, tUV.u2, tUV.v2);
		break;
	default:
		break;
	}
}

auto DX9AnimUnit::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)->DX9AnimUnit*
{
	m_AnimData.push_back(DX9Common::AnimationData(AnimID, StartFrame, EndFrame));

	return this;
}

void DX9AnimUnit::SetAnimation(AnimationID AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
{
	int AnimIDInt = static_cast<int>(AnimID);

	if (AnimIDInt > m_AnimData.size())
		return;

	if ((m_CurrAnimID != AnimID) || (bForcedSet))
	{
		m_CurrAnimID = AnimID;
		m_CurrFrameID = m_AnimData[AnimIDInt].FrameS;
		m_bRepeating = bRepeating;
		m_bBeingAnimated = !bCanInterrupt;

		SetFrame(m_CurrFrameID);
	}
}

void DX9AnimUnit::Animate()
{	
	if (m_CurrFrameID < m_AnimData[static_cast<int>(m_CurrAnimID)].FrameE)
	{
		m_CurrFrameID++;
	}
	else
	{
		m_CurrFrameID = m_AnimData[static_cast<int>(m_CurrAnimID)].FrameS;
		if (!m_bRepeating)
			m_bBeingAnimated = false;
	}
	
	SetFrame(m_CurrFrameID);
}

void DX9AnimUnit::SetDirection(AnimationDir Direction)
{
	m_AnimDir = Direction;
}

auto DX9AnimUnit::SetBoundingBox(D3DXVECTOR2 Size)->DX9AnimUnit*
{
	DX9Image::SetBoundingBox(Size);
	return this;
}

void DX9AnimUnit::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Image::SetPosition(Pos);
}

void DX9AnimUnit::SetPositionCentered(D3DXVECTOR2 Pos)
{
	DX9Image::SetPositionCentered(Pos);
}

auto DX9AnimUnit::IsBeingAnimated() const->bool
{
	return m_bBeingAnimated;
}

auto DX9AnimUnit::GetScaledUnitWidth() const->int
{
	return m_ScaledWidth;
}

auto DX9AnimUnit::GetScaledUnitHeight() const->int
{
	return m_ScaledHeight;
}

auto DX9AnimUnit::GetDirection() const->AnimationDir
{
	return m_AnimDir;
}

auto DX9AnimUnit::GetCenterPosition() const->D3DXVECTOR2
{
	return DX9Image::GetCenterPosition();
}

auto DX9AnimUnit::GetBoundingBox() const->BoundingBox
{
	return DX9Image::GetBoundingBox();
}

void DX9AnimUnit::Draw()
{
	DX9Image::Draw();
}

void DX9AnimUnit::DrawBoundingBox()
{
	DX9Image::DrawBoundingBox();
}