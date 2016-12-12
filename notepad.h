
/* =====[ NotePad.h ]=========================================================

   Description:

            Interface for Marx debug pad.

      REV     DATE		BY           DESCRIPTION
      ----  --------	----------    --------------------------------------
      0.00  03/09/99	Peter Glen    Initial version.
	  0.00  aug/13/2007 Peter Glen    Added dump, objectified

   ======================================================================= */

#ifndef mxpad_defined
#define mxpad_defined

#ifdef  EXPORT
#define EXTERN
#else
#define EXTERN extern
#endif

/* -------- Defines: ----------------------------------------------------- */

#define P2N		mxpad.PrintToNotepad
#define T2N		mxpad.ToNotepad
#define D2N		mxpad.DumpToNotepad
#define C2N		mxpad.ClearNotepad

#define MBOXP	mxpad.AfxPrintf
#define MBOXD	mxpad.AfxDump

#define	MBOX	AfxMessageBox

//////////////////////////////////////////////////////////////////////////
// Internal params

#define PRINTBUFSIZE	512				// Internal buffer size for MBOXP
#define MXBUFSIZE		4096			// Internal buffer size for MBOXP


/* -------- Macros: ------------------------------------------------------ */

/* -------- Protos ------------------------------------------------------- */

/* -------- Declarations: ------------------------------------------------ */

/* -------- Definitions: ------------------------------------------------- */

#undef  EXTERN
#undef  EXPORT

class NotePad

{

protected:

	char    szOutString[MXBUFSIZE];				// BUFFER to use
	
public:

	//////////////////////////////////////////////////////////////////////
	//

	// Just like printf, but prints to debug pad
	int     PrintToNotepad(const char *Format, ...);

	// Just like hexdump, but prints to debug pad
	int     DumpToNotepad(void *mem, int len);

	// Just like print, but prints to debug pad
	int     ToNotepad(char  *str);

	// Just like printf, but prints to messagebox
	int		AfxPrintf(const char *Format, ...);

	// Just like hexdump, but prints to messagebox
	int		AfxDump(void *mem, int len);

	//////////////////////////////////////////////////////////////////////
	//

	// Convert numeric message number to readable message
	char    *num2msg(int msg);

	// convert numeric error number to readable message
	char    *num2err(int msg);

	// Return TRUE if message is non-client
	int     is_nonclient(int msg);

	// Return TRUE if message is async junc (mouse move etc ...)
	int     is_junk(int msg);

	// Clear notepad window
	void     NotePad::ClearNotepad();  


protected:

	HWND    GetChild(HWND  hwndParent);
	HWND    child;

	static BOOL    _stdcall NotePad::EnumChildProc(HWND  hwnd, LPARAM  lParam);

#ifdef UNICODE

	int     PrintToNotepadW(const char *Format, ...);
	int     ToNotepadW(LPWSTR   *str);

	DWORD   UnicodeToAnsi(LPWSTR pszW, LPSTR* ppszA);
	DWORD   AnsiToUnicode(LPCSTR pszA, LPWSTR* ppszW);

#endif

};

extern	NotePad		mxpad;

#endif

/* EOF */
