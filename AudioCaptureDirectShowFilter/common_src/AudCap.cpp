// AudCap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AudCap.h"
#include <windows.h>
#include <tchar.h>


AudCap::AudCap()
{
	m_nDev = 0;
	m_nInPin = 0;
	m_nFormat = 0;
	m_nSelDev = -1;
	m_nSelInput = -1;
	m_nSelFormat = -1;

	memset(m_strFriendlyName, 0, sizeof(m_strFriendlyName));
	memset(m_strInPinName, 0, sizeof(m_strInPinName));
	memset(m_pAudCaps, 0, sizeof(m_pAudCaps));
	memset(m_pInputPin, 0, sizeof(m_pInputPin));
	memset(m_pFormat, 0, sizeof(m_pFormat));
	m_pOutputPin = 0;
	m_pMixer = 0;
	m_pMainMixer = 0;
	m_pConfig = 0;
}

AudCap::~AudCap()
{
	Reset();
}

void AudCap::Reset()
{
	int i;
	for (i=0; i<m_nInPin; i++)
	{
		if (m_pInputPin[i])
		{
			m_pInputPin[i]->Release();
		}
	}

	if (m_pOutputPin)
	{
		m_pOutputPin->Release();
	}

	for (i=0; i<m_nDev; i++)
	{
		if (m_pAudCaps[i])
		{
			m_pAudCaps[i]->Release();
		}
	}

	if (m_pMainMixer)
	{
		m_pMainMixer->Release();
	}

	if (m_pMixer)
	{
		m_pMixer->Release();
	}

	if (m_pConfig)
	{
		m_pConfig->Release();
		m_pConfig = 0;
	}

	if (m_nFormat)
	{
		for (int i=0; i<m_nFormat; i++)
		{
			if (m_pFormat[i])
			{
				DeleteMediaType(m_pFormat[i]);
				m_pFormat[i] = 0;
			}
		}
	}

	m_nDev = 0;
	m_nInPin = 0;
	m_nFormat = 0;
	m_nSelDev = -1;
	m_nSelInput = -1;
	m_nSelFormat = -1;

	memset(m_strFriendlyName, 0, sizeof(m_strFriendlyName));
	memset(m_strInPinName, 0, sizeof(m_strInPinName));
	memset(m_pAudCaps, 0, sizeof(m_pAudCaps));
	memset(m_pInputPin, 0, sizeof(m_pInputPin));
	memset(m_pFormat, 0, sizeof(m_pFormat));
	m_pOutputPin = 0;
	m_pMainMixer = 0;
	m_pMixer = 0;
	m_pConfig = 0;
}

HRESULT AudCap::GetProps(ALLOCATOR_PROPERTIES *pProps)
{	
	IAMBufferNegotiation *pNego;
	CHK(m_pOutputPin->QueryInterface(IID_IAMBufferNegotiation, (void **) &pNego));
	CHK(pNego->GetAllocatorProperties(pProps));
	pNego->Release();
	return S_OK;
}

HRESULT AudCap::GetMediaType(AM_MEDIA_TYPE *pMediaType)
{
	AM_MEDIA_TYPE *pmt = 0;
	HRESULT hr = m_pConfig->GetFormat(&pmt);
	if (SUCCEEDED(hr) && pmt)
		*pMediaType = *pmt;
	return hr;
}

HRESULT AudCap::SetCapLatency(int nMillis)
{
	ALLOCATOR_PROPERTIES props;
	WAVEFORMATEX *wf = 0;	
	AM_MEDIA_TYPE *pmt = 0;

	m_pConfig->GetFormat(&pmt);
	wf = (WAVEFORMATEX *) pmt->pbFormat;

	IAMBufferNegotiation *pNego;
	CHK(m_pOutputPin->QueryInterface(IID_IAMBufferNegotiation, (void **) &pNego));

	//pNego->GetAllocatorProperties(&props);		if you want get propertis that pin must be connected
	props.cBuffers = -1;
	props.cbBuffer = (int)(wf->nAvgBytesPerSec * (double)nMillis/1000.0f);
	props.cbAlign = -1;
	props.cbPrefix = -1;
	pNego->SuggestAllocatorProperties(&props);

	pNego->Release();
	return S_OK;
}

HRESULT AudCap::SelFormat(int nIndex)
{
	HRESULT hr;
	if (!(nIndex < m_nFormat) || m_pConfig == 0)
	{
		return S_FALSE;
	}

	m_nSelFormat = nIndex;
	hr = m_pConfig->SetFormat(m_pFormat[m_nSelFormat]);
	return hr;
}

HRESULT AudCap::ListFormat()
{
	HRESULT hr;
	if (m_pConfig == 0)
	{
		return S_FALSE;
	}

	int size;
	if (SUCCEEDED(hr = m_pConfig->GetNumberOfCapabilities(&m_nFormat, &size)))
	{
		if (size == sizeof(AUDIO_STREAM_CONFIG_CAPS))
		{
			AUDIO_STREAM_CONFIG_CAPS caps;
			for (int i=0; i<m_nFormat; i++)
			{
				m_pConfig->GetStreamCaps(i, &m_pFormat[i], (BYTE*)&caps);
			}
		}
	}

	return hr;
}

HRESULT AudCap::SetVol(double fVal)
{
	HRESULT hr;
	if (SUCCEEDED(hr = m_pMixer->put_MixLevel(fVal)))
	{
		_tprintf(_T("SetVol: %s\n"), m_strInPinName[m_nSelInput]);
	} else {
		
		if (SUCCEEDED(hr = m_pMainMixer->put_MixLevel(fVal)))
		{
			_tprintf(_T("SetVol: Main, %.2f\n"), fVal);
		}
	}
	return hr;
}

HRESULT AudCap::GetVol(double *pVal)
{
	HRESULT hr;
	double val;
	if (SUCCEEDED(hr = m_pMixer->get_MixLevel(&val)))
	{
		_tprintf(_T("GetVol: %s\n"), m_strInPinName[m_nSelInput]);
		pVal[0] = val;
	} else {
		if (SUCCEEDED(hr = m_pMainMixer->get_MixLevel(&val)))
		{
			_tprintf(_T("GetVol: Main\n"));
			pVal[0] = val;
		}
	}
	return hr;
}

HRESULT AudCap::SelPin(int nIndex)
{
	if (!(m_nInPin > 0))
	{
		return S_FALSE;
	}

	if (m_pMixer)
	{
		m_pMixer->put_Enable(FALSE);
		m_pMixer->Release();
		m_pMixer = 0;
	}

	HRESULT hr;
	m_nSelInput = nIndex;
	m_pInputPin[m_nSelInput];
	hr = m_pInputPin[m_nSelInput]->QueryInterface(IID_IAMAudioInputMixer, (void **) &m_pMixer);
	if (!(SUCCEEDED(hr)))
	{
		return hr;
	}

	m_pMixer->put_Enable(TRUE);
	return S_OK;
}

HRESULT AudCap::ListPins()
{
	if (m_nSelDev < 0)
	{
		return S_FALSE;
	}

	if (m_pConfig)
	{
		m_pConfig->Release();
		m_pConfig = 0;
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
			m_pInputPin[m_nInPin] = pPin;
			_tcscpy(m_strInPinName[m_nInPin], info.achName);
			//_tprintf(_T("\t%s\n"), m_strInPinName[m_nInPin]);
			m_nInPin++;
		} else {
			m_pOutputPin = pPin;
			if (SUCCEEDED(hr =m_pOutputPin->QueryInterface(IID_IAMStreamConfig, (void **) &m_pConfig)))
			{
				ListFormat();
			}
		}
	}

	pEnum->Release();
	return S_OK;
}

HRESULT AudCap::SelDev(int nIndex)
{
	HRESULT hr;
	
	if (!(nIndex < m_nDev))
	{
		return S_FALSE;
	}

	if (m_pMainMixer)
	{
		m_pMainMixer->Release();
		m_pMainMixer = 0;
	}

	if (m_pMixer)
	{
		m_pMixer->put_Enable(FALSE);
		m_pMixer->Release();
		m_pMixer = 0;
	}

	if (m_nInPin)
	{
		for (int i=0; i<m_nInPin; i++)
		{
			m_pInputPin[i]->Release();
			m_pInputPin[i] = 0;
		}

		memset(m_pInputPin, 0, sizeof(m_pInputPin));
		m_nInPin = 0;
	}

	if (m_nFormat)
	{
		for (int i=0; i<m_nFormat; i++)
		{
			if (m_pFormat[i])
			{
				DeleteMediaType(m_pFormat[i]);
				m_pFormat[i] = 0;
			}
		}
		m_nFormat = 0;
	}

	m_nSelDev = nIndex;
	hr = m_pAudCaps[m_nSelDev]->QueryInterface(IID_IAMAudioInputMixer, (void **) &m_pMainMixer);
	if (!SUCCEEDED(hr))
	{
		return hr;
	}
	ListPins();
	return S_OK;
}

HRESULT AudCap::ListDevices()
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
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumCat, 0);

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