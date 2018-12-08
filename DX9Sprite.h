#pragma once

#ifndef DX9SPRITE_H
#define DX9SPRITE_H

#include "DX9Image.h"

class DX9Sprite : public DX9Image
{
private:
	int		mnRows = 0;
	int		mnCols = 0;
	float	mSheetWidth = 0.0f;
	float	mSheetHeight = 0.0f;

public:
	DX9Sprite();
	int DX9Sprite::SetNumRowsAndCols(int numRows, int numCols);
	int DX9Sprite::SetFrame(int FrameID);
};

#endif