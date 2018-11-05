#if !defined(AFX_CPROGRESS_H__0745B90B_2B99_4E73_8ACE_755E53ED1F60__INCLUDED_)
#define AFX_CPROGRESS_H__0745B90B_2B99_4E73_8ACE_755E53ED1F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCProgress window

class CCProgress : public CProgressCtrl
{
// Construction
public:
	CCProgress();

// Attributes
public:
	bool bLMouseDown;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCProgress)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCProgress();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCProgress)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPROGRESS_H__0745B90B_2B99_4E73_8ACE_755E53ED1F60__INCLUDED_)
