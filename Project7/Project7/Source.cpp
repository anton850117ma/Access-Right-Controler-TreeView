#define _CRT_SECURE_NO_WARNINGS 
#pragma comment(lib, "comctl32.lib")

#include "driver\lib\SSCharAry.h"

#include <windows.h>
#include <iostream>
#include <cstring>
#include <commctrl.h>
#include <winuser.h>
#include <TChar.h>
#include <map>
#include <algorithm>
#include <fstream>

#define CX_BITMAP 24
#define CY_BITMAP 24
#define NUM_BITMAPS 2

using namespace std;

HINSTANCE g_hInst;
HMENU IDC_TVMAIN;
map<HTREEITEM, HTREEITEM> tvmap;
HTREEITEM AddItemToTree2(HWND hwndTrV, LPTSTR name, HTREEITEM hPrev, LPTSTR path, int expended);
DWORD ListTempFileInDrectory(LPTSTR szPath, HWND hwndTrV, HTREEITEM parent, int times, int inserted);
TCHAR szFullPath[MAX_PATH];

// Structure for recording nodes in TreeView
struct node {
	bool Once;		// If the children files and folders in this folder have been traveled
	SSCharAry AbsPath;	// Absolute path
	TVITEM tvir;		// TreeView item
};

// An application-defined function that processes messages sent to a window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// To deal with different messages sent by a common control to its parent window when an event has occurred
	LPNMHDR lpnmh = (LPNMHDR)lParam;
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
	switch (msg)
	{
		// Mainly focus on clicks on TreeView items
		case WM_NOTIFY:
			if (lpnmh->idFrom == wParam) 
			{
				switch (lpnmh->code) 
				{	
					// If selected item is changed, get the new item
					case TVN_SELCHANGED:
					{  
						node* getnode = (node*)(pnmtv->itemNew.lParam);
						break;
					}
					// If the item is expended, get its first child item
					case TVN_ITEMEXPANDED:
					{
						node* getnode = (node*)(pnmtv->itemNew.lParam);
						if (!getnode->Once) 
						{
							HTREEITEM child = TreeView_GetChild(hwnd, pnmtv->itemNew.hItem);
							getnode->Once = true;
						}
						break;
					}
				}
			}
		case WM_COMMAND:
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

DWORD ListTempFileInDrectory(LPTSTR szPath, HWND hwndTrV, HTREEITEM parent, int times, int inserted)
{	
	// Travel all children files and folders in the current folder and add them to the TreeView
	TCHAR szFilePath[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	HTREEITEM tvpar2;
	
	// Make the Path be possible to travel
	_tcscpy(szFilePath, szPath);
	_tcscat(szFilePath, _T("\\*"));
	hListFile = FindFirstFile(szFilePath, &FindFileData);

	if (hListFile == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	else
	{
		do
		{
			if (_tcscmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				_tcscmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			wsprintf(szFullPath, _T("%s\\%s"), szPath, FindFileData.cFileName);
			if(inserted == 1)
				tvpar2 = AddItemToTree2(hwndTrV, FindFileData.cFileName, parent, szFullPath, times);
			
			if (times == 1) 
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{	
					// Encounter a folder and get into it
					ListTempFileInDrectory(szFullPath, hwndTrV, tvpar2, 2, 1);
				}
			}
			
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}


HTREEITEM AddItemToTree2(HWND hwndTrV, LPTSTR name, HTREEITEM hPrev, LPTSTR path, int expended) 
{
	
	/*
	Add an item to the TreeView.
	
	hwndTrV 	TreeView handle
	name		The file or folder's name
	hPrev		The handle of the parent item
	path		Absolute path of the file or folder
	expended	Check if expended before
	*/
	// 
	TVITEM tvi;
	TVINSERTSTRUCT tvins;
	node *newnode = new node;
	newnode->AbsPath = path;
	if(expended == 1)newnode->Once = true;
	else newnode->Once = false;

	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.pszText = name;
	tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);
	tvi.iImage = 0;
	tvi.iSelectedImage = 1;
	tvi.lParam = (LPARAM)newnode;
	tvi.hItem = ;
	newnode->tvir = tvi;
	tvins.item = tvi;
	tvins.hInsertAfter = TVI_SORT;
	tvins.hParent = hPrev;
	
	// Send a message to insert the item
	hPrev = (HTREEITEM)SendMessage(hwndTrV, TVM_INSERTITEM, 0,
		(LPARAM)(LPTVINSERTSTRUCT)&tvins);

	return hPrev;
}

BOOL InitTreeViewImageLists(HWND hwndTV)
{
	HIMAGELIST himl;  // handle to image list 
	HBITMAP hbmp;     // handle to bitmap 

	// Create the image list. 
	if ((himl = ImageList_Create(CX_BITMAP,	CY_BITMAP, FALSE,NUM_BITMAPS, 0)) == NULL)
		return FALSE;

	// Add opened file, closed file bitmaps. For now, I just used two different eomji pictures.
	hbmp = (HBITMAP)LoadImage(g_hInst, _T("EYEROLL.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int g_nOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(g_hInst, _T("CAT.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int g_nClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	// Associate the image list with the TreeView's handle. 
	TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);
	return TRUE;
}

BOOL InitTreeViewItems(HWND hwndTrV) 
{
	/*
	Initialize the root of the TreeView.
	Add the root item (C drive) to the TreeView and travel only first level children of the root item
	*/
	HTREEITEM root;
	TCHAR szCurrentPath[MAX_PATH] = { _T("C:") };
	root = AddItemToTree2(hwndTrV, _T("C:"), TVI_ROOT, szCurrentPath, 1);
	ListTempFileInDrectory(szCurrentPath, hwndTrV, root, 1, 1);

	return TRUE;
}

HWND CreateTreeView(HWND parent)
{
	// Create a TreeView window in the parent window
	HWND hwndTrV;
	RECT rcClient;
	IDC_TVMAIN = NULL;

	// Retrieves the coordinates of a window's client area
	GetClientRect(parent, &rcClient);
	
	// Create the TreeView window
	hwndTrV = CreateWindowEx(0, WC_TREEVIEW, _T("TV_Menu"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES |
		TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES,
		0, 0, rcClient.right, rcClient.bottom, parent,
		IDC_TVMAIN, g_hInst, NULL);

	// If fail to initialize TreeView, destroy the specified window
	if (!InitTreeViewImageLists(hwndTrV) ||
		!InitTreeViewItems(hwndTrV))
	{
		DestroyWindow(hwndTrV);
		return FALSE;
	}
	return hwndTrV;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	// Use Windows API to open a window
	WNDCLASSEX wc;		// window class
	HWND hwnd;		// a handle for the window
	MSG Msg;
	g_hInst = hInstance;	// globalize the handle for future use
	
	// Initialize the properties of the main window
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("WinMain");
	wc.hIconSm = NULL;
	
	// Register the window class for the main window
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	// Create the main window 
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		TEXT("WinMain"),
		TEXT("Kongo"),
		WS_BORDER | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 440,
		NULL, NULL, hInstance, NULL);
	
	// Create TreeView and return its handle
	CreateTreeView(hwnd);
	
	// If the main window cannot be created, terminate the application
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"),
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	// Show the window and paint its contents 
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	// Start the message loop 
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	// Return the exit code to the system
	return Msg.wParam;
}
