#pragma once
#include "stdafx.h"
#include <queue>
#include <stack>
#include <stdlib.h> 
#define Red 1
#define Blue -1

using namespace std;

struct Loc
{
	int x;
	int y;
	inline void Set(int x, int y);
};

struct MovePiece
{
	Loc loc;
	int piece;
};

struct Move_Recording						//棋盘移动信息记录
{
	Loc end_move_piece;						//终点
	Loc start_move_piece;					//起点
	int covered_piece;						//被覆盖的子
	void Set(Loc start_move_piece, Loc end_move_piece, int covered_piece)
	{
		Move_Recording::start_move_piece = start_move_piece;
		Move_Recording::end_move_piece = end_move_piece;
		Move_Recording::covered_piece = covered_piece;
	}
};


class Board
{

public:
	Board(int arr[5][5]);								 //构造函数
	void show();										 //展示当前棋盘
	Loc GetPiece(int piece);							 //得到棋子位置
	int GetPiece(Loc loc);								 //得到该位置的棋子值
	inline int GetPieceFaction(int piece);				 //判断棋子阵营
	inline bool GetPieceLegality(Loc ploc);				 //判断走子合法性
	inline bool IsAlive(int piece);						 //获得棋子存活状态
	int GetStep();										 //获得当前执行步数
	bool Is_Waning_Zone(Loc loc);						 //判断是否为禁区（距离）
	int GetAllMoves(MovePiece* Moves, int Piece);		 //获得所有的走法
	int GetLargerPiece(int Piece);						 //转到号大的棋子
	int GetSmallerPiece(int Piece);						 //转到号小的棋子
	int Winner();										 //判断输赢
	void Move(Loc nloc, int piece);						 //移子
	double GameState();									 //局面评估

	void Regret_Game(stack<Move_Recording> &Recording, int step);                          //悔棋

private:
	int step;				 //步数
	int board[5][5];		 //5×5棋盘
	int RedNum;				 //红方势力数
	int BlueNum;			 //蓝方势力数
	int RedState[7];         //红方棋子状态（下标0代表棋子存活个数，下标从1至6代表存活状态）
	int BlueState[7];        //蓝方棋子状态
};

void print();										     //打印空爱恩斯坦棋盘

inline int RandDice()
{
	return rand() % 6 + 1;
}

