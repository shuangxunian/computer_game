//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//点格棋组负责人陈校楠所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//陈校楠	QQ：1198583339 
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "math.h"
#include "windows.h"

#include "Define.h"
#include "Paradox.h"
#include "BoxBoard.h"
#include "MCTS.h"

#define HUMAN 1
#define COMPUTER 2

using namespace std;

void Model_AI_Game(Board &CB)
{
	//int model;
	//char red_name[20];
	//char blue_name[20];
	//void(*func_red)(Board&, int, bool);
	//void(*func_blue)(Board&, int, bool);
	//clock_t t_start, t_finish;		//设置计时器的变量
	//double t_totaltime;				//设置计时器的变量
	//srand((unsigned)time(NULL));	//进行一次随机数播种


	//								//选择对弈的算法
	//for (;;)
	//{
	//	cprintf("为红方选择算法：\n【 1.GameTurnMove_Rave 】\n【 2.GameTurnMove_RaveSolverDouble 】\n【 3.UCT算法 】\n【 4.UCT算法_2 】\n【 5.GameTurnMove_T 】\n【 6.GameTurnMove 】\n\n选择为：", 12);
	//	cin >> model;
	//	if (model >= 1 && model <= 6)//【记得改总数】
	//	{
	//		if (model == 1)
	//		{
	//			func_red = GameTurnMove_Rave;
	//			strcpy_s(red_name, "GameTurnMove_Rave");
	//		}
	//		else if (model == 2)
	//		{
	//			func_red = RaveSolverDouble;
	//			strcpy_s(red_name, "RaveSolverDouble");
	//		}
	//		else if (model == 3)
	//		{
	//			func_red = UCTMove;
	//			strcpy_s(red_name, "UCT算法");
	//		}
	//		else if (model == 4)
	//		{
	//			func_red = UCTMove_2;
	//			strcpy_s(red_name, "UCT_2算法");
	//		}
	//		else if (model == 5)
	//		{
	//			func_red = GameTurnMove_T;
	//			strcpy_s(red_name, "GameTurnMove_T算法");
	//		}
	//		else if (model == 6)
	//		{
	//			func_red = GameTurnMove;
	//			strcpy_s(red_name, "GameTurnMove");
	//		}
	//		break;
	//	}
	//	else
	//	{
	//		cprintf("输入错误！", 8);
	//	}
	//}
	//for (;;)
	//{
	//	cprintf("为蓝方选择算法：\n【 1.GameTurnMove_Rave 】\n【 2.RaveSolverDouble 】\n【 3.UCT算法 】\n【 4.UCT算法_2 】\n【 5.GameTurnMove_T 】\n【 6.GameTurnMove 】\n\n选择为：", 9);
	//	cin >> model;
	//	if (model >= 1 && model <= 6)//【记得改总数】
	//	{
	//		if (model == 1)
	//		{
	//			func_blue = GameTurnMove_Rave;
	//			strcpy_s(blue_name, "GameTurnMove_Rave");
	//		}
	//		else if (model == 2)
	//		{
	//			func_blue = RaveSolverDouble;
	//			strcpy_s(blue_name, "RaveSolverDouble");
	//		}
	//		else if (model == 3)
	//		{
	//			func_blue = UCTMove;
	//			strcpy_s(blue_name, "UCT算法");
	//		}
	//		else if (model == 4)
	//		{
	//			func_blue = UCTMove_2;
	//			strcpy_s(blue_name, "UCT_2算法");
	//		}
	//		else if (model == 5)
	//		{
	//			func_blue = GameTurnMove_T;
	//			strcpy_s(blue_name, "GameTurnMove_T算法");
	//		}
	//		else if (model == 6)
	//		{
	//			func_blue = GameTurnMove;
	//			strcpy_s(blue_name, "GameTurnMove");
	//		}
	//		break;
	//	}
	//	else
	//	{
	//		cprintf("输入错误！", 8);
	//	}
	//}

	////开始比赛
	//int red_count = 0, blue_count = 0;
	//for (int n = 1; n<200; n++)
	//{
	//	//system("pause");
	//	Board NewBoard = CB;
	//	NewBoard.PrintBoard();
	//	int player = 1 - (2 * (n % 2));//先手为双方轮流交替
	//	cout << "第" << n << "场开始" << endl;
	//	printf("%s先手", player == RED ? "红方" : "蓝方");
	//	for (;;)
	//	{
	//		int w = 0, tz = 0;
	//		if (player == RED)
	//			cprintf("红方 ", 12);
	//		else
	//			cprintf("蓝方 ", 9);
	//		cout << "正在考虑第" << NewBoard.Step + 1 << "步" << endl;
	//		cout << "形势为 红方格子数[" << NewBoard.GetPlayerBoxes(RED) << "]  蓝方格子数[" << NewBoard.GetPlayerBoxes(BLUE) << "]" << endl;
	//		cout << "第[" << n << "]场，";
	//		cprintf(red_name, 12);
	//		cout << "胜[" << red_count << "]局，";
	//		cprintf(blue_name, 9);
	//		cout << "胜[" << blue_count << "]局" << endl;

	//		//w = GetBoardWinner(NewBoard, -player);
	//		w = NewBoard.Winner();

	//		if (w == RED)
	//		{
	//			red_count++;
	//			break;
	//		}
	//		if (w == BLUE)
	//		{
	//			blue_count++;
	//			break;
	//		}
	//		t_start = clock();
	//		if (player == RED)
	//		{

	//			func_red(NewBoard, player, true);
	//		}
	//		else
	//		{

	//			func_blue(NewBoard, player, true);
	//		}
	//		t_finish = clock();
	//		t_totaltime = (double)(t_finish - t_start) / CLOCKS_PER_SEC;
	//		cout << "\n消耗时间为" << t_totaltime << "秒！" << endl;
	//		player = -player;//玩家切换
	//		NewBoard.PrintBoard();//显示局面
	//		w = NewBoard.Winner();
	//		if (w == RED)
	//		{
	//			red_count++;
	//			break;
	//		}
	//		if (w == BLUE)
	//		{
	//			blue_count++;
	//			break;
	//		}
	//	}
	//}
	//cout << "全部计算结束\n\n\n红方胜利" << red_count << "    蓝方胜利" << blue_count << endl;
}
void Model_Rnd_Test(Board &CB)
{
	srand((unsigned)time(NULL));//进行一次随机数播种

	int player = RED;
	Board NewBoard = CB;
	NewBoard.PrintBoard();
	for (;;)
	{
		int w = 0, tz = 0;
		bool Cap = false;
		if (NewBoard.GetFreeEdgeNum()>0)
			RndFilterTurn(NewBoard, player, false, FILTER_RANGE);
		else
			RndTurn(NewBoard, player, false);
		//
		if (player == RED)
			cprintf("\n红方已经落子\n", 12);
		else
			cprintf("\n蓝方已经落子\n", 9);
		player = -player;//玩家切换
		NewBoard.PrintBoard();//显示局面

		system("pause");
		w = NewBoard.Winner();
		if (w == RED)
		{
			break;
		}
		if (w == BLUE)
		{
			break;
		}
	}
	cout << "\n\nRnd Test Finish\n\n" << endl;
	exit(0);
}
void Model_Board_Winner_Test()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		Board Test;
		Test.LoadBoard();
		int r = GetBoardWinner(Test, BLUE);
		int b = GetBoardWinner(Test, RED);
		if (r || b)
		{
			cout << "当先手为红时这个局面的胜利者为";
			if (r == RED)
				cprintf("红方", 12);
			else
				cprintf("蓝方", 9);

			cout << endl << "当先手为蓝时这个局面的胜利者为";
			if (b == RED)
				cprintf("红方", 12);
			else
				cprintf("蓝方", 9);
		}
		cout << endl;
		system("pause");
	}
}
void Model_Define_Board(Board &CB)
{
	CB.LoadBoard();
	cout << "Define Finish!" << endl;
	system("pause");
	system("CLS");

}
void Model_Define_Chain()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		Board Test;
		Test.LoadBoard();
		GameTurnMove(Test, RED, true);
		system("pause");
		system("CLS");
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	Board CB;						//新建一个游戏主显示棋盘CB
	clock_t start, finish;			//设置计时器的变量
	double totaltime;				//设置计时器的变量
	srand((unsigned)time(NULL));	//进行一次随机数播种

	cout << "Board size from 488 to " << sizeof(Board) << endl;
	cout << "Node size from 1232 to " << sizeof(UCTNode) << endl;

	//初始界面信息
	cprintf("【DOT&BOX KILLER】\n\n", 14);
	cprintf("【  0 】默认布局 \n【  1 】算法对弈模式\n【  2 】随机测试模式\n【  3 】测试胜利者\n【  4 】测试\n【其他】自定义布局\n\n", 10);
	//模式选择
	int model;
	cin >> model;
	if (model == 0)
		cprintf("默认模式开启", 8);
	else if (model == 1)
		Model_AI_Game(CB);
	else if (model == 2)
		Model_Rnd_Test(CB);
	else if (model == 3)
		Model_Board_Winner_Test();
	else if (model == 4)
		Model_Define_Chain();
	else
		Model_Define_Board(CB);


	for (;;)
	{
		//先检查是否胜利(因为游戏有可能从一个已经胜利的局面开始)
		int W = CB.Winner();
		if (W == RED)
		{
			cout << "红方胜利，游戏结束了。" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}
		if (W == BLUE)
		{
			cout << "蓝方胜利，游戏结束了。" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}

		//然后开始执行双方走步
		CB.PrintBoard();
		cout << "\n【0】人工占边  【1】AI占边" << endl;
		int com;
		for (;;)
		{
			int inputnumber;
			cin >> inputnumber;
			if (inputnumber == 0 || inputnumber == 1)
			{
				com = inputnumber;
				break;
			}
			else
			{
				cprintf("输入错误，请重新输入命令", 8);
			}
		}
		if (com == 0)
		{
			system("cls");
			//CB.PrintBoard();
			printf("○(0, 1)○(0 ,3)○(0 ,5)○(0,7 )○(0, 9)○\n1      1      1      1      1      1\n0      2      4      6      8     10\n○(2, 1)○(2, 3)○(2, 5)○(2 ,7)○(2, 9)○\n3      3      3      3      3      3\n0      2      4      6      8     10\n○(4, 1)○(4 ,3)○(4, 5)○(4 ,7)○(4, 9)○\n5      5      5      5      5      5\n0      2      4      6      8     10\n○(6, 1)○(6, 3)○(6, 5)○(6 ,7)○(6, 9)○\n7      7      7      7      7      7\n0      2      4      6      8     10\n○ (8,1)○(8 ,3)○(8, 5)○(8, 7)○(8 ,9)○\n9      9      9      9      9      9\n0      2      4      6      8     10\n○(10,1)○(10,3)○(10,5)○(10,7)○(10,9)○\n");
			cout << "\n请输入（落点横坐标，落点横坐标），如（2,1）" << endl;
			for (;;)
			{
				int locx, locy, owner;
				owner = HUMAN;
				scanf("%d,%d", &locy, &locx);
				if ((owner == 1 || owner == 2) && locx >= 0 && locx <= (LEN - 1) && locy >= 0 && locy <= (LEN - 1))//限制三个数的输入范围
				{
					if (owner == 2)
						owner = -1;
					if ((IfOddNumber(locx) && IfEvenNumber(locy)) || (IfEvenNumber(locx) && IfOddNumber(locy)))//输入的xy为一个奇数与一个偶数
					{
						bool HH = CB.Move(locx, locy, owner);
						if (HH)
							cout << owner << "占据了一个格子！" << endl;
						break;
					}
					else
					{
						cout << "XY轴应该是一个奇数与一个偶数！" << endl;
					}
				}
				else
				{
					cprintf("输入错误，请重新输入命令\n", 8);
					scanf("%d,%d,%d", &locx, &locy, &owner);
				}
				cprintf("输入错误，请重新输入命令\n", 8);
			}
			system("cls");
		}
		else if (com == 1)
		{
			printf("正在计算…………");
			//cout << "请输入落子方(1或者2)" << endl;
			for (;;)
			{
				int owner;
				//cin >> owner;
				owner = COMPUTER;
				if (owner == RED || owner == 2)
				{
					if (owner == 2)
						owner = BLUE;
					//电脑的移动函数
					start = clock();
					GameTurnMove(CB, owner, true);
					finish = clock();
					totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
					cout << "\n消耗时间为" << totaltime << "秒！" << endl;
					break;
				}
				cprintf("输入错误，请重新输入命令", 8);
			}
		}
	}
	return 0;
}
