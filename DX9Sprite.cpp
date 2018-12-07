#include "DX9Sprite.h"

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

	UpdateVertData(u1, v1, u2, v2);

	return 0;
}