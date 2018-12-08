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
	int		mnRows;
	int		mnCols;
	float	mfSheetWidth;
	float	mfSheetHeight;

	int				mSprDir;
	int				mCurrAnimID;
	int				mCurrFrame;
	DX9SpriteAnim	mAnims[DX9MAXSPRANIMNUM];
	int				mNumAnims;

public:
	DX9Sprite();
	~DX9Sprite() {};

	int DX9Sprite::SetNumRowsAndCols(int numRows, int numCols);
	int DX9Sprite::SetFrame(int FrameID);
	int DX9Sprite::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Sprite::SetAnimation(int AnimID);
	int DX9Sprite::Animate();
	int DX9Sprite::GetSpriteDir() { return mSprDir; };
};

#endif