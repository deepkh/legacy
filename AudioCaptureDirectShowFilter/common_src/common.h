
#ifndef _COMMON_H_
#define _COMMON_H_
#ifndef __AFX_H__
#include <windows.h>
#endif
#include <stdio.h>
#include <tchar.h>

void gtrace(WCHAR *format, ...);
void gtrace(char *format, ...);

#define CHK(hr) CHK0(__FILE__, __FUNCTION__, __LINE__, hr)
void CHK0(char *szFile, char *szFunc, int nLine, HRESULT hr);
int gettimeofday (struct timeval *tv, void *tz);
int gettimeofms();

#endif
