#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"
#include "Paradox.h"
#include <hash_map>
#include <math.h>
#include "MCTS.h"

using namespace std;



//Some  Game Function
void MakeLaterSituation(Board &NewB,int StartPlayer,bool Show)
{
	int player = StartPlayer;
	for (; NewB.GetFreeEdgeNum()>0;)
	{
		if (NewB.GetFreeEdgeNum() == 0)
			break;
		bool Cap = false;
		player = RndFilterTurn(NewB, player,false,FILTER_RANGE);
	}
	if (Show)
		NewB.PrintBoard();
}
int GetBoardWinner(Board &CB, int LatterPlayer)
{
	LOC a[MOVENUM];
	if (CB.GetFilterMoves(a) != 0)
		return 0;
	BoxBoard Advanced(CB);
	int w = Advanced.GetBoardWinner(LatterPlayer);
	return w;
}
/*int GetBoardWinner2(Board &CB, int LatterPlayer)
{
	if (CB.GetFreeEdgeNum() != 0)
		return 0;
	BoxBoard Advanced(CB);
	
	int w = Advanced.GetBoardWinner(LatterPlayer);
	return w;
}*/

//随机回合。会先使用贪婪
void RndTurn(Board &CB, int Player, bool Msg)
{
	LOC Moves[MOVENUM];
	CB.GetAllCTypeBoxes(Player, Msg);
	int MoveNum = CB.GetAllMoves(Moves);//确定这个局面下TurnNum的数量
	if (MoveNum == 0)
	{
		cout << "RndTurn函数中的TurnNum =0" << endl;
		CB.PrintBoard();
		system("pause");
	}
	int Rnd = rand() % MoveNum;//在0-MoveNum中抽取一个随机数
	CB.Move(Moves[Rnd].x, Moves[Rnd].y, Player);
	if (Msg)
		MoveMsg(Moves[Rnd].x, Moves[Rnd].y, Player);
}
int RndFilterTurn(Board &CB, int Player, bool Msg, int Filter_Range)
{
	LOC Moves[MOVENUM];
	CB.GetAllCTypeBoxes(Player, Msg);

	int MoveNum;
	int FreeEdge = CB.GetFreeEdgeNum();
	if (FreeEdge < Filter_Range)//仅在FreeEdge数量小于25的情况下考虑Filter
		MoveNum = CB.GetFilterMoves(Moves);//确定这个局面下MoveNum的数量
	else
		MoveNum = CB.GetFreeMoves(Moves);//确定这个局面下MoveNum的数量

	if (MoveNum != 0)//在某些时候，由于吃掉了前面的C型格。可能导致MoveNum的数量为0.这时候只要跳过这一步自然就会开始判断胜利。
	{
		int Rnd = rand() % MoveNum;//在0-MoveNum中抽取一个随机数
		CB.Move(Moves[Rnd].x, Moves[Rnd].y, Player);
		if (Msg)
			MoveMsg(Moves[Rnd].x, Moves[Rnd].y, Player);
		return -Player;//换手
	}
	else
	{
		return Player;//不换手
	}
	
}

//MC
int GetMCWinner(Board &CB, int NextPlayer)
{
	int player = NextPlayer;
	while (CB.GetFreeEdgeNum() != 0)//当还存在自由边的时候
	{
		RndTurn(CB, player, false);
		player = -player;
	}
	int W = GetBoardWinner(CB, -player);
	return W;
}
float GetMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES)
{
	Board MCB = CB;//先复制一个棋盘
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetMCWinner(MCB, NextPlayer) == Winner)
			MCE++;
	}
	float score = ((float)MCE) / ((float)TIMES);
	return score;
}
void MonteCarloMove(Board &CB, int Player, bool Msg)
{
	//必须注意的是，MC开始时候，一定要注意是没有C型格的状态。如果存在C型格，请先决定是否占据。
	CB.GetAllCTypeBoxes(Player, Msg);//首先吃掉所有的C型格
	LOC Moves[MOVENUM];
	int MoveNum = CB.GetAllMoves(Moves);		//确定这个局面下TurnNum的数量
	float Best = -1;							//确保无论多烂的成绩都能有一个Best
	int BestMove;
	for (int i = 0; i<MoveNum; i++)
	{
		Board MCB(CB.board, CB.Step);				//先创建一个新棋盘
		MCB.Move(Moves[i].x, Moves[i].y, Player);	//假定我方移动了一个步
		int NextPlayer = -Player;					//下一个移动者将是对方
		float Eva = GetMCEvalution(MCB, NextPlayer, Player, MC_TIMES);
		if (Eva >= Best)//如果大于等于最佳成绩，更新记录
		{
			Best = Eva;
			BestMove = i;
		}
	}
	CB.Move(Moves[BestMove].x, Moves[BestMove].y, Player);//选择最佳决策
	if (Msg)
		MoveMsg(Moves[BestMove].x, Moves[BestMove].y, Player);
}

//Filter MC
int GetFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range)
{
	int player = NextPlayer;
	while (CB.GetFreeEdgeNum() !=0)//当还存在自由边的时候
	{
		player = RndFilterTurn(CB, player, false, Filter_Range);
		//CB.PrintBoard();
		//system("pause");
	}
	//cout << "Finish" << endl;
	int W = GetBoardWinner(CB, -player);
	return W;
}
float GetFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range)
{
	Board MCB = CB;//先复制一个棋盘
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
			MCE++;
	}
	float score = ((float)MCE) / ((float)TIMES);
	//MCB.ShowBoard();
	//cout << "This one score is " << score << endl;
	return score;
}
float GetFilterMCSingleEva(Board &CB, int NextPlayer, int Winner, int Filter_Range)
{
	Board MCB = CB;//先复制一个棋盘
	if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
		return 1;
	return 0;
}
void FilterMCMove(Board &CB, int Player, bool Msg)
{
	//必须注意的是，MC开始时候，一定要注意是没有C型格的状态。如果存在C型格，请先决定是否占据。
	CB.GetAllCTypeBoxes(Player,Msg);//首先吃掉所有的C型格
	LOC Moves[MOVENUM];
	int MoveNum;
	int FreeEdge = CB.GetFreeEdgeNum();
	if (FreeEdge < FILTER_RANGE)//仅在FreeEdge数量大于4的情况下考虑Filter
		MoveNum = CB.GetFilterMoves(Moves);//确定这个局面下MoveNum的数量
	else
		MoveNum = CB.GetFreeMoves(Moves);//确定这个局面下MoveNum的数量

	float Best = -1;							//确保无论多烂的成绩都能有一个Best
	int BestMove;
	for (int i = 0; i<MoveNum; i++)
	{
		Board MCB(CB.board, CB.Step);				//先创建一个新棋盘
		MCB.Move(Moves[i].x, Moves[i].y, Player);	//假定我方移动了一个步
		int NextPlayer = -Player;					//下一个移动者将是对方
		float Eva = GetFilterMCEvalution(MCB, NextPlayer, Player, MC_TIMES,FILTER_RANGE);
		if (Eva >= Best)//如果大于等于最佳成绩，更新记录
		{
			Best = Eva;
			BestMove = i;
		}
	}
	CB.Move(Moves[BestMove].x, Moves[BestMove].y, Player );//选择最佳决策
	if (Msg)
		MoveMsg(Moves[BestMove].x, Moves[BestMove].y, Player);
}

float GetRaveFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range,int RaveTable[LEN][LEN])
{
	Board MCB = CB;//先复制一个棋盘
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
		{
			MCE++;
			for (int y = 0; y < LEN; y++)
			{
				for (int x = 0; x < 0; x++)
				{
					if (MCB.board[x][y] == Winner)
						RaveTable[x][y]++;
				}
			}
		}
	}
	float score = ((float)MCE) / ((float)TIMES);
	//MCB.ShowBoard();
	//cout << "This one score is " << score << endl;
	return score;
}
