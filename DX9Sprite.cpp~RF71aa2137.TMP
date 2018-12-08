#include "DX9Sprite.h"

DX9Sprite::DX9Sprite() {
	mnRows = 0;
	mnCols = 0;
	mSheetWidth = 0.0f;
	mSheetHeight = 0.0f;
	mNumAnims = 0;
	mCurrAnimID = 0;
	mCurrFrame = 0;
}

int DX9Sprite::SetNumRowsAndCols(int numRows, int numCols)
{
	mnRows = numRows;
	mnCols = numCols;

	mSheetWidth = mWidth * mnCols;
	mSheetHeight = mHeight * mnRows;

	SetFrame(0);

	return 0;
}

int DX9Sprite::SetFrame(int FrameID)
{
	if (mnRows == 0)
		return -1;
	if (mnCols == 0)
		return -1;
	
	int FrameX = (FrameID % mnCols);
	int FrameY = (FrameID / mnCols);

	float u1 = ((float)FrameX / (float)mnCols);
	float u2 = ((float)(FrameX + 1) / (float)mnCols);
	float v1 = ((float)FrameY / (float)mnRows);
	float v2 = ((float)(FrameY + 1) / (float)mnRows);

	if (mAnims[mCurrAnimID].HFlip)
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
	mAnims[AnimID].FrameS = StartFrame;
	mAnims[AnimID].FrameE = EndFrame;
	mAnims[AnimID].HFlip = HFlip;

	return 0;
}

int DX9Sprite::SetAnimation(int AnimID) {
	if (mCurrAnimID != AnimID)
	{
		mCurrAnimID = AnimID;
		mCurrFrame = mAnims[AnimID].FrameS;
	}
	return 0;
}

int DX9Sprite::Animate() {
	
	if (mCurrFrame < mAnims[mCurrAnimID].FrameE) {
		mCurrFrame++;
	}
	else {
		mCurrFrame = mAnims[mCurrAnimID].FrameS;
	}
	
	SetFrame(mCurrFrame);
	return 0;
}