
/* =====[ timedimDlg.cpp ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the timedimDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/13/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "timedim.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <math.h>

#include "pausdlg.h"
#include "timedimDlg.h"
#include "FileDialogST.h"
#include "copyfile.h"
#include "logdlg.h"
#include "FileDialogST.h"
#include "label.h"
#include "usbdrive.h"
#include "support.h"
#include "dbt.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OFLOWLINES		200
#define ERROFLOWLINES	2000

char *idlemsg = "TimeDim is Idle.";

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes = NULL;

CCopyFile	copyfile;
CLogDlg		logdlg;
CString		dosession;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:

	int	first;

	CAboutDlg();

	CBitmap  m_bitmap;			// Splash Bitmap
	int m_nWidth, m_nHeight;	// Splash Bitmap's width & height

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CxButton	m_ok;
	CLabel	m_ver;
	CLabel	m_cr;
	CLabel	m_head;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnButton1();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg about;


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	first = true;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_ok);
	DDX_Control(pDX, IDC_VER, m_ver);
	DDX_Control(pDX, IDC_CR, m_cr);
	DDX_Control(pDX, IDC_HEAD, m_head);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_ENTERIDLE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimedimDlg dialog

CTimedimDlg::CTimedimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimedimDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimedimDlg)
	m_check1 = FALSE;
	m_check2 = FALSE;
	m_check3 = FALSE;
	m_check4 = FALSE;
	m_check5 = FALSE;
	m_check6 = FALSE;
	m_check7 = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	drag = 0;
	focus = 0;
	busy = false;
	error = 0; counter = 0;	fcounter = 0; dcounter = 0; stop = 0;
	validcomline = 0;
	waitfortimer = false;
	devtimer = 0;
	pending = 0;
	tdata = 0;
	waitfordown = 0;
}

void CTimedimDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimedimDlg)
	DDX_Control(pDX, IDC_UNC11, m_progress_lab);
	DDX_Control(pDX, IDC_UNC10, m_friend);
	DDX_Control(pDX, IDC_UNC9, m_unc9);
	DDX_Control(pDX, IDC_UNC8, m_unc8);
	DDX_Control(pDX, IDC_UNC7, m_unc7);
	DDX_Control(pDX, IDC_UNC6, m_unc6);
	DDX_Control(pDX, IDC_UNC4, m_unc4);
	DDX_Control(pDX, IDC_UNC5, m_unc5);
	DDX_Control(pDX, IDC_BAR, m_bar);
	DDX_Control(pDX, IDC_RATE, m_rate);
	DDX_Control(pDX, IDC_BUTTON21, m_b21);
	DDX_Control(pDX, IDC_BUTTON20, m_b20);
	DDX_Control(pDX, IDC_BUTTON19, m_b19);
	DDX_Control(pDX, IDC_BUTTON18, m_b18);
	DDX_Control(pDX, IDC_BUTTON17, m_b17);
	DDX_Control(pDX, IDC_BUTTON16, m_mydoc11);
	DDX_Control(pDX, IDC_BUTTON15, m_mydoc1);
	DDX_Control(pDX, IDC_LAB, m_lab);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, ID_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_BUTTON10, m_b1);
	DDX_Control(pDX, IDC_BUTTON11, m_b2);
	DDX_Control(pDX, IDC_BUTTON9, m_but9);
	DDX_Control(pDX, IDC_BUTTON8, m_but8);
	DDX_Control(pDX, IDC_BUTTON7, m_but7);
	DDX_Control(pDX, ID_HELP, m_help);
	DDX_Control(pDX, IDC_BUTTON6, m_but6);
	DDX_Control(pDX, IDC_BUTTON4, m_but4);
	DDX_Control(pDX, IDC_BUTTON2, m_but2);
	DDX_Control(pDX, IDC_BUTTON1, m_but1);
	DDX_Control(pDX, IDC_PROG2, m_prog2);
	DDX_Control(pDX, IDC_PROG, m_prog);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_HEADER2, m_header2);
	DDX_Control(pDX, IDC_LABR, m_labr);
	DDX_Control(pDX, IDC_LABL, m_labl);
	DDX_Control(pDX, IDC_TREE2, m_tree2);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_HEADER, m_header);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
	DDX_Check(pDX, IDC_CHECK2, m_check2);
	DDX_Check(pDX, IDC_CHECK3, m_check3);
	DDX_Check(pDX, IDC_CHECK4, m_check4);
	DDX_Check(pDX, IDC_CHECK5, m_check5);
	DDX_Check(pDX, IDC_CHECK6, m_check6);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Check(pDX, IDC_CHECK7, m_check7);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTimedimDlg, CDialog)
	//{{AFX_MSG_MAP(CTimedimDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SETCURSOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab1)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree1)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE2, OnItemexpandingTree2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_UNC4, OnUnc4)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(ID_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSKEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimedimDlg message handlers

BOOL CTimedimDlg::OnInitDialog()

{
	CDialog::OnInitDialog();

	SetBusy(true);  

	// Estabilish initial set of logical drives;
	logical = GetLogicalDrives();

	BYTE	bAlpha = 240;

	m_cursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	ASSERT(m_cursor);

	GetSystemPowerStatus(&inisps);
			
	m_but8.EnableWindow(false);
	m_but9.EnableWindow(false);

	//logwin.CreateFrom(this, IDC_LOGWIN);
	logwin.CreateFrom(this, IDC_EDIT2);
	errwin.CreateFrom(this, IDC_EDIT1);
	dirwin.CreateFrom(this, IDC_EDIT3);

	m_bar.SetColBlockFace(RGB(200,200,200));
	m_bar.SetColBlockFaceHi(RGB(230,230,230));
	m_bar.SetColBlockEdge(RGB(100,100,100));
	m_bar.SetColBlockEdgeHi(RGB(150,150,150));

	m_bar.SetNumSteps(20);
	m_bar.SetMode(BPC_MODE_PROGRESS);

#if 0
	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}
#endif

	SetWindowText("Time Dimensional Backup");

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%d/%m/%Y  %H:%M");		

	C2N;
	P2N("Started Timedim App at %s\r\n", datestr);

	m_bitmap.LoadBitmap(IDB_BITMAP7);
	
	BITMAP BitMap;
	m_bitmap.GetBitmap(&BitMap);
	m_nWidth = BitMap.bmWidth;
	m_nHeight = BitMap.bmHeight;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//m_unc.SetTrans();
	//m_unc2.SetTrans();
	//m_unc3.SetTrans();
	m_unc4.SetTrans();
	m_unc5.SetTrans();
	m_unc6.SetTrans();
	m_unc7.SetTrans();
	m_unc8.SetTrans();
	m_unc9.SetTrans();
	m_friend.SetTrans();

	m_rate.SetTrans();
	//m_rate.CenterText(true);
	
	m_lab.SetTrans();
	m_lab.SetText(idlemsg);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//m_header.SetText("Timedin APP");

	m_header.SetTextColor(RGB(0, 0, 0));
	m_header.SetFontSize(28);
	m_header.SetFontBold(true);
	m_header.SetTrans();
	
	m_header2.SetTextColor(RGB(220, 220, 220));
	m_header2.SetFontSize(28);
	m_header2.SetFontBold(true);
	m_header2.SetTrans();

	// Disabled shadow
	m_header2.ShowWindow(false);

	m_labr.SetTrans();
	m_labr.SetFontBold(true);

	m_labl.SetTrans();
	m_labl.SetFontBold(true);

	//m_prog.SetFontBold(true);
	m_prog.SetTrans();

	m_prog2.SetTrans();
	//m_prog2.SetFontBold(true);
	
	m_tree.Init();
	m_tree.result = &m_prog; 
	m_tree.prog = &m_lab;
	
	m_tree2.Init();
	m_tree2.result = &m_prog2;
	m_tree2.prog = &m_lab;

	m_mydoc11.SetToolTipText("Jump to User's Home Directory");
	m_mydoc1.SetToolTipText("Jump to User's 'My Documents' Directory");

	m_b17.SetToolTipText("Jump to User's Home Directory");
	m_b19.SetToolTipText("Jump to User's 'My Documents' Directory");

	m_b20.SetToolTipText("Jump to next USB removable drive.");
	m_b21.SetToolTipText("Jump to next USB removable drive.");

	m_but7.SetToolTipText("Clear progress Window.(Shift Click to Clear All)");
	m_b14.SetToolTipText("Show Activity Log.");

	m_but1.SetToolTipText("Backup files. (from Left to Right)");
	m_but2.SetToolTipText("Restore files. (from Right to Left)");

	m_but8.SetToolTipText("Stop Copy Progress (3 times to abort on long file)");
	m_but9.SetToolTipText("Pause Copy. Will resume from point of pause.");

	// Align sub windows to tab
	WINDOWPLACEMENT wp;
	m_tab.GetWindowPlacement(&wp);

	m_edit1.ShowWindow(false);
	errwin.SetWindowPos(NULL, wp.rcNormalPosition.left + 4,  wp.rcNormalPosition.top + 25, 
				wp.rcNormalPosition.right - wp.rcNormalPosition.left - 6, 
					wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - 27, SWP_NOZORDER );

	//m_edit2.SetWindowPos(NULL, wp.rcNormalPosition.left + 4,  wp.rcNormalPosition.top + 25, 0, 0, SWP_NOZORDER  | SWP_NOSIZE  );
	m_edit2.ShowWindow(false);

	logwin.SetWindowPos(NULL, wp.rcNormalPosition.left + 4,  wp.rcNormalPosition.top + 25, 
		wp.rcNormalPosition.right - wp.rcNormalPosition.left - 6, 
			wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - 27, SWP_NOZORDER );

	m_edit3.ShowWindow(false);
	dirwin.SetWindowPos(NULL, wp.rcNormalPosition.left + 4,  wp.rcNormalPosition.top + 25, 
				wp.rcNormalPosition.right - wp.rcNormalPosition.left - 6, 
					wp.rcNormalPosition.bottom - wp.rcNormalPosition.top - 27, SWP_NOZORDER );
	

	//Invalidate();

	SetTimer(1, 333, NULL);

	//CTabCtrl
	TCITEM TabCtrlItem;
	
	TabCtrlItem.pszText = "Progress";
	//TabCtrlItem.lParam = (long) &m_edit2;
	TabCtrlItem.lParam = (long) &logwin;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;

	m_tab.InsertItem(0, &TabCtrlItem );

	TabCtrlItem.pszText = "Errors";
	//TabCtrlItem.lParam = (long)&m_edit1;
	TabCtrlItem.lParam = (long)&errwin;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;

	m_tab.InsertItem(1, &TabCtrlItem );

	m_tab.SetCurSel(0);
	m_edit1.ShowWindow(SW_HIDE);

	TabCtrlItem.pszText = "Directories";
	//TabCtrlItem.lParam = (long)&m_edit3;
	TabCtrlItem.lParam = (long)&dirwin;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;
	m_tab.InsertItem(2, &TabCtrlItem );
	
	m_tab.SetCurSel(0);
	logwin.ShowWindow(SW_SHOW);
	errwin.ShowWindow(SW_HIDE);
	dirwin.ShowWindow(SW_HIDE);

	SetWindowText("Time Dimensional Backup");

	copyfile.lab = &m_lab;
	copyfile.bar = &m_bar;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTimedimDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
		}
	else
		{
		CDialog::OnSysCommand(nID, lParam);
		}
}

void CTimedimDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTimedimDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
	
	CPaintDC dc(this); // device context for painting

	int		loop = 0;

	//P2N("Paint DrawPal dialog\r\n");
	//int start = m_sb.GetScrollPos();

	//
	//  ----------------------------
	// |                          x |
	// |                            |
	// |                            |
	//  ----------------------------
	//

	CPen Pen2;	Pen2.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&Pen2);

	// Vert
	RECT	rc3;
	GetClientRect(&rc3);

	closerect.top = rc3.top + 4;
	closerect.left = rc3.right - 12;
	closerect.bottom = closerect.top + 8;
	closerect.right = closerect.left + 8;

	dc.MoveTo(closerect.left, closerect.top);
	dc.LineTo(closerect.right,  closerect.bottom);

	dc.MoveTo(closerect.left, closerect.bottom );
	dc.LineTo(closerect.right, closerect.top);

	minrect.top = rc3.top + 4;
	minrect.left = rc3.right - 28;
	minrect.bottom = minrect.top + 8;
	minrect.right = minrect.left + 8;
	
	dc.MoveTo(minrect.left, minrect.bottom );
	dc.LineTo(minrect.right, minrect.bottom);

	// Free resource(s)
	dc.SelectObject(pOldPen);

	// Do not call CDialog::OnPaint() for painting messages
	//CDialog::OnPaint();

	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//DEL HCURSOR CTimedimDlg::OnQueryDragIcon()
//DEL {
//DEL 	return (HCURSOR) m_hIcon;
//DEL }

void CTimedimDlg::OnCancel()
{

}

void CTimedimDlg::OnButton5() 
{
	EndDialog(IDOK);	
}

void CTimedimDlg::OnOK() 

{
	CDialog::OnOK();
}

void CTimedimDlg::OnButton6() 
{
	EndDialog(IDOK);		
}

void CTimedimDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	//PrintToNotepad("OnMouseMove TransDlg\r\n");
	RECT rc;
	
	//if(!focus)
	//	{
	//	goto endd;
	//	}

	if(drag)
		{
		GetWindowRect(&rc);
				SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
						(point.y + rc.top) - drag_y, 0, 0, 
								SWP_NOSIZE | SWP_NOZORDER ) ;  
		goto endd;
		}

	// Moved?
	if(drag_x != point.x || drag_y != point.y)
		{
		// Is drag treshold?

		if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
			{
			//P2N("Moved\r\n");

			drag_x = point.x;
			drag_y = point.y;

			// Button?
			if(nFlags & MK_LBUTTON)
				{
				SetCapture();
				drag = 1;
				}
			}
		goto endd;
		}

endd:
	CDialog::OnMouseMove(nFlags, point);
}

void CTimedimDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect cr(closerect);

	cr.InflateRect(5,5);

	// Cloxing on 'x'
	if(cr.PtInRect(point))
		{
		//HideAll();
		//MBOX("Close");
		if(busy)
			{
			ProgText("Cannot exit while busy\r\n");
			}
		else
			{
			EndDialog(IDOK);
			}	
		return;
		}

	// Cloxing on '_'
	CRect cr2(minrect);

	cr2.InflateRect(5,5);

	// Cloxing on 'x'
	if(cr2.PtInRect(point))
		{
		//AfxMessageBox("Min");
	
	    WINDOWPLACEMENT wp2;
	    GetWindowPlacement(&wp2);

		wp2.showCmd = SW_MINIMIZE ;
		SetWindowPlacement(&wp2);
		}

	drag_x = point.x; drag_y = point.y;

	//PrintToNotepad("Mouse down: Drag_x=%d drag_y=%d\r\n", drag_x, drag_y);
	focus = true;
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CTimedimDlg::OnLButtonUp(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		}

	//PrintToNotepad("Mouse up\r\n", drag_x, drag_y);
	
	CDialog::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnTimer(UINT nIDEvent) 

{
	CString tmp; 

	// Kill all except 100
	if(nIDEvent != 100)
		KillTimer(nIDEvent);

	if(nIDEvent == 1)
		{
		ProgText("Scanning Source Tree ... ");

		m_tree.AddAllDrive();
		m_tree.Invalidate();

		ProgText("OK, Scanning  Target Tree ... ");

		m_tree2.AddAllDrive();
		m_tree2.Invalidate();
		
		ProgText("OK\r\n");

		SetBusy(false);
		
		// Verify log file size
		struct stat buffer;	
		CString logname(appdata); 	logname += "activity.txt";
				
		if(stat(logname, &buffer) >= 0)
			{
			if(buffer.st_size > 50000)
				{
				CTime ct3 = buffer.st_mtime;
				CString datestr3 = ct3.Format("activity-%02d%02m%y.txt");
				
				CString newname(appdata); newname += datestr3;
				
				//P2N("Created backup datalog file %s\r\n", newname);
				rename(logname, newname);
				}
			}

		// Open data log file
		open_datalog();

		// Instructions from command line 
		ParseComandLine(AfxGetApp()->m_lpCmdLine, argarr, ARGARRSIZE);

		int direrr = false;

		InterpretComandLine(argarr, ARGARRSIZE);

		if(dosession != "")
			{
			struct stat buff;

			P2N("Session file from command line %s\r\n", dosession);

			PrintToLog("Session file from command line %s\r\n", dosession);

			// correct file name to appdata
			if(stat(dosession, &buff) < 0)
				{
				dosession = appdata + dosession;
				P2N("Session file from command line2 %s\r\n", dosession);
				if(stat(dosession, &buff) < 0)
					{
					MBOX("Session file does not exist. (comline)");
					PrintToLog("Session file does not exist. (comline)\r\n");
					}
				else
					{
					LoadSessionfile(dosession);

					tmp.Format("Command line initiated session backup of '%s' to '%s'\r\n", 
					m_tree.m_result, m_tree2.m_result);
					ProgText(tmp); PrintToLog(tmp);

					goto warp;
					}
				}
			}
		else
			{		
			if(argarr[0] != "" && argarr[1] != "")
				{
				//P2N("Command line: indir='%s' outdir='%s'\r\n", argarr[0], argarr[1]);

				if(!IsDir(argarr[0]))
					{
					//MBOX("Cannot stat source dir");
					ProgTextP("Cannot stat source dir from command line '%s'\r\n", argarr[0]);
					PrintToLog("Cannot stat source dir from command line '%s'\r\n", argarr[0]);

					direrr = true;
					}

				if(!IsDir(argarr[1]))
					{
					//MBOX("Cannot stat target dir");
					ProgTextP("Cannot stat target dir from command line '%s'\r\n", argarr[1]);
					PrintToLog("Cannot stat target dir from command line '%s'\r\n", argarr[1]);

					direrr = true;
					}
				
				if(!direrr)
					{
					// Repair backslash at the ends
					if(argarr[0].Right(1) != '\\')
						argarr[0] += "\\";

					if(argarr[1].Right(1) != '\\')
						argarr[1] += "\\";

					m_tree.EnableWindow(false);
					m_tree2.EnableWindow(false);

					P2N("Preset from command line %s -- %s\r\n", argarr[0], argarr[1]);
				
					m_prog.SetText(argarr[0]);
					m_prog2.SetText(argarr[1]);

					m_tree.m_result = argarr[0];
					m_tree2.m_result = argarr[1];
					
					SetBusy(true);
					m_tree.SetCurr(argarr[0]);
					m_tree2.SetCurr(argarr[1]);
					SetBusy(false);
		
					UpdateData(false);
					YieldToWinEx();
					//UpdateData();
			
					tmp.Format("Command line initiated backup of '%s' to '%s'\r\n", 
					m_tree.m_result, m_tree2.m_result);
					ProgText(tmp); PrintToLog(tmp);


warp:
					ProgText("Starting copy in 5 seconds, press the 'Stop Copy' button to cancel ...\r\n");

					waitfortimer = true;
					validcomline = true;

					m_tree.EnableWindow(false);
					m_tree2.EnableWindow(false);

					m_but8.EnableWindow(true);
					m_but9.EnableWindow(true);

					m_but1.EnableWindow(false);
					m_but2.EnableWindow(false);
					m_but4.EnableWindow(false);
					m_but6.EnableWindow(false);

					m_but8.EnableWindow(true);
			
					SetTimer(2, 5000, NULL);			
					m_tree.SetFocus();
					m_tree.SetActiveWindow();
					}
				else
					{
					ProgText("Ready.\r\n");
					m_lab.SetText(idlemsg);
					//m_tree.SetFocus();
					//m_tree.Invalidate();
					//m_tree.SetActiveWindow();

					}
				}
			}
		
		//P2N("indir='%s' outdir='%s'\r\n", indir, outdir);
		}

	//////////////////////////////////////////////////////////////////////

	if(nIDEvent == 2)
		{
		waitfortimer = false;
		if(stop)
			{
			m_but8.EnableWindow(false);
			m_tree.EnableWindow(true);
			m_tree2.EnableWindow(true);

			ProgText("Aborted command line initiated copy ...\r\n");
			}
		else
			{
			CopyBiDir(m_tree, m_tree2);

			m_but1.EnableWindow(true);
			m_but2.EnableWindow(true);
			m_but4.EnableWindow(true);
			m_but6.EnableWindow(true);

			m_but8.EnableWindow(true);

			if(!stop)
				{
				// Just in case the user was curiuos
				if(::IsWindow(about.m_hWnd))
					about.EndDialog(IDOK);

				if(::IsWindow(logdlg.m_hWnd))
					logdlg.EndDialog(IDOK);
				
				ProgText("Finished copy ...  Exiting in 5 seconds ...\r\n");
				SetTimer(3, 5000, NULL);			
				}
			}
		}

	if(nIDEvent == 3)
		{
		EndDialog(IDOK);
		}

	if(nIDEvent == 4)
		{
		PrintToLog("Shutting down system.\r\n");
		ShutDown();
		}

	if(nIDEvent == 99)
		{
		unsigned int newlog = GetLogicalDrives();
		if(logical != newlog)
			{
			unsigned int logical2 = logical;
			logical = newlog;

			unsigned int changed = logical2 ^ newlog, mask = 1, loop; 
			
			for(loop = 0; loop < 32; loop ++)
				{
				if((mask << loop) & changed) 
					{
					if(logical2 < newlog) 
						remdir = "added";
					else
						remdir = "removed";
					
					drive.Format("%c:\\", loop + 'A');

					//P2N("found bit at %d %s %s\r\n", loop, drive, remdir);
					break;
					}
				}

			P2N("Logical drive change %x %x %x drive %s %s\r\n", 
					logical, newlog, logical ^ newlog, drive, remdir );

			if(busy) 
				{
				pending = true;
				P2N("Pending logical drive scan for later\r\n");
				}
			else
				{
				ProgTextP("The system %s logical drive %s\r\n", remdir, drive);
				m_tree.AddAllDrive(); m_tree2.AddAllDrive();
				}
			}
		devtimer = false;
		}

	if(nIDEvent == 100)	
		{
		//P2N("Speed update timer\r\n");

		double rate = 0, frate = 0;
		unsigned int currc = clock();
		if(currc - copyfile.startc > 0)
			{
			CString tmp, tmp1, tmp2;
			frate = (copyfile.tdata / 1000) /  ((double)(currc - copyfile.startc) / CLOCKS_PER_SEC) ;
			rate = (tdata / 1000) /  ((double)(currc - sclock) / CLOCKS_PER_SEC) ;
			
			tmp1.Format("%.0f", frate); addcommas(tmp1);
			tmp2.Format("%.0f", rate);	addcommas(tmp2);

			//P2N("frate %s rate %s\r\n", tmp1, tmp2);

			tmp.Format("File: %s kBytes/s  Avg: %s kBytes/s", tmp1, tmp2);
			m_rate.SetText(tmp);				
			}
		}
	
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

BOOL CTimedimDlg::OnEraseBkgnd(CDC* pDC) 

{
	RECT rc; GetClientRect(&rc);

	CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
	
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	CClientDC dc(this);

	//AlphaBlend(dc, 0,0, m_nWidth, m_nHeight, dcMem, 0,0,m_nWidth, m_nHeight, m_bf); 

	int loop, loopv;
	m_nWidth, m_nHeight;

	for(loop = 0; loop < rc.right; loop += m_nWidth)
		for(loopv = 0; loopv < rc.bottom; loopv += m_nHeight)
			pDC->BitBlt(loop, loopv, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY ); 		

	dcMem.SelectObject(pOldBitmap);

	// We handled it
	return(false);
}


//////////////////////////////////////////////////////////////////////////

void	Callback(const char *ptr)

{
	P2N("got %s\r\n", ptr);
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Function name:    double2ascii
// Description:      Convert a double to a string. Add commas as convetional. 
// Return type:      none
// Argument:         double num
// Argument:         CString &str
// Argument:         int prec = 2
// Argument:         int commas = false
// Argument:         int radix = 10
//
//////////////////////////////////////////////////////////////////////////

void	double2ascii(double num, CString &str, int prec = 2, int commas = false, int radix = 10)

{
	CString tmp;
	CString fra;

	str = "";				// init string for append operation

	int		prog = 0, digits = 0;
	double	intnum, frac, div = radix;

	// Convert fractional part
	frac = modf(num, &intnum);

#if 0
	// Fractianl part
	while(true)
		{
		double intnum;
		modf(frac/div, &intnum);

		fra += (char)(num - intnum * 10) + '0';

		P2N("num %d %c\r\n", (char)(num - intnum * 10),  (char)(num - intnum * 10) + '0');

		fra = intnum;

		if(fra == 0)
			break;
		}
#endif


	//Whole part
	while(true)
		{
		double intnum;
		modf(num/div, &intnum);

		if(digits++ % 3 == 0  && digits > 1 && commas)
			tmp +=  ',';

		char diff = (char)(num - intnum * div);

		if(diff > 10)
			diff += 'a';
		else
			diff += '0';

		tmp += diff;
		
		P2N("num %d %c\r\n", diff,  diff);

		num = intnum;

		if(num == 0)
			break;
		}

	int loop2 = 0;
	int len = tmp.GetLength();

	// Reverse string
	for(int loop = 0; loop < len ; loop++)
		{
		str += tmp[len - 1 - loop];
		}

}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnButton1() 

{
	CString str;
	
	double2ascii(11, str, 3, true, 16);
	P2N("doubleret='%s'\r\n", str);

	//CString str2("654321");	
	//addcommas(str2);
	//P2N("commaret='%s'\r\n", str2);

	CopyBiDir(m_tree, m_tree2);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::CopyBiDir(CFolderTreeCtrl &rm_tree, CFolderTreeCtrl &rm_tree2) 

{
	CString tmp; tmp.Format("Initiated backup of '%s' to '%s'\r\n", 
			rm_tree.m_result, rm_tree2.m_result);

	ProgText(tmp); PrintToLog(tmp);
	
	// Re - Arm
	stop = false;

	SetBusy(true);

	counter = 0; fcounter = 0; dcounter = 0;
	tdata = 0;
	sclock = clock();

	SetTimer(100, 1000, NULL);

	// CWnd
	rm_tree.EnableWindow(false);
	rm_tree2.EnableWindow(false);

	m_but8.EnableWindow(true);
	m_but9.EnableWindow(true);

	m_but1.EnableWindow(false);
	m_but2.EnableWindow(false);
	//m_but3.EnableWindow(false);
	m_but4.EnableWindow(false);
	m_but6.EnableWindow(false);
	
	m_b12.EnableWindow(false);
	m_b13.EnableWindow(false);
	m_b14.EnableWindow(false);
	m_help.EnableWindow(false);

	m_mydoc1.EnableWindow(false);
	m_mydoc11.EnableWindow(false);
	m_b17.EnableWindow(false);
	m_b19.EnableWindow(false);
	m_b20.EnableWindow(false);
	m_b21.EnableWindow(false);

	GetDlgItem(IDC_CHECK1)->EnableWindow(false);
	GetDlgItem(IDC_CHECK2)->EnableWindow(false);
	GetDlgItem(IDC_CHECK3)->EnableWindow(false);
	GetDlgItem(IDC_CHECK4)->EnableWindow(false);
	GetDlgItem(IDC_CHECK5)->EnableWindow(false);
	//GetDlgItem(IDC_CHECK7)->EnableWindow(false);

	m_unc4.EnableWindow(false); 	m_unc5.EnableWindow(false);
	m_unc6.EnableWindow(false);		m_unc7.EnableWindow(false);
	m_unc8.EnableWindow(false);		m_unc9.EnableWindow(false);
	m_friend.EnableWindow(false);


//	m_save.EnableWindow(false);
//	m_help.EnableWindow(false);

	// CString
	const char *ptr1 = rm_tree.m_result;
	const char *ptr2 = rm_tree2.m_result;

	int	len1 = rm_tree.m_result.GetLength();
	int	len2 = rm_tree2.m_result.GetLength();

	int loop = 0;

	// Check conditions	
	UpdateData();	

	if(rm_tree.m_result == "")
		{
		MBOX("Please select a SOURCE path.");
		goto endd;
		}

	if((rm_tree.m_result == "" || rm_tree2.m_result == "") && !m_check5)
		{
		MBOX("Please select both SOURCE and TARGET path.");
		goto endd;
		}

	if((rm_tree.m_result == rm_tree2.m_result) && !m_check5)
		{
		MBOX("Source and Target are identical. Cannot backup.");
		PrintToLog("Source and Target are identical. Cannot backup.\r\n");
		goto endd;
		}

	for(loop = 0; loop < len1; loop++)
		{
		if(loop >= len1)
			break;

		if(loop >= len2)
			break;

		if(ptr1[loop] != ptr2[loop])
			break;
		}

	if(loop >= len1 && !m_check5)
		{
		CString str = "Source and Target are nested, may create unintentional looping.\r\n"
			"Are you sure you want to continue?";

		if(MBOX(str, MB_YESNO) == IDNO)
			{
			goto endd;
			}

		PrintToLog("Source and Target are nested, user override.\r\n");
		//goto endd;
		}

	if((!IsDir(rm_tree.m_result) || !IsDir(rm_tree2.m_result)) && !m_check5)
		{
		MBOX("Both Source and Target directory must exist. Cannot backup.");
		PrintToLog("Both Source and Target directory must exist. Cannot backup.");		
		goto endd;
		}

	if(m_check4 && m_check3)
		{
		MBOX("Both 'simulate' and 'verify' options are active, unchecing 'verify'");
		m_check4 = false;
		UpdateData(false);
		ProgText("Unchecked 'Verify' option.\r\n");
		}

	if(m_check2 && m_check3)
		{
		MBOX("Both 'simulate' and 'backup preexisting' options are active, unchecking 'backup preexisting'");
		m_check2 = false;
		UpdateData(false);
		ProgText("Unchecked 'Backup Preexisting' option.\r\n");
		}

	// Evaluate check boxes
	if(m_check1)
		{
		CString str, backstr;
		
		if(m_check2)
			backstr = " (with old files are backed up to *.tdm.ddmmyy) ";
			
		str.Format(" About to copy %s:\r\n\r\n'%s'   ==>   '%s'", 
				backstr, rm_tree.m_result, rm_tree2.m_result);
		
		if(MBOX(str, MB_YESNO) == IDNO)
			{
			goto endd;
			}
		}

	if(m_check4)
		{
		P2N("Verify is ON\r\n");
		ProgText("Verify is ON, verifying copied files\r\n");
		}

	if(m_check5)
		{
		if(m_check4 && m_check5)
			{
			MBOX("Both 'Test Readability' and 'Verify' options are active, unchecking 'Verify' option");
			m_check4 = false;
			UpdateData(false);
			ProgText("Unchecked 'Verify' option.\r\n");
			}

		P2N("test Read is ON\r\n");

		ProgText("Test Readability is ON, verifying file readabilty (bad sector scan)\r\n");
		}
	
	P2N("Started Iterate.\r\n");
	//m_edit2.ReplaceSel("Started backup of ...\r\n");
	//m_bar.Start();

	CopyRecurse(rm_tree.m_result, rm_tree2.m_result);

endd:

	UpdateData();

	m_unc4.EnableWindow(true);
	m_unc5.EnableWindow(true);
	m_unc6.EnableWindow(true);
	m_unc7.EnableWindow(true);
	m_unc8.EnableWindow(true);
	m_unc9.EnableWindow(true);
	m_friend.EnableWindow(true);

	m_unc4.SetTrans();
	m_unc5.SetTrans();
	m_unc6.SetTrans();
	m_unc7.SetTrans();
	m_unc8.SetTrans();
	m_unc9.SetTrans();
	m_friend.SetTrans();

	GetDlgItem(IDC_CHECK1)->EnableWindow(true);
	GetDlgItem(IDC_CHECK2)->EnableWindow(true);
	GetDlgItem(IDC_CHECK3)->EnableWindow(true);
	GetDlgItem(IDC_CHECK4)->EnableWindow(true);
	GetDlgItem(IDC_CHECK5)->EnableWindow(true);
	//GetDlgItem(IDC_CHECK7)->EnableWindow(true);

	//m_but6.SetWindowText(orig);

	m_but1.EnableWindow(true); 
	m_but2.EnableWindow(true);
	//m_but3.EnableWindow(true);
	m_but4.EnableWindow(true);
	m_b12.EnableWindow(true);
	m_b13.EnableWindow(true);
	m_b14.EnableWindow(true);
	m_help.EnableWindow(true);
	
	//m_save.EnableWindow(true);
	//m_help.EnableWindow(true);
	m_but6.EnableWindow(true);

	m_but8.EnableWindow(false);
	m_but9.EnableWindow(false);

	m_mydoc1.EnableWindow(true);
	m_mydoc11.EnableWindow(true);
	m_b17.EnableWindow(true);
	m_b19.EnableWindow(true);
	m_b20.EnableWindow(true);
	m_b21.EnableWindow(true);

	//m_bar.End();
	m_bar.SetCurPos(0);
	//m_bar.Reset();

	KillTimer(100);

	CString cpy;

	if(m_check3)
		cpy = "would copy ";
	else if(m_check5)
		cpy = "scanned ";
	else
		cpy = "copied";

	SetBusy(false);

	if(stop)
		{
		CString tmp4;

		tmp4 = "\r\nCopy was stopped by the request of the user. Some files may not have been copied. It is safe to resume copy in the future.\r\n";
		
		ProgText(tmp4 + "\r\n");
		PrintToLog(tmp4 + "\r\n");
		}

	CString tmp3; 
	eclock = clock();

	double rate = 0;
	if(eclock - sclock > 0)
		{
		rate = (tdata / 1000) /  ((double)(eclock - sclock) / CLOCKS_PER_SEC) ;
		//P2N("elapsed %d %d sub=%d %.2f\r\n", eclock, sclock, eclock - sclock, rate);
		}

	CString total; total.Format("%.0f", tdata);
	addcommas(total);

	tmp3.Format("Copied %s bytes in %.2f seconds, avarage %.2f kBytes/sec\r\n", total, 
			(double)(eclock - sclock) / CLOCKS_PER_SEC, rate);
					
	ProgText(tmp3); PrintToLog(tmp3); 

	if(!m_check5)
		{
		tmp3.Format("Finished backup of '%s' to '%s' scanned %d files, %s %d files.\r\n", 
				rm_tree.m_result, rm_tree2.m_result, fcounter, cpy, counter);
		}
	else
		{
		tmp3.Format("Finished scan of '%s' scanned %d files %s bytes.\r\n", 
								rm_tree.m_result, fcounter, total);
		}
		
	ProgText(tmp3); PrintToLog(tmp3); 
				
	rm_tree.EnableWindow(true);
	rm_tree2.EnableWindow(true);
	m_rate.SetText("");

	if(pending)
		{
		pending = false;
		ProgTextP("The system %s logical drive %s\r\n", remdir, drive);
		m_tree.AddAllDrive(); m_tree2.AddAllDrive();
		}

	m_lab.SetText(idlemsg);

	if(m_check6)
		{
		ShutDownTimer();
		}
}

///////////////////////////////////////////////////////////////////////

BOOL CTimedimDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	if(busy)
		{
		//P2N("Set busy cursor %d\r\n", pWnd);
		SetCursor(m_cursor);
		return true;
		}
	//else if(corner)
	//	{
	//	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////
// Copy files (recurse)

static SYSTEMTIME old;
static unsigned int oldc;

int CTimedimDlg::CopyRecurse(const char *src, const char *dst)

{
	heapcheck("Started recurse\r\n", 0);

	if(stop)
		{
		return 0;
		}

	YieldToWinEx();

	dcounter++;

	// Parse into components, reassemble
	CString buffer(src);
	CString drive, dir, fname, ext;
	SplitPath( buffer, drive, dir, fname, ext);

	//P2N("Recursing to dir %s\r\n", drive + dir);
	DirText(drive + dir + "\r\n");

	CString sdir(drive + dir);
	ShortenPath(sdir, 84);
	
	// Time for dir refresh?
	unsigned int iclock = clock();
	if(iclock > oldc + CLOCKS_PER_SEC / 4)
		{
		//P2N("Clock %u cps: %u\r\n", iclock, CLOCKS_PER_SEC);
		oldc = iclock;
		m_lab.SetText("Scan Dir:  " + sdir);
		}
		
	// Time for window taskbar text refresh?
	SYSTEMTIME SystemTime;
	GetSystemTime(&SystemTime);
	if(abs(SystemTime.wSecond - old.wSecond) > 1)
		{
		old.wSecond = SystemTime.wSecond;		
		SetWindowText(sdir);
		}

	CString buffer2(dst);
	CString drive2, dir2, fname2, ext2;
	SplitPath( buffer2, drive2, dir2, fname2, ext2);

	// Assure target exists, create it:
	if(!m_check5)
		{
		if(_access(drive2 + dir2, 0) < 0)
			{
			//m_prog.ReplaceSel("Create dir: ");
			//m_prog.ReplaceSel(drive2 + dir2);
			//m_prog.ReplaceSel("\r\n");
			
			_mkdir(drive2 + dir2);
			ProgTextP("Created new dir: %s\r\n", drive2 + dir2);
			}
		}	

	struct _finddata_t c_file;
    long hFile;

    if( (hFile = _findfirst(drive + dir + "*.*", &c_file )) == -1L )
		{
		//MBOXP("No files in directory (%s)", drive + dir);
		goto endd2;
		}
	else 
		{
		while( _findnext( hFile, &c_file ) == 0 )
			{
			CString fff(c_file.name);

			CString nnn = drive  + dir  + fff;
			CString ttt = drive2 + dir2 + fff;

			CString drive3, dir3, fname3, ext3;
			SplitPath( nnn, drive3, dir3, fname3, ext3);

			if(fff.GetAt(0) == '.')
				{
				//m_prog.ReplaceSel("System implicit: \r\n");
				continue;
				}
			//else if(fff.GetAt(0) == '_')
			//	{
			//	//m_prog.ReplaceSel("Hidden: ");
			//	//m_prog.ReplaceSel(nnn); 
			//	//m_prog.ReplaceSel("\r\n");
			//	}			
			//else if(ext3 == ".db")
			//	{
			//	P2N("Ignored %s\r\n", nnn);
			//	continue;
			//	}

			else if(IsDir(nnn))
				{
				// see if we have no disk space

				//m_prog.ReplaceSel("Dir: ");
				//m_prog.ReplaceSel(nnn); 
				//m_prog.ReplaceSel("\r\n");

				// Do not descend on recycle bin
				if(nnn.Find("\\RECYCLER\\", 2 ) >= 0)
					{
					//P2N("Not copying %s %x\r\n", nnn, GetFileAttributes(nnn));
					}
				else
					{
					CopyRecurse(nnn + "\\", ttt + "\\");
					}
				//continue;
				}
			else
				{
				// do not copy thumbs.db files (superseeded by system file copy)
				//if(fff == "Thumbs.db")
				//	{
				//	//P2N("Not copying %s %x\r\n", nnn, GetFileAttributes(nnn));
				//	}
			
				int attrib = GetFileAttributes(nnn);
				if(attrib  & FILE_ATTRIBUTE_SYSTEM)
					{
					//P2N("Not copying sys file %s %x\r\n", nnn, GetFileAttributes(nnn));
					}
				if(attrib  & FILE_ATTRIBUTE_TEMPORARY)
					{
					//P2N("Not copying tmp file %s %x\r\n", nnn, GetFileAttributes(nnn));
					}
				else
					{
					CopyIf(nnn, ttt);
					}
				}

			if(IsIconic())
				{
				Sleep(2);
				support.YieldToWinEx();
				}
				
			YieldToWinEx();

			if(stop)
				break;
			}		
		}

	_findclose( hFile );

endd2:
	return 0; 
}

//////////////////////////////////////////////////////////////////////////

int CTimedimDlg::CopyIf(const char *src, const char *dst)

{
	//P2N("Copy if: %s \r\n", src);

	fcounter++;

	struct _stat buf, buf2;

	memset(&buf, 0, sizeof(struct _stat));
	memset(&buf2, 0, sizeof(struct _stat));
 
	int		result, result2;

	result = _stat(src, &buf );
	if( result != 0 )
		{
		//MBOXP( "Problem getting file information %s", src );
		P2N("Cannot stat source '%s'\r\n", src);
		return(0);
		}

	result2 = _stat(dst, &buf2 );
	if( result2 != 0 )
		{
		memset(&buf2, 0, sizeof(struct _stat));
		}

	if(buf.st_mtime > buf2.st_mtime)
		{
		//strftime
		//m_prog.ReplaceSel("NEW: ");

		CTime ct = buf.st_mtime;
		CString datestr = ct.Format("%A, %B %d, %Y - %I:%M %p");
		
		CTime ct2 = buf2.st_mtime;
		CString datestr2 = ct.Format("%A, %B %d, %Y - %I:%M %p");
		
		//P2N("Copy active --> old: %d--%s  %s\r\n" 
		//	"            --> new: %d--%s  %s\r\n", 
		//			buf.st_mtime,	datestr, src,
		//				buf2.st_mtime, datestr2, dst);

		//P2N("Copy from %d %s\r\n", buf.st_mtime, src);
		//P2N("Copy to %d %s\r\n", buf2.st_mtime, dst);

		if(m_check5)
			ProgText(src);
		else
			ProgText(dst);

#if 1
		if(m_check3)
			{	
			CString str, exists;

			// Target did exist
			if( result2 == 0 )
				exists = " + older version of file at target";
			
			str.Format(" -- would copy %s\r\n", exists);

			ProgText(str);
			counter++;
			return 0;
			}

		// Create indexed backup of old file
		if(m_check2)
			{
			// Target did exist
			if( result2 == 0 )
				{
				// Save renamed file date
				int ret;
				FILETIME CreationTime, LastAccessTime, LastWriteTime;
				HANDLE handle = CreateFile(dst, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
				if(handle != INVALID_HANDLE_VALUE)
					{
					ret = GetFileTime(handle, &CreationTime, &LastAccessTime, &LastWriteTime);
					CloseHandle(handle);
					}
				
				// Construct new name				
				CString newname(dst);

				CString drive, dir, fname, ext;
				SplitPath( newname, drive, dir, fname, ext);

				CTime ct3 = buf2.st_mtime;
				CString datestr3 = ct.Format(".tdm.%M%H.%d%m%y");		
				
				newname = drive + dir + fname + datestr3 + ext;
				P2N("New name for destination: %s\r\n", newname);
				rename(dst, newname);

				// Set backup file to old file's date
				if(ret)
					{
					HANDLE handle2 = CreateFile(newname, GENERIC_WRITE | GENERIC_READ, 
										0, NULL, OPEN_EXISTING, 0, NULL);
					if(handle2 != INVALID_HANDLE_VALUE)
						{
						int ret2 = SetFileTime(handle2, &CreationTime, &LastAccessTime, &LastWriteTime);
						CloseHandle(handle2);
						}
					}	
				}
			}

		struct stat buffer;
		
		if(stat(dst, &buffer) >= 0)
			{
			if(!(buffer.st_mode & _S_IWRITE))
				{
				P2N("Read only attribute set on %s\r\n", dst);
				error++;

				CString tmp; tmp.Format(" -- Error: Read only destination file");
				ProgText(tmp);

				ErrText(dst + tmp + "\r\n");

				PrintToLog("Read only destination '%s' --> '%s'\r\n", 
						src, dst);

				goto endd;
				}
			}
		else
			{
			//P2N("Cannot stat destination\r\n");
			}

		{

		//m_bar.End();
		m_bar.SetCurPos(0);
		//m_bar.Reset();

		// Propagate flags
		copyfile.badsect = m_check5;
		copyfile.friendly = m_check7;

		int ret = copyfile.CopyTimeDimFile(src, dst, false);
		
		//m_bar.SetMode(BPC_MODE_BUSY);
		//m_bar.SetCurPos(0); //m_bar.Reset(); //m_bar.Start();

		int err = GetLastError();
			
		if(ret != COPY_NO_ERROR)	
			{
			CString buffer(src), drive, dir, fname, ext;
			SplitPath(buffer, drive, dir, fname, ext);

			// Update statistics
			error++;

			CString tmp; tmp.Format(" -- Error: %s", strerror(err), copyfile.err2txt(ret));

			ProgText(tmp); 

			ErrText(dst); ErrText(tmp);
			tmp.Format(" Ext. Code: %s\r\n",  copyfile.err2txt(ret));
			ErrText(tmp);

			PrintToLog("Error copying '%s' --> '%s' error: %s\r\n", 
					src, dst, strerror(err));

			double	ds = GetFreeDiskSpace(dst);
			if(ds < 10000)
				{
				ProgText("\r\n");
				CString str("Stopping copy progress because of low target disk space or read only drive\r\n");

				P2N("Low disk space\r\n");
			
				ProgText(str);	PrintToLog(str);
				stop = true;
				}

			}
		else
			{
			ProgText(" -- OK ");
			
			// Update statistics
			counter++;
			
			tdata += copyfile.tdata;

			if(!m_check5)
				{
				// Set target date to source date
				FILETIME CreationTime, LastAccessTime, LastWriteTime;
				int ret;
				HANDLE handle = CreateFile(src, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
				if(handle != INVALID_HANDLE_VALUE)
					{
					ret = GetFileTime(handle, &CreationTime, &LastAccessTime, &LastWriteTime);
					CloseHandle(handle);
					}
				
				if(ret)
					{
					HANDLE handle2 = CreateFile(dst, GENERIC_WRITE | GENERIC_READ, 
										0, NULL, OPEN_EXISTING, 0, NULL);
					if(handle2!= INVALID_HANDLE_VALUE)
						{
						int ret2 = SetFileTime(handle2, &CreationTime, &LastAccessTime, &LastWriteTime);
						CloseHandle(handle2);
						}
					}
				else
					{	
					int err = GetLastError();
					CString errd;

					errd.Format("'%s' Cannot set target file date. Error: %s\r\n", 
								dst, strerror(err));

					PrintToLog(errd); ErrText(errd);

					ProgText(" -- Warn: cannot set filetime.");
					}
				}
				
			if(m_check4)
				{
				//P2N("Verify %s -- \r\n", src);

				ProgText(" ... Verifying ... ");
				
				int ret = copyfile.VerifyTimeDim(src, dst);

				if(ret == VERIFY_ERR)
					{
					ProgText(" FAILED ");

					CString errv;
					errv.Format("Error verifying '%s' --> '%s'\r\n", src, dst);
					PrintToLog(errv); ErrText(errv);
					}
				else
					ProgText(" OK ");
				}
			}
		}
#endif
		
endd:
		ProgText("\r\n");
		}

	YieldToWinEx();
	return 0; 
}

void CTimedimDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel = m_tab.GetCurSel();

	TC_ITEM tc;

	tc.mask = TCIF_PARAM;
	m_tab.GetItem(sel, &tc);
			
	P2N("Change tab enter %x %d\r\n", tc.lParam, sel);

	CEdit *m_edit = (CEdit	*) tc.lParam;

	//m_edit->SetWindowPos(&wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	m_edit->ShowWindow(SW_SHOW);
	
	//cwnd
	//m_edit->Invalidate();
	//YieldToWinEx();

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	int sel = m_tab.GetCurSel();

	TC_ITEM tc;

	tc.mask = TCIF_PARAM;
	m_tab.GetItem(sel, &tc);
			
	P2N("Change tab leave %x %d\r\n", tc.lParam, sel);

	CEdit *m_edit = (CEdit	*) tc.lParam;

	m_edit->ShowWindow(SW_HIDE);
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
//

void CTimedimDlg::ProgText(const char *str, int log)

{
	logwin.AddString(str);
	
	//m_edit2.SetSel(-1, -1);

	// Check for overflow: CEdit
	if(m_edit2.GetLineCount()  > OFLOWLINES)
		{
		//P2N("Cutting text %d\r\n", m_edit2.GetLineCount());

		m_edit2.SetSel(0, m_edit2.LineIndex( m_edit2.GetLineCount()/2), true);
		m_edit2.ReplaceSel(" --- Timedim has cut text here (text overflow) --- \r\n"); 
		
		YieldToWinEx();

		m_edit2.SetSel(0, -1);
		m_edit2.SetSel(-1, -1);

		//P2N("Cut text to %d\r\n", m_edit2.GetLineCount());
		//P2N("New text %s\r\n", str);

		//int idx = m_edit2.LineIndex(m_edit2.GetLineCount() - 1);
		//idx += m_edit2.LineLength();
		//P2N("Line index = %d\r\n", idx);
		//m_edit2.SetSel(idx - 5, idx - 1);
		}

	m_edit2.ReplaceSel(str);
	//Sleep(100);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::ProgTextP(const char *Format, ...)

{
	static char szOutString[800];

    char    *asc = NULL;
    va_list ArgList;
    va_start(ArgList, Format);
    _vsnprintf(szOutString, sizeof(szOutString), Format, ArgList);

    ProgText(szOutString);
}

//////////////////////////////////////////////////////////////////////////

BOOL CTimedimDlg::PreTranslateMessage(MSG* pMsg) 

{

#if 0

	if(pMsg->message == WM_POWER)
		{
		P2N("POWER message: %d %s -- powerevent = %d\r\n", pMsg->message, mxpad.num2msg(pMsg->message), pMsg->wParam);
		}

	if(pMsg->message == WM_POWERBROADCAST)
		{
		P2N("POWER broadcast: %d %s -- powerevent = %d\r\n", pMsg->message, mxpad.num2msg(pMsg->message), pMsg->wParam);
		}
#endif


	if(WM_POWERBROADCAST == pMsg->message)
		{
		return(0);
		}

	//if(WM_MOUSEMOVE != pMsg->message)
	//	{
	//	P2N("Pretranslate: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
	//	}

	//if(pMsg->message == WM_SETTINGCHANGE)
	//	{
	//	P2N("Settings Change: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
	//	}

	if(pMsg->message == WM_DEVICECHANGE)
		{
		//P2N("Device Change: %d %s code = %d\r\n", 
		//		pMsg->message, mxpad.num2msg(pMsg->message), pMsg->wParam);

#if 0
		if(pMsg->wParam == DBT_DEVICEARRIVAL)
			{
			P2N("Device ARRIVED: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
			}
		if(pMsg->wParam == DBT_CONFIGCHANGED)
			{
			P2N("Device CONFIG Change: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
			}
		if(pMsg->wParam ==  DBT_DEVICEREMOVECOMPLETE )
			{
			P2N("Device remove complete: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));
			}
#endif

		if(pMsg->wParam ==  DBT_DEVNODES_CHANGED)
			{
			//P2N("DBT_DEVNODES_CHANGED: %d %s\r\n", pMsg->message, mxpad.num2msg(pMsg->message));

			// Start the drive change detection timer if none
			if(!devtimer)
				{
				devtimer = true;
				SetTimer(99, 200, NULL);			
				}
			}
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		P2N("Key down preview %d %d\r\n", pMsg->wParam, pMsg->lParam);

		// ALT-S --- stop
		if(pMsg->wParam == 'S')
			{
			P2N("Stop command ALT-C %d %d\r\n", pMsg->wParam, pMsg->lParam);
			stop++;
			}
		

		// ALT-F4
		if(pMsg->wParam == 115)
			{
			if(busy)
				{
				ProgText("\r\nCannot exit TimeDim while operation in progress\r\n");
				}
			else
				{
				//MBOX("ALT F4");
				EndDialog(IDOK);
				}
			}
		}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// Clear progress windows

void CTimedimDlg::OnButton7() 

{
	logwin.Clear();

	if(GetKeyState(VK_SHIFT) & 0x80)
		{
		errwin.Clear();
		dirwin.Clear();	
		}
}

void CTimedimDlg::ErrText(const char *str)

{
	errwin.AddString(str);
	
#if 0
	m_edit1.SetSel(-1, -1);

	// Check for overflow: CEdit
	if(m_edit1.GetLineCount( )  > ERROFLOWLINES)
		{
		m_edit1.SetSel(0, m_edit1.LineIndex( m_edit1.GetLineCount()/2), true);
		m_edit1.ReplaceSel(" --- Timedim has cut the error text here (text overflow) --- \r\n"); 
		m_edit1.SetSel(0, -1);
		m_edit1.SetSel(-1, -1);

		//m_edit1.ReplaceSel("\r\n");
		}
	m_edit1.ReplaceSel(str);

#endif

}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::DirText(const char *str)

{
	dirwin.AddString(str);

#if 0
	m_edit3.SetSel(-1, -1);

	// Check for overflow: CEdit
	if(m_edit3.GetLineCount( )  > OFLOWLINES)
		{
		m_edit3.SetSel(0, m_edit3.LineIndex( m_edit3.GetLineCount()/2), true);
		m_edit3.ReplaceSel(" --- Timedim has cut directory progress text here (text overflow) --- \r\n"); 
		m_edit3.SetSel(0, -1);
		m_edit3.SetSel(-1, -1);
		//m_edit3.ReplaceSel("\r\n");
		}
	m_edit3.ReplaceSel(str);

#endif
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnButton8() 

{		
	if(waitfordown)
		{
		KillTimer(4);

		m_but8.SetWindowText(oldstop);
		waitfordown = false;
		ProgText(" ... Received 'Cancel Shutdown'. Shutdown process averted.\r\n");
		m_but8.EnableWindow(false);
		return;
		}

	if(waitfortimer)
		{
		SetTimer(2, 100, NULL);			
		m_but1.EnableWindow(true);
		m_but2.EnableWindow(true);
		m_but4.EnableWindow(true);
		m_but6.EnableWindow(true);
		}

	if(stop > 2)
		{
		ProgText("\r\n");
		ProgText(" ... Received ABORT command, clearing partial file, Please Wait ... \r\n");
		}
	else
		{
		ProgText("\r\n");
		ProgText(" ... Received STOP command, Please Wait ... (press at least three stops to abort) ");
		}

	stop++;	
	copyfile.stop++;
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N("Expanding items\r\n");

	char  text[_MAX_PATH] = "Hello";

	HTREEITEM hItem = m_tree.GetSelectedItem();

	if(!hItem)
		return;

	m_tree.DrawChildren(hItem);
	m_tree.DrawNumSelectedFolder();

	m_tree.SelectItem(hItem);

	TVITEM tv;
	memset(&tv, 0, sizeof(tv));

	tv.pszText = text;
	tv.cchTextMax = _MAX_PATH;

	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.stateMask = -1;

	tv.hItem = hItem;
	m_tree.GetItem(&tv);

	//if(tv.state | TVIS_EXPANDED)
	//	P2N("Expanded Item\r\n");

	//P2N("Selected %s\r\n", text);

	*pResult = 0;
}


void CTimedimDlg::OnItemexpandingTree2(NMHDR* pNMHDR, LRESULT* pResult) 

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N("Expanding items\r\n");

	char  text[_MAX_PATH] = "Hello";

	HTREEITEM hItem = m_tree2.GetSelectedItem();

	if(!hItem)
		return;

	m_tree2.DrawChildren(hItem);
	m_tree2.DrawNumSelectedFolder();

	m_tree2.SelectItem(hItem);

	TVITEM tv;
	memset(&tv, 0, sizeof(tv));

	tv.pszText = text;
	tv.cchTextMax = _MAX_PATH;

	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.stateMask = -1;

	tv.hItem = hItem;
	m_tree2.GetItem(&tv);

	if(tv.state | TVIS_EXPANDED)
		{
		//P2N("Expanded Item\r\n");
		}

	//P2N("Selected %s\r\n", text);

	*pResult = 0;
}

void CTimedimDlg::OnButton2() 

{
	CopyBiDir(m_tree2, m_tree);	
}


void CTimedimDlg::OnButton10() 

{
	MBOX("Browse not implemented yet");	
	return;
	
	//CFileDialog
	CFileDialogST dlgFile(true);

	CString FileName("Any File");

	//dlgFile.m_ofn.Flags = 0;
	dlgFile.m_ofn.lpstrFilter = "All Files;*.*";
	dlgFile.m_ofn.lpstrTitle = "Browse for Source directory";
	dlgFile.m_ofn.lpstrFile = FileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = ""; //str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	FileName.ReleaseBuffer();

	MBOX(FileName);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnHelp() 

{
	if(!IsWindow(about.m_hWnd))
		about.DoModal();
}

#if 0
void CAboutDlg::OnOk2() 

{
	AfxMessageBox("Not implemented yet");	
}
#endif

void CTimedimDlg::OnButton11() 
{
	MBOX("Browse not implementd yet");	
	return;		
}

void CAboutDlg::OnButton1() 

{
	CString str(approot);

	str += "html\\toc.html";
	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);
	return;
}

void CAboutDlg::OnCancel() 

{
	CDialog::OnCancel();
}

void CTimedimDlg::OnUnc4() 
{
	prompt = true;	
}

void CTimedimDlg::OnButton9() 
{
	CPausDlg pause;
	pause.DoModal();
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnButton4() 

{
	CString  fileName;
	
	P2N("Loading new Session\r\n");

	CFileDialogST dlgFile(true);

	CString title = "Open Session File"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "sess";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Session file (*.sess)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.sess";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);

	dlgFile.m_ofn.lpstrInitialDir = appdata;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	//P2N("Got file name %s\r\n", fileName);

	LoadSessionfile(fileName);

}


void CTimedimDlg::OnButton3() 

{
	MBOX("Feature not implemented yet");		
}

BOOL CAboutDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	if(first)
		{
		first = false;
		m_bitmap.LoadBitmap(IDB_BITMAP7);
		
		BITMAP BitMap;
		m_bitmap.GetBitmap(&BitMap);
		m_nWidth = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;

		m_head.SetTrans();

		//m_head.CenterText(true);
		m_head.SetFontSize(-36);
		m_head.SetFontBold(true);
		//m_head.CenterText(true);
		
		m_cr.SetTrans();
		m_ver.SetTrans();

		}
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	CDialog::OnEnterIdle(nWhy, pWho);
	
	// TODO: Add your message handler code here
	
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 

{
	RECT rc;
	GetClientRect(&rc);

	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	
	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
	CClientDC dc(this);

	//AlphaBlend(dc, 0,0, m_nWidth, m_nHeight, dcMem, 0,0,m_nWidth, m_nHeight, m_bf); 

	int loop, loopv;
	m_nWidth, m_nHeight;

	for(loop = 0; loop < rc.right; loop += m_nWidth)
		for(loopv = 0; loopv < rc.bottom; loopv += m_nHeight)
			pDC->BitBlt(loop, loopv, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY ); 		

	dcMem.SelectObject(pOldBitmap);

	// We handled it
	return(false);	
	return CDialog::OnEraseBkgnd(pDC);
}

void CTimedimDlg::OnSetFocus(CWnd* pOldWnd) 

{
	CDialog::OnSetFocus(pOldWnd);
	//P2N("Window input focus\r\n");
}

//DEL void CTimedimDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
//DEL {
//DEL 	CDialog::OnActivate(nState, pWndOther, bMinimized);
//DEL 
//DEL #if 0
//DEL 	if(nState & (WA_ACTIVE | WA_CLICKACTIVE ))
//DEL 		{
//DEL 		P2N("Actiate app\r\n");
//DEL 
//DEL 		if(!busy)
//DEL 			{
//DEL 			m_tree.Refresh();
//DEL 			m_tree2.Refresh();
//DEL 			}
//DEL 		}
//DEL 	else
//DEL 		P2N("Deactivate app\r\n");
//DEL #endif
//DEL 
//DEL }

void CTimedimDlg::OnButton12() 

{	
	CString  fileName;
		
	UpdateData();

	CTime ct = CTime::GetCurrentTime();
	fileName = ct.Format("%a%b%d_%H_%M.sess");

	P2N("Saving Session %s\r\n", fileName);

	// CFileDialog
	CFileDialogST dlgFile(false);

	CString title = "Save Session File"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "sess";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Session file (*.sess)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.sess";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(100 * _MAX_PATH);
	dlgFile.m_ofn.nMaxFile = 100 * _MAX_PATH;

	dlgFile.m_ofn.lpstrInitialDir = appdata;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	//P2N("Got file name '%s'\r\n", fileName);

	//MBOX(fileName);

	CFile cf;
	cf.Open(fileName, CFile::modeWrite | CFile::modeCreate);

	//P2N("Session: %s\r\n%s\r\n", m_tree.m_result, m_tree2.m_result);

	cf.Write( m_tree.m_result,  m_tree.m_result.GetLength());
	cf.Write("\r\n", 2);
	cf.Write( m_tree2.m_result,  m_tree2.m_result.GetLength());
	cf.Write("\r\n", 2);

	CString cline;

	if(m_check4)
		{ cline += "/v "; }
	if(m_check3)
		{ cline += "/s "; }
	if(m_check2)
		{ cline += "/b "; }
	if(m_check1)
		{ cline += "/p "; }

	//P2N("Cparm: %s\r\n", cline);

	cf.Write(cline,  cline.GetLength());
	cf.Write("\r\n", 2);

	cf.Close();
}

void CTimedimDlg::OnButton13() 

{
	CString str(approot);

	str += "html\\toc.html";

	HINSTANCE  ret = ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWMAXIMIZED);	

	if((int)ret <= 32)
		{
		AfxMessageBox("No help here");
		}
}

void	CTimedimDlg::OnButton14() 

{
	logdlg.DoModal();
}

void CTimedimDlg::OnButton15() 

{
	CString str;

	//P2N("Loading My Documents\r\n");

	SetBusy(true);
		
	GetSpecialFolder(CSIDL_PERSONAL, str);
	m_prog.SetText(str);
	m_tree.SetCurr(str);
	m_tree.m_result = str;

	SetBusy(false);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnButton16() 

{	
	CString str;
	SetBusy(true);
		
	GetSpecialFolder(CSIDL_PERSONAL, str);

	str.Replace("My Documents\\", "");
	m_prog.SetText(str);
	m_tree.SetCurr(str);
	m_tree.m_result = str;

	SetBusy(false);
		
}

void CTimedimDlg::OnButton17() 

{
	CString str;
		
	SetBusy(true);
		
	GetSpecialFolder(CSIDL_PERSONAL, str);

	str.Replace("My Documents\\", "");
	m_prog2.SetText(str);
	m_tree2.SetCurr(str);
	m_tree2.m_result = str;

	SetBusy(false);		
}

void CTimedimDlg::OnButton19() 

{
	CString str;

	SetBusy(true);
		
	GetSpecialFolder(CSIDL_PERSONAL, str);
	m_prog2.SetText(str);
	m_tree2.SetCurr(str);
	m_tree2.m_result = str;

	SetBusy(false);		
}

void CTimedimDlg::OnButton20() 

{
	CString str = "c:\\";
	
	int rmd = FindFirstRemovable();

	if(!rmd)
		{
		return;
		}
		
	str.Format("%c:\\", rmd + 'A');
		
	SetBusy(true);
		
	m_prog.SetText(str);
	m_tree.SetCurr(str);
	m_tree.m_result = str;

	SetBusy(false);		
}


CString GetCurrentDir()

{
	CString dir;
	char buffer[_MAX_PATH + 4];

	getcwd(buffer, _MAX_PATH);

	// Fix up path for split
	if(strlen(buffer) > 3 )
	 	strcat(buffer, "\\");

	dir = buffer;
	return(dir);
}


static int skip = 0;

int CTimedimDlg::FindFirstRemovable()

{
	int localskip = skip;
	CString str, newName, temp_cdir; // = camera_dir;

	int	nDocType = -1;
	int	gotremovable = false;

	CString cwdir = GetCurrentDir();
	DWORD  drives = GetLogicalDrives();

	char dname[3]; dname[0] = 'A'; dname[1] = ':'; dname[2] = 0;

	// ABORT RETRY FAIL AVOIDED HERE
	int old_err =  SetErrorMode(SEM_FAILCRITICALERRORS), loop;

	char VolumeNameBuffer[MAX_PATH], FileSystemNameBuffer[MAX_PATH];

	DWORD VolumeSerialNumber, MaximumComponentLength, FileSystemFlags;

newscan:
	
	// Second pass, search for last removable
	for(loop = 31; loop > 1; loop--)
		{
		DWORD mask = 1;
		mask = mask << loop;

		if(drives & mask)
			{
			dname[0] = loop + 'A';

			P2N("Drive %s [%d], type=%d ", dname, loop, GetDriveType(dname));

			*VolumeNameBuffer = 0; 	*FileSystemNameBuffer = 0;

			BOOL ret = GetVolumeInformation(
				dname,						// address of root directory of the
				VolumeNameBuffer,			// address of name of the volume
				MAX_PATH,					// length of lpVolumeNameBuffer
				&VolumeSerialNumber,		// address of volume serial number
				&MaximumComponentLength,
				&FileSystemFlags,			// address of file system flags
				FileSystemNameBuffer,		// address of name of file system
				MAX_PATH					// length of lpFileSystemNameBuffer
						);

			STORAGE_BUS_TYPE bt = GetBustype(dname);

			P2N("vol:%s  fs:%s bustype=%d\r\n", VolumeNameBuffer, FileSystemNameBuffer, bt);

			if(bt == BusTypeUsb)
				{
				if(!localskip)
					{
					temp_cdir = dname;
					gotremovable = true;
					P2N("Removable drive in %s\r\n", dname);
					skip++;
					break;
					}
				else
					{
					localskip--;
					}
				}

			//if((GetDriveType(dname) == DRIVE_REMOVABLE) && ret)
			//	{	
			//	temp_cdir = dname;
			//	gotremovable = true;
			//	P2N("Removable drive in %s", dname);
			//	break;
			//	}

			}
		}

	SetErrorMode(old_err);

	if(!gotremovable)
		{
		if(skip > 0)
			{
			skip = 0;
			goto newscan;
			}

		// ???ProgText("No removable drive detected.");
		MBOX("No removable drive detected\r\n");
		return 0;
		}
	
	return loop;
}

void CTimedimDlg::OnButton21() 

{
	CString str = "c:\\";
	
	int rmd = FindFirstRemovable();

	if(!rmd)
		return;
			
	SetBusy(true);
		
	str.Format("%c:\\", rmd + 'A');
	m_prog2.SetText(str);
	m_tree2.SetCurr(str);
	m_tree2.m_result = str;

	SetBusy(false);		
}

void CTimedimDlg::OnCheck3() 
{
	UpdateData();
	P2N("simulate %d\r\n", m_check3);

	// Uncheck the others
	if(m_check3)
		{
		m_check4 = m_check2 = false;
		UpdateData(false);
		}
}

//DEL void CTimedimDlg::OnDevModeChange(LPTSTR lpDeviceName) 
//DEL {
//DEL 	CDialog::OnDevModeChange(lpDeviceName);
//DEL 	
//DEL 	P2N("Message: WM_DEVICECHANGE\r\n");
//DEL }

void CTimedimDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	
	P2N("OnSettingChange\r\n");	
}

//GetDisksProperty DeviceIoControl QueryDosDevice CM_Query_And_Remove_SubTree
// SetupDiGetClassDevs

void CTimedimDlg::LoadSessionfile(const char *fname)

{
	CFile	cf; 
	int		len;
	char	*buff;

	CString str;

	ProgTextP("Loading session file: '%s' ... ", fname);

	if(!cf.Open(fname, CFile::modeRead))
		{
		MBOX("Cannot open file\r\n");
		goto endd;
		}

	len = cf.GetLength();

	buff = str.GetBuffer(len + 1);
	if(!buff)
		{
		MBOX("No memory to read in file\r\n");
		goto endd;
		}

	cf.Read(buff, len);
	buff[len] = 0;

	str.ReleaseBuffer();

	//P2N("Got:\r\n%s\r\n", str);

	{
	int old = 0;
	int idx = str.Find("\r\n");
	if(idx >= 0)
		{
		CString str2 = str.Mid(old, idx - old);
		//P2N("first: '%s'\r\n", str2);
		
		SetBusy(true);
		m_prog.SetText(str2);
		m_tree.SetCurr(str2);
		m_tree.m_result = str2;
		SetBusy(false);
		
		old = idx+2;
		idx = str.Find("\r\n", old);
		if(idx >= 0)
			{
			str2 = str.Mid(old, idx - old);
			//P2N("second: '%s'\r\n", str2);
			
			SetBusy(true);
			m_prog2.SetText(str2);
			m_tree2.SetCurr(str2);
			m_tree2.m_result = str2;
			SetBusy(false);
		
			old = idx+2;
			idx = str.Find("\r\n", old);
			if(idx >= 0)
				{
				str2 = str.Mid(old, idx - old);
				P2N("Third (swiches) : '%s'\r\n", str2);

				CString argarr[ARGARRSIZE];
				// Instructions from command line 
				ParseComandLine(str2, argarr, ARGARRSIZE);
				InterpretComandLine(argarr, ARGARRSIZE);
				}			
			}
		}
	}
	
	//ProgTextP("Finished loading session from '%s'\r\n", fileName);

endd:

	ProgText("OK\r\n");
	cf.Close();
}

//DEL void CTimedimDlg::OnActivateApp(BOOL bActive, HTASK hTask) 
//DEL 
//DEL {
//DEL 	CDialog::OnActivateApp(bActive, hTask);
//DEL }


LRESULT CTimedimDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 

{		
	if(WM_POWERBROADCAST == message)
		{
		if(wParam == PBT_APMPOWERSTATUSCHANGE)
			{
			SYSTEM_POWER_STATUS sps;
			GetSystemPowerStatus(&sps);

			P2N("Power bradcast event%d\r\n", wParam);
	
			// Change in power status
			if(sps.ACLineStatus != inisps.ACLineStatus)
				{
				inisps.ACLineStatus = sps.ACLineStatus;

				if(sps.ACLineStatus == 1)
					{
					ProgText("\r\nAC LINE CONNECTED\r\n");
					}
				else
					{
					ProgText("\r\nAC LINE DICONNECTED\r\n");
					}
				}

			if(wParam == PBT_APMQUERYSUSPEND)
				{
				P2N("Timedim prevented the system to enter standby.\r\n");
				ProgText("Timedim prevented the system to enter standby.\r\n");
				//return (BROADCAST_QUERY_DENY);
				return true;
				}

			if(wParam == PBT_APMBATTERYLOW)
				{
				// Battery low, aborting copy proress
				ProgText("\r\nBattery low, aborting copy process\r\n");
				stop = true;
				}
			}
		}

	return CDialog::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	P2N("CTimedimDlg::OnSyskeyDown %c\r\n", nChar);
	
	if(nChar == 'S')
		{
		//P2N("Stop command ALT-S %c\r\n", nChar);

		ProgText("Stop command received via Hotkey (send three times to abort current file copy)\r\n");
		stop++;	copyfile.stop++;
		}

	CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
//

void CTimedimDlg::ShutDown(UINT command)

{	  
	OSVERSIONINFO osVersion;  GetVersionEx(&osVersion);

	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 

	OpenProcessToken(GetCurrentProcess(), 
	  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) ;

	// Get the LUID for shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
	  &tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
	  (PTOKEN_PRIVILEGES) NULL, 0); 

	int nArgument = EWX_POWEROFF;
	switch(command)
		{  
		case 0:nArgument = EWX_POWEROFF; break;
		case 1:nArgument = EWX_REBOOT;   break;
		case 2:nArgument = EWX_LOGOFF;   break;
		case 3:nArgument = EWX_SHUTDOWN; break;
		}
	ExitWindowsEx(nArgument, 0);
	EndDialog(IDOK);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::ShutDownTimer() 

{
	ProgText("Finished copy ...  Shutting down in 10 seconds ... Press 'Cancel Shutdown' to stop the shutdown process.\r\n");	

	PrintToLog("Requested system shutdown\r\n");

	waitfordown = true;

	m_but8.GetWindowText(oldstop);
	m_but8.SetWindowText("Cancel Shutdown");
	m_but8.EnableWindow(true);

	SetTimer(4, 10000, NULL);
}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnButton22() 

{
	P2N("CTimedimDlg::OnButton22\r\n");
}


void CTimedimDlg::SetBusy(int flag)

{
	busy = flag;

	if(IsWindow(m_hWnd))
		SendMessage(WM_SETCURSOR, HTCLIENT, 0);

}

//////////////////////////////////////////////////////////////////////////

void CTimedimDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 

{
	CDialog::OnWindowPosChanged(lpwndpos);
		
	copyfile.iconic = IsIconic();
	
	//P2N("CTimedimDlg::OnWindowPosChanged %d (%x)\r\n", 
	//									lpwndpos->flags, IsIconic());	
}

void CTimedimDlg::OnCheck7() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK7))->GetCheck();
	//P2N("CTimedimDlg::OnCheck7 %d\r\n", flag);
	copyfile.friendly = flag;		
}
