#include "DX9Anim.h"

DX9Anim::DX9Anim()
{
	m_nRows = 0;
	m_nCols = 0;

	m_nUnitW = 0;
	m_nUnitH = 0;

	m_nAnimDir = DX9ANIMDIR::Right;
	m_nCurrAnimID = DX9ANIMID::Idle;
	m_nCurrFrameID = 0;
	m_nAnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_SprFeetPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
}

int DX9Anim::MakeUnit(std::wstring TextureFN, int numCols, int numRows, float Scale)
{
	DX9Image::SetTexture(TextureFN);
	SetNumRowsAndCols(numCols, numRows);
	DX9Image::SetScale(D3DXVECTOR2(Scale, Scale));
	m_nScaledW = (int)(m_nUnitW * Scale);
	m_nScaledH = (int)(m_nUnitH * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	SetBoundingnBox(D3DXVECTOR2(0.0f, 0.0f));

	return 0;
}

int DX9Anim::SetNumRowsAndCols(int numCols, int numRows)
{
	m_nCols = numCols;
	m_nRows = numRows;

	m_nUnitW = (int)(m_nWidth / numCols);
	m_nUnitH = (int)(m_nHeight / numRows);
	
	SetSize(m_nUnitW, m_nUnitH);
	SetFrame(0);

	return 0;
}

int DX9Anim::SetFrame(int FrameID)
{
	if (m_nRows == 0)
		return -1;
	if (m_nCols == 0)
		return -1;
	
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
		UpdateVertData(u2, v1, u1, v2);
		break;
	case DX9ANIMDIR::Right:
		UpdateVertData(u1, v1, u2, v2);
		break;
	default:
		break;
	}
	
	return 0;
}

int DX9Anim::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip)
{
	m_Anims[(int)AnimID].FrameS = StartFrame;
	m_Anims[(int)AnimID].FrameE = EndFrame;

	return 0;
}

int DX9Anim::SetAnimation(DX9ANIMID AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
{
	if ((m_nCurrAnimID != AnimID) || (bForcedSet))
	{
		m_nCurrAnimID = AnimID;
		m_nCurrFrameID = m_Anims[(int)AnimID].FrameS;
		m_bRepeating = bRepeating;
		m_bBeingAnimated = !bCanInterrupt;

		SetFrame(m_nCurrFrameID);
	}

	return 0;
}

int DX9Anim::Animate()
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

	return 0;
}

int DX9Anim::SetAnimDir(DX9ANIMDIR Direction)
{
	m_nAnimDir = Direction;

	return 0;
}

int DX9Anim::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Image::SetPosition(Pos);
	m_Pos = Pos;
	m_SprFeetPos.x = Pos.x + m_nScaledW / 2;
	m_SprFeetPos.y = Pos.y + m_nScaledH;

	return 0;
}

int DX9Anim::SetPositionCentered(D3DXVECTOR2 Pos)
{
	DX9Image::SetPositionCentered(Pos);
	m_Pos = Pos;
	m_SprFeetPos.x = Pos.x + m_nScaledW / 2;
	m_SprFeetPos.y = Pos.y + m_nScaledH;

	return 0;
}

int DX9Anim::Accelerate(D3DXVECTOR2 Accel)
{
	m_Velocity += Accel;

	return 0;
}

int DX9Anim::AddVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity += Vel;

	return 0;
}

int DX9Anim::SetVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity = Vel;

	return 0;
}

int DX9Anim::MoveWithVelocity()
{
	m_Pos += m_Velocity;
	m_SprFeetPos += m_Velocity;

	SetPosition(m_Pos);

	return 0;
}

int DX9Anim::MoveConst(D3DXVECTOR2 dXY)
{
	m_Pos += dXY;
	m_SprFeetPos += dXY;

	SetPosition(m_Pos);

	return 0;
}