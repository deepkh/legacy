#include "StdAfx.h"
#include "CCMediaPlay.h"


HRESULT GetUnconnectedPin(
						  IBaseFilter *pFilter,   // Pointer to the filter.
						  PIN_DIRECTION PinDir,   // Direction of the pin to find.
						  IPin **ppPin)           // Receives a pointer to the pin.
{
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
	
    if (!ppPin)
        return E_POINTER;
    *ppPin = 0;
	
    // Get a pin enumerator
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
        return hr;
	
    // Look for the first unconnected pin
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
		
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;
			
            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))  // Already connected, not the pin we want.
            {
                pTmp->Release();
            }
            else  // Unconnected, this is the pin we want.
            {
                pEnum->Release();
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }
	
    // Release the enumerator
    pEnum->Release();
	
    // Did not find a matching pin
    return E_FAIL;
}

void AFX_CDECL MyAfxTrace(LPCTSTR format, ...)
{
	static TCHAR buffer[512];
	va_list args;
	va_start(args,format);
	_vstprintf(buffer,format,args);
	//::MessageBox(0, buffer, 0, MB_OK);
	::OutputDebugString(buffer);
}

HRESULT BindToDevice(GUID guid, const char *szFriendlyName, void **pBaseFilter)
{

	HRESULT hr = NULL;
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = ::CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
							IID_ICreateDevEnum, (void **) &pSysDevEnum);
	if (FAILED(hr)) {
//		::MessageBox(0, "CoCreateInstance CreateSystemEnum Faild.", 0, MB_OK);
		return -1;
	}

	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(guid, &pEnumCat, 0);

	if (hr == S_OK)
	{
		IMoniker *pMoniker = NULL;
		ULONG cFetched = 0;
		while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			IPropertyBag *pPropBag = NULL;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **) &pPropBag);
			if (SUCCEEDED(hr))
			{
				VARIANT varName;
				VariantInit(&varName);

				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
					
				if (SUCCEEDED(hr))
				{
					char szChar[512] = {0};
					WideCharToMultiByte(CP_ACP, 0, varName.bstrVal, -1, szChar, 512, 0, 0);
					if (strcmp(szChar, szFriendlyName) == 0)
					{
						wprintf(L"Found Device Name: %s\n", varName.bstrVal);
						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, pBaseFilter);
						pPropBag->Release();
						pMoniker->Release();
						pEnumCat->Release();
						pSysDevEnum->Release();
						return hr;
					}
				}

				VariantClear(&varName);
				pPropBag->Release();
			}
			pMoniker->Release();	
		}
		pEnumCat->Release();
	}

	pSysDevEnum->Release();

	return -1;
}

IPin *GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	BOOL bFound = FALSE;
	IEnumPins *pEnum;
	IPin *pPin;

	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
//		MessageBox(0, "GetPing Err.", 0, MB_OK);
		return NULL;
	}

	while(pEnum->Next(1, &pPin, 0) == S_OK)
	{
		PIN_DIRECTION PinDirThis;
		IPin *pTmp = 0;
		pPin->QueryDirection(&PinDirThis);

		if (bFound = (PinDir == PinDirThis)) {
			pPin->Release();
			break;
		}
		pPin->Release();
	}

	pEnum->Release();
	return (bFound ? pPin : NULL);
}



HRESULT GetUnConnectPin(IBaseFilter *pFilter,
						PIN_DIRECTION PinDirect,
						IPin **pPin)
{
	HRESULT hr = NULL;
	IEnumPins *pEnum = NULL;
	IPin *pPin2 = NULL;
	BOOL bFound = FALSE;

	if (pFilter == NULL)
	{
		MessageBox(0, _T("Err5"), 0, MB_OK);
		return E_POINTER;
	}


	hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr)) 
	{
		MessageBox(0, _T("Err6"), 0, MB_OK);
		return hr;
	}

	while(pEnum->Next(1, &pPin2, 0) == S_OK)
	{
		IPin *pTmp = NULL;
		PIN_DIRECTION PinDirect2;
		PIN_INFO pInfo;
		pPin2->QueryPinInfo(&pInfo);
		//wprintf(L"achName: %s\n", pInfo.achName);
		TRACE(_T("achName: %s\n"), pInfo.achName);
		pPin2->QueryDirection(&PinDirect2);
		if (PinDirect2 == PinDirect)
		{
			hr = pPin2->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr)) { // has connected
				pTmp->Release();
			} else {			 // not connect, that's we want
				pPin2->Release();
				bFound = TRUE;
				*pPin = pPin2;
				break;
			}
		}
		pPin2->Release();
	}
	pEnum->Release();

	return (bFound ? (HRESULT) 1 : (HRESULT) -1);
}

HRESULT ConnectFilters(IGraphBuilder *pGraph,
					IPin *pOutPin,
					IBaseFilter *pInFilter) 
{
	HRESULT hr = NULL;
	IPin *pInPin;
	if (pGraph == NULL || pOutPin == NULL || pInFilter == NULL)
	{
		MessageBox(0, _T("Err3"), 0, MB_OK);
		return E_POINTER;
	}

	hr = GetUnConnectPin(pInFilter, PINDIR_INPUT, &pInPin);
	if (FAILED(hr))
	{
		MessageBox(0, _T("Err4"), 0, MB_OK);
		return hr;
	}

	hr = pGraph->Connect(pOutPin, pInPin);
	pOutPin->Release();
	pInPin->Release();
	return hr;

	//return pGraph->Connect(pOutPin, pInPin);
}


HRESULT ConnectFilters(IGraphBuilder *pGraph,
					IBaseFilter *pOutFilter,
					IBaseFilter *pInFilter) 
{
	HRESULT hr = NULL;
	IPin *pOutPin = NULL;

	if (pGraph == NULL || pOutFilter == NULL || pInFilter == NULL)
	{
		MessageBox(0, _T("Err1"), 0, MB_OK);
		return E_POINTER;
	}

	
	hr = GetUnConnectPin(pOutFilter, PINDIR_OUTPUT, &pOutPin);
	if (FAILED(hr))
	{
		MessageBox(0, _T("Err2"), 0, MB_OK);
		return hr;
	}

	return ConnectFilters(pGraph, pOutPin, pInFilter);
}



HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath) 
{
    const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
    HRESULT hr;
    
    IStorage *pStorage = NULL;
    hr = StgCreateDocfile(
        wszPath,
        STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0, &pStorage);
    if(FAILED(hr)) 
    {
        return hr;
    }

    IStream *pStream;
    hr = pStorage->CreateStream(
        wszStreamName,
        STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
        0, 0, &pStream);
    if (FAILED(hr)) 
    {
        pStorage->Release();    
        return hr;
    }

    IPersistStream *pPersist = NULL;
    pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
    hr = pPersist->Save(pStream, TRUE);
    pStream->Release();
    pPersist->Release();
    if (SUCCEEDED(hr)) 
    {
        hr = pStorage->Commit(STGC_DEFAULT);
    }
    pStorage->Release();
    return hr;
}

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker * pMoniker;
    IRunningObjectTable *pROT;
    if (FAILED(GetRunningObjectTable(0, &pROT))) {
        return E_FAIL;
    }
    WCHAR wsz[256];
    wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
            pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
    return hr;
}

void RemoveFromRot(DWORD pdwRegister)
{
    IRunningObjectTable *pROT;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}


HRESULT InitWindowlessVMR(
		HWND hWnd,
		IGraphBuilder *pGraph,
		IBaseFilter **ppVmr, 
		IVMRWindowlessControl9 **ppWc)
{
	IBaseFilter *pVmr = NULL;
	IVMRWindowlessControl9 *pWc = NULL;
	IVMRFilterConfig9 *pCfg = NULL;
	HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL,
								  CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pVmr);
	if (FAILED(hr)) {
		return hr;
	}

	hr = pGraph->AddFilter(pVmr, L"Video Mixing Renderer9");
	if (FAILED(hr)) {
		pVmr->Release();
		return hr;
	}

	hr = pVmr->QueryInterface(IID_IVMRFilterConfig9, (void **)&pCfg);
	if (SUCCEEDED(hr))
		hr = pCfg->SetRenderingMode(VMRMode_Windowless);


	if (SUCCEEDED(hr)) {
		hr = pVmr->QueryInterface(IID_IVMRWindowlessControl9, (void **)&pWc);
		if (SUCCEEDED(hr)) {
			hr = pWc->SetVideoClippingWindow(hWnd);
			
			if (SUCCEEDED(hr)) {
				hr = pWc->SetAspectRatioMode(VMR9ARMode_LetterBox);
				if (FAILED(hr)) {
					MessageBox(0, _T("SetAspectRatioMode"), 0, MB_OK);
				}
				*ppWc = pWc;
			} else {
				pWc->Release();
			}
		}
	}

	pCfg->Release();
	if (pVmr)
		*ppVmr = pVmr;
//	pVmr->Release();
	return hr;
}

//////////////////////////////////////////////////////////////////////////
///
///			Class	CCMediaPlay
///
//////////////////////////////////////////////////////////////////////////

GUID CLSID_PDVD7 =	{0xF2E3D920,0x0F9B,0x4319,{0xBE,0x87,0xEB,0x94,0xCC,0xEB,0x6C,0x09}};
GUID CLSID_CoreAVC = {0x09571A4B,0xF1FE,0x4C60,{0x97,0x60,0xDE,0x6D,0x31,0x0C,0x7C,0x31}};
GUID CLSID_PDVD8 = {0xC16541FF,0x49ED,0x4DEA,{0x91,0x26,0x86,0x2F,0x57,0x72,0x2E,0x31}};
GUID CLSID_PDVD8_VC1 = {0x9EC248ED,0x333F,0x4A19,{0x9C,0xEC,0x5E,0x6E,0xB5,0x13,0x29,0x0C}};

HRESULT AddFilterByCLSID(
						 IGraphBuilder *pGraph,  // Pointer to the Filter Graph Manager.
						 const GUID& clsid,      // CLSID of the filter to create.
						 LPCWSTR wszName,        // A name for the filter.
						 IBaseFilter **ppF)      // Receives a pointer to the filter.
{
    if (!pGraph || ! ppF) return E_POINTER;
    *ppF = 0;
    IBaseFilter *pF = 0;
    HRESULT hr = CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, reinterpret_cast<void**>(&pF));
	

    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter(pF, wszName);
        if (SUCCEEDED(hr))
            *ppF = pF;
        else
            pF->Release();
    } else {
		MessageBox(0, _T("Can't Load CLSID"),0,MB_OK);
	}
    return hr;
}


IBaseFilter *pCyber7 = 0;

CCMediaPlay::CCMediaPlay(HWND hWnd)
			:pGraph(0), pControl(0), pEvent(0), pEventEx(0), pSeeking(0), pBA(0),pVmr(0),pWc(0),
			hWnd(hWnd),bCanSeek(0),
			pAudioRenderer(0),bRun(FALSE),pVideoWindow(0)
{
	HRESULT hr = NULL;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	CHECKED(hr, _T("CoInitializeEx"));

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
	CHECKED(hr, _T("CLSID_FilterGraph"));

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **) &pControl);
	CHECKED(hr, _T("IID_IMediaControl"));

	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent);
	CHECKED(hr, _T("IID_IMediaEvent"));

	hr = pGraph->QueryInterface(IID_IMediaEventEx, (void **) &pEventEx);
	CHECKED(hr, _T("IMediaEventEx"));

	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **) &pSeeking);
	CHECKED(hr, _T("IMediaSeeking"));

	hr = pGraph->QueryInterface(IID_IBasicAudio, (void **) &pBA);
	CHECKED(hr, _T("IID_IBasicAudio"));

	hr = pEventEx->SetNotifyWindow((OAHWND) hWnd, WM_GRAPHNOTIFY, 0);
	CHECKED(hr, _T("SetNotifyWnd"));


	// vmr
//	hr = InitWindowlessVMR(hWnd, pGraph, &pVmr, &pWc);
//	CHECKED(hr, _T("InitWindowlessVMR"));

	
#ifdef _DEBUG
	DWORD dwRegister;
	AddToRot(pGraph, &dwRegister);
#endif	

//	IBaseFilter *pVob = 0;
//	hr = BindToDevice(CLSID_LegacyAmFilterCategory, "DirectVobSub (auto-loading version)", (void **) &pVob);
//	CHECKED(hr, _T("Vob"));
//	hr = pGraph->AddFilter(pVob, L"VOB");
//	CHECKED(hr, _T("AddFilter(vob add"));
//	pVob->Release();

	
	//hr = AddFilterByCLSID(pGraph, CLSID_PDVD8, L"PDVD7", &pCyber7); 
//	BindToDevice(CLSID_LegacyAmFilterCategory, "CyberLink H.264/AVC Decoder (PDVD8)", (void **) &pCyber7);
//	CHECKED(hr, _T("CyberLink H.264/AVC Decoder (PDVD8)"));
//	hr = pGraph->AddFilter(pCyber7, L"CyberLink H.264/AVC Decoder (PDVD8)");
//	CHECKED(hr, _T("AddFilter(vob add"));
//	pCyber7->Release();
	
										 
//	CHECKED(hr, _T("CyberLink H.264/AVC Decoder"));
//	pCyber7->Release();
	
	// Audio Renderer
//	hr = BindToDevice(CLSID_AudioRendererCategory, "Default DirectSound Device", (void **)&pAudioRenderer);
//	CHECKED(hr, _T("BindToDevice AudioRenderer"));
	
//	hr = pGraph->AddFilter(pAudioRenderer, L"Default DirectSound Device");
//	CHECKED(hr, _T("AddFilter(pAudioRenderer"));


	hr = pGraph->QueryInterface(IID_IVideoWindow, (void **) &pVideoWindow);
	CHECKED(hr, _T("CoInitializeEx"));

	pVideoWindow->put_Owner((OAHWND) hWnd);
	pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);

}

#define SAFE_RELEASE(p) {if ( (p) ) {(p)->Release(); (p) = 0; }}

CCMediaPlay::~CCMediaPlay()
{

	Stop();

	if (pEventEx)
		pEventEx->SetNotifyWindow(0, WM_GRAPHNOTIFY, 0);


	IEnumFilters *pEnum = NULL;
	HRESULT hr = pGraph->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			pGraph->RemoveFilter(pFilter);
			pEnum->Reset();
			pFilter->Release();
		}
		pEnum->Release();
	}
	SaveGraphFile(pGraph, _T("abc2.grf"));

	SAFE_RELEASE(pWc);
	SAFE_RELEASE(pVmr);
	SAFE_RELEASE(pBA);
	SAFE_RELEASE(pSeeking);
	SAFE_RELEASE(pEventEx);
	SAFE_RELEASE(pEvent);
	SAFE_RELEASE(pControl);
	SAFE_RELEASE(pGraph);


	
	TRACE(_T("[CCMediaPlay::~CCMediaPlay()]\n"));

  
#ifdef _DEBUG
	DWORD dwRegister;
	RemoveFromRot(dwRegister);
#endif
	CoUninitialize();

/*
	if (bRun)
		pControl->Stop();

	if (pAudioRenderer) {
		pAudioRenderer->Release();
	}

	if(pVmr) {
		pVmr->Release();
	}

	if (pWc) {

		pWc->Release();
	}

	if (pBA)
		pBA->Release();

	if (pSeeking) {
		pSeeking->Release();
	}

	if (pEventEx) {
		pEventEx->SetNotifyWindow(0, 0, 0);
		pEventEx->Release();
	}

	if (pEvent) 
		pEvent->Release();
	
	if (pControl)
		pControl->Release();



	IEnumFilters *pEnum = NULL;
	HRESULT hr = pGraph->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			pGraph->RemoveFilter(pFilter);
			pEnum->Reset();
			pFilter->Release();
		}
		pEnum->Release();
	}
	

	SaveGraphFile(pGraph, _T("abc2.grf"));

	
	if (pGraph)
		pGraph->Release();

	TRACE(_T("[CCMediaPlay::~CCMediaPlay()]\n"));

	CoUninitialize();*/
}

char *tombcs(wchar_t *szwcs)
{
	static char mbcs[512];
	memset(mbcs, 0, 512);
	wcstombs(mbcs, szwcs, 512);
	return mbcs;
}

void CHK(int i, HRESULT hr)
{
	if (FAILED(hr))
	{
		mbox(_T("FAILED@%2d : %x"), i, hr);
		exit(0);
	}
}

void RemoveDownstream(IGraphBuilder *pGraph, IBaseFilter *pFilter)
{
	IEnumPins *pEnum = 0;
	IPin *pPin = 0, *pPin2 = 0;
	CHK(999, pFilter->EnumPins(&pEnum));
	ULONG lFetched = 0;
	while(pEnum->Next(1, &pPin,&lFetched) == S_OK)
	{
		PIN_INFO info, info2;
		CHK(9991, pPin->QueryPinInfo(&info));
		pPin->ConnectedTo(&pPin2);
		if (info.dir == PINDIR_OUTPUT && pPin2)
		{
			CHK(9992, pPin2->QueryPinInfo(&info2));
			RemoveDownstream(pGraph, info2.pFilter);
			pGraph->Disconnect(pPin2);
			pGraph->Disconnect(pPin);
			pGraph->RemoveFilter(info2.pFilter);
			pPin2->Release();
		}
		pPin->Release();
		
	}
	pEnum->Release();
}

HRESULT BindFilterByCLSID(const GUID& clsid,      // CLSID of the filter to create.
						  IBaseFilter **ppF)      // Receives a pointer to the filter.
{
    *ppF = 0;
    IBaseFilter *pF = 0;
    HRESULT hr = CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER,
        IID_IBaseFilter, reinterpret_cast<void**>(&pF));
    if (SUCCEEDED(hr))
    {
        if (SUCCEEDED(hr))
            *ppF = pF;
        else
            pF->Release();
    }
    return hr;
}

HRESULT BindFilterByName(GUID inCategory, WCHAR *szName, IBaseFilter **pFilter)
{
	HRESULT hr = S_FALSE;
	ICreateDevEnum *pDevEnum = 0;
	IEnumMoniker *pEnumMoniker = 0;
	IMoniker *pMoniker = 0;
	IPropertyBag *pPropertyBag = 0;
	ULONG uFetched = 0;
	CHK(0,CoCreateInstance(CLSID_SystemDeviceEnum, 0, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **) &pDevEnum));
	CHK(0, pDevEnum->CreateClassEnumerator(inCategory, &pEnumMoniker, 0));
	
	
	while(pEnumMoniker->Next(1, &pMoniker, &uFetched) == S_OK)
	{
		/** FriendlyName */
		
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **) &pPropertyBag);
		if (hr == S_OK)
		{
			VARIANT var;
			VariantInit(&var);
			hr = pPropertyBag->Read(L"FriendlyName", &var, 0);
			if (hr == S_OK)
			{
				if (wcscmp(var.bstrVal, szName) == 0)
				{
					hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **) pFilter);
					VariantClear(&var);
					goto JUMPOUT;
				}
				SAFE_RELEASE(pPropertyBag);
			}
			VariantClear(&var);
		}
		SAFE_RELEASE(pMoniker);
	}
	
JUMPOUT:
	SAFE_RELEASE(pPropertyBag);
	SAFE_RELEASE(pMoniker);
	SAFE_RELEASE(pEnumMoniker);
	SAFE_RELEASE(pDevEnum);
	return hr;
}

void GetCrossbarFilter(IBaseFilter *pVideoCap, IBaseFilter **pCrossFilter)
{
	IEnumPins *pEnum = 0;
	IPin *pPin = 0, *pPin2 = 0;
	CHK(999, pVideoCap->EnumPins(&pEnum));
	ULONG lFetched = 0;
	while(pEnum->Next(1, &pPin,&lFetched) == S_OK)
	{
		PIN_INFO info, info2;
		CHK(9991, pPin->QueryPinInfo(&info));
		pPin->ConnectedTo(&pPin2);
		if (info.dir == PINDIR_INPUT && pPin2)
		{
			CHK(9992, pPin2->QueryPinInfo(&info2));
			pCrossFilter[0] = info2.pFilter;
			pPin->Release();
			pPin2->Release();
			pEnum->Release();
			return;
		}
		pPin->Release();
	}
	pEnum->Release();
	pCrossFilter[0] = 0;
}



void CCMediaPlay::PlayFile(const WCHAR *szFile)
{
//	CComPtr <IFilterGraph2> pFG;
//	CComPtr <IPin> pOutputPin;
//	CComPtr <IBaseFilter> pSource;
//	CComPtr <IBaseFilter> pAudioRenderer;
//	IBaseFilter *pSource = 0;


	CString str = szFile;
	
	str = str.Left(str.ReverseFind(_T('\\'))+1);
	::SetCurrentDirectory(str);
	
	TRACE(_T("%s\n"),str);

	// source
//	HRESULT hr = pGraph->AddSourceFilter(szFile, L"SOURCE", &pSource);
//	CHECKED(hr, _T("AddSourceFilter"));


	
//	hr = pGraph->QueryInterface(IID_IFilterGzzzraph2, (void **)&pFG);
//	CHECKED(hr, _T("IID_IFilterGraph2"));
	
//	hr = GetUnconnectedPin(pSource, PINDIR_OUTPUT, &pOutputPin);
//	CHECKED(hr, _T("GetUnconnectedPin"));
	
//	pFG->RenderEx(pOutputPin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL);
//	CHECKED(hr, _T("RenderEx"));



//	hr = ConnectFilters(pGraph, pSource, pVmr);
//	CHECKED(hr, _T("ConnectFilters pSource => pVmr"));

//	hr = ConnectFilters(pGraph, pSource, pAudioRenderer);
//	if (hr != S_OK) {
//		::MessageBox(0, _T("沒聲音."), 0, MB_OK);
	//	pGraph->RemoveFilter(pAudioRenderer);
//	}
	//CHECKED(hr, _T("ConnectFilters(ConnectFilters"));
	

//	pSource->Release();
	
//	HRESULT hr = pGraph->RenderFile(szFile, NULL);
//	CHECKED(hr, _T("pGraph->RenderFile"));






	HRESULT hr = 0;

	IBaseFilter *pVideoCap = 0;
	CHK(99, BindFilterByName(CLSID_VideoInputDeviceCategory, L"713x BDA Analog Capture", &pVideoCap));
	CHK(101, pGraph->AddFilter(pVideoCap, L"713x BDA Analog Capture")); 
	
	IBaseFilter *pFilter = 0;
	CGrayTransform *pTrans = new CGrayTransform;
	CHK(123, pTrans->QueryInterface(IID_IBaseFilter, (void **) &pFilter));
	CHK(101, pGraph->AddFilter(pFilter, L"CGrayTransform")); 
	

	ICaptureGraphBuilder2 *pBuilder = 0;
	CHK(102, CoCreateInstance(CLSID_CaptureGraphBuilder2, 0, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&pBuilder));
	CHK(103, pBuilder->SetFiltergraph(pGraph));
	CHK(104, pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pVideoCap, 0, 0));
	RemoveDownstream(pGraph, pVideoCap);


	IBaseFilter *pSmartTee = 0;
	CHK(239, BindFilterByCLSID(CLSID_SmartTee, &pSmartTee));
	CHK(249, pGraph->AddFilter(pSmartTee, L"Smart Tee"));
	
	IBaseFilter *pVideoRender = 0;
	CHK(259, BindFilterByCLSID(CLSID_VideoRenderer, &pVideoRender));
	CHK(269, pGraph->AddFilter(pVideoRender, L"Video Render"));

	IBaseFilter *pCrossbarFilter = 0;
	GetCrossbarFilter(pVideoCap, &pCrossbarFilter);
	CHK(222, pCrossbarFilter ? S_OK : S_FALSE);


	/************************************************************************/
	/* Crossvar Route                                                                     */
	/************************************************************************/
	IAMCrossbar *pCrossbar = 0;
	long lPinIn = 0, lPinOut = 0;
	long lVideoTuner = 0, lAudioTuner = 0;
	long lVideoDecoder = 0, lAudioDecoder = 0;
	CHK(106, pBuilder->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pVideoCap, IID_IAMCrossbar, (void **)&pCrossbar));
	CHK(107, pCrossbar->get_PinCounts(&lPinOut, &lPinIn));
	
	/** Find PhysConn_Video_Tuner In */
	for (int i=0; i<lPinIn; i++)
	{
		long lRelated = 0;
		long lType = 0;
		CHK(108, pCrossbar->get_CrossbarPinInfo(TRUE, i, &lRelated, &lType));
		if (lType == PhysConn_Video_Tuner)
		{
			lVideoTuner = i;
			printf("[PhysConn_Video_Tuner Pin Index]: %d, [Related Pin]%d\n", i, lRelated);
		}
		if (lType == PhysConn_Audio_Tuner)
		{
			lAudioTuner = i;
			printf("[PhysConn_Video_Tuner Pin Index]: %d, [Related Pin]%d\n", i, lRelated);
		}
	}
	/** Find PhysConn_Video_Tuner Out */
	for ( i=0; i<lPinOut; i++)
	{
		long lRelated = 0;
		long lType = 0;
		CHK(108, pCrossbar->get_CrossbarPinInfo(FALSE, i, &lRelated, &lType));
		if (lType == PhysConn_Video_VideoDecoder)
		{
			lVideoDecoder = i;
			printf("[PhysConn_Video_VideoDecoder Pin Index]: %d, [Related Pin]%d\n", i, lRelated);
		}
		if (lType == PhysConn_Audio_AudioDecoder)
		{
			lAudioDecoder = i;
			printf("[PhysConn_Audio_AudioDecoder Pin Index]: %d, [Related Pin]%d\n", i, lRelated);
		}
	}
	
	CHK(109, pCrossbar->Route(lVideoDecoder, lVideoTuner));
	CHK(119, pCrossbar->Route(lAudioDecoder, lAudioTuner));

	/************************************************************************/
	/* Tuner Set Mode, CountryCode, Channel                                 */
	/************************************************************************/
	IAMTVTuner *pTuner = 0;
	CHK(106, pBuilder->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pVideoCap, IID_IAMTVTuner, (void **)&pTuner));
	long lModes = 0;
	CHK(108, pTuner->GetAvailableModes(&lModes));
	if (lModes & AMTUNER_MODE_TV)
	{
		CHK(109, pTuner->put_Mode(AMTUNER_MODE_TV));
	}
	CHK(110, pTuner->put_CountryCode(886));
	CHK(111, pTuner->put_Channel(52, AMTUNER_SUBCHAN_DEFAULT , AMTUNER_SUBCHAN_DEFAULT ));


	IAMAnalogVideoDecoder *pVideoDecoder = 0;
	CHK(999, pVideoCap->QueryInterface(IID_IAMAnalogVideoDecoder, (void **)&pVideoDecoder));
	pVideoDecoder->put_TVFormat(AnalogVideo_NTSC_M);
	pVideoDecoder->Release();

	IAMStreamConfig *pCfg = 0;
	CHK(9991, pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, 0, pVideoCap, IID_IAMStreamConfig, (void **)&pCfg));

		

	VIDEO_STREAM_CONFIG_CAPS  pSCC;
	AM_MEDIA_TYPE * pmt = NULL;
	
	int nCounts, nSize;
	int preferredIndex = -1;
	enum {
		 RGB24= 0, RGB32 , RGB565, RGB555, YUY2, UYVY, Unknown
	} currentPreferred, temp;
	currentPreferred = Unknown;


	pCfg->GetNumberOfCapabilities(&nCounts, &nSize);
		for ( i = 0; i < nCounts; i++)
		{
			if (pCfg->GetStreamCaps(i, &pmt, (BYTE *)&pSCC) == S_OK)
			{
				if (pmt->subtype == MEDIASUBTYPE_RGB32)
				{
					temp = RGB32;
					printf("RGB32\n");
				}
				else if (pmt->subtype == MEDIASUBTYPE_RGB24)
				{
					printf("RGB24\n");
					temp = RGB24;
				}
				else if (pmt->subtype == MEDIASUBTYPE_RGB565)
				{
					printf("RGB32\n");
					temp = RGB565;
				}
				else if (pmt->subtype == MEDIASUBTYPE_RGB555)
				{
					printf("RGB32\n");
					temp = RGB555;
				}
				else if (pmt->subtype == MEDIASUBTYPE_YUY2)
				{
					printf("RGB32\n");
					temp = YUY2;
				}
				else if (pmt->subtype == MEDIASUBTYPE_UYVY)
				{
					printf("UYUY\n");
					temp = UYVY;
				}
				else
				{
					temp = Unknown;
				}
				
				if (temp < currentPreferred)
				{
					currentPreferred = temp;
					preferredIndex   = i;
				}
				DeleteMediaType(pmt);
			}
		}
		
		// Get the preferred RGB media type
		if (preferredIndex != -1)
		{
			hr = pCfg->GetStreamCaps(preferredIndex, &pmt, (BYTE *)&pSCC);
		}
		else
		{
			hr = pCfg->GetFormat(&pmt);
		}
		
		if (pmt->formattype == FORMAT_VideoInfo)
		{
			long width = 320, height = 240;
			long avgTimePerFrame = 333667;       // 29.97fps
			VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pmt->pbFormat;
			pvi->AvgTimePerFrame       = avgTimePerFrame;
			pvi->bmiHeader.biWidth     = width;
			pvi->bmiHeader.biHeight    = height;
			pvi->bmiHeader.biSizeImage = width * height * pvi->bmiHeader.biBitCount / 8;
			CHK(9991, pCfg->SetFormat(pmt));
			printf("zzz\n");
		}
		DeleteMediaType(pmt);



	CHK(259, pBuilder->RenderStream(NULL, &MEDIATYPE_Video, pVideoCap, pSmartTee, pFilter));
	CHK(259, pBuilder->RenderStream(NULL, &MEDIATYPE_Video, pFilter, 0, pVideoRender));






	/** 將 PlayWnd 設成影片大小 */
	if (pWc)
	{
		
		/** VMR7 or VMR9 Windowless config */
		long lWidth, lHeight; 
		HRESULT hr = pWc->GetNativeVideoSize(&lWidth, &lHeight, NULL, NULL); 
		CHECKED(hr, _T("pWc->GetNativeVideoSize"));
		HWND hParent = GetParent(hWnd);
		RECT rect = {0};
		GetWindowRect(hParent, &rect);
		MoveWindow(hParent, rect.left, rect.top, lWidth, lHeight+200, TRUE);
	}

	Resizing();
	SaveGraphFile(pGraph, _T("abc.grf"));

	DWORD dwCaps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
	bCanSeek = (S_OK == pSeeking->GetCapabilities(&dwCaps));

}

void CCMediaPlay::Run()
{



	HRESULT hr = 0;



	if ((hr = pVideoWindow->put_Owner((OAHWND)hWnd)) != S_OK)
		mbox(_T("FAILED a %x"),  hr);
	if ((hr = pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS)) != S_OK)
		mbox(_T("FAILED b  %x"),  hr);
	if ((hr = pVideoWindow->put_Visible(OATRUE)) != S_OK)
		mbox(_T("FAILED c  %x"),  pVideoWindow->put_Visible(OATRUE));

	Resizing();
	CHECKED(
		pControl->Run()
		, _T("pControl->Run()"));
	bRun = TRUE;


	/** 控制顏色
	IVMRMixerControl9 *pmc;
	HRESULT hr = pVmr->QueryInterface(IID_IVMRMixerControl9, (void **) &pmc);
	if (SUCCEEDED(hr)) {
		VMR9ProcAmpControl pac = {0};
		pac.dwSize = sizeof(pac);
		hr = pmc->GetProcAmpControl(0, &pac);
		if (SUCCEEDED(hr)) {
			TRACE(_T("am control succeed. %f %d\n"), pac.Brightness, pac.Contrast);
			pac.Brightness = 12.0f;
			pac.Contrast = pac.Contrast + 0.25f;
			if (SUCCEEDED(pmc->SetProcAmpControl(0, &pac)))
				TRACE(_T("Set OK\n"));


		}
		pmc->Release();
	}*/





	
}

void CCMediaPlay::Stop()
{	
	CHECKED(pControl->Stop(), _T("pControl->Stop()"));
	bRun = FALSE;
}



void CCMediaPlay::Resizing()
{
	if (pWc)
	{
		/** VMR7 or VMR9 Windowless config */
		long lWidth, lHeight; 
		HRESULT hr = pWc->GetNativeVideoSize(&lWidth, &lHeight, NULL, NULL); 
		CHECKED(hr, _T("pWc->GetNativeVideoSize"));


		// Set the source rectangle.
		SetRect(&rcSrc, 0, 0, lWidth, lHeight); 
    
		// Get the window client area.
		GetClientRect(hWnd, &rcDest); 

				
		// Set the destination rectangle.
		SetRect(&rcDest, 0, 0, rcDest.right, rcDest.bottom);
			//rcDest.right, rcDest.bottom); 
    
		// Set the video position.
		hr = pWc->SetVideoPosition(&rcSrc, &rcDest); 
		CHECKED(hr, _T("pWc->SetVideoPosition"));

		//TRACE(_T("\t\t\t\t\t\t\tResize %d\n"));
	} else {
		RECT grc;
		GetClientRect(hWnd, &grc);
		pVideoWindow->SetWindowPosition(0, 0, grc.right-grc.left, grc.bottom-grc.top);
		
	}
}

void CCMediaPlay::Redraw(HDC hdc)
{
	if (pWc)
	{
		
		RECT        rcClient; 
		GetClientRect(hWnd, &rcClient); 
		

		
		HRGN rgnClient = CreateRectRgnIndirect(&rcClient); 
		HRGN rgnVideo  = CreateRectRgnIndirect(&rcDest);  
		CombineRgn(rgnClient, rgnClient, rgnVideo, RGN_DIFF); 

		
		HBRUSH hbr = GetSysColorBrush(COLOR_BTNFACE); 
		FillRgn(hdc, rgnClient, hbr); 

		DeleteObject(hbr); 
		DeleteObject(rgnClient); 
		DeleteObject(rgnVideo);
		
		
		HRESULT hr = pWc->RepaintVideo(hWnd, hdc); 
	}
}

void CCMediaPlay::ProcessedNotify()
{
	if (pEventEx)
	{
		long evCode, param1, param2;
		HRESULT hr = 0;
		while(SUCCEEDED(pEventEx->GetEvent(&evCode, &param1, &param2, 0)))
		{
			pEventEx->FreeEventParams(evCode, param1, param2);
			switch(evCode)
			{
				case EC_COMPLETE:
					MessageBox(0, _T("EC_COMPLETE"), 0, MB_OK);
					return;
				case EC_USERABORT:
					MessageBox(0, _T("EC_USERABORT"), 0, MB_OK);
					return;
				case EC_ERRORABORT:
					MessageBox(0, _T("EC_ERRORABORT"), 0, MB_OK);
					return;
			}
		}
	}
}

void CCMediaPlay::SetVolume(int nVolume)		// 0 ~ 100
{
	LONG lVal = (nVolume-100) * 100;
	if (pBA)
		pBA->put_Volume(lVal);
}

int CCMediaPlay::GetVolume()
{
	if (pBA) {
		LONG lVal = 0;
		pBA->get_Volume(&lVal);
		lVal /= 100;
		lVal += 100;
		return lVal;
	}
	return 0;
}

BOOL CCMediaPlay::CanSeek()
{
	return bCanSeek;
}


LONGLONG CCMediaPlay::GetDuration()
{
	LONGLONG ll = -1;
	if (pSeeking)
	{
		pSeeking->GetDuration(&ll);
	}
	return ll;
}

void CCMediaPlay::CHECKED(HRESULT hr, const TCHAR *szSection)
{
	if (FAILED(hr))
	{
		MessageBox(0, szSection, 0, MB_OK);
		throw CCMediaPlayException(szSection);
	}
}
