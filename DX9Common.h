#pragma once
#ifndef DX9COMMON_H
#define DX9COMMON_H

#include <iostream>
#include <Windows.h>
#include <d3dx9.h>

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

#define CINT const int

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

class DX9COLOR
{
public:
	int r, g, b;

public:
	DX9COLOR(int Red, int Green, int Blue) : r(Red), g(Green), b(Blue) {};
};

#endif