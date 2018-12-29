#pragma once

#ifndef _DX9ANIM_H_
#define _DX9ANIM_H_

#include "DX9Image.h"

constexpr auto MAX_ANIM_COUNT = 20;

class DX9Anim : public DX9Image
{
private:
	int m_nRows;
	int m_nCols;

	// Animation
	DX9ANIMDIR m_nAnimDir;
	DX9ANIMID m_nCurrAnimID;
	int m_nCurrFrameID;
	DX9ANIMDATA m_Anims[MAX_ANIM_COUNT];
	int m_nAnimCount;
	bool m_bBeingAnimated;
	bool m_bRepeating;

protected:
	int m_UnitW;
	int m_UnitH;
	int m_WindowW;
	int m_WindowH;
	float m_WindowHalfW;
	float m_WindowHalfH;
	D3DXVECTOR2 m_GlobalPos;

private:
	int DX9Anim::SetNumRowsAndCols(int numCols, int numRows);

protected:
	int DX9Anim::SetPosition(D3DXVECTOR2 Pos);
	int DX9Anim::SetPositionCentered(D3DXVECTOR2 Pos);

public:
	DX9Anim();
	~DX9Anim() {};

	int DX9Anim::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir, int WindowWidth, int WindowHeight);
	int DX9Anim::MakeUnit(std::wstring TextureFN, int numCols, int numRows, float Scale = 1.0f);

	int DX9Anim::SetFrame(int FrameID);
	int DX9Anim::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	int DX9Anim::SetAnimation(DX9ANIMID AnimID, bool CanInterrupt = false, bool ForcedSet = false, bool Repeating = false);
	int DX9Anim::Animate();
	bool DX9Anim::IsBeingAnimated() { return m_bBeingAnimated; };
	
	int DX9Anim::SetAnimDir(DX9ANIMDIR Direction);

	DX9ANIMDIR DX9Anim::GetAnimDir() { return m_nAnimDir; };
	int DX9Anim::GetScaledSprWidth() { return m_ScaledW; };
	int DX9Anim::GetScaledSprHeight() { return m_ScaledH; };
	D3DXVECTOR2 DX9Anim::GetCenterPosition() { return DX9Image::GetCenterPosition(); };
};

#endif