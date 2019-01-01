#include "DX9AnimUnit.h"

DX9AnimUnit::DX9AnimUnit()
{
	m_nRows = 0;
	m_nCols = 0;

	m_nAnimDir = DX9ANIMDIR::Right;
	m_nCurrAnimID = DX9ANIMID::Idle;
	m_nCurrFrameID = 0;
	m_nAnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_UnitW = 0;
	m_UnitH = 0;
}

void DX9AnimUnit::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DX9Image::Create(pDevice);
}

void DX9AnimUnit::Destroy()
{
	DX9Image::Destroy();
}

void DX9AnimUnit::MakeUnit(std::wstring TextureFN, int numCols, int numRows, float Scale)
{
	DX9Image::SetTexture(TextureFN);
	DX9Image::SetScale(D3DXVECTOR2(Scale, Scale));
	SetNumRowsAndCols(numCols, numRows); // m_UnitW & m_UnitH are set here
	m_ScaledW = (int)(m_UnitW * Scale);
	m_ScaledH = (int)(m_UnitH * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	SetBoundingnBox(D3DXVECTOR2(0.0f, 0.0f));
}

void DX9AnimUnit::SetNumRowsAndCols(int numCols, int numRows)
{
	m_nCols = numCols;
	m_nRows = numRows;

	m_UnitW = (int)(m_Width / numCols);
	m_UnitH = (int)(m_Height / numRows);
	
	SetSize(m_UnitW, m_UnitH);
	SetFrame(0);
}

void DX9AnimUnit::SetTexture(std::wstring FileName)
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
	if ((m_nRows == 0) || (m_nCols == 0))
		return;
	
	int FrameX = (FrameID % m_nCols);
	int FrameY = (FrameID / m_nCols);

	float u1 = ((float)FrameX / (float)m_nCols);
	float u2 = ((float)(FrameX + 1) / (float)m_nCols);
	float v1 = ((float)FrameY / (float)m_nRows);
	float v2 = ((float)(FrameY + 1) / (float)m_nRows);

	//@warning: UV offset is done in order to make sure the image borders do not invade contiguous images
	u1 += UV_OFFSET;
	v1 += UV_OFFSET;

	switch (m_nAnimDir)
	{
	case DX9ANIMDIR::Left:
		UpdateVertexData(u2, v1, u1, v2);
		break;
	case DX9ANIMDIR::Right:
		UpdateVertexData(u1, v1, u2, v2);
		break;
	default:
		break;
	}
}

void DX9AnimUnit::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip)
{
	m_Anims[(int)AnimID].FrameS = StartFrame;
	m_Anims[(int)AnimID].FrameE = EndFrame;
}

void DX9AnimUnit::SetAnimation(DX9ANIMID AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
{
	if ((m_nCurrAnimID != AnimID) || (bForcedSet))
	{
		m_nCurrAnimID = AnimID;
		m_nCurrFrameID = m_Anims[(int)AnimID].FrameS;
		m_bRepeating = bRepeating;
		m_bBeingAnimated = !bCanInterrupt;

		SetFrame(m_nCurrFrameID);
	}
}

void DX9AnimUnit::Animate()
{	
	if (m_nCurrFrameID < m_Anims[(int)m_nCurrAnimID].FrameE)
	{
		m_nCurrFrameID++;
	}
	else
	{
		m_nCurrFrameID = m_Anims[(int)m_nCurrAnimID].FrameS;
		if (!m_bRepeating)
			m_bBeingAnimated = false;
	}
	
	SetFrame(m_nCurrFrameID);
}

void DX9AnimUnit::SetDirection(DX9ANIMDIR Direction)
{
	m_nAnimDir = Direction;
}

void DX9AnimUnit::SetBoundingnBox(D3DXVECTOR2 Size)
{
	DX9Image::SetBoundingnBox(Size);
}

void DX9AnimUnit::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Image::SetPosition(Pos);
}

void DX9AnimUnit::SetPositionCentered(D3DXVECTOR2 Pos)
{
	DX9Image::SetPositionCentered(Pos);
}

bool DX9AnimUnit::IsBeingAnimated() const
{
	return m_bBeingAnimated;
}

int DX9AnimUnit::GetScaledUnitWidth() const
{
	return m_ScaledW;
}

int DX9AnimUnit::GetScaledUnitHeight() const
{
	return m_ScaledH;
}

DX9ANIMDIR DX9AnimUnit::GetDirection() const
{
	return m_nAnimDir;
}

D3DXVECTOR2 DX9AnimUnit::GetCenterPosition() const
{
	return DX9Image::GetCenterPosition();
}

DX9BOUNDINGBOX DX9AnimUnit::GetBoundingBox() const
{
	return DX9Image::GetBoundingBox();
}

void DX9AnimUnit::Draw() const
{
	DX9Image::Draw();
}

void DX9AnimUnit::DrawBoundingBox()
{
	DX9Image::DrawBoundingBox();
}