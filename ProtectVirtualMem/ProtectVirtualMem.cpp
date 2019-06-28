// ProtectVirtualMem.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

#define ALLOC_MEM_BLOCK_SIZE 537 //NO ACTUAL REGION SIZE
LPVOID	lpMemBlock;

HMENU hProtectMenu;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
 
void WndProcOnPaint  (HWND hWnd); 
void WndProcOnDestroy(HWND hWnd); 
void WndProcOnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL WndProcOnCreate (HWND hWnd, LPCREATESTRUCT lpCreateStruct);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROTECTVIRTUALMEM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PROTECTVIRTUALMEM);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_PROTECTVIRTUALMEM);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_PROTECTVIRTUALMEM;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		HANDLE_MSG(hWnd, WM_CREATE,		WndProcOnCreate);	//! return -1;
		HANDLE_MSG(hWnd, WM_COMMAND,	WndProcOnCommand);	// return 0;	
		HANDLE_MSG(hWnd, WM_PAINT,		WndProcOnPaint); 	//! return 0;	 
		HANDLE_MSG(hWnd, WM_DESTROY,	WndProcOnDestroy); 	//! return 0;	
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
//   return 0;
}

BOOL WndProcOnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	TCHAR message[260];
	//Reserve a block with PAGE_NOACCESS
	LPVOID lpReserve = VirtualAlloc(NULL,ALLOC_MEM_BLOCK_SIZE,MEM_RESERVE,PAGE_NOACCESS);
	if(!lpReserve)
	{
		wsprintf(message,TEXT("MEM_RESERVE Error: %ld"),GetLastError());
		MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	MEMORY_BASIC_INFORMATION meminfo;

	VirtualQuery(lpReserve,&meminfo,sizeof(meminfo));
	lpMemBlock=VirtualAlloc(lpReserve,meminfo.RegionSize,MEM_COMMIT,PAGE_NOACCESS);

	if(!lpMemBlock)
	{
		wsprintf(message,TEXT("MEM_COMMIT Error: %ld"),GetLastError());
		MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	hProtectMenu=GetSubMenu(GetMenu(hWnd),1);	
	CheckMenuItem(hProtectMenu,IDM_PAGE_NOACCESS,MF_CHECKED);

	TCHAR szTextResCom[]=
		TEXT("Actual size of a reserved and committed region is ");

	wsprintf(message,TEXT("Region size asked for was %d\n%s %ld"),
		ALLOC_MEM_BLOCK_SIZE,szTextResCom,meminfo.RegionSize);

		MessageBox(hWnd,message,szTitle,MB_OK);

	return TRUE;

}//WndProcOnCreate

void WndProcOnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{	
	DWORD dwOldProtectMode;
	MEMORY_BASIC_INFORMATION meminfo;

// Parse the menu selections:
 switch (id)
 {
	 

	case IDM_PAGE_NOACCESS:

		VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		if(meminfo.Protect & PAGE_NOACCESS)	return;	

		VirtualProtect(lpMemBlock,meminfo.RegionSize ,PAGE_NOACCESS,&dwOldProtectMode);

		CheckMenuItem(hProtectMenu,IDM_PAGE_NOACCESS,				MF_CHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_READWRITE,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_READONLY,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_GUARD_PAGE_READWRITE,	MF_UNCHECKED);
	 break;

	 case IDM_PAGE_READWRITE:

		VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		if(meminfo.Protect & PAGE_READWRITE)	return;	

		VirtualProtect(lpMemBlock,meminfo.RegionSize ,PAGE_READWRITE,&dwOldProtectMode);

		CheckMenuItem(hProtectMenu,IDM_PAGE_NOACCESS,				MF_UNCHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_READWRITE,				MF_CHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_READONLY,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_GUARD_PAGE_READWRITE,	MF_UNCHECKED);
	 break;

	 case IDM_PAGE_READONLY:

		VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		if(meminfo.Protect & PAGE_READONLY)	return;	

		VirtualProtect(lpMemBlock,meminfo.RegionSize ,PAGE_READONLY,&dwOldProtectMode);

		CheckMenuItem(hProtectMenu,IDM_PAGE_NOACCESS,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_READWRITE,				MF_UNCHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_READONLY,				MF_CHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_GUARD_PAGE_READWRITE,	MF_UNCHECKED);
	 break;

	 case IDM_PAGE_GUARD_PAGE_READWRITE:

		VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		if((meminfo.Protect&(PAGE_GUARD|PAGE_READWRITE)) == (PAGE_GUARD|PAGE_READWRITE))
			return;	
		VirtualProtect(lpMemBlock,meminfo.RegionSize ,PAGE_GUARD|PAGE_READWRITE,&dwOldProtectMode);

		CheckMenuItem(hProtectMenu,IDM_PAGE_NOACCESS,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_READWRITE,				MF_UNCHECKED);
		CheckMenuItem(hProtectMenu,IDM_PAGE_READONLY,				MF_UNCHECKED);

		CheckMenuItem(hProtectMenu,IDM_PAGE_GUARD_PAGE_READWRITE,	MF_CHECKED);
	 break;
	 case IDM_WRITE:
		 __try {
			*(int *)lpMemBlock = 8;

			MessageBox(hWnd,TEXT("Writing 8 successful"),szTitle,MB_OK );
		 }
		__except (EXCEPTION_EXECUTE_HANDLER){
			TCHAR message[260];
			wsprintf(message,TEXT("EXCEPTION EXECUTE write Error: %ld"),GetLastError());
		    MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
			return;
		 }			
		 break;

	 case IDM_READ:
		 __try {                           //STATUS_GUARD_PAGE 
			int n = *(int *)lpMemBlock ;
			TCHAR message[260];
			wsprintf(message,TEXT("Reading %ld successful"),n);
			MessageBox(hWnd,message,szTitle,MB_OK );

		 }
		__except (EXCEPTION_EXECUTE_HANDLER){
			TCHAR message[260];
			wsprintf(message,TEXT("EXCEPTION EXECUTE read Error: %ld"),GetLastError());
		    MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
			return;
		 }
		 break;
	 case IDM_LOCK:

		 VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		 if(!VirtualLock(lpMemBlock,meminfo.RegionSize))
		 {
			 TCHAR message[260];
			wsprintf(message,TEXT("Virtual Lock Error: %ld"),GetLastError());
		    MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
			return;
		 }
		 MessageBox(hWnd,TEXT("Locking successful!"),szTitle,MB_OK );

		break;

		case IDM_UNLOCK:

		 VirtualQuery(lpMemBlock,&meminfo,sizeof(meminfo));
		 if(!VirtualUnlock(lpMemBlock,meminfo.RegionSize))
		 {
			 TCHAR message[260];
			wsprintf(message,TEXT("Virtual Unlock Error: %ld"),GetLastError());
		    MessageBox(hWnd,message,szTitle,MB_OK|MB_ICONEXCLAMATION);
			return;
		 }
		 MessageBox(hWnd,TEXT("Unlocking successful!"),szTitle,MB_OK );

		break;

	case IDM_ABOUT:
		DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	default:
	FORWARD_WM_COMMAND(hWnd, id, hwndCtl, codeNotify, DefWindowProc);
					   //DefWindowProc(hWnd, message, wParam, lParam);
 }

}//WndProcOnCommand

void WndProcOnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
		hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);

}//WndProcOnPaint
void WndProcOnDestroy(HWND hWnd)
{
	//Releas the memory block
 if(!lpMemBlock)
	VirtualFree(lpMemBlock,0,MEM_RELEASE);//If MEM_RELEASE , the second parameter must be zero. 
	PostQuitMessage(0);
}//WndProcOnDestroy

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
