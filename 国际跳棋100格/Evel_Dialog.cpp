// Evel_Dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Sau_Engine.h"
#include "Evel_Dialog.h"
#include "afxdialogex.h"


// CEvel_Dialog 对话框

IMPLEMENT_DYNAMIC(CEvel_Dialog, CDialogEx)

CEvel_Dialog::CEvel_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEvel_Dialog::IDD, pParent)
{

}

CEvel_Dialog::~CEvel_Dialog()
{
}

void CEvel_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Evel, Edit_Evel);
}


BEGIN_MESSAGE_MAP(CEvel_Dialog, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE(UpdateMoveDataMess, &CEvel_Dialog::OnUpdateMoveData)
END_MESSAGE_MAP()


// CEvel_Dialog 消息处理程序
void CEvel_Dialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CWnd *pWnd;   
	pWnd = GetDlgItem(IDC_EDIT_Evel);
	if(pWnd!=NULL)
	{	
		pWnd->SetWindowPos(0,0,0,cx-21,cy-23,SWP_NOZORDER | SWP_NOMOVE ); //把编辑控件的大小设为(100,80)，位置不变
	}
	// TODO: 在此处添加消息处理程序代码
}

afx_msg LRESULT CEvel_Dialog::OnUpdateMoveData(WPARAM wParam, LPARAM lParam)
{
	/*CFont m_Font;
	m_Font.CreateFont(0,0,0,0,  
		0,FALSE,FALSE,0,ANSI_CHARSET,  
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,  
		DEFAULT_QUALITY,FF_SWISS,(LPCTSTR)"Arial");//创建字体  
	Edit_All_Move.SetFont(&m_Font);*/
//	Edit_All_Move.SetSel(1); 
	CString *Evel_Str;
	CString Temp_Str;
	Evel_Str=(CString*)lParam;
	Temp_Str.Format("步数：%d\r\n",wParam);
	Edit_Evel.ReplaceSel(Temp_Str);
	Edit_Evel.ReplaceSel(*Evel_Str);
	return 0;
}



