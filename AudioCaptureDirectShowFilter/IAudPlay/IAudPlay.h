// IAudPlay.h : main header file for the IAudPlay DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIAudPlayApp
// See IAudPlay.cpp for the implementation of this class
//

class CIAudPlayApp : public CWinApp
{
public:
	CIAudPlayApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
