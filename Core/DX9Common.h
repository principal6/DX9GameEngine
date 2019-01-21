#pragma once

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>
#include <vector>
#include <memory>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#ifndef CONVENIENT_STD
#define CONVENIENT_STD
	using STRING = std::string;
	using WSTRING = std::wstring;

	using WIFSTREAM = std::wifstream;
	using WOFSTREAM = std::wofstream;

	template <typename T>
	using VECTOR = std::vector<T>;

	template <typename T>
	using UNIQUE_PTR = std::unique_ptr<T>;

	template <typename T>
	using SHARED_PTR = std::shared_ptr<T>;

	#define MAKE_UNIQUE(T) std::make_unique<T>
	#define MAKE_SHARED(T) std::make_shared<T>
#endif

namespace DX9ENGINE
{
	#define DX_SUCCEEDED(func) (func == Error::OK)
	#define DX_FAILED(func) (func != Error::OK)
	#define DX_DESTROY(obj) {if(obj) {obj->Destroy(); delete obj; obj = nullptr;}}
	#define DX_DESTROY_UNIQUE(obj) {if(obj) {obj->Destroy();}}
	#define DX_RELEASE(obj) {if(obj) {obj->Release(); obj = nullptr;}}

	using CINT = const int;

	static constexpr int MAX_FILE_LEN = 260;
	static constexpr int MAX_UNIT_COUNT = 100;
	static constexpr int NUM_KEYS = 256; // @warning: This value must be 256

	static const DWORD D3DFVF_TEXTURE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	static const DWORD D3DFVF_LINE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	const wchar_t ASSET_DIR[] = L"\\Asset\\";

	enum class Error
	{
		// No error
		OK,

		// Base creation
		WINDOW_NOT_CREATED,
		DIRECTX_NOT_CREATED,

		// Core creation
		BASE_NOT_CREATED,
		INPUT_NOT_CREATED,
		IMAGE_NOT_CREATED,

		// Sub-class creation
		MAP_NOT_CREATED,
		SPRITE_NOT_CREATED,
		MONSTERMANAGER_NOT_CREATED,
		EFFECTMANAGER_NOT_CREATED,
		FONTMANAGER_NOT_CREATED,
		OBJECT_NOT_CREATED,
		TILESELECTOR_NOT_CREATED,

		// Null pointer
		DEVICE_NULL,
		MAP_NULL,
		BASE_NULL,
	};

	enum class WindowStyle : DWORD
	{
		Overlapped = WS_OVERLAPPED,
		Popup = WS_POPUP,
		Child = WS_CHILD,
		Minimize = WS_MINIMIZE,
		Visible = WS_VISIBLE,
		DIsabled = WS_DISABLED,
		ClipSiblings = WS_CLIPSIBLINGS,
		ClipChildren = WS_CLIPCHILDREN,
		Maximize = WS_MAXIMIZE,
		Caption = WS_CAPTION, // = WS_BORDER | WS_DLGFRAME
		Border = WS_BORDER,
		DlgFrame = WS_DLGFRAME,
		VScroll = WS_VSCROLL,
		HScroll = WS_HSCROLL,
		SysMenu = WS_SYSMENU,
		ThickFrame = WS_THICKFRAME,
		Group = WS_GROUP,
		TabStop = WS_TABSTOP,
		MinimizeBox = WS_MINIMIZEBOX,
		MaximizeBox = WS_MAXIMIZEBOX,
		OverlappedWindow = WS_OVERLAPPEDWINDOW, // = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		PopupWindow = WS_POPUPWINDOW, // = WS_POPUP | WS_BORDER | WS_SYSMENU
		ChildWindow = WS_CHILDWINDOW, // = WS_CHILD
		ChildWindow2 = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
	};

	enum class FontID
	{
		Title,
		Subtitle,
		Text,
		Debug,
	};

	enum class MapMode
	{
		TileMode,
		MoveMode,
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

	struct FloatUV
	{
		float u1, u2, v1, v2;

		FloatUV() : u1(0), u2(0), v1(0), v2(0) {};
		FloatUV(float U1, float U2, float V1, float V2) : u1(U1), u2(U2), v1(V1), v2(V2) {};
	};

	static void ConvertFrameIDIntoUV(int FrameID, int NumCols, int NumRows, FloatUV* UV)
	{
		UV->u1 = static_cast<float>(FrameID % NumCols) / static_cast<float>(NumCols);
		UV->u2 = UV->u1 + (1.0f / static_cast<float>(NumCols));
		UV->v1 = static_cast<float>(FrameID / NumCols) / static_cast<float>(NumRows);
		UV->v2 = UV->v1 + (1.0f / static_cast<float>(NumRows));
	}

	static void GetTileCols(int SheetWidth, int TileWidth, int* TileCols)
	{
		*TileCols = static_cast<int>(SheetWidth / TileWidth);
	}

	static void GetTileRows(int SheetHeight, int TileHeight, int* TileRows)
	{
		*TileRows = static_cast<int>(SheetHeight / TileHeight);
	}

	inline auto GetColorR(DWORD Color)->BYTE { return ((Color << 8) >> 24); }
	inline auto GetColorG(DWORD Color)->BYTE { return ((Color << 16) >> 24); }
	inline auto GetColorB(DWORD Color)->BYTE { return ((Color << 24) >> 24); }
};