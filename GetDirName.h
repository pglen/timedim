#if !defined(AFX_GETDIRNAME_H__64F9572F_3364_4FEC_827D_5624CB33C4EA__INCLUDED_)
#define AFX_GETDIRNAME_H__64F9572F_3364_4FEC_827D_5624CB33C4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetDirName.h : header file
//

#include "label.h"

/////////////////////////////////////////////////////////////////////////////
// CGetDirName dialog

class CGetDirName : public CDialog
{
// Construction
public:
	CString txt;
	CGetDirName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetDirName)
	enum { IDD = IDD_DIALOG2 };
	CLabel	m_ptext;
	CString	m_dir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDirName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetDirName)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETDIRNAME_H__64F9572F_3364_4FEC_827D_5624CB33C4EA__INCLUDED_)
