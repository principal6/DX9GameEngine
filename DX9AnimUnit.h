#pragma once

#include "Core\\DX9Image.h"

class DX9AnimUnit : protected DX9Image
{
private:
	int m_NumRows;
	int m_NumCols;

	AnimationDir m_AnimDir;
	AnimationID m_CurrAnimID;
	int m_CurrFrameID;
	std::vector<AnimationData> m_AnimData;
	int m_AnimCount;
	bool m_bBeingAnimated;
	bool m_bRepeating;

protected:
	int m_UnitWidth;
	int m_UnitHeight;

private:
	void DX9AnimUnit::SetNumRowsAndCols(int numCols, int numRows);
	void DX9AnimUnit::SetTexture(WSTRING FileName) override;

protected:
	void DX9AnimUnit::SetPosition(D3DXVECTOR2 Pos) override;
	void DX9AnimUnit::SetPositionCentered(D3DXVECTOR2 Pos) override;

public:
	DX9AnimUnit();
	virtual ~DX9AnimUnit() {};

	virtual ReturnValue DX9AnimUnit::Create(LPDIRECT3DDEVICE9 pDevice) override;
	virtual void DX9AnimUnit::Destroy() override;
	virtual DX9AnimUnit* DX9AnimUnit::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f);

	void DX9AnimUnit::SetScale(D3DXVECTOR2 Scale) override;
	void DX9AnimUnit::SetAlpha(int Alpha) override;

	virtual DX9AnimUnit* DX9AnimUnit::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame);
	void DX9AnimUnit::Animate();
	void DX9AnimUnit::SetFrame(int FrameID);
	void DX9AnimUnit::SetAnimation(AnimationID AnimID, bool CanInterrupt = false, bool ForcedSet = false, bool Repeating = false);
	void DX9AnimUnit::SetDirection(AnimationDir Direction);
	virtual DX9AnimUnit* DX9AnimUnit::SetBoundingBox(D3DXVECTOR2 Size) override;

	bool DX9AnimUnit::IsBeingAnimated() const;
	int DX9AnimUnit::GetScaledUnitWidth() const;
	int DX9AnimUnit::GetScaledUnitHeight() const;
	DX9Common::AnimationDir DX9AnimUnit::GetDirection() const;
	D3DXVECTOR2 DX9AnimUnit::GetCenterPosition() const override;
	DX9Image::BoundingBox DX9AnimUnit::GetBoundingBox() const override;

	virtual void DX9AnimUnit::Draw() override;
	virtual void DX9AnimUnit::DrawBoundingBox() override;
};