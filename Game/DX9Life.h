#pragma once

#include "../Core/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Base;
	class DX9Map;
	// ***

	class DX9Life : public DX9Image
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
		static const D3DXVECTOR2 JUMP_POWER;
		static const D3DXVECTOR2 GRAVITY;
		static const float STRIDE;

		const DX9Map* m_pMap;
		D3DXVECTOR2 m_GlobalPos;
		D3DXVECTOR2 m_GlobalPosInverse;
		D3DXVECTOR2 m_Velocity;
		bool m_bHitGround;

		int m_UnitWidth;
		int m_UnitHeight;

	private:
		virtual void DX9Life::SetNumRowsAndCols(int numCols, int numRows);

	protected:
		void DX9Life::CalculateGlobalPositionInverse();
		void DX9Life::CalculateGlobalPosition();

	public:
		DX9Life();
		virtual ~DX9Life() {};

		virtual auto DX9Life::Create(DX9Base* pBase, WSTRING BaseDir, DX9Map* pMap)->Error;

		virtual auto DX9Life::MakeLife(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f)->DX9Life*;

		virtual auto DX9Life::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Life*;

		virtual auto DX9Life::GetGlobalPosition() const->D3DXVECTOR2;
		virtual auto DX9Life::GetGlobalPositionInverse() const->D3DXVECTOR2;
		virtual auto DX9Life::GetVelocity() const->D3DXVECTOR2;
		virtual auto DX9Life::GetOffsetForMapMove() const->D3DXVECTOR2;

		// Animation
		virtual auto DX9Life::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)->DX9Life*;
		virtual void DX9Life::Animate();
		virtual void DX9Life::SetFrame(int FrameID);
		virtual void DX9Life::SetAnimation(AnimationID AnimID, bool CanInterrupt = false,
			bool ForcedSet = false, bool Repeating = false);
		virtual void DX9Life::SetDirection(AnimationDir Direction);

		virtual auto DX9Life::IsBeingAnimated() const->bool;
		virtual auto DX9Life::GetScaledUnitWidth() const->int;
		virtual auto DX9Life::GetScaledUnitHeight() const->int;
		virtual auto DX9Life::GetDirection() const->AnimationDir;

		// Move
		virtual void DX9Life::Accelerate(D3DXVECTOR2 Accel);
		virtual void DX9Life::AddVelocity(D3DXVECTOR2 Vel);
		virtual void DX9Life::SetVelocity(D3DXVECTOR2 Vel);
		virtual void DX9Life::MoveWithVelocity();
		virtual void DX9Life::MoveConst(D3DXVECTOR2 dXY);
		virtual void DX9Life::Walk(AnimationDir Direction);
		virtual void DX9Life::Jump();
		virtual void DX9Life::Gravitate();
	};
};