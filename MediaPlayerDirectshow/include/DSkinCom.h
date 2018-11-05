

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Thu Oct 30 13:36:21 2008
 */
/* Compiler settings for .\DSkinCom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __DSkinCom_h__
#define __DSkinCom_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDskin_FWD_DEFINED__
#define __IDskin_FWD_DEFINED__
typedef interface IDskin IDskin;
#endif 	/* __IDskin_FWD_DEFINED__ */


#ifndef __Dskin_FWD_DEFINED__
#define __Dskin_FWD_DEFINED__

#ifdef __cplusplus
typedef class Dskin Dskin;
#else
typedef struct Dskin Dskin;
#endif /* __cplusplus */

#endif 	/* __Dskin_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IDskin_INTERFACE_DEFINED__
#define __IDskin_INTERFACE_DEFINED__

/* interface IDskin */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDskin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F8839FB2-B1CC-4B8E-8F5B-D32AC267BC07")
    IDskin : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadSkin( 
            BSTR strSkinPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExitSkin( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SkinWindow( 
            /* [in] */ ULONG hwnd,
            /* [in] */ ULONG uCtrlType,
            /* [in] */ BSTR szXmlLabel,
            /* [in] */ BOOL bChildSkin) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AdjustColor( 
            /* [in] */ ULONG nHue,
            /* [in] */ ULONG nSaturation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableMenuNoBorder( 
            /* [in] */ BOOL bEable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLastError( 
            /* [out][in] */ ULONG *uErrorCode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TrackPopupMenu( 
            /* [in] */ ULONG hMenu,
            /* [in] */ ULONG uFlags,
            /* [in] */ ULONG x,
            /* [in] */ ULONG y,
            /* [in] */ ULONG hWnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddMenuBar( 
            /* [in] */ ULONG Hwnd,
            /* [out][in] */ ULONG *pToolBar,
            /* [in] */ ULONG hMenu,
            /* [in] */ BSTR strToolBarLabel,
            /* [in] */ BSTR strMenuLabel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetIcon( 
            /* [in] */ BSTR strIconLabel,
            /* [out][in] */ ULONG *hIcon) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColor( 
            /* [in] */ BSTR strColorLabel,
            /* [out][in] */ ULONG *crValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBitmap( 
            /* [in] */ BSTR strBitmapLabel,
            /* [out][in] */ ULONG *hBitmap) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFont( 
            /* [in] */ BSTR strFontLabel,
            /* [out][in] */ ULONG *hFont) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDskinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDskin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDskin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDskin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDskin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDskin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDskin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDskin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadSkin )( 
            IDskin * This,
            BSTR strSkinPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExitSkin )( 
            IDskin * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SkinWindow )( 
            IDskin * This,
            /* [in] */ ULONG hwnd,
            /* [in] */ ULONG uCtrlType,
            /* [in] */ BSTR szXmlLabel,
            /* [in] */ BOOL bChildSkin);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AdjustColor )( 
            IDskin * This,
            /* [in] */ ULONG nHue,
            /* [in] */ ULONG nSaturation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableMenuNoBorder )( 
            IDskin * This,
            /* [in] */ BOOL bEable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLastError )( 
            IDskin * This,
            /* [out][in] */ ULONG *uErrorCode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TrackPopupMenu )( 
            IDskin * This,
            /* [in] */ ULONG hMenu,
            /* [in] */ ULONG uFlags,
            /* [in] */ ULONG x,
            /* [in] */ ULONG y,
            /* [in] */ ULONG hWnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddMenuBar )( 
            IDskin * This,
            /* [in] */ ULONG Hwnd,
            /* [out][in] */ ULONG *pToolBar,
            /* [in] */ ULONG hMenu,
            /* [in] */ BSTR strToolBarLabel,
            /* [in] */ BSTR strMenuLabel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetIcon )( 
            IDskin * This,
            /* [in] */ BSTR strIconLabel,
            /* [out][in] */ ULONG *hIcon);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColor )( 
            IDskin * This,
            /* [in] */ BSTR strColorLabel,
            /* [out][in] */ ULONG *crValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBitmap )( 
            IDskin * This,
            /* [in] */ BSTR strBitmapLabel,
            /* [out][in] */ ULONG *hBitmap);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFont )( 
            IDskin * This,
            /* [in] */ BSTR strFontLabel,
            /* [out][in] */ ULONG *hFont);
        
        END_INTERFACE
    } IDskinVtbl;

    interface IDskin
    {
        CONST_VTBL struct IDskinVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDskin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDskin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDskin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDskin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDskin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDskin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDskin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDskin_LoadSkin(This,strSkinPath)	\
    (This)->lpVtbl -> LoadSkin(This,strSkinPath)

#define IDskin_ExitSkin(This)	\
    (This)->lpVtbl -> ExitSkin(This)

#define IDskin_SkinWindow(This,hwnd,uCtrlType,szXmlLabel,bChildSkin)	\
    (This)->lpVtbl -> SkinWindow(This,hwnd,uCtrlType,szXmlLabel,bChildSkin)

#define IDskin_AdjustColor(This,nHue,nSaturation)	\
    (This)->lpVtbl -> AdjustColor(This,nHue,nSaturation)

#define IDskin_EnableMenuNoBorder(This,bEable)	\
    (This)->lpVtbl -> EnableMenuNoBorder(This,bEable)

#define IDskin_GetLastError(This,uErrorCode)	\
    (This)->lpVtbl -> GetLastError(This,uErrorCode)

#define IDskin_TrackPopupMenu(This,hMenu,uFlags,x,y,hWnd)	\
    (This)->lpVtbl -> TrackPopupMenu(This,hMenu,uFlags,x,y,hWnd)

#define IDskin_AddMenuBar(This,Hwnd,pToolBar,hMenu,strToolBarLabel,strMenuLabel)	\
    (This)->lpVtbl -> AddMenuBar(This,Hwnd,pToolBar,hMenu,strToolBarLabel,strMenuLabel)

#define IDskin_GetIcon(This,strIconLabel,hIcon)	\
    (This)->lpVtbl -> GetIcon(This,strIconLabel,hIcon)

#define IDskin_GetColor(This,strColorLabel,crValue)	\
    (This)->lpVtbl -> GetColor(This,strColorLabel,crValue)

#define IDskin_GetBitmap(This,strBitmapLabel,hBitmap)	\
    (This)->lpVtbl -> GetBitmap(This,strBitmapLabel,hBitmap)

#define IDskin_GetFont(This,strFontLabel,hFont)	\
    (This)->lpVtbl -> GetFont(This,strFontLabel,hFont)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_LoadSkin_Proxy( 
    IDskin * This,
    BSTR strSkinPath);


void __RPC_STUB IDskin_LoadSkin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_ExitSkin_Proxy( 
    IDskin * This);


void __RPC_STUB IDskin_ExitSkin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_SkinWindow_Proxy( 
    IDskin * This,
    /* [in] */ ULONG hwnd,
    /* [in] */ ULONG uCtrlType,
    /* [in] */ BSTR szXmlLabel,
    /* [in] */ BOOL bChildSkin);


void __RPC_STUB IDskin_SkinWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_AdjustColor_Proxy( 
    IDskin * This,
    /* [in] */ ULONG nHue,
    /* [in] */ ULONG nSaturation);


void __RPC_STUB IDskin_AdjustColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_EnableMenuNoBorder_Proxy( 
    IDskin * This,
    /* [in] */ BOOL bEable);


void __RPC_STUB IDskin_EnableMenuNoBorder_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_GetLastError_Proxy( 
    IDskin * This,
    /* [out][in] */ ULONG *uErrorCode);


void __RPC_STUB IDskin_GetLastError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_TrackPopupMenu_Proxy( 
    IDskin * This,
    /* [in] */ ULONG hMenu,
    /* [in] */ ULONG uFlags,
    /* [in] */ ULONG x,
    /* [in] */ ULONG y,
    /* [in] */ ULONG hWnd);


void __RPC_STUB IDskin_TrackPopupMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_AddMenuBar_Proxy( 
    IDskin * This,
    /* [in] */ ULONG Hwnd,
    /* [out][in] */ ULONG *pToolBar,
    /* [in] */ ULONG hMenu,
    /* [in] */ BSTR strToolBarLabel,
    /* [in] */ BSTR strMenuLabel);


void __RPC_STUB IDskin_AddMenuBar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_GetIcon_Proxy( 
    IDskin * This,
    /* [in] */ BSTR strIconLabel,
    /* [out][in] */ ULONG *hIcon);


void __RPC_STUB IDskin_GetIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_GetColor_Proxy( 
    IDskin * This,
    /* [in] */ BSTR strColorLabel,
    /* [out][in] */ ULONG *crValue);


void __RPC_STUB IDskin_GetColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_GetBitmap_Proxy( 
    IDskin * This,
    /* [in] */ BSTR strBitmapLabel,
    /* [out][in] */ ULONG *hBitmap);


void __RPC_STUB IDskin_GetBitmap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDskin_GetFont_Proxy( 
    IDskin * This,
    /* [in] */ BSTR strFontLabel,
    /* [out][in] */ ULONG *hFont);


void __RPC_STUB IDskin_GetFont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDskin_INTERFACE_DEFINED__ */



#ifndef __DSkinComLib_LIBRARY_DEFINED__
#define __DSkinComLib_LIBRARY_DEFINED__

/* library DSkinComLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DSkinComLib;

EXTERN_C const CLSID CLSID_Dskin;

#ifdef __cplusplus

class DECLSPEC_UUID("13C32721-A4CB-4D57-A919-5AF9C82C37B2")
Dskin;
#endif
#endif /* __DSkinComLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


