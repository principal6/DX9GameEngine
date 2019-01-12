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

		virtual ReturnValue DX9Life::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData) override;
		void DX9Life::Destroy() override;
		virtual DX9Life* DX9Life::SetGlobalPosition(D3DXVECTOR2 Position) = 0;

		D3DXVECTOR2 DX9Life::GetGlobalPosition() const;
		D3DXVECTOR2 DX9Life::GetGlobalPositionInverse() const;
		D3DXVECTOR2 DX9Life::GetVelocity() const;
		D3DXVECTOR2 DX9Life::GetOffsetForMapMove() const;

		void DX9Life::Accelerate(D3DXVECTOR2 Accel);
		void DX9Life::AddVelocity(D3DXVECTOR2 Vel);
		void DX9Life::SetVelocity(D3DXVECTOR2 Vel);
		void DX9Life::MoveWithVelocity();
		void DX9Life::MoveConst(D3DXVECTOR2 dXY);

		void DX9Life::Walk(AnimationDir Direction);
		void DX9Life::Jump();
		void DX9Life::Gravitate();
	};
};