#include "DX9Image.h"

using namespace DX9ENGINE;

// Static member variable declaration
const DWORD DX9Image::DEF_BOUNDINGBOX_COLOR = D3DCOLOR_ARGB(255, 0, 150, 50);

DX9Image::DX9Image()
{
	m_pDevice = nullptr;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pTexture = nullptr;

	ClearVertexAndIndexData();

	m_Width = 100;
	m_Height = 100;
	m_ScaledWidth = m_Width;
	m_ScaledHeight = m_Height;
	m_VisibleWidth = -1;
	m_VisibleHeight = -1;
	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);

	m_BoundingBoxColor = DEF_BOUNDINGBOX_COLOR;
}

DX9Common::ReturnValue DX9Image::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData)
{
	if (pDevice == nullptr)
		return ReturnValue::DEVICE_NULL;

	m_pDevice = pDevice;
	m_WindowData = refData;

	ClearVertexAndIndexData();
	CreateVertexBuffer();
	CreateIndexBuffer();
	UpdateVertexBuffer();
	UpdateIndexBuffer();

	m_BoundingBoxLine.Create(m_pDevice);
	m_BoundingBoxLine.AddBox(D3DXVECTOR2(0, 0), D3DXVECTOR2(10, 10), m_BoundingBoxColor);
	m_BoundingBoxLine.AddEnd();

	return ReturnValue::OK;
}

void DX9Image::Destroy()
{
	m_pDevice = nullptr; // Just set to nullptr cuz it's newed in <DX9Base> class
	ClearVertexAndIndexData();

	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
}

void DX9Image::ClearVertexAndIndexData()
{
	m_Vertices.clear();
	m_Indices.clear();
}

void DX9Image::CreateVertexBuffer()
{
	if (m_Vertices.size() == 0)
	{
		m_Vertices.push_back(VertexImage(m_Position.x, m_Position.y, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f));
		m_Vertices.push_back(VertexImage(m_Position.x + m_Width, m_Position.y, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f));
		m_Vertices.push_back(VertexImage(m_Position.x, m_Position.y + m_Height, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f));
		m_Vertices.push_back(VertexImage(m_Position.x + m_Width, m_Position.y + m_Height, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f));
	}

	int rVertSize = sizeof(VertexImage) * static_cast<int>(m_Vertices.size());
	if (FAILED(m_pDevice->CreateVertexBuffer(rVertSize, 0, D3DFVF_TEXTURE, D3DPOOL_MANAGED, &m_pVertexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Image::CreateIndexBuffer()
{
	if (m_Indices.size() == 0)
	{
		m_Indices.push_back(Index3(0, 1, 3));
		m_Indices.push_back(Index3(0, 3, 2));
	}

	int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
	if (FAILED(m_pDevice->CreateIndexBuffer(rIndSize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, nullptr)))
	{
		return;
	}
}

void DX9Image::UpdateVertexBuffer()
{
	if (m_Vertices.size() > 0)
	{
		int rVertSize = sizeof(VertexImage) * static_cast<int>(m_Vertices.size());
		VOID* pVertices;
		if (FAILED(m_pVertexBuffer->Lock(0, rVertSize, (void**)&pVertices, 0)))
		{
			return;
		}
		memcpy(pVertices, &m_Vertices[0], rVertSize);
		m_pVertexBuffer->Unlock();
	}
}

void DX9Image::UpdateIndexBuffer()
{
	if (m_Indices.size() > 0)
	{
		int rIndSize = sizeof(Index3) * static_cast<int>(m_Indices.size());
		VOID* pIndices;
		if (FAILED(m_pIndexBuffer->Lock(0, rIndSize, (void **)&pIndices, 0)))
		{
			return;
		}
		memcpy(pIndices, &m_Indices[0], rIndSize);
		m_pIndexBuffer->Unlock();
	}
}

void DX9Image::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	if (m_pTexture)
	{
		m_pDevice->SetTexture(0, m_pTexture);

		// Texture alpha * Diffuse color alpha
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}

	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(VertexImage));
	m_pDevice->SetFVF(D3DFVF_TEXTURE);
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, static_cast<int>(m_Vertices.size()), 0, static_cast<int>(m_Indices.size()));

	m_pDevice->SetTexture(0, nullptr);
}

void DX9Image::DrawBoundingBox()
{
	m_BoundingBoxLine.SetBoxPosition(m_Position + m_BoundingBox.PositionOffset, m_BoundingBox.Size);
	m_BoundingBoxLine.Draw();
}

void DX9Image::FlipHorizontal()
{
	float tempu1 = m_Vertices[0].u;

	m_Vertices[0].u = m_Vertices[1].u;
	m_Vertices[2].u = m_Vertices[3].u;
	m_Vertices[1].u = tempu1;
	m_Vertices[3].u = tempu1;

	UpdateVertexBuffer();
}

void DX9Image::FlipVertical()
{
	float tempv1 = m_Vertices[0].v;

	m_Vertices[0].v = m_Vertices[2].v;
	m_Vertices[1].v = m_Vertices[3].v;
	m_Vertices[2].v = tempv1;
	m_Vertices[3].v = tempv1;

	UpdateVertexBuffer();
}

void DX9Image::SetSize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;
	m_ScaledWidth = static_cast<int>(m_Width * m_Scale.x);
	m_ScaledHeight = static_cast<int>(m_Height * m_Scale.y);
	UpdateVertexData();
}

void DX9Image::SetTexture(WSTRING FileName)
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	WSTRING NewFileName;
	NewFileName = m_WindowData.AppDir;
	NewFileName += ASSET_DIR;
	NewFileName += FileName;

	D3DXIMAGE_INFO tImageInfo;
	if (FAILED(D3DXCreateTextureFromFileEx(m_pDevice, NewFileName.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, &tImageInfo, nullptr, &m_pTexture)))
		return;

	m_Width = tImageInfo.Width;
	m_Height = tImageInfo.Height;
	m_ScaledWidth = static_cast<int>(m_Width * m_Scale.x);
	m_ScaledHeight = static_cast<int>(m_Height * m_Scale.y);

	UpdateVertexData();
}

void DX9Image::SetPosition(D3DXVECTOR2 Position)
{
	m_Position = Position;
	UpdateVertexData();
}

void DX9Image::SetPositionCentered(D3DXVECTOR2 Position)
{
	m_Position = D3DXVECTOR2(Position.x - (static_cast<float>(m_ScaledWidth) / 2.0f), Position.y - (static_cast<float>(m_ScaledHeight) / 2.0f));
	m_Position = Position;
	UpdateVertexData();
}

void DX9Image::SetScale(D3DXVECTOR2 Scale)
{
	m_Scale = Scale;
	m_ScaledWidth = static_cast<int>(m_Width * m_Scale.x);
	m_ScaledHeight = static_cast<int>(m_Height * m_Scale.y);

	UpdateVertexData();
}

void DX9Image::SetVisibleRange(int Width, int Height)
{
	m_VisibleWidth = Width;
	m_VisibleHeight = Height;

	UpdateVertexData();
}

void DX9Image::SetUVRange(float u1, float u2, float v1, float v2)
{
	if (m_Vertices.size())
	{
		//@warning: FloatUV offset is done in order to make sure the image borders do not invade contiguous images
		u1 += UV_OFFSET;
		v1 += UV_OFFSET;
		u2 -= UV_OFFSET;
		v2 -= UV_OFFSET;
		UpdateVertexData(u1, v1, u2, v2);
	}
}

void DX9Image::SetAlpha(int Alpha)
{
	if (m_Vertices.size())
	{
		Alpha = min(255, Alpha);
		Alpha = max(0, Alpha);

		for (VertexImage& iterator : m_Vertices)
		{
			iterator.color = D3DCOLOR_ARGB(Alpha, 255, 255, 255);
		}
		UpdateVertexBuffer();
	}
}

DX9Image* DX9Image::SetBoundingBox(D3DXVECTOR2 Size)
{
	m_BoundingBox.PositionOffset.x = -Size.x / 2.0f;
	m_BoundingBox.PositionOffset.y = -Size.y;

	m_BoundingBox.Size.x = static_cast<float>(m_ScaledWidth) + Size.x;
	m_BoundingBox.Size.y = static_cast<float>(m_ScaledHeight) + Size.y;

	return this;
}

void DX9Image::SetBoundingBoxColor(DWORD Color)
{
	DX9Image::m_BoundingBoxColor = Color;
}

void DX9Image::UpdateVertexData()
{
	if (m_Vertices.size() < 4)
		return;

	int tW = m_Width;
	int tH = m_Height;

	if (m_VisibleWidth != -1)
		tW = m_VisibleWidth;
	if (m_VisibleHeight != -1)
		tH = m_VisibleHeight;

	m_Vertices[0].x = m_Position.x;
	m_Vertices[0].y = m_Position.y;
	m_Vertices[1].x = m_Position.x + tW * m_Scale.x;
	m_Vertices[1].y = m_Position.y;
	m_Vertices[2].x = m_Position.x;
	m_Vertices[2].y = m_Position.y + tH * m_Scale.y;
	m_Vertices[3].x = m_Position.x + tW * m_Scale.x;
	m_Vertices[3].y = m_Position.y + tH * m_Scale.y;

	UpdateVertexBuffer();
}

void DX9Image::UpdateVertexData(float u1, float v1, float u2, float v2)
{
	if (m_Vertices.size() < 4)
		return;

	m_Vertices[0].u = u1;
	m_Vertices[0].v = v1;
	m_Vertices[1].u = u2;
	m_Vertices[1].v = v1;
	m_Vertices[2].u = u1;
	m_Vertices[2].v = v2;
	m_Vertices[3].u = u2;
	m_Vertices[3].v = v2;

	UpdateVertexBuffer();
}

int DX9Image::GetWidth() const
{
	return m_Width;
}

int DX9Image::GetHeight() const
{
	return m_Height;
}

int DX9Image::GetScaledWidth() const
{
	return m_ScaledWidth;
}

int DX9Image::GetScaledHeight() const 
{ 
	return m_ScaledHeight;
}

D3DXVECTOR2 DX9Image::GetPosition() const
{
	return m_Position;
}

D3DXVECTOR2 DX9Image::GetCenterPosition() const
{
	D3DXVECTOR2 Result = m_Position;
	Result.x += static_cast<float>(m_ScaledWidth) / 2.0f;
	Result.y += static_cast<float>(m_ScaledHeight) / 2.0f;

	return Result;
}

DX9Image::BoundingBox DX9Image::GetBoundingBox() const
{
	BoundingBox Result;
	Result.PositionOffset = m_Position + m_BoundingBox.PositionOffset;
	Result.Size = m_BoundingBox.Size;

	return Result;
}

bool DX9Image::IsTextureLoaded() const
{
	if (m_pTexture)
		return true;
	return false;
}