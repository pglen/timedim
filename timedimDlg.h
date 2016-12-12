
/* =====[ timedimDlg.h ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the timedimDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/15/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// timedimDlg.h : header file
//

#if !defined(AFX_TIMEDIMDLG_H__E8F0F186_FFEA_4A69_A194_DAF6FCC2A053__INCLUDED_)
#define AFX_TIMEDIMDLG_H__E8F0F186_FFEA_4A69_A194_DAF6FCC2A053__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xbutton.h"
#include "Label.h"
#include "CFolderTreeCtrl.h"
#include "logwin.h"
#include "KCBusyProgressCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTimedimDlg dialog

class CTimedimDlg : public CDialog

{

public:
	void SetBusy(int flag);

	void	ShutDown(UINT command = 0);

	CLogWin	logwin;
	CLogWin	errwin;
	CLogWin	dirwin;

	SYSTEM_POWER_STATUS inisps;

	void	LoadSessionfile(const char *fname);
	void	CTimedimDlg::CopyBiDir(CFolderTreeCtrl &rm_tree, CFolderTreeCtrl &rm_tree2);
	int		FindFirstRemovable();
	void	ProgTextP(const char *Format, ...);
	void	ProgText(const char *str, int log = false);
	void	DirText(const char *str);
	void	ErrText(const char *str);
	int		CopyIf(const char *src, const char *dst);
	int		CopyRecurse(const char *src, const char *dst);

	void CTimedimDlg::ShutDownTimer() ;

	HCURSOR m_cursor;
	int		prompt, waitfortimer, validcomline;
	int		busy;	
	
	int		error, counter, fcounter, dcounter;
	
	volatile int stop;

	double	tdata;

	int		drag, drag_x, drag_y;
	int		focus;

	CBitmap  m_bitmap;				// Splash Bitmap
	int		m_nWidth, m_nHeight;	// Splash Bitmap's width & height

	void	OnOK();
	void	OnCancel();

	CTimedimDlg(CWnd* pParent = NULL);	// standard constructor

	RECT	closerect;
	RECT	minrect;
	int		devtimer;
	int		waitfordown;

	CString drive, remdir, oldstop; 

	int		pending;

	unsigned int logical;
	unsigned int sclock, eclock;  //start/end clock values

// Dialog Data
	//{{AFX_DATA(CTimedimDlg)
	enum { IDD = IDD_TIMEDIM_DIALOG };
	CLabel	m_progress_lab;
	CLabel	m_friend;
	CLabel	m_unc9;
	CLabel	m_unc8;
	CLabel	m_unc7;
	CLabel	m_unc6;
	CLabel	m_unc4;
	CLabel	m_unc5;
	CKCBusyProgressCtrl	m_bar;
	CLabel	m_rate;
	CxButton	m_b21;
	CxButton	m_b20;
	CxButton	m_b19;
	CxButton	m_b18;
	CxButton	m_b17;
	CxButton	m_mydoc11;
	CxButton	m_mydoc1;
	CxButton	m_home1;
	CLabel	m_lab;
	CxButton	m_b14;
	CxButton	m_b12;
	CxButton	m_b5;
	CxButton	m_b3;
	CxButton	m_b13;
	CxButton	m_b1;
	CxButton	m_b2;
	CxButton	m_save;
	CxButton	m_but9;
	CxButton	m_but8;
	CxButton	m_but7;
	CxButton	m_help;
	CxButton	m_but6;
	CxButton	m_but4;
	CxButton	m_but2;
	CxButton	m_but1;
	CLabel		m_prog2;
	CLabel		m_prog;
	CEdit		m_edit3;
	CEdit		m_edit2;
	CEdit		m_edit1;
	CLabel		m_header2;
	CLabel		m_labr;
	CLabel		m_labl;
	CFolderTreeCtrl	m_tree2;
	CFolderTreeCtrl	m_tree;
	CLabel	m_header;
	BOOL	m_check1;
	BOOL	m_check2;
	BOOL	m_check3;
	BOOL	m_check4;
	BOOL	m_check5;
	BOOL	m_check6;
	CTabCtrl	m_tab;
	BOOL	m_check7;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimedimDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTimedimDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButton1();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingTree2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton2();
	afx_msg void OnButton10();
	afx_msg void OnHelp();
	afx_msg void OnButton11();
	afx_msg void OnUnc4();
	afx_msg void OnButton9();
	afx_msg void OnButton4();
	afx_msg void OnButton3();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnCheck3();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButton22();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCheck7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMEDIMDLG_H__E8F0F186_FFEA_4A69_A194_DAF6FCC2A053__INCLUDED_)
