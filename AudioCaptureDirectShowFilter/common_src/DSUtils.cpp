#include "stdafx.h"
#include "DSUtils.h"


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
		//PIN_INFO pInfo;
		//pPin2->QueryPinInfo(&pInfo);
		//wprintf(L"achName: %s\n", pInfo.achName);
		//TRACE(_T("achName: %s\n"), pInfo.achName);
		
		pPin2->QueryDirection(&PinDirect2);
		//gtrace(L"achName: %s PinDir:%d Match_Dir:%d.\n", pInfo.achName, PinDirect2, PinDirect);
		if (PinDirect2 == PinDirect)
		{
			hr = pPin2->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr)) { // has connected
				//gtrace(L"achName: %s PinDir:%d has connected.\n", pInfo.achName, PinDirect2);
				pTmp->Release();
			} else {			 
				// not connect, that's we want
//				gtrace(L"achName: %s PinDir:%d ok.\n", pInfo.achName, PinDirect2);
				//pPin2->Release(); pupupuppupuuuuuuuuu
				//pInfo.pFilter->Release();
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

	hr = ConnectFilters(pGraph, pOutPin, pInFilter);
	pOutPin->Release();
	return hr;
}


void RemoveDownstream(IGraphBuilder *pGraph, IBaseFilter *pFilter)
{
	IEnumPins *pEnum = 0;
	IPin *pPin = 0, *pPin2 = 0;
	pFilter->EnumPins(&pEnum);
	ULONG lFetched = 0;
	while(pEnum->Next(1, &pPin,&lFetched) == S_OK)
	{
		PIN_INFO info, info2;
		pPin->QueryPinInfo(&info);
		pPin->ConnectedTo(&pPin2);
		if (info.dir == PINDIR_OUTPUT && pPin2)
		{
			pPin2->QueryPinInfo(&info2);
			RemoveDownstream(pGraph, info2.pFilter);
			pGraph->Disconnect(pPin2);
			pPin2->Release();
			pGraph->Disconnect(pPin);
			pPin->Release();
			pGraph->RemoveFilter(info2.pFilter);
		} else {
			pPin->Release();
		}
	}
	pEnum->Release();
}
