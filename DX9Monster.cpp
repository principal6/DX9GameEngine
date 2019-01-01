#include "DX9Monster.h"

DX9Monster::DX9Monster() {
	m_HPMax = 0;
	m_HPCurr = 0;
	m_bUILoaded = false;
	m_HPFrame = nullptr;
	m_HPBar = nullptr;
}

void DX9Monster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DX9Life::Create(pDevice);
	
	m_HPFrame = new DX9Image;
	m_HPFrame->Create(pDevice);
	m_HPFrame->SetTexture(L"hpbarbg.png");

	m_HPBar = new DX9Image;
	m_HPBar->Create(pDevice);
	m_HPBar->SetTexture(L"hpbar.png");

	m_bUILoaded = true;
}

void DX9Monster::Destroy()
{
	if (m_HPFrame)
	{
		m_HPFrame->Destroy();
		delete m_HPFrame;
	}

	if (m_HPBar)
	{
		m_HPBar->Destroy();
		delete m_HPBar;
	}

	DX9Life::Destroy();
}

void DX9Monster::SetMaxHP(int HPMax)
{
	m_HPMax = HPMax;
	m_HPCurr = HPMax;
}

void DX9Monster::SetUIPosition(D3DXVECTOR2 Position)
{
	D3DXVECTOR2 tPos = GetCenterPosition();
	tPos.y = Position.y;
	tPos.y -= OFFSET_Y_HPBAR;
	tPos.x -= (float)(m_HPFrame->GetWidth() / 2);
	m_HPFrame->SetPosition(tPos);
	m_HPBar->SetPosition(tPos);
}

void DX9Monster::SetGlobalPosition(D3DXVECTOR2 Position)
{
	m_GlobalPos = Position;
	CalculateGlobalPositionInverse();

	SetPosition(m_GlobalPosInverse);

	if (m_bUILoaded)
		SetUIPosition(m_GlobalPosInverse);
}

void DX9Monster::UpdateGlobalPosition(D3DXVECTOR2 MapOffset, float MapOffsetZeroY)
{
	D3DXVECTOR2 tGP = m_GlobalPos;
	m_GlobalPos.x += MapOffset.x;
	m_GlobalPos.y -= MapOffset.y - MapOffsetZeroY;

	CalculateGlobalPositionInverse();
	m_GlobalPos = tGP;

	SetPosition(m_GlobalPosInverse);

	if (m_bUILoaded)
		SetUIPosition(m_GlobalPosInverse);
}

void DX9Monster::CalculateHP()
{
	float fPercent = (float)DX9Monster::m_HPCurr / (float)m_HPMax;
	float tW = (float)m_HPBar->GetScaledWidth();

	m_HPBar->SetVisibleRange((int)(tW * fPercent), m_HPBar->GetScaledHeight());
}

void DX9Monster::Damage(int Damage)
{
	m_HPCurr -= Damage;
	if (m_HPCurr < 0)
		m_HPCurr = 0;
}

void DX9Monster::Draw()
{
	CalculateHP();

	DX9Life::Draw();
	m_HPFrame->Draw();
	m_HPBar->Draw();
}