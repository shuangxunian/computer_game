//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//亚马逊棋组负责人王富帅所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//王富帅	QQ：963356506
/*   �����������֣��Һ����������̵��·���
       ������ʱ������Ժڷ�������������վ�ںڷ��Ƕ�����*/

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
int mode = 0;     //ģʽ�ĳ�ʼ��

/////////������/////////
void main()
{
	//int mode;
	int firsthand = 0;//�涨����Ϊ�ڣ����������У���0��ʾ
	initgraph(1000,700);
	InitBoard(Board);
	Show(Board);
	mode = Mode();   //ѡ����Ϸģʽ
	if (mode<0)//����ģʽ
	{
		MessageBox(NULL,"��������","��ʾ",MB_OK);
		while (!JudgeWin(Board))
		{
			Show(Board);
			Move(firsthand,Board,bufa);
			firsthand=abs(firsthand - 1);//�Ⱥ���ת��
		}
	}
	if (mode >= 0)//�˻�ģʽ
	{
		if (mode == 0)//��������
		{
			firsthand = 1;
			MessageBox(NULL, "��������", "��ʾ", MB_OK);
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
		if (mode == 1)//��������
		{
			firsthand = 1;    //�������µ�����ת��Ϊ���壬�����������
			MessageBox(NULL, "��������", "��ʾ", MB_OK);
			while (!JudgeWin(Board))
			{
				SearchAGoodMove(Board);
				Show(Board);
				Move(firsthand, Board,bufa);
			}
		}
	}
	if (JudgeWin(Board) == BLACKCHESS)
		MessageBox(NULL,"����Ӯ��","��ʾ",MB_OK);
	if (JudgeWin(Board) == WHITECHESS)
		MessageBox(NULL, "����Ӯ��", "��ʾ", MB_OK);
	getchar();
	closegraph();
}  


////////////�������ݽṹ�ĳ�ʼ��///////////
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
///////////UI����///////////////
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
	settextstyle(40, 20, "����");
	outtextxy(700, 80, "����ģʽ");

	rectangle(700, 200, 950, 265);
	settextstyle(50, 14, "����");
	outtextxy(700, 205, "�˻�ģʽ ��������");
	rectangle(700, 265, 950, 330);
	outtextxy(700, 270, "�˻�ģʽ ��������");

	rectangle(700,350,800,450);
	settextstyle(50,25,"����");
	outtextxy(700, 370, "����");
}
//////////ʹ����Ϸ���//////////
int LegalMove(int x1, int y1, int x2, int y2, int Board[12][12])//����Ϸ���
{
	//(x1,y1)����ʼ�㣬��x2,y2�����յ�
	int a, b, m, n;
	a = x1;
	b = y1;
	m = x2;
	n = y2;
	if (abs(y2 - y1) == abs(x2 - x1) || x2 - x1 == 0 || y2 - y1 == 0)//ȷ����ʼ������ӵ����������·��
	{
		//���´��������Ӱ˸������·�����ж��Ƿ��в�Ϊ�յĵط�
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
/////////�ƶ�����and����//////////
//�ú����������firsthand
int Move(int first,int Board[12][12],int bufa[3][2])
{
	int n = 1;        //ʹ�ñ���n��Ϊ�����˳�ѭ��
	int times = 1;    //���������
	int qx, qy, lx, ly, bx, by;    //��ʼ����㣬�ϰ�������
	MOUSEMSG msg;
	while (n)
	{
	loop:msg = GetMouseMsg();   ///��ȡ�����Ϣ
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (msg.x >= 0 && msg.x <= 650 && msg.y >= 0 && msg.y <= 650)
			{
				if (times % 3 == 1)    //��һ�ε������ѡ��������
				{
					//������ʼ����������
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
						MessageBox(NULL, "�����Ϲ���������ѡ������,", "��ʾ", MB_OK);
						goto loop;
					}
				}
				if (times % 3 == 2)//�ڶ��ε����ѡ�����ӵ�
				{
					//�������ӵ�
					lx = msg.y / 65 + 1;
					ly = msg.x / 65 + 1;
					if (first == 0 && Board[lx][ly] == EMPTY&&LegalMove(qx, qy, lx, ly, Board))
					{
						//�����޸���������
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
						//�����޸���������
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
						MessageBox(NULL, "�����Ϲ���������ѡ���ӵ�,", "��ʾ", MB_OK);
						goto loop;
					}
				}
				if (times % 3 == 0)//�����ε���������ϰ�
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
						MessageBox(NULL, "�����Ϲ��������·����ϰ�,", "��ʾ", MB_OK);
						goto loop;
					}
				}
			}
			////////////////////////���岿��////////////////////////////
			if (msg.x >= 700 && msg.x <= 800 && msg.y >= 350 && msg.y <= 450)
			{
				if (first == 0)//�ֵ������ߣ���һ���ڰ���
				{
					Board[bufa[2][0]][bufa[2][1]] = EMPTY;
					Board[bufa[1][0]][bufa[1][1]] = EMPTY;
					Board[bufa[0][0]][bufa[0][1]] = WHITECHESS;
					n = 0;
					return first;
				}
				if (first == 1)//�ֵ������ߣ���һ���ں���
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
//////////ʤ���ж�////////////
int JudgeWin(int Board[12][12])//ʤ���ж�
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
		return BLACKCHESS;   //����ȫ��
	else if (C_White == 4)
		return WHITECHESS;  //����ȫ��
	else
		return 0;//��δ�ֳ�ʤ��
}
//////////ģʽѡ��////////////
int Mode()//ģʽѡ�񡪡����ˣ��˻�ģʽ
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
				return -1;//����ģʽ
			}
			else if (msg.x >= 700 && msg.x <= 950 && msg.y >= 200 && msg.y <= 265)
			{
				n = 0;
				return 0;//�˻�ģʽ ��������
			}
			else if (msg.x >= 700 && msg.x <= 950 && msg.y >= 265 && msg.y <= 330)
			{
				n = 0;
				return 1;//�˻�ģʽ ��������
			}
			else
			{
				MessageBox(NULL,"��ѡ����Ϸģʽ","��ʾ",MB_OK);
				goto loop1;
			}
			break;
		default:
			break;
		}
	}
}
