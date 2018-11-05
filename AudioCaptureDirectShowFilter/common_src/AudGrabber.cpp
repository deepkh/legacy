#include "stdafx.h"
#include "AudGrabber.h"

const GUID CLSID_AudGrabber  =
{ 0x03B60FE0,0x7168,0x44f2,{0x8D,0x22,0xB7,0x98,0x9C,0xFF,0xB0,0x05}};


AudGrabberInputPin::AudGrabberInputPin(AudGrabber *pFilter, HRESULT *phr)
:CBaseInputPin(NAME("Aud Grabber Input Pin"), pFilter, &pFilter->m_csFilter, phr, L"XForm In")
,m_pFilter(pFilter)
{
	m_nPrevTime = 0;
	gtrace("%s\n",__FUNCTION__);
}

AudGrabberInputPin::~AudGrabberInputPin()
{
	gtrace("%s\n",__FUNCTION__);
}

//HRESULT AudGrabberInputPin::GetMediaType(int iPosition, CMediaType *pMediaType)
//{
//
//}

HRESULT AudGrabberInputPin::CheckMediaType(const CMediaType* pmt)
{
	gtrace("%s\n",__FUNCTION__);
	if (pmt->majortype == MEDIATYPE_Audio &&
		//pmt->subtype == MEDIASUBTYPE_PCM &&
		pmt->formattype == FORMAT_WaveFormatEx &&
		pmt->cbFormat == sizeof(WAVEFORMATEX))
	{
		WAVEFORMATEX *p = (WAVEFORMATEX*) pmt->pbFormat;
		if (p->nChannels == m_pFilter->m_sFmt.nChannels						//1
			&& p->wBitsPerSample == m_pFilter->m_sFmt.wBitsPerSample		//8bits
			&& p->nSamplesPerSec == m_pFilter->m_sFmt.nSamplesPerSec )		//8000
			return S_OK;
	}

	return VFW_E_TYPE_NOT_ACCEPTED;

    //HRESULT hr = m_pTransformFilter->CheckInputType(pmt);
    //if (S_OK != hr) {
    //    return hr;
    //}
	
    //if ((m_pTransformFilter->m_pOutput != NULL) &&
    //    (m_pTransformFilter->m_pOutput->IsConnected())) {
    //        return m_pTransformFilter->CheckTransform(
    //                  pmt,
		  //    &m_pTransformFilter->m_pOutput->CurrentMediaType());
    //} else {
    //    return hr;
    //}
}


HRESULT AudGrabberInputPin::SetMediaType(const CMediaType* mtIn)
{
    HRESULT hr = CBasePin::SetMediaType(mtIn);
	gtrace("%s\n",__FUNCTION__);
    if (FAILED(hr)) {
        return hr;
    }
	return hr;

    //ASSERT(SUCCEEDED(m_pTransformFilter->CheckInputType(mtIn)));

    //return m_pTransformFilter->SetMediaType(PINDIR_INPUT,mtIn);
}


STDMETHODIMP AudGrabberInputPin::EndOfStream(void)
{
    CAutoLock lck(&m_pFilter->m_csReceive);
	gtrace("%s\n",__FUNCTION__);
    //HRESULT hr = CheckStreaming();
    //if (S_OK == hr) {
    //   hr = m_pTransformFilter->EndOfStream();
    //}
    //return hr;
	return NOERROR;
}


STDMETHODIMP AudGrabberInputPin::BeginFlush(void)
{
    CAutoLock lck(&m_pFilter->m_csFilter);
	gtrace("%s\n",__FUNCTION__);
    //ASSERT(m_pTransformFilter->m_pOutput != NULL);
    //if (!IsConnected() ||
    //    !m_pTransformFilter->m_pOutput->IsConnected()) {
    //    return VFW_E_NOT_CONNECTED;
    //}
    HRESULT hr = CBaseInputPin::BeginFlush();
    if (FAILED(hr)) {
    	return hr;
    }

	return hr;
    //return m_pTransformFilter->BeginFlush();
}


STDMETHODIMP AudGrabberInputPin::EndFlush(void)
{
    CAutoLock lck(&m_pFilter->m_csFilter);
	gtrace("%s\n",__FUNCTION__);
    //ASSERT(m_pTransformFilter->m_pOutput != NULL);
    //if (!IsConnected() ||
    //    !m_pTransformFilter->m_pOutput->IsConnected()) {
    //    return VFW_E_NOT_CONNECTED;
    //}

    //HRESULT hr = m_pTransformFilter->EndFlush();
    //if (FAILED(hr)) {
    //    return hr;
    //}
    return CBaseInputPin::EndFlush();
}


HRESULT AudGrabberInputPin::Active(void)
{
	gtrace("%s\n",__FUNCTION__);
	m_nPrevTime = gettimeofms();
	return CBaseInputPin::Active();
}


HRESULT AudGrabberInputPin::Receive(IMediaSample * pSample)
{
	int ms = gettimeofms();
	//gtrace("%s %dms %dbytes\n",__FUNCTION__, ms-m_nPrevTime, pSample->GetSize());
    HRESULT hr;
    CAutoLock lck(&m_pFilter->m_csReceive);
    ASSERT(pSample);

    hr = CBaseInputPin::Receive(pSample);
    if (S_OK == hr) {
		vector <Receiver *>::iterator iter;
		for (iter=m_pFilter->recvList.begin(); iter != m_pFilter->recvList.end(); ++iter)
		{
			Receiver *p = *iter;
			BYTE *pBuf = 0;
			pSample->GetPointer(&pBuf);

			p->pReceiver->Clip(pBuf, pSample->GetSize(), ms, (unsigned char *)p->lpParam);
		}
        //hr = m_pTransformFilter->Receive(pSample);
    }
	
	m_nPrevTime = ms;
    return hr;
}


STDMETHODIMP AudGrabberInputPin::NewSegment(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop,
    double dRate)
{
	
	gtrace("%s\n",__FUNCTION__);
    return CBasePin::NewSegment(tStart, tStop, dRate);
    //return m_pTransformFilter->NewSegment(tStart, tStop, dRate);
}


/*************************************************
	FILTER
*************************************************/
AudGrabber::AudGrabber()
:CBaseFilter(NAME("AudGrabber"), /*pUnk*/ 0,&m_csFilter, CLSID_AudGrabber)
,m_pInPin(0)
{
	gtrace(_T("AudGrabber::AudGrabber()\n"));
}

AudGrabber::~AudGrabber()
{
	int i;
	vector <Receiver *>::iterator iter;
	for (iter = recvList.begin(), i=0; iter != recvList.end(); ++iter, i++)
	{
		gtrace("%s erase data at %d addr:%p\n", __FUNCTION__, i, *iter);
		recvList.erase(iter);
		delete *iter;
		break;
	}
	delete m_pInPin;
}

void AudGrabber::SetFormat(WAVEFORMATEX fmt)
{
	this->m_sFmt = fmt;
}

int AudGrabber::GetPinCount()
{
	gtrace("%s\n",__FUNCTION__);
    return 1;
}

CBasePin *AudGrabber::GetPin(int n)
{
    HRESULT hr = S_OK;
	
	if (m_pInPin == 0)
	{
		m_pInPin = new AudGrabberInputPin(this, &hr);

		//  Can't fail
        ASSERT(SUCCEEDED(hr));
        if (m_pInPin == NULL) {
			gtrace("%s %d NO PIN\n",__FUNCTION__, n, m_pInPin);
            return NULL;
        }
	}

	if (n == 0) {
		gtrace("%s %d %p\n",__FUNCTION__, n, m_pInPin);
		return m_pInPin;
	} else {
		gtrace("%s %d NO PIN\n",__FUNCTION__, n, m_pInPin);
		return NULL;
	}
}

STDMETHODIMP AudGrabber::Stop()
{
	CBaseFilter::Stop();
	gtrace("%s\n",__FUNCTION__);
	return NOERROR;
}

STDMETHODIMP AudGrabber::Pause()
{
	CBaseFilter::Pause();
	gtrace("%s\n",__FUNCTION__);
	return NOERROR;
}


STDMETHODIMP AudGrabber::FindPin(LPCWSTR Id, IPin **ppPin)
{
	gtrace("%s\n",__FUNCTION__);

    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(IPin *));

    if (0==lstrcmpW(Id,L"In")) {
        *ppPin = GetPin(0);
    /*} else if (0==lstrcmpW(Id,L"Out")) {
        *ppPin = GetPin(1);*/
    } else {
        *ppPin = NULL;
        return VFW_E_NOT_FOUND;
    }

    HRESULT hr = NOERROR;
    //  AddRef() returned pointer - but GetPin could fail if memory is low.
    if (*ppPin) {
        (*ppPin)->AddRef();
    } else {
        hr = E_OUTOFMEMORY;  // probably.  There's no pin anyway.
    }
    return hr;

}

void AudGrabber::AddReceiver(IAudReceiver *pReceiver, void *lpParam)
{
	CAutoLock lck(&m_csReceive);
	Receiver *p = new Receiver;
	p->pReceiver = pReceiver;
	p->lpParam = lpParam;
	recvList.push_back(p);
	gtrace("%s addr: %p\n", __FUNCTION__, pReceiver);
}

void AudGrabber::DelReceiver(IAudReceiver *pReceiver)
{
	CAutoLock lck(&m_csReceive);
	vector <Receiver *>::iterator iter;
	int i;
	for (iter = recvList.begin(), i=0; iter != recvList.end(); ++iter, i++)
	{
		if ((*iter)->pReceiver == pReceiver)
		{
			gtrace("%s erase data at %d addr:%p\n", __FUNCTION__, i, *iter);
			delete (*iter);
			recvList.erase(iter);
			break;
		}
	}
}