#if !defined(AFX_PAUSDLG_H__51DEF057_9D65_4921_B449_A15ED84177C4__INCLUDED_)
#define AFX_PAUSDLG_H__51DEF057_9D65_4921_B449_A15ED84177C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PausDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPausDlg dialog

class CPausDlg : public CDialog
{
// Construction
public:
	CPausDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPausDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPausDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPausDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAUSDLG_H__51DEF057_9D65_4921_B449_A15ED84177C4__INCLUDED_)
