

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Apr 04 22:29:52 2011
 */
/* Compiler settings for .\IAudCapture.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IAudReceiver,0x03B60FE0,0x7168,0x44f2,0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB0,0x01);


MIDL_DEFINE_GUID(IID, IID_IAudCapture,0x03B60FE0,0x7168,0x44f2,0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB0,0x03);


MIDL_DEFINE_GUID(IID, LIBID_IAudCaptureLib,0x3827C1B9,0xBB77,0x4BDF,0x98,0x02,0xA1,0x30,0x8E,0x08,0x02,0x16);


MIDL_DEFINE_GUID(CLSID, CLSID_AudCapture,0x03B60FE0,0x7168,0x44f2,0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB0,0x00);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



