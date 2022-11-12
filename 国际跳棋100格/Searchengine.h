#pragma once
#include"makeNowMoves.h"
#include"CEvelation.h"
#include"head.h"
class CSearchengine: 
	public makeNowMoves,
	public CEvelation
{
public:
	CSearchengine();
	~CSearchengine();
	void copy(int board[10][10], int Color);
	void goAMove(int board[10][10],MOVE aMove);//走一步招法的函数
	void goBack(int board[10][10],MOVE aMove);//撤回一步的函数
	int IsGameOver();										  //判断是否结束
	bool KING_FOUR_DIRECTION_CAN_EAT(int X, int Y, int Color);//这个是判断王的能不能吃
	bool BING_FOUR_DIRECTION_CAN_EAT(int X, int Y, int Color);//判段兵能不能吃
	bool IsBingCanMove(int X, int Y, int Color);			  //判断兵能不能走
	bool IsKingCanMove(int X, int Y, int Color);			  //判断王能不能走
protected:
	MOVE PossibleMoves[20][150];							//招法数组
	//MOVE **PossibleMoves; 

	int nowColor;					 						  //当前走子方
	int board[10][10];										  //棋盘信息
	clock_t timeStart;
	clock_t timeStop;
	MOVE theBestMove;
	MOVE BestMove;
	
};
