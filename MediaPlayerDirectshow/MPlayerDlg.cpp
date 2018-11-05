// MPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MPlayerDlg.h"
#include "Canvas.h"
#include "CCMediaPlay.h"
#include "CProgress.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayerDlg dialog

CMPlayerDlg::CMPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMPlayerDlg::IDD, pParent), bInitialRate(false),pPlayer(0),pCanvas(0)
{
	//{{AFX_DATA_INIT(CMPlayerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CMPlayerDlg::Free()
{
	if (pPlayer) {
		//pPlayer->Run();
		if (nTimerID)
		{
			KillTimer(nTimerID);
			nTimerID = 0;
		}
		
		delete pPlayer;
		pPlayer = 0;
	}
}

CMPlayerDlg::~CMPlayerDlg()
{
	Free();
}

void CMPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPlayerDlg)
	DDX_Control(pDX, IDC_VOLUME, mVolume);
	DDX_Control(pDX, IDC_PLAY, mPlayer);
	DDX_Control(pDX, IDC_PAUSE, mPause);
	DDX_Control(pDX, IDC_LOAD, mLoad);
	DDX_Control(pDX, IDC_DATE, mDate);
	DDX_Control(pDX, IDC_PROGRESS, mProgress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CMPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayerDlg message handlers

int nIndex = 0;

BOOL CALLBACK CMPlayerDlg::EnumChildProc(HWND hWnd, LPARAM lParam)
{

	CMPlayerDlg *pDlg = (CMPlayerDlg *) lParam;
	CRate *pRate = &pDlg->pRate[nIndex];
	CRate *pRate2 = &pDlg->pRate2[nIndex];
	RECT rect1 = {0}, rect2 = {0}, rect3 = {0};
	POINT pt1 = {0}, pt2 = {0}, pt3 = {0};
	pDlg->GetClientRect(&rect1);
	::GetClientRect(hWnd, &rect2);
	pDlg->ClientToScreen(&pt1);
	::ClientToScreen(hWnd, &pt2);
	

	if (!pDlg->bInitialRate)
	{
		pRate->fRateX = (double)(rect2.right) /  (double)(rect1.right);
		pRate->fRateY = (double)(rect2.bottom) / (double)(rect1.bottom);
		pRate2->fRateX = (double)(pt2.x-pt1.x) / (double)(rect1.right);
		pRate2->fRateY = (double)(pt2.y-pt1.y) / (double)(rect1.bottom);
		pDlg->ptDimension[nIndex].x = rect2.right;
		pDlg->ptDimension[nIndex].y = rect2.bottom;
		TCHAR str[128] = {0};
		::GetWindowText(hWnd, str, 128);
		TRACE(_T("%s %.3f %.3f %.3f %.3f\n"), str, pRate->fRateX, pRate->fRateY, pRate2->fRateX, pRate2->fRateY);
	} else {
		rect3.left =  (int)(pRate2->fRateX * (double)(rect1.right));
		rect3.top =  (int)(pRate2->fRateY * (double)(rect1.bottom));
		rect3.right =  (int)(pRate->fRateX * (double)(rect1.right));
		rect3.bottom = (int)(pRate->fRateY * (double)(rect1.bottom));
		TRACE(_T("%.3f %.3f %.3f %.3f\n"),  pRate->fRateX, pRate->fRateY, pRate2->fRateX, pRate2->fRateY);
		
		if (nIndex == 6)
		{
			rect3.left = 0;
			rect3.top = 0;
			rect3.right = rect1.right;
		//	TRACE(_T("\t\t\t\t\t %d\n"), rect3.right);
		}


		if (pRate->fRateY < 0.1f )
		{
		//	rect3.right = pDlg->ptDimension[nIndex].x;
		//	rect3.bottom = pDlg->ptDimension[nIndex].y;
		}
		::MoveWindow(hWnd, rect3.left, rect3.top, rect3.right, rect3.bottom, TRUE);
		TCHAR str[128] = {0};
		::GetWindowText(hWnd, str, 128);
		//TRACE(_T("%s, %d, %d, %d, %d\n"), str, rect3.left, rect3.top, rect3.right, rect3.bottom);
		//TRACE(_T("%d, %d\n"), rect2.right, rect2.bottom);

		//if (nIndex == 6)
		//	TRACE(_T("\n"));

		pDlg->Invalidate();
		pDlg->UpdateData();
	}



	nIndex++;
	return TRUE;
}

BOOL CMPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	mVolume.SetRange(0, 100);
	mProgress.SetRange32(0, SPLIDER_LEN);
	if (pCanvas == 0)
	{
		RECT rect = {0,0,490,320};
		pCanvas = new CCanvas(this);
		//pCanvas->Create(NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, rect, this, IDC_PANEL);
		pCanvas->CreateEx(
			WS_EX_STATICEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, IDC_PANEL);
		
	}
	

	RECT rect2 = {0};
	GetClientRect(&rect2);
	
	//TRACE(_T("%d, %d, %d, %d\n"), rect2.left, rect2.top, rect2.right, rect2.bottom);
	::EnumChildWindows(GetSafeHwnd(), EnumChildProc, (LPARAM)(void *)this);
	bInitialRate = true;



//	SET_NOSKIN( 0, pCanvas->GetSafeHwnd());
//	dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_DIALOG, NULL, FALSE);
	


//	dsSkinWindow(mLoad.GetSafeHwnd(), SKIN_TYPE_BUTTON, NULL, FALSE);
//	dsSkinWindow(mPlayer.GetSafeHwnd(), SKIN_TYPE_BUTTON, NULL, FALSE);
//	dsSkinWindow(mPause.GetSafeHwnd(), SKIN_TYPE_BUTTON, NULL, FALSE);
//	dsSkinWindow(mProgress.GetSafeHwnd(), SKIN_TYPE_PROGRESS, NULL, FALSE);
//	dsSkinWindow(mVolume.GetSafeHwnd(), SKIN_TYPE_SLIDER, _T("sliderctrl"), FALSE);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
	//	dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMPlayerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	nIndex = 0;
	::EnumChildWindows(GetSafeHwnd(), EnumChildProc, (LPARAM)(void *)this);

	if (pPlayer)
	{
		pPlayer->Resizing();
	}
}

void CMPlayerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CDialog::OnSizing(fwSide, pRect);
	
	nIndex = 0;
	::EnumChildWindows(GetSafeHwnd(), EnumChildProc, (LPARAM)(void *)this);

	if (pPlayer)
	{
	//	HDC hdc = ::GetDC(mPanel.GetSafeHwnd());
		pPlayer->Resizing();
		//pPlayer->Redraw(::GetDC(GetSafeHwnd()));
	//	Invalidate(TRUE);
	}
}

void CMPlayerDlg::OnTimer(UINT nIDEvent) 
{
	
	if (pPlayer && pPlayer->CanSeek())
	{
        REFERENCE_TIME timeNow;
        if (SUCCEEDED(pPlayer->pSeeking->GetCurrentPosition(&timeNow)))
        {
			long sliderTick = (long)((timeNow * SPLIDER_LEN) / pPlayer->GetDuration());
			mProgress.SetPos(sliderTick);

			LONGLONG duration = 0, position = 0;
			int hour, min, sec, hour2, min2, sec2;


			GUID oldTimeFormat, newTimeFormat = TIME_FORMAT_MEDIA_TIME;
			HRESULT hr = pPlayer->pSeeking->GetTimeFormat(&oldTimeFormat);
			if (oldTimeFormat != newTimeFormat)
			{
				pPlayer->pSeeking->GetPositions(&position, NULL);
				pPlayer->pSeeking->GetDuration(&duration);
			} else {
				pPlayer->pSeeking->GetPositions(&position, NULL);
				pPlayer->pSeeking->GetDuration(&duration);
				double tmp = ((double)duration) / UNITS;
				duration = (LONGLONG) tmp;

				tmp = ((double)position) / UNITS;
				position = (LONGLONG) tmp;
			}

			hour = (int)duration/3600;
			min = ((int)duration%3600) / 60;
			sec = ((int)duration%3600) % 60;

			hour2 = (int)position / 3600;
			min2 = ((int)position%3600) / 60;
			sec2 = ((int)position%3600) % 60;

			CString tmp;
			tmp.Format(_T("%02d:%02d:%02d / %02d:%02d:%02d"), hour2, min2, sec2, hour, min, sec);
			mDate.SetWindowText(tmp);


		//	TRACE(_T("%d:%d:%d %d:%d:%d\n"), hour, min, sec, hour2,min2,sec2);
			//pPlayer->pSeeking->SetTimeFormat(&oldTimeFormat);
		}
	}
}



void CMPlayerDlg::OnLoad() 
{
	CString szFilter = _T("Media Type(*.avi;*.mkv;*.wmv;*.asf;*.rmvb;*.mpg;*.mp3)|*.avi;*.mkv;*.wmv;*.asf;*.rmvb;*.mpg;*.mp3|All Type|*.*||");
	CFileDialog fd(TRUE, _T("zzz"), _T(""), OFN_HIDEREADONLY,szFilter,this);


	if (fd.DoModal() == IDOK)
	{
		TRACE(_T("%s\n"), strFileName = fd.GetPathName());
		CString str = strFileName.Right(strFileName.GetLength()-strFileName.ReverseFind(_T('\\'))-1);
		this->SetWindowText(str);
		
		Free();
		
		try {
			pPlayer = new CCMediaPlay(pCanvas->GetSafeHwnd());
			pPlayer->PlayFile(strFileName);
			//mPlayer.SetFocus();
			mProgress.SetFocus();
			mVolume.SetPos(pPlayer->GetVolume());
			//mPlay.EnableWindow(TRUE);
		} catch (CCMediaPlayException e) {
			MessageBox(e._szMsg, 0, MB_ICONERROR);
			delete pPlayer;
			pPlayer = 0;
		}
	}
}

void CMPlayerDlg::OnPlay() 
{
	if (pPlayer)
	{
		pPlayer->Run();
	//	nTimerID = SetTimer(1, 50, NULL);
	//	mProgress.SetFocus();
		//mPause.SetFocus();
	}
}

void CMPlayerDlg::OnPause() 
{
	if (pPlayer)
	{
		pPlayer->Stop();
		//mPlayer.SetFocus();
		mProgress.SetFocus();
		if (nTimerID)
		{
			KillTimer(nTimerID);
			nTimerID = 0;
		}
	}
}


BOOL CMPlayerDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush(GetSysColor(COLOR_BTNFACE)); 
	RECT rect1 = {0}, rect2 = {0};
	GetClientRect(&rect1);
	if (pPlayer == 0)
	{
		return CDialog::OnEraseBkgnd(pDC);
		//pDC->SetStretchBltMode(COLORONCOLOR);
		//pDC->FillRect(&rect1, &brush);
	} else {

		rect1.top = (int)(pRate[6].fRateY * (double)(rect1.bottom));
		pDC->FillRect(&rect1, &brush);
	}
	

	
	//::SendMessage(pCanvas->GetSafeHwnd(), WM_ERASEBKGND, 0, 0);
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CMPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	if (pPlayer)
		pPlayer->SetVolume(mVolume.GetPos());
}

void CMPlayerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CMPlayerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	Free();
	CDialog::OnClose();
}

BOOL CMPlayerDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}
