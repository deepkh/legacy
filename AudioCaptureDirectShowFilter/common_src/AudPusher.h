
#include <streams.h>

#ifndef _AUD_PUSHER_
#define _AUD_PUSHER_

extern const GUID CLSID_AudPusher;
class AudPusher;
class AudPusherOutputPin;

class AudPusherOutputPin :  public CBaseOutputPin {
protected:
	CMediaType mt;
	WAVEFORMATEX *pbFormat;
	int cbBuffer;

public:
	AudPusherOutputPin(HRESULT *phr, AudPusher *pFilter, WAVEFORMATEX fmt, int cbBuffer);
	~AudPusherOutputPin(void);

public:
	virtual HRESULT Active(void);
	HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);
	HRESULT CheckMediaType(const CMediaType *pMediaType);
	HRESULT DecideBufferSize(IMemAllocator *pAllocator, ALLOCATOR_PROPERTIES *pProp);
	
	STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q)
	{
		return E_FAIL;
	};
};

class AudPusher : public CBaseFilter {
public:
	CCritSec m_csFilter;
	int cbBuffer;
	AudPusherOutputPin *m_pOutPin;
	int nPrevTime;
	int nTotalPlayMs;

public:
	AudPusher(IUnknown *pUnk, HRESULT *phr, WAVEFORMATEX fmt, int cbBuffer);
	virtual ~AudPusher();

public:
    virtual int GetPinCount();
    virtual CBasePin * GetPin(int n);
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

public:
	void Receive(unsigned char *pBuf, int nSize, REFERENCE_TIME *rtStart, REFERENCE_TIME *rtEnd);
};


#endif
