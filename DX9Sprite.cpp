#include "DX9Sprite.h"

DX9Sprite::DX9Sprite() {
	m_nRows = 0;
	m_nCols = 0;
	m_nSheetWidth = 0;
	m_nSheetHeight = 0;

	m_nSprDir = 0;
	m_nAnimCount = 0;
	m_nCurrAnimID = 0;
	m_nCurrFrame = 0;
}

int DX9Sprite::SetNumRowsAndCols(int numRows, int numCols) {
	m_nRows = numRows;
	m_nCols = numCols;

	m_nSheetWidth = m_nWidth * m_nCols;
	m_nSheetHeight = m_nHeight * m_nRows;

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