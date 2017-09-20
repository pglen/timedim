
/* =====[ timedim.cpp ]========================================== 
                                                                             
   Description:     The timedim project, implementation of the timedim.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  8/12/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// timedim.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "timedim.h"
#include "timedimDlg.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTimedimDlg* pMainDialog;

CString approot;
CString	appdata;

CFile datalog;

//////////////////////////////////////////////////////////////////////////
// Logging subsystem

void	open_datalog()

{
	CString str(appdata);

	if(!datalog.Open(str + "activity.txt", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone))
		{
		P2N("Cannot open 'datalog' log file, Logging will not be available.\r\n");
		MBOX("Cannot open 'datalog' log file, Logging will not be available.\r\n");
		}
	else
		{
		datalog.Seek(0, CFile::end );
		}
	
	PrintToLog("Started TimeDim Application Version 2.0\r\n");
}

void	close_datalog()

{
	PrintToLog("Ended TimeDim Application\r\n");
	datalog.Close();
}

static 	char    szOutString[1000];

//////////////////////////////////////////////////////////////////////////

int     PrintToLog(const char *Format, ...)

{
    char    *asc = NULL;

	// Do nothing if log is not opened
	if(datalog.m_hFile == CFile::hFileNull )
		{
		P2N("Log attempted to NULL file\r\n");
		return 0;
		}

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format("%A, %B %d, %Y - %I:%M %p ");
	
	datalog.Write(datestr, datestr.GetLength());

    va_list ArgList;
    va_start(ArgList, Format);
    int len = _vsnprintf_s(szOutString, sizeof(szOutString), Format, ArgList);
	datalog.Write(szOutString, len);

    return(0);
}

/////////////////////////////////////////////////////////////////////////////
// CTimedimApp

BEGIN_MESSAGE_MAP(CTimedimApp, CWinApp)
	//{{AFX_MSG_MAP(CTimedimApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimedimApp construction

CTimedimApp::CTimedimApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTimedimApp object

CTimedimApp theApp;

CString argarr[ARGARRSIZE];

//////////////////////////////////////////////////////////////////////////
//
// Function name:    InterpretComandLine
// Description:      <empty description>
// Return type:      
// Argument:         CString *arr
// Argument:         int lim
//
//////////////////////////////////////////////////////////////////////////

int		InterpretComandLine(CString *arr, int lim)

{	
	CTimedimDlg * mainwin = (CTimedimDlg *)AfxGetApp()->m_pMainWnd;

	for(int loop = 0; loop < lim; loop++)
		{
		if(arr[loop] == "/p")
			{
			mainwin->m_check1 = true;
			}
		if(arr[loop] == "/b")
			{
			mainwin->m_check2 = true;
			}
		if(arr[loop] == "/s")
			{
			mainwin->m_check3 = true;
			}
		if (arr[loop] == "/v")
		{
			mainwin->m_check4 = true;
		}
		if (arr[loop] == "/r")
		{
			mainwin->m_check6 = true;
		}
		if(arr[loop] == "/f")
			{
			dosession = arr[loop + 1];
			}
		}

	mainwin->UpdateData(false);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    ParseComandLine
// Description:      Parse command line strings into a string array
//					  Obeys '\ ' '\"' "str" escapes -- similar to W32 cmd
// Return type:      
// Argument:         const char *cmdline
// Argument:         CString *arr
// Argument:         int lim
//
//////////////////////////////////////////////////////////////////////////

int		ParseComandLine(const char *cmdline, CString *arr, int lim)

{
	int in_quote = 0, esc = 0, idx = 0;
	int loop, len = strlen(cmdline);

	const char *cmd = cmdline;

	char prev = 0;

	for(loop = 0; loop < len; loop++)
		{
		char  chh = *cmdline++;

		switch(chh)
			{
			case '\"':
				if(esc)
					{
					esc = false;
					arr[idx] += chh;
					}
				else
					{
					if(in_quote)
						in_quote = false;
					else
						in_quote = true;
					}
				break;

			case '\\':
				if(esc)
					{
					// Double backslash
					arr[idx] += chh;
					esc = false;
					}
				else
					{
					esc = true;
					}
				break;
		
			case ' ':
				if(esc)
					arr[idx] += chh;	
				else
					{
					if(in_quote)
						arr[idx] += chh;	
					else
						{
						// Only skip on one space
						if(prev != ' ')
							idx ++;
						}
					}
				break;

			default:
				// See if we have a dangleing escape, output prev
				if(esc)
					{	
					esc = false;
					arr[idx] += prev; 	
					}
					
				arr[idx] += chh;
			}

		prev = chh;

		// Terminating condition
		if(idx >= lim)
			break;
		}

	// For testing purposes
#if 0
	for(loop = 0; loop < lim; loop++)
		{
		P2N("String array %d '%s'\r\n", loop, arr[loop]);
		}
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void sighandle(int sig)

{
	P2N("Signal Received\r\n");
}

#include "signal.h"

/////////////////////////////////////////////////////////////////////////////
// CTimedimApp initialization

BOOL CTimedimApp::InitInstance()
{
	if (!AfxSocketInit())
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	AfxEnableControlContainer();

	//signal(SIGINT, sighandle);
	//raise(SIGINT);

	// Get where the application lives
	char *buff = approot.GetBuffer(MAX_PATH + 1);
	GetModuleFileName(AfxGetInstanceHandle(), buff, MAX_PATH);

	// get application root
	char *ptr = strrchr(buff, '\\');
	if(ptr)
		*(ptr+1) = '\0';

	approot.ReleaseBuffer();
	
	// Get where the application data is:
	GetSpecialFolder(CSIDL_APPDATA,	appdata);
	appdata += "timedim";

	if(!IsDir(appdata))
		_mkdir(appdata);

	appdata += "\\";

	//P2N("approot='%s' appdata='%s'\r\n", approot, appdata);
	//P2N("Clock %u cps: %u\r\n", clock(), CLOCKS_PER_SEC);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	//P2N("Init Timedim App Commmand line is: '%s'\r\n", m_lpCmdLine);


	CTimedimDlg *dlg = new CTimedimDlg;

	m_pMainWnd = dlg;
	pMainDialog = dlg;

	int nResponse = dlg->DoModal();

#if 0
	if (nResponse == IDOK)
		{
		}
	else if (nResponse == IDCANCEL)
		{
		}
#endif

	delete dlg;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTimedimApp::ExitInstance() 

{
	P2N("Exited TimeDim App\r\n");

	//PrintToLog("Exited TimeDim Application.");
	
	close_datalog();

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    heapcheck
// Description:      Check heap status 
// Return type:      
// Argument:         const char *str	-- string to display
// Argument:         int showok			-- flag to show messages if OK 
//
//////////////////////////////////////////////////////////////////////////

#include "malloc.h"


int		heapcheck(const char *str, int showok)

{	 
	CString str2 = str;
	
	int heapstatus = _heapchk();

	//if(str2 == "")
	//	{
	//	str2.Format("%s -- line :: %d ", __FILE__ , __LINE__);
	//	}

   //P2N("Heap check ret = %d\r\n", _heapchk());

	switch( heapstatus )
		{
		case _HEAPOK:
			if(showok)
				P2N("%s Heap Check -- OK - heap is fine\r\n", str2);
			break;

		case _HEAPEMPTY:
			P2N("%s Heap Check -- OK - heap is empty\r\n", str2);
			break;

		case _HEAPBADBEGIN:
			P2N( "%s Heap Check --ERROR - bad start of heap\r\n", str2);
			break;

		case _HEAPBADNODE:
			P2N( "%s Heap Check --ERROR - bad node in heap\r\n", str2);
			break;

		default:
			P2N("%s UKN - heap ret code\r\n", str2);
			break;
		}

   return 0;
}

///////////////////////////////////////////////////////////////////////////
// YieldToWinEx
// Let every accumulated message go through
// Return 1 if ESC | Ctrl-Break pressed -- 2 if EXITING


int     YieldToWinEx()

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
           break;

        // Ctrl-Break
        if(msg.message == WM_KEYDOWN)
            {
            if(msg.wParam == VK_CANCEL)
                {
                ret_val = 1; break;
                }
            }
        // Esc
        if(msg.message == WM_CHAR)
            {
            if(msg.wParam == VK_ESCAPE)
                {
                ret_val = 1; break;
                }
            }
        // App exit
        if(msg.message == WM_QUIT)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // App kill
        if(msg.message == WM_DESTROY)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // Every other message is processed
        TranslateMessage(&msg); DispatchMessage(&msg);
        }
    return(ret_val);
}

/////////////////////////////////////////////////////////////////////////////
//  void	ShortenPath(CString &str, int len)
// Shorten path to len

void	ShortenPath(CString &str, int len)

{
	int slen = str.GetLength();

	// Nothing to do
	if(slen <= len)
		return;

	CString lstr = str.Left ((len-5)/2);
	CString rstr = str.Right((len-5)/2);
	str = lstr + " ... " + rstr;
}


/////////////////////////////////////////////////////////////////////////////
// Wrapper for split path

void	SplitPath(CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext)

{
	//char sbuffer[_MAX_PATH + 1];
	char sdrive[_MAX_DRIVE];
	char sdir[_MAX_DIR];
	char sfname[_MAX_FNAME];
	char sext[_MAX_EXT];
	
	//strncpy(sbuffer, full, MAX_PATH);

	//Parse it
	_splitpath_s(full, sdrive, sdir, sfname, sext);

	//P2N("path components sdrive=%s sdir=%s  sfname=%s sext=%s\r\n",
	//						sdrive, sdir, sfname, sext);

	drive = sdrive;
    dir   = sdir;
    fname = sfname;
    ext   = sext;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    IsDir
// Description:      return true if it is a directory 
// Return type:      
// Argument:         const char *str
//
//////////////////////////////////////////////////////////////////////////

int		IsDir(const char *str)

{
	int ret = 0;		// Default to NOT
	struct _stat buff; 

	CString str2(str);

	if(str2 == "")
		goto endd;

	// Repair path if it ends in backslash
	if(str2[str2.GetLength() - 1] == '\\')
		{
		if(str2[str2.GetLength() - 2] != ':')
			{
			//P2N("backslash on IsDir()\r\n");
			str2 = str2.Mid(0, str2.GetLength() - 1);
			}
		}

	if(_stat(str2, &buff) < 0)
		{
		//P2N("Not stat\r\n");
		goto endd;
		}

	if(buff.st_mode & _S_IFDIR)
		{
		//P2N("Not dir\r\n");
		ret = true;
		}
endd:
	return ret;
}

#if 0
	CString indir, outdir;

	if(cline != "")
		{
		// Parse command line:
		int idx3, idx2, idx1 = cline.Find(' ');
		if(idx1 >= 0)
			{
			indir = cline.Mid(0, idx1);
			idx2 = cline.Find(' ', idx1);
			if(idx2 >= 0)
				{
				idx3 = cline.Find(' ', idx1);
				if(idx3 >= 0)
					{
					outdir = cline.Mid(idx2 + 1, idx3);
					}
				else{
					outdir = cline.Mid(idx2 + 1);
					}
				}
			}
		else
			{
			indir = cline.Mid(0);
			}

		// Eval correct command line
		if(outdir == "")
			{	
			MBOX("Timedim: Cannot automate with one command argument. Use: timedim in_dir out_dir");
			}
		else
			{
			//P2N("Automating. indir='%s' outdir='%s'\r\n", indir, outdir);
			}
		}
	else
		{
		P2N("Empty command line\r\n");
		}

#endif

/////////////////////////////////////////////////////////////////////////////////////
//
// This routine is a helper that finds the path to the special folder:
//
// \param SpecialFolder			- an UINT-define (See #defines above or (MSDN))\n
// \param SpecialFolderString	- Reference to a CString that receives the path to the special folder
//
// Returns a BOOL - Found or not \n
//
// CSIDL_APPDATA
// CSIDL_ALTSTARTUP  	File system directory that corresponds to the user's nonlocalized Startup program group.
// CSIDL_APPDATA  		File system directory that serves as a common repository for application-specific data.
// CSIDL_BITBUCKET  	File system directory containing file objects in the user's Recycle Bin. The location of this directory is not in the registry; it is marked with the hidden and system attributes to prevent the user from moving or deleting it.
// CSIDL_COMMON_ALTSTARTUP  File system directory that corresponds to the nonlocalized Startup program group for all users.
// CSIDL_COMMON_DESKTOPDIRECTORY  File system directory that contains files and folders that appear on the desktop for all users.
// CSIDL_COMMON_FAVORITES  File system directory that serves as a common repository for all users' favorite items.
// CSIDL_COMMON_PROGRAMS  File system directory that contains the directories for the common program groups that appear on the Start menu for all users.
// CSIDL_COMMON_STARTMENU  File system directory that contains the programs and folders that appear on the Start menu for all users.
// CSIDL_COMMON_STARTUP  File system directory that contains the programs that appear in the Startup folder for all users.
// CSIDL_CONTROLS  		Virtual folder containing icons for the Control Panel applications.
// CSIDL_COOKIES  		File system directory that serves as a common repository for Internet cookies.
// CSIDL_DESKTOP  		Windows Desktop virtual folder at the root of the namespace.
// CSIDL_DESKTOPDIRECTORY  File system directory used to physically store file objects on the desktop (not to be confused with the desktop folder itself).
// CSIDL_DRIVES  		My Computer  virtual folder containing everything on the local computer: storage devices, printers, and Control Panel. The folder may also contain mapped network drives.
// CSIDL_FAVORITES  	File system directory that serves as a common repository for the user's favorite items.
// CSIDL_FONTS  		Virtual folder containing fonts.
// CSIDL_HISTORY  		File system directory that serves as a common repository for Internet history items.
// CSIDL_INTERNET  		Virtual folder representing the Internet.
// CSIDL_INTERNET_CACHE  File system directory that serves as a common repository for temporary Internet files.
// CSIDL_NETHOOD  		File system directory containing objects that appear in the network neighborhood.
// CSIDL_NETWORK  		Network Neighborhood Folder—virtual folder representing the top level of the network hierarchy.
// CSIDL_PERSONAL  		File system directory that serves as a common repository for documents.
// CSIDL_PRINTERS  		Virtual folder containing installed printers.
// CSIDL_PRINTHOOD  	File system directory that serves as a common repository for printer links.
// CSIDL_PROGRAMS  		File system directory that contains the user's program groups (which are also file system directories).
// CSIDL_RECENT  		File system directory that contains the user's most recently used documents.
// CSIDL_SENDTO  		File system directory that contains Send To menu items.
// CSIDL_STARTMENU  	File system directory containing Start menu items.
// CSIDL_STARTUP  		File system directory that corresponds to the user's Startup program group. The system starts these programs whenever any user logs onto Windows NT or starts Windows 95.
// CSIDL_TEMPLATES  	File system directory that serves as a common repository for document templates.
//


BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString)

{
	HRESULT hr;

	LPITEMIDLIST pidl;

	hr = SHGetSpecialFolderLocation(NULL, SpecialFolder, &pidl);

    if(SUCCEEDED(hr))
		{
		// Convert the item ID list's binary representation into a file system path
		char szPath[_MAX_PATH];

		if(SHGetPathFromIDList(pidl, szPath))
			{
			// Allocate a pointer to an IMalloc interface
			LPMALLOC pMalloc;

			// Get the address of our task allocator's IMalloc interface
			hr = SHGetMalloc(&pMalloc);

			// Free the item ID list allocated by SHGetSpecialFolderLocation
			pMalloc->Free(pidl);

			// Free our task allocator
			pMalloc->Release();

			// Work with the special folder's path (contained in szPath)
			SpecialFolderString = szPath;	SpecialFolderString += "\\";
			return TRUE;
			}
		}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
// Function name:    largeuint2double
// Description:      <empty description>
// Return type:      
// Argument:         ULARGE_INTEGER lint
//
//////////////////////////////////////////////////////////////////////////

double	largeuint2double(ULARGE_INTEGER lint)

{
	double ret;

	ret =  ((double)lint.HighPart) * (1 << 16);
	ret *= (1 << 16);
	ret  += lint.LowPart;

	return(ret);
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    uint64todouble
// Description:      <empty description>
// Return type:      
// Argument:         __int64 int64val
//
//////////////////////////////////////////////////////////////////////////

double	uint64todouble(__int64 int64val)

{
	double ret = 0;

	unsigned int *ptr = (unsigned int*)&int64val;

	ret = *(ptr + 1);
	ret *= (1 << 16); ret *= (1 << 16);	
	ret += *(ptr);
	
	return(ret);
}

//////////////////////////////////////////////////////////////////////////

double	GetFreeDiskSpace(const char *drv)

{
	ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	
	CString str(drv);

	// Repair free space drive string
	str = str.Left(3);

	double freeb = 0;

	BOOL ret = GetDiskFreeSpaceEx(
			str,
			&FreeBytesAvailableToCaller,	// disk available to the caller
			&TotalNumberOfBytes,			// receives the number of bytes on disk
			&TotalNumberOfFreeBytes		// receives the free bytes on disk
			);

	if(ret)
		{
		freeb = largeuint2double(FreeBytesAvailableToCaller);
		}

	//P2N("Get free space %s %.0f\r\n", str, freeb);

	return freeb;
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    addcommas
// Description:      <empty description>
// Return type:      
// Argument:         CString &str
//
//////////////////////////////////////////////////////////////////////////

void	addcommas(CString &str) 

{
	CString tmp;

	//P2N("addcomas() in='%s'  ", str);

	int len = str.GetLength();

	// Reverse string, add commas
	for(int loop = 0; loop < len ; loop++)
		{
		if(loop % 3 == 0 && loop != 0 ) 
			tmp+= ',';
		
		tmp += str[len - 1 - loop];
		}

	// Get ready to reconstruct
	str = ""; len = tmp.GetLength();

	// Reverse back to original
	for(int loop2 = 0; loop2 < len ; loop2++)
		{
		str += tmp[len - 1 - loop2];
		}

	//P2N("out='%s'\r\n", str);
}
