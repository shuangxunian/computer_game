#pragma once//保证头文件只被编译一次

//宏定义
#define BLACKCHESS 2
#define WHITECHESS 1
#define BARRIER -1//障碍
#define EMPTY 0
#define INFI 9999 //无穷大

extern int Board[12][12];//全局变量
extern int bufa[3][2];//存储每步的步法，用于悔棋
//extern int mode;//游戏模式选择

typedef struct Pos//位置坐标结构体
{
	int x;
	int y;
}CHESSPOS;

typedef struct Move  //步法结构体
{
	int ChessID;//行棋方
	double score;//评估得分
	CHESSPOS From;
	CHESSPOS To;
	CHESSPOS Bar;
}CHESSMOVE;

////////////函数声明////////////////
void Show(int Board[12][12]);
void InitBoard(int Board[12][12]);
int Move(int firsthand, int Board[12][12], int bufa[3][2]);
int LegalMove(int x1, int y1, int x2, int y2, int Board[12][12]);
int JudgeWin(int Board[12][12]);
int Mode();