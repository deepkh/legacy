// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__44A10253_CE58_47B9_9A1F_6489DC952525__INCLUDED_)
#define AFX_STDAFX_H__44A10253_CE58_47B9_9A1F_6489DC952525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#ifdef DSKINDLL_EXPORTS
#define DSKINDLL_API extern "C" __declspec(dllexport)
#else
#define DSKINDLL_API extern "C" __declspec(dllimport)
#endif

//#include ".\include\dskindll.h"
//#include ".\include\dskindef.h"


//#define _WIN32_DCOM		//這行很重要 ~~ CoInitializeEx 要用的
#include <Math.h>
#include <dshow.h>
#include <atlbase.h>
#include <objbase.h>
#include <d3d9.h>
#include <vmr9.h>
#include <Mtype.h>
#include <Wxdebug.h>
#include <initguid.h>
#include <Streams.h>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "strmbasd.lib")
#pragma comment(lib, "winmm.lib")
#define WM_GRAPHNOTIFY WM_APP + 1
#define SPLIDER_LEN 1000

class CRate {
public:
	double fRateX;
	double fRateY;
};

#define ONE_SECOND 10000000
#define ONE_MINUTE 10000000*60
#define ONE_HOUR 10000000*60*60

//const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
//const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
//const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

void mbox(LPCTSTR format, ...);
#include "GrayTransform.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__44A10253_CE58_47B9_9A1F_6489DC952525__INCLUDED_)
