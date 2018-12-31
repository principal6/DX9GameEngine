#include "DX9Anim.h"

// Static member declaration
int DX9Anim::m_WindowW;
int DX9Anim::m_WindowH;
float DX9Anim::m_WindowHalfW;
float DX9Anim::m_WindowHalfH;

DX9Anim::DX9Anim()
{
	m_nRows = 0;
	m_nCols = 0;

	m_nAnimDir = DX9ANIMDIR::Right;
	m_nCurrAnimID = DX9ANIMID::Idle;
	m_nCurrFrameID = 0;
	m_nAnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_WindowW = 0;
	m_WindowH = 0;
	m_WindowHalfW = 0.0f;
	m_WindowHalfH = 0.0f;

	m_UnitW = 0;
	m_UnitH = 0;
	m_GlobalPos = D3DXVECTOR2(0, 0);
	m_GlobalPosInverse = D3DXVECTOR2(0, 0);
}

void DX9Anim::Create(int WindowWidth, int WindowHeight)
{
	DX9Image::Create();

	m_WindowW = WindowWidth;
	m_WindowH = WindowHeight;

	m_WindowHalfW = (float)(m_WindowW / 2);
	m_WindowHalfH = (float)(m_WindowH / 2);
}

void DX9Anim::MakeUnit(std::wstring TextureFN, int numCols, int numRows, float Scale)
{
	DX9Image::SetTexture(TextureFN);
	SetNumRowsAndCols(numCols, numRows);
	DX9Image::SetScale(D3DXVECTOR2(Scale, Scale));
	m_ScaledW = (int)(m_UnitW * Scale);
	m_ScaledH = (int)(m_UnitH * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	SetBoundingnBox(D3DXVECTOR2(0.0f, 0.0f));
}

void DX9Anim::SetNumRowsAndCols(int numCols, int numRows)
{
	m_nCols = numCols;
	m_nRows = numRows;

	m_UnitW = (int)(m_Width / numCols);
	m_UnitH = (int)(m_Height / numRows);
	
	SetSize(m_UnitW, m_UnitH);
	SetFrame(0);
}

void DX9Anim::SetFrame(int FrameID)
{
	if ((m_nRows == 0) || (m_nCols == 0))
		return;
	
	int FrameX = (FrameID % m_nCols);
	int FrameY = (FrameID / m_nCols);

	float u1 = ((float)FrameX / (float)m_nCols);
	float u2 = ((float)(FrameX + 1) / (float)m_nCols);
	float v1 = ((float)FrameY / (float)m_nRows);
	float v2 = ((float)(FrameY + 1) / (float)m_nRows);

	// 이걸 해야 주변 텍스처를 침범하지 않음!★
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

void DX9Anim::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip)
{
	m_Anims[(int)AnimID].FrameS = StartFrame;
	m_Anims[(int)AnimID].FrameE = EndFrame;
}

void DX9Anim::SetAnimation(DX9ANIMID AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
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

void DX9Anim::Animate()
{	
	if (m_nCurrFrameID < m_Anims[(int)m_nCurrAnimID].FrameE) {
		m_nCurrFrameID++;
	}
	else {
		m_nCurrFrameID = m_Anims[(int)m_nCurrAnimID].FrameS;
		if (!m_bRepeating)
			m_bBeingAnimated = false;
	}
	
	SetFrame(m_nCurrFrameID);
}

void DX9Anim::SetAnimDir(DX9ANIMDIR Direction)
{
	m_nAnimDir = Direction;
}

void DX9Anim::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Image::SetPosition(Pos);
}

void DX9Anim::SetPositionCentered(D3DXVECTOR2 Pos)
{
	DX9Image::SetPositionCentered(Pos);
}

void DX9Anim::CalculateGlobalPositionInverse()
{
	m_GlobalPosInverse = m_GlobalPos;
	m_GlobalPosInverse.y = m_WindowH - m_ScaledH - m_GlobalPos.y;
}

void DX9Anim::CalculateGlobalPosition()
{
	m_GlobalPos = m_GlobalPosInverse;
	m_GlobalPos.y = m_WindowH - m_ScaledH - m_GlobalPosInverse.y;
}

bool DX9Anim::IsBeingAnimated() const
{
	return m_bBeingAnimated;
}

int DX9Anim::GetScaledSprWidth() const
{
	return m_ScaledW;
}

int DX9Anim::GetScaledSprHeight() const
{
	return m_ScaledH;
}

DX9ANIMDIR DX9Anim::GetAnimDir() const
{
	return m_nAnimDir;
}

D3DXVECTOR2 DX9Anim::GetCenterPosition() const
{
	return DX9Image::GetCenterPosition();
}