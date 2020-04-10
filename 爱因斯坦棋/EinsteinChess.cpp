//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//爱因斯坦组负责人蒋嘉辰所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//蒋嘉辰	QQ：843795534 
// MonteCarlo.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include "CBoard.h"
#include "CMove.h"
#include "Competition.h"

using namespace std;

/*int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned)time(NULL));   //随机数播种
	int LayoutMode;				   //布局模式（默认布局，手动布局）
	int BattleMode;				   //对战模式（人人对战，人机对战）
	int FirstHand;				   //先手方（红方，蓝方）
	int AI;						   //AI方
	int Dice;                      //骰子
	MovePiece move;                //移动棋子
	int win =0;                    //胜方（1：红方，-1：蓝方）
	while (1)
	{
		system("cls");
		cout << "Welcome Einstein Chess!\n" << endl;
		cout << "【 0 】默认布局     【 1 】手动布局 " << endl;
		cin >> LayoutMode;
		{}
		while (LayoutMode != 0 && LayoutMode != 1)
		{
			cout << "请重新选择布局模式！" << endl;
			cin >> LayoutMode;
		}
		//int BOARD[5][5] = { { 0,0,0,0,0 },{ 1,-1,0,2,0 },{ 0,0,0,-6,0 },{ 0,0,-2,0,0 },{ 0,0,0,6,-0 } };  //默认棋
		int BOARD[5][5] = { { 1,2,3,0,0 },{ 4,5,0,0,0 },{ 6,0,0,0,-6 },{ 0,0,0,-5,-4 },{ 0,0,-3,-2,-1 } };  //默认棋局
		if (LayoutMode == 1)
		{
			print();
			cout << "输入红方棋子摆布:(1,1) (1,2) (1,3) (2,1) (2,2) (3,1)" << endl;
			cin >> BOARD[0][0] >> BOARD[0][1] >> BOARD[0][2] >> BOARD[1][0] >> BOARD[1][1] >> BOARD[2][0];  //定义红方
			cout << "输入蓝方棋子摆布:(5,5) (5,4) (5,3) (4,5) (4,4) (3,5)" << endl;
			int a[6];
			for (int i = 0; i < 6; i++)
				cin >> a[i];
			BOARD[4][4] = -a[0];
			BOARD[4][3] = -a[1];
			BOARD[4][2] = -a[2];
			BOARD[3][4] = -a[3];
			BOARD[3][3] = -a[4];
			BOARD[2][4] = -a[5]; //定义蓝方
		}
		Board CB(BOARD);           //定义棋盘并初始化
		CB.show();
		cout << "【 0 】人机对战     【 1 】人人对战 " << endl;
		cin >> BattleMode;
		while (BattleMode != 0 && BattleMode != 1)
		{
			cout << "请重新选择对战模式！" << endl;
			cin >> BattleMode;
		}
		cout << "【 0 】红方先手     【 1 】蓝方先手 " << endl;         
		cin >> FirstHand;
		while (FirstHand != 0 && FirstHand != 1)
		{
			cout << "请重新选择先手方！" << endl;
			cin >> FirstHand;
		}
		int faction = -(FirstHand * 2 - 1);                  //1为红，-1为蓝
		switch (BattleMode)
		{
		case 0:
			cout << "【 0 】AI红方       【 1 】AI蓝方 " << endl;
			cin >> AI;
			while (AI != 0 && AI != 1)
			{
				cout << "请重新选择AI方！" << endl;
				cin >> AI;
			}
			AI = -(AI * 2 - 1);                       //1为红，-1为蓝
			while (1)
			{
				CB.show();
				//CB.GameState();
				if (win == 1)
				{
					cout << "\n红方获胜！";
					win = 0;
					system("pause");
					break;
				}
				if (win == -1)
				{
					cout << "\n蓝方获胜！";
					win = 0;
					system("pause");
					break;
				}
				//Dice = RandDice();
				
				//cout << "骰子数为:" << Dice << endl;
				if (faction == AI)
				{
					cout << "骰子数为:" << endl;
					cin >> Dice;
					cout <<"输入回车确定AI落子"<<endl;
					if (AI == Red)
						cout << "红方";
					else
						cout << "蓝方";
					cout << "AI落子思考中..." << endl;
					MonteCarloMove(CB, AI*Dice, 1);
					system("pause");
				}
				else
				{
					if (faction == Red)
						cout << "红方走子" << endl;
					else
						cout << "蓝方走子" << endl;
					cout << "输入格式:要移动的棋子 移动后的X坐标 移动后的Y坐标" << endl;
					cin >> move.piece >> move.loc.x >> move.loc.y;
					move.loc.x--;
					move.loc.y--;
					CB.Move(move.loc, faction*move.piece);
				}
				win = CB.Winner();
				faction *= -1;
			}
			break;
		case 1:
			while (1)
			{
				CB.show();
				if (win == 1)
				{
					cout << "\n红方获胜！";
					win = 0;
					system("pause");
					break;
				}
				if (win == -1)
				{
					cout << "\n蓝方获胜！";
					win = 0;
					system("pause");
					break;
				}
				Dice = RandDice();
				cout << "骰子数为:" << Dice << endl;
				if (faction == Red)
					cout << "红方走子" << endl;
				else
					cout << "蓝方走子" << endl;
				cout << "输入格式:要移动的棋子 移动后的X坐标 移动后的Y坐标" << endl;
				cin >> move.piece >> move.loc.x >> move.loc.y;
				move.loc.x--;
				move.loc.y--;
				CB.Move(move.loc, faction*move.piece);
				win = CB.Winner();
				faction *= -1;
			}
		}
	}
	return 0;
}*/

//测试
/*int main()
{
	srand((unsigned)time(NULL));
	double red = 0, blue = 0;
	int win, player = -1, Dice, times = 1;
	int BOARD[5][5] = { { 1,2,3,0,0 },{ 4,5,0,0,0 },{ 6,0,0,0,-6 },{ 0,0,0,-5,-4 },{ 0,0,-3,-2,-1 } };
	for (int i = 0; i < times; i++)
	{
		Board CB(BOARD);
		player = -1 * player;
		int _player = player;
		while (1)
		{
			win = CB.Winner();
			_player *= -1;
			system("cls");
			CB.show();
			if (win == Red)
			{
				red = red + 1;
				break;
			}
			else if (win == Blue)
			{
				blue = blue + 1;
				break;
			}
			else
			{
				
				Dice = RandDice();
				cout << CB.GameState() << endl;
				cout << "骰子数为:" << _player*Dice << endl;
				system("pause");
				if (_player == 1)
					MonteCarloMove(CB, _player*Dice, 0);
				else
					MonteCarloMove(CB, _player*Dice, 1);
			}
		}
	}
	cout << red/times  << endl << blue/times ;
	return 0;
}*/

int main()
{
	Match();
}
