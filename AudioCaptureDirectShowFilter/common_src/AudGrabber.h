
#include "../IAudCapture/IAudCapture_h.h"
#include <streams.h>
#include <iostream>
#include <vector>

using namespace std;

#ifndef _AUD_GRABBER_
#define _AUD_GRABBER_

extern const GUID IID_AudGrabber;
class AudGrabber;
class AudGrabberInputPin;

class Receiver {
public:
	IAudReceiver *pReceiver;
	void *lpParam;
} ;

class AudGrabberInputPin : public CBaseInputPin
{
public:
	AudGrabberInputPin(AudGrabber *pFilter, HRESULT * phr);
	virtual ~AudGrabberInputPin();

public:
    STDMETHODIMP QueryId(LPWSTR * Id)
    {
        return AMGetWideString(L"In", Id);
    }


	//virtual HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);

	virtual HRESULT Active(void);

    // check that we can support this output type
    HRESULT CheckMediaType(const CMediaType* mtIn);

    // set the connection media type
    HRESULT SetMediaType(const CMediaType* mt);

    // --- IMemInputPin -----

    // here's the next block of data from the stream.
    // AddRef it yourself if you need to hold it beyond the end
    // of this call.
    STDMETHODIMP Receive(IMediaSample * pSample);

    // provide EndOfStream that passes straight downstream
    // (there is no queued data)
    STDMETHODIMP EndOfStream(void);

    // passes it to CTransformFilter::BeginFlush
    STDMETHODIMP BeginFlush(void);

    // passes it to CTransformFilter::EndFlush
    STDMETHODIMP EndFlush(void);

    STDMETHODIMP NewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

public:
    CMediaType& CurrentMediaType() { return m_mt; };
	AudGrabber *m_pFilter;
	int m_nPrevTime;
};

class AudGrabber: public CBaseFilter
{
public:
	AudGrabber();
	virtual ~AudGrabber();

	void SetFormat(WAVEFORMATEX fmt);

public:

	// map getpin/getpincount for base enum of pins to owner
    // override this to return more specialised pin objects

    virtual int GetPinCount();
    virtual CBasePin * GetPin(int n);
    STDMETHODIMP FindPin(LPCWSTR Id, IPin **ppPin);

    // override state changes to allow derived transform filter
    // to control streaming start/stop
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

public:
	void AddReceiver(IAudReceiver *pReceiver, void *lpParam);
	void DelReceiver(IAudReceiver *pReceiver);

public:
    // critical section protecting filter state.
    CCritSec m_csFilter;
	CCritSec m_csReceive;

	AudGrabberInputPin *m_pInPin;

	WAVEFORMATEX m_sFmt;
	
	vector <Receiver *> recvList;
};

#endif