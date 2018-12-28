#include "DX9Monster.h"

DX9Monster::DX9Monster() {
	m_fHPPercent = 1.0f;
	m_HPFrame = nullptr;
	m_HPBar = nullptr;
}

int DX9Monster::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir)
{
	DX9Anim::Create(pD3DDev, BaseDir);
	
	m_HPFrame = new DX9Image;
	m_HPFrame->Create(pD3DDev, BaseDir);
	m_HPFrame->SetTexture(L"hpbarbg.png");

	m_HPBar = new DX9Image;
	m_HPBar->Create(pD3DDev, BaseDir);
	m_HPBar->SetTexture(L"hpbar.png");

	return 0;
}

int DX9Monster::Destroy()
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

	DX9Anim::Destroy();
	return 0;
}

int DX9Monster::Draw()
{
	DX9Anim::Draw();
	m_HPFrame->Draw();
	m_HPBar->Draw();

	return 0;
}

int DX9Monster::SetPosition(D3DXVECTOR2 Pos)
{
	DX9Anim::SetPosition(Pos);
	D3DXVECTOR2 tPos = GetCenterPosition();
	tPos.y = Pos.y;
	tPos.y -= OFFSET_Y_HPBAR;
	tPos.x -= (float)(m_HPFrame->GetWidth() / 2);
	m_HPFrame->SetPosition(tPos);
	m_HPBar->SetPosition(tPos);

	return 0;
}