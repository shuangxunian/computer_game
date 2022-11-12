#pragma once
#include"Data.h"
#include"MoveGenerator.h"
#include"EvaluateEngine.h"


extern int CurPosition[12][12];
extern CHESSMOVE BestMove;

///////函数声明//////
void MakeMove(CHESSMOVE* move);//传入一个步法的结构体指针
void UnMakeMove(CHESSMOVE* move);