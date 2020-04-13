// GoView.h : interface of the CGoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GOVIEW_H__1718AD69_2377_412E_8377_97E06D307306__INCLUDED_)
#define AFX_GOVIEW_H__1718AD69_2377_412E_8377_97E06D307306__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"

class CGoView : public CView
{
protected: // create from serialization only
	CGoView();
	DECLARE_DYNCREATE(CGoView)

// Attributes
public:
	CGoDoc* GetDocument();
	void DrawStone(int d, CDC *pWhiteStoneDC, CDC *pBlackStoneDC);

// Operations
public:

	int boardwidth,d;
	int blackplayer;
	Function game;
	COLORREF boardcolor;
	BOOL GameSetup;

	CRect rcWindow,rcBoard;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGoView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGamePass();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GoView.cpp
inline CGoDoc* CGoView::GetDocument()
   { return (CGoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOVIEW_H__1718AD69_2377_412E_8377_97E06D307306__INCLUDED_)
