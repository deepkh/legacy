 
#include "IAudCapture_h.h"

class AudCap;
class AudGrabber;

class CAudCapture: public IAudCapture
{


public:
    CAudCapture();
	virtual ~CAudCapture();

public:
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IAudCapture)
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

	STDMETHODIMP SetCapLatency(int nMillis);
	STDMETHODIMP GetProps(unsigned char *pProps); //ALLOCATOR_PROPERTIES
	STDMETHODIMP GetMediaType(unsigned char *pMediaType);//AM_MEDIA_TYPE

	STDMETHODIMP SetVol(double fVal);
	STDMETHODIMP GetVol(double *pVal);

	STDMETHODIMP SelFmt(int nIndex);
	STDMETHODIMP GetFmtProps(int nIndex, WAVEFORMATEX2 *pFmt);
	STDMETHODIMP GetFmtCount(int *pCount);

	STDMETHODIMP SelInPin(int nIndex);
	STDMETHODIMP GetInPinProps(int nIndex, wchar_t *pPinName);
	STDMETHODIMP GetInPinCount(int *pCount);

	STDMETHODIMP SelDev(int nIndex);
	STDMETHODIMP GetDevProps(int nIndex, wchar_t *pDevName);
	STDMETHODIMP GetDevCount(int *pCount);

	STDMETHODIMP AddReceiver(IAudReceiver *pReceiver, unsigned char *pParam);
	STDMETHODIMP DelReceiver(IAudReceiver *pReceiver);

    STDMETHODIMP InitGraph();
	STDMETHODIMP UnInitGraph();

public:
	STDMETHODIMP Run();
	STDMETHODIMP Pause();
	STDMETHODIMP Stop();

private:
	CCriticalSection g_cs;
	RunStatus nStatus;
	DWORD dwRegister;
	LONG m_lRef;
	AudCap *pAud;
	AudGrabber *pGrabber;
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEvent *pEvent;
};