#pragma once

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>
#include <vector>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

using WSTRING = std::wstring;
using CINT = const int;

#define DX_FAILED(func) (func != DX9Common::ReturnValue::OK)

struct WindowData
{
	static const int MAX_FILE_LEN = 255;
	int WindowWidth;
	int WindowHeight;
	float WindowHalfWidth;
	float WindowHalfHeight;
	wchar_t AppDir[MAX_FILE_LEN] = { 0 };
};

class DX9Common
{
public:
	enum class FontID
	{
		Title,
		Subtitle,
		Text,
		Debug,
	};

	enum class EffectType
	{
		Still,
		FlyRight,
		FlyDown,
		FlyUp,
	};

	enum class AnimationDir
	{
		Left,
		Right,
	};

	enum class AnimationID
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

	struct AnimationData
	{
		AnimationID AnimID;
		int FrameS, FrameE;

		AnimationData() : FrameS(0), FrameE(0) {};
		AnimationData(AnimationID _AnimID, int StartFrame, int EndFrame) : AnimID(_AnimID), FrameS(StartFrame), FrameE(EndFrame) {};
	};

	enum class ReturnValue
	{
		OK,
		WINDOW_NOT_CREATED,
		DIRECTX_NOT_CREATED,
		BASE_NOT_CREATED,
		INPUT_NOT_CREATED,
		IMAGE_NOT_CREATED,
		MAP_NOT_CREATED,
		SPRITE_NOT_CREATED,
		MONSTERMANAGER_NOT_CREATED,
		EFFECTMANAGER_NOT_CREATED,
		FONTMANAGER_NOT_CREATED,
		OBJECT_NOT_CREATED,
		CREATION_FAILED,
		DEVICE_NULL,
		MAP_NULL,
	};

protected:
	struct FloatUV
	{
		float u1, u2, v1, v2;

		FloatUV() : u1(0), u2(0), v1(0), v2(0) {};
		FloatUV(float U1, float U2, float V1, float V2) : u1(U1), u2(U2), v1(V1), v2(V2) {};
	};

protected:
	static const DWORD D3DFVF_TEXTURE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const DWORD D3DFVF_LINE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	static const int MAX_UNIT_COUNT = 100;
	static const int NUM_KEYS = 256;
	static const float UV_OFFSET;
	static const wchar_t ASSET_DIR[];

	static HINSTANCE m_hInstance;
	static HWND m_hWnd;

	static WindowData m_WindowData;

protected:
	static void DX9Common::ConvertFrameIDIntoUV(int FrameID, int NumCols, int NumRows, FloatUV* UV);
	static void DX9Common::GetTileCols(int SheetWidth, int TileWidth, int* TileCols);
	static void DX9Common::GetTileRows(int SheetHeight, int TileHeight, int* TileRows);

public:
	DX9Common() {};
	~DX9Common() {};

	virtual void Destroy() = 0;
};