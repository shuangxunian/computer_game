#include"SearchEngine.h"


//对CurPosition进行操作
void MakeMove(CHESSMOVE* move)//模拟行棋
{
	CurPosition[move->From.x][move->From.y] = EMPTY;
	CurPosition[move->To.x][move->To.y] = move->ChessID;//该行棋方是已经找到的配对的行棋方,或者可以借鉴老程序的写法
	CurPosition[move->Bar.x][move->Bar.y] = BARRIER;
}

void UnMakeMove(CHESSMOVE* move)
{
	CurPosition[move->Bar.x][move->Bar.y] = EMPTY;
	CurPosition[move->To.x][move->To.y] = EMPTY;
	CurPosition[move->From.x][move->From.y] = move->ChessID;
}