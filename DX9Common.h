#pragma once
#ifndef DX9COMMON_H
#define DX9COMMON_H

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>
#include <cassert>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#define CINT const int

#define D3DFVF_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_LINE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct DX9VERTEX_LINE {
	DX9VERTEX_LINE() : x(0), y(0), z(0), rhw(1), color(0xffffffff) {};
	DX9VERTEX_LINE(float _x, float _y, float _z, float _rhw, DWORD _color) :
		x(_x), y(_y), z(_z), rhw(_rhw), color(_color) {};

	FLOAT x, y, z, rhw;
	DWORD color;
};

struct DX9VERTEX_IMAGE {
	DX9VERTEX_IMAGE() : x(0), y(0), z(0), rhw(1), color(0xffffffff), u(0), v(0) {};
	DX9VERTEX_IMAGE(float _x, float _y, float _z, float _rhw,
		DWORD _color, float _u, float _v) : x(_x), y(_y), z(_z), rhw(_rhw),
		color(_color), u(_u), v(_v) {};

	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT u, v;
};

struct DX9INDEX3 {
	DX9INDEX3() : _0(0), _1(0), _2(0) {};
	DX9INDEX3(int ID0, int ID1, int ID2) : _0(ID0), _1(ID1), _2(ID2) {};

	WORD	_0, _1, _2;
};

struct DX9INDEX2 {
	DX9INDEX2() : _0(0), _1(0) {};
	DX9INDEX2(int ID0, int ID1) : _0(ID0), _1(ID1) {};

	WORD	_0, _1;
};

enum struct DX9MAPMODE
{
	TileMode,
	MoveMode,
};

enum struct DX9WINDOW_STYLE : DWORD
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

class COLORRGB
{
public:
	int r, g, b;

public:
	COLORRGB(int Red, int Green, int Blue) : r(Red), g(Green), b(Blue) {};
};

#endif