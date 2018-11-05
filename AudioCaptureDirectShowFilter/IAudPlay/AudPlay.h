#include "IAudPlay_h.h"
#include "../common_src/LinkedListBuffer.h"
#include <windows.h>

class AudRender;
class AudPusher;

class CAudPlay: public IAudPlay
{
private:
    
public:
    CAudPlay();
    virtual ~CAudPlay();

public:
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IAudPlay)
        {
            *ppv = (IAudPlay *) this;
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

	STDMETHODIMP SelDev(int nIndex);
	STDMETHODIMP GetDevProps(int nIndex, wchar_t *pDevName);
	STDMETHODIMP GetDevCount(int *pCount);
    STDMETHODIMP InitGraph();
	STDMETHODIMP UnInitGraph();


	STDMETHODIMP SetFormat(WAVEFORMATEX2 *pFmt, int cbBuffer);
	STDMETHODIMP AddClip(unsigned char *pBuf, int nSize, int nTimeStamp);
	STDMETHODIMP Run();
	STDMETHODIMP Pause();
	STDMETHODIMP Stop();

public:
	static UINT WINAPI RunThd(LPVOID lpVoid);

public:
	CCriticalSection g_cs;
	Buffer *pBuf_First;
	HANDLE	m_hRunThread;
	int m_bThdExit;

public:
	RunStatus nStatus;
	DWORD dwRegister;
	LONG m_lRef;
	AudRender *pRender;
	AudPusher *pPusher;
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEvent *pEvent;
};
