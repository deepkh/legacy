#include "stdafx.h"
#include "AudPusher.h"


const GUID CLSID_AudPusher =
{ 0x03B60FE0,0x7168,0x44f2,{0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB1,0x05}};

AudPusherOutputPin::AudPusherOutputPin(HRESULT *phr, AudPusher *pFilter, WAVEFORMATEX fmt, int cbBuffer)
:CBaseOutputPin(NAME("Aud Pusher"), pFilter, &pFilter->m_csFilter, phr, L"Audio Out")
,cbBuffer(0)
,pbFormat(0)
{
	
	WAVEFORMATEX *pfmt = (WAVEFORMATEX *) mt.AllocFormatBuffer(sizeof(WAVEFORMATEX));
	ZeroMemory(pfmt, sizeof(WAVEFORMATEX));
	*pfmt = fmt;

	mt.SetType(&MEDIATYPE_Audio);
	mt.SetSubtype(&MEDIASUBTYPE_PCM);
	mt.SetFormatType(&FORMAT_WaveFormatEx);
	mt.SetTemporalCompression(FALSE);
	//mt.SetSampleSize(cbBuffer);
	this->cbBuffer = cbBuffer;
	gtrace("%s cbBuffer:%d, nAvgBytesPerSec:%d\n", __FUNCTION__, cbBuffer, ((WAVEFORMATEX*)mt.Format())->nAvgBytesPerSec);
}

AudPusherOutputPin::~AudPusherOutputPin(void)
{
	gtrace("%s\n", __FUNCTION__);
}


HRESULT AudPusherOutputPin::GetMediaType(int iPosition, CMediaType *pmt)
{
	CheckPointer(pmt,E_POINTER);
	//CAutoLock lock(m_pFilter->pStateLock());

	gtrace("%s\n", __FUNCTION__);
	if (iPosition == 0)
	{
		*pmt = mt;
		return NOERROR;
	} else {
		if(iPosition < 0)
			return E_INVALIDARG;

		if (iPosition > 0)
			return VFW_S_NO_MORE_ITEMS;
	}
}

HRESULT AudPusherOutputPin::CheckMediaType(const CMediaType *pmt)
{
	CheckPointer(pmt,E_POINTER);
	//CAutoLock lock(m_pFilter->pStateLock());

	gtrace("%s\n", __FUNCTION__);
	if (*pmt == mt)
		return NOERROR;
	else	
		return E_INVALIDARG;


	if (*pmt == mt)
	{
		return NOERROR;
	} else {
		return E_INVALIDARG;
	}
}

HRESULT AudPusherOutputPin::DecideBufferSize(IMemAllocator *pAllocator, ALLOCATOR_PROPERTIES *pProp)
{
	CheckPointer(pAllocator,E_POINTER);
	CheckPointer(pProp,E_POINTER);
	//CAutoLock lock(m_pFilter->pStateLock());

	WAVEFORMATEX *pmt = (WAVEFORMATEX*) mt.Format();

	gtrace("%s\n", __FUNCTION__);

	pProp->cbBuffer = this->cbBuffer;	
		//pmt->nAvgBytesPerSec * pmt->wBitsPerSample / 8;

    if (pProp->cbAlign == 0)
    {
        pProp->cbAlign = 1;
    }
    if (pProp->cBuffers == 0)
    {
        pProp->cBuffers = 1;
    }

	gtrace("cBuffers:%d\n", pProp->cBuffers);
	gtrace("cbBuffer:%d\n", pProp->cbBuffer);
	gtrace("cbAlign:%d\n", pProp->cbAlign);
	gtrace("cbPrefix:%d\n", pProp->cbPrefix);

	ASSERT(pProp->cbBuffer);

	ALLOCATOR_PROPERTIES Actual;
	HRESULT hr = pAllocator->SetProperties(pProp, &Actual);
	if (FAILED(hr)) {
		CHK(-1);		
		return hr;
	}

	if (Actual.cBuffers < pProp->cBuffers ||
		Actual.cbBuffer < pProp->cbBuffer) {
		CHK(-1);
		return E_FAIL;
	}
	return NOERROR;
}

HRESULT AudPusherOutputPin::Active(void)
{
	gtrace("%s\n",__FUNCTION__);
	return CBaseOutputPin::Active();
}


/************************************************************************/
/* AudPusher															*/
/************************************************************************/

AudPusher::AudPusher(IUnknown *pUnk, HRESULT *phr, WAVEFORMATEX fmt, int cbBuffer)
:CBaseFilter(NAME("CPushSource"), /*pUnk*/ 0,&m_csFilter, CLSID_AudPusher)
,m_pOutPin(0)
,nPrevTime(0)
,nTotalPlayMs(0)
{
	HRESULT hr;
	this->cbBuffer = cbBuffer;
	m_pOutPin = new AudPusherOutputPin(&hr, this, fmt, cbBuffer);
	gtrace("%s\n", __FUNCTION__);
}


AudPusher::~AudPusher()
{
	gtrace("%s\n", __FUNCTION__);
	delete m_pOutPin;
}

int AudPusher::GetPinCount()
{
	gtrace("%s\n",__FUNCTION__);
    return 1;
}

CBasePin *AudPusher::GetPin(int n)
{
    HRESULT hr = S_OK;
	
	if (n == 0) {
		gtrace("%s(%d)\n",__FUNCTION__, n);
		return m_pOutPin;
	} else {
		gtrace("%s %d NO PIN\n",__FUNCTION__, n);
		return NULL;
	}
}

STDMETHODIMP AudPusher::Stop()
{
	CBaseFilter::Stop();
	gtrace("%s\n",__FUNCTION__);
	return NOERROR;
}

STDMETHODIMP AudPusher::Pause()
{
	CBaseFilter::Pause();
	gtrace("%s\n",__FUNCTION__);
	return NOERROR;
}

void AudPusher::Receive(unsigned char *pBuf, int nSize, REFERENCE_TIME *rtStart, REFERENCE_TIME *rtEnd)
{
	if (pBuf == 0)
	{
		gtrace("%s DeliverEndOfStream()\n", __FUNCTION__);
		m_pOutPin->DeliverEndOfStream();
		return;
	}

	IMediaSample *pSample = 0;
	do {
	    HRESULT hr = m_pOutPin->GetDeliveryBuffer(&pSample,NULL,NULL,0);
		// go round again. Perhaps the error will go away
		// or the allocator is decommited & we will be asked to
		// exit soon.
		//gtrace("wait for get sample.\n");
		if (FAILED(hr)) {
			Sleep(1);
			continue;
		}
		
		break;
	} while(TRUE);

	BYTE *pb = 0;
	pSample->GetPointer(&pb);
	memcpy(pb, pBuf, nSize);
	pSample->SetActualDataLength(nSize);
	pSample->SetSyncPoint(TRUE);
	int curTime = gettimeofms();
	if (nPrevTime == 0)
		nPrevTime = curTime;
	nTotalPlayMs +=   (curTime-nPrevTime);
	//gtrace("srt:%lld, end:%lld, dur:%lld, ms:%d pts:%.2f pb:%02X %02X %02X %02X\n", *rtStart, *rtEnd, *rtEnd-*rtStart, curTime-nPrevTime, nTotalPlayMs/1000.0f, pb[0], pb[1], pb[2], pb[3]);
	nPrevTime = curTime;
	pSample->SetTime(rtStart, rtEnd);
	m_pOutPin->Deliver(pSample);
	pSample->Release();
}