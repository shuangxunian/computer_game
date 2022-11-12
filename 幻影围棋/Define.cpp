#ifndef	_DEFINE_CPP_
#define	_DEFINE_CPP_

#include "Define.h"


char		board[GRID_NUM][GRID_NUM];		//棋盘表示
char		board_know[GRID_NUM][GRID_NUM]; //试探棋盘
int			mark[GRID_NUM][GRID_NUM];		//MonteCarlo分数棋盘
char		chessColor;				    	//自己棋的颜色
SMove		bestMove ;						//最优棋

int boardBaseScore[GRID_NUM][GRID_NUM] = 			//棋盘的基础分数
{	
	0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,2,3,3,3,3,3,2,1,0,
	0,1,2,3,4,4,4,3,2,1,0,
	0,1,2,3,4,5,4,3,2,1,0,
	0,1,2,3,4,4,4,3,2,1,0,
	0,1,2,3,3,3,3,3,2,1,0,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0
};

SPlayer player[3];		

#endif