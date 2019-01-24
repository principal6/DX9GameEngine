#pragma once

#include "../CoreBase/DX9Image.h"

namespace DX9ENGINE
{
	// ***
	// *** Forward declaration ***
	class DX9Window;
	class DX9Map;
	// ***

	enum class EAnimationDirection
	{
		Left,
		Right,
	};

	enum class EAnimationID
	{
		Idle,
		Walk,
		Jumping,
		Falling,
		Landing,
		Attack1,
		Attack2,
		Attack3,

		Effect,
	};

	struct SAnimationData
	{
		EAnimationID AnimID;
		int FrameS, FrameE;

		SAnimationData() : FrameS(0), FrameE(0) {};
		SAnimationData(EAnimationID _AnimID, int StartFrame, int EndFrame) : AnimID(_AnimID), FrameS(StartFrame), FrameE(EndFrame) {};
	};

	class DX9Life : public DX9Image
	{
	public:
		DX9Life();
		virtual ~DX9Life() {};

		virtual auto DX9Life::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError;

		virtual auto DX9Life::MakeLife(WSTRING TextureFN, POINT UnitSize, int numCols, int numRows, float Scale = 1.0f)->DX9Life*;

		virtual auto DX9Life::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Life*;

		virtual auto DX9Life::GetGlobalPosition() const->D3DXVECTOR2;
		virtual auto DX9Life::GetGlobalPositionInverse() const->D3DXVECTOR2;
		virtual auto DX9Life::GetVelocity() const->D3DXVECTOR2;
		virtual auto DX9Life::GetOffsetForMapMove() const->D3DXVECTOR2;

		// Animation
		virtual auto DX9Life::AddAnimation(EAnimationID AnimID, int StartFrame, int EndFrame)->DX9Life*;
		virtual void DX9Life::Animate();
		virtual void DX9Life::SetFrame(int FrameID);
		virtual void DX9Life::SetAnimation(EAnimationID AnimID, bool CanInterrupt = false,
			bool ForcedSet = false, bool Repeating = false);
		virtual void DX9Life::SetDirection(EAnimationDirection Direction);

		virtual auto DX9Life::IsBeingAnimated() const->bool;
		virtual auto DX9Life::GetScaledLifeSize() const->D3DXVECTOR2;
		virtual auto DX9Life::GetDirection() const->EAnimationDirection;

		// Move
		virtual void DX9Life::Accelerate(D3DXVECTOR2 Accel);
		virtual void DX9Life::AddVelocity(D3DXVECTOR2 Vel);
		virtual void DX9Life::SetVelocity(D3DXVECTOR2 Vel);
		virtual void DX9Life::MoveWithVelocity();
		virtual void DX9Life::MoveConst(D3DXVECTOR2 dXY);
		virtual void DX9Life::Walk(EAnimationDirection Direction);
		virtual void DX9Life::Jump();
		virtual void DX9Life::Gravitate();

	protected:
		void DX9Life::CalculateGlobalPositionInverse();
		void DX9Life::CalculateGlobalPosition();

	protected:
		static const D3DXVECTOR2 JUMP_POWER;
		static const D3DXVECTOR2 GRAVITY;
		static const float STRIDE;

		const DX9Map* m_pMap;
		D3DXVECTOR2 m_GlobalPos;
		D3DXVECTOR2 m_GlobalPosInverse;
		D3DXVECTOR2 m_Velocity;
		bool m_bHitGround;

		D3DXVECTOR2 m_UnitSize;

	private:
		virtual void DX9Life::SetNumRowsAndCols(POINT UnitSize, int numCols, int numRows);

	private:
		int m_SheetRows;
		int m_SheetCols;

		EAnimationDirection m_AnimDir;
		EAnimationID m_CurrAnimID;
		int m_CurrFrameID;
		VECTOR<SAnimationData> m_AnimData;
		int m_AnimCount;
		bool m_bBeingAnimated;
		bool m_bRepeating;
	};
};