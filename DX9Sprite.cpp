#include "DX9Sprite.h"

DX9Sprite::DX9Sprite() {
	m_nRows = 0;
	m_nCols = 0;

	m_nSprW = 0;
	m_nSprH = 0;
	m_nSprScaledW = 0;
	m_nSprScaledH = 0;

	m_nSprDir = 0;
	m_nAnimCount = 0;
	m_nCurrAnimID = 0;
	m_nCurrFrame = 0;

	m_SprX = 0.0f;
	m_SprY = 0.0f;
	m_SprFeetY = 0.0f;
}

int DX9Sprite::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir) {
	DX9Image::Create(pD3DDev, BaseDir);
	return 0;
}

int DX9Sprite::MakeSprite(std::wstring TextureFN, int numCols, int numRows, float Scale) {
	DX9Image::SetTexture(L"advnt_full.png");
	SetNumRowsAndCols(numCols, numRows);
	DX9Image::SetScale(Scale, Scale);
	m_nSprScaledW = (int)(m_nSprW * Scale);
	m_nSprScaledH = (int)(m_nSprH * Scale);

	SetPosition(0.0f, 0.0f);
	return 0;
}

int DX9Sprite::SetNumRowsAndCols(int numCols, int numRows) {
	m_nCols = numCols;
	m_nRows = numRows;

	m_nSprW = (int)(m_nWidth / numCols);
	m_nSprH = (int)(m_nHeight / numRows);
	
	SetSize(m_nSprW, m_nSprH);
	SetFrame(0);

	return 0;
}

int DX9Sprite::SetFrame(int FrameID) {
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

int DX9Sprite::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip) {
	m_Anims[AnimID].FrameS = StartFrame;
	m_Anims[AnimID].FrameE = EndFrame;
	m_Anims[AnimID].HFlip = HFlip;

	return 0;
}

int DX9Sprite::SetAnimation(int AnimID) {
	if (m_nCurrAnimID != AnimID)
	{
		m_nCurrAnimID = AnimID;
		m_nCurrFrame = m_Anims[AnimID].FrameS;
		m_nSprDir = m_Anims[AnimID].HFlip;
	}
	return 0;
}

int DX9Sprite::Animate() {
	
	if (m_nCurrFrame < m_Anims[m_nCurrAnimID].FrameE) {
		m_nCurrFrame++;
	}
	else {
		m_nCurrFrame = m_Anims[m_nCurrAnimID].FrameS;
	}
	
	SetFrame(m_nCurrFrame);
	return 0;
}

int DX9Sprite::SetPosition(float X, float Y) {
	DX9Image::SetPosition(X, Y);
	m_SprX = X;
	m_SprY = Y;
	m_SprFeetX = X + m_nSprScaledW / 2;
	m_SprFeetY = Y + m_nSprScaledH;
	return 0;
}

int DX9Sprite::SetPositionY(float Y) {
	DX9Image::SetPosition(m_SprX, Y);
	m_SprY = Y;
	m_SprFeetY = Y + m_nSprScaledH;
	return 0;
}

int DX9Sprite::Move(float dX, float dY) {
	m_SprX += dX;
	m_SprY += dY;
	m_SprFeetX += dX;
	m_SprFeetY += dY;

	DX9Image::SetPosition(m_SprX, m_SprY);
	return 0;
}