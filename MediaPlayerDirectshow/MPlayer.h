// MPlayer.h : main header file for the MPLAYER application
//

#if !defined(AFX_MPLAYER_H__1B816AB6_72B7_4468_948D_B1B1A83A4A24__INCLUDED_)
#define AFX_MPLAYER_H__1B816AB6_72B7_4468_948D_B1B1A83A4A24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMPlayerApp:
// See MPlayer.cpp for the implementation of this class
//

class CMPlayerApp : public CWinApp
{
public:
	CMPlayerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlayerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMPlayerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPLAYER_H__1B816AB6_72B7_4468_948D_B1B1A83A4A24__INCLUDED_)
