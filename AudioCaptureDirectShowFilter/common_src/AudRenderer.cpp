// AudCap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AudRenderer.h"
#include <windows.h>
#include <tchar.h>

AudRender::AudRender()
{
    m_nDev = 0;
    m_nSelDev = -1;
	m_pInputPin = 0;

    memset(m_strFriendlyName, 0, sizeof(m_strFriendlyName));
    memset(m_strInPinName, 0, sizeof(m_strInPinName));
    memset(m_pAudCaps, 0, sizeof(m_pAudCaps));
}

AudRender::~AudRender()
{
    Reset();
}

void AudRender::Reset()
{
    int i;

    if (m_pInputPin)
    {
        m_pInputPin->Release();
    }

    for (i=0; i<m_nDev; i++)
    {
        if (m_pAudCaps[i])
        {
            m_pAudCaps[i]->Release();
        }
    }

    m_nDev = 0;
    m_nSelDev = -1;
	m_pInputPin = 0;
    memset(m_strFriendlyName, 0, sizeof(m_strFriendlyName));
    memset(m_strInPinName, 0, sizeof(m_strInPinName));
    memset(m_pAudCaps, 0, sizeof(m_pAudCaps));
}


HRESULT AudRender::ListPins()
{
    if (m_nSelDev < 0)
    {
        return S_FALSE;
    }

    IEnumPins *pEnum = 0;
    IPin *pPin = 0;

    HRESULT hr = m_pAudCaps[m_nSelDev]->EnumPins(&pEnum);

    if (!SUCCEEDED(hr))
    {
        return S_FALSE;
    }

    while(pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION dir;
        PIN_INFO info;
        pPin->QueryDirection(&dir);
        pPin->QueryPinInfo(&info);
        if (dir == PINDIR_INPUT)
        {
            m_pInputPin = pPin;
            _tcscpy(m_strInPinName, info.achName);
            break;
        }
    }

    pEnum->Release();
    return S_OK;
}

HRESULT AudRender::SelDev(int nIndex)
{
    HRESULT hr;

    if (!(nIndex < m_nDev))
    {
        return S_FALSE;
    }

    if (m_pInputPin)
    {
        m_pInputPin->Release();
        m_pInputPin = 0;
    }

    m_nSelDev = nIndex;
    ListPins();
    return S_OK;
}

HRESULT AudRender::ListDevices()
{
    HRESULT hr;
    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void **)&pSysDevEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    memset(m_strFriendlyName, 0, sizeof(m_strFriendlyName));
    // Obtain a class enumerator for the video compressor category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioRendererCategory, &pEnumCat, 0);

    if (hr == S_OK)
    {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
        {
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                (void **)&pPropBag);
            if (SUCCEEDED(hr))
            {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    //::MessageBoxW(0, varName.bstrVal, 0, 0);
                    //_tprintf(_T("%s\n"), varName.bstrVal);
                    _tcscpy(m_strFriendlyName[m_nDev], varName.bstrVal);
                    // Display the name in your UI somehow.
                }
                VariantClear(&varName);

                // To create an instance of the filter, do the following:
                hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
                    (void**)&m_pAudCaps[m_nDev++]);
                //m_pAudCaps[m_nDev++]->Release();
                // Now add the filter to the graph.
                //Remember to release pFilter later.
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();

    return S_OK;
}

/*
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "cht");
	CoInitialize(NULL);
	AudCap *pAud = new AudCap;

	if (SUCCEEDED(pAud->ListDevices()))
	{
		for (int i=0; i<pAud->m_nDev; i++)
		{
			_tprintf(_T("%d: %s\n"), i, pAud->m_strFriendlyName[i]);
		}

		int dev;
		_tprintf(_T("Sel dev:"));
		scanf("%d", &dev);

		if (SUCCEEDED(pAud->SelDev(dev)))
		{
			for (int j=0; j<pAud->m_nInPin; j++)
			{
				_tprintf(_T("%d: %s\n"), j, pAud->m_strInPinName[j]);
			}
		}

		int input;
		_tprintf(_T("Sel Input:"));
		scanf("%d", &input);

		if (SUCCEEDED(pAud->SelPin(input)))
		{
			double fVal = 0, fVal2 = 0;
			pAud->GetVol(&fVal);
			_tprintf(_T("Current Vol:%.2f\n"), fVal);
			_tprintf(_T("Set New Vol:"));
			scanf("%.2f", &fVal2);
			pAud->SetVol(fVal2);
		}

		int fmt;
		for (int j=0; j<pAud->m_nFormat; j++)
		{
			WAVEFORMATEX *p = (WAVEFORMATEX *) pAud->m_pFormat[j]->pbFormat;
			_tprintf(_T("wFormatTag: %d\n"), p->wFormatTag);
			_tprintf(_T("nChannel: %d\n"), p->nChannels);
			_tprintf(_T("nSamplePerSec: %d\n"), p->nSamplesPerSec);
			_tprintf(_T("nAvgBytesPerSec: %d\n"), p->nAvgBytesPerSec);
			_tprintf(_T("nBlockAlign: %d\n"), p->nBlockAlign);
			_tprintf(_T("wBitsPerSample: %d\n"), p->wBitsPerSample);
			_tprintf(_T("cbSize: %d\n\n"), p->cbSize);
		}
	}

	delete pAud;



    AudRender *p = new AudRender;
    p->ListDevices();
    for (int i=0; i<p->m_nDev; i++)
    {
        _tprintf(_T("%d %s\n"), i, p->m_strFriendlyName[i]);
    }

    int ndev = 0;
    _tprintf(_T("sel dev:"));
    _tscanf(_T("%d"), &ndev);
    p->SelDev(ndev);

    _tprintf(_T("0 %s\n"), p->m_strInPinName);

    delete p;

	CoUninitialize();
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}

*/
