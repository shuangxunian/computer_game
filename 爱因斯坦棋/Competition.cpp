#include "stdafx.h"
#include "Competition.h"

using namespace std;

void Match()
{
	srand((unsigned)time(NULL));						//随机数播种
	int AI;												//AI方
	int Dice;											//骰子
	int win = 0;									    //胜方（1：红方，-1：蓝方）
	int player;											//当前出子方
	while (1)
	{
		stack<Move_Recording> Recording;				//棋盘记录
		player = -1;						
		win = 0;										//每回合开始初始胜方
		int temp;										//逻辑转换临时变量
		system("cls");
		cout << "欢迎来到命运之石\n" << endl;
		int BOARD[5][5] = { 0 };					    //初始化棋盘
		print();
		cout << "输入红方棋子摆布:(1,1)(1,2)(1,3)(2,1)(2,2)(3,1)" << endl << "例如：“123456”" << endl;
		cin >> temp;
		int piece;
		for (int i = 6; i >0; i--)
		{
			piece = temp % 10;
			temp = (temp - piece) / 10;
			switch (i)
			{
			case 1:BOARD[0][0] = piece; break;
			case 2:BOARD[0][1] = piece; break;
			case 3:BOARD[0][2] = piece; break;
			case 4:BOARD[1][0] = piece; break;
			case 5:BOARD[1][1] = piece; break;
			case 6:BOARD[2][0] = piece; break;
			}
		}
		cout << "输入蓝方棋子摆布:(5,5)(5,4)(5,3)(4,5)(4,4)(3,5)" << endl << "例如：“123456”" << endl;
		cin >> temp;
		for (int i = 6; i >0; i--)
		{
			piece = temp % 10;
			temp = (temp - piece) / 10;
			switch (i)
			{
			case 1:BOARD[4][4] = -piece; break;
			case 2:BOARD[4][3] = -piece; break;
			case 3:BOARD[4][2] = -piece; break;
			case 4:BOARD[3][4] = -piece; break;
			case 5:BOARD[3][3] = -piece; break;
			case 6:BOARD[2][4] = -piece; break;
			}
		}
		cout << "【 0 】AI红方       【 1 】AI蓝方 " << endl;
		cin >> AI;
		while (AI != 0 && AI != 1)
		{
			cout << "开什么玩笑，请重新输入：" << endl;
			cin >> AI;
		}
		AI = -(AI * 2 - 1);                  //1为红，-1为蓝
		Board CB(BOARD);
		while (1)
		{
			CB.show();
			cout << "不悔棋输入0，悔棋输入悔棋步数" << endl;
			cin >> temp;
			while (temp > CB.GetStep())
			{
				cout << "你怎么不回到清朝?重新输入！" << endl;
				cin >> temp;
			}
			if (temp % 2 == 1)
				player *= -1;
			CB.Regret_Game(Recording, temp);
			CB.show();
			CB.GameState();
			win = CB.Winner();
			if (win == 1)
			{
				cout << "红方获胜啦！" << endl;
				system("pause");
				break;
			}
			else if (win == -1)
			{
				cout << "蓝方获胜啦！" << endl;
				system("pause");
				break;
			}
			else
			{
				player = player*-1;
				if (player == AI)
				{
					cout << "骰子数为:" << endl;
					cin >> Dice;
					while (Dice < 1 || Dice>6)
					{
						cout << "皮？重新输入！" << endl;
						cin >> Dice;
					}
					cout << "输入回车确定AI落子" << endl;
					if (AI == 1)
						cout << "红方";
					else
						cout << "蓝方";
					cout << "AI落子思考中..." << endl;
					MonteCarloMove(CB, AI*Dice, 1, Recording);
					system("pause");
				}
				else
				{
					if (player == 1)
						cout << "红方";
					else
						cout << "蓝方";
					cout << "走子 终点(xy)" << endl << "例如：“1 22”" << endl;
					cin >> Dice >> temp;
					while (!CB.IsAlive(player*Dice) || temp < 11 || temp > 55 || temp % 10 == 0)
					{
						cout << "你不要瞎搞好伐！重新输入！" << endl;
						cin >> Dice >> temp;
					}
					Loc loc;
					loc.Set(temp / 10 - 1, temp % 10 - 1);
					Move_Recording temp;
					temp.Set(CB.GetPiece(player*Dice), loc, CB.GetPiece(loc));
					Recording.push(temp);
					CB.Move(loc, player*Dice);
				}
			}
		}
	}
}
