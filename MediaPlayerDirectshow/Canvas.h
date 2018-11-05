#if !defined(AFX_CANVAS_H__907DAB27_1B5E_4765_A89F_0C24691AD71A__INCLUDED_)
#define AFX_CANVAS_H__907DAB27_1B5E_4765_A89F_0C24691AD71A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Canvas.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCanvas window

class CMPlayerDlg;

class CCanvas : public CStatic
{
// Construction
public:
	CCanvas(CMPlayerDlg *pView);

// Attributes
public:
	CMPlayerDlg *pView;

// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvas)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCanvas();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCanvas)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HRESULT OnMPlayerEvent(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVAS_H__907DAB27_1B5E_4765_A89F_0C24691AD71A__INCLUDED_)
