//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//亚马逊棋组负责人王富帅所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//王富帅	QQ：963356506
/*   黑棋总是先手，且黑棋总在棋盘的下方。
       评估的时候是针对黑方进行评估，即站在黑方角度评估*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"Data.h"
#include"EvaluateEngine.h"
#include"AlphaBeta.h"
#include<graphics.h>


int Board[12][12] = { 0 };
int bufa[3][2] = { 0 };
int mode = 0;     //模式的初始化

/////////主函数/////////
void main()
{
	//int mode;
	int firsthand = 0;//规定先手为黑，即黑棋先行，用0表示
	initgraph(1000,700);
	InitBoard(Board);
	Show(Board);
	mode = Mode();   //选择游戏模式
	if (mode<0)//人人模式
	{
		MessageBox(NULL,"黑棋先行","提示",MB_OK);
		while (!JudgeWin(Board))
		{
			Show(Board);
			Move(firsthand,Board,bufa);
			firsthand=abs(firsthand - 1);//先后手转换
		}
	}
	if (mode >= 0)//人机模式
	{
		if (mode == 0)//人类先手
		{
			firsthand = 1;
			MessageBox(NULL, "人类先行", "提示", MB_OK);
			while (!JudgeWin(Board))
			{
				Move(firsthand, Board, bufa);
				Show(Board);
				//Move(firsthand, Board,bufa);
				SearchAGoodMove(Board);
				//Show(Board);
				//SearchAGoodMove(Board);
				//Show(Board);
			}
		}
		if (mode == 1)//电脑先手
		{
			firsthand = 1;    //将人类下的棋子转换为白棋，即人类操作方
			MessageBox(NULL, "电脑先行", "提示", MB_OK);
			while (!JudgeWin(Board))
			{
				SearchAGoodMove(Board);
				Show(Board);
				Move(firsthand, Board,bufa);
			}
		}
	}
	if (JudgeWin(Board) == BLACKCHESS)
		MessageBox(NULL,"白棋赢！","提示",MB_OK);
	if (JudgeWin(Board) == WHITECHESS)
		MessageBox(NULL, "黑棋赢！", "提示", MB_OK);
	getchar();
	closegraph();
}  


////////////棋盘数据结构的初始化///////////
void InitBoard(int Board[12][12])
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i == 0 || j == 0 || i == 11 || j == 11)
				Board[i][j] = BARRIER;
			else
				Board[i][j] = EMPTY;
		}
	}
	Board[1][4] = WHITECHESS;
	Board[1][7] = WHITECHESS;
	Board[4][1] = WHITECHESS;
	Board[4][10] = WHITECHESS;
	Board[7][1] = BLACKCHESS;
	Board[7][10] = BLACKCHESS;
	Board[10][4] = BLACKCHESS;
	Board[10][7] = BLACKCHESS;


}
///////////UI界面///////////////
void Show(int Board[12][12])
{
	IMAGE img[4];
	IMAGE bg;
	loadimage(&img[0], "0.jpg",65,65);
	loadimage(&img[1], "1.jpg", 65, 65);
	loadimage(&img[2], "boom.jpg", 65, 65);
	loadimage(&img[3], "blank.jpg", 65, 65);
	loadimage(&bg, "background.jpg",1000,700);
	putimage(0, 0, &bg);
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			if (Board[i][j] == BLACKCHESS)
				putimage((j-1) * 65, (i-1) * 65, &img[0]);
			else if (Board[i][j] == WHITECHESS)
				putimage((j-1)  * 65, (i-1) * 65, &img[1]);
			else if (Board[i][j]==BARRIER)
				putimage((j-1) * 65, (i-1) * 65, &img[2]);
			else
				putimage((j-1) * 65, (i-1) * 65, &img[3]);
		}
	}

	setlinecolor(RGB(123,15,211));
	setlinestyle(0,4);
	rectangle(700, 65, 910, 130);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(211, 107, 145));
	settextstyle(40, 20, "隶书");
	outtextxy(700, 80, "人人模式");

	rectangle(700, 200, 950, 265);
	settextstyle(50, 14, "隶书");
	outtextxy(700, 205, "人机模式 人类先手");
	rectangle(700, 265, 950, 330);
	outtextxy(700, 270, "人机模式 电脑先手");

	rectangle(700,350,800,450);
	settextstyle(50,25,"隶书");
	outtextxy(700, 370, "悔棋");
}
//////////使行棋合法化//////////
int LegalMove(int x1, int y1, int x2, int y2, int Board[12][12])//行棋合法化
{
	//(x1,y1)是起始点，（x2,y2）是终点
	int a, b, m, n;
	a = x1;
	b = y1;
	m = x2;
	n = y2;
	if (abs(y2 - y1) == abs(x2 - x1) || x2 - x1 == 0 || y2 - y1 == 0)//确保起始点和落子点符合米字型路线
	{
		//以下代码是棋子八个方向的路径上判断是否有不为空的地方
		if (y2 - y1 > 0 && x2 - x1 == 0)
		{
			for (b = b + 1; b < n; b++)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 > 0 && x2 - x1 > 0)
		{
			for (a = a + 1, b = b + 1; a < m, b < n; a++, b++)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 == 0 && x2 - x1 > 0)
		{
			for (a = a + 1; a < m; a++)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 < 0 && x2 - x1>0)
		{
			for (a = a + 1, b = b - 1; a<m, b>n; a++, b--)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 < 0 && x2 - x1 == 0)
		{
			for (b = b - 1; b > n; b--)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 < 0 && x2 - x1 < 0)
		{
			for (a = a - 1, b = b - 1; a > m, b > n; a--, b--)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1 == 0 && x2 - x1 < 0)
		{
			for (a = a - 1; a > m; a--)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else if (y2 - y1>0 && x2 - x1 < 0)
		{
			for (a = a - 1, b = b + 1; a > m, b < n; a--, b++)
				if (Board[a][b] != EMPTY)
					return 0;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}
/////////移动棋子and悔棋//////////
//该函数传入参数firsthand
int Move(int first,int Board[12][12],int bufa[3][2])
{
	int n = 1;        //使用变量n是为了能退出循环
	int times = 1;    //鼠标点击次数
	int qx, qy, lx, ly, bx, by;    //起始，落点，障碍的坐标
	MOUSEMSG msg;
	while (n)
	{
	loop:msg = GetMouseMsg();   ///获取鼠标消息
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (msg.x >= 0 && msg.x <= 650 && msg.y >= 0 && msg.y <= 650)
			{
				if (times % 3 == 1)    //第一次点击，即选择起点的棋
				{
					//保存起始点棋子坐标
					qx = msg.y / 65 + 1;
					qy = msg.x / 65 + 1;
					if (first == 0 && Board[qx][qy] == BLACKCHESS)
					{
						times++;
						bufa[0][0] = qx;
						bufa[0][1] = qy;
						goto loop;
					}
					else if (first == 1 && Board[qx][qy] == WHITECHESS)
					{
						times++;
						bufa[0][0] = qx;
						bufa[0][1] = qy;
						goto loop;
					}
					else
					{
						MessageBox(NULL, "不符合规则，请重新选择棋子,", "提示", MB_OK);
						goto loop;
					}
				}
				if (times % 3 == 2)//第二次点击，选择落子点
				{
					//保存落子点
					lx = msg.y / 65 + 1;
					ly = msg.x / 65 + 1;
					if (first == 0 && Board[lx][ly] == EMPTY&&LegalMove(qx, qy, lx, ly, Board))
					{
						//并且修改棋盘数据
						Board[qx][qy] = EMPTY;
						Board[lx][ly] = BLACKCHESS;
						Show(Board);
						times++;
						bufa[1][0] = lx;
						bufa[1][1] = ly;
						goto loop;
					}
					else if (first == 1 && Board[lx][ly] == EMPTY&&LegalMove(qx, qy, lx, ly, Board))
					{
						//并且修改棋盘数据
						Board[qx][qy] = EMPTY;
						Board[lx][ly] = WHITECHESS;
						Show(Board);
						times++;
						bufa[1][0] = lx;
						bufa[1][1] = ly;
						goto loop;
					}
					else
					{
						MessageBox(NULL, "不符合规则，请重新选落子点,", "提示", MB_OK);
						goto loop;
					}
				}
				if (times % 3 == 0)//第三次点击，防置障碍
				{
					bx = msg.y / 65 + 1;
					by = msg.x / 65 + 1;
					if (Board[bx][by] == EMPTY&&LegalMove(lx, ly, bx, by, Board))
					{
						Board[bx][by] = BARRIER;
						Show(Board);
						times++;
						bufa[2][0] = bx;
						bufa[2][1] = by;
						n = 0;
						return 1;
					}
					else
					{
						MessageBox(NULL, "不符合规则，请重新防置障碍,", "提示", MB_OK);
						goto loop;
					}
				}
			}
			////////////////////////悔棋部分////////////////////////////
			if (msg.x >= 700 && msg.x <= 800 && msg.y >= 350 && msg.y <= 450)
			{
				if (first == 0)//轮到黑棋走，上一步悔白棋
				{
					Board[bufa[2][0]][bufa[2][1]] = EMPTY;
					Board[bufa[1][0]][bufa[1][1]] = EMPTY;
					Board[bufa[0][0]][bufa[0][1]] = WHITECHESS;
					n = 0;
					return first;
				}
				if (first == 1)//轮到白棋走，上一步悔黑棋
				{
					Board[bufa[2][0]][bufa[2][1]] = EMPTY;
					Board[bufa[1][0]][bufa[1][1]] = EMPTY;
					Board[bufa[0][0]][bufa[0][1]] = BLACKCHESS;
					n = 0;
					return first;
				}
			}
			break;
		default:
			break;
		}
	}
}
//////////胜负判断////////////
int JudgeWin(int Board[12][12])//胜负判断
{
	int C_Black = 0, C_White = 0;
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 10; j++)
		{
			if (Board[i][j] == BLACKCHESS&&Board[i - 1][j - 1] != EMPTY&&Board[i - 1][j] != EMPTY&&Board[i - 1][j + 1] != EMPTY&&Board[i][j + 1] != EMPTY&&Board[i + 1][j + 1] != EMPTY&&Board[i + 1][j] != EMPTY&&Board[i + 1][j - 1] != EMPTY&&Board[i][j - 1] != EMPTY)
				C_Black++;
			else if (Board[i][j] == WHITECHESS&&Board[i - 1][j - 1] != EMPTY&&Board[i - 1][j] != EMPTY&&Board[i - 1][j + 1] != EMPTY&&Board[i][j + 1] != EMPTY&&Board[i + 1][j + 1] != EMPTY&&Board[i + 1][j] != EMPTY&&Board[i + 1][j - 1] != EMPTY&&Board[i][j - 1] != EMPTY)
				C_White++;
		}
	}
	if (C_Black == 4)
		return BLACKCHESS;   //黑棋全死
	else if (C_White == 4)
		return WHITECHESS;  //白棋全死
	else
		return 0;//尚未分出胜负
}
//////////模式选择////////////
int Mode()//模式选择——人人，人机模式
{
	MOUSEMSG msg;
	int n = 1;
	while (n)
	{
		loop1:msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (msg.x >= 700 && msg.x <= 910 && msg.y >= 65 && msg.y <= 130)
			{
				n = 0;
				return -1;//人人模式
			}
			else if (msg.x >= 700 && msg.x <= 950 && msg.y >= 200 && msg.y <= 265)
			{
				n = 0;
				return 0;//人机模式 人类先手
			}
			else if (msg.x >= 700 && msg.x <= 950 && msg.y >= 265 && msg.y <= 330)
			{
				n = 0;
				return 1;//人机模式 电脑先手
			}
			else
			{
				MessageBox(NULL,"请选择游戏模式","提示",MB_OK);
				goto loop1;
			}
			break;
		default:
			break;
		}
	}
}
