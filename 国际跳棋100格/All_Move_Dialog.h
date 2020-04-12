#pragma once


// CAll_Move_Dialog 对话框

class CAll_Move_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAll_Move_Dialog)

public:
	CAll_Move_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAll_Move_Dialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALL_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	CEdit Edit_All_Move;
	BOOL InitFinish;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpdateMoveData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
