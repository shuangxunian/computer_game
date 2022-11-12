// WzdSplash.cpp : 实现文件
//

#include "stdafx.h"
#include "Sau_Engine.h"
#include "Welcome.h"
#include "afxdialogex.h"

// CWzdSplash

IMPLEMENT_DYNAMIC(CWelcome, CWnd)

CWelcome::CWelcome()
{

}

CWelcome::~CWelcome()
{
}


BEGIN_MESSAGE_MAP(CWelcome, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWzdSplash 消息处理程序  
void CWelcome::Create(UINT nBitmapID)  
{  
    BOOL hBitmap = m_bitmap.LoadBitmap(nBitmapID);  
    BITMAP bitmap;  
    int ret = m_bitmap.GetBitmap(&bitmap);  
    //CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
    CreateEx(0,  
            AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  
            NULL, WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN), NULL , NULL);  
}  



void CWelcome::OnPaint()
{
	// TODO: 在此处添加消息处理程序代码  
    CPaintDC dc(this); // device context forpainting  
    BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    CDC dcComp;  
    dcComp.CreateCompatibleDC(&dc);  
    dcComp.SelectObject(&m_bitmap);  
    int   cx   =   GetSystemMetrics(SM_CXFULLSCREEN)/2;   
	int   cy   =   GetSystemMetrics(SM_CYFULLSCREEN)/2;
	dc.SetStretchBltMode(HALFTONE);
	dc.StretchBlt(cx-200,cy-200,400,400,&dcComp,0,0,850,850,SRCAND);			
}


void CWelcome::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值  
    //CWnd::OnTimer(nIDEvent);  
    DestroyWindow(); //销毁初始画面窗口 
}
