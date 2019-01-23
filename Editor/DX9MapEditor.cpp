#include "DX9MapEditor.h"
#include "resource.h"

using namespace DX9ENGINE;

// Static member variables
UNIQUE_PTR<DX9Window> DX9MapEditor::ms_WindowParent;
UNIQUE_PTR<DX9Window> DX9MapEditor::ms_WindowLeft;
UNIQUE_PTR<DX9Window> DX9MapEditor::ms_WindowRight;
SMapInfo DX9MapEditor::ms_MapInfo;
UNIQUE_PTR<DX9Image> DX9MapEditor::ms_TileImage;
UNIQUE_PTR<DX9Image> DX9MapEditor::ms_MoveImage;
UNIQUE_PTR<DX9Image> DX9MapEditor::ms_MapBG;
UNIQUE_PTR<DX9Map> DX9MapEditor::ms_Map;
UNIQUE_PTR<DX9MapTileSelector> DX9MapEditor::ms_MapTileSelector;

auto DX9ENGINE::GetLeftChildPositionAndSizeFromParent(RECT Rect)->RECT
{
	RECT Result = {0, 0,
		DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

auto DX9ENGINE::GetRightChildPositionAndSizeFromParent(RECT Rect)->RECT
{
	RECT Result = { DX9MapEditor::WINDOW_SEPERATE_X + DX9MapEditor::WINDOW_SEPERATE_INTERVAL, 0,
		Rect.right - DX9MapEditor::WINDOW_SEPERATE_X - DX9MapEditor::WINDOW_VSCROLL_SIZE - DX9MapEditor::WINDOW_PADDING_X,
		Rect.bottom - DX9MapEditor::WINDOW_HSCROLL_SIZE };
	return Result;
}

STATIC void DX9MapEditor::LoadTileImages()
{
	ms_Map->GetMapInfo(&ms_MapInfo);

	ms_TileImage->SetTexture(ms_MapInfo.TileSheetName);
	ms_MoveImage->SetTexture(ms_MapInfo.MoveSheetName);
	ms_MapTileSelector->SetMapInfo(&ms_MapInfo);

	ms_MapBG->SetSize(ms_MapInfo.MapSize);
}

STATIC void DX9MapEditor::UpdateMapEditorCaption()
{
	WSTRING tempCaption = MAP_EDITOR_NAME;

	if (ms_MapInfo.MapName.size())
	{
		tempCaption += L"  #Map=\"";
		tempCaption += ms_MapInfo.MapName;
		tempCaption += L"\" (";
		tempCaption += ConvertIntToWSTRING(ms_MapInfo.MapCols);
		tempCaption += L"x";
		tempCaption += ConvertIntToWSTRING(ms_MapInfo.MapRows);
		tempCaption += L") #TileSize=";
		tempCaption += ConvertIntToWSTRING(ms_MapInfo.TileSize);
		tempCaption += L" #Tile=\"";
		tempCaption += ms_MapInfo.TileSheetName;
		tempCaption += L"\" (";
		tempCaption += ConvertIntToWSTRING(ms_MapInfo.TileSheetCols);
		tempCaption += L"x";
		tempCaption += ConvertIntToWSTRING(ms_MapInfo.TileSheetRows);
		tempCaption += L")";

		tempCaption += L" #Mouse= ";
		tempCaption += ConvertIntToWSTRING(ms_MapTileSelector->GetMapSelectorPositionInCells().x);
		tempCaption += L" / ";
		tempCaption += ConvertIntToWSTRING(ms_MapTileSelector->GetMapSelectorPositionInCells().y);
	}

	ms_WindowParent->SetWindowCaption(tempCaption);
}

LRESULT CALLBACK DX9ENGINE::ParentWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	RECT tempRect{ 0, 0, 0, 0 };

	switch (Message)
	{
	case WM_COMMAND:
		// Menu & accelerator
		switch (LOWORD(wParam)) // in LOWORD of wParam is the resource id
		{
		case ID_ACCELERATOR40013: // ACCEL <Ctrl + N>
		case ID_MAP_NEWMAP:
			// Show new map dialog
			DialogBox(nullptr, MAKEINTRESOURCE(IDD_DIALOG1), DX9MapEditor::ms_WindowParent->GethWnd(), DlgProcNewMap);
			break;
		case ID_ACCELERATOR40014: // ACCEL <Ctrl + O>
		case ID_MAP_OPENMAP:
			// Show load map dialog
			if (DX9MapEditor::ms_WindowParent->OpenFileDlg(L"JWM File\0*.jwm\0"))
			{
				if (DX9MapEditor::ms_WindowParent->GetDlgFileTitle().size())
				{
					// Load map
					DX9MapEditor::ms_Map->LoadMap(DX9MapEditor::ms_WindowParent->GetDlgFileTitle());

					// Load tile images
					DX9MapEditor::LoadTileImages();

					// Set editor caption
					DX9MapEditor::UpdateMapEditorCaption();
				}
			}
			break;
		case ID_ACCELERATOR40015: // ACCEL <Ctrl + S>
		case ID_MAP_SAVEMAP:
			// Show load map dialog
			if (DX9MapEditor::ms_Map->DoesMapExist())
			{
				if (DX9MapEditor::ms_WindowParent->SaveFileDlg(L"JWM File\0*.jwm\0"))
				{
					DX9MapEditor::ms_Map->SaveMap(DX9MapEditor::ms_WindowParent->GetDlgFileTitle());
				}
			}
			break;
		case ID_ACCELERATOR40017: // ACCEL <F1>
		case ID_HELP_INFO:
			break;
		case ID_ACCELERATOR40008: // ACCEL <F2>
		case ID_MODE_TILEMODE:
			DX9MapEditor::ms_Map->SetMode(EMapMode::TileMode);
			DX9MapEditor::ms_MapTileSelector->UpdateMapMode(DX9MapEditor::ms_Map->GetMode());
			break;
		case ID_ACCELERATOR40010: // ACCEL <F3>
		case ID_MODE_MOVEMODE:
			DX9MapEditor::ms_Map->SetMode(EMapMode::MoveMode);
			DX9MapEditor::ms_MapTileSelector->UpdateMapMode(DX9MapEditor::ms_Map->GetMode());
			break;
		}
		break;
	case WM_SIZE:
		// Resize ms_WindowLeft and ms_WindowRight and their correspondent window in accordance with the parent window size
		if (DX9MapEditor::ms_WindowLeft && DX9MapEditor::ms_WindowRight)
		{
			// Resize ms_WindowLeft and its correspondent window
			GetClientRect(hWnd, &tempRect);
			tempRect = GetLeftChildPositionAndSizeFromParent(tempRect);
			MoveWindow(DX9MapEditor::ms_WindowLeft->GethWnd(), tempRect.left, tempRect.top,
				tempRect.right, tempRect.bottom, TRUE);

			// Resize ms_WindowRight and its correspondent window
			GetClientRect(hWnd, &tempRect);
			tempRect = GetRightChildPositionAndSizeFromParent(tempRect);
			MoveWindow(DX9MapEditor::ms_WindowRight->GethWnd(), tempRect.left, tempRect.top,
				tempRect.right, tempRect.bottom, TRUE);

			if (DX9MapEditor::ms_WindowLeft->GetDevice() && DX9MapEditor::ms_WindowRight->GetDevice())
			{
				DX9MapEditor::ms_WindowLeft->Resize();
				DX9MapEditor::ms_WindowRight->Resize();
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

LRESULT CALLBACK DX9ENGINE::LeftChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	DX9MapEditor::ms_WindowLeft->EditorChildWindowMessageHandler(Message, wParam, lParam);

	if (DX9MapEditor::ms_MapTileSelector)
	{
		if (DX9MapEditor::ms_WindowLeft->IsMouseLeftButtonPressed())
		{
			DX9MapEditor::ms_MapTileSelector->UpdateTileSelector(DX9MapEditor::ms_WindowLeft->GetMouseData());
		}
	}

	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

LRESULT CALLBACK DX9ENGINE::RightChildWindowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	DX9MapEditor::ms_WindowRight->EditorChildWindowMessageHandler(Message, wParam, lParam);

	if (DX9MapEditor::ms_MapTileSelector)
	{
		DX9MapEditor::ms_MapTileSelector->UpdateMapSelector(DX9MapEditor::ms_WindowRight->GetMouseData());

		if (DX9MapEditor::ms_WindowRight->IsMouseLeftButtonPressed())
		{
			// Edit map with seleted tiles
			DX9MapEditor::ms_Map->EditMap(DX9MapEditor::ms_MapTileSelector.get());
		}

		if (DX9MapEditor::ms_WindowRight->IsMouseRightButtonPressed())
		{
			// Edit map with seleted tiles
			DX9MapEditor::ms_Map->EditMap(DX9MapEditor::ms_MapTileSelector.get(), true);
		}

		// Set editor caption (for mouse position change)
		DX9MapEditor::UpdateMapEditorCaption();
	}

	return(DefWindowProc(hWnd, Message, wParam, lParam));
}

// Procedure for <New map> dialog window
LRESULT CALLBACK DX9ENGINE::DlgProcNewMap(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	wchar_t tWC[MAX_FILE_LEN] {};
	WSTRING tStr;

	WSTRING Combobox_Ratio[] = { L"x32", L"x64" };
	HWND tempHWND;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		// Initialize the dialog
		tempHWND = (HWND)GetDlgItem(hDlg, IDC_COMBO1);

		for (size_t i = 0; i < Combobox_Ratio->size() - 1; i++)
		{
			// Send a CB_ADDSTRING message to load each item (Add string to combobox.)
			SendMessage(tempHWND, CB_ADDSTRING, (WPARAM)0, (LPARAM)Combobox_Ratio[i].c_str());
		}

		// Send the CB_SETCURSEL message to display an initial item in the selection field
		SendMessage(tempHWND, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		PostMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_BUTTON1:
			// Tile load button
			if (DX9MapEditor::ms_WindowParent->OpenFileDlg(L"All files\0*.*\0"))
			{
				// Get the name of the tile
				DX9MapEditor::ms_MapInfo.TileSheetName = DX9MapEditor::ms_WindowParent->GetDlgFileTitle();
				SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), DX9MapEditor::ms_MapInfo.TileSheetName.c_str());
			}
			break;
		case IDOK:
			// Create the map only it the tile is specified
			GetDlgItemText(hDlg, IDC_EDIT4, tWC, MAX_FILE_LEN);
			if (wcslen(tWC))
			{
				// Get map name
				GetDlgItemText(hDlg, IDC_EDIT1, tWC, MAX_FILE_LEN);
				DX9MapEditor::ms_MapInfo.MapName = tWC;

				// Get map cols and rows
				DX9MapEditor::ms_MapInfo.MapCols = GetDlgItemInt(hDlg, IDC_EDIT2, FALSE, FALSE);
				DX9MapEditor::ms_MapInfo.MapRows = GetDlgItemInt(hDlg, IDC_EDIT3, FALSE, FALSE);
				
				// Get tile size
				GetDlgItemText(hDlg, IDC_COMBO1, tWC, MAX_FILE_LEN);
				tStr = tWC;
				tStr = tStr.substr(1);
				DX9MapEditor::ms_MapInfo.TileSize = _wtoi(tStr.c_str());

				if (DX9MapEditor::ms_MapInfo.MapCols && DX9MapEditor::ms_MapInfo.MapRows)
				{
					// Create a new map
					DX9MapEditor::ms_Map->CreateMap(&DX9MapEditor::ms_MapInfo);

					// Load tile images
					DX9MapEditor::LoadTileImages();

					// Set editor caption
					DX9MapEditor::UpdateMapEditorCaption();
				}
			}
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

auto DX9MapEditor::Create(int Width, int Height)->EError
{
	// Kill the console window, which is for debugging
	FreeConsole();

	// Set base directory
	wchar_t tempDir[MAX_FILE_LEN]{};
	GetCurrentDirectoryW(MAX_FILE_LEN, tempDir);
	m_BaseDir = tempDir;
	m_BaseDir = m_BaseDir.substr(0, m_BaseDir.find_last_not_of(PROJECT_FOLDER));

	// Create parent window
	if (ms_WindowParent = MAKE_UNIQUE(DX9Window)())
	{
		if (DX_FAILED(ms_WindowParent->CreateParentWindow(WINDOW_X, WINDOW_Y, Width, Height,
			D3DCOLOR_XRGB(50, 50, 80), ParentWindowProc, MAKEINTRESOURCE(IDR_MENU1))))
			return EError::WINDOW_NOT_CREATED;

		// Set main window handle
		m_hWndMain = ms_WindowParent->GethWnd();

		// Set main window caption
		ms_WindowParent->SetWindowCaption(MAP_EDITOR_NAME);
	}

	// Load accelerator
	m_hAccel = LoadAccelerators(nullptr, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// Create left child base and initialize Direct3D9
	if (ms_WindowLeft = MAKE_UNIQUE(DX9Window)())
	{
		RECT tempRect;

		// Get main window RECT
		GetClientRect(m_hWndMain, &tempRect);
		tempRect = GetLeftChildPositionAndSizeFromParent(tempRect);
		if (DX_FAILED(ms_WindowLeft->CreateChildWindow(m_hWndMain, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom,
			D3DCOLOR_XRGB(160, 160, 160), LeftChildWindowProc)))
			return EError::WINDOW_NOT_CREATED;
	}

	// Create image objects
	if (ms_TileImage = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(ms_TileImage->Create(ms_WindowLeft.get(), m_BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}
	if (ms_MoveImage = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(ms_MoveImage->Create(ms_WindowLeft.get(), m_BaseDir)))
			return EError::IMAGE_NOT_CREATED;
	}

	// Create right child base and initialize Direct3D9
	if (ms_WindowRight = MAKE_UNIQUE(DX9Window)())
	{
		RECT tempRect;

		// Get main window RECT
		GetClientRect(m_hWndMain, &tempRect);
		tempRect = GetRightChildPositionAndSizeFromParent(tempRect);
		if (DX_FAILED(ms_WindowRight->CreateChildWindow(m_hWndMain, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom,
			D3DCOLOR_XRGB(200, 200, 200), RightChildWindowProc)))
			return EError::WINDOW_NOT_CREATED;
	}
	
	// Create map object
	if (ms_Map = MAKE_UNIQUE(DX9Map)())
	{
		if (DX_FAILED(ms_Map->Create(ms_WindowRight.get(), m_BaseDir)))
			return EError::MAP_NOT_CREATED;
	}

	// Create map background
	if (ms_MapBG = MAKE_UNIQUE(DX9Image)())
	{
		if (DX_FAILED(ms_MapBG->Create(ms_WindowRight.get(), m_BaseDir)))
			return EError::IMAGE_NOT_CREATED;
		ms_MapBG->SetXRGB(D3DCOLOR_XRGB(20, 20, 80));
	}

	// Create map-tile selector
	if (ms_MapTileSelector = MAKE_UNIQUE(DX9MapTileSelector)())
	{
		if (DX_FAILED(ms_MapTileSelector->Create(ms_WindowLeft.get(), ms_WindowRight.get(), m_BaseDir)))
			return EError::TILESELECTOR_NOT_CREATED;
	}

	return EError::OK;
}

void DX9MapEditor::Run()
{
	while (GetMessage(&m_MSG, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(m_hWndMain, m_hAccel, &m_MSG))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);

			MainLoop();
		}
	}

	Destroy();
}

PRIVATE void DX9MapEditor::MainLoop()
{
	ms_WindowLeft->BeginRender();
	ms_WindowRight->BeginRender();
	if (ms_Map)
	{
		if (ms_Map->GetMode() == EMapMode::TileMode)
		{
			if (ms_TileImage)
			{
				ms_TileImage->Draw();
			}
		}
		else
		{
			if (ms_MoveImage)
			{
				ms_MoveImage->Draw();
			}
		}

		ms_MapBG->SetPosition(D3DXVECTOR2(0, 0));
		ms_Map->SetPosition(D3DXVECTOR2(0, 0));
		ms_MapBG->Draw();
		ms_Map->Draw();

		if (ms_MapTileSelector)
			ms_MapTileSelector->Draw();
	}
	ms_WindowLeft->EndRender();
	ms_WindowRight->EndRender();
}

PRIVATE void DX9MapEditor::Destroy()
{
	DX_DESTROY_SMART(ms_Map);
	DX_DESTROY_SMART(ms_MapTileSelector);
	DX_DESTROY_SMART(ms_TileImage);
	DX_DESTROY_SMART(ms_MoveImage);
	DX_DESTROY_SMART(ms_WindowLeft);
	DX_DESTROY_SMART(ms_WindowRight);
	DX_DESTROY_SMART(ms_WindowParent);
}