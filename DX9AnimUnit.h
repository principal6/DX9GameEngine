#pragma once

#include "DX9Image.h"

constexpr auto MAX_ANIM_COUNT = 30;

class DX9AnimUnit : protected DX9Image
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

private:
	void DX9AnimUnit::SetNumRowsAndCols(int numCols, int numRows);
	void DX9AnimUnit::SetTexture(WSTRING FileName) override;

protected:
	void DX9AnimUnit::SetPosition(D3DXVECTOR2 Pos) override;
	void DX9AnimUnit::SetPositionCentered(D3DXVECTOR2 Pos) override;

public:
	DX9AnimUnit();
	virtual ~DX9AnimUnit() {};

	virtual void DX9AnimUnit::Create(LPDIRECT3DDEVICE9 pDevice, DX9SHARE_DATA* pData) override;
	virtual void DX9AnimUnit::Destroy() override;
	virtual DX9AnimUnit* DX9AnimUnit::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f);

	void DX9AnimUnit::SetScale(D3DXVECTOR2 Scale) override;
	void DX9AnimUnit::SetAlpha(int Alpha) override;

	virtual DX9AnimUnit* DX9AnimUnit::AddAnimation(DX9ANIMID AnimID, int StartFrame, int EndFrame, bool HFlip = false);
	void DX9AnimUnit::Animate();
	void DX9AnimUnit::SetFrame(int FrameID);
	void DX9AnimUnit::SetAnimation(DX9ANIMID AnimID, bool CanInterrupt = false, bool ForcedSet = false, bool Repeating = false);
	void DX9AnimUnit::SetDirection(DX9ANIMDIR Direction);
	virtual DX9AnimUnit* DX9AnimUnit::SetBoundingnBox(D3DXVECTOR2 Size) override;

	bool DX9AnimUnit::IsBeingAnimated() const;
	int DX9AnimUnit::GetScaledUnitWidth() const;
	int DX9AnimUnit::GetScaledUnitHeight() const;
	DX9ANIMDIR DX9AnimUnit::GetDirection() const;
	D3DXVECTOR2 DX9AnimUnit::GetCenterPosition() const override;
	DX9BOUNDINGBOX DX9AnimUnit::GetBoundingBox() const override;

	virtual void DX9AnimUnit::Draw() const override;
	virtual void DX9AnimUnit::DrawBoundingBox() override;
};