#include "stdafx.h"
#include "common.h"
#include "Errors.h"
#pragma comment(lib, "Quartz.lib")

void gtrace(WCHAR *format, ...)
{
	//if (g_nDebugWnd == -1) return;

	WCHAR buffer[512] = {0};
	va_list args;
	va_start(args,format);
	_vswprintf(buffer,format,args);
	::OutputDebugStringW(buffer);
	wprintf(buffer);

	va_end (args);
	//if (g_nDebugWnd == 0) 
	//{
	//	g_hDebugWnd = ::FindWindow(0, "DebugWnd");
	//	if (g_hDebugWnd == 0)
	//	{
	//		g_nDebugWnd = -1;
	//		return;
	//	}
	//}

	//COPYDATASTRUCT cps = {0};
	//cps.dwData = 0x1234;
	//cps.lpData = &buffer;
	//cps.cbData = strlen(buffer)+1;
	//::SendMessage(g_hDebugWnd, WM_COPYDATA, 0, (LPARAM)&cps);
}

void gtrace(char* format, ...)
{
	//if (g_nDebugWnd == -1) return;

	char buffer[512] = {0};
	va_list args;
	va_start(args,format);
	vsprintf(buffer,format,args);
	::OutputDebugStringA(buffer);
	printf(buffer);
	va_end (args);

	//if (g_nDebugWnd == 0) 
	//{
	//	g_hDebugWnd = ::FindWindow(0, "DebugWnd");
	//	if (g_hDebugWnd == 0)
	//	{
	//		g_nDebugWnd = -1;
	//		return;
	//	}
	//}

	//COPYDATASTRUCT cps = {0};
	//cps.dwData = 0x1234;
	//cps.lpData = &buffer;
	//cps.cbData = strlen(buffer)+1;
	//::SendMessage(g_hDebugWnd, WM_COPYDATA, 0, (LPARAM)&cps);
}

void CHK0(char *szFile, char *szFunc, int nLine, HRESULT hr)
{
	if (FAILED(hr))
	{
		char tmp[128] = {0};
        char szErr[MAX_ERROR_TEXT_LEN];
        AMGetErrorTextA(hr, szErr, MAX_ERROR_TEXT_LEN);
		sprintf(tmp, "%s(%d) failed %X (%s)\n", szFile, nLine, hr, szErr);
		::MessageBoxA(0, tmp, 0, 0);
		exit(0);
	}
}


#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS) || defined(__WATCOMC__)
  #define DELTA_EPOCH_IN_USEC  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_USEC  11644473600000000ULL
#endif
typedef unsigned __int64 u_int64_t;

static u_int64_t filetime_to_unix_epoch (const FILETIME *ft)
{
    u_int64_t res = (u_int64_t) ft->dwHighDateTime << 32;

    res |= ft->dwLowDateTime;
    res /= 10;                   /* from 100 nano-sec periods to usec */
    res -= DELTA_EPOCH_IN_USEC;  /* from Win epoch to Unix epoch */
    return (res);
}

int gettimeofday (struct timeval *tv, void *tz)
{
    FILETIME  ft;
    u_int64_t tim;

    if (!tv) {
        //errno = EINVAL;
        return (-1);
    }
    GetSystemTimeAsFileTime (&ft);
    tim = filetime_to_unix_epoch (&ft);
    tv->tv_sec  = (long) (tim / 1000000L);
    tv->tv_usec = (long) (tim % 1000000L);
    return (0);
}

int gettimeofms()
{
	struct timeval tim = {0};
	gettimeofday(&tim, 0);
	return tim.tv_sec*1000+tim.tv_usec/1000;
}