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
//DWORD ListTempFileInDrectory(LPSTR szPath, HWND hwndTrV, HTREEITEM parent, int times);
TCHAR szFullPath[MAX_PATH];

struct node {
	bool Once;
	SSCharAry AbsPath;
	TVITEM tvir;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//NMHDR  *nmptr;
	LPNMHDR lpnmh = (LPNMHDR)lParam;
	LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
	//UINT check;
	switch (msg)
	{
	case WM_NOTIFY:
		if (lpnmh->idFrom == wParam) {
			switch (lpnmh->code) {
				case TVN_SELCHANGED:
				{  
					//mask, hItem, state, and lParam(itemOld and itemNew)
					//LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
					node* getnode = (node*)(pnmtv->itemNew.lParam);
					//wstring str = getnode->AbsPath;
					//ofstream outFile("text.txt", ios::out | ios::binary);
					//outFile.write((char *)str.c_str(), str.length() * sizeof(wchar_t));
					//outFile.close();
					/*TVITEM tvi;
					tvi.mask = TVIF_PARAM | TVIF_TEXT;
					tvi.hItem = pnmtv->itemNew.hItem;
					TreeView_GetItem(hwnd, &tvi);*/
					//MessageBox(NULL, getnode->AbsPath.Ptr() ,_T("selected"), MB_OK);
					break;
				}
				case TVN_ITEMEXPANDED:
				{
					//hItem, state, and lParam (itemNew)
					//LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
					//MessageBox(NULL, "", "expanded", MB_OK);
					//check = TreeView_GetItemState(hwnd, pnmtv->itemNew.hItem, TVIS_EXPANDEDONCE);
					/*if (check == 0) {
						HTREEITEM child = TreeView_GetChild(hwnd, pnmtv->itemNew.hItem);
						ListTempFileInDrectory(szFullPath, hwnd, pnmtv->itemNew.hItem, 2);
						while (TreeView_GetNextSibling(hwnd,child) != NULL) {

						}
					}*/
					node* getnode = (node*)(pnmtv->itemNew.lParam);
					//HTREEITEM child = TreeView_GetChild(hwnd, pnmtv->itemNew.hItem);

					if (!getnode->Once) {
						HTREEITEM child = TreeView_GetChild(hwnd, pnmtv->itemNew.hItem);
						//MessageBox(NULL, , _T("selected"), MB_OK);
						/*ListTempFileInDrectory(getnode->AbsPath.Ptr(), hwnd, child, 1, 2);
						while (TreeView_GetNextSibling(hwnd, child) != NULL) {

							ListTempFileInDrectory(getnode->AbsPath.Ptr(), hwnd, child, 1, 2);
						}*/
						getnode->Once = true;
					}
					break;
				}
				
				case TVN_KEYDOWN:  // tree has keyboard focus and user pressed a key
				{
					/*LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)lParam;

					if (ptvkd->wVKey == VK_SPACE)  // if user pressed spacebar
					{

						// get the currently selected item
						HTREEITEM ht = TreeView_GetSelection(ptvkd->hdr.hwndFrom);

						// Prepare to test items state

						TVITEM tvItem;

						tvItem.mask = TVIF_HANDLE | TVIF_STATE;
						tvItem.hItem = (HTREEITEM)ht;
						tvItem.stateMask = TVIS_STATEIMAGEMASK;

						// Request the information.
						TreeView_GetItem(ptvkd->hdr.hwndFrom, &tvItem);

						// Return zero if it's not checked, or nonzero otherwise.
						if ((BOOL)(tvItem.state >> 12) - 1)
							//MessageBox(hwnd, "Not checked!", "", MB_OK);
						else
							//MessageBox(hwnd, "Checked!", "", MB_OK);

					}*/
				}
				break;  // see the documentation for TVN_KEYDOWN
				case NM_CLICK:  // user clicked on a tree
				{
					//MessageBox(NULL, "click", "click", MB_OK);
					/*HTREEITEM hti;
					TVHITTESTINFO PrevTest;
					POINT pti;
					DWORD dwPos = GetMessagePos();
					pti.x = LOWORD(dwPos); pti.y = HIWORD(dwPos);
					ScreenToClient(hwnd, &pti);
					PrevTest.flags = TVHT_ONITEM | TVHT_ONITEMICON;
					PrevTest.pt = pti;
					hti = TreeView_HitTest(hwnd , &PrevTest);*/

					//UINT state1 = TreeView_GetItemState(hwnd, HTREEITEM hItem, UINT  stateMask);
					/*TVHITTESTINFO ht = { 0 };

					DWORD dwpos = GetMessagePos();

					// include <windowsx.h> and <windows.h> header files
					ht.pt.x = LOWORD(dwpos);
					ht.pt.y = HIWORD(dwpos);
					MapWindowPoints(HWND_DESKTOP, lpnmh->hwndFrom, &ht.pt, 1);

					TreeView_HitTest(lpnmh->hwndFrom, &ht);

					if (TVHT_ONITEMSTATEICON & ht.flags)
					{
						// Prepare to receive the desired information.

						TVITEM tvItem;

						tvItem.mask = TVIF_HANDLE | TVIF_STATE;
						tvItem.hItem = (HTREEITEM)ht.hItem;
						tvItem.stateMask = TVIS_STATEIMAGEMASK;

						// Request the information.
						TreeView_GetItem(lpnmh->hwndFrom, &tvItem);

						// Return zero if it's not checked, or nonzero otherwise.
						if (tvItem.state == TVIS_SELECTED)
							MessageBox(hwnd, "Not checked!", "", MB_OK);
						else
							MessageBox(hwnd, "Checked!", "", MB_OK);

					}*/
					//TVITEM item;
					//item.hItem = TreeView_GetSelection(hwnd);
					//if (item.hItem != NULL) {
					//	MessageBox(hwnd, item.pszText, "", MB_OK);
					//}
					
					
					
				}
				break;
			}
		}
		/*switch (LOWORD(wParam))
		{
			case IDC_TVMAIN:
			{
				nmptr = (LPNMHDR)lParam;
				switch (nmptr->code)
				{
					case TVN_SELCHANGED:
					{
					stringstream ss;
					HTREEITEM tvi = ((LPNM_TREEVIEW)nmptr)->itemNew.hItem;
					if (tvi == TVI_MAIN) {
					ss << "Selected Item : Main";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_HSTATS) {
					ss << "Selected Item : Hub Statistics";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_APP) {
					ss << "Selected Item : Appearance";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_NTWRK) {
					ss << "Selected Item : Network";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_HLST) {
					ss << "Selected Item : Hublists";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_SETEX) {
					ss << "Selected Item : Expert Settings";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_USET) {
					ss << "Selected Item : User Settings";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
					else if (tvi == TVI_BOTS) {
					ss << "Selected Item : Bots";
					MessageBox(NULL, ss.str().c_str(), "Item Selection", MB_OK);
					}
				}
				break;
				}
			}
			break;
		}
		break;*/
	case WM_COMMAND:
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

DWORD ListTempFileInDrectory(LPTSTR szPath, HWND hwndTrV, HTREEITEM parent, int times, int inserted)
{
	TCHAR szFilePath[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	HTREEITEM tvpar2;
	//TCHAR szFullPath[MAX_PATH];

	_tcscpy(szFilePath, szPath);
	_tcscat(szFilePath, _T("\\*"));
	hListFile = FindFirstFile(szFilePath, &FindFileData);

	if (hListFile == INVALID_HANDLE_VALUE)
	{
		//printf("¿ù»~¡G%d", GetLastError());
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
			//dwTotalFileNum++;
			//_tprintf(_T("%s\n"), szFullPath);
			if (times == 1) {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//printf("<DIR>");
					ListTempFileInDrectory(szFullPath, hwndTrV, tvpar2, 2, 1);
				}
			}
			
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}


HTREEITEM AddItemToTree2(HWND hwndTrV, LPTSTR name, HTREEITEM hPrev, LPTSTR path, int expended) {
	
	TVITEM tvi;
	TVINSERTSTRUCT tvins;
	node *newnode = new node;
	newnode->AbsPath = path;
	if(expended == 1)newnode->Once = true;
	else newnode->Once = false;
	
	//_tcscpy(ptr->absolutepath, path);
	//_tcscat(ssave,path);
	//_tcscat(ssave,"\1");

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

	hPrev = (HTREEITEM)SendMessage(hwndTrV, TVM_INSERTITEM, 0,
		(LPARAM)(LPTVINSERTSTRUCT)&tvins);

	//free(newnode);
	return hPrev;
}

BOOL InitTreeViewImageLists(HWND hwndTV)
{
	HIMAGELIST himl;  // handle to image list 
	HBITMAP hbmp;     // handle to bitmap 

					  // Create the image list. 
	if ((himl = ImageList_Create(CX_BITMAP,	CY_BITMAP, FALSE,NUM_BITMAPS, 0)) == NULL)
		return FALSE;

	// Add the open file, closed file, and document bitmaps. 
	hbmp = (HBITMAP)LoadImage(g_hInst, _T("EYEROLL.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int g_nOpen = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = (HBITMAP)LoadImage(g_hInst, _T("CAT.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	int g_nClosed = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	/*hbmp = LoadBitmap(g_hInst, "EYEROLL.bmp");
	int g_nDocument = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);*/

	// Associate the image list with the tree-view control. 
	TreeView_SetImageList(hwndTV, himl, TVSIL_NORMAL);
	return TRUE;
}

BOOL InitTreeViewItems(HWND hwndTrV) 
{
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
