/***********************************************************************
 * 2011/11/09: Gary
 * 		Draft
 **********************************************************************/

#include "Common.h"

void gtrace(const char * szFormat, ...)
{
	char szBuffer[4096] = {0};
	int	iReturn;
	va_list pArgs;
	va_start (pArgs, szFormat) ;
	iReturn = vsprintf (szBuffer, szFormat, pArgs) ;
	va_end (pArgs);
	printf(szBuffer);
}

int getErrno() const {
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
	return WSAGetLastError();
#else
	return errno;
#endif
}

void PrtErr(char *title)
{
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN32_WCE)
	
	DWORD errnum = GetLastError();
	LPSTR lpBuffer;
	char errbuf[256];
	char modulename[MAX_PATH];
	DWORD numread;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errnum,
			LANG_NEUTRAL,
			(LPTSTR)&lpBuffer,
			0,
			NULL );
	sprintf(errbuf, "%s err: %d \"%s\"\n", title, errnum, lpBuffer);
	gtrace(errbuf);
#else
	gtrace("errno:%d ", errno);
	perror(title);
#endif
}

