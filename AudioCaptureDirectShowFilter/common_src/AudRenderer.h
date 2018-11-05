#ifndef _AUD_RENDERER_
#define _AUD_RENDERER_

#include <dshow.h>
#include <Mtype.h>
#include <streams.h>

#ifndef MAX_DV
#define MAX_DEV 20
#define MAX_PIN 10
#define MAX_FMT 50
#endif

class AudRender {
public:
	AudRender();
	~AudRender();

public:
	void Reset();
	
	HRESULT ListPins();
	
	HRESULT SelDev(int nIndex);
	HRESULT ListDevices();

public:
	int m_nDev;
	IBaseFilter *m_pAudCaps[MAX_DEV];
	WCHAR m_strFriendlyName[MAX_DEV][64];
	IPin *m_pInputPin;
	WCHAR m_strInPinName[64];
	int m_nSelDev;
};

#endif
