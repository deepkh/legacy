

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Apr 04 22:29:50 2011
 */
/* Compiler settings for .\IAudPlay.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IAudPlay_h_h__
#define __IAudPlay_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAudPlay_FWD_DEFINED__
#define __IAudPlay_FWD_DEFINED__
typedef interface IAudPlay IAudPlay;
#endif 	/* __IAudPlay_FWD_DEFINED__ */


#ifndef __AudPlay_FWD_DEFINED__
#define __AudPlay_FWD_DEFINED__

#ifdef __cplusplus
typedef class AudPlay AudPlay;
#else
typedef struct AudPlay AudPlay;
#endif /* __cplusplus */

#endif 	/* __AudPlay_FWD_DEFINED__ */


/* header files for imported files */
#include "Unknwn.h"
#include "ShareData.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAudPlay_INTERFACE_DEFINED__
#define __IAudPlay_INTERFACE_DEFINED__

/* interface IAudPlay */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAudPlay;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03B60FE0-7168-44f2-8D22-B7989CFFB101")
    IAudPlay : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SelDev( 
            /* [in] */ int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevProps( 
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pDevName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevCount( 
            /* [in] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ WAVEFORMATEX2 *pFmt,
            /* [in] */ int cbBuffer) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddClip( 
            /* [in] */ unsigned char *pBuf,
            /* [in] */ int nSize,
            /* [in] */ int nTimeStamp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitGraph( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnInitGraph( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAudPlayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudPlay * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudPlay * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudPlay * This);
        
        HRESULT ( STDMETHODCALLTYPE *SelDev )( 
            IAudPlay * This,
            /* [in] */ int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE *GetDevProps )( 
            IAudPlay * This,
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pDevName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDevCount )( 
            IAudPlay * This,
            /* [in] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *SetFormat )( 
            IAudPlay * This,
            /* [in] */ WAVEFORMATEX2 *pFmt,
            /* [in] */ int cbBuffer);
        
        HRESULT ( STDMETHODCALLTYPE *AddClip )( 
            IAudPlay * This,
            /* [in] */ unsigned char *pBuf,
            /* [in] */ int nSize,
            /* [in] */ int nTimeStamp);
        
        HRESULT ( STDMETHODCALLTYPE *InitGraph )( 
            IAudPlay * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnInitGraph )( 
            IAudPlay * This);
        
        HRESULT ( STDMETHODCALLTYPE *Run )( 
            IAudPlay * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IAudPlay * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IAudPlay * This);
        
        END_INTERFACE
    } IAudPlayVtbl;

    interface IAudPlay
    {
        CONST_VTBL struct IAudPlayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudPlay_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudPlay_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudPlay_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudPlay_SelDev(This,nIndex)	\
    ( (This)->lpVtbl -> SelDev(This,nIndex) ) 

#define IAudPlay_GetDevProps(This,nIndex,pDevName)	\
    ( (This)->lpVtbl -> GetDevProps(This,nIndex,pDevName) ) 

#define IAudPlay_GetDevCount(This,pCount)	\
    ( (This)->lpVtbl -> GetDevCount(This,pCount) ) 

#define IAudPlay_SetFormat(This,pFmt,cbBuffer)	\
    ( (This)->lpVtbl -> SetFormat(This,pFmt,cbBuffer) ) 

#define IAudPlay_AddClip(This,pBuf,nSize,nTimeStamp)	\
    ( (This)->lpVtbl -> AddClip(This,pBuf,nSize,nTimeStamp) ) 

#define IAudPlay_InitGraph(This)	\
    ( (This)->lpVtbl -> InitGraph(This) ) 

#define IAudPlay_UnInitGraph(This)	\
    ( (This)->lpVtbl -> UnInitGraph(This) ) 

#define IAudPlay_Run(This)	\
    ( (This)->lpVtbl -> Run(This) ) 

#define IAudPlay_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IAudPlay_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudPlay_INTERFACE_DEFINED__ */



#ifndef __IAudPlay_LIBRARY_DEFINED__
#define __IAudPlay_LIBRARY_DEFINED__

/* library IAudPlay */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_IAudPlay;

EXTERN_C const CLSID CLSID_AudPlay;

#ifdef __cplusplus

class DECLSPEC_UUID("03B60FE0-7168-44f2-8D22-B7989CFFB100")
AudPlay;
#endif
#endif /* __IAudPlay_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


