#pragma once

#include "DX9AnimUnit.h"
#include "DX9Map.h"

namespace DX9ENGINE
{
	class DX9Life : public DX9AnimUnit
	{
	protected:
		static const D3DXVECTOR2 JUMP_POWER;
		static const D3DXVECTOR2 GRAVITY;
		static const float STRIDE;

		const DX9Map* m_pMap;
		D3DXVECTOR2 m_GlobalPos;
		D3DXVECTOR2 m_GlobalPosInverse;
		D3DXVECTOR2 m_Velocity;
		bool m_bHitGround;

	protected:
		void DX9Life::SetMapPointer(DX9Map* pMap);
		void DX9Life::CalculateGlobalPositionInverse();
		void DX9Life::CalculateGlobalPosition();

	public:
		DX9Life();
		virtual ~DX9Life() {};

		virtual auto DX9Life::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData)->Error override;
		virtual void DX9Life::Destroy() override;

		virtual auto DX9Life::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Life* = 0;

		virtual auto DX9Life::GetGlobalPosition() const->D3DXVECTOR2;
		virtual auto DX9Life::GetGlobalPositionInverse() const->D3DXVECTOR2;
		virtual auto DX9Life::GetVelocity() const->D3DXVECTOR2;
		virtual auto DX9Life::GetOffsetForMapMove() const->D3DXVECTOR2;

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