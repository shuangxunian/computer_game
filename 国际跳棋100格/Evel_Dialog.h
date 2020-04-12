#pragma once


// CEvel_Dialog 对话框

class CEvel_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEvel_Dialog)

public:
	CEvel_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEvel_Dialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_EVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CEdit Edit_Evel;
	BOOL InitFinish;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateMoveData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
