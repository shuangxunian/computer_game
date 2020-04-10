#pragma once
#include "stdafx.h"
#include "CBoard.h"

#define MC_CONSTANT 2000														 //模拟次数

//普通蒙特卡洛

void RandMove(Board &CB, int Piece);															 //随机走子
int GetMCWinner(Board &CB, int NextPlayer, int algorithm);                                       //模拟
double GetMCEvalution(Board &CB, int NextPlayer, int times, int algorithm);				         //评估
void MonteCarloMove(Board &CB, int Piece, int algorithm, stack<Move_Recording> &Recording);      //移子

//////////////////////////////////////////////////////////////////////////////////////////////////

//混合局面评估的蒙特卡洛
void Rand_Best_Move(Board &CB, int Picece);										  //从所有最好的局面中随机走子

///////////////////////////////////////////////////////////////////////////////////////////////////
