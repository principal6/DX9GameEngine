#pragma once

#include "DX9Life.h"

namespace DX9ENGINE
{
	class DX9Sprite final : public DX9Life
	{
	public:
		DX9Sprite() {};
		~DX9Sprite() {};

		auto DX9Sprite::Create(LPDIRECT3DDEVICE9 pDevice, WindowData& refData, DX9Map* pMap)->Error;
		auto DX9Sprite::SetGlobalPosition(D3DXVECTOR2 Position)->DX9Sprite* override;

		auto DX9Sprite::MakeUnit(WSTRING TextureFN, int numCols, int numRows, float Scale = 1.0f)->DX9Sprite* override;
		auto DX9Sprite::AddAnimation(AnimationID AnimID, int StartFrame, int EndFrame)->DX9Sprite* override;
		auto DX9Sprite::SetBoundingBox(D3DXVECTOR2 Size)->DX9Sprite* override;
	};
};