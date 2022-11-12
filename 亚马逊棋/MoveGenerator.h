#pragma once

#include"Data.h"

extern CHESSMOVE MoveList[8][2500];     //用以存储步法,结构体数组
extern int MoveCount;       //记录步法总数
extern int Offset[8][2];      //定义偏移量，简化代码

/////////////////////函数声明////////////////////////////
void InitOffset();
int CreatePossibleMove(int Board[12][12],int depth,int Side);//传入棋盘，深度和行棋方
void CreatePossibleBar(int Board[12][12], int depth, int Side, int FromX, int FromY, int ToX, int ToY);
