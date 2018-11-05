// IAudCapture.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "IAudPlay.h"
#include "AudPlay.h"
#include "..\IAudCapture\registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static LONG g_lLock = 0;
const char g_szFriendlyName[] = "Aud Play";
const char g_szVerProgID[] = "AudPlay";
const char g_szProgID[] = "AudPlay.1";

class CFactoryImp: public IClassFactory
{
private:
    LONG m_lRef;
public:
    CFactoryImp():m_lRef(0){;};
    ~CFactoryImp(){;};
public:
    //IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IClassFactory)
        {
            *ppv = (IClassFactory *) this;
        } else {
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        ((IUnknown *) *ppv)->AddRef();
        return S_OK;
    };
    STDMETHODIMP_(ULONG) AddRef(void)
    {
        return InterlockedIncrement(&m_lRef);
    };
    STDMETHODIMP_(ULONG) Release(void)
    {
        if (InterlockedDecrement(&m_lRef) == 0)
        {
            delete this;
            return 0;
        }
        return m_lRef;
    };

    //IClassFactory
    //一個 IClassFactory 只提供一個 COM Object instance，所以這邊沒有 CLSID 可供判別
    STDMETHODIMP CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv)
    {
		HRESULT hr = E_NOINTERFACE;
		if (riid == IID_IAudPlay)
		{
			CAudPlay *pList = new CAudPlay;
			hr = pList->QueryInterface(riid, ppv);
			if (FAILED(hr))
			{
				delete pList;
			}
		} 
        return hr;
    };

    //
    STDMETHODIMP LockServer(BOOL bLock)
    {
        if (bLock)
            InterlockedIncrement(&g_lLock);
        else
            InterlockedDecrement(&g_lLock);
        return S_OK;
    };
};




// DllGetClassObject - Returns class factory

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (rclsid == CLSID_AudPlay)
    {
        CFactoryImp *pFactory = new CFactoryImp;
        HRESULT hr = pFactory->QueryInterface(riid, ppv);
        //pFactory->Release();
        return hr;
    } else {
        return CLASS_E_CLASSNOTAVAILABLE;
    }
	//return AfxDllGetClassObject(rclsid, riid, ppv);
}


// DllCanUnloadNow - Allows COM to unload DLL

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
	//	return SELFREG_E_TYPELIB;

	//if (!COleObjectFactory::UpdateRegistryAll())
	//	return SELFREG_E_CLASS;
    return RegisterServer((HINSTANCE) &__ImageBase,
                        CLSID_AudPlay,
                        g_szFriendlyName,
                        g_szVerProgID,
                        g_szProgID);
	return S_OK;
}


// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
	//	return SELFREG_E_TYPELIB;

	//if (!COleObjectFactory::UpdateRegistryAll(FALSE))
	//	return SELFREG_E_CLASS;

	return S_OK;
}
