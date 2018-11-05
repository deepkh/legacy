// stdafx.cpp : source file that includes just the standard includes
// IAudPlay.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

const GUID CLSID_AudPlay = 
{0x03B60FE0,0x7168,0x44f2,{0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB1,0x00}};

const GUID IID_IAudPlay = 
{0x03B60FE0,0x7168,0x44f2,{0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB1,0x01}};


#pragma comment(lib, "strmiids")
#pragma comment(lib, "WINMM.LIB")
#ifdef _DEBUG
	#pragma comment(lib, "Strmbasd.lib")
#else
	#pragma comment(lib, "Strmbase.lib")
#endif
