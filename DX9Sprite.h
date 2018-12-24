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

class DX9Sprite : protected DX9Image {
private:
	int	m_nRows;
	int	m_nCols;
	int m_nSprW;
	int m_nSprH;
	int m_nSprScaledW;
	int m_nSprScaledH;

	int				m_nSprDir;
	int				m_nCurrAnimID;
	int				m_nCurrFrame;
	DX9SpriteAnim	m_Anims[DX9MAXSPRANIMNUM];
	int				m_nAnimCount;

	float m_SprX;
	float m_SprY;
	float m_SprFeetX;
	float m_SprFeetY;

public:
	DX9Sprite();
	~DX9Sprite() {};

	int DX9Sprite::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Sprite::Destroy() { return DX9Image::Destroy(); };
	
	int DX9Sprite::MakeSprite(std::wstring TextureFN, int numCols, int numRows, float Scale);
	int DX9Sprite::SetNumRowsAndCols(int numCols, int numRows);
	
	int DX9Sprite::SetFrame(int FrameID);
	int DX9Sprite::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Sprite::SetAnimation(int AnimID);
	int DX9Sprite::Animate();

	int DX9Sprite::Draw() { return DX9Image::Draw(); };

	int DX9Sprite::SetPosition(float X, float Y);
	int DX9Sprite::SetPositionY(float Y);
	int DX9Sprite::Move(float dX, float dY);

	int DX9Sprite::GetSpriteDir() { return m_nSprDir; };
	int DX9Sprite::GetSpriteH() { return (int)(m_fScaleY * m_nSprH); };
	float DX9Sprite::GetSpriteFeetX() { return m_SprFeetX; };
	float DX9Sprite::GetSpriteFeetY() { return m_SprFeetY; };
};

#endif