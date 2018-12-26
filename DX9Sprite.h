#pragma once

#ifndef DX9SPRITE_H
#define DX9SPRITE_H

#include "DX9Image.h"
#include "DX9Line.h"

#define MAX_SPR_ANIM_COUNT 20

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

	// Animation
	int				m_nAnimSprDir;
	int				m_nCurrAnimID;
	int				m_nCurrFrame;
	DX9SpriteAnim	m_Anims[MAX_SPR_ANIM_COUNT];
	int				m_nAnimCount;

	// Collision
	DX9BOUNDINGBOX	m_BB;
	DX9Line			m_BBLine;
	D3DXVECTOR2		m_SprPos;
	D3DXVECTOR2		m_SprFeetPos;
	D3DXVECTOR2		m_Velocity;

public:
	DX9Sprite();
	~DX9Sprite() {};

	int DX9Sprite::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Sprite::Destroy();
	
	int DX9Sprite::MakeSprite(std::wstring TextureFN, int numCols, int numRows, float Scale);
	int DX9Sprite::SetNumRowsAndCols(int numCols, int numRows);
	int DX9Sprite::SetBoundingnBox(D3DXVECTOR2 Size);
	
	// Animator
	int DX9Sprite::SetFrame(int FrameID);
	int DX9Sprite::AddAnimation(int AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Sprite::SetAnimation(int AnimID);
	int DX9Sprite::Animate();

	// Graphics
	int DX9Sprite::Draw() { return DX9Image::Draw(); };
	int DX9Sprite::DrawBoundingBox();

	// Position
	int DX9Sprite::SetPosition(D3DXVECTOR2 Pos);
	int DX9Sprite::MoveWithVelocity();
	int DX9Sprite::MoveOnce(D3DXVECTOR2 dXY);
	int DX9Sprite::Accelerate(D3DXVECTOR2 Accel);
	int DX9Sprite::AddVelocity(D3DXVECTOR2 Vel);
	int DX9Sprite::SetVelocity(D3DXVECTOR2 Vel);
	int DX9Sprite::SetVelocityY(float Vel);
	D3DXVECTOR2 DX9Sprite::GetVelocity();

	// Getter
	int DX9Sprite::GetSpriteDir() { return m_nAnimSprDir; };
	int DX9Sprite::GetSpriteH() { return (int)(m_fScaleY * m_nSprH); };
	D3DXVECTOR2 DX9Sprite::GetSpriteFeet() { return m_SprFeetPos; };
	DX9BOUNDINGBOX DX9Sprite::GetBoundingBox();
};

#endif