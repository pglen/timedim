// CopyFile.h: interface for the CCopyFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYFILE_H__4334AAF3_AE0A_404A_8B1E_884BC99E810C__INCLUDED_)
#define AFX_COPYFILE_H__4334AAF3_AE0A_404A_8B1E_884BC99E810C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BUFSIZE  0xf000

#define COPY_NO_ERROR			0x0
#define SYSTEM_NO_MEMORY        0x1

#define SOURCE_OPEN_ERROR       0x2
#define SOURCE_READ_ERROR       0x3

#define TARGET_OPEN_ERROR       0x11
#define TARGET_CREATE_ERROR     0x12
#define TARGET_READ_ERROR       0x13
#define TARGET_WRITE_ERROR      0x14
#define TARGET_FILE_SHORT       0x15
#define TARGET_FILE_EXISTS		0x16 

#define TARGET_DIR_ERROR        0x21
#define TARGET_MISMATCH         0x22

#define	VERIFY_OK				0x00
#define	VERIFY_ERR				0x01

#include "label.h"
#include "KCBusyProgressCtrl.h"


class CCopyFile  
{
public:

	int badsect, friendly, iconic;

	char* err2txt(int err);

	double		tdata;
	const char * currfile;

	int		stop;
	int		CCopyFile::VerifyTimeDim(const char *src, const char *dest);
	int		CCopyFile::CopyTimeDimFile(const char *src, const char *dest, int failif);

	CLabel	*lab;
	CKCBusyProgressCtrl	*bar;

	//int		flength;
	double		dflen;
	__int64		flen64;
	unsigned int oldc, startc, endc;

	CCopyFile();
	virtual ~CCopyFile();

private:
	int     CCopyFile::copy_data(FILE *fpin, FILE *fpout);
	int     CCopyFile::comp_data(FILE *fpin, FILE *fpout);

};

#endif // !defined(AFX_COPYFILE_H__4334AAF3_AE0A_404A_8B1E_884BC99E810C__INCLUDED_)
