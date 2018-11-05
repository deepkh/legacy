// MPlayer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPlayerApp

BEGIN_MESSAGE_MAP(CMPlayerApp, CWinApp)
	//{{AFX_MSG_MAP(CMPlayerApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayerApp construction

CMPlayerApp::CMPlayerApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMPlayerApp object

CMPlayerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMPlayerApp initialization

BOOL CMPlayerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

//	if (dsLoadSkin( _T("SkinQQ")) == 0)
//		::MessageBox(0, _T("z"), 0, MB_OK);

	CMPlayerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
