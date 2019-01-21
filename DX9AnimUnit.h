#pragma once

#include "Core/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Base;
	// ***

	class DX9AnimUnit : public DX9Image
	{
	private:
		int m_NumRows;
		int m_NumCols;

		AnimationDir m_AnimDir;
		AnimationID m_CurrAnimID;
		int m_CurrFrameID;
		VECTOR<AnimationData> m_AnimData;
		int m_AnimCount;
		bool m_bBeingAnimated;
		bool m_bRepeating;

	protected:
		int m_UnitWidth;
		int m_UnitHeight;

	private:
		virtual void DX9AnimUnit::SetNumRowsAndCols(int numCols, int numRows);
		virtual void DX9AnimUnit::SetTexture(WSTRING FileName) override;

	protected:
		virtual void DX9AnimUnit::SetPosition(D3DXVECTOR2 Pos) override;
		virtual void DX9AnimUnit::SetPositionCentered(D3DXVECTOR2 Pos) override;

	public:
		DX9AnimUnit();
		virtual ~DX9AnimUnit() {};

		virtual auto DX9AnimUnit::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f)->DX9AnimUnit*;

		virtual auto DX9AnimUnit::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)->DX9AnimUnit*;
		virtual void DX9AnimUnit::Animate();
		virtual void DX9AnimUnit::SetFrame(int FrameID);
		virtual void DX9AnimUnit::SetAnimation(AnimationID AnimID, bool CanInterrupt = false,
			bool ForcedSet = false, bool Repeating = false);
		virtual void DX9AnimUnit::SetDirection(AnimationDir Direction);

		virtual auto DX9AnimUnit::IsBeingAnimated() const->bool;
		virtual auto DX9AnimUnit::GetScaledUnitWidth() const->int;
		virtual auto DX9AnimUnit::GetScaledUnitHeight() const->int;
		virtual auto DX9AnimUnit::GetDirection() const->AnimationDir;
	};
};