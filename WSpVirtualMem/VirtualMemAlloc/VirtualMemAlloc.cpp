// VirtualMemAlloc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#define INDEX_RANGE 40
#define PAGE_SIZE	4096
#define K			2

typedef struct{

	bool bUsed;
	BYTE bCompData[PAGE_SIZE / K - sizeof(bool)];

}COMPONENT, *PCOMPONENT;


#define RHANDLE_DMSG(hwnd, message, fn)    \
    case (message): return RHANDLE_##message((hwnd), (wParam), (lParam), (fn))

#define RHANDLE_WM_COMMAND(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), -1L)

#define RHANDLE_WM_INITDIALOG(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (HWND)(wParam), lParam), -1L)

#define RHANDLE_WM_PAINT(hwnd, wParam, lParam, fn) \
    ((fn)(hwnd), -1L)

#define RHANDLE_WM_DESTROY(hwnd, wParam, lParam, fn) \
    ((fn)(hwnd), -1L)

#define RHANDLE_WM_CLOSE(hwnd, wParam, lParam, fn) \
    ((fn)(hwnd), -1L)

//    (void)(fn)((hwnd), WM_DESTROY, 0L, 0L)

BOOL CALLBACK Dlg_Proc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam);
BOOL Dlg_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam);
BOOL Dlg_OnCommand(HWND hDlg,int id,HWND hwndCtl,UINT codeNotify);
BOOL Dlg_OnPaint(HWND hDlg );
BOOL Dlg_OnDestroy(HWND hDlg );
BOOL Dlg_OnClose(HWND hDlg );

void CollectDecommitted(PCOMPONENT pArrayComponent,DWORD dwIndexRange);


UINT uPageSize =  0;	//Real System Page Size
RECT rctMapWnd;

PCOMPONENT pArrayComponent = NULL;

HBRUSH hBrushWhite,hBrushBlue,hBrushRed;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
 	// TODO: Place code here.
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	uPageSize = sysinfo.dwPageSize;

	DialogBox(hInstance,(LPCTSTR)IDD_VM_ALLOC, NULL, Dlg_Proc);

	return 0;
}

BOOL CALLBACK Dlg_Proc(HWND hDlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		RHANDLE_DMSG(hDlg,WM_INITDIALOG, Dlg_OnInitDialog); //!!!
 		RHANDLE_DMSG(hDlg,WM_COMMAND,	 Dlg_OnCommand);		//!!!
		RHANDLE_DMSG(hDlg,WM_PAINT,		 Dlg_OnPaint);		//!!!
		RHANDLE_DMSG(hDlg,WM_CLOSE,		 Dlg_OnClose);		//!!!
 		RHANDLE_DMSG(hDlg,WM_DESTROY,	 Dlg_OnDestroy);		//!!!
		//return true; for all handled messages
		default:
	 	return FALSE;//not handled messages	

	}

}

BOOL Dlg_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam)
{
	hBrushWhite	= CreateSolidBrush(RGB(255,255,255));
	hBrushBlue	= CreateSolidBrush(RGB(0	 ,0  ,255));
	hBrushRed	= CreateSolidBrush(RGB(255,0  ,0  ));

//Get and Set up an instance icon for the window hDlg
	SendMessage(hDlg,WM_SETICON,TRUE,
				(LPARAM)LoadIcon((HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE),(LPCTSTR)IDI_ICON_VIRTUAL_MEM)
				);
	SendMessage(hDlg,WM_SETICON,FALSE,
				(LPARAM)LoadIcon((HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE),(LPCTSTR)IDI_ICON_VIRTUAL_MEM)
				);
//----Initialize---controls----------------------------------------//
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_RELEASE),FALSE);

	EnableWindow(GetDlgItem(hDlg,IDC_EDIT_INDEX),FALSE);
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE),FALSE);
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),FALSE);
	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_COLLECT),FALSE);

	//The dimensions are given in screen coordinates 
	//that are relative to the upper-left corner of the screen. 
	GetWindowRect(GetDlgItem(hDlg,IDC_STATIC_MAP_WND),&rctMapWnd);

	//converts (maps) a set of points from a coordinate space relative to one window
	//to a coordinate space relative to another window. 
	MapWindowPoints(NULL,hDlg,(LPPOINT)&rctMapWnd,2);//rectangle is defined with 2 points
 
	DestroyWindow(GetDlgItem(hDlg,IDC_STATIC_MAP_WND));

	//Get and Show System Page Size
	TCHAR szPageSize[20];
	wsprintf(szPageSize,TEXT("%d KB"),uPageSize/1024);
 	SetDlgItemText(hDlg,IDC_STATIC_PG_SIZE_VALUE,szPageSize);
//	SetWindowText(GetDlgItem(hDlg,IDC_STATIC_PG_SIZE_VALUE),szPageSize);

 
	//Initialize an array index with 0
	SetDlgItemInt(hDlg,IDC_EDIT_INDEX,0,FALSE);

	return TRUE;
}

BOOL Dlg_OnCommand(HWND hDlg,int id,HWND hwndCtl,UINT codeNotify)
{
UINT uCurIndex=0;
 switch(id)
 {
	case IDC_BUTTON_RESERVE_REGION:
 
		pArrayComponent = (PCOMPONENT) VirtualAlloc(NULL,
													INDEX_RANGE * sizeof(COMPONENT),
													MEM_RESERVE,
													PAGE_READWRITE);
 
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_RESERVE_REGION),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_RELEASE),TRUE);

		EnableWindow(GetDlgItem(hDlg,IDC_EDIT_INDEX)	,TRUE);

		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE)	,TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),TRUE);

		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_COLLECT),TRUE);

		SetFocus(GetDlgItem(hDlg,IDC_EDIT_INDEX));

		InvalidateRect(hDlg,&rctMapWnd,FALSE);
		return TRUE;
		break;

	case IDC_EDIT_INDEX:

		if(codeNotify!=EN_CHANGE)return TRUE;//on WM_INITDIALOG "zero" is set up to IDC_EDIT_INDEX

		//From "user" codeNotify == EN_CHANGE
		uCurIndex = GetDlgItemInt(hDlg,id,NULL,FALSE);

		if( pArrayComponent && (0<=uCurIndex)&&(uCurIndex<INDEX_RANGE) )
		{
			MEMORY_BASIC_INFORMATION mbinfo;

			VirtualQuery( &pArrayComponent[uCurIndex],&mbinfo,sizeof(mbinfo));

			bool bCommit = mbinfo.State == MEM_COMMIT;
			if (bCommit)	bCommit=pArrayComponent[uCurIndex].bUsed;

			//If not committed we can only	"use" IDC_BUTTON_USE
			//If  committed we can only		"use" IDC_BUTTON_DISPOSE
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE)	,!bCommit);
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE), bCommit);
		}
		else
		{			//Both are inaccessible!!
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE)	,FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),FALSE);

		}
		return TRUE;
		break;

	case IDC_BUTTON_USE:
		//Use the selected component of array.
		//Get its index. 
		uCurIndex = GetDlgItemInt(hDlg,IDC_EDIT_INDEX,NULL,FALSE);
		 
		if( (0<=uCurIndex)&&(uCurIndex<INDEX_RANGE) )
		{
			//It is on a page never committed!
			VirtualAlloc(&pArrayComponent[uCurIndex],
						sizeof(COMPONENT),
						MEM_COMMIT,
						PAGE_READWRITE);
			//Mark the component and use it...
			pArrayComponent[uCurIndex].bUsed = true;

			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE)	,FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),TRUE);

			SetFocus(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE));

			InvalidateRect(hDlg,&rctMapWnd,FALSE);			
		}
		return TRUE;
		break;

	case IDC_BUTTON_DISPOSE:
		//Dispose the selected component of array.
		//Get its index. 
		uCurIndex = GetDlgItemInt(hDlg,IDC_EDIT_INDEX,NULL,FALSE);
		 
		if( (0<=uCurIndex)&&(uCurIndex<INDEX_RANGE) )
		{
			//It is on a committed page!		
			//Mark the component as unused .
			pArrayComponent[uCurIndex].bUsed = FALSE;

			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE)	,TRUE);
			EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),FALSE);

			SetFocus(GetDlgItem(hDlg,IDC_BUTTON_USE));			 			
		}
		return TRUE;
		break;

	case IDC_BUTTON_COLLECT:
		CollectDecommitted(pArrayComponent,(DWORD)INDEX_RANGE);

		InvalidateRect(hDlg,&rctMapWnd,FALSE);	
		return TRUE;
		break;

	case IDC_BUTTON_RELEASE:

		if(!pArrayComponent)
		{
			MessageBox(NULL,"pArrayComponent=0","IDC_BUTTON_RELEASE",MB_OK);
			return TRUE;
		}
		 //pArrayComponent!=NULL
		if(!VirtualFree(pArrayComponent,0,MEM_RELEASE))
		{
			MessageBox(NULL,"MEMORY NOT RELEASED","IDC_BUTTON_RELEASE",MB_OK);
			return TRUE;
		}
		//Memory released!!!
		//----Initialize---controls----------------------------------------//
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_RESERVE_REGION),TRUE);

		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_RELEASE),FALSE);

		EnableWindow(GetDlgItem(hDlg,IDC_EDIT_INDEX),FALSE);
		SetDlgItemInt(hDlg,IDC_EDIT_INDEX,0,FALSE);//Before IDC_BUTTON_USE

		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_USE),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_DISPOSE),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_COLLECT),FALSE);
		//===================================================================//
		pArrayComponent=NULL; //No nead if unifying
		InvalidateRect(hDlg,&rctMapWnd,FALSE);
		MessageBox(NULL,"MEM_RELEASE","IDC_BUTTON_RELEASE",MB_OK);

		return TRUE;		
		break;

	default:
		return TRUE;	
 } 
}

BOOL Dlg_OnPaint(HWND hDlg )
{
	PAINTSTRUCT ps;

	UINT nPages		  = INDEX_RANGE * sizeof(COMPONENT)/uPageSize;
	UINT uMapWndWidth = rctMapWnd.right - rctMapWnd.left;
	UINT uResidue	  =	uMapWndWidth % nPages;
	UINT uPgRectWidth = uMapWndWidth/nPages;

	BeginPaint(hDlg,&ps);

	if(!pArrayComponent) for(UINT uPj = 0; uPj < nPages; uPj++)
		//It is better to unify both branches!
		{
		HBRUSH hOldBrush;	
		hOldBrush=SelectBrush(ps.hdc,hBrushWhite);		

		   Rectangle(ps.hdc,
						rctMapWnd.left +  uPgRectWidth * uPj,
						rctMapWnd.top,
						rctMapWnd.left +  uPgRectWidth * (uPj + 1),
						rctMapWnd.bottom);
	/***********************************************
	Rectangle(ps.hdc,rctMapWnd.left,rctMapWnd.top,rctMapWnd.right - uResidue,rctMapWnd.bottom);
	*************************************************/
		SelectBrush(ps.hdc,hOldBrush) ;
		}//to next page uPg
  
	else for(UINT uPg=0; uPg < nPages; uPg++)
	{	 
		UINT i0 = uPg * uPageSize / sizeof(COMPONENT);
		UINT m	=		uPageSize / sizeof(COMPONENT);

		for(UINT i=i0 ; i < i0+m; i++)
		{
			MEMORY_BASIC_INFORMATION mbinfo;

			VirtualQuery( &pArrayComponent[i],&mbinfo,sizeof(mbinfo));
			
			 HBRUSH hOldBrush;							//int nBrush = 0;
			switch(mbinfo.State)
			{
				case MEM_FREE		:hOldBrush=SelectBrush(ps.hdc,hBrushWhite);	break;//nBrush = WHITE_BRUSH;
				case MEM_RESERVE	:hOldBrush=SelectBrush(ps.hdc,hBrushBlue);	break;//nBrush = GRAY_BRUSH;
				case MEM_COMMIT		:hOldBrush=SelectBrush(ps.hdc,hBrushRed);	break;//nBrush = BLACK_BRUSH;
				default :	break;
			}
		   //Paint a page rectangle with hBrush*			 
														//SelectObject(ps.hdc, GetStockObject(nBrush));
		   Rectangle(ps.hdc,
						rctMapWnd.left +  uPgRectWidth * uPg,
						rctMapWnd.top,
						rctMapWnd.left +  uPgRectWidth * (uPg + 1),
						rctMapWnd.bottom);	
			SelectBrush(ps.hdc,hOldBrush) ;
		}		 
	} //to next page uPg	 

	EndPaint(hDlg,&ps);

	return TRUE;
}

BOOL Dlg_OnClose(HWND hDlg ) 
{
//if (End of Dialog){
	EndDialog(hDlg, 1);		 
	return	TRUE;
//}else return TRUE;   
}

BOOL Dlg_OnDestroy(HWND hDlg )
{
	DeleteObject(hBrushWhite);
	DeleteObject(hBrushBlue);
	DeleteObject(hBrushRed);
	if(pArrayComponent)
	{
		VirtualFree(pArrayComponent,0,MEM_RELEASE);

		MessageBox(NULL,"MEM_RELEASE","DESTROY",MB_OK);
		return TRUE;
	}
		MessageBox(NULL,"pArrayComponent=0","DESTROY",MB_OK);
		return TRUE;
}

void CollectDecommitted(PCOMPONENT pArrayComponent,DWORD dwIndexRange)
{
	UINT nPages = dwIndexRange * sizeof(COMPONENT)/uPageSize;

	for(UINT uPg=0; uPg < nPages; uPg++)
	{
		bool isAllocated = false;
		UINT i0 = uPg * uPageSize / sizeof(COMPONENT);
		UINT m	=		uPageSize / sizeof(COMPONENT);

		for(UINT i=i0 ; i < i0+m; i++)
		{
			MEMORY_BASIC_INFORMATION mbinfo;

			VirtualQuery( &pArrayComponent[i],&mbinfo,sizeof(mbinfo));

			bool bCommit = mbinfo.State == MEM_COMMIT;
			isAllocated  = bCommit && pArrayComponent[i].bUsed ;

			if(isAllocated) break; //Go to next page
		}
		if(!isAllocated)
		//No components used on this page! So decommit it.
		VirtualFree(&pArrayComponent[i0+m-1],(DWORD)sizeof(*pArrayComponent),MEM_DECOMMIT);
	}
	return;
}//CollectDecommitted