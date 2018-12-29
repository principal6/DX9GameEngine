#pragma once

#ifndef _DX9COMMON_H_
#define _DX9COMMON_H_

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#define CINT const int

#define D3DFVF_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_LINE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

const float UV_OFFSET = 0.005f;

enum class DX9EFF_TYPE
{
	Still,
	FlyRight,
	FlyDown,
	FlyUp,
};

class DX9EFF_TYPE_DATA
{
private:
	DX9EFF_TYPE m_Type;
	int m_StartFrame;
	int m_EndFrame;
	D3DXVECTOR2 m_SpawnOffset;
	int m_RepeatCount;

public:
	DX9EFF_TYPE_DATA() : m_StartFrame(0), m_EndFrame(0), m_RepeatCount(1) {};
	DX9EFF_TYPE_DATA(DX9EFF_TYPE Type, int StartFrame, int EndFrame, D3DXVECTOR2 SpawnOffset, int RepeatCount = 1) :
		m_Type(Type), m_StartFrame(StartFrame), m_EndFrame(EndFrame), m_SpawnOffset(SpawnOffset), m_RepeatCount(RepeatCount) {};

	D3DXVECTOR2 GetSpawnOffset() { return m_SpawnOffset; };
	int GetStartFrame() { return m_StartFrame; };
	int GetEndFrame() { return m_EndFrame; };
};

class DX9EFF_INST_DATA
{
private:
	DX9EFF_INST_DATA* m_pNext;
	DX9EFF_TYPE m_Type;
	int m_TypeDataID;
	D3DXVECTOR2 m_Pos;
	int m_CurrFrame;
	int m_RepeatCount;

public:
	DX9EFF_INST_DATA(): m_pNext(nullptr), m_Pos(D3DXVECTOR2(0, 0)) {};
	DX9EFF_INST_DATA(int TypeDataID, D3DXVECTOR2 Position, int CurrFrame) :
		m_pNext(nullptr), m_TypeDataID(TypeDataID), m_Pos(Position), m_CurrFrame(CurrFrame) {};
	
	// Setter
	void SetNext(DX9EFF_INST_DATA* Next) { m_pNext = Next; };
	void SetCurrFrame(int FrameID) { m_CurrFrame = FrameID; };

	// Getter
	DX9EFF_INST_DATA* GetNext() { return m_pNext; };
	DX9EFF_TYPE GetType() { return m_Type; };
	D3DXVECTOR2 GetPos() { return m_Pos; };
	int GetCurrFrame() { return m_CurrFrame; };
	int GetTypeDataID() { return m_TypeDataID; };
};

enum class DX9MAPDIR
{
	Up,
	Down,
	Left,
	Right,
};

struct DX9UV
{
	float u1;
	float u2;
	float v1;
	float v2;

	DX9UV() : u1(0), u2(0), v1(0), v2(0) {};
	DX9UV(float U1, float U2, float V1, float V2) : u1(U1), u2(U2), v1(V1), v2(V2) {};
};

struct DX9MAPDATA
{
	int TileID;
	int MoveID;

	DX9MAPDATA() : TileID(0), MoveID(0) {};
	DX9MAPDATA(int TILEID, int MOVEID) : TileID(TILEID), MoveID(MOVEID) {};
};

enum class DX9ANIMID
{
	Idle = 0,
	Walk,
	Attack1,
	Attack2,
	Attack3,

	Effect,
};

enum class DX9MAPMODE
{
	TileMode,
	MoveMode,
};

enum class DX9WINDOW_STYLE : DWORD
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
	Caption = WS_CAPTION, // WS_BORDER | WS_DLGFRAME
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
	OverlappedWindow = WS_OVERLAPPEDWINDOW, // WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
	PopupWindow = WS_POPUPWINDOW, // WS_POPUP | WS_BORDER | WS_SYSMENU
	ChildWindow = WS_CHILDWINDOW, // WS_CHILD
};

enum class DX9ANIMDIR
{
	Left,
	Right,
};

struct DX9VERTEX_LINE
{
	FLOAT x, y, z, rhw;
	DWORD color;

	DX9VERTEX_LINE() : x(0), y(0), z(0), rhw(1), color(0xffffffff) {};
	DX9VERTEX_LINE(float _x, float _y, float _z, float _rhw, DWORD _color) :
		x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {};
};

struct DX9VERTEX_IMAGE
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT u, v;

	DX9VERTEX_IMAGE() : x(0), y(0), z(0), rhw(1), color(0xFFFFFFFF), u(0), v(0) {};
	DX9VERTEX_IMAGE(float _x, float _y, float _u, float _v) : x(_x), y(_y), z(0), rhw(1), color(0xFFFFFFFF), u(_u), v(_v) {};
	DX9VERTEX_IMAGE(float _x, float _y, float _z, float _rhw, DWORD _color, float _u, float _v) :
		x(_x), y(_y), z(_z), rhw(_rhw), color(_color), u(_u), v(_v) {};
};

struct DX9INDEX3
{
	DX9INDEX3() : _0(0), _1(0), _2(0) {};
	DX9INDEX3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};

	WORD	_0, _1, _2;
};

struct DX9INDEX2
{
	DX9INDEX2() : _0(0), _1(0) {};
	DX9INDEX2(int ID0, int ID1) : _0(ID0), _1(ID1) {};

	WORD	_0, _1;
};

struct DX9ANIMDATA
{
	int FrameS;
	int FrameE;

	DX9ANIMDATA() : FrameS(0), FrameE(0) {};
	DX9ANIMDATA(int _FrameS, int _FrameE) : FrameS(_FrameS), FrameE(_FrameE) {};
};

struct DX9BOUNDINGBOX
{
	D3DXVECTOR2 PosOffset;
	D3DXVECTOR2 Size;

	DX9BOUNDINGBOX() : PosOffset(D3DXVECTOR2(0, 0)), Size(D3DXVECTOR2(0, 0)) {};
	DX9BOUNDINGBOX(D3DXVECTOR2 _POSOFFSET, D3DXVECTOR2 _SIZE) :
		PosOffset(_POSOFFSET), Size(_SIZE) {};
};

struct COLORRGB
{
	int r, g, b;

	COLORRGB() : r(0), g(0), b(0) {};
	COLORRGB(int Red, int Green, int Blue) : r(Red), g(Green), b(Blue) {};
};

#endif