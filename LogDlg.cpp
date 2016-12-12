// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "timedim.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	bgcolor = RGB(192, 192, 192);
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

BOOL CLogDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	filename = appdata + "activity.txt";

	SetTimer(1, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogDlg::OnTimer(UINT nIDEvent) 

{
	KillTimer(nIDEvent);

	char *buff;	CFile dlog;

	if(!dlog.Open(filename, CFile::modeRead | CFile::shareDenyNone))
		{
		P2N("Cannot open 'datalog' log file.\r\n");
		MBOX("Cannot open 'datalog' log file.\r\n");
		return;
		}

	buff = (char *) malloc(dlog.GetLength() + 1);

	if(!buff)
		{
		MBOX("Not enough memory to show log file or log file is too big.");
		dlog.Close();
		return;
		}

	int len = dlog.Read(buff, dlog.GetLength());
	buff[len] = 0;
	dlog.Close();
	
	//P2N("Log buffer %s\r\n", buff);

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT1);
	
	ASSERT(ed);

	ed->SetWindowText(buff);	

	free(buff);
	
	CDialog::OnTimer(nIDEvent);
}

#include "FileDialogST.h"

void CLogDlg::OnButton1() 

{

	CFileDialogST dlgFile(true);

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "txt";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Log text file (*.log)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.log";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	CString FileName("*.txt");

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = "Browse for Archived Logs";
	dlgFile.m_ofn.lpstrFile = FileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrInitialDir = ""; //str;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	FileName.ReleaseBuffer();

	if(!bRet)
		return;

	filename = dlgFile.GetPathName();
	//P2N("Got file name %s\r\n", filename);

	SetTimer(1, 100, NULL);
}

BOOL CLogDlg::OnEraseBkgnd(CDC* pDC) 
{
	RECT	rc;	GetClientRect(&rc);

	//P2N("CLogWin::Erase Dialog\r\n");

	pDC->FillSolidRect(&rc, bgcolor);

	return 1;
	//return CWnd::OnEraseBkgnd(pDC);
	
	return CDialog::OnEraseBkgnd(pDC);
}
