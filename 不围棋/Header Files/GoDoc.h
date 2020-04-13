// GoDoc.h : interface of the CGoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GODOC_H__2EC6F21B_0B20_4B0F_9CC9_4FC861DBEF9F__INCLUDED_)
#define AFX_GODOC_H__2EC6F21B_0B20_4B0F_9CC9_4FC861DBEF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGoDoc : public CDocument
{
protected: // create from serialization only
	CGoDoc();
	DECLARE_DYNCREATE(CGoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GODOC_H__2EC6F21B_0B20_4B0F_9CC9_4FC861DBEF9F__INCLUDED_)
