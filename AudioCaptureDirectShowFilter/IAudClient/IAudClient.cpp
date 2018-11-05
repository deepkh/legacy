// IAudClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../IAudCapture/IAudCapture_h.h"
#include "../IAudPlay/IAudPlay_h.h"
#include "../common_src/audiodetect.h"
#include <windows.h>
#include <objbase.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <crtdbg.h>

#include <strmif.h>


class CAudReceiver: public IAudReceiver {
public:
	IAudPlay *pPlay;
	LONG m_lRef;
	AudioDetect *pDetect;
	__int64 nAvg;
	int nCount;
	int nMax;
	int nMin;
	int nDevAry[18];
	int nAvgAry[20];		//1 secs
	int nCount2;
	CAudReceiver(IAudPlay *p, int nShift):pPlay(p),m_lRef(0)
	,nAvg(0), nCount(0), nMin(9999), nMax(-9999),nCount2(0)
	{
		pDetect = new AudioDetect(nShift);	
	};
	~CAudReceiver()
	{
		delete pDetect;
	};
public:
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IAudReceiver)
        {
            *ppv = (IAudCapture *) this;
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

	STDMETHODIMP Clip(unsigned char *pBuf, int nSize, int nTimeStamp, unsigned char *pParam)
	{
		if (pPlay)
		{
			for (int i=0; i<nSize; i++)
			{
				if (pBuf[i] < nMin)
					nMin = pBuf[i];
				if (pBuf[i] > nMax)
					nMax = pBuf[i];
				nAvg += pBuf[i];
			}

			

			nCount += nSize;
			int nAvg2 = nAvg/nCount;
			int nDet = pDetect->Detect(pBuf, nSize);
			int def = nDet - nAvg2;
			int len = sizeof(nDevAry)/sizeof(nDevAry[0]);
			nDevAry[nCount2%len] = def;
			int bDet = 0;
			for (int j=0; (j+3)<len;j+=3)
			{
				/*
				if (abs(nDevAry[j])
					&& abs(nDevAry[j+1])
					&& abs(nDevAry[j+2]))*/
				if ((abs(nDevAry[j])
					+ abs(nDevAry[j+1])
					+ abs(nDevAry[j+2])) >= 1)
				{
					printf("def(%d~%d):%d, %d, %d\n", j, j+2, nDevAry[j], nDevAry[j+1], nDevAry[j+2]);
					bDet = 1;
					break;
				}
				
			}
			if (bDet)
			{
				gtrace("nAvg:%d, detect:%d, def:%d\n", nAvg2, nDet, def);
				pPlay->AddClip(pBuf, nSize, nTimeStamp);
			}
			nCount2++;
			//gtrace("%s size:%d pts:%d\n", __FUNCTION__, nSize, nTimeStamp);
		}
		return S_OK;
	};
};

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "cht");
	CoInitialize(0);
	IAudCapture *pCap;
	int i;
		
	CHK(CoCreateInstance(CLSID_AudCapture,
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_IAudCapture,
							(void **)&pCap));

	int nCapLatency = 50; //ms

	/**
		Initial IAudioCapture
	*/
	int nFmtCount = 0;
	int nDevCount = 0;
	int nInPinCount = 0;
	wchar_t szDevName[64] = {0};
	wchar_t szInPinName[64] = {0};
	WAVEFORMATEX2 sFmt = {0};

	int nSelDev = -1;
	int nSelInPin = -1;
	int nSelFmt = -1;
	CHK(pCap->GetDevCount(&nDevCount));
	for (i = 0; i<nDevCount; i++)
	{
		memset(szDevName, 0, sizeof(szDevName));
		CHK(pCap->GetDevProps(i, szDevName));
		gtrace(L"%d: %s\n", i, szDevName);
	}

	gtrace("sel:");
	scanf("%d", &nSelDev);
	CHK(pCap->SelDev(nSelDev));
	CHK(pCap->GetInPinCount(&nInPinCount));
	for (i=0; i<nInPinCount; i++)
	{
		memset(szInPinName, 0, sizeof(szInPinName));
		CHK(pCap->GetInPinProps(i, szInPinName));
		gtrace(L"%d:%s\n", i, szInPinName);
	}

	gtrace("sel:");
	scanf("%d", &nSelInPin);
	CHK(pCap->SelInPin(nSelInPin));
	CHK(pCap->GetFmtCount(&nFmtCount));
	for (i=0; i<nFmtCount; i++)
	{
		memset(&sFmt, 0, sizeof(sFmt));
		CHK(pCap->GetFmtProps(i, &sFmt));
		gtrace(L"%d:%s\n", i, szInPinName);

		WAVEFORMATEX *p = (WAVEFORMATEX *) &sFmt;
		gtrace(_T("wFormatTag: %d\n"), p->wFormatTag);
		gtrace(_T("nChannel: %d\n"), p->nChannels);
		gtrace(_T("nSamplePerSec: %d\n"), p->nSamplesPerSec);
		gtrace(_T("nAvgBytesPerSec: %d\n"), p->nAvgBytesPerSec);
		gtrace(_T("nBlockAlign: %d\n"), p->nBlockAlign);
		gtrace(_T("wBitsPerSample: %d\n"), p->wBitsPerSample);
		gtrace(_T("cbSize: %d\n\n"), p->cbSize);
	}

	gtrace("sel:");
	scanf("%d", &nSelFmt);
	CHK(pCap->SelFmt(nSelFmt));
	CHK(pCap->SetVol(0.5));
	CHK(pCap->SetCapLatency(nCapLatency));
	CHK(pCap->InitGraph());

	AM_MEDIA_TYPE amType = {0};
	ALLOCATOR_PROPERTIES props = {0};
	CHK(pCap->GetMediaType((unsigned char *)&amType));
	CHK(pCap->GetProps((unsigned char *)&props));

	printf("\n\nchoose\n");
	WAVEFORMATEX *p = (WAVEFORMATEX *) amType.pbFormat;
		gtrace(_T("wFormatTag: %d\n"), p->wFormatTag);
		gtrace(_T("nChannel: %d\n"), p->nChannels);
		gtrace(_T("nSamplePerSec: %d\n"), p->nSamplesPerSec);
		gtrace(_T("nAvgBytesPerSec: %d\n"), p->nAvgBytesPerSec);
		gtrace(_T("nBlockAlign: %d\n"), p->nBlockAlign);
		gtrace(_T("wBitsPerSample: %d\n"), p->wBitsPerSample);
		gtrace(_T("cbSize: %d\n\n"), p->cbSize);
	gtrace("\n");
	gtrace("cbBuffer:%d\n", props.cbBuffer);





	
	IAudPlay *pPlay = 0;
	int nRenderCount = 0;
	wchar_t szRenderDevName[256] = {0};

	int nSelRenderDev = -1;
	CHK(CoCreateInstance(CLSID_AudPlay,
                                        NULL,
                                        CLSCTX_INPROC_SERVER,
                                        IID_IAudPlay,
                                        (void **)&pPlay));
	
	CHK(pPlay->GetDevCount(&nRenderCount));
	
	gtrace("\nchoose audio render:\n");
	for (i=0; i<nRenderCount; i++)
	{
		memset(szRenderDevName, 0, sizeof(szRenderDevName));
		CHK(pPlay->GetDevProps(i, szRenderDevName));
		gtrace(_T("%d %s\n"), i, szRenderDevName);
	}
	scanf("%d", &nSelRenderDev);
	CHK(pPlay->SelDev(nSelRenderDev));
	CHK(pPlay->SetFormat((WAVEFORMATEX2 *) amType.pbFormat, props.cbBuffer));
	CHK(pPlay->InitGraph());
	//unsigned char *buf = new unsigned char[props.cbBuffer];
	//for (int j=0; j<props.cbBuffer; j++)
	//	buf[j] = j%256;
	//int ms = gettimeofms();
	//for (int i=0; i<(100*1000)/nCapLatency; i++)
	//	pPlay->AddClip(buf, props.cbBuffer, ms+i*nCapLatency);
	//delete [] buf;


	// Start Cap
	system("pause");
	CAudReceiver *pReceiver = new CAudReceiver(pPlay, 100);
	CHK(pCap->AddReceiver(pReceiver, 0));
	CHK(pCap->Run());


	//Start Play
	CHK(pPlay->Run());


	system("pause");
	//Stop Cap
	CHK(pCap->Pause());
	CHK(pCap->DelReceiver(pReceiver));
	CHK(pCap->Stop());
	CHK(pCap->UnInitGraph());
	CHK(pCap->Release());

	gtrace("nMax:%d, nMin:%d, nAvg:%lld\n", pReceiver->nMax, pReceiver->nMin, pReceiver->nAvg/pReceiver->nCount);

	system("pause");
	delete pReceiver;

	//Stop Play
	CHK(pPlay->Stop());
	CHK(pPlay->UnInitGraph());
	CHK(pPlay->Release());

	
	CoUninitialize();
	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}

