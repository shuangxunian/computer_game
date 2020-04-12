#pragma once
#include "afxwin.h"


// CMove_History_Dialog 对话框

class CMove_History_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMove_History_Dialog)

public:
	CMove_History_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMove_History_Dialog();
// 对话框数据
	enum { IDD = IDD_DIALOG_MOVE_HIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit Edit_Move_His;
	BOOL InitFinish;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateMoveData(WPARAM wParam, LPARAM lParam);
};
