/******************************************************************/
/*** Class to reprisent and select folders contain in a drive   ***/
/***                                                            ***/
/*** Use this class like you want,  it's free !                 ***/
/***                                                            ***/
/***                                            By Adrien Pinet ***/
/******************************************************************/


#ifndef __CFOLDERTREECTRL_H__
#define __CFOLDERTREECTRL_H__

typedef unsigned long uLong;

typedef void (*PFVC)(const char *);

class CFolderTreeCtrl : public CTreeCtrl
{
public:

	void Refresh();

	int		prompt;
	CString		m_progstr;

	void SetResText(const char *text);

	CFolderTreeCtrl(); // default constructor
	virtual ~CFolderTreeCtrl();
	
	void Init(); // Load icons and initialize the tree
	void SetState(CEdit* p_state);

	void AddAllDrive(); // Add All Drive Letter
	void AddDrive(char drivePathName[]); // Add a drive in the tree
	uLong GetNumSelectedFolder(); // Return num of selected folder

	void SelectFolder(char folderPathName[]); // Select a folder

	CString LastDir;
	CString	m_result;

	int CFolderTreeCtrl::IsFolder1IsInFolder2(const char folderPathName1[], 
													const char folderPathName2[]);

protected:
	//{{AFX_MSG(ComboTreeDropList)

	afx_msg void OnLButtonDown(UINT nFlags,  CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,  CPoint point);
	afx_msg void MenuEventOpen(CCmdUI* pCmdUI);
	afx_msg void MenuEventMkdir(CCmdUI* pCmdUI);
	afx_msg void MenuEventRefresh(CCmdUI* pCmdUI);
	afx_msg void MenuEventRefreshAll(CCmdUI* pCmdUI);
	afx_msg void MenuEventSelectAll(CCmdUI* pCmdUI);
	afx_msg void MenuEventUnSelectAll(CCmdUI* pCmdUI);
	afx_msg	void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnSetFocus( CWnd* pOldWnd );
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetProgText(const char * text);

	CLabel	*prog;
	CLabel	*result;

	int GetCheck(HTREEITEM hItem); // return the item/folder state 1 if it's checked 0 otherwise

//private:

	/*************************************************************************/
	/*                 Functions to change and get item state                */
	/*************************************************************************/
	
	void SetItemFull(HTREEITEM hItem); // check an item/folder
	void SetItemAndChildrenFull(HTREEITEM hItem); // check an item/folder and subitems and subfolders

	void SetItemEmpty(HTREEITEM hItem); // uncheck an item/folder
	void SetItemAndChildrenEmpty(HTREEITEM hItem); // uncheck an item/folder and subitems and subfolders

	void VerifBoldParents(HTREEITEM hItem); // verify if all parent of a checked item/folder is bold
	void VerifFullParents(HTREEITEM hItem); // verify if all children of a item/folder are ckecked

	int IsExistChildBold(HTREEITEM hItem); // return 1 if an item/folder have a bold children 0 otherwise
	int IsAllChildFull(HTREEITEM hItem); // return 1 if an item/folder have all children are checked 0 otherwise
	int IsAllChildEmpty(HTREEITEM hItem); // return 1 if an item/folder have all children are unchecked 0 otherwise

	int IsValidFolder(char folderPathName[]);

	void GetFolderPathName(HTREEITEM currentItem, char* folderPathName); // folderPathName contain the physical path of an item/folder
	void DrawChildren(HTREEITEM parentItem); // add and raw all children of an item/folder
	int HaveChildren(char parentFolderPathName[]); // return 1 if parentFolderPathName have children 0 otherwise 

	
	void OnSelchangedDirTree(); // Display all children of a selected item/folder

public:
	void SetCurr(const char * str);
	
	void Iterate(PFVC func, HTREEITEM currentItem = 0);
	
	/*************************************************************************/
	/*           Functions to count and draw num of selected folder          */
	/*************************************************************************/
	void GetNumSelectedFolder(HTREEITEM currentItem, uLong* numSelectedFolder);
	void DrawNumSelectedFolder();


	/*************************************************************************/
	/*                   Functions to preselect a folder                     */
	/*************************************************************************/
	void CutFolderPathName(char folderPathName[], int deep, char folderName[]); // extract a folder name from a string at the position 'deep'
	int  GetDeepFolderPathName(char folderPathName[]); // Count the deeper of a folderPathName string
	HTREEITEM InsertChildFolder(char childFolderName[], HTREEITEM parentItem); // insert a string in an item and return the handle of the new child (return the old child if it already exists)
	HTREEITEM InsertItem(char lpszItem[], int nImage, int nSelectedIamage, HTREEITEM hParent, HTREEITEM hInsertAfter, int haveChildren); // insert an item and verify if it must have a 'plus-box'


	/*************************************************************************/
	/*       Functions to display the context menu from right clic event     */
	/*************************************************************************/
	void DrawContextMenu(HTREEITEM currentItem, CPoint pos); // display all child selected folder from the currentItem
	void CFolderTreeCtrl::DrawContextMenu(CPoint pos);
	void InsertSubFolderSelectedInMenu(HTREEITEM currentItem); // a long function name ;)

	/*************************************************************************/
	/*                              Other function                           */
	/*************************************************************************/
	int GetVolumeName(char rootDrive[], char volumeName[]);

private:
	CMenu Menu;
	CEdit* p_state; // p_state is a pointer to a CEdit area to display the num of selected folder
	CImageList m_imageListFolder; // Image List that contain the open and close folder icons
	CImageList m_imageListCheck; // Image List that contain the checked and unchecked folder icons
};

#endif