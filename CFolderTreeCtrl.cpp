
/* =====[ CFolderTreeCtrl.cpp ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the CFolderTreeCtrl.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/22/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "direct.h"

#include "resource.h"
#include "timedim.h"
#include "timedimDlg.h"
#include "mxpad.h"
#include "support.h"
#include "GetDirName.h"

#include "CFolderTreeCtrl.h"
#include "usbdrive.h"


extern	CTimedimDlg* pMainDialog;

#define IDM_EVENT				32000 // I think it's enough to be in peace
#define IDM_EVENT_COLLAPSE		IDM_EVENT + 1
#define IDM_EVENT_UNCOLLAPSE	IDM_EVENT + 2
#define IDM_EVENT_SELECTALL		IDM_EVENT + 3
#define IDM_EVENT_UNSELECTALL	IDM_EVENT + 4
#define IDM_EVENT_OPEN			IDM_EVENT + 5
#define IDM_EVENT_MKDIR			IDM_EVENT + 6
#define IDM_EVENT_REFRESH		IDM_EVENT + 7
#define IDM_EVENT_REFRESH_ALL	IDM_EVENT + 8

BEGIN_MESSAGE_MAP(CFolderTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CFolderTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN( )
	
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDM_EVENT_OPEN,MenuEventOpen)
	ON_UPDATE_COMMAND_UI(IDM_EVENT_MKDIR,MenuEventMkdir)
	ON_UPDATE_COMMAND_UI(IDM_EVENT_REFRESH,MenuEventRefresh)
	ON_UPDATE_COMMAND_UI(IDM_EVENT_REFRESH_ALL,MenuEventRefreshAll)
	ON_UPDATE_COMMAND_UI(IDM_EVENT_SELECTALL,MenuEventSelectAll)
	ON_UPDATE_COMMAND_UI(IDM_EVENT_UNSELECTALL,MenuEventUnSelectAll)
	
END_MESSAGE_MAP()

CFolderTreeCtrl::CFolderTreeCtrl()
{
	p_state = NULL;
	prog = NULL;
	result = NULL;
}

CFolderTreeCtrl::~CFolderTreeCtrl()

{
	p_state = NULL;
	Menu.DestroyMenu();
}

void CFolderTreeCtrl::SetState(CEdit* p_state)

{
	this->p_state = p_state;
}

void CFolderTreeCtrl::Init()

{
	p_state = NULL;
	m_imageListFolder.Create( 16, 16, ILC_COLOR8, 0, 0);

	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_FOLDER_CLOSE));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_FOLDER_OPEN));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_CDROM));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_REMOVABLE));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_FLOPPY));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_FIXED));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_USB));
	m_imageListFolder.Add(AfxGetApp()->LoadIcon(IDI_DRIVE_NET));
		
	SetImageList(&m_imageListFolder, TVSIL_NORMAL);

	m_imageListCheck.Create( 16, 16, ILC_COLOR8, 0, 0);

	m_imageListCheck.Add(AfxGetApp()->LoadIcon(IDI_UNCHECK));
	m_imageListCheck.Add(AfxGetApp()->LoadIcon(IDI_UNCHECK));
	m_imageListCheck.Add(AfxGetApp()->LoadIcon(IDI_CHECK));
	
	//SetImageList(&m_imageListCheck, TVSIL_STATE );

	//P2N("Inited tree control %p\r\n", this);
}


//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::AddDrive(char drivePathName[])

{
	char newDrive[_MAX_PATH], volumeName[_MAX_PATH];
	
	ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	
	double totalb, freeb;

	BOOL ret = GetDiskFreeSpaceEx(
			drivePathName,
			&FreeBytesAvailableToCaller,	// disk available to the caller
			&TotalNumberOfBytes,			// receives the number of bytes on disk
			&TotalNumberOfFreeBytes		// receives the free bytes on disk
		);

	if(ret)
		{
		totalb = largeuint2double(TotalNumberOfBytes);
		freeb = largeuint2double(FreeBytesAvailableToCaller);

		//P2N("totalb = %.0fGig freeb %.0f Gig\r\n", totalb / (1024*1024*1024), freeb/(1024*1024*1024));		
		}
	else
		{
		totalb = freeb = 0;
		//P2N("Cannot get vol size on %s\r\n", drivePathName);
		}

	totalb /= (1024*1024*1024);
	freeb /= (1024*1024*1024);

	if(!GetVolumeName(drivePathName, volumeName))
		{
		strcpy_s(volumeName, _MAX_PATH, "None");
		}
		
	//else
	//	sprintf(newDrive, "%c:", drivePathName[0]);
	
	sprintf_s(newDrive, "%c: [%s] %.0f Gig Total, %.0f Gig Free [%0d%%]", drivePathName[0], volumeName, 
				totalb, freeb, (int)(freeb/totalb * 100));

	//P2N("Adding Drive %s\r\n", newDrive);
	//DrawChildren();

	InsertChildFolder(newDrive, TVI_ROOT);
}

void CFolderTreeCtrl::AddAllDrive()

{
	DeleteAllItems( );

	uLong driveList = GetLogicalDrives();
	char drivePathName[] = " :\\";
	
	for(int i = 1 ; i < 32 ; i++)
	{
		if(0x01 & (driveList >> i))
		{
			drivePathName[0] = 'A' + i;
			AddDrive(drivePathName);
		}
	}

	SetProgText("");
}

int CFolderTreeCtrl::IsExistChildBold(HTREEITEM hItem)
{
	hItem = GetChildItem(hItem);
	
	while(hItem)
	{
		if(GetCheck(hItem))
			return 1;

		hItem = GetNextItem(hItem, TVGN_NEXT);	
	}

	return 0;
}


void CFolderTreeCtrl::VerifBoldParents(HTREEITEM hItem)

{
	HTREEITEM hParentItem = hItem;

	return;

	while(hParentItem = GetParentItem(hParentItem))
		{ 
		//SetCheck(hParentItem, IsExistChildBold(hParentItem));	
		}
			
	//SetItemState(hParentItem, IsExistChildBold(hParentItem) ? TVIS_BOLD: 0, TVIS_BOLD);
}

int CFolderTreeCtrl::IsAllChildFull(HTREEITEM hItem)
{
	hItem = GetChildItem(hItem);

	while(hItem)
	{
		if(GetCheck(hItem) == 0)
		return 0;

		hItem = GetNextItem(hItem, TVGN_NEXT);	
	}

	return 1;
}

int CFolderTreeCtrl::IsAllChildEmpty(HTREEITEM hItem)
{
	hItem = GetChildItem(hItem);

	while(hItem)
	{
		if(GetCheck(hItem))
		return 0;

		hItem = GetNextItem(hItem, TVGN_NEXT);	
	}

	return 1;
}

void CFolderTreeCtrl::VerifFullParents(HTREEITEM hItem)
{
	HTREEITEM hParentItem = hItem;

	return;

	while(hParentItem = GetParentItem(hParentItem))
	{
		if(IsAllChildFull(hParentItem))
		SetItemFull(hParentItem);
		else
		SetItemEmpty(hParentItem);
	}
}

//////////////////////////////////////////////////////////////////////////

int CFolderTreeCtrl::GetCheck(HTREEITEM hItem)

{
	int ret = CTreeCtrl::GetCheck(hItem);

#if 0
	unsigned long state = GetItemState(hItem, 0xFFFFFFFF);

	state &= 0x00003000;;
	int ret2 = (state >> 12) - 1;

	CString str = GetItemText( hItem );
	//P2N("Getting checked state %s, %d -- %d\r\n", str, ret, ret2);
#endif

	return ret;
}

////////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::SetItemEmpty(HTREEITEM hItem)
{
#if 0
	unsigned long state = GetItemState(hItem, 0xFFFFFFFF);
	unsigned long mask = 0x00003000^0xFFFFFFFF;
	state &= mask;// on mais les 2 bits à 0;
	state |= 4096;
	
	//SetItemState(hItem, state, 0xFFFFFFFF);
	//SetItemState(hItem, false ? TVIS_BOLD: 0, TVIS_BOLD);
#endif

	//SetCheck(hItem, false);
}

void CFolderTreeCtrl::SetItemFull(HTREEITEM hItem)

{
#if 0
	unsigned long state = GetItemState(hItem, 0xFFFFFFFF);
	unsigned long mask = 0x00003000^0xFFFFFFFF;
	state &= mask;// on mais les 2 bits à 0;
	state |= 8192;
#endif

	//SetItemState(hItem, state, 0xFFFFFFFF);
	//SetItemState(hItem, true ? TVIS_BOLD: 0, TVIS_BOLD);

	//SetCheck(hItem, true);
}

void CFolderTreeCtrl::SetItemAndChildrenEmpty(HTREEITEM hItem)
{
	SetItemEmpty(hItem);
	HTREEITEM hChildItem = GetChildItem(hItem);
	
	while(hChildItem)
		{
		SetItemEmpty(hChildItem);
		SetItemAndChildrenEmpty(hChildItem);

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
}

void CFolderTreeCtrl::SetItemAndChildrenFull(HTREEITEM hItem)
{
	SetItemFull(hItem);
	HTREEITEM hChildItem = GetChildItem(hItem);

	while(hChildItem)
		{
		SetItemFull(hChildItem);
		SetItemAndChildrenFull(hChildItem);

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
}

void CFolderTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags = 0;
	HTREEITEM hItem = HitTest(point, &uFlags);
	
	if(!hItem)
		return;
	
	SelectItem(hItem);
	SetFocus();
	OnSelchangedDirTree();

	if(uFlags & TVHT_ONITEMSTATEICON)
	{
		if(GetCheck(hItem))
			SetItemAndChildrenEmpty(hItem);
		else
			SetItemAndChildrenFull(hItem);
		
		VerifFullParents(hItem);
		VerifBoldParents(hItem);

		DrawNumSelectedFolder();
	
		return;
	}

	// Expand if the '+' button is hit
	if(uFlags & TVHT_ONITEMBUTTON )
		{
		Expand(hItem, TVE_TOGGLE);
		OnSelchangedDirTree();
		}
}

void CFolderTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags = 0;
	HTREEITEM hItem = HitTest(point, &uFlags);
	
	if(!hItem)
		{
		//MBOX("Please Right click on an active item");

		DrawContextMenu(point);
		return;
		}
	
	SelectItem(hItem);
	SetFocus();
	OnSelchangedDirTree();

	DrawContextMenu(hItem, point);
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::GetFolderPathName(HTREEITEM currentItem, char* folderPathName)

{
	folderPathName[0] = 0;

	char* buffer[1000];
	int count = 0;

	if(currentItem == TVI_ROOT)
	return;

	while(currentItem)
		{
		buffer[count] = new char[GetItemText(currentItem).GetLength() + 1];
		strcpy_s(buffer[count], _MAX_PATH, GetItemText(currentItem).GetBuffer(_MAX_PATH));
		count++;

		currentItem = GetParentItem(currentItem);
		}

	sprintf_s(folderPathName, _MAX_PATH, "%c:\\", buffer[count - 1][0]);

	for(int i = count-2 ; i >= 0 ; i--)
		{
		strcat_s(folderPathName, _MAX_PATH, buffer[i]);
		strcat_s(folderPathName, _MAX_PATH, "\\");
		}

	for(int ii = 0; ii < count; ii++)
		{
		ASSERT(buffer[ii]);
		delete[] buffer[ii];
		}
}

int CFolderTreeCtrl::IsValidFolder(char folderPathName[])

{
	// I think it's not the best method but it works !
	char oldCurrentDirectory[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, oldCurrentDirectory);

	if(!SetCurrentDirectory(folderPathName))
	{
		SetCurrentDirectory(oldCurrentDirectory);
		return 0;
	}

	SetCurrentDirectory(oldCurrentDirectory);
	return 1;
}

void CFolderTreeCtrl::DrawChildren(HTREEITEM parentItem)

{
	WIN32_FIND_DATA Find;
	HANDLE		hFind;
	char parentFolderPathName[_MAX_PATH];

	CString tmp;

	GetFolderPathName(parentItem, parentFolderPathName);

	if(parentFolderPathName[strlen(parentFolderPathName) - 1] != '\\')
		strcat_s(parentFolderPathName, _MAX_PATH, "\\");

	tmp.Format("%s*", parentFolderPathName);
	hFind = FindFirstFile(tmp, &Find);

	if(hFind == INVALID_HANDLE_VALUE)
		return;

	char currentFolderPathName[_MAX_PATH];
	
	while(hFind)
		{
		sprintf_s(currentFolderPathName, "%s%s", parentFolderPathName, Find.cFileName);

		//P2N("Scanning for folder %s\r\n", currentFolderPathName);

		if(IsValidFolder(currentFolderPathName) && strcmp(Find.cFileName, ".") != 0 && strcmp(Find.cFileName, "..") != 0)
			{
			InsertChildFolder(Find.cFileName, parentItem);

			if(GetCheck(parentItem))
				SetItemAndChildrenFull(parentItem);
			}

		if(!FindNextFile(hFind, &Find))
			{
			break;
			//return;
			}
		}

	FindClose(hFind);

	SetProgText("");
}

int CFolderTreeCtrl::HaveChildren(char parentFolderPathName[])

{
	WIN32_FIND_DATA Find;
	HANDLE		hFind;
	char TEMP[_MAX_PATH];

	if(parentFolderPathName[strlen(parentFolderPathName)-1] != '\\')
	//strcat_s(parentFolderPathName, 2, "\\");
	strcat_s(parentFolderPathName, _MAX_PATH, "\\");

	sprintf_s(TEMP, "%s*", parentFolderPathName);

	hFind = FindFirstFile(TEMP, &Find);
	if(hFind == INVALID_HANDLE_VALUE)
		return 0;

	char currentFolderPathName[_MAX_PATH];

	while(hFind)
		{
		sprintf_s(currentFolderPathName, "%s%s", parentFolderPathName, Find.cFileName);

		if(IsValidFolder(currentFolderPathName) && strcmp(Find.cFileName, ".") != 0 && strcmp(Find.cFileName, "..") != 0)
		return 1;

		if(!FindNextFile(hFind, &Find))
			break;

			//return 0;
		}

	FindClose(hFind);

	return 0;
}

void CFolderTreeCtrl::OnSelchangedDirTree() 

{
	HTREEITEM hItem = GetSelectedItem();
	TVITEM tv;
	memset(&tv, 0, sizeof(tv));
	tv.mask = TVIF_HANDLE;
	tv.hItem = hItem;
	GetItem(&tv);

	//if(tv.state | TVIS_EXPANDED)
	//	P2N("Expanded Item\r\n");

	//DrawChildren(hItem);
	//DrawNumSelectedFolder();
	//SelectItem(hItem);

	char	PathName[_MAX_PATH];

	GetFolderPathName(hItem, PathName);

	//SetProgText(PathName);
	SetResText(PathName);
	m_result = PathName;
}

void CFolderTreeCtrl::GetNumSelectedFolder(HTREEITEM currentItem, uLong* numSelectedFolder)

{	
	HTREEITEM last = 0;
	HTREEITEM hChildItem = GetChildItem(currentItem);

	// Seek to last
	while(hChildItem)
		{
		last = hChildItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}

	hChildItem = last;

	while(hChildItem)
		{		
		if(GetCheck(hChildItem))
			{
			(*numSelectedFolder)++;
			}
		GetNumSelectedFolder(hChildItem, numSelectedFolder);
		hChildItem = GetNextItem(hChildItem, TVGN_PREVIOUS);
		}

	char folderPathName[_MAX_PATH];

	GetFolderPathName(currentItem, folderPathName);

	//if(!IsFolder1IsInFolder2(LastDir, folderPathName))

	if(GetCheck(currentItem))
		P2N("Terminal %s\r\n", folderPathName);

	LastDir = folderPathName;
}

////////////////////////////////////////////////////////////////////////////

uLong CFolderTreeCtrl::GetNumSelectedFolder()

{
	uLong numSelectedFolder = 0;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = GetChildItem(TVI_ROOT);

	//P2N("Counting number of selected folders\r\n");

	// Seek to last
	while(hChildItem)
		{
		last = hChildItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}

	hChildItem = last;

	while(hChildItem)
		{
		//if(TVIS_BOLD & GetItemState(hChildItem, TVIS_BOLD))

		if(GetCheck(hChildItem))
			{
			numSelectedFolder++;
			}
		GetNumSelectedFolder(hChildItem, &numSelectedFolder);
		
		hChildItem = GetNextItem(hChildItem, TVGN_PREVIOUS);
		}

	return numSelectedFolder;
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::DrawNumSelectedFolder()
{
	if(!p_state)
	return;

	char sNumSelectedFolder[_MAX_PATH];
	sprintf_s(sNumSelectedFolder, "%d Selected Folder(s)", GetNumSelectedFolder());
	p_state->SetWindowText(sNumSelectedFolder);
}

void CFolderTreeCtrl::SelectFolder(char folderPathName[])
{
	char folderName[_MAX_PATH];

	int deep = GetDeepFolderPathName(folderPathName);
	HTREEITEM parentItem = TVI_ROOT;

	for(int i = 0 ; i < deep ; i++)
	{
		CutFolderPathName(folderPathName, i, folderName);
		parentItem = InsertChildFolder(folderName, parentItem);
		DrawChildren(parentItem);
	}

	SetItemAndChildrenFull(parentItem);

	VerifFullParents(parentItem);
	VerifBoldParents(parentItem);

	DrawNumSelectedFolder();
}

int CFolderTreeCtrl::GetDeepFolderPathName(char folderPathName[])
{
	int deep = 0;

	for(int i = 0 ; folderPathName[i] ; i++)
		if(folderPathName[i] == '\\')
		deep++;

	return deep;
}



void CFolderTreeCtrl::CutFolderPathName(char folderPathName[], int deep, char folderName[])
{
	int start = 0, currentDeep = 0;
	
	while(currentDeep < deep)
	{
		if(folderPathName[start++] == '\\')
		currentDeep++;	
	}

	int k = 0;
	for(int i = start ; folderPathName[i] != '\\' ; i++)
	folderName[k++] = folderPathName[i];

	folderName[k] = 0;
}

//////////////////////////////////////////////////////////////////////////

HTREEITEM CFolderTreeCtrl::InsertChildFolder(char childFolderName[], HTREEITEM parentItem)

{
	/* We search if it already exists */
	
	((CTimedimDlg *)AfxGetApp()->m_pMainWnd)->busy = true;

	int haveChildren;
	HTREEITEM fake = NULL;	

	HTREEITEM pCurrentItem = GetChildItem(parentItem);

	while(pCurrentItem)
		{
		if(strcmp(childFolderName, GetItemText(pCurrentItem).GetBuffer(_MAX_PATH)) == 0)
			{	
			//P2N("Child already exists %s\r\n", childFolderName);
			goto endd;
			}
		pCurrentItem = GetNextItem(pCurrentItem, TVGN_NEXT);
		}

	pCurrentItem = GetChildItem(parentItem);

	while(pCurrentItem)
		{
		if(strcmp("<Empty>", GetItemText(pCurrentItem).GetBuffer(_MAX_PATH)) == 0)
			{	
			//P2N("Empty %s\r\n", GetItemText(pCurrentItem).GetBuffer(_MAX_PATH));
			fake = pCurrentItem;
			break;
			}
		pCurrentItem = GetNextItem(pCurrentItem, TVGN_NEXT);
		}

	if(fake)
		{
		DeleteItem(fake);
		}

	pCurrentItem = NULL;

	/* We looking for if the folder have children */

	char parentFolderPathName[_MAX_PATH];
	GetFolderPathName(parentItem, parentFolderPathName);
	strcat_s(parentFolderPathName, childFolderName);
	haveChildren = HaveChildren(parentFolderPathName);

	/* Sinon on l'insert */
	if(parentItem == TVI_ROOT)
		{
		CString drv = childFolderName;	drv = drv.Left(3);
		int type = GetDriveType(drv);
		//P2N("Drive type on init tree %s %d\r\n", drv, type);
		
		switch(type)
			{
			case DRIVE_FIXED:
				{
				//P2N("USB+ICON %s\r\n", childFolderName);
				if(GetBustype(childFolderName) == BusTypeUsb)
					pCurrentItem = InsertItem(childFolderName, 6, 6, parentItem, TVI_LAST, haveChildren);
				else
					pCurrentItem = InsertItem(childFolderName, 5, 5, parentItem, TVI_LAST, haveChildren);
				}
				break;
			
			case DRIVE_CDROM:
				pCurrentItem = InsertItem(childFolderName, 2, 2, parentItem, TVI_LAST, haveChildren);
				break;
		
			case DRIVE_REMOVABLE:
				if(childFolderName[0]  ==  'A')
					pCurrentItem = InsertItem(childFolderName, 4, 4, parentItem, TVI_LAST, haveChildren);
				else
					pCurrentItem = InsertItem(childFolderName, 3, 3, parentItem, TVI_LAST, haveChildren);
				break;

			case DRIVE_REMOTE:
				pCurrentItem = InsertItem(childFolderName, 7, 7, parentItem, TVI_LAST, haveChildren);
				break;

			}

		//P2N("Scanning drive: %s\r\n", childFolderName);

		if(pCurrentItem  == NULL)
			{
			pCurrentItem = InsertItem(childFolderName, 5, 5, parentItem, TVI_LAST, haveChildren);
			}	
		
		// Insert fake entry into the drive 
		pCurrentItem = InsertItem("<Empty>", 0, 1, pCurrentItem, TVI_LAST, haveChildren);

		m_progstr.Format("Scanning: %s", childFolderName);
		}
	else
		{
		pCurrentItem = InsertItem(childFolderName, 0, 1, parentItem, TVI_LAST, haveChildren);
		//P2N("Scanning folder: %s\r\n", childFolderName);		
		m_progstr.Format("Scanning: %s", childFolderName);
		}

endd:

	SetProgText(m_progstr);
	((CTimedimDlg *)AfxGetApp()->m_pMainWnd)->busy = false;
	return pCurrentItem;
}

//////////////////////////////////////////////////////////////////////////

HTREEITEM CFolderTreeCtrl::InsertItem(char lpszItem[], int nImage, int nSelectedIamage, HTREEITEM hParent, HTREEITEM hInsertAfter, int haveChildren)

{
	if(!haveChildren)
	return CTreeCtrl::InsertItem(lpszItem, nImage, nSelectedIamage, hParent, hInsertAfter);

	TV_INSERTSTRUCT tvis;

	ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
	tvis.hParent			 = hParent;
	tvis.hInsertAfter		 = TVI_LAST;
	tvis.item.mask			 = TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	//tvis.item.mask			 = TVIF_CHILDREN|TVIF_IMAGE|TVIF_TEXT;
	tvis.item.pszText		 = lpszItem;
	tvis.item.iImage		 = nImage;
	tvis.item.iSelectedImage = nSelectedIamage;
	tvis.item.cChildren		 = TRUE;
		
	return CTreeCtrl::InsertItem(&tvis);
}

void CFolderTreeCtrl::DrawContextMenu(HTREEITEM currentItem, CPoint pos)

{
	Menu.DestroyMenu();
	Menu.CreatePopupMenu();

	CString itemText = GetItemText(currentItem);
	
	//if(GetParentItem(currentItem) == NULL)
	//	sprintf(itemText,"Display Disk '%c'", GetItemText(currentItem).GetBuffer(100)[0]);
	//else

	CString tmp; tmp.Format("Display '%s' in Explorer", itemText); 
	Menu.AppendMenu(MF_STRING, IDM_EVENT_OPEN, tmp);

	CString tmp2; tmp2.Format("Make new directory under '%s'", itemText); 
	Menu.AppendMenu(MF_STRING, IDM_EVENT_MKDIR, tmp2);

	Menu.AppendMenu(MF_SEPARATOR);
	
	CString tmp3; tmp3.Format("Refresh Current Branch"); 
	Menu.AppendMenu(MF_STRING, IDM_EVENT_REFRESH, tmp3);

	CString tmp4; tmp4.Format("Refresh All"); 
	Menu.AppendMenu(MF_STRING, IDM_EVENT_REFRESH_ALL, tmp4);

	
	//if(!GetCheck(currentItem))
	//	Menu.AppendMenu(MF_STRING , IDM_EVENT_SELECTALL, "Select this folder and all SubFolder");
	//else
	//	Menu.AppendMenu(MF_STRING , IDM_EVENT_UNSELECTALL, "Deselect this folder and all SubFolder");
	//
	//Menu.AppendMenu(MF_SEPARATOR);
	
	//if(IsExistChildBold(currentItem))
	//	InsertSubFolderSelectedInMenu(currentItem);
	//else
	//	Menu.AppendMenu(MF_STRING, 0, "0 Subfolder Selected");

	CRect rect;
	this->GetWindowRect(&rect);
	Menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + pos.x, rect.top + pos.y, this);
}

//////////////////////////////////////////////////////////////////////////
// Draw context menu without selection

void CFolderTreeCtrl::DrawContextMenu(CPoint pos)

{
	Menu.DestroyMenu();
	Menu.CreatePopupMenu();

	CString tmp4; tmp4.Format("Refresh All"); 
	Menu.AppendMenu(MF_STRING, IDM_EVENT_REFRESH_ALL, tmp4);

	CRect rect;
	this->GetWindowRect(&rect);
	Menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + pos.x, rect.top + pos.y, this);
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::InsertSubFolderSelectedInMenu(HTREEITEM currentItem)

{
	currentItem = GetChildItem(currentItem);
	
	while(currentItem)
	{
		if(GetCheck(currentItem))
		{
			char folderPathName[_MAX_PATH];
			GetFolderPathName(currentItem,folderPathName);
			Menu.AppendMenu(MF_STRING, IDM_EVENT, folderPathName);
		}

		InsertSubFolderSelectedInMenu(currentItem);
		currentItem = GetNextItem(currentItem, TVGN_NEXT);
	}
}

void CFolderTreeCtrl::MenuEventOpen(CCmdUI* pCmdUI)

{
	char folderPathName[_MAX_PATH];
	GetFolderPathName(GetSelectedItem(), folderPathName);
	ShellExecute(NULL,NULL, folderPathName, NULL,NULL,SW_SHOW);
}

void CFolderTreeCtrl::MenuEventMkdir(CCmdUI* pCmdUI)

{
	char folderPathName[_MAX_PATH];
	GetFolderPathName(GetSelectedItem(), folderPathName);
	
	CGetDirName dn;

	dn.txt = folderPathName;
	ShortenPath(dn.txt, 52);

	if(dn.DoModal() == IDCANCEL)
		return;

	CString newdir;
	newdir = folderPathName; newdir += dn.m_dir;

	//MBOXP("Making '%s'", newdir);

	if(IsDir(newdir))
		{
		MBOXP("This directory already exists ('%s')", newdir);
		return;
		}
	else
		{
		int ret = _mkdir(newdir);
		if(ret < 0)
			{
			char errx[_MAX_PATH];
			strerror_s(errx, _MAX_PATH, errno);
			MBOXP("Error on creating new dir: '%s' Error description: %s", newdir,
				errx);
			}
		}
	
	Refresh();

	// Goto newly creted dir
	SetCurr(newdir + "\\");
	m_result = newdir;
}


void CFolderTreeCtrl::MenuEventRefresh(CCmdUI* pCmdUI)

{
	//char folderPathName[_MAX_PATH];	
	//GetFolderPathName(GetSelectedItem(), folderPathName);
	//P2N("Original tree item (before refresh) %s\r\n",  folderPathName);
	
	Refresh();	

	//AddAllDrive();
	// Go back to original place
}

void CFolderTreeCtrl::MenuEventRefreshAll(CCmdUI* pCmdUI)

{
	//Refresh();	
	AddAllDrive();
	// Go back to original place
}


void CFolderTreeCtrl::Refresh()

{
	HTREEITEM delitem, currentItem = GetSelectedItem();
	int itemcnt = 0;
	
	if(!currentItem)
		return;

	//if(!GetParentItem(currentItem))
	//	return;

	//P2N("Refresh tree.\r\n");

	// Make sure current entries have existing folders
	HTREEITEM pCurrentItem = GetChildItem(currentItem);

	while(pCurrentItem)
		{
		char folderPathName[_MAX_PATH];
		GetFolderPathName(pCurrentItem, folderPathName);

		if(!IsValidFolder(folderPathName))
			{
			delitem = pCurrentItem;
			}
		pCurrentItem = GetNextItem(pCurrentItem, TVGN_NEXT);
		
		if (!IsValidFolder(folderPathName))
		{
			DeleteItem(delitem);
		}

		itemcnt++;
		}

	DrawChildren(currentItem);
}


void CFolderTreeCtrl::MenuEventSelectAll(CCmdUI* pCmdUI)

{
	HTREEITEM currentItem = GetSelectedItem();
	SetItemAndChildrenFull(currentItem);
	
	VerifFullParents(currentItem);
	VerifBoldParents(currentItem);

	DrawNumSelectedFolder();
}

void CFolderTreeCtrl::MenuEventUnSelectAll(CCmdUI* pCmdUI)

{
	HTREEITEM currentItem = GetSelectedItem();
	SetItemAndChildrenEmpty(currentItem);

	VerifFullParents(currentItem);
	VerifBoldParents(currentItem);

	DrawNumSelectedFolder();
}

//////////////////////////////////////////////////////////////////////////

int CFolderTreeCtrl::GetVolumeName(char rootDrive[], char volumeName[])

{
	char FileSystemNameBuffer[_MAX_PATH];

	//CString inname(rootDrive);
	//inname.MakeLower();

	//inname = inname.Mid(0, 2);

	DWORD VolumeSerialNumber, MaximumComponentLength, FileSystemFlags;

	int ret = GetVolumeInformation(rootDrive, 
			volumeName, _MAX_PATH,
				&VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, 
					FileSystemNameBuffer, _MAX_PATH);

	//if(!ret)
	//	{
	//	CString str; str.Format("%s %s", inname, strerror(GetLastError()));
	//	AfxMessageBox(str);
	//	}

	//&& strlen(volumeName) != 0;

	return ret;	
}

int CFolderTreeCtrl::IsFolder1IsInFolder2(const char folderPathName1[], 
													const char folderPathName2[])

{
	unsigned int len = strlen(folderPathName2);

	for(uLong i = 0; i < len;i++)
		{
		if(folderPathName1[i] != folderPathName2[i])
		return 0;
		}
	return 1;
}

void CFolderTreeCtrl::SetProgText(const char *text)

{
	CString  str(text);
	ShortenPath(str, 56);
	if(prog)
		{	
		prog->SetText(str);
		prog->Invalidate();
		RECT rect;
		prog->GetWindowRect(&rect);
		GetParent()->InvalidateRect(&rect);
		YieldToWinEx();
		Sleep(20);
		}
}

void CFolderTreeCtrl::SetResText(const char *text)

{
	CString  str(text);
	ShortenPath(str, 56);
	if(prog)
		{	
		result->SetText(str);
		result->Invalidate();
		RECT rect;
		result->GetWindowRect(&rect);
		GetParent()->InvalidateRect(&rect);
		YieldToWinEx();
		Sleep(20);
		}
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::Iterate(PFVC func, HTREEITEM currentItem )

{
	uLong numSelectedFolder = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = GetChildItem(currentItem);

	//P2N("Counting number of selected folders\r\n");

#if 0
	// Seek to last
	while(hChildItem)
		{
		last = hChildItem;
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}

	hChildItem = last;
#endif

	char	fname[_MAX_PATH];

	while(hChildItem)
		{
		//if(TVIS_BOLD & GetItemState(hChildItem, TVIS_BOLD))

		if(GetCheck(hChildItem))
			{
			numSelectedFolder++;

			GetFolderPathName(hChildItem, fname);

			// P2N("sel: %s\r\n", str);
			(func)(fname);
			}
			
		Iterate(func, hChildItem);

		if(GetCheck(hChildItem))
			{
			CString str = GetItemText(hChildItem);
			//P2N("Terminal: %s\r\n", str);
			}
			
#if 0
		hChildItem = GetNextItem(hChildItem, TVGN_PREVIOUS);
#endif

		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::OnSetFocus(CWnd *pOldWnd)

{
	//P2N("CFolderTreeCtrl::OnSetFocus(%p) %p\r\n", pOldWnd, this);
	
	//Refresh();
	
	CTreeCtrl::OnSetFocus(pOldWnd);
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	//P2N("CFolderTreeCtrl::OnKeyDown %c (%d)\r\n", nChar, nChar);

	switch(nChar)
		{
		case VK_RIGHT:
			{
			//P2N("OnKeyDown right arrow\r\n");
			}
			break;

		case VK_F5:
			//P2N("F5 function key\r\n");
			AddAllDrive();
			break;
		}

	HTREEITEM old = GetSelectedItem();

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	
	// Eval after every key press
	if(GetSelectedItem() != old)
		{
		//P2N("sel changed on keystroke\r\n");
		OnSelchangedDirTree();
		}
}

//////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )

{
	//P2N("CFolderTreeCtrl::OnChar %c\r\n", nChar);

	HTREEITEM old = GetSelectedItem();

	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags );

	// Eval after every key press
	if(GetSelectedItem() != old)
		{
		//P2N("sel changed on keystroke\r\n");
		OnSelchangedDirTree();
		}

}
 

void CFolderTreeCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	//P2N("CFolderTreeCtrl::OnSysChar %c\r\n", nChar);
	CTreeCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

///////////////////////////////////////////////////////////////////////////

void CFolderTreeCtrl::SetCurr(const char *str)

{
	int progidx = 2;
	int found = false, found2 = false;
	
	//P2N("Selecting folder '%s' in tree control\r\n", str);

	int	alen = strlen(str);

	CString rname, name2;

	//CWaitCursor cursor;

	//HTREEITEM hChildItem2; 

	HTREEITEM hChildItem = GetChildItem(TVI_ROOT);

	// Seek to find root entry
	while(hChildItem)
		{
		rname = GetItemText(hChildItem);

		//P2N("Root Item %s \r\n", rname);

		rname = rname.Left(2);
		if(_strnicmp(rname, str, rname.GetLength()) == 0)
			{
			found = true;
			//P2N("Found item\r\n");
			break;
			}
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}

	rname += "\\";

	if(!found)
		return;
			
	SelectItem(hChildItem);		
	Expand(hChildItem, TVE_EXPAND);
	
	while(true)
		{
		SelectItem(hChildItem);
		
		hChildItem = GetChildItem(hChildItem);
		found2 = false;
		
		// Seek to find next entry
		while(hChildItem)
			{
			//Sleep(50);

			CString name = GetItemText(hChildItem);

			name2 = rname + name + "\\";
			
			if(_strnicmp(name2, str, name2.GetLength()) == 0)
				{
				//P2N("Found new %s\r\n", name2);

				found2 = true;
				SelectItem(hChildItem);
				Expand(hChildItem, TVE_EXPAND);

				rname = name2;
				break;
				}

			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
			}

		if(found2 == false)
			{
			//P2N("Break at NOT found2\r\n");
			break;
			}

		if(alen <= name2.GetLength())
			{
			//P2N("Break at getlen\r\n");
		
			SelectItem(hChildItem);
			//SetFocus();
			OnSelchangedDirTree();
			break;
			}
		}

	//P2N("Opened %s\r\n", name2);
}


//////////////////////////////////////////////////////////////////////////
// Trick to get 32 bit operation is making two 16 bit shifts
//


