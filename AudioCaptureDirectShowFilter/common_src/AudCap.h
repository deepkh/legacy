#ifndef _AUD_CAP_
#define _AUD_CAP_

#include <dshow.h>
#include <Mtype.h>
#include <streams.h>

#ifndef MAX_DV
#define MAX_DEV 20
#define MAX_PIN 10
#define MAX_FMT 50
#endif

class AudCap {
public:
	AudCap();
	~AudCap();

public:
	void Reset();

	HRESULT SetCapLatency(int nMillis);
	HRESULT GetProps(ALLOCATOR_PROPERTIES *pProps);
	HRESULT GetMediaType(AM_MEDIA_TYPE *pMediaType);

	HRESULT SelFormat(int nIndex);
	HRESULT ListFormat();
	
	HRESULT GetVol(double *pVal);
	HRESULT SetVol(double fVal);
	
	HRESULT SelPin(int nIndex);
	HRESULT ListPins();
	
	HRESULT SelDev(int nIndex);
	HRESULT ListDevices();
	void SelCapDev(int nIndex);


public:
	int m_nDev;
	int m_nInPin;
	int m_nFormat;
	IBaseFilter *m_pAudCaps[MAX_DEV];
	WCHAR m_strFriendlyName[MAX_DEV][64];
	IPin *m_pInputPin[MAX_PIN];
	WCHAR m_strInPinName[MAX_PIN][64];
	IPin *m_pOutputPin;
	IAMAudioInputMixer *m_pMixer;
	IAMAudioInputMixer *m_pMainMixer;
	IAMStreamConfig *m_pConfig;
	AM_MEDIA_TYPE	*m_pFormat[MAX_FMT];
	int m_nSelDev;
	int m_nSelInput;
	int	m_nSelFormat;
};

#endif
