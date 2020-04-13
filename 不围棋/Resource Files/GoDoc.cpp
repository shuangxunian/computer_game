// GoDoc.cpp : implementation of the CGoDoc class
//

#include "stdafx.h"
#include "Go.h"

#include "GoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGoDoc

IMPLEMENT_DYNCREATE(CGoDoc, CDocument)

BEGIN_MESSAGE_MAP(CGoDoc, CDocument)
	//{{AFX_MSG_MAP(CGoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoDoc construction/destruction

CGoDoc::CGoDoc()
{
	// TODO: add one-time construction code here

}

CGoDoc::~CGoDoc()
{
}

BOOL CGoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle   ( "   ÎÄµµ±êÌâ   "   );   

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGoDoc serialization

void CGoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGoDoc diagnostics

#ifdef _DEBUG
void CGoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGoDoc commands
