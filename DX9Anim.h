#pragma once

#ifndef _DX9ANIM_H_
#define _DX9ANIM_H_

#include "DX9Image.h"
#include "DX9Line.h"

constexpr auto MAX_ANIM_COUNT = 20;

class DX9Anim : protected DX9Image
{
private:
	int m_nRows;
	int m_nCols;

	int m_nSprW;
	int m_nSprH;
	int m_nSprScaledW;
	int m_nSprScaledH;

	// Animation
	DX9ANIMDIR m_nAnimDir;
	DX9ANIMID m_nCurrAnimID;
	int m_nCurrFrameID;
	DX9ANIMDATA m_Anims[MAX_ANIM_COUNT];
	int m_nAnimCount;
	bool m_bBeingAnimated;
	bool m_bRepeating;

	// Collision
	DX9BOUNDINGBOX m_BB;
	DX9Line m_BBLine;

	// Position
	D3DXVECTOR2 m_SprPos;
	D3DXVECTOR2 m_SprFeetPos;
	D3DXVECTOR2 m_Velocity;

private:
	int DX9Anim::SetNumRowsAndCols(int numCols, int numRows);

public:
	DX9Anim();
	~DX9Anim() {};

	int DX9Anim::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir);
	int DX9Anim::MakeSprite(std::wstring TextureFN, int numCols, int numRows, float Scale = 1.0f);
	int DX9Anim::Destroy();

	// Collision
	int DX9Anim::SetBoundingnBox(D3DXVECTOR2 Size);
	DX9BOUNDINGBOX DX9Anim::GetBoundingBox();

	// Animator
	int DX9Anim::SetFrame(int FrameID);
	int DX9Anim::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Anim::SetAnimation(DX9ANIMID AnimID, bool CanInterrupt = false, bool ForcedSet = false, bool Repeating = false);
	int DX9Anim::Animate();
	bool DX9Anim::IsBeingAnimated() { return m_bBeingAnimated; };
	int DX9Anim::SetAnimDir(DX9ANIMDIR Direction);
	DX9ANIMDIR DX9Anim::GetAnimDir() { return m_nAnimDir; };

	// Graphics
	int DX9Anim::Draw() { return DX9Image::Draw(); };
	int DX9Anim::DrawBoundingBox();

	// Position
	int DX9Anim::SetPosition(D3DXVECTOR2 Pos);
	int DX9Anim::SetPositionCentered(D3DXVECTOR2 Pos);
	D3DXVECTOR2 DX9Anim::GetPosition() { return m_SprPos; };
	D3DXVECTOR2 DX9Anim::GetCenterPosition();
	D3DXVECTOR2 DX9Anim::GetSpriteFeetPos() { return m_SprFeetPos; };
	int DX9Anim::GetSpriteScaledH() { return m_nSprScaledH; };
	int DX9Anim::MoveWithVelocity();
	int DX9Anim::MoveConst(D3DXVECTOR2 dXY);
	int DX9Anim::Accelerate(D3DXVECTOR2 Accel);
	int DX9Anim::AddVelocity(D3DXVECTOR2 Vel);
	int DX9Anim::SetVelocity(D3DXVECTOR2 Vel);
	D3DXVECTOR2 DX9Anim::GetVelocity() { return m_Velocity; };
};

#endif