//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//国际跳棋组负责人于望所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//于望		QQ：2969235803
#include "stdafx.h"
#include "Sau_Engine.h"
#include "Sau_EngineDlg.h"
#include "afxdialogex.h"
#include "Game.h"
#include "Move_History_Dialog.h"
#include "All_Move_Dialog.h"
#include "Evel_Dialog.h"
#include "WelCome.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int turn = 3;////////////原本保存文件里的turn定义在这里

int StepCount = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSau_EngineDlg 对话框


CSau_EngineDlg::CSau_EngineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSau_EngineDlg::IDD, pParent)
	, d(0)
	,Step(0)
	,Round(0)
	, Engine(0)
	, Mode(0)
	,Time(10)
	,Floor(7)
	,Is_Move_History_Show(0)
	,Is_All_Move_Show(0)
	,Is_Evel_Show(0)
	,Is_Set_Board(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSau_EngineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSau_EngineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MY_MESSAGE, &CSau_EngineDlg::The_Move_Finish) 
	ON_MESSAGE(UPDATE_TREE, &CSau_EngineDlg::OnUpdateTree) 
	ON_COMMAND(ID_ENGINE_N, &CSau_EngineDlg::OnEngine_N)
	ON_COMMAND(ID_ENGINE_AB, &CSau_EngineDlg::OnEngine_AB)
	ON_COMMAND(ID_ENGINE_M, &CSau_EngineDlg::OnEngine_M)
	ON_COMMAND(ID_MODE_HvsAI, &CSau_EngineDlg::OnMode_HvsAI)
	ON_COMMAND(ID_MODE_AIvsH, &CSau_EngineDlg::OnMode_AIvsH)
	ON_COMMAND(ID_MODE_HvsH, &CSau_EngineDlg::OnMode_HvsH)
	ON_COMMAND(ID_NEW_GAME, &CSau_EngineDlg::OnNew_Game)
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_Spare_Input, &CSau_EngineDlg::OnSpareInput)
	ON_COMMAND(ID_Board_Inverse, &CSau_EngineDlg::OnBoardInverse)
	ON_COMMAND(ID_Time_1s, &CSau_EngineDlg::OnTime1s)
	ON_COMMAND(ID_Time_2s, &CSau_EngineDlg::OnTime2s)
	ON_COMMAND(ID_Time_5s, &CSau_EngineDlg::OnTime5s)
	ON_COMMAND(ID_Time_10s, &CSau_EngineDlg::OnTime10s)
	ON_COMMAND(ID_Time_15s, &CSau_EngineDlg::OnTime15s)
	ON_COMMAND(ID_Time_20s, &CSau_EngineDlg::OnTime20s)
	ON_COMMAND(ID_All_Move, &CSau_EngineDlg::OnAllMove)
	ON_COMMAND(ID_About, &CSau_EngineDlg::OnAbout)
	ON_COMMAND(ID_Move_History, &CSau_EngineDlg::OnMoveHistory)
	ON_COMMAND(ID_LOAD_SAVE, &CSau_EngineDlg::OnLoadSave)
	ON_COMMAND(ID_Evel, &CSau_EngineDlg::OnEvel)
	ON_WM_CREATE()
	ON_COMMAND(ID_Floor_1, &CSau_EngineDlg::OnFloor1)
	ON_COMMAND(ID_Floor_3, &CSau_EngineDlg::OnFloor3)
	ON_COMMAND(ID_Floor_5, &CSau_EngineDlg::OnFloor5)
	ON_COMMAND(ID_Floor_7, &CSau_EngineDlg::OnFloor7)
	ON_COMMAND(ID_Floor_9, &CSau_EngineDlg::OnFloor9)
	ON_COMMAND(ID_Floor_11, &CSau_EngineDlg::OnFloor11)
	ON_WM_TIMER()
	ON_COMMAND(ID_Board_User_Defined, &CSau_EngineDlg::OnBoardUserDefined)
	ON_COMMAND(ID_32847, &CSau_EngineDlg::On32847)
END_MESSAGE_MAP()

void CSau_EngineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


HCURSOR CSau_EngineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CSau_EngineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CWelcome Welcome;                 //创建启动窗口类的实例  
	Welcome.Create(IDB_BITMAP_Welcome);  
	Welcome.CenterWindow();  
	Welcome.UpdateWindow();          //send WM_PAINT  
	Sleep(1500);  
	Welcome.DestroyWindow();//销毁初始画面窗口  
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CSau_EngineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	 
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	ShowWindow(SW_HIDE);
	m_Menu.LoadMenu(IDR_MENU1); 
	SetMenu(&m_Menu);

	// TODO: 在此添加额外的初始化代码
	
	//获取窗口大小以调整棋盘
	RECT rt;
	GetClientRect(&rt);
	SetBoard(rt);
	
	//一些默认选项（模式：人人 引擎：Negascout+AlphaBeta）
	Mode= HvsH;
	Engine= Negascout|AlphaBeta	;
	Is_Board_Inverse= Normal;
	//输出信息的窗口开（估值，着发，历史）
	OnAllMove();
	OnMoveHistory();
	OnEvel();
	congli=0;
	//声明线程函数 并创建
	UINT AI_Thread(LPVOID pParam) ;
 	theApp.Handle_AI_Thread=AfxBeginThread(AI_Thread, NULL,NULL,NULL,CREATE_SUSPENDED); 
	ShowWindow(SW_MAXIMIZE);
	//状态栏
	if(!m_Bar.Create(this))
	{
	  MessageBox("状态栏创建错误！", _T("错误" ), MB_OK);
	} 
	m_Bar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	RECT rect;  
	GetClientRect(&rect); 
	m_Bar.MoveWindow(2,rect.bottom-20,rect.right-2,20);
	m_Bar.SetPaneInfo(0, IDS_STRING_PRO, SBPS_NORMAL,120); 
	m_Bar.SetPaneInfo(1, IDS_STRING_NOTE, SBPS_NORMAL,250); 
	m_Bar.SetPaneInfo(2, IDS_STRING_ENGINE, SBPS_NORMAL,300);
	m_Bar.SetPaneInfo(3, IDS_STRING_TREE,SBPS_NORMAL , 160);
	m_Bar.SetPaneInfo(4, IDS_STRING_OTHER,SBPS_STRETCH,0);
	m_Bar.SetPaneInfo(5, IDS_STRING_POINT, SBPS_NORMAL,100);
	
	if (!m_Pro.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH,CRect(1,3,126,19),&m_Bar,1))
	{
		MessageBox("进度条创建错误！", _T("错误" ), MB_OK);
	}
	m_Pro.SetStep(4);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSau_EngineDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawChess(D_Pieces|D_Board|D_Prompt);
	}
}


void CSau_EngineDlg::SetBoard(RECT rtBoard)
{
	d=rtBoard.bottom-rtBoard.top;
	Size_Pieces=d-d/4;
	Size_Prompt=d-d/6;

	Board_Rect.left=d;
	Board_Rect.top=d;
	Board_Rect.right=d+d*10;
	Board_Rect.bottom=d+d*10;
}


void CSau_EngineDlg::PlayMySound(int i)
{
	char filename[MAX_PATH] = { 0 };

	switch (i)
	{
	case 0://选子音效
		strcpy_s(filename, "C:\\Users\\凯凯\\Desktop\\Sau_Engine\\SOUND\\选子.wav");
		break;
	case 1://落子音效
		strcpy_s(filename, "C:\\Users\\凯凯\\Desktop\\Sau_Engine\\SOUND\\落子.wav");
		break;
	case 2://吃子音效
		strcpy_s(filename, "C:\\Users\\凯凯\\Desktop\\Sau_Engine\\SOUND\\吃子.wav");
		break;
	default:
		break;
	}

	//播放音效
	PlaySound(filename, NULL, SND_NOWAIT | SND_FILENAME);

}


void CSau_EngineDlg::DrawChess(int type)
{
	CClientDC dc(this);
	if(type&D_Board)
	{
		DrawBoard(&dc);
	}
	if(type&D_Pieces)
	{
		DrawPieces(&dc);
	}
	if(type&D_Prompt)
	{
		DrawPrompt(&dc);
	}
}


void CSau_EngineDlg::DrawPieces(CClientDC *dc)
{
	POINT Screen_Pt;
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_Pieces);
	PiecesDC.CreateCompatibleDC(dc);
	CBitmap *OldBitmap = PiecesDC.SelectObject(&bitmap);
	dc->SetStretchBltMode(HALFTONE);
	
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{	
			if(Is_Board_Inverse==Normal)
			{ 
				Screen_Pt.x=d+d*i+d/8;
				Screen_Pt.y=d+d*j+d/8;
			}
			else if(Is_Board_Inverse==Flip)
			{
				Screen_Pt.x=d+d*(9-i)+d/8;
				Screen_Pt.y=d+d*(9-j)+d/8;
			}
			switch(Board[i][j])
			{
				case 1:
					{dc->StretchBlt(Screen_Pt.x,Screen_Pt.y,Size_Pieces,Size_Pieces,&PiecesDC,0,0,50,50,SRCCOPY);break;}
				case 2:
					{dc->StretchBlt(Screen_Pt.x,Screen_Pt.y,Size_Pieces,Size_Pieces,&PiecesDC,0,50,50,50,SRCCOPY);break;}
				case 3:
					{dc->StretchBlt(Screen_Pt.x,Screen_Pt.y,Size_Pieces,Size_Pieces,&PiecesDC,50,0,50,50,SRCCOPY);break;}
				case 4:
					{dc->StretchBlt(Screen_Pt.x,Screen_Pt.y,Size_Pieces,Size_Pieces,&PiecesDC,50,50,50,50,SRCCOPY);break;}
				default:
					break;
			}
		}
	}
	PiecesDC.SelectObject(OldBitmap);
	bitmap.DeleteObject();
	PiecesDC.DeleteDC();
}


void CSau_EngineDlg::DrawBoard(CClientDC *dc)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_Board);
	PiecesDC.CreateCompatibleDC(dc);
	CBitmap *OldBitmap = PiecesDC.SelectObject(&bitmap);
	dc->SetStretchBltMode(HALFTONE);
	dc->StretchBlt(Board_Rect.left-d/12,Board_Rect.top-d/11,Board_Rect.Width()+d/5,Board_Rect.Height()+d/4,&PiecesDC,0,0,612,614,SRCCOPY);
	bitmap.DeleteObject();
	PiecesDC.DeleteDC();
}


void CSau_EngineDlg::DrawPrompt(CClientDC *dc)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_Prompt);
	PiecesDC.CreateCompatibleDC(dc);
	CBitmap *OldBitmap = PiecesDC.SelectObject(&bitmap);//int i=Prompt_Move[0].move_star_x*d+d//int i=（10-Prompt_Move[0].move_star_x）*d+d//棋盘反转

	POINT Prompt_Pt_Star,Prompt_Pt_Stop,Prompt_Pt_Eat[20];

	if(SpareInput==-2)//输入方式一 
	{
		if(Progress==Chose)
		{
			if(Is_Board_Inverse==Normal)
			{ 
				Prompt_Pt_Star.x=Prompt_Move[0].move_star_x*d+d+d/12;
				Prompt_Pt_Star.y=Prompt_Move[0].move_star_y*d+d+d/12;

				Prompt_Pt_Stop.x=Prompt_Move[Mouse_Move].move_stop_x*d+d+d/12;
				Prompt_Pt_Stop.y=Prompt_Move[Mouse_Move].move_stop_y*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=Prompt_Move[Mouse_Move].eatPoint[i][0]*d+d+d/12;
					Prompt_Pt_Eat[i].y=Prompt_Move[Mouse_Move].eatPoint[i][1]*d+d+d/12;
				}
			}
			else if(Is_Board_Inverse==Flip)
			{
				Prompt_Pt_Star.x=(9-Prompt_Move[0].move_star_x)*d+d+d/12;
				Prompt_Pt_Star.y=(9-Prompt_Move[0].move_star_y)*d+d+d/12;

				Prompt_Pt_Stop.x=(9-Prompt_Move[Mouse_Move].move_stop_x)*d+d+d/12;
				Prompt_Pt_Stop.y=(9-Prompt_Move[Mouse_Move].move_stop_y)*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=(9-Prompt_Move[Mouse_Move].eatPoint[i][0])*d+d+d/12;
					Prompt_Pt_Eat[i].y=(9-Prompt_Move[Mouse_Move].eatPoint[i][1])*d+d+d/12;
				}
			}
			//标记起点
			switch(Board[Prompt_Move[0].move_star_x][Prompt_Move[0].move_star_y])
			{
				case 1:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,0,50,50,SRCCOPY);break;
			case 2:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,50,50,50,SRCCOPY);break;
			case 3:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,0,50,50,SRCCOPY);break;
			case 4:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,50,50,50,SRCCOPY);break;
			}
			//标记可行落点
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,50,50,50,50,SRCAND);
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,0,50,50,50,SRCPAINT);
			//标记吃子
			for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
			{
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,50,0,50,50,SRCAND);
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,0,0,50,50,SRCPAINT);
			}

		}
		else if(Progress==Blocked)
		{
			if(Is_Board_Inverse==Normal)
			{ 
				Prompt_Pt_Star.x=Prompt_Move[0].move_stop_x*d+d+d/12;
				Prompt_Pt_Star.y=Prompt_Move[0].move_stop_y*d+d+d/12;

				Prompt_Pt_Stop.x=Prompt_Move[Mouse_Move].move_star_x*d+d+d/12;
				Prompt_Pt_Stop.y=Prompt_Move[Mouse_Move].move_star_y*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=Prompt_Move[Mouse_Move].eatPoint[i][0]*d+d+d/12;
					Prompt_Pt_Eat[i].y=Prompt_Move[Mouse_Move].eatPoint[i][1]*d+d+d/12;
				}
			}
			else if(Is_Board_Inverse==Flip)
			{
				Prompt_Pt_Star.x=(9-Prompt_Move[0].move_stop_x)*d+d+d/12;
				Prompt_Pt_Star.y=(9-Prompt_Move[0].move_stop_y)*d+d+d/12;

				Prompt_Pt_Stop.x=(9-Prompt_Move[Mouse_Move].move_star_x)*d+d+d/12;
				Prompt_Pt_Stop.y=(9-Prompt_Move[Mouse_Move].move_star_y)*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=(9-Prompt_Move[Mouse_Move].eatPoint[i][0])*d+d+d/12;
					Prompt_Pt_Eat[i].y=(9-Prompt_Move[Mouse_Move].eatPoint[i][1])*d+d+d/12;
				}
			}
			switch(Board[Prompt_Move[0].move_stop_x][Prompt_Move[0].move_stop_y])
			{
			case 1:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,0,50,50,SRCCOPY);break;
			case 2:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,50,50,50,SRCCOPY);break;
			case 3:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,0,50,50,SRCCOPY);break;
			case 4:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,50,50,50,SRCCOPY);break;
			}
			//标记点
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,50,50,50,50,SRCAND);
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,0,50,50,50,SRCPAINT);
			//标记吃子
			for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
			{
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,50,0,50,50,SRCAND);
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,0,0,50,50,SRCPAINT);
			}
		}
	}
	else //输入方式二
	{
		if(Is_Board_Inverse==Normal)
		{ 
			Prompt_Pt_Star.x=Prompt_Move[0].move_star_x*d+d+d/12;
			Prompt_Pt_Star.y=Prompt_Move[0].move_star_y*d+d+d/12;

			Prompt_Pt_Stop.x=Prompt_Move[0].move_stop_x*d+d+d/12;
			Prompt_Pt_Stop.y=Prompt_Move[0].move_stop_y*d+d+d/12;
		
			for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
			{		
				Prompt_Pt_Eat[i].x=Prompt_Move[0].eatPoint[i][0]*d+d+d/12;
				Prompt_Pt_Eat[i].y=Prompt_Move[0].eatPoint[i][1]*d+d+d/12;
			}
		}
		else if(Is_Board_Inverse==Flip)
		{
				Prompt_Pt_Star.x=(9-Prompt_Move[0].move_star_x)*d+d+d/12;
				Prompt_Pt_Star.y=(9-Prompt_Move[0].move_star_y)*d+d+d/12;

				Prompt_Pt_Stop.x=(9-Prompt_Move[0].move_stop_x)*d+d+d/12;
				Prompt_Pt_Stop.y=(9-Prompt_Move[0].move_stop_y)*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=(9-Prompt_Move[0].eatPoint[i][0])*d+d+d/12;
					Prompt_Pt_Eat[i].y=(9-Prompt_Move[0].eatPoint[i][1])*d+d+d/12;
				}
		}
		if(SpareInput>=0)
		{
			switch(Board[Prompt_Move[0].move_star_x][Prompt_Move[0].move_star_y])
			{
			case 1:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,0,50,50,SRCCOPY);break;
			case 2:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,50,50,50,SRCCOPY);break;
			case 3:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,0,50,50,SRCCOPY);break;
			case 4:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,50,50,50,SRCCOPY);break;
			}
		}
		if(SpareInput>0)
		{
			for(int i=0;i<SpareInput;i++)
			{
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,50,0,50,50,SRCAND);
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,0,0,50,50,SRCPAINT);
			}
		}
		if(SpareInput==-1)
		{
			if(Is_Board_Inverse==Normal)
			{ 
				Prompt_Pt_Star.x=Prompt_Move[0].move_stop_x*d+d+d/12;
				Prompt_Pt_Star.y=Prompt_Move[0].move_stop_y*d+d+d/12;

				Prompt_Pt_Stop.x=Prompt_Move[0].move_star_x*d+d+d/12;
				Prompt_Pt_Stop.y=Prompt_Move[0].move_star_y*d+d+d/12;

				for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
				{		
					Prompt_Pt_Eat[i].x=Prompt_Move[0].eatPoint[i][0]*d+d+d/12;
					Prompt_Pt_Eat[i].y=Prompt_Move[0].eatPoint[i][1]*d+d+d/12;
				}

			}
			else if(Is_Board_Inverse==Flip)
			{
					Prompt_Pt_Star.x=(9-Prompt_Move[0].move_stop_x)*d+d+d/12;
					Prompt_Pt_Star.y=(9-Prompt_Move[0].move_stop_y)*d+d+d/12;

					Prompt_Pt_Stop.x=(9-Prompt_Move[0].move_star_x)*d+d+d/12;
					Prompt_Pt_Stop.y=(9-Prompt_Move[0].move_star_y)*d+d+d/12;

					for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
					{		
						Prompt_Pt_Eat[i].x=(9-Prompt_Move[0].eatPoint[i][0])*d+d+d/12;
						Prompt_Pt_Eat[i].y=(9-Prompt_Move[0].eatPoint[i][1])*d+d+d/12;
					}
			}
			switch(Board[Prompt_Move[0].move_stop_x][Prompt_Move[0].move_stop_y])
			{
			case 1:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,0,50,50,SRCCOPY);break;
			case 2:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,150,50,50,50,SRCCOPY);break;
			case 3:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,0,50,50,SRCCOPY);break;
			case 4:
				dc->StretchBlt(Prompt_Pt_Star.x,Prompt_Pt_Star.y,Size_Prompt,Size_Prompt,&PiecesDC,100,50,50,50,SRCCOPY);break;
			}
			//标记可行落点
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,50,50,50,50,SRCAND);
			dc->StretchBlt(Prompt_Pt_Stop.x,Prompt_Pt_Stop.y,Size_Prompt,Size_Prompt,&PiecesDC,0,50,50,50,SRCPAINT);
			//标记吃子
			for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
			{
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,50,0,50,50,SRCAND);
				dc->StretchBlt(Prompt_Pt_Eat[i].x,Prompt_Pt_Eat[i].y,Size_Prompt,Size_Prompt,&PiecesDC,0,0,50,50,SRCPAINT);
			}
		}
	}
	PiecesDC.SelectObject(OldBitmap);
	bitmap.DeleteObject();
	PiecesDC.DeleteDC();
}


void CSau_EngineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	d=(cx<cy?cx:cy)/12;
	Size_Pieces=d-d/4;
	Size_Prompt=d-d/6;
	//DrawChess(Board);
	Board_Rect.left=d;
	Board_Rect.top=d;
	Board_Rect.right=d+d*10;
	Board_Rect.bottom=d+d*10;
	Invalidate(1);//重绘棋盘
	UpdateWindow();
	

	// TODO: 在此处添加消息处理程序代码
}


void CSau_EngineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//跳出循环体 bug//////////////////////
	CPoint pt;
	CRect Mouse_Rect;
	if(Progress!=Blocked)
		if(Board_Rect.PtInRect(point))
			for(int i=0;i<10;i++)
			{
				for(int j=0;j<10;j++)
				{
					if(Is_Board_Inverse==Normal)
					{ 
						Mouse_Rect.SetRect(
							d*i+Board_Rect.left,
							d*j+Board_Rect.top,
							d*i+d+Board_Rect.left,
							d*j+d+Board_Rect.top);
					}
					else if(Is_Board_Inverse==Flip)
					{
						Mouse_Rect.SetRect(
							(9-i)*d+Board_Rect.left,
							(9-j)*d+Board_Rect.top,
							(9-i)*d+d+Board_Rect.left,
							(9-j)*d+d+Board_Rect.top);
					}
					if( Mouse_Rect.PtInRect(point) )
					{
						pt.x=i;
						pt.y=j;
						if(Is_Set_Board==0)
						{
							if(SpareInput==-2)
							{
								if(SToS(pt)==2)
								{
									::PostMessage(m_hWnd, WM_MY_MESSAGE, 0,0);
									//跳出循环
									i=10;j=10;
								}
							}
							else if(SpareInput==0||SpareInput==-1)
							{
								if(Spare_SToS(pt)==2)
								{
									::PostMessage(m_hWnd, WM_MY_MESSAGE, 0, 0);
									//跳出循环
									i=10;j=10;
								}
							}
						}
						else
						{
							Set_Board(pt);
							i=10;j=10;
						}
					}
				}
			}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CSau_EngineDlg::OnEngine_N()
{
 if (Engine&Negascout)
 {
  GetMenu()->CheckMenuItem(ID_ENGINE_N,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
  Engine= Engine&~Negascout;
 }
 else
 {
   GetMenu()->CheckMenuItem(ID_ENGINE_N,MF_BYCOMMAND|MF_CHECKED);//标记
   Engine= Engine|Negascout;
 }

}


void CSau_EngineDlg::OnEngine_AB()
{
	// TODO: 在此添加命令处理程序代码
	if (Engine&AlphaBeta)
	 {
	  GetMenu()->CheckMenuItem(ID_ENGINE_AB,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	  Engine= Engine&~AlphaBeta;
	 }
	 else
	 {
	   GetMenu()->CheckMenuItem(ID_ENGINE_AB,MF_BYCOMMAND|MF_CHECKED);//标记
	   Engine= Engine|AlphaBeta;
	 }
}


void CSau_EngineDlg::OnEngine_M()
{
	// TODO: 在此添加命令处理程序代码
	if (Engine&Minimax)
	 {
	  GetMenu()->CheckMenuItem(ID_ENGINE_M,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	  Engine= Engine&~Minimax;
	 }
	 else
	 {
	   GetMenu()->CheckMenuItem(ID_ENGINE_M,MF_BYCOMMAND|MF_CHECKED);//标记
	   Engine= Engine|Minimax;
	 }
}


void CSau_EngineDlg::OnMode_HvsAI()
{
	// TODO: 在此添加命令处理程序代码
	
	   GetMenu()->CheckMenuItem(ID_MODE_HvsAI,MF_BYCOMMAND|MF_CHECKED);//标记
	   GetMenu()->CheckMenuItem(ID_MODE_HvsH,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	   GetMenu()->CheckMenuItem(ID_MODE_AIvsH,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	   Mode=HvsAI;
}


void CSau_EngineDlg::OnMode_AIvsH()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(ID_MODE_AIvsH,MF_BYCOMMAND|MF_CHECKED);//标记
	GetMenu()->CheckMenuItem(ID_MODE_HvsAI,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_MODE_HvsH,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	Mode=AIvsH;
}


void CSau_EngineDlg::OnMode_HvsH()
{
	// TODO: 在此添加命令处理程序代码
	   GetMenu()->CheckMenuItem(ID_MODE_HvsH,MF_BYCOMMAND|MF_CHECKED);//标记
	   GetMenu()->CheckMenuItem(ID_MODE_HvsAI,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	   GetMenu()->CheckMenuItem(ID_MODE_AIvsH,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	   Mode=HvsH;
}


void CSau_EngineDlg::OnNew_Game()
{
	Init_Board(Board);
	Round++;
	Step=0;
	StepCount = 0;
	Color=BLACK;
	SpareInput=-2;
	DrawChess(D_Pieces|D_Board);
	if(Mode==HvsH)
	{
		Progress=Open;//解除鼠标屏蔽
	}
	else if(Mode==HvsAI)
	{
		Human_Color=BLACK;
		Computer_Color=WHITE;
		Progress=Open;//解除鼠标屏蔽
	}
	else if(Mode==AIvsH)
	{
		Progress=Blocked;//屏蔽鼠标
		Computer_Color=BLACK;
		Human_Color=WHITE;
	}
	Get_All_Move();
}


afx_msg LRESULT CSau_EngineDlg::The_Move_Finish(WPARAM wParam, LPARAM lParam)
{
	if(wParam!=0)
	{
		if(wParam==BLACK)
			MessageBox("白（蓝）方胜","终局", MB_OK);
		else
			MessageBox("黑（红）方胜","终局", MB_OK);
		Save_File(Color);
	}
	else
	{
		if(lParam==All_Move_Finish)
		{	
			if(Mode==HvsH)
			{
				Show_All_Move();
				Show_Evel();
				Progress=Open;
				return 0;
			}
			else if(Mode==HvsAI||Mode==AIvsH)
			{
				if(Color==Computer_Color)
				{
					Show_All_Move();
					Show_Evel();
					ProgressStar();
					AI_Star(Board, Color);//AI开始 思考
					return 0;
				}
				else if(Color==Human_Color)
				{
					Show_All_Move();
					Show_Evel();
					//恢复菜单
					CMenu *pMenu = GetMenu();
					if(pMenu)
					{
						pMenu->EnableMenuItem(ID_Time_1s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Time_2s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Time_5s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Time_10s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Time_15s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Time_20s, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_1, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_3, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_5, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_7, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_9, MF_ENABLED);
						pMenu->EnableMenuItem(ID_Floor_11, MF_ENABLED);
						pMenu->EnableMenuItem(ID_NEW_GAME, MF_ENABLED);
						pMenu->EnableMenuItem(ID_LOAD_SAVE, MF_ENABLED);
						pMenu->EnableMenuItem(ID_ENGINE_N, MF_ENABLED);
						pMenu->EnableMenuItem(ID_ENGINE_AB, MF_ENABLED);
					}
					Progress=Open;
					return 0;
				}
			}
		}
		else if(lParam==0)
		{
			if(Mode==HvsH)
			{	
				Go_The_Move(Prompt_Move[0]);
				DrawChess(D_Board|D_Pieces|D_Prompt);
				Step++; StepCount++;
				Show_The_Move(Prompt_Move);
				Color=3-Color;	
			}
			else if(Mode==HvsAI||Mode==AIvsH)
			{
				if(Color==Human_Color)
				{
					Go_The_Move(Prompt_Move[0]);
					DrawChess(D_Board|D_Pieces|D_Prompt);
					Step++; StepCount++;
					Color=Computer_Color;
					Show_The_Move(Prompt_Move);
				}
				else if(Color==Computer_Color)
				{
					Get_Computer_Move();
					Go_The_Move(Prompt_Move[0]);
					DrawChess(D_Board|D_Pieces|D_Prompt);
					Step++; StepCount++;
					Color=Human_Color;
					Show_The_Move(Prompt_Move);
				}
			}
			if(SpareInput!=-2)SpareInput=-2;//恢复输入方式一
			Get_All_Move();
		}
	}
	return 0;
}


void CSau_EngineDlg::OnClose()//加 保存
{
	/*int i;
	//MessageBox("正在等待AI线程结束！","AI Thread Message", MB_OK);
	while(theApp.Thread_Sign==1);
	theApp.Board_Mess.Add_Mess=Dele;
	i=theApp.Handle_AI_Thread->ResumeThread();
	//i=theApp.Handle_AI_Thread->ResumeThread();
	Sleep(0);
	while(theApp.Thread_Sign);
	MessageBox("AI线程已结束！","AI Thread Message", MB_OK);


	Sleep(1000);*/
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}


void CSau_EngineDlg::Get_Computer_Move()
{
	Prompt_Move[0]=theApp.Move_Mess.The_Good_Move;
	Mouse_Move=0;
	if(theApp.Move_Mess.Add_Mess&Err_Engine_AB)
		m_Bar.SetPaneText(2,"AlphaBeta引擎引起出错！随机选择着法！",1); 
	else if(theApp.Move_Mess.Add_Mess&Err_Engine_Check)
		m_Bar.SetPaneText(2,"备用引擎引起出错！随机选择着法！",1); 
	else
		m_Bar.SetPaneText(2,"AI_Thread_Suspend",1);
	if(SpareInput!=-2)SpareInput=-2;//恢复输入方式一

}


void CSau_EngineDlg::AI_Star(int Board[][10], int Color)
{
	//屏蔽一些菜单
	CMenu *pMenu = GetMenu();
	if(pMenu)
	{
		pMenu->EnableMenuItem(ID_Time_1s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Time_2s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Time_5s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Time_10s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Time_15s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Time_20s, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_1, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_3, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_5, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_7, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_9, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_Floor_11, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_NEW_GAME, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_LOAD_SAVE, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENGINE_N, MF_DISABLED|MF_GRAYED);
		pMenu->EnableMenuItem(ID_ENGINE_AB, MF_DISABLED|MF_GRAYED);
	}

	//赋值Board_Mess
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
		{
			theApp.Board_Mess.Board[i][j]=Board[i][j];
		}
	theApp.Board_Mess.Add_Mess=Engine;
	theApp.Board_Mess.Color=Color;
	//恢复线程
	theApp.Handle_AI_Thread->ResumeThread();
}


void CSau_EngineDlg::Init_Board(int Board[10][10])
{
	for(int j=0;j<10;j++)
	{
		for(int i=0;i<10;i++)
		{
			Board[i][j]=0;
				if((i+j)%2)
				{	//Board[i][j]=0;
					if(j<4)
						Board[i][j]=1;	
					else if(j>5)
						Board[i][j]=2;
				}
		}
	}
	//Board[1][0]=3;
	//Board[8][9]=4;
}


int CSau_EngineDlg::SToS(CPoint Mouse_Pt)
{
	
	if (Progress==Open)//选中棋子
	{
		Prompt_Move_Num=0;
		for(int i=0;i< Move_Num;i++)
		{			
			if(Mouse_Pt.x==All_Moves[i].move_star_x&&Mouse_Pt.y==All_Moves[i].move_star_y)//鼠标点是否匹配到棋子起点 并标记
			{
				Prompt_Move[Prompt_Move_Num]=All_Moves[i];
				Prompt_Move_Num++;
				Progress=Chose;
			}	
		}
		if(Progress==Chose)
		{
			Mouse_Move=-1;
			DrawChess(D_Board|D_Pieces|D_Prompt);
			return 0;//着法进行
		}
		
	}
	else if (Progress==Chose)//走子
	{
	
		if (Board[Mouse_Pt.x][Mouse_Pt.y]%2!=Color-1&&Board[Mouse_Pt.x][Mouse_Pt.y]!=0)//选中本方棋子
		{	
			int temp=0;
			for(int i=0;i< Move_Num;i++)
			{		
				if(Mouse_Pt.x==All_Moves[i].move_star_x&&Mouse_Pt.y==All_Moves[i].move_star_y)//鼠标点是否匹配到棋子起点 并标记
				{
					if(temp==0){Prompt_Move_Num=0;temp=1;}
					Prompt_Move[Prompt_Move_Num]=All_Moves[i];
					Prompt_Move_Num++;
					Progress=Chose;
				}	
			}	
			Mouse_Move=-1;
			DrawChess(D_Board|D_Pieces|D_Prompt);
			return 0;//着法进行
		}
		else
		{
			for(int i=0;i<Prompt_Move_Num;i++)
			{
				
					if(Prompt_Move[i].move_stop_x==Mouse_Pt.x&&Prompt_Move[i].move_stop_y==Mouse_Pt.y)
					{

						Prompt_Move[0]=Prompt_Move[i];
						Progress=Blocked;
						DrawChess(D_Board|D_Pieces|D_Prompt);
						return 2;//着法回合结束
					}
					else if(Prompt_Move[i].eatHowMany)
					{
						for(int k=0;k<=All_Moves[i].eatHowMany;k++)
							if(Prompt_Move[i].eatPoint[k][0]==Mouse_Pt.x&&Prompt_Move[i].eatPoint[k][1]==Mouse_Pt.y)
							{
								Prompt_Move[0]=Prompt_Move[i];
								Progress=Blocked;
								DrawChess(D_Board|D_Pieces|D_Prompt);
								return 2;//着法回合结束
							}
					}
				}	
			}

	}
	return -1;
}


void CSau_EngineDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			
					if(Is_Board_Inverse==Normal)
					{ 
						Mouse_Rect.SetRect(
							d*i+Board_Rect.left,
							d*j+Board_Rect.top,
							d*i+d+Board_Rect.left,
							d*j+d+Board_Rect.top);
					}
					else if(Is_Board_Inverse==Flip)
					{
						Mouse_Rect.SetRect(
							(9-i)*d+Board_Rect.left,
							(9-j)*d+Board_Rect.top,
							(9-i)*d+d+Board_Rect.left,
							(9-j)*d+d+Board_Rect.top);
					}
					if( Mouse_Rect.PtInRect(point) )
					{
						Temp_Str.Format("X=%d,Y=%d.",i,j);
						m_Bar.SetPaneText(5,Temp_Str,1) ;
					}
		}
	}
	
	if(Progress==Chose)
	{
		int ii=0;
		int Temp_Mouse_Move=0;
		for(int i=0;i<Prompt_Move_Num;i++)
		{
			if(Is_Board_Inverse==Normal)
			{ 
				Mouse_Rect.SetRect(
						Prompt_Move[i].move_stop_x*d+Board_Rect.left,
						Prompt_Move[i].move_stop_y*d+Board_Rect.top,
						Prompt_Move[i].move_stop_x*d+d+Board_Rect.left,
						Prompt_Move[i].move_stop_y*d+d+Board_Rect.top);
			}
			else if(Is_Board_Inverse==Flip)
			{
				Mouse_Rect.SetRect(
						(9-Prompt_Move[i].move_stop_x)*d+Board_Rect.left,
						(9-Prompt_Move[i].move_stop_y)*d+Board_Rect.top,
						(9-Prompt_Move[i].move_stop_x)*d+d+Board_Rect.left,
						(9-Prompt_Move[i].move_stop_y)*d+d+Board_Rect.top);
			}
			if(Mouse_Rect.PtInRect(point))
			{
				ii=1; Temp_Mouse_Move=i;break;
			}
			else
			{
				for(int j=0;j<Prompt_Move[i].eatHowMany;j++)
				{
					if(Is_Board_Inverse==Normal)
					{ 
						Mouse_Rect.SetRect(
							Prompt_Move[i].eatPoint[j][0]*d+Board_Rect.left,
							Prompt_Move[i].eatPoint[j][1]*d+Board_Rect.top,
							Prompt_Move[i].eatPoint[j][0]*d+d+Board_Rect.left,
							Prompt_Move[i].eatPoint[j][1]*d+d+Board_Rect.top);
					}
					else if(Is_Board_Inverse==Flip)
					{
						Mouse_Rect.SetRect(
							(9-Prompt_Move[i].eatPoint[j][0])*d+Board_Rect.left,
							(9-Prompt_Move[i].eatPoint[j][1])*d+Board_Rect.top,
							(9-Prompt_Move[i].eatPoint[j][0])*d+d+Board_Rect.left,
							(9-Prompt_Move[i].eatPoint[j][1])*d+d+Board_Rect.top);
					}
					if(Mouse_Rect.PtInRect(point))
					{
						ii=1; Temp_Mouse_Move=i;break;
					}
				}
			}
		}

		if(ii==1&&Mouse_Move!=Temp_Mouse_Move)
		{
			Mouse_Move=Temp_Mouse_Move;
			//PlayMySound(1);
			DrawChess(D_Board|D_Pieces|D_Prompt);//重绘棋盘
		}

		else if(ii==0&&Mouse_Move!=-1)
		{
			Mouse_Move=-1;
			DrawChess(D_Board|D_Pieces|D_Prompt);//重绘棋盘
		}
		if(ii==0)
			Mouse_Move=-1;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


int CSau_EngineDlg::Spare_SToS(CPoint Mouse_Pt)
{
	if(SpareInput==0)
	{
		
		for(int i=0;i< Move_Num;i++)
		{			
			if(Mouse_Pt.x==All_Moves[i].move_star_x&&Mouse_Pt.y==All_Moves[i].move_star_y)//鼠标点是否匹配到棋子起点 并标记
			{
				Prompt_Move[0].move_star_x=Mouse_Pt.x;
				Prompt_Move[0].move_star_y=Mouse_Pt.y;
				DrawChess(D_Board|D_Pieces|D_Prompt);
				SpareInput++;
				if(SpareInput-1==All_Moves[i].eatHowMany)//实际一定不会为真
				{
					SpareInput=-1;
				}
				return 0;//break;
			}	
		}	
	}
	else if(SpareInput>0)
	{
		int temp=0;
		for(int i=0;i< Move_Num;i++)
		{	
				if(Prompt_Move[0].move_star_x==All_Moves[i].move_star_x&&Prompt_Move[0].move_star_y==All_Moves[i].move_star_y)
				{	
					for(int j=0;j<SpareInput-1;j++)
						if(Prompt_Move[0].eatPoint!=All_Moves[i].eatPoint)
							 temp=1;

					if(All_Moves[i].eatPoint[SpareInput-1][0]==Mouse_Pt.x&&All_Moves[i].eatPoint[SpareInput-1][1]==Mouse_Pt.y&&temp!=1)
					{
						Prompt_Move[0].eatPoint[SpareInput-1][0]=Mouse_Pt.x;
						Prompt_Move[0].eatPoint[SpareInput-1][1]=Mouse_Pt.y;
						Prompt_Move[0].eatHowMany=SpareInput;
						DrawChess(D_Board|D_Pieces|D_Prompt);
						SpareInput++;

						if(SpareInput-1==All_Moves[i].eatHowMany)
						{
							SpareInput=-1;
						}
						
						return 0;
					}
					

				}
		}
	}
	else if(SpareInput==-1)
	{
		for(int i=0;i< Move_Num;i++)
		{	
				if(Prompt_Move[0].move_star_x==All_Moves[i].move_star_x&&Prompt_Move[0].move_star_y==All_Moves[i].move_star_y)
				{	
					
					if(All_Moves[i].move_stop_x==Mouse_Pt.x&&All_Moves[i].move_stop_y==Mouse_Pt.y)
					{
						Prompt_Move[0].move_stop_x=Mouse_Pt.x;
						Prompt_Move[0].move_stop_y=Mouse_Pt.y;
						DrawChess(D_Board|D_Pieces|D_Prompt);
						
						return 2;
					}


				}
		}
	}
	return 0;
}


void CSau_EngineDlg::OnSpareInput()
{
	Progress=Open;//重置鼠标状态
	//清空临时着法
	Prompt_Move[0].move_star_x=0;
	Prompt_Move[0].move_star_y=0;
	Prompt_Move[0].move_stop_x=0;
	Prompt_Move[0].move_stop_x=0;
	for(int i=0;i<Prompt_Move[0].eatHowMany;i++)
	{
		Prompt_Move[0].eatPoint[i][0]=0;
		Prompt_Move[0].eatPoint[i][1]=0;
	}
	Prompt_Move[0].eatHowMany=0;
	Invalidate(FALSE);//重绘棋盘
	SpareInput=0;
	// TODO: 在此添加命令处理程序代码
}


void CSau_EngineDlg::OnBoardInverse()
{
	if(Is_Board_Inverse==0)
	{ 
		GetMenu()->CheckMenuItem(ID_Board_Inverse,MF_BYCOMMAND|MF_CHECKED);//标记
		Invalidate(FALSE);//重绘棋盘
		Is_Board_Inverse=1;
	}
	else if(Is_Board_Inverse==1)
	{
		 GetMenu()->CheckMenuItem(ID_Board_Inverse,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
		 Invalidate(FALSE);//重绘棋盘
		 Is_Board_Inverse=0;
	}  
	// TODO: 在此添加命令处理程序代码
}


void CSau_EngineDlg::OnTime1s()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_1s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=1;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnTime2s()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_2s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=2;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnTime5s()
{
	
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_5s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=5;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnTime10s()
{
	
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_10s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=10;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnTime15s()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_15s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=15;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnTime20s()
{
	// TODO: 在此添加命令处理程序代码
	
	int ID;
	if(Time==1)ID=ID_Time_1s;
	else if(Time==2)ID=ID_Time_2s;
	else if(Time==5)ID=ID_Time_5s;
	else if(Time==10)ID=ID_Time_10s;
	else if(Time==15)ID=ID_Time_15s;
	else if(Time==20)ID=ID_Time_20s;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Time_20s,MF_BYCOMMAND|MF_CHECKED);//标记
	Time=20;
	Input_Depth_Time(Floor,Time);
}


void CSau_EngineDlg::OnAllMove()
{
	if(Is_All_Move_Show==0)
	{
		Is_All_Move_Show=1;
		GetMenu()->CheckMenuItem(ID_All_Move,MF_BYCOMMAND|MF_CHECKED);//取消标记
		CAll_Move_Dialog *dg=new CAll_Move_Dialog;
		dg->Create(IDD_DIALOG_ALL_MOVE);
		dg->ShowWindow(SW_SHOW);
		All_Move_dghWnd=dg->m_hWnd;
	}
	else if(Is_All_Move_Show==1)
	{
		Is_All_Move_Show=0;
		GetMenu()->CheckMenuItem(ID_All_Move,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
		::PostMessage(All_Move_dghWnd,WM_CLOSE, 0, 0);
	}
	// TODO: 在此添加命令处理程序代码CDialogDemo *dlg=new CDialogDemo(this); 
}

void CSau_EngineDlg::OnAbout()
{
	OnSysCommand(IDM_ABOUTBOX,0);
	// TODO: 在此添加命令处理程序代码
}

void CSau_EngineDlg::OnLoadSave()
{
	CFileDialog *mFileDlg=new CFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,"All Files (*.*)|*.*||");
	mFileDlg->DoModal();
	/*
	CString str(" ",10000);

	mFileDlg->lpstrFile=str.GetBuffer(10000);

	str.ReleaseBuffer();

	POSITION mPos=mFileDlg.GetStartPosition();

	CString pathName(" ",128);

	CFileStatus status;

	while(mPos!=NULL)

	{

	pathName=mFileDlg.GetNextPathName(mPos);

	CFile::GetStatus( pathName, status );

	}*/
	// TODO: 在此添加命令处理程序代码
}

void CSau_EngineDlg::OnEvel()
{
	if(Is_Evel_Show==0)
	{
		
		Is_Evel_Show=1;
		GetMenu()->CheckMenuItem(ID_Evel,MF_BYCOMMAND|MF_CHECKED);//标记
		CEvel_Dialog *dg=new CEvel_Dialog;
		dg->Create(IDD_DIALOG_EVEL);
		dg->ShowWindow(SW_SHOW);
		Evel_dghWnd=dg->m_hWnd;
	}
	else if(Is_Evel_Show==1)
	{
		Is_Evel_Show=0;
		GetMenu()->CheckMenuItem(ID_Evel,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
		::PostMessage(Evel_dghWnd,WM_CLOSE, 0, 0);
	}
	// TODO: 在此添加命令处理程序代码
}

void CSau_EngineDlg::OnMoveHistory()
{
	if(Is_Move_History_Show==0)
	{
		GetMenu()->CheckMenuItem(ID_Move_History,MF_BYCOMMAND|MF_CHECKED);//标记
		Is_Move_History_Show=1;
		if( Move_His_dghWnd != NULL && ::IsWindow(Move_His_dghWnd) )
		{
			::ShowWindow(Move_His_dghWnd,SW_SHOW);
		} 
		else
		{
			CMove_History_Dialog *dg=new CMove_History_Dialog;
			dg->Create(IDD_DIALOG_MOVE_HIS);
			dg->ShowWindow(SW_SHOW);
			Move_His_dghWnd=dg->m_hWnd;
		}
		
	/*	for(int i=0;i<=Step;i++)
		{
			MOVE TheMove=Load_File(Step,Round);
			::PostMessage(Move_His_dghWnd,UpdateMoveDataMess, 0, (LPARAM)&TheMove);
		}*/
	}
	else if(Is_Move_History_Show==1)
	{
		Is_Move_History_Show=0;
		GetMenu()->CheckMenuItem(ID_Move_History,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
		//::PostMessage(Move_His_dghWnd,WM_CLOSE, 0, 0);
		::ShowWindow(Move_His_dghWnd,SW_HIDE);
	}
	// TODO: 在此添加命令处理程序代码
}

void CSau_EngineDlg::Show_The_Move(MOVE* TheMove)
{
	::PostMessage(Move_His_dghWnd,UpdateMoveDataMess, Step, (LPARAM)TheMove);
		Save_File(*TheMove,Step,Round);

}

void CSau_EngineDlg::Show_All_Move()
{
	Move_Num= theApp.Move_Num;
	for(int i=0;i< Move_Num;i++)
		All_Moves[i]=theApp.Moves[i];
	::PostMessage(All_Move_dghWnd,UpdateMoveDataMess,Move_Num, (LPARAM)All_Moves);
}

void CSau_EngineDlg::Show_Evel()
{
	
	//Evel_Mess=MyGame.Get_Evel(Board,Color);
	::PostMessage(Evel_dghWnd,UpdateMoveDataMess,Step,(LPARAM)&theApp.Evel_Mess);
}

MOVE CSau_EngineDlg::Load_File(int Step, int Round)
{

	return MOVE();
}
///////////////////////////////////////////////
int finditpos(int star_x, int  star_y, int eatPointx, int  eatPointy)
{
	int  i = 0;
	for (i = 1; i < 9; i++)
	{
		if (eatPointx == star_x + i && eatPointy == star_y + i)
			return 1;
		if (eatPointx == star_x + i && eatPointy == star_y - i)
			return 2;
		if (eatPointx == star_x - i && eatPointy == star_y - i)
			return 3;
		if (eatPointx == star_x - i && eatPointy == star_y + i)
			return 4;
	}
	return 0;
}
int CSau_EngineDlg::newxy(int star_x, int star_y)
{
	if (Is_Board_Inverse == Flip)
	{
		return ((9 - star_x + (9 - star_y) * 10) / 2 + 1);
	}
	else
		return ((star_x + star_y * 10) / 2 + 1);
}
//void CSau_EngineDlg::Save_File(MOVE Move, int Step, int Round)
//{
	void CSau_EngineDlg::Save_File(MOVE Move, int Step, int Round)
	{
		CString sWrite;
		CString Temp_str;
		int newmovestartx = Move.move_star_x;
		int newmovestarty = Move.move_star_y;
		int eatpointx;
		int eatpointy;
		int nexteatpointx;
		int nexteatpointy;
		int result;
		int anotherresult1, anotherresult2, anotherresult3, anotherresult4;
		int pos = 0;


		if (Step == 1)
		{
			CTime Now_time = CTime::GetCurrentTime();
			sWrite = Now_time.Format(_T("\r\n时间%Y.%m.%d"));
			sWrite += Temp_str;
			if (Mode == AIvsH) { sWrite += "\r\n先手: "; }
			else { sWrite += "\r\n先手："; }
			sWrite += "\r\n先手方：";
			sWrite += "\r\n后手方：";
			sWrite += "\r\n结果：";
			sWrite += "\r\n";
		}
		if (turn % 2 != 0)
		{
			Temp_str.Format(_T("\r\n%d."), (turn / 2));
			sWrite += Temp_str;
		}


		if (Move.eatHowMany == 0)
		{
			if (newxy(Move.move_star_x, Move.move_star_y) >= 10)
			{
				Temp_str.Format(_T(" %2d-"), newxy(Move.move_star_x, Move.move_star_y));
			}
			else
			{
				Temp_str.Format(_T(" 0%d-"), newxy(Move.move_star_x, Move.move_star_y));
			}
			sWrite += Temp_str;
			if (newxy(Move.move_stop_x, Move.move_stop_y) >= 10)
			{
				Temp_str.Format(_T("%2d"), newxy(Move.move_stop_x, Move.move_stop_y));
			}
			else
			{
				Temp_str.Format(_T("0%d"), newxy(Move.move_stop_x, Move.move_stop_y));
			}
			sWrite += Temp_str;
		}
		else
		{
			if (Move.Kind <= 2)
			{
				for (int i = 0; i < Move.eatHowMany; i++)
				{

					if (i == 0)
					{
						if (newxy(Move.move_star_x, Move.move_star_y) >= 10)
						{
							Temp_str.Format(_T(" %2dx"), newxy(Move.move_star_x, Move.move_star_y));
						}
						else
						{
							Temp_str.Format(_T(" 0%dx"), newxy(Move.move_star_x, Move.move_star_y));
						}
						sWrite += Temp_str;

					}
					if (i != Move.eatHowMany - 1)
					{
						eatpointx = Move.eatPoint[i][0];
						eatpointy = Move.eatPoint[i][1];
						result = finditpos(newmovestartx, newmovestarty, eatpointx, eatpointy);
						if (result == 1)
						{
							newmovestartx = newmovestartx + 2;
							newmovestarty = newmovestarty + 2;

						}
						if (result == 2)
						{
							newmovestartx = newmovestartx + 2;
							newmovestarty = newmovestarty - 2;
						}
						if (result == 3)
						{
							newmovestartx = newmovestartx - 2;
							newmovestarty = newmovestarty - 2;
						}
						if (result == 4)
						{
							newmovestartx = newmovestartx - 2;
							newmovestarty = newmovestarty + 2;
						}
						if (result == 0)
						{
							newmovestartx = 0;
							newmovestarty = 0;
						}
						if (newxy(newmovestartx, newmovestarty) >= 10)
						{
							Temp_str.Format(_T("%2dx"), newxy(newmovestartx, newmovestarty));
						}
						else
						{
							Temp_str.Format(_T("0%dx"), newxy(newmovestartx, newmovestarty));
						}
						sWrite += Temp_str;

					}
					else
					{
						if (newxy(Move.move_stop_x, Move.move_stop_y) >= 10)
						{
							Temp_str.Format(_T("%2d"), newxy(Move.move_stop_x, Move.move_stop_y));
						}
						else
						{
							Temp_str.Format(_T("0%d"), newxy(Move.move_stop_x, Move.move_stop_y));
						}
						sWrite += Temp_str;
					}
				}

			}
			else
			{
				for (int i = 0; i < Move.eatHowMany; i++)
				{
					if (i == 0)
					{
						if (newxy(Move.move_star_x, Move.move_star_y) >= 10)
						{
							Temp_str.Format(_T(" %2dx"), newxy(Move.move_star_x, Move.move_star_y));
						}
						else
						{
							Temp_str.Format(_T(" 0%dx"), newxy(Move.move_star_x, Move.move_star_y));
						}
						sWrite += Temp_str;
					}
					if (i != Move.eatHowMany - 1)
					{
						eatpointx = Move.eatPoint[i][0];
						eatpointy = Move.eatPoint[i][1];
						nexteatpointx = Move.eatPoint[i + 1][0];
						nexteatpointy = Move.eatPoint[i + 1][1];
						result = finditpos(newmovestartx, newmovestarty, eatpointx, eatpointy);
						for (pos = 1; pos < 9; pos++)
						{
							anotherresult1 = finditpos(eatpointx + pos, eatpointy + pos, nexteatpointx, nexteatpointy);
							anotherresult2 = finditpos(eatpointx - pos, eatpointy - pos, nexteatpointx, nexteatpointy);
							anotherresult3 = finditpos(eatpointx + pos, eatpointy - pos, nexteatpointx, nexteatpointy);
							anotherresult4 = finditpos(eatpointx - pos, eatpointy + pos, nexteatpointx, nexteatpointy);
							if (anotherresult1)
							{
								if (result == 1)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy + pos;

								}
								if (result == 2)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 3)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 4)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy + pos;
								}
								break;

							}
							if (anotherresult2)
							{
								if (result == 1)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy + pos;

								}
								if (result == 2)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 3)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 4)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy + pos;
								}
								break;


							}
							if (anotherresult3)
							{
								if (result == 1)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy + pos;

								}
								if (result == 2)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 3)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 4)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy + pos;
								}
								break;


							}
							if (anotherresult4)
							{
								if (result == 1)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy + pos;

								}
								if (result == 2)
								{
									newmovestartx = eatpointx + pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 3)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy - pos;
								}
								if (result == 4)
								{
									newmovestartx = eatpointx - pos;
									newmovestarty = eatpointy + pos;
								}
								break;

							}

						}
						if (newxy(newmovestartx, newmovestarty) >= 10)
						{
							Temp_str.Format(_T("%2dx"), newxy(newmovestartx, newmovestarty));
						}
						else
						{
							Temp_str.Format(_T("0%dx"), newxy(newmovestartx, newmovestarty));
						}
						sWrite += Temp_str;
					}
					else
					{
						if (newxy(Move.move_stop_x, Move.move_stop_y) >= 10)
						{
							Temp_str.Format(_T("%2d"), newxy(Move.move_stop_x, Move.move_stop_y));
						}
						else
						{
							Temp_str.Format(_T("0%d"), newxy(Move.move_stop_x, Move.move_stop_y));
						}
						sWrite += Temp_str;
					}
				}

			}

		}





		turn++;
		Temp_str.Format(_T(""));
		sWrite += Temp_str;
		CFile mFile(_T(File_Path), CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate);
		mFile.SeekToEnd();
		mFile.Write(sWrite, sWrite.GetLength());
		mFile.Flush();
		mFile.Close();
	//}

	
	/*CString sWrite;
	CString Temp_str;
	int newmovestartx = Move.move_star_x;
	int newmovestarty = Move.move_star_y;
	int eatpointx;
	int eatpointy;
	int nexteatpointx;
	int nexteatpointy;
	int result;
	int anotherresult1, anotherresult2, anotherresult3, anotherresult4;
	int pos = 0;
	int turn = 3;
	if (Step == 1)
	{
		CTime Now_time = CTime::GetCurrentTime();
		sWrite = Now_time.Format(_T("\r\n\r\n时间：%Y.%m.%d"));
		sWrite += Temp_str;
		if (Mode == AIvsH) { sWrite += "\r\n先手：1"; }
		else { sWrite += "\r\n先手：2"; }
		sWrite += "\r\n先手方：";
		sWrite += "\r\n后手方：";
		sWrite += "\r\n结果：";
		sWrite += "\r\n";
	}
	if (turn % 2 != 0)
	{
		Temp_str.Format(_T("\r\n%d."), (turn / 2));
		sWrite += Temp_str;
	}


	if (Move.eatHowMany == 0)
	{
		if ((Move.move_star_x + Move.move_star_y * 8) / 2 + 1 >= 10)
		{
			Temp_str.Format(_T(" %2d-"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
		}
		else
		{
			Temp_str.Format(_T(" 0%d-"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
		}
		sWrite += Temp_str;
		if ((Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1 >= 10)
		{
			Temp_str.Format(_T("%2d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
		}
		else
		{
			Temp_str.Format(_T("0%d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
		}
		sWrite += Temp_str;
	}
	else
	{
		if (Move.Kind <= 2)
		{
			for (int i = 0; i < Move.eatHowMany; i++)
			{

				if (i == 0)
				{
					if ((Move.move_star_x + Move.move_star_y * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T(" %2dx"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T(" 0%dx"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
					}
					sWrite += Temp_str;

				}
				if (i != Move.eatHowMany - 1)
				{
					eatpointx = Move.eatPoint[i][0];
					eatpointy = Move.eatPoint[i][1];
					result = finditpos(newmovestartx, newmovestarty, eatpointx, eatpointy);
					if (result == 1)
					{
						newmovestartx = newmovestartx + 2;
						newmovestarty = newmovestarty + 2;

					}
					if (result == 2)
					{
						newmovestartx = newmovestartx + 2;
						newmovestarty = newmovestarty - 2;
					}
					if (result == 3)
					{
						newmovestartx = newmovestartx - 2;
						newmovestarty = newmovestarty - 2;
					}
					if (result == 4)
					{
						newmovestartx = newmovestartx - 2;
						newmovestarty = newmovestarty + 2;
					}
					if (result == 0)
					{
						newmovestartx = 0;
						newmovestarty = 0;
					}
					if ((newmovestartx + newmovestarty * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T("%2dx"), (newmovestartx + newmovestarty * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T("0%dx"), (newmovestartx + newmovestarty * 8) / 2 + 1);
					}
					sWrite += Temp_str;

				}
				else
				{
					if ((Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T("%2d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T("0%d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
					}
					sWrite += Temp_str;
				}
			}

		}
		else
		{
			for (int i = 0; i < Move.eatHowMany; i++)
			{
				if (i == 0)
				{
					if ((Move.move_star_x + Move.move_star_y * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T(" %2dx"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T(" 0%dx"), (Move.move_star_x + Move.move_star_y * 8) / 2 + 1);
					}
					sWrite += Temp_str;
				}
				if (i != Move.eatHowMany - 1)
				{
					eatpointx = Move.eatPoint[i][0];
					eatpointy = Move.eatPoint[i][1];
					nexteatpointx = Move.eatPoint[i + 1][0];
					nexteatpointy = Move.eatPoint[i + 1][1];
					result = finditpos(newmovestartx, newmovestarty, eatpointx, eatpointy);
					for (pos = 1; pos < 7; pos++)
					{
						anotherresult1 = finditpos(eatpointx + pos, eatpointy + pos, nexteatpointx, nexteatpointy);
						anotherresult2 = finditpos(eatpointx - pos, eatpointy - pos, nexteatpointx, nexteatpointy);
						anotherresult3 = finditpos(eatpointx + pos, eatpointy - pos, nexteatpointx, nexteatpointy);
						anotherresult4 = finditpos(eatpointx - pos, eatpointy + pos, nexteatpointx, nexteatpointy);
						if (anotherresult1)
						{
							if (result == 1)
							{
								newmovestartx = eatpointx + pos;
								newmovestarty = eatpointy + pos;

							}
							break;

						}
						if (anotherresult2)
						{
							if (result == 2)
							{
								newmovestartx = eatpointx + pos;
								newmovestarty = eatpointy - pos;
							}
							break;


						}
						if (anotherresult3)
						{
							if (result == 3)
							{
								newmovestartx = eatpointx - pos;
								newmovestarty = eatpointy - pos;
							}
							break;


						}
						if (anotherresult4)
						{
							if (result == 4)
							{
								newmovestartx = eatpointx - pos;
								newmovestarty = eatpointy + pos;
							}
							break;

						}

					}
					if ((newmovestartx + newmovestarty * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T("%2dx"), (newmovestartx + newmovestarty * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T("%0dx"), (newmovestartx + newmovestarty * 8) / 2 + 1);
					}
					sWrite += Temp_str;
				}
				else
				{
					if ((Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1 >= 10)
					{
						Temp_str.Format(_T("%2d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
					}
					else
					{
						Temp_str.Format(_T("0%d"), (Move.move_stop_x + Move.move_stop_y * 8) / 2 + 1);
					}
					sWrite += Temp_str;
				}
			}

		}

	}





	turn++;
	Temp_str.Format(_T(""));
	sWrite += Temp_str;
	CFile mFile(_T(File_Path), CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate);
	mFile.SeekToEnd();
	mFile.Write(sWrite, sWrite.GetLength());
	mFile.Flush();
	mFile.Close();*/
}
///////////////////////////////////////////////
/////////////////////////////估值存入？
//void CSau_EngineDlg::Save_File(MOVE Move,int Step, int Round)
//{
//	CString sWrite;
//	CString Temp_str;
//	if(Step==1)
//	{
//		CTime Now_time=CTime::GetCurrentTime();
//		sWrite=Now_time.Format(_T("\r\n\r\n\r\n%Y年%m月%d日 %H时%M分%S秒"));
//		if (Mode == AIvsH) { sWrite += "\r\n先手: 1"; }
//		else { sWrite += "\r\n先手: 2"; }
//		//Temp_str.Format(_T("\r\n第%d场\r\n"),Round);
//		//sWrite+=Temp_str;
//		//if(Mode==HvsH){sWrite+="模式:人人";}
//		//else if(Mode==HvsAI){sWrite+="模式:人机";}
//		//else if(Mode==AIvsH){sWrite+="模式:机人";}
//		//if(Engine&Negascout){sWrite+="  引擎:Negascout";}
//		//if(Engine&AlphaBeta){sWrite+="|AlphaBeta";}
//		//Temp_str.Format(_T("\r\nAlphaBeta层数%d，Negascout时间%d\r\n"),Floor,Time);
//		//sWrite+=Temp_str;
//		//sWrite+=("\r\n步数	起点	落点	吃子数	吃子\r\n");
//		sWrite += ("\r\n先手方：\r\n后手方：\r\n结果:\r\n");
//		//if(Mode==HvsH){sWrite+="模式:人人";}
//	}
//	//int sss = 2 * Step;
//	Temp_str.Format(_T("\r\n%2d"),Step);//步数
//	//Temp_str.Format(_T("\r\n%2d"), sss);//回合
//	sWrite+=Temp_str;
//	//if((Move.move_star_x + Move.move_star_y * 10) / 2 + 1)<=9|| Move.move_star_y<=9|| Temp_str.Format(_T("%2d"),(Move.move_stop_x+Move.move_stop_y*10)/2+1)<=9)
//	//Temp_str.Format(_T(". 0%2d-"), (Move.move_star_x + Move.move_star_y * 10) / 2 + 1);//起点
//	if ((Move.move_star_x + 1 == Move.move_stop_x) || (Move.move_star_x - 1 == Move.move_stop_x))
//	{
//		/*if ((Move.move_star_x + Move.move_star_y * 10) / 2 + 1)<=9)
//		{
//			Temp_str.Format(_T(". 0%2d-"), (Move.move_star_x + Move.move_star_y * 10) / 2 + 1);
//		}
//		else
//		{*/
//			Temp_str.Format(_T(". %2d-"), (Move.move_star_x + Move.move_star_y * 10) / 2 + 1);//起点
//		//}
//		sWrite += Temp_str;
//	}
//	else
//	{
//		Temp_str.Format(_T(". %2dx"), (Move.move_star_x + Move.move_star_y * 10) / 2 + 1);//起点
//		sWrite += Temp_str;
//	}
//	Temp_str.Format(_T("%2d"),(Move.move_stop_x+Move.move_stop_y*10)/2+1);//终点
//	sWrite+=Temp_str;
//	//Temp_str.Format(_T("	%2d "),Move.eatHowMany);
//	//sWrite+=Temp_str;
//
//	//for(int i=0;i<Move.eatHowMany;i++)
//	//{
//	//	Temp_str.Format(_T("	%2d"),(Move.eatPoint[i][0]+Move.eatPoint[i][1]*10)/2+1);
//	//	sWrite+=Temp_str;
//	//
//	//}
//	Temp_str.Format(_T(""));
//	sWrite+=Temp_str;
//	CFile mFile(_T(File_Path), CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate);
//	mFile.SeekToEnd();
//	mFile.Write(sWrite,sWrite.GetLength());
//	mFile.Flush();
//	mFile.Close();
//}

void CSau_EngineDlg::Save_File(int Depth,double Time)
{
	CString Temp_str;
	Temp_str.Format(_T("\r\n搜索深度已更改！\r\nAlphaBeta层数%d，Negascout时间%.2f"),Depth,Time);
	CFile mFile(_T(File_Path), CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate);
	mFile.SeekToEnd();
	mFile.Write(Temp_str,Temp_str.GetLength());
	mFile.Flush();
	mFile.Close();
}

void CSau_EngineDlg::Save_File(int Color)
{
	CString Temp_str;
	if( Color==BLACK)
		Temp_str=_T("\r\n终局！白方胜！");
	else if(Color==WHITE)
		Temp_str=_T("\r\n终局！黑方胜！");
	CFile mFile(_T(File_Path), CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate);
	mFile.SeekToEnd();
	mFile.Write(Temp_str,Temp_str.GetLength());
	mFile.Flush();
	mFile.Close();
}

void CSau_EngineDlg::OnFloor1()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_1,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=1;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::OnFloor3()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_3,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=3;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::OnFloor5()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_5,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=5;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::OnFloor7()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_7,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=7;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::OnFloor9()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_9,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=9;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::OnFloor11()
{
	// TODO: 在此添加命令处理程序代码
	int ID;
	if(Floor==1)ID=ID_Floor_1;
	else if(Floor==3)ID=ID_Floor_3;
	else if(Floor==5)ID=ID_Floor_5;
	else if(Floor==7)ID=ID_Floor_7;
	else if(Floor==9)ID=ID_Floor_9;
	else if(Floor==11)ID=ID_Floor_11;
	GetMenu()->CheckMenuItem(ID,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
	GetMenu()->CheckMenuItem(ID_Floor_11,MF_BYCOMMAND|MF_CHECKED);//标记
	Floor=11;
	Input_Depth_Time(Floor,Time);
}

void CSau_EngineDlg::Input_Depth_Time(int Depth,double Time)
{
	theApp.Board_Mess.Add_Mess=Input;
	theApp.Floor=Depth;
	theApp.Time=Time;
	//恢复线程
	Save_File(Depth,Time);
	theApp.Handle_AI_Thread->ResumeThread();
}

void CSau_EngineDlg::Get_All_Move()
{
	theApp.Board_Mess.Add_Mess=GetAllMove;
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
		{
			theApp.Board_Mess.Board[i][j]=Board[i][j];
		}
	theApp.Board_Mess.Color=Color;
	theApp.Handle_AI_Thread->ResumeThread();
}


UINT AI_Thread(LPVOID pParam) 
 { 
     CWnd *pMainWnd = AfxGetMainWnd(); 
	 Game M_Game;
	while(1)
	{
		theApp.Thread_Sign=1;
		M_Game.Error=0;//错误代码清零
		
		if(theApp.Board_Mess.Add_Mess==Dele)//-1时对局结束释放线程
		{ 
			theApp.Thread_Sign=0;
			AfxEndThread(TRUE);
		}
		else if(theApp.Board_Mess.Add_Mess==Input)//-//给搜索时间层数赋值
		{
			M_Game.Input_Depth_Time(theApp.Floor,theApp.Time);
			::PostMessage(pMainWnd->m_hWnd, Input_Depth_Time_Finish,0,0);
			theApp.Handle_AI_Thread->SuspendThread();
		}
		else if(theApp.Board_Mess.Add_Mess==GetAllMove)//
		{
			for(int i=0;i< theApp.Move_Num;i++)
			{
				theApp.Moves[i].move_star_x=-1;
				theApp.Moves[i].move_star_y=-1;
				theApp.Moves[i].move_stop_x=-1;
				theApp.Moves[i].move_stop_y=-1;
				for(int j=0;j< theApp.Moves[i].eatHowMany;j++)
				{
					theApp.Moves[i].eatPoint[j][0]=-1;
					theApp.Moves[i].eatPoint[j][1]=-1;
				}
			}

			theApp.Move_Num=M_Game.Search_All_Moves(theApp.Board_Mess.Board,theApp.Moves,theApp.Board_Mess.Color);
			theApp.Evel_Mess=M_Game.Get_Evel(theApp.Board_Mess.Board,theApp.Board_Mess.Color);
			if(theApp.Move_Num==0)
				::PostMessage(pMainWnd->m_hWnd,WM_MY_MESSAGE,theApp.Board_Mess.Color,0);
			else
				::PostMessage(pMainWnd->m_hWnd,WM_MY_MESSAGE,0,All_Move_Finish);
			theApp.Handle_AI_Thread->SuspendThread();
		}
		else if( (theApp.Board_Mess.Add_Mess&Negascout||theApp.Board_Mess.Add_Mess&AlphaBeta ||theApp.Board_Mess.Add_Mess&Minimax)&&(theApp.Board_Mess.Color==BLACK||theApp.Board_Mess.Color==WHITE) )//下棋方&&引擎信息校验
		{
			//AI给出着法
			theApp.Move_Mess.The_Good_Move=M_Game.Search_Good_Move(theApp.Board_Mess.Board, theApp.Board_Mess.Color,theApp.Board_Mess.Add_Mess, StepCount);
			theApp.Move_Mess.Add_Mess=M_Game.Error;
			//完成后发送着法完成消息
			::PostMessage(pMainWnd->m_hWnd, WM_MY_MESSAGE, 0,0);//::PostThreadMessage(theApp.m_nThreadID,WM_MY_MESSAGE, 0, 0);
			//挂起线程等待下次调用
			theApp.Thread_Sign=2;
			theApp.Handle_AI_Thread->SuspendThread();
		}
		else
		{
			::MessageBox(pMainWnd->m_hWnd, "AI获取信息错误！", _T("AI Thread Message" ), MB_OK);
			theApp.Thread_Sign=2;
			theApp.Handle_AI_Thread->SuspendThread();
		}
	}
     return  0 ; 
} 


void CSau_EngineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Pro.StepIt();
	if(m_Pro.GetPos()==100)
		KillTimer(TIMER);
	CDialogEx::OnTimer(nIDEvent);
}

void CSau_EngineDlg::ProgressStar()
{
	if(Move_Num!=1)
		SetTimer(TIMER,Time*40,0);

}

afx_msg LRESULT CSau_EngineDlg::OnUpdateTree(WPARAM wParam, LPARAM lParam)
{
	
	if( wParam==-1&&Engine&Negascout)
	{
		m_Bar.SetPaneText(2,"Negascout引擎出错！正在使用AlphaBeta引擎。。。",1);
		Temp_Str.Format("节点数：%dK",lParam/1000);
		m_Bar.SetPaneText(3,Temp_Str,1);
	}
	else if( wParam==-1)
	{
		m_Bar.SetPaneText(2,"正在使用AlphaBeta引擎。。。",1); 
		Temp_Str.Format("节点数：%dK",lParam/1000);
		m_Bar.SetPaneText(3,Temp_Str,1); 
	}
	else
	{
		m_Bar.SetPaneText(2,"正在使用Negascout引擎。。。",1); 
		Temp_Str.Format("层数：%d，节点数：%dK",wParam,lParam/1000);
		m_Bar.SetPaneText(3,Temp_Str,1); 
	}
	return 0;
}

void CSau_EngineDlg::Go_The_Move(MOVE The_Move)
{
	int nowX=The_Move.move_star_x;
	int nowY=The_Move.move_star_y;
	int stopX=The_Move.move_stop_x;
	int stopY=The_Move.move_stop_y;
	int record=Board[nowX][nowY];
	Board[nowX][nowY]=EMPTY;
	if(The_Move.eatHowMany>0)
	{
		for(int i=0;i<The_Move.eatHowMany;i++)
		{
			Board[The_Move.eatPoint[i][0]][The_Move.eatPoint[i][1]]=EMPTY;
		}
	}
	Board[stopX][stopY]=record;
	if(stopY==0&&record==WHITE)Board[stopX][stopY]=W_KING;
	if(stopY==9&&record==BLACK)Board[stopX][stopY]=B_KING;
}

/*void CSau_EngineDlg::Go_The_Move(MOVE The_Move)
{
	int nowX=The_Move.move_star_x;
	int nowY=The_Move.move_star_y;
	int stopX=The_Move.move_stop_x;
	int stopY=The_Move.move_stop_y;
	int record=Board[nowX][nowY];
	Board[nowX][nowY]=EMPTY;
	if(The_Move.eatHowMany>0)
	{
		for(int i=0;i<The_Move.eatHowMany;i++)
		{
			Board[The_Move.eatPoint[i][0]][The_Move.eatPoint[i][1]]=EMPTY;
		}
	}
	Board[stopX][stopY]=record;
	if(stopY==0&&record==WHITE)Board[stopX][stopY]=W_KING;
	if(stopY==9&&record==BLACK)Board[stopX][stopY]=B_KING;
	}*/

void CSau_EngineDlg::OnBoardUserDefined()
{
	// TODO: 在此添加命令处理程序代码
	if(Is_Set_Board==0)
	{
		GetMenu()->CheckMenuItem(ID_Board_User_Defined,MF_BYCOMMAND|MF_CHECKED);//标记
		Is_Set_Board=1;
	}
	else
	{
		GetMenu()->CheckMenuItem(ID_Board_User_Defined,MF_BYCOMMAND|MF_UNCHECKED);//取消标记
		Is_Set_Board=0;
		Get_All_Move();
	}
	
}

void CSau_EngineDlg::Set_Board(CPoint pt)
{
	Board[pt.x][pt.y]=Board[pt.x][pt.y]+1;
	if(Board[pt.x][pt.y]>=5)
		Board[pt.x][pt.y]=0;
	DrawChess(D_Board|D_Pieces|D_Prompt);

}


void CSau_EngineDlg::On32847()
{
	// TODO: 在此添加命令处理程序代码
	
	if(congli==0)
		congli=999;
	else
		congli=0;

}
