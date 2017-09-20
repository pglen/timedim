
/* =====[ CopyFile.cpp ]========================================================== 
                                                                             
   Description:     The timedim project, implementation of the CopyFile.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/10/2011  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "timedim.h"
#include "CopyFile.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#include "support.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Deprecation warnings OFF (deal with it at some point)
#pragma warning( disable : 4996 )

/////////////////////////////////////////////////////////////////////////////
// Shorten path to file name

void	PathToFname(CString &docname)

{
	int idx;
	CString filename;

	if((idx = docname.ReverseFind('\\')) != -1)
		{
		filename = docname.Right(docname.GetLength() - (idx + 1));
		docname = filename;
		}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCopyFile::CCopyFile()

{
	lab = NULL;	currfile = NULL;
	stop = friendly = badsect = iconic = false;
}

//////////////////////////////////////////////////////////////////////////

CCopyFile::~CCopyFile()
{

}

//////////////////////////////////////////////////////////////////////////
//

int     CCopyFile::comp_data(FILE *fpin, FILE *fpout)

{
    int     err = 0;
	double	prog = 0;
	char    *buff_ptr = NULL;
	char    *buff_ptr2 = NULL;

	 // Get resources
    buff_ptr = (char *) malloc(BUFSIZE);
    if(!buff_ptr)
        {
        P2N("CCopyFile::Out of memory. ");
		err = SYSTEM_NO_MEMORY;
		goto endd;
        }

	buff_ptr2 = (char *) malloc(BUFSIZE);
    if(!buff_ptr2)
        {
        P2N("CCopyFile::Out of memory. ");
		err = SYSTEM_NO_MEMORY;
		goto endd;
        }

    while(TRUE)
        {
        unsigned int    one_go, read_in;

        one_go  = fread(buff_ptr, sizeof(char), BUFSIZE, fpin);

		prog += one_go;
		//tdata += one_go;

		YieldToWinEx();

		CString str; str.Format("Verifying %.0f of %.0f bytes", prog, dflen);

		//P2N("label string %s", str);
		if(lab)
			lab->SetText(str);

        read_in = fread(buff_ptr2, sizeof(char), BUFSIZE, fpout);

        if(ferror(fpin))
            {
            P2N("CCopyFile::Error on source read. ");
            err = SOURCE_READ_ERROR;
            break;
            }
        if(ferror(fpout))
            {
            P2N("CCopyFile::Error on target read. ");
            err = TARGET_READ_ERROR;
            break;
            }

        if(read_in != one_go)
            {
            P2N("CCopyFile::Mismatched file size. ");
            err = VERIFY_ERR;
            break;
            }

		if(memcmp(buff_ptr, buff_ptr2, one_go))
			{
			P2N(" Mismatched file content. ");
            err = VERIFY_ERR;
            break;
            }
        /*
         * End of read:
         */
        if(one_go < BUFSIZE)
            break;
        }

endd:

	if(buff_ptr)
		free(buff_ptr);

	if(buff_ptr2)
		free(buff_ptr2);

    return(err);
}

/*
 * Copy data from one to other:
 * Return error code
 */

int     CCopyFile::copy_data(FILE *fpin, FILE *fpout)

{
    int     err = 0;
	//unsigned int prog = 0;
	double dprog = 0;
	char    *buff_ptr = NULL;

	 // Get resources
    buff_ptr = (char *) malloc(BUFSIZE);
    if(!buff_ptr)
        {
        P2N("CCopyFile::Out of memory. ");
		err = SYSTEM_NO_MEMORY;
		goto endd;
        }

    while(TRUE)
        {
        unsigned int    one_go, read_in;

        one_go  = fread(buff_ptr, sizeof(char), BUFSIZE, fpin);

		YieldToWinEx();

		if(!badsect)
			{
			read_in = fwrite(buff_ptr, sizeof(char), one_go, fpout);
			}

		if(stop > 2)
			{
			// Abort detected
			break;
			}

		if(badsect)
			{
			dprog += one_go;
			tdata += one_go;
			}
		else		
			{
			dprog += read_in;
			tdata += read_in;
			}

		// Be friendly
		if(friendly || iconic)
			{
			support.YieldToWinEx();

			if(friendly)
				Sleep(20);
			else
				Sleep(2);
			
			support.YieldToWinEx();
			}
		
		// Update only 4/sec
		unsigned int now = clock();
		if(now > (oldc + CLOCKS_PER_SEC / 4) )
			{
			oldc = now;

			CString fname(currfile);
			PathToFname(fname);	ShortenPath(fname, 84);

			CString str, tmp, tmp2;
			tmp.Format("%.0f", dprog);	addcommas(tmp);
			tmp2.Format("%.0f", dflen);	addcommas(tmp2);

			str.Format("Copying: '%s'   %s of %s bytes",
							fname, tmp, tmp2);

			//P2N("label string %s", str);

			if(lab)
				lab->SetText(str);

			if(bar)
				{
				if(dflen != 0)
					{
					int proc = (int) ((dprog * 100)  /  dflen) ;
					//P2N("proc = %d\r\n", proc);	
					bar->SetCurPos(proc);
					}
				}
			}

        if(ferror(fpin))
            {
            P2N("CCopyFile::Error on read. ");
            err = SOURCE_READ_ERROR;
            break;
            }
		if(!badsect)
			{
			if(ferror(fpout))
				{
				P2N(" Error on write. ");
				err = TARGET_READ_ERROR;
				break;
				}
			}
		if(!badsect)
			{
			if(read_in != one_go)
				{
				P2N(" Cannot write. ");
				err = TARGET_WRITE_ERROR;
				break;
				}
			}	
        /*
         * End of read:
         */
        if(one_go < BUFSIZE)
            break;
        }

	if(buff_ptr)
		free(buff_ptr);

endd:
    return(err);
}

//////////////////////////////////////////////////////////////////////////
//
// The CopyFile function copies an existing file to a new file.
//		it  replace this W32 function ==> CopyFile(src, dest, false);
// If the function succeeds, the return value is zero.
//

int		CCopyFile::CopyTimeDimFile(const char *src, const char *dest, int failif)

{
	int err = COPY_NO_ERROR;

	FILE *fpin = NULL, *fpout = NULL;

	currfile = dest;
	tdata = 0;
	startc = clock();
	stop = 0;

	// ABORT RETRY FAIL AVOIDED HERE
	int old_err =  SetErrorMode(SEM_FAILCRITICALERRORS);

	//P2N("Copy File src=%s dst=%s\r\n", src, dest);

	if(failif)
		{
		if(_access(dest, 0) >= 0)
			{
			err = TARGET_FILE_EXISTS;
			goto endd;
			}
		}

	if ((fpin = fopen(src, "rb")) == NULL)
        {
        err = SOURCE_OPEN_ERROR ;
        P2N("CCopyFile::CopyTimeDimFile - cannot open source file: %s ", src);
        goto endd;
        }

	//flength = filelength(fileno(fpin));
	flen64	=	_filelengthi64(fileno(fpin));
	dflen	=	uint64todouble(flen64);
	
	//P2N("File Length %.2f\r\n", dflen);

	// 
	if(!badsect)
		{
		if ((fpout = fopen(dest,"wb")) == NULL)
			{
			err = TARGET_CREATE_ERROR;
			P2N(" cannot create file: %s\r\n", dest);
			goto endd;
			}
		}

    if((err = copy_data(fpin, fpout)))
        {
		P2N(" cannot copy file: %s\r\n", dest);
		}
endd:

	if(fpin)
		fclose(fpin);

	if(fpout)
		fclose(fpout);

	//P2N("Tdata %.0f\r\n", tdata);
	SetErrorMode(old_err);

	if(stop > 2)
		{
		// Unlink partial file
		_unlink(dest);
		}

	endc = clock();
	stop = 0;
	return err;
}

//////////////////////////////////////////////////////////////////////////

int		CCopyFile::VerifyTimeDim(const char *src, const char *dest)

{
	int err = VERIFY_OK;

	FILE *fpin = NULL, *fpout = NULL;

	tdata = 0;

	//P2N("Verify file src=%s dst=%s\r\n", src, dest);

	if ((fpin = fopen(src, "rb")) == NULL)
        {
        err = SOURCE_OPEN_ERROR ;
        P2N("CCopyFile::CopyTimeDimFile - cannot open source file: %s ", src);
        goto endd;
        }
	 if ((fpout = fopen(dest,"rb")) == NULL)
        {
        err = TARGET_OPEN_ERROR;
        P2N("CCopyFile::CopyTimeDimFile - cannot create file: %s\r\n", dest);
        goto endd;
        }
    if((err = comp_data(fpin, fpout)))
        {
		P2N(" compare file failed: %s\r\n", dest);
		}
endd:

	if(fpin)
		fclose(fpin);

	if(fpout)
		fclose(fpout);

	return err;
}

///////////////////////////////////////////////////////////////////////////

char * CCopyFile::err2txt(int err)

{
	char *ptr = "Unkown";

	switch(err)
		{
		case SYSTEM_NO_MEMORY  :      ptr = "SYSTEM_NO_MEMORY";  break;
		case SOURCE_OPEN_ERROR :      ptr = "SOURCE_OPEN_ERROR";  break;
		case SOURCE_READ_ERROR :      ptr = "SOURCE_READ_ERROR";  break;
		case TARGET_OPEN_ERROR :      ptr = "TARGET_OPEN_ERROR";  break;
		case TARGET_CREATE_ERROR:     ptr = "TARGET_CREATE_ERROR";break;
		case TARGET_READ_ERROR :      ptr = "TARGET_READ_ERROR";  break;
		case TARGET_WRITE_ERROR:      ptr = "TARGET_WRITE_ERROR"; break;
		case TARGET_FILE_SHORT :      ptr = "TARGET_FILE_SHORT";  break;
		case TARGET_FILE_EXISTS:	  ptr = "TARGET_FILE_EXISTS"; break;
		case TARGET_DIR_ERROR  :      ptr = "TARGET_DIR_ERROR";   break;
		case TARGET_MISMATCH   :      ptr = "TARGET_MISMATCH";    break;
	    }
	return(ptr);
}


