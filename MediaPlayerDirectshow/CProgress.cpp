// CProgress.cpp : implementation file
//

#include "stdafx.h"
#include "MPlayer.h"
#include "CProgress.h"
#include "MPlayerDlg.h"
#include "CCMediaPlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCProgress

CCProgress::CCProgress()
:bLMouseDown(false)
{
}

CCProgress::~CCProgress()
{
}


BEGIN_MESSAGE_MAP(CCProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CCProgress)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCProgress message handlers

void CCProgress::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMPlayerDlg *pDlg = (CMPlayerDlg *) GetParent();
	if (pDlg->pPlayer)
	{
		pDlg->KillTimer(pDlg->nTimerID);
		RECT rect1 = {0};
		GetClientRect(&rect1);
		
		int nNewPos = ((double)point.x / (double)rect1.right)*SPLIDER_LEN;
		SetPos(nNewPos);
		REFERENCE_TIME newTime = (pDlg->pPlayer->GetDuration()/SPLIDER_LEN) * nNewPos;
		pDlg->pPlayer->pSeeking->SetPositions(&newTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
		
		bLMouseDown = true;
		TRACE(_T("%d %d %d\n"), point.x, point.y, nNewPos);

		pDlg->nTimerID = pDlg->SetTimer(1, 100, NULL);
	}

	CProgressCtrl::OnLButtonDown(nFlags, point);
}

void CCProgress::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMPlayerDlg *pDlg = (CMPlayerDlg *) GetParent();
	if (bLMouseDown)
	{	
		pDlg->KillTimer(pDlg->nTimerID);
		RECT rect1 = {0};
		GetClientRect(&rect1);
		
		int nNewPos = ((double)point.x / (double)rect1.right)*SPLIDER_LEN;
		SetPos(nNewPos);
		REFERENCE_TIME newTime = (pDlg->pPlayer->GetDuration()/SPLIDER_LEN) * nNewPos;
		pDlg->pPlayer->pSeeking->SetPositions(&newTime, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);
		
		bLMouseDown = true;
		TRACE(_T("%d %d %d\n"), point.x, point.y, nNewPos);

		pDlg->nTimerID = pDlg->SetTimer(1, 100, NULL);
	}
	CProgressCtrl::OnMouseMove(nFlags, point);
}

void CCProgress::OnLButtonUp(UINT nFlags, CPoint point) 
{


	bLMouseDown = false;
	CProgressCtrl::OnLButtonUp(nFlags, point);
}
