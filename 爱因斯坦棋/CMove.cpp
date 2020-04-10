#include "stdafx.h"
#include "CMove.h"

using namespace std;

void RandMove(Board &CB, int piece)
{
	MovePiece move[6];
	int dice= rand() % CB.GetAllMoves(move, piece);
	CB.Move(move[dice].loc, move[dice].piece);
}

int GetMCWinner(Board &CB, int NextPlayer, int algorithm)
{
	Board NCB(CB);
	int player = NextPlayer;
	int win;
	if (algorithm == 0)
	{
		while (1)
		{
			win = NCB.Winner();
			if (win != 0)
				return win;
			player *= -1;
			RandMove(NCB, player*RandDice());
		}
	}
	else
	{
		while (1)
		{
			win = NCB.Winner();
			if (win != 0)
				return win;
			player *= -1;
			Rand_Best_Move(NCB, player*RandDice());
		}
	}
}

double GetMCEvalution(Board &CB, int NextPlayer, int times, int algorithm)
{
	Board NCB = CB;
	int Eva = 0;															 //定义初始估值函数为0
	for (int i = 0; i<times; i++)
		if (GetMCWinner(NCB, NextPlayer, algorithm) == -NextPlayer)           //NextPlayer输
			Eva++;
	double Result = (double)Eva / (double)times;
	return Result;
}

void MonteCarloMove(Board &CB, int piece, int algorithm, stack<Move_Recording> &Recording)
{
	int f = CB.GetPieceFaction(piece);
	double BestVal = -1;
	int BestMove = 0;
	MovePiece Moves[6];
	int MoveNum = CB.GetAllMoves(Moves, piece);
	for (int i = 0; i < MoveNum; i++)
	{
		Board NCB(CB);
		NCB.Move(Moves[i].loc, Moves[i].piece);
		double val = GetMCEvalution(NCB, -f, MC_CONSTANT, algorithm);
		if (BestVal < val)
		{
			BestVal = val;
			BestMove = i;
		}
	}
	Move_Recording temp;
	temp.Set(CB.GetPiece(Moves[BestMove].piece), Moves[BestMove].loc, CB.GetPiece(Moves[BestMove].loc));
	Recording.push(temp);
	CB.Move(Moves[BestMove].loc, Moves[BestMove].piece);
}

///////////////////////////////////////////////////////////////////
void Rand_Best_Move(Board &CB, int piece)
{
	queue<MovePiece> Q;
	MovePiece move[6];
	int Move_Num = CB.GetAllMoves(move, piece);
	double Best_Value = -100;
	int faction = CB.GetPieceFaction(piece);
	for (int i = 0; i < Move_Num; i++)
	{
		Board NCB(CB);
		NCB.Move(move[i].loc, move[i].piece);
		if (Best_Value < faction*NCB.GameState())
		{
			Best_Value = faction*NCB.GameState();
			while (!Q.empty())
				Q.pop();
		}
		if(Best_Value== faction*NCB.GameState())
			Q.push(move[i]);
	}
	int dice = rand() % Q.size();
	for (int i = 0; i < dice; i++)
		Q.pop();
	//cout << "false";
	CB.Move(Q.front().loc, Q.front().piece);
}