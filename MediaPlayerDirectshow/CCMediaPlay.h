#ifndef _H_MEDIAPLAY_
#define _H_MEDIAPLAY_ 1



HRESULT InitWindowlessVMR(HWND hWnd,IGraphBuilder *pGraph,IVMRWindowlessControl9 **ppWc);


class CCMediaPlayException {
public:
	CCMediaPlayException(const TCHAR *szMsg):_szMsg(szMsg) {;};
	
	const TCHAR *_szMsg;
};

class CCMediaPlay {
public:
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEvent *pEvent;
	IMediaEventEx *pEventEx;
	IMediaSeeking *pSeeking;
	IBasicAudio *pBA;

	IBaseFilter *pVmr;
	IBaseFilter *pAudioRenderer;

	IVMRWindowlessControl9 *pWc;
	
	IVideoWindow *pVideoWindow;
	HWND hWnd;
	RECT rcSrc, rcDest;
	BOOL bCanSeek;
	BOOL bRun;
	
public:
	CCMediaPlay(HWND hWnd);
	~CCMediaPlay();
	
public:
	void PlayFile(const TCHAR *szFile);
	void Redraw(HDC hdc);
	void Resizing();
	void Run();
	void Stop();
	BOOL CanSeek();
	void ProcessedNotify();
	void SetVolume(int nVolume);
	int GetVolume();
	LONGLONG GetDuration();
	
protected:
	void CHECKED(HRESULT hr, const TCHAR *szSection);
	
	
};



#endif