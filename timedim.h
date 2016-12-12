// timedim.h : main header file for the TIMEDIM application
//

#if !defined(AFX_TIMEDIM_H__8939C397_0C94_468A_B87E_E02B268EB5A5__INCLUDED_)
#define AFX_TIMEDIM_H__8939C397_0C94_468A_B87E_E02B268EB5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "mxpad.h"
#include "xButton.h"

extern CString  approot;
extern CString	appdata;

#define ARGARRSIZE 12

#define MBOX	AfxMessageBox

// These variables should have been defined in some standard header but is not
#define WS_EX_LAYERED 0x00080000 
//#define LWA_COLORKEY 1 // Use color as the transparency color.
//#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes ;

extern	CString argarr[];
extern	CString dosession;

int     YieldToWinEx();
void	ShortenPath(CString &str, int len);
void	SplitPath(CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext);
int		IsDir(const char *str);

void	SplitPath(CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext);

int		ParseComandLine(const char *cmdline, CString *arr, int lim);
int		InterpretComandLine(CString *arr, int lim);

int     PrintToLog(const char *Format, ...);

BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);

void	open_datalog();
double	largeuint2double(ULARGE_INTEGER lint);
double	uint64todouble(__int64 int64val);
double	GetFreeDiskSpace(const char *drv);
int		heapcheck(const char *str, int showok);
void	addcommas(CString &str);

/////////////////////////////////////////////////////////////////////////////
// CTimedimApp:
// See timedim.cpp for the implementation of this class
//

class CTimedimApp : public CWinApp
{
public:
	CTimedimApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimedimApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTimedimApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEDIM_H__8939C397_0C94_468A_B87E_E02B268EB5A5__INCLUDED_)
