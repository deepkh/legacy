// stdafx.cpp : source file that includes just the standard includes
//	MPlayer.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



void mbox(LPCTSTR format, ...)
{
	static TCHAR buffer[512];
	va_list args;
	va_start(args,format);
	_vstprintf(buffer,format,args);
	::MessageBox(0, buffer, 0, MB_OK);
}
