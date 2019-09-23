; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgNewRec
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mine.h"
LastPage=0

ClassCount=5
Class1=CDlgCustom
Class2=CDlgRank
Class3=CMineApp
Class4=CMineWnd

ResourceCount=6
Resource1=IDD_DLG_CUSTOM
Resource2=IDR_MAINFRAME
Resource3=IDR_MENU
Resource4=IDD_RANK
Resource5=IDD_ABOUTBOX
Class5=CDlgNewRec
Resource6=IDD_NEWREC

[CLS:CDlgCustom]
Type=0
BaseClass=CDialog
HeaderFile=DlgCustom.h
ImplementationFile=DlgCustom.cpp
LastObject=CDlgCustom

[CLS:CDlgRank]
Type=0
BaseClass=CDialog
HeaderFile=DlgRank.h
ImplementationFile=DlgRank.cpp

[CLS:CMineApp]
Type=0
BaseClass=CWinApp
HeaderFile=Mine.h
ImplementationFile=Mine.cpp

[CLS:CMineWnd]
Type=0
BaseClass=CWnd
HeaderFile=MineWnd.h
ImplementationFile=MineWnd.cpp

[DLG:IDD_DLG_CUSTOM]
Type=1
Class=CDlgCustom
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_EDIT3,edit,1350631552
Control7=IDC_OK,button,1342242816
Control8=IDC_CANNCEL,button,1342242816

[DLG:IDD_RANK]
Type=1
Class=CDlgRank
ControlCount=17
Control1=IDC_STATIC,static,1342308865
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308865
Control7=IDC_STATIC,static,1342308865
Control8=IDC_STATIC,static,1342308865
Control9=IDC_PRI_CAMP,static,1342308608
Control10=IDC_SEC_CAMP,static,1342308608
Control11=IDC_ADV_HOLDER,static,1342308608
Control12=IDC_SEC_HOLDER,static,1342308608
Control13=IDC_ADV_BEST,static,1342308608
Control14=IDC_SEC_BEST,static,1342308608
Control15=IDC_ADV_CAMP,static,1342308608
Control16=IDC_PRI_BEST,static,1342308608
Control17=IDC_PRI_HOLDER,static,1342308608

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MENU]
Type=1
Class=?
Command1=IDM_START
Command2=IDM_LEVEL_PRIMARY
Command3=IDM_LEVEL_SECONDARY
Command4=IDM_LEVEL_ADVANCE
Command5=IDM_LEVEL_CUSTOM
Command6=IDM_MARK
Command7=IDM_COLOR
Command8=IDM_SOUND
Command9=IDM_RANK
Command10=IDM_EXIT
Command11=IDM_HELP
Command12=IDM_ABOUT
CommandCount=12

[DLG:IDD_NEWREC]
Type=1
Class=CDlgNewRec
ControlCount=7
Control1=IDC_STATIC,static,1342308865
Control2=IDC_STATIC,static,1342308865
Control3=IDC_STATIC,static,1342308865
Control4=IDC_EDIT_CAMP,edit,1350631552
Control5=IDC_EDIT_HOLDER,edit,1350631552
Control6=IDC_LEVEL,static,1342309121
Control7=IDOK,button,1342242816

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[CLS:CDlgNewRec]
Type=0
HeaderFile=DlgNewRec.h
ImplementationFile=DlgNewRec.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LEVEL
VirtualFilter=dWC

