#include "StdAfx.h"
#include "AudPlay.h"
#include "../common_src/AudRenderer.h"
#include "../common_src/AudPusher.h"
#include "../common_src/DSUtils.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

CAudPlay::CAudPlay()
:pBuf_First(0)
,m_hRunThread(0)
,m_bThdExit(1)
,nStatus(STATUS_STOP)
,dwRegister(0)
,m_lRef(0)
,pRender(0)
,pPusher(0)
,pGraph(0)
,pControl(0)
,pEvent(0)
{
	pRender = new AudRender;
	//pPusher = new AudPusher(;
	pRender->ListDevices();
	pBuf_First = Buffer_New((unsigned char *)0, 0, 0);
	gtrace("FIRST BUFFER:%p\n", pBuf_First);
}

CAudPlay::~CAudPlay()
{
	gtrace("%s\n", __FUNCTION__);
	UnInitGraph();
	delete pRender;
	if (pPusher)
		delete pPusher;
	g_cs.Lock();
	Buffer_Del_All(pBuf_First);
	g_cs.Unlock();
}

STDMETHODIMP CAudPlay::SelDev(int nIndex)
{
	if (nIndex < pRender->m_nDev)
	{
		return pRender->SelDev(nIndex);
	}
	return -1;
}
STDMETHODIMP CAudPlay::GetDevProps(int nIndex, wchar_t *pDevName)
{
	if (pDevName && nIndex < pRender->m_nDev)
	{
		wcscpy(pDevName, pRender->m_strFriendlyName[nIndex]);
		return S_OK;
	}
	return -1;
}

STDMETHODIMP CAudPlay::GetDevCount(int *pCount)
{
	if (pCount)
	{
		*pCount = pRender->m_nDev;
		return S_OK;
	} 
	return -1;
}


STDMETHODIMP CAudPlay::InitGraph()
{
	if (pPusher == 0)
	{
		::MessageBoxA(0, "CAudPlay::InitGraph pPusher can't be null.", 0, 0);
		exit(0);
	}

	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **) &pGraph);
	AddToRot(pGraph, &dwRegister);

	CHK(pGraph->QueryInterface(IID_IMediaControl, (void **) &pControl));
	CHK(pGraph->QueryInterface(IID_IMediaEvent, (void **) &pEvent));

	pRender->m_pAudCaps[pRender->m_nSelDev]->AddRef();
	CHK(pGraph->AddFilter(pRender->m_pAudCaps[pRender->m_nSelDev], pRender->m_strFriendlyName[pRender->m_nSelDev]));

	pPusher->AddRef();
	CHK(pGraph->AddFilter(pPusher, L"Audio Pusher"));

	//pGrabber->SetFormat(*((WAVEFORMATEX*)pAud->m_pFormat[pAud->m_nSelFormat]->pbFormat));

	CHK(ConnectFilters(pGraph, pPusher, pRender->m_pAudCaps[pRender->m_nSelDev]));

	return S_OK;
}

STDMETHODIMP CAudPlay::UnInitGraph()
{
	if (pGraph)
	{
		//RemoveFilter the Filter that inherits from CBaseFilter
//		RemoveDownstream(pGraph, pRender->m_pAudCaps[pRender->m_nSelDev]);
		//Dont RemoveFilter the Filter that filter is provide from windows.
//		pGraph->RemoveFilter(pRender->m_pAudCaps[pRender->m_nSelDev]);

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

STDMETHODIMP CAudPlay::SetFormat(WAVEFORMATEX2 *pFmt, int cbBuffer)
{
	gtrace("%s\n", __FUNCTION__);
	WAVEFORMATEX *pfmt2 = (WAVEFORMATEX*) pFmt;
	if (pPusher == 0)
	{
		HRESULT hr = 0;
		pPusher = new AudPusher(0, &hr, *pfmt2, cbBuffer);
	}
	return S_OK;
}

STDMETHODIMP CAudPlay::AddClip(unsigned char *pBuf, int nSize, int nTimeStamp)
{
	int nIndex;
	Buffer_AddNext(&pBuf_First, g_cs, pBuf, nSize, nTimeStamp, &nIndex);
	//gtrace("[ADD] pBuf:%p nSize:%d nTimeStamp:%d pos:%d\n", pBuf, nSize, nTimeStamp, nIndex);
	return S_OK;
}

UINT WINAPI CAudPlay::RunThd(LPVOID lpVoid)
{
	CAudPlay *pPlay = (CAudPlay *) lpVoid;
	Buffer *pBuf = 0;
	Buffer *pPrevBuf = 0;
	int nPrevTimeStamp = 0;
	REFERENCE_TIME rtStart = 0, rtEnd = 0, rtLastTime = 0;
	gtrace("RunThd Start\n");
	do {
		pBuf = Buffer_GetNext(&pPlay->pBuf_First, pPlay->g_cs);
		if (pBuf != 0)
		{
			if (pPrevBuf == 0)
			{
				pPrevBuf = Buffer_New(pBuf->buf, pBuf->size, pBuf->nTimeStamp);
				nPrevTimeStamp = pBuf->nTimeStamp;
				rtLastTime = 0;
				continue;
			}

			rtStart = rtLastTime;
			rtEnd = rtLastTime + (REFERENCE_TIME)((pBuf->nTimeStamp-pPrevBuf->nTimeStamp)*10000);	//convert ms to us(10^-7) => 1/1,000,000,0 secs
			pPlay->pPusher->Receive(pPrevBuf->buf, pPrevBuf->size, &rtStart, &rtEnd);

			Buffer_Copy(pPrevBuf, pBuf);
			rtLastTime = rtEnd;
		}
		Sleep(10);
	} while(pPlay->nStatus == STATUS_RUN);
	pPlay->pPusher->Receive(0,0,0,0);
	pPlay->m_bThdExit = 1;
	return 0;
}

STDMETHODIMP CAudPlay::Run()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_STOP
		|| nStatus == STATUS_PAUSE)
	{
		gtrace("%s 1\n", __FUNCTION__);
		g_cs.Lock();
		nStatus = STATUS_RUN;
		m_bThdExit = 0;
		hr = pControl->Run();
		m_hRunThread = (HANDLE)::_beginthreadex(0, 0, RunThd, (LPVOID)this, 0, 0);
		g_cs.Unlock();
		gtrace("%s 2\n", __FUNCTION__);
	}
	return hr;
}

STDMETHODIMP CAudPlay::Pause()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_RUN)
	{
		g_cs.Lock();
		nStatus = STATUS_PAUSE;
		hr = pControl->Pause();
		g_cs.Unlock();
		gtrace("%s\n", __FUNCTION__);
	}
	return hr;
}

STDMETHODIMP CAudPlay::Stop()
{
	HRESULT hr = S_OK;
	if (nStatus == STATUS_RUN
		|| nStatus == STATUS_PAUSE)
	{
		gtrace("%s 1\n", __FUNCTION__);
		g_cs.Lock();
		nStatus = STATUS_STOP;
		do {

			gtrace("%s wait for thd exit\n", __FUNCTION__);
			Sleep(50);
		} while(m_bThdExit == 0);
		m_hRunThread = 0;
		hr = pControl->Stop();
		g_cs.Unlock();
		gtrace("%s 2\n", __FUNCTION__);
	}
	return hr;
}