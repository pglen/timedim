// GetDirName.cpp : implementation file
//

#include "stdafx.h"
#include "timedim.h"
#include "GetDirName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetDirName dialog

CGetDirName::CGetDirName(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDirName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetDirName)
	m_dir = _T("");
	//}}AFX_DATA_INIT
}

void CGetDirName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetDirName)
	DDX_Control(pDX, IDC_PTEXT, m_ptext);
	DDX_Text(pDX, IDC_EDIT1, m_dir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetDirName, CDialog)
	//{{AFX_MSG_MAP(CGetDirName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDirName message handlers

BOOL CGetDirName::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ptext.SetText(txt);
	//m_ptext.SetTrans();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
