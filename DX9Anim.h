#pragma once

#include "DX9Image.h"

constexpr auto MAX_ANIM_COUNT = 30;

class DX9Anim : public DX9Image
{
private:
	int m_nRows;
	int m_nCols;

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
	D3DXVECTOR2 m_GlobalPos;
	D3DXVECTOR2 m_GlobalPosInverse;

private:
	void DX9Anim::SetNumRowsAndCols(int numCols, int numRows);

protected:
	void DX9Anim::SetPosition(D3DXVECTOR2 Pos);
	void DX9Anim::SetPositionCentered(D3DXVECTOR2 Pos);
	void DX9Anim::CalculateGlobalPositionInverse();
	void DX9Anim::CalculateGlobalPosition();

public:
	DX9Anim();
	virtual ~DX9Anim() {};

	virtual void DX9Anim::Create() override;
	void DX9Anim::MakeUnit(std::wstring TextureFN, int numCols, int numRows, float Scale = 1.0f);

	void DX9Anim::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip = false);	
	void DX9Anim::Animate();

	void DX9Anim::SetFrame(int FrameID);
	void DX9Anim::SetAnimation(DX9ANIMID AnimID, bool CanInterrupt = false, bool ForcedSet = false, bool Repeating = false);
	void DX9Anim::SetDirection(DX9ANIMDIR Direction);

	bool DX9Anim::IsBeingAnimated() const;
	int DX9Anim::GetScaledUnitWidth() const;
	int DX9Anim::GetScaledUnitHeight() const;
	DX9ANIMDIR DX9Anim::GetDirection() const;
	D3DXVECTOR2 DX9Anim::GetCenterPosition() const;
};