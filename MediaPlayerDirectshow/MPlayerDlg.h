// MPlayerDlg.h : header file
//

#if !defined(AFX_MPLAYERDLG_H__5BA85C7C_4085_4DFF_AA3E_ED3EF972BA82__INCLUDED_)
#define AFX_MPLAYERDLG_H__5BA85C7C_4085_4DFF_AA3E_ED3EF972BA82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "CProgress.h"
class CCMediaPlay;
class CCanvas;


/////////////////////////////////////////////////////////////////////////////
// CMPlayerDlg dialog

//#define IDD_MPLAYER_DIALOG 102




class CMPlayerDlg : public CDialog
{
// Construction
public:
	CMPlayerDlg(CWnd* pParent = NULL);	// standard constructor
	~CMPlayerDlg();

public:
	static BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam);
	bool bInitialRate;
	CRate pRate[7];
	CRate pRate2[7];
	POINT ptDimension[7];
	CCanvas *pCanvas;
	int nPanelIndex;
	//DirectShow
	CString strFileName;
	CCMediaPlay *pPlayer;
	UINT nTimerID;
	void Free();

public:
// Dialog Data
	//{{AFX_DATA(CMPlayerDlg)
	enum { IDD = IDD_MPLAYER_DIALOG };
	CSliderCtrl	mVolume;
	CCProgress mProgress;
	//CProgressCtrl	mProgress;
	CButton	mPlayer;
	CButton	mPause;
	CButton	mLoad;
	CStatic	mDate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlayerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLoad();
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPLAYERDLG_H__5BA85C7C_4085_4DFF_AA3E_ED3EF972BA82__INCLUDED_)
