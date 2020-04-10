#pragma once
#include"SearchEngine.h"

//估分变量---都是全局变量
extern double t1, t2;    //领土特征值,t1----queenmove  t2----kingmove
extern double c1, c2;   //位置特征值,c1---queenmove   c2---kingmove
extern double w, s;

//KingMove和QueenMove的模拟棋盘----都是全局变量
extern int KingmoveBlack[12][12];
extern int KingmoveWhite[12][12];
extern int QueenmoveBlack[12][12];
extern int QueenmoveWhite[12][12];
extern int Mobility[12][12];//灵活度的棋盘
extern int aWhite[4];
extern int aBlack[4];

/////////函数声明//////////
void SearchKingMove();
void SearchQueenMove();
void t1account();
void t2account();
void c1account();
void c2account();
void MonilitySearch();
void aaccount();
void saccount();
double Evaluate();