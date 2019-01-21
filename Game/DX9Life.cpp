#include "../Core/DX9Window.h"
#include "DX9Life.h"
#include "DX9Map.h"

using namespace DX9ENGINE;

// Static member variables declaration
const D3DXVECTOR2 DX9Life::JUMP_POWER = D3DXVECTOR2(0.0f, -14.0f);
const D3DXVECTOR2 DX9Life::GRAVITY = D3DXVECTOR2(0.0f, 0.5f);
const float DX9Life::STRIDE = 5.0f;

DX9Life::DX9Life()
{
	m_NumRows = 0;
	m_NumCols = 0;

	m_AnimDir = EAnimationDirection::Right;
	m_CurrAnimID = EAnimationID::Idle;
	m_CurrFrameID = 0;
	m_AnimCount = 0;
	m_bBeingAnimated = false;
	m_bRepeating = false;

	m_LifeWidth = 0;
	m_LifeHeight = 0;

	m_GlobalPos = D3DXVECTOR2(0.0f, 0.0f);
	m_GlobalPosInverse = D3DXVECTOR2(0.0f, 0.0f);
	m_Velocity = D3DXVECTOR2(0.0f, 0.0f);
	m_bHitGround = true;
}

auto DX9Life::Create(DX9Window* pDX9Window, WSTRING BaseDir, DX9Map* pMap)->EError
{
	if (DX_SUCCEEDED(DX9Image::Create(pDX9Window, BaseDir)))
	{
		if (nullptr == (m_pMap = pMap))
			return EError::NULLPTR_MAP;

		SetGlobalPosition(m_GlobalPos);

		return EError::OK;
	}

	return EError::IMAGE_NOT_CREATED;
}

auto DX9Life::MakeLife(WSTRING TextureFN, int numCols, int numRows, float Scale)->DX9Life*
{
	DX9Image::SetTexture(TextureFN);
	DX9Image::SetScale(D3DXVECTOR2(Scale, Scale));
	SetNumRowsAndCols(numCols, numRows); // m_UnitWidth & m_UnitHeight are set here
	m_ScaledWidth = static_cast<int>(m_LifeWidth * Scale);
	m_ScaledHeight = static_cast<int>(m_LifeHeight * Scale);

	SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	SetBoundingBox(D3DXVECTOR2(0.0f, 0.0f));

	return this;
}

void DX9Life::SetNumRowsAndCols(int numCols, int numRows)
{
	m_NumCols = numCols;
	m_NumRows = numRows;

	m_LifeWidth = static_cast<int>(m_Width / numCols);
	m_LifeHeight = static_cast<int>(m_Height / numRows);

	SetSize(m_LifeWidth, m_LifeHeight);
	SetFrame(0);
}

void DX9Life::SetFrame(int FrameID)
{
	if ((m_NumRows == 0) || (m_NumCols == 0))
		return;

	STextureUV tUV;
	ConvertFrameIDIntoUV(FrameID, m_NumCols, m_NumRows, &tUV);

	switch (m_AnimDir)
	{
	case EAnimationDirection::Left:
		UpdateVertexData(tUV.u2, tUV.v1, tUV.u1, tUV.v2);
		break;
	case EAnimationDirection::Right:
		UpdateVertexData(tUV.u1, tUV.v1, tUV.u2, tUV.v2);
		break;
	default:
		break;
	}
}

auto DX9Life::AddAnimation(EAnimationID AnimID, int StartFrame, int EndFrame)->DX9Life*
{
	m_AnimData.push_back(SAnimationData(AnimID, StartFrame, EndFrame));

	return this;
}

void DX9Life::SetAnimation(EAnimationID AnimID, bool bCanInterrupt, bool bForcedSet, bool bRepeating)
{
	int AnimIDInt = static_cast<int>(AnimID);

	if (AnimIDInt > m_AnimData.size())
		return;

	if ((m_CurrAnimID != AnimID) || (bForcedSet))
	{
		m_CurrAnimID = AnimID;
		m_CurrFrameID = m_AnimData[AnimIDInt].FrameS;
		m_bRepeating = bRepeating;
		m_bBeingAnimated = !bCanInterrupt;

		SetFrame(m_CurrFrameID);
	}
}

void DX9Life::Animate()
{
	if (m_CurrFrameID < m_AnimData[static_cast<int>(m_CurrAnimID)].FrameE)
	{
		m_CurrFrameID++;
	}
	else
	{
		m_CurrFrameID = m_AnimData[static_cast<int>(m_CurrAnimID)].FrameS;
		if (!m_bRepeating)
			m_bBeingAnimated = false;
	}

	SetFrame(m_CurrFrameID);
}

void DX9Life::SetDirection(EAnimationDirection Direction)
{
	m_AnimDir = Direction;
}

auto DX9Life::IsBeingAnimated() const->bool
{
	return m_bBeingAnimated;
}

auto DX9Life::GetScaledUnitWidth() const->int
{
	return m_ScaledWidth;
}

auto DX9Life::GetScaledUnitHeight() const->int
{
	return m_ScaledHeight;
}

auto DX9Life::GetDirection() const->EAnimationDirection
{
	return m_AnimDir;
}

void DX9Life::CalculateGlobalPositionInverse()
{
	m_GlobalPosInverse = m_GlobalPos;
	m_GlobalPosInverse.y = m_pDX9Window->GetWindowData()->WindowHeight - m_ScaledHeight - m_GlobalPos.y;
}

void DX9Life::CalculateGlobalPosition()
{
	m_GlobalPos = m_GlobalPosInverse;
	m_GlobalPos.y = m_pDX9Window->GetWindowData()->WindowHeight - m_ScaledHeight - m_GlobalPosInverse.y;
}

auto DX9Life::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Life*
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x > m_pDX9Window->GetWindowData()->WindowHalfWidth)
		m_GlobalPosInverse.x = m_pDX9Window->GetWindowData()->WindowHalfWidth;

	if (m_GlobalPosInverse.y < m_pDX9Window->GetWindowData()->WindowHalfHeight)
		m_GlobalPosInverse.y = m_pDX9Window->GetWindowData()->WindowHalfHeight;

	SetPosition(m_GlobalPosInverse);

	return this;
}

auto DX9Life::GetGlobalPosition() const->D3DXVECTOR2
{
	return m_GlobalPos;
}

auto DX9Life::GetGlobalPositionInverse() const->D3DXVECTOR2
{
	return m_GlobalPosInverse;
}

auto DX9Life::GetVelocity() const->D3DXVECTOR2
{
	return m_Velocity;
}

auto DX9Life::GetOffsetForMapMove() const->D3DXVECTOR2
{
	D3DXVECTOR2 Result;
	Result.x = m_GlobalPos.x - m_pDX9Window->GetWindowData()->WindowHalfWidth;
	Result.y = m_GlobalPosInverse.y - m_pDX9Window->GetWindowData()->WindowHalfHeight;

	if (Result.x < 0)
		Result.x = 0;

	if (Result.y > 0)
		Result.y = 0;

	return Result;
}

void DX9Life::Accelerate(D3DXVECTOR2 Accel)
{
	m_Velocity += Accel;
}

void DX9Life::AddVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity += Vel;
}

void DX9Life::SetVelocity(D3DXVECTOR2 Vel)
{
	m_Velocity = Vel;
}

void DX9Life::MoveWithVelocity()
{
	m_GlobalPos.x += m_Velocity.x;
	m_GlobalPos.y -= m_Velocity.y; // @warning: Y value of GlobalPos is in the opposite direction!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_pDX9Window->GetWindowData()->WindowHalfWidth)
	{
		m_Position.x = m_GlobalPos.x;
	}
	if (m_GlobalPosInverse.y > m_pDX9Window->GetWindowData()->WindowHalfHeight)
	{
		m_Position.y = m_GlobalPosInverse.y;
	}

	SetPosition(m_Position);
}

void DX9Life::MoveConst(D3DXVECTOR2 dXY)
{
	m_GlobalPos.x += dXY.x;
	m_GlobalPos.y -= dXY.y; // @warning: Y value of GlobalPos is in the opposite direction!

	CalculateGlobalPositionInverse();

	if (m_GlobalPosInverse.x < m_pDX9Window->GetWindowData()->WindowHalfWidth)
	{
		m_Position.x = m_GlobalPos.x;
	}
	else
	{
		if (m_Position.x != m_pDX9Window->GetWindowData()->WindowHalfWidth)
			m_Position.x = m_pDX9Window->GetWindowData()->WindowHalfWidth;
	}

	if (m_GlobalPosInverse.y > m_pDX9Window->GetWindowData()->WindowHalfHeight)
	{
		m_Position.y = m_GlobalPosInverse.y;
	}
	else
	{
		if (m_Position.y != m_pDX9Window->GetWindowData()->WindowHalfHeight)
			m_Position.y = m_pDX9Window->GetWindowData()->WindowHalfHeight;
	}

	SetPosition(m_Position);
}

void DX9Life::Walk(EAnimationDirection Direction)
{
	D3DXVECTOR2 Velocity;
	switch (Direction)
	{
	case EAnimationDirection::Left:
		Velocity = D3DXVECTOR2(-STRIDE, 0);
		break;
	case EAnimationDirection::Right:
		Velocity = D3DXVECTOR2(STRIDE, 0);
		break;
	default:
		break;
	}

	SetAnimation(EAnimationID::Walk, true);
	SetDirection(Direction);

	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), Velocity);

	MoveConst(tNewVel);

}
void DX9Life::Jump()
{
	if ((m_bHitGround == false) || (m_Velocity.y > 0)) // Currently the sprite is falling down
		return;

	m_bHitGround = false;

	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), JUMP_POWER);
	SetVelocity(tNewVel);
}

void DX9Life::Gravitate()
{
	Accelerate(GRAVITY);
	D3DXVECTOR2 tNewVel = m_pMap->GetVelocityAfterCollision(GetBoundingBox(), m_Velocity);

	if (tNewVel.y < m_Velocity.y)
	{
		if (m_bHitGround)
		{
			tNewVel.y = 0;
		}
		else
		{
			m_bHitGround = true;
			//SetAnimation(AnimationID::Landing, true, true); // This is only for main Sprite
			SetAnimation(EAnimationID::Idle, true, true);
		}
	}
	else
	{
		if (m_Velocity.y < 0)
			SetAnimation(EAnimationID::Jumping, true, true);

		if (m_Velocity.y > 0)
			SetAnimation(EAnimationID::Falling, true, true);

		m_bHitGround = false;
	}

	SetVelocity(tNewVel);

	MoveWithVelocity();
}