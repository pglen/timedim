; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTimedimDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "timedim.h"
LastPage=0

ClassCount=10
Class1=CLabel
Class2=CTimedimApp
Class3=CAboutDlg
Class4=CTimedimDlg

ResourceCount=5
Resource1=IDD_TIMEDIM_DIALOG
Class5=CxButton
Resource2=IDD_ABOUTBOX
Class6=CPausDlg
Resource3=IDD_DIALOG2
Class7=CGetDirName
Resource4=IDD_DIALOG1
Class8=CLogDlg
Class9=CLogWin
Class10=CKCBusyProgressCtrl
Resource5=IDD_DIALOG3

[CLS:CLabel]
Type=0
BaseClass=CStatic
HeaderFile=label.h
ImplementationFile=label.cpp

[CLS:CTimedimApp]
Type=0
BaseClass=CWinApp
HeaderFile=timedim.h
ImplementationFile=timedim.cpp
Filter=N
VirtualFilter=AC

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=timedimDlg.cpp
ImplementationFile=timedimDlg.cpp
Filter=W
VirtualFilter=dWC
LastObject=CAboutDlg

[CLS:CTimedimDlg]
Type=0
BaseClass=CDialog
HeaderFile=timedimDlg.h
ImplementationFile=timedimDlg.cpp
LastObject=IDC_CHECK6
Filter=W
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_HEAD,static,1342308481
Control2=IDC_CR,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_BUTTON1,button,1208025088
Control5=IDC_STATIC,static,1342177294
Control6=IDC_VER,static,1342308352

[DLG:IDD_TIMEDIM_DIALOG]
Type=1
Class=CTimedimDlg
ControlCount=56
Control1=IDC_TREE1,SysTreeView32,1350631463
Control2=IDC_TREE2,SysTreeView32,1350631463
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON14,button,1342242816
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON13,button,1342242816
Control8=ID_HELP,button,1342242816
Control9=IDC_BUTTON6,button,1342242816
Control10=IDC_BUTTON8,button,1342242816
Control11=IDC_BUTTON7,button,1342242816
Control12=IDC_BUTTON9,button,1342242816
Control13=ID_BUTTON12,button,1342242816
Control14=IDC_UNC4,static,1342308352
Control15=IDC_CHECK1,button,1342177283
Control16=IDC_UNC6,static,1342308352
Control17=IDC_CHECK3,button,1342177283
Control18=IDC_UNC5,static,1342308352
Control19=IDC_CHECK2,button,1342177283
Control20=IDC_UNC7,static,1342308352
Control21=IDC_CHECK4,button,1342177283
Control22=IDC_EDIT1,edit,1353777156
Control23=IDC_EDIT2,edit,1353777348
Control24=IDC_EDIT3,edit,1353777348
Control25=IDC_TAB1,SysTabControl32,1342177344
Control26=IDC_PROG,static,1342312448
Control27=IDC_BUTTON20,button,1342242816
Control28=IDC_BUTTON15,button,1342242816
Control29=IDC_BUTTON16,button,1342242816
Control30=IDC_PROG2,static,1342312448
Control31=IDC_BUTTON21,button,1342242816
Control32=IDC_BUTTON19,button,1342242816
Control33=IDC_BUTTON17,button,1342242816
Control34=IDC_BUTTON5,button,1073807360
Control35=IDC_HEADER,static,1342312449
Control36=IDC_LABL,static,1342308353
Control37=IDC_STATIC,static,1342177287
Control38=IDC_LABR,static,1342308353
Control39=IDC_HEADER2,static,1342308353
Control40=IDC_BUTTON10,button,1208025088
Control41=IDC_BUTTON11,button,1208025088
Control42=IDC_STATIC,button,1342177287
Control43=IDC_BUTTON3,button,1208025088
Control44=IDC_LAB,static,1342312448
Control45=IDC_BUTTON18,button,1208025088
Control46=IDC_RATE,static,1342308353
Control47=IDC_BAR,static,1342312448
Control48=IDC_CHECK5,button,1207959555
Control49=IDC_UNC8,static,1208090624
Control50=IDC_BUTTON12,button,1208025088
Control51=IDC_BUTTON22,button,1208025088
Control52=IDC_UNC9,static,1342308352
Control53=IDC_CHECK6,button,1342177283
Control54=IDC_UNC10,static,1342308352
Control55=IDC_CHECK7,button,1342177283
Control56=IDC_UNC11,static,1342308352

[CLS:CxButton]
Type=0
HeaderFile=xButton.h
ImplementationFile=xButton.cpp
BaseClass=CButton

[DLG:IDD_DIALOG1]
Type=1
Class=CPausDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352

[CLS:CPausDlg]
Type=0
HeaderFile=PausDlg.h
ImplementationFile=PausDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPausDlg

[DLG:IDD_DIALOG2]
Type=1
Class=CGetDirName
ControlCount=5
Control1=IDC_EDIT1,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_PTEXT,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:CGetDirName]
Type=0
HeaderFile=GetDirName.h
ImplementationFile=GetDirName.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CLogDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1208025088
Control3=IDC_EDIT1,edit,1353779204
Control4=IDC_BUTTON1,button,1342242816

[CLS:CLogDlg]
Type=0
HeaderFile=LogDlg.h
ImplementationFile=LogDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=CLogDlg
VirtualFilter=dWC

[CLS:CLogWin]
Type=0
HeaderFile=LogWin.h
ImplementationFile=LogWin.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CLogWin

[CLS:CKCBusyProgressCtrl]
Type=0
HeaderFile=KCBusyProgressCtrl.h
ImplementationFile=KCBusyProgressCtrl.cpp
BaseClass=CStatic

