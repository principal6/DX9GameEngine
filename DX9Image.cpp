#include "DX9Image.h"

DX9Image::DX9Image() 
{
	m_pDevice = nullptr;

	m_pVB = nullptr;
	m_pIB = nullptr;
	m_pTexture = nullptr;

	m_VertCount = 0;
	m_IndCount = 0;

	m_Pos = D3DXVECTOR2(0.0f, 0.0f);
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);

	m_Width = 10;
	m_Height = 10;
	m_ScaledW = 10;
	m_ScaledH = 10;
	m_VisibleW = -1;
	m_VisibleH = -1;
}

int DX9Image::Create(LPDIRECT3DDEVICE9 pD3DDev, std::wstring BaseDir)
{
	m_pDevice = pD3DDev;
	m_Vert.clear();
	m_Ind.clear();
	m_BaseDir = BaseDir;

	CreateVB();
	CreateIB();

	m_BBLine.Create(pD3DDev);
	m_BBLine.AddBox(D3DXVECTOR2(0, 0), D3DXVECTOR2(10, 10), D3DCOLOR_ARGB(255, 255, 255, 255));
	m_BBLine.AddEnd();

	return 0;
}

int DX9Image::CreateVB()
{
	m_Vert.push_back(DX9VERTEX_IMAGE(m_Pos.x, m_Pos.y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f));
	m_Vert.push_back(DX9VERTEX_IMAGE(m_Pos.x + m_Width, m_Pos.y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f));
	m_Vert.push_back(DX9VERTEX_IMAGE(m_Pos.x, m_Pos.y + m_Height, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f));
	m_Vert.push_back(DX9VERTEX_IMAGE(m_Pos.x + m_Width, m_Pos.y + m_Height, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f));
	m_VertCount = (int)m_Vert.size();

	int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_VertCount;
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVB, nullptr)))
	{
		return -1;
	}

	return 0;
}

int DX9Image::CreateIB()
{
	m_Ind.push_back(DX9INDEX3(0, 1, 3));
	m_Ind.push_back(DX9INDEX3(0, 3, 2));
	m_IndCount = (int)m_Ind.size();

	int rIndSize = sizeof(DX9INDEX3) * m_IndCount;
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	{
		return -1;
	}

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
	{
		return -1;
	}

	memcpy(pIndices, &m_Ind[0], rIndSize);
	m_pIB->Unlock();

	return 0;
}

int DX9Image::Destroy()
{
	m_pDevice = nullptr; // DX9Base에서 생성했으므로 여기서는 참조 해제만 한다!

	m_Vert.clear();
	m_Ind.clear();

	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
		
	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}
		
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}

	return 0;
}

int DX9Image::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	if (m_pTexture)
	{
		m_pDevice->SetTexture(0, m_pTexture);
		// 텍스처 알파 * 디퓨즈 컬러 알파
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(DX9VERTEX_IMAGE));
	m_pDevice->SetFVF(D3DFVF_TEXTURE);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertCount, 0, m_IndCount);

	return 0;
}

int DX9Image::DrawBoundingBox()
{
	m_BBLine.SetBoxPosition(m_Pos + m_BB.PosOffset, m_BB.Size);
	m_BBLine.Draw();

	return 0;
}

int DX9Image::SetPosition(D3DXVECTOR2 Pos)
{
	m_Pos = Pos;
	UpdateVertData();
	return 0;
}

int DX9Image::SetPositionCentered(D3DXVECTOR2 Pos)
{
	m_Pos = D3DXVECTOR2(Pos.x - (m_ScaledW / 2), Pos.y - (m_ScaledH / 2));
	m_Pos = Pos;
	UpdateVertData();
	return 0;
}

int DX9Image::SetSize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;
	m_ScaledW = (int)(m_Width * m_Scale.x);
	m_ScaledH = (int)(m_Height * m_Scale.y);
	UpdateVertData();
	return 0;
}

int DX9Image::SetScale(D3DXVECTOR2 Scale)
{
	m_Scale = Scale;
	m_ScaledW = (int)(m_Width * m_Scale.x);
	m_ScaledH = (int)(m_Height * m_Scale.y);
	UpdateVertData();
	return 0;
}

int DX9Image::SetVisibleRange(int Width, int Height)
{
	m_VisibleW = Width;
	m_VisibleH = Height;
	UpdateVertData();
	return 0;
}

int DX9Image::SetUVRange(float u1, float u2, float v1, float v2)
{
	if (m_Vert.size())
	{
		UpdateVertData(u1, v1, u2, v2);
		return 0;
	}
	return -1;
}

int DX9Image::SetAlpha(int Alpha)
{
	if (m_Vert.size())
	{
		Alpha = min(255, Alpha);
		Alpha = max(0, Alpha);

		for (int i = 0; i < m_Vert.size(); i++)
		{
			m_Vert[i].color = D3DCOLOR_ARGB(Alpha, 255, 255, 255);
		}
		UpdateVB();
		return 0;
	}
	return -1;
}

int DX9Image::FlipHorizontal()
{
	float tempu1 = m_Vert[0].u;

	m_Vert[0].u = m_Vert[1].u;
	m_Vert[2].u = m_Vert[3].u;
	m_Vert[1].u = tempu1;
	m_Vert[3].u = tempu1;

	UpdateVB();
	return 0;
}

int DX9Image::FlipVertical()
{
	float tempv1 = m_Vert[0].v;

	m_Vert[0].v = m_Vert[2].v;
	m_Vert[1].v = m_Vert[3].v;
	m_Vert[2].v = tempv1;
	m_Vert[3].v = tempv1;

	UpdateVB();
	return 0;
}

int DX9Image::SetTexture(std::wstring FileName)
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	std::wstring NewFileName;
	NewFileName = m_BaseDir;
	NewFileName += L"\\Data\\";
	NewFileName += FileName;

	// 텍스처 불러오기
	D3DXIMAGE_INFO tImgInfo;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &tImgInfo, nullptr, &m_pTexture)))
		return -1;

	m_Width = tImgInfo.Width;
	m_Height = tImgInfo.Height;

	m_ScaledW = (int)(m_Width * m_Scale.x);
	m_ScaledH = (int)(m_Height * m_Scale.y);
	
	UpdateVertData();

	return 0;
}

int DX9Image::SetBoundingnBox(D3DXVECTOR2 Size)
{
	m_BB.PosOffset.x = -Size.x / 2;
	m_BB.PosOffset.y = -Size.y;

	m_BB.Size.x = (float)m_ScaledW + Size.x;
	m_BB.Size.y = (float)m_ScaledH + Size.y;

	return 0;
}

int DX9Image::UpdateVB()
{
	if (m_Vert.size() > 0)
	{
		int rVertSize = sizeof(DX9VERTEX_IMAGE) * m_VertCount;
		VOID* pVertices;
		if (FAILED(m_pVB->Lock(0, rVertSize, (void**)&pVertices, 0)))
		{
			return -1;
		}
		memcpy(pVertices, &m_Vert[0], rVertSize);
		m_pVB->Unlock();

		return 0;
	}

	return -1;
}

int DX9Image::UpdateIB()
{
	if (m_Ind.size() > 0)
	{
		int rIndSize = sizeof(DX9INDEX3) * m_IndCount;
		VOID* pIndices;
		if (FAILED(m_pIB->Lock(0, rIndSize, (void **)&pIndices, 0)))
		{
			return -1;
		}
		memcpy(pIndices, &m_Ind[0], rIndSize);
		m_pIB->Unlock();

		return 0;
	}
	
	return -1;
}

int DX9Image::UpdateVertData()
{
	if (m_Vert.size() < 4)
		return -1;

	int tW = m_Width;
	int tH = m_Height;

	if (m_VisibleW != -1)
		tW = m_VisibleW;
	if (m_VisibleH != -1)
		tH = m_VisibleH;

	m_Vert[0].x = m_Pos.x;
	m_Vert[0].y = m_Pos.y;
	m_Vert[1].x = m_Pos.x + tW * m_Scale.x;
	m_Vert[1].y = m_Pos.y;
	m_Vert[2].x = m_Pos.x;
	m_Vert[2].y = m_Pos.y + tH * m_Scale.y;
	m_Vert[3].x = m_Pos.x + tW * m_Scale.x;
	m_Vert[3].y = m_Pos.y + tH * m_Scale.y;

	UpdateVB();
	return 0;
}

int DX9Image::UpdateVertData(float u1, float v1, float u2, float v2)
{
	if (m_Vert.size() < 4)
		return -1;

	m_Vert[0].u = u1;
	m_Vert[0].v = v1;
	m_Vert[1].u = u2;
	m_Vert[1].v = v1;
	m_Vert[2].u = u1;
	m_Vert[2].v = v2;
	m_Vert[3].u = u2;
	m_Vert[3].v = v2;
	UpdateVertData();
	return 0;
}

bool DX9Image::IsTextureLoaded()
{
	if (m_pTexture)
		return true;
	return false;
}

D3DXVECTOR2 DX9Image::GetCenterPosition()
{
	D3DXVECTOR2 Result = m_Pos;
	Result.x += m_ScaledW / 2;
	Result.y += m_ScaledH / 2;

	return Result;
}

DX9BOUNDINGBOX DX9Image::GetBoundingBox()
{
	DX9BOUNDINGBOX Result;
	Result.PosOffset = m_Pos + m_BB.PosOffset;
	Result.Size = m_BB.Size;

	return Result;
}