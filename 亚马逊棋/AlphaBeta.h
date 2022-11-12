#pragma once
#include"Data.h"
#include"SearchEngine.h"
#include<string.h>

extern int SearchDepth;//设置搜索深度
extern int Step;//记录交替走过的步数
extern int mode;

///////函数声明///////
void SearchAGoodMove(int Board[12][12]);
double AlphaBeta(int depth, double alpha=-INFI, double beta=INFI);//传入两个默认参数alpha和beta