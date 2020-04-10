#ifndef	_DEFINE_H_
#define	_DEFINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>


#define GRID_NUM 11			//棋盘行数，其中1..9为棋的坐标，其他为边界
#define GRID_COUNT 81		//可放棋子总数
#define BLACK 	1			//黑棋		黑白棋转换时,可对求异或
#define WHITE 	2			//白棋
#define BORDER	3			//边界
#define NOSTONE 	0		//空子无棋

typedef struct _stoneposition
{
		char x;
		char y;
		int	score;
}SMove;				    	//招法

typedef struct SGroup
{
	int count ;
	float eyes;
	char color;
}SGroup;

typedef struct SPlayer
{
	int total;
	int know;
}SPlayer;

extern char		board[GRID_NUM][GRID_NUM];		//棋盘表示
extern char		board_know[GRID_NUM][GRID_NUM]; //试探棋盘
extern char		chessColor;						//自己棋的颜色
extern SMove	bestMove;						//最优棋

extern SPlayer  player[3];			        	//白和黑棋,记录各方的走子总数和知道对方的子数

extern int boardBaseScore[GRID_NUM][GRID_NUM];

void 	InitGame();
void	record();
void	Undo();

void PrintBoard();

#include "Engine.h"

#endif
