#include "stdafx.h"
#include "../common_src/AudCap.h"
#include "../common_src/AudGrabber.h"
#include "../common_src/DSUtils.h"
#include "AudCapture.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

CAudCapture::CAudCapture()
:nStatus(STATUS_STOP)
,dwRegister(0)
,m_lRef(0)
,pAud(0)
,pGrabber(0)
,pGraph(0)
,pControl(0)
,pEvent(0)
{
	pAud = new AudCap;
	pGrabber = new AudGrabber;
	pAud->ListDevices();
}

CAudCapture::~CAudCapture()
{
	gtrace("%s\n", __FUNCTION__);
	UnInitGraph();
	delete pGrabber;
	delete pAud;
}


STDMETHODIMP CAudCapture::GetProps(unsigned char *pProps)
{
	return pAud->GetProps((ALLOCATOR_PROPERTIES *) pProps);
}

STDMETHODIMP CAudCapture::GetMediaType(unsigned char *pMediaType)
{
	return pAud->GetMediaType((AM_MEDIA_TYPE*) pMediaType);
}

STDMETHODIMP CAudCapture::SetCapLatency(int nMillis)
{
	return pAud->SetCapLatency(nMillis);
}


STDMETHODIMP CAudCapture::SetVol(double fVal)
{
	pAud->SetVol(fVal);
	return S_OK;
}

STDMETHODIMP CAudCapture::GetVol(double *pVal)
{
	if (pVal)
	{
		pAud->GetVol(pVal);
	}
	return S_OK;
}

STDMETHODIMP CAudCapture::SelFmt(int nIndex)
{
	if (nIndex < pAud->m_nFormat)
	{
		pAud->SelFormat(nIndex);
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::GetFmtProps(int nIndex, WAVEFORMATEX2 *pFmt)
{
	if (nIndex < pAud->m_nFormat && pFmt)
	{
		memcpy(pFmt, pAud->m_pFormat[nIndex]->pbFormat, sizeof(WAVEFORMATEX2));
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::GetFmtCount(int *pCount)
{
	if (pCount)
	{
		*pCount = pAud->m_nFormat;
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::SelInPin(int nIndex)
{
	if (nIndex < pAud->m_nInPin)
	{
		pAud->SelPin(nIndex);
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::GetInPinProps(int nIndex, wchar_t *pPinName)
{
	if (pPinName && nIndex < pAud->m_nInPin)
	{
		wcscpy(pPinName, pAud->m_strInPinName[nIndex]);
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::GetInPinCount(int *pCount)
{
	if (pCount)
	{
		*pCount = pAud->m_nInPin;
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::SelDev(int nIndex)
{
	if (nIndex < pAud->m_nDev)
	{
		return pAud->SelDev(nIndex);
	}
	return -1;
}
STDMETHODIMP CAudCapture::GetDevProps(int nIndex, wchar_t *pDevName)
{
	if (pDevName && nIndex < pAud->m_nDev)
	{
		wcscpy(pDevName, pAud->m_strFriendlyName[nIndex]);
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudCapture::GetDevCount(int *pCount)
{
	if (pCount)
	{
		*pCount = pAud->m_nDev;
		return S_OK;
	} 
	return -1;
}



STDMETHODIMP CAudCapture::AddReceiver(IAudReceiver *pReceiver, unsigned char *pParam)
{
	pGrabber->AddReceiver(pReceiver, pParam);
	return S_OK;
}

STDMETHODIMP CAudCapture::DelReceiver(IAudReceiver *pReceiver)
{
	pGrabber->DelReceiver(pReceiver);
	return S_OK;
}


STDMETHODIMP CAudCapture::InitGraph()
{
	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **) &pGraph);
	AddToRot(pGraph, &dwRegister);

	CHK(pGraph->QueryInterface(IID_IMediaControl, (void **) &pControl));
	CHK(pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent));

	pAud->m_pAudCaps[pAud->m_nSelDev]->AddRef();
	CHK(pGraph->AddFilter(pAud->m_pAudCaps[pAud->m_nSelDev], pAud->m_strFriendlyName[pAud->m_nSelDev]));

	pGrabber->AddRef();
	CHK(pGraph->AddFilter(pGrabber, L"Audio Grabber"));


	pGrabber->SetFormat(*((WAVEFORMATEX*)pAud->m_pFormat[pAud->m_nSelFormat]->pbFormat));

	CHK(ConnectFilters(pGraph, pAud->m_pAudCaps[pAud->m_nSelDev], pGrabber));

	return S_OK;
}

STDMETHODIMP CAudCapture::UnInitGraph()
{
	if (pGraph)
	{
		//RemoveFilter the Filter that inherits from CBaseFilter
		RemoveDownstream(pGraph, pAud->m_pAudCaps[pAud->m_nSelDev]);
		//Dont RemoveFilter the Filter that filter is provide from windows.
		pGraph->RemoveFilter(pAud->m_pAudCaps[pAud->m_nSelDev]);

		pControl->Release();
		pControl = 0;

		pEvent->Release();
		pEvent = 0;

		RemoveFromRot(dwRegister);
		pGraph->Release();
		pGraph = 0;
	}

	return S_OK;
}

STDMETHODIMP CAudCapture::Run()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_STOP
		|| nStatus == STATUS_PAUSE)
	{
		g_cs.Lock();
		nStatus = STATUS_RUN;
		hr = pControl->Run();
		g_cs.Unlock();
		gtrace("%s\n", __FUNCTION__);
	}
	return hr;
}

STDMETHODIMP CAudCapture::Pause()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_RUN)
	{
		g_cs.Lock();
		nStatus = STATUS_PAUSE;
		hr = pControl->Pause();
		g_cs.Unlock();
	}
	gtrace("%s\n", __FUNCTION__);
	return hr;
}

STDMETHODIMP CAudCapture::Stop()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_RUN
		|| nStatus == STATUS_PAUSE)
	{
		g_cs.Lock();
		nStatus = STATUS_STOP;
		hr = pControl->Stop();
		g_cs.Unlock();
		gtrace("%s\n", __FUNCTION__);
	}
	return hr;
}