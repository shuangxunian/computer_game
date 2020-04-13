// GoSetup.cpp : implementation file
//

#include "stdafx.h"
#include "go.h"
#include "GoSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGoSetup dialog


CGoSetup::CGoSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CGoSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGoSetup)
	m_boardsize = 0;
	m_black = 0;
	//}}AFX_DATA_INIT
}


void CGoSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGoSetup)
	DDX_CBIndex(pDX, IDC_COMBO_BOARDSIZE, m_boardsize);
	DDX_CBIndex(pDX, IDC_COMBO2, m_black);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGoSetup, CDialog)
	//{{AFX_MSG_MAP(CGoSetup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoSetup message handlers

BOOL CGoSetup::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(true);
	return CDialog::DestroyWindow();
}
