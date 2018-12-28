#include "DX9Anim.h"

DX9Anim::DX9Anim()
{
	m_nRows = 0;
	m_nCols = 0;

	m_nSprW = 0;
	m_nSprH = 0;
	m_nSprScaledW = 0;
	m_nSprScaledH = 0;

	m_nAnimDir = DX9ANIMDIR::Right;
	m_nCurrAnimID = 0;
	m_nCurrFrame = 0;
	m_nAnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_SprPos = D3DXVECTOR2(0.0f, 0.0f);
	m_SprFeetPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
}

int DX9Anim::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir)
{
	DX9Image::Create(pD3DDev, BaseDir);

	m_BBLine.Create(pD3DDev);
	m_BBLine.AddBox(D3DXVECTOR2(0, 0), D3DXVECTOR2(10, 10), D3DCOLOR_ARGB(255, 255, 255, 255));
	m_BBLine.AddEnd();
	return 0;
}

int DX9Anim::Destroy()
{
	m_BBLine.Destroy();
	DX9Image::Destroy();
	return 0;
}

int DX9Anim::MakeSprite(std::wstring TextureFN, int numCols, int numRows, float Scale)
{
	DX9Image::SetTexture(TextureFN);
	SetNumRowsAndCols(numCols, numRows);
	DX9Image::SetScale(Scale, Scale);
	m_nSprScaledW = (int)(m_nSprW * Scale);
	m_nSprScaledH = (int)(m_nSprH * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	return 0;
}

int DX9Anim::SetNumRowsAndCols(int numCols, int numRows)
{
	m_nCols = numCols;
	m_nRows = numRows;

	m_nSprW = (int)(m_nWidth / numCols);
	m_nSprH = (int)(m_nHeight / numRows);
	
	SetSize(m_nSprW, m_nSprH);
	SetFrame(0);

	return 0;
}

int DX9Anim::SetBoundingnBox(D3DXVECTOR2 Size)
{
	m_BB.PosOffset.x = -Size.x / 2;
	m_BB.PosOffset.y = -Size.y;

	m_BB.Size.x = (float)m_nSprScaledW + Size.x;
	m_BB.Size.y = (float)m_nSprScaledH + Size.y;

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

	// 이걸 해야 옆줄 침범하지 않음!★
	u1 += 0.01f;
	v1 += 0.01f;

	if (m_Anims[m_nCurrAnimID].HFlip)
	{
		UpdateVertData(u2, v1, u1, v2);
	}
	else
	{
		UpdateVertData(u1, v1, u2, v2);
	}
	
	return 0;
}

int DX9Anim::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip)
{
	m_Anims[AnimID].FrameS = StartFrame;
	m_Anims[AnimID].FrameE = EndFrame;
	m_Anims[AnimID].HFlip = HFlip;

	return 0;
}

int DX9Anim::SetAnimation(int AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
{
	if ((m_nCurrAnimID != AnimID) || (bForcedSet))
	{
		m_nCurrAnimID = AnimID;
		m_nCurrFrame = m_Anims[AnimID].FrameS;
		
		if (m_Anims[AnimID].HFlip)
			m_nAnimDir = DX9ANIMDIR::Right;
		else
			m_nAnimDir = DX9ANIMDIR::Left;

		m_bRepeating = bRepeating;
		m_bBeingAnimated = !bCanInterrupt;
	}
	return 0;
}

int DX9Anim::Animate()
{	
	if (m_nCurrFrame < m_Anims[m_nCurrAnimID].FrameE) {
		m_nCurrFrame++;
	}
	else {
		m_nCurrFrame = m_Anims[m_nCurrAnimID].FrameS;
		if (!m_bRepeating)
			m_bBeingAnimated = false;
	}
	
	SetFrame(m_nCurrFrame);
	return 0;
}

int DX9Anim::DrawBoundingBox()
{
	m_BBLine.SetBoxPosition(m_SprPos + m_BB.PosOffset, m_BB.Size);
	m_BBLine.Draw();
	return 0;
}

DX9BOUNDINGBOX DX9Anim::GetBoundingBox()
{
	DX9BOUNDINGBOX Result;
	Result.PosOffset = m_SprPos + m_BB.PosOffset;
	Result.Size = m_BB.Size;
	return Result;
}

int DX9Anim::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Image::SetPosition(Pos.x, Pos.y);
	m_SprPos = Pos;
	m_SprFeetPos.x = Pos.x + m_nSprScaledW / 2;
	m_SprFeetPos.y = Pos.y + m_nSprScaledH;
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
	m_SprPos += m_Velocity;
	m_SprFeetPos += m_Velocity;

	DX9Image::SetPosition(m_SprPos.x, m_SprPos.y);
	return 0;
}

int DX9Anim::MoveConst(D3DXVECTOR2 dXY)
{
	m_SprPos += dXY;
	m_SprFeetPos += dXY;

	DX9Image::SetPosition(m_SprPos.x, m_SprPos.y);
	return 0;
}