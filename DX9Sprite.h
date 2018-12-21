#pragma once

#ifndef DX9SPRITE_H
#define DX9SPRITE_H

#include "DX9Image.h"

#define DX9MAXSPRANIMNUM 20

struct DX9SpriteAnim {
	int FrameS;
	int FrameE;
	bool HFlip;

	DX9SpriteAnim() { FrameS = 0; FrameE = 0; HFlip = false; };
};

class DX9Sprite : public DX9Image {
private:
	int	m_nRows;
	int	m_nCols;
	int	m_nSheetWidth;
	int	m_nSheetHeight;

	int				m_nSprDir;
	int				m_nCurrAnimID;
	int				m_nCurrFrame;
	DX9SpriteAnim	m_Anims[DX9MAXSPRANIMNUM];
	int				m_nAnimCount;

public:
	DX9Sprite();
	~DX9Sprite() {};

	int DX9Sprite::SetNumRowsAndCols(int numRows, int numCols);
	int DX9Sprite::SetFrame(int FrameID);
	int DX9Sprite::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Sprite::SetAnimation(int AnimID);
	int DX9Sprite::Animate();
	int DX9Sprite::GetSpriteDir() { return m_nSprDir; };
};

#endif