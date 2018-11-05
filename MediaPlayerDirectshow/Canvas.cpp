// Canvas.cpp : implementation file
//

#include "stdafx.h"
#include "Canvas.h"
#include "CCMediaPlay.h"
#include "MPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvas

CCanvas::CCanvas(CMPlayerDlg *pView)
:pView(pView)
{
	
}

CCanvas::~CCanvas()
{
}


BEGIN_MESSAGE_MAP(CCanvas, CStatic)
//{{AFX_MSG_MAP(CCanvas)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_MESSAGE(WM_GRAPHNOTIFY, OnMPlayerEvent)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvas message handlers

void CCanvas::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	if (pView->pPlayer)
	{
		pView->pPlayer->Redraw(dc.GetSafeHdc());
	}
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CCanvas::PreCreateWindow(CREATESTRUCT& cs) 
{
	
	return CStatic::PreCreateWindow(cs);
}

BOOL CCanvas::OnEraseBkgnd(CDC* pDC) 
{
	
	return CStatic::OnEraseBkgnd(pDC);
}

HRESULT CCanvas::OnMPlayerEvent(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("NOTIFY\n"));
	if (pView->pPlayer)
	{
		pView->pPlayer->ProcessedNotify();
	}

	return 1;
}