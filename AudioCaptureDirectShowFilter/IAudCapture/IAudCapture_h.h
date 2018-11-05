

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __IAudCapture_h_h__
#define __IAudCapture_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAudReceiver_FWD_DEFINED__
#define __IAudReceiver_FWD_DEFINED__
typedef interface IAudReceiver IAudReceiver;
#endif 	/* __IAudReceiver_FWD_DEFINED__ */


#ifndef __IAudCapture_FWD_DEFINED__
#define __IAudCapture_FWD_DEFINED__
typedef interface IAudCapture IAudCapture;
#endif 	/* __IAudCapture_FWD_DEFINED__ */


#ifndef __AudCapture_FWD_DEFINED__
#define __AudCapture_FWD_DEFINED__

#ifdef __cplusplus
typedef class AudCapture AudCapture;
#else
typedef struct AudCapture AudCapture;
#endif /* __cplusplus */

#endif 	/* __AudCapture_FWD_DEFINED__ */


/* header files for imported files */
#include "Unknwn.h"
#include "ShareData.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAudReceiver_INTERFACE_DEFINED__
#define __IAudReceiver_INTERFACE_DEFINED__

/* interface IAudReceiver */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAudReceiver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03B60FE0-7168-44f2-8D22-B7989CFFB001")
    IAudReceiver : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Clip( 
            /* [in] */ unsigned char *pBuf,
            /* [in] */ int nSize,
            /* [in] */ int nTimeStamp,
            /* [in] */ unsigned char *pParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAudReceiverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudReceiver * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudReceiver * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudReceiver * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clip )( 
            IAudReceiver * This,
            /* [in] */ unsigned char *pBuf,
            /* [in] */ int nSize,
            /* [in] */ int nTimeStamp,
            /* [in] */ unsigned char *pParam);
        
        END_INTERFACE
    } IAudReceiverVtbl;

    interface IAudReceiver
    {
        CONST_VTBL struct IAudReceiverVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudReceiver_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudReceiver_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudReceiver_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudReceiver_Clip(This,pBuf,nSize,nTimeStamp,pParam)	\
    ( (This)->lpVtbl -> Clip(This,pBuf,nSize,nTimeStamp,pParam) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudReceiver_INTERFACE_DEFINED__ */


#ifndef __IAudCapture_INTERFACE_DEFINED__
#define __IAudCapture_INTERFACE_DEFINED__

/* interface IAudCapture */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IAudCapture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03B60FE0-7168-44f2-8D22-B7989CFFB003")
    IAudCapture : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetCapLatency( 
            /* [in] */ int nMillis) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetProps( 
            /* [in] */ unsigned char *pProps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMediaType( 
            /* [in] */ unsigned char *pMediaType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetVol( 
            /* [in] */ double fVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVol( 
            /* [in] */ double *pVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelFmt( 
            /* [in] */ int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFmtProps( 
            /* [in] */ int nIndex,
            /* [in] */ WAVEFORMATEX2 *pFmt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFmtCount( 
            /* [in] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelInPin( 
            /* [in] */ int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInPinProps( 
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pPinNamet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInPinCount( 
            /* [in] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SelDev( 
            /* [in] */ int nIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevProps( 
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pDevName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDevCount( 
            /* [in] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddReceiver( 
            /* [in] */ IAudReceiver *pReceiver,
            /* [in] */ unsigned char *pParam) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DelReceiver( 
            /* [in] */ IAudReceiver *pReceiver) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitGraph( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE UnInitGraph( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAudCaptureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAudCapture * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAudCapture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAudCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetCapLatency )( 
            IAudCapture * This,
            /* [in] */ int nMillis);
        
        HRESULT ( STDMETHODCALLTYPE *GetProps )( 
            IAudCapture * This,
            /* [in] */ unsigned char *pProps);
        
        HRESULT ( STDMETHODCALLTYPE *GetMediaType )( 
            IAudCapture * This,
            /* [in] */ unsigned char *pMediaType);
        
        HRESULT ( STDMETHODCALLTYPE *SetVol )( 
            IAudCapture * This,
            /* [in] */ double fVal);
        
        HRESULT ( STDMETHODCALLTYPE *GetVol )( 
            IAudCapture * This,
            /* [in] */ double *pVal);
        
        HRESULT ( STDMETHODCALLTYPE *SelFmt )( 
            IAudCapture * This,
            /* [in] */ int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE *GetFmtProps )( 
            IAudCapture * This,
            /* [in] */ int nIndex,
            /* [in] */ WAVEFORMATEX2 *pFmt);
        
        HRESULT ( STDMETHODCALLTYPE *GetFmtCount )( 
            IAudCapture * This,
            /* [in] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *SelInPin )( 
            IAudCapture * This,
            /* [in] */ int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE *GetInPinProps )( 
            IAudCapture * This,
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pPinNamet);
        
        HRESULT ( STDMETHODCALLTYPE *GetInPinCount )( 
            IAudCapture * This,
            /* [in] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *SelDev )( 
            IAudCapture * This,
            /* [in] */ int nIndex);
        
        HRESULT ( STDMETHODCALLTYPE *GetDevProps )( 
            IAudCapture * This,
            /* [in] */ int nIndex,
            /* [in] */ wchar_t *pDevName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDevCount )( 
            IAudCapture * This,
            /* [in] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *AddReceiver )( 
            IAudCapture * This,
            /* [in] */ IAudReceiver *pReceiver,
            /* [in] */ unsigned char *pParam);
        
        HRESULT ( STDMETHODCALLTYPE *DelReceiver )( 
            IAudCapture * This,
            /* [in] */ IAudReceiver *pReceiver);
        
        HRESULT ( STDMETHODCALLTYPE *InitGraph )( 
            IAudCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *UnInitGraph )( 
            IAudCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *Run )( 
            IAudCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IAudCapture * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IAudCapture * This);
        
        END_INTERFACE
    } IAudCaptureVtbl;

    interface IAudCapture
    {
        CONST_VTBL struct IAudCaptureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAudCapture_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAudCapture_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAudCapture_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAudCapture_SetCapLatency(This,nMillis)	\
    ( (This)->lpVtbl -> SetCapLatency(This,nMillis) ) 

#define IAudCapture_GetProps(This,pProps)	\
    ( (This)->lpVtbl -> GetProps(This,pProps) ) 

#define IAudCapture_GetMediaType(This,pMediaType)	\
    ( (This)->lpVtbl -> GetMediaType(This,pMediaType) ) 

#define IAudCapture_SetVol(This,fVal)	\
    ( (This)->lpVtbl -> SetVol(This,fVal) ) 

#define IAudCapture_GetVol(This,pVal)	\
    ( (This)->lpVtbl -> GetVol(This,pVal) ) 

#define IAudCapture_SelFmt(This,nIndex)	\
    ( (This)->lpVtbl -> SelFmt(This,nIndex) ) 

#define IAudCapture_GetFmtProps(This,nIndex,pFmt)	\
    ( (This)->lpVtbl -> GetFmtProps(This,nIndex,pFmt) ) 

#define IAudCapture_GetFmtCount(This,pCount)	\
    ( (This)->lpVtbl -> GetFmtCount(This,pCount) ) 

#define IAudCapture_SelInPin(This,nIndex)	\
    ( (This)->lpVtbl -> SelInPin(This,nIndex) ) 

#define IAudCapture_GetInPinProps(This,nIndex,pPinNamet)	\
    ( (This)->lpVtbl -> GetInPinProps(This,nIndex,pPinNamet) ) 

#define IAudCapture_GetInPinCount(This,pCount)	\
    ( (This)->lpVtbl -> GetInPinCount(This,pCount) ) 

#define IAudCapture_SelDev(This,nIndex)	\
    ( (This)->lpVtbl -> SelDev(This,nIndex) ) 

#define IAudCapture_GetDevProps(This,nIndex,pDevName)	\
    ( (This)->lpVtbl -> GetDevProps(This,nIndex,pDevName) ) 

#define IAudCapture_GetDevCount(This,pCount)	\
    ( (This)->lpVtbl -> GetDevCount(This,pCount) ) 

#define IAudCapture_AddReceiver(This,pReceiver,pParam)	\
    ( (This)->lpVtbl -> AddReceiver(This,pReceiver,pParam) ) 

#define IAudCapture_DelReceiver(This,pReceiver)	\
    ( (This)->lpVtbl -> DelReceiver(This,pReceiver) ) 

#define IAudCapture_InitGraph(This)	\
    ( (This)->lpVtbl -> InitGraph(This) ) 

#define IAudCapture_UnInitGraph(This)	\
    ( (This)->lpVtbl -> UnInitGraph(This) ) 

#define IAudCapture_Run(This)	\
    ( (This)->lpVtbl -> Run(This) ) 

#define IAudCapture_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IAudCapture_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAudCapture_INTERFACE_DEFINED__ */



#ifndef __IAudCaptureLib_LIBRARY_DEFINED__
#define __IAudCaptureLib_LIBRARY_DEFINED__

/* library IAudCaptureLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_IAudCaptureLib;

EXTERN_C const CLSID CLSID_AudCapture;

#ifdef __cplusplus

class DECLSPEC_UUID("03B60FE0-7168-44f2-8D22-B7989CFFB000")
AudCapture;
#endif
#endif /* __IAudCaptureLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


