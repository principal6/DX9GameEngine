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
	#define DX_FAILED(func) (func != DX9Common::Error::OK)

	using CINT = const int;

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
		struct RGBInt
		{
			int Red, Green, Blue;

			RGBInt() : Red(0), Green(0), Blue(0) {};
			RGBInt(int _Red, int _Green, int _Blue) : Red(_Red), Green(_Green), Blue(_Blue) {};
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

		enum class Error
		{
			// No error
			OK,

			// Creation failure
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
			
			// Null pointer
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
		static constexpr int MAX_UNIT_COUNT = 100;
		static constexpr int NUM_KEYS = 256;

		static const DWORD D3DFVF_TEXTURE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
		static const DWORD D3DFVF_LINE = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

		static const float UV_OFFSET;
		static const wchar_t ASSET_DIR[];

		static HINSTANCE ms_hInstance;
		HWND m_hWnd;
		WindowData m_WindowData;

	protected:
		static void DX9Common::ConvertFrameIDIntoUV(int FrameID, int NumCols, int NumRows, FloatUV* UV);
		static void DX9Common::GetTileCols(int SheetWidth, int TileWidth, int* TileCols);
		static void DX9Common::GetTileRows(int SheetHeight, int TileHeight, int* TileRows);

	public:
		DX9Common() {};
		virtual ~DX9Common() {};

		virtual void Destroy() = 0;

		virtual void DX9Common::SethWnd(HWND hWnd);
		virtual auto DX9Common::GethWnd() const->HWND;
	};
};

