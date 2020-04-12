#pragma once


// CWzdSplash

class CWelcome : public CWnd
{
	DECLARE_DYNAMIC(CWelcome)

public:
	CWelcome();
	virtual ~CWelcome();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CBitmap m_bitmap;  
    void Create(UINT nBitmapID); 
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
