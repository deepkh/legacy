; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCProgress
LastTemplate=CProgressCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MPlayer.h"

ClassCount=5
Class1=CMPlayerApp
Class2=CMPlayerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CCanvas
Class5=CCProgress
Resource3=IDD_MPLAYER_DIALOG

[CLS:CMPlayerApp]
Type=0
HeaderFile=MPlayer.h
ImplementationFile=MPlayer.cpp
Filter=N

[CLS:CMPlayerDlg]
Type=0
HeaderFile=MPlayerDlg.h
ImplementationFile=MPlayerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMPlayerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MPlayerDlg.h
ImplementationFile=MPlayerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MPLAYER_DIALOG]
Type=1
Class=CMPlayerDlg
ControlCount=6
Control1=IDC_PROGRESS,msctls_progress32,1350565889
Control2=IDC_LOAD,button,1342177280
Control3=IDC_PLAY,button,1342177280
Control4=IDC_PAUSE,button,1342177280
Control5=IDC_VOLUME,msctls_trackbar32,1342177304
Control6=IDC_DATE,static,1342308866

[CLS:CCanvas]
Type=0
HeaderFile=Canvas.h
ImplementationFile=Canvas.cpp
BaseClass=CStatic
Filter=W

[CLS:CCProgress]
Type=0
HeaderFile=CProgress.h
ImplementationFile=CProgress.cpp
BaseClass=CProgressCtrl
Filter=W
VirtualFilter=NWC

