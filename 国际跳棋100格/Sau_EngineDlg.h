
// Sau_EngineDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "head.h"

static UINT /*BASED_CODE*/ indicators[]=  
{ 
	IDS_STRING_PRO,
    IDS_STRING_NOTE, 
	IDS_STRING_ENGINE,
	IDS_STRING_TREE,
	IDS_STRING_OTHER,
	IDS_STRING_POINT
};  

// CSau_EngineDlg 对话框
class CSau_EngineDlg : public CDialogEx
{
// 构造
public:
	CSau_EngineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SAU_ENGINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	//一些初始化
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);//不知道
	afx_msg HCURSOR OnQueryDragIcon();//不知道

	//窗口重绘
	afx_msg void OnPaint();

	//
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//回合结束时触发此函数
	afx_msg LRESULT The_Move_Finish(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnInputDepthTimeFinish(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnGetAllMoveFinish(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBoardUserDefined();
	afx_msg LRESULT OnUpdateTree(WPARAM wParam, LPARAM lParam);
	//改变窗口时触发此函数
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//关于
	afx_msg void OnAbout();

	//单击左键时触发此函数
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//选择引擎时触发此函数
	afx_msg void OnEngine_N();
	afx_msg void OnEngine_AB();
	afx_msg void OnEngine_M();

	//选择模式时触发此函数
	afx_msg void OnMode_HvsAI();
	afx_msg void OnMode_AIvsH();
	afx_msg void OnMode_HvsH();

	//选择新游戏时触发此函数
	afx_msg void OnNew_Game();

	//备用输入
	afx_msg void OnSpareInput();

	//关闭程序时触发此函数
	afx_msg void OnClose();

	//棋盘反转
	afx_msg void OnBoardInverse();

	//选择Negascout 时间
	afx_msg void OnTime1s();
	afx_msg void OnTime2s();
	afx_msg void OnTime5s();
	afx_msg void OnTime10s();
	afx_msg void OnTime15s();
	afx_msg void OnTime20s();

	//选择AlphaBeta层数
	afx_msg void OnFloor1();
	afx_msg void OnFloor3();
	afx_msg void OnFloor5();
	afx_msg void OnFloor7();
	afx_msg void OnFloor9();
	afx_msg void OnFloor11();

	//输出框
	afx_msg void OnAllMove();
	afx_msg void OnMoveHistory();
	afx_msg void OnEvel();

	//文件
	afx_msg void OnLoadSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	DECLARE_MESSAGE_MAP()
private:
	int d;//记录棋盘各大小
	int Size_Prompt;//棋子标记的大小
	int Size_Pieces;//棋子的大小
	int Engine;//记录引擎
	int Mode;//记录模式
	int Progress;//进度:开放鼠标->选中棋子->完成输入
	int SpareInput;//是否使用备用输入  0：是， -2：否 ， -1：输入完成
	int Color;//执棋方
	int Step;//步数
	int Round;//场数
	int Board[10][10];//棋盘数组
	int Time;//Negascout 时间
	int Floor;//AlphaBeta层数
	int Computer_Color;
	int Human_Color;
	//储存选中棋子的所有着法数量
	int Prompt_Move_Num;
	//可行着法数量
	int Move_Num;
	//鼠标悬停的吃子路线 0为。。
	int Mouse_Move;
	//1棋盘反转0为不翻
	int Is_Board_Inverse;
	//1着法历史显示0为着法历史
	int Is_Move_History_Show;
	//1所有着法显示0为所有着法
	int Is_All_Move_Show;
	//1估值显示0估值不显示
	int Is_Evel_Show;
	//
	int Is_Set_Board;
	//储存所有着法
	MOVE All_Moves[200];
	//储存选中棋子的所有着法（画标记用）
	MOVE Prompt_Move[20];

	CProgressCtrl m_Pro;
	CStatusBar m_Bar;
	CMenu m_Menu;
	CBitmap PiecesBmp;
	CBitmap BoardBmp;
	CDC BoardDC;
	CDC PiecesDC;
	CRect Board_Rect;
	CRect Mouse_Rect;
	CString Temp_Str;
	HWND Move_His_dghWnd;
	HWND All_Move_dghWnd;
	HWND Evel_dghWnd;
	//根据窗口计算棋盘大小
	void SetBoard(RECT rtBoard);
	//播放音效
	void PlayMySound(int i);
	//重绘
	void DrawChess(int type);
	//画棋子
	void DrawPieces(CClientDC *dc);
	//画棋盘
	void DrawBoard(CClientDC *dc);
	//画标记
	void DrawPrompt(CClientDC *dc);
	//初始化棋盘
	void Init_Board(int Board[][10]);
	//获取AI着法
	void Get_Computer_Move();
	//AI开始思考
	void AI_Star(int Board[][10], int Color);
	//鼠标输入逻辑
	int SToS(CPoint Mouse_Pt);
	//备用鼠标输入
	int Spare_SToS(CPoint Mouse_Pt);
	//显示
	void Show_The_Move(MOVE* TheMove);
	void Show_Evel();
	void Show_All_Move();
	//文件
	MOVE Load_File(int Step, int Round);
	void Save_File(MOVE Move,int Step,int Round);
	void Input_Depth_Time(int Depth,double Time);
	void Get_All_Move();
	void ProgressStar();
	void Go_The_Move(MOVE The_Move);
	void Save_File(int Depth,double Time);
	void Save_File(int Color);
	void Set_Board(CPoint pt);
	int newxy(int star_x, int star_y);
public:
	afx_msg void On32847();
};
