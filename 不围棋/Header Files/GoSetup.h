#if !defined(AFX_GOSETUP_H__639B2D11_7083_4735_B5A9_05DFD38116EE__INCLUDED_)
#define AFX_GOSETUP_H__639B2D11_7083_4735_B5A9_05DFD38116EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GoSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGoSetup dialog

class CGoSetup : public CDialog
{
// Construction
public:
	CGoSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGoSetup)
	enum { IDD = IDD_SETUP_DIALOG };
	int		m_boardsize;
	int		m_black;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoSetup)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGoSetup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOSETUP_H__639B2D11_7083_4735_B5A9_05DFD38116EE__INCLUDED_)
