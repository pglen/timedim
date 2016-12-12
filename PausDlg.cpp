// PausDlg.cpp : implementation file
//

#include "stdafx.h"
#include "timedim.h"
#include "PausDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPausDlg dialog


CPausDlg::CPausDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPausDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPausDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPausDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPausDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPausDlg, CDialog)
	//{{AFX_MSG_MAP(CPausDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPausDlg message handlers
