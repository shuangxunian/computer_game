#include"MoveGenerator.h"
#include<windows.h>
#include<string.h>

int Offset[8][2] = { 0 };
int MoveCount = 0;
CHESSMOVE MoveList[8][2500] = { 0 };

void InitOffset()
{
	Offset[0][0] = 0; Offset[0][1] = -1;//正左方偏移，依次类推
	Offset[1][0] = -1; Offset[1][1] = -1;
	Offset[2][0] = -1; Offset[2][1] = 0;
	Offset[3][0] = -1; Offset[3][1] = 1;
	Offset[4][0] = 0; Offset[4][1] = 1;
	Offset[5][0] = 1; Offset[5][1] = 1;
	Offset[6][0] = 1; Offset[6][1] = 0;
	Offset[7][0] = 1; Offset[7][1] = -1;
}

int CreatePossibleMove(int Board[12][12], int depth, int Side)
{
	int i, j, k, l, t;
	int Position[12][12] = { 0 };   //临时存储的棋盘，防止对原棋盘误操作
	InitOffset();       //调用偏移量
	memcpy(Position, Board, 144 * sizeof(int));   //复制棋盘信息
	MoveCount = 0;  //计步器清零
	for ( i = 1; i <= 10; i++)
	{
		for ( j = 1; j <= 10; j++)
		{
			if (Position[i][j] == Side)//找到棋子
			{
				k = i;
				l = j;
				for ( t = 0; t < 8; t++)//该棋子的周围八个方向进行偏移
				{
					//尝试每个偏移路径
					k += Offset[t][0];//进行偏移
					l += Offset[t][1];
					while (Position[k][l] == EMPTY)
					{
						CreatePossibleBar(Position, depth, Side, i, j, k, l);
						////继续沿该路径偏移
						k += Offset[t][0];
						l += Offset[t][1];
					}
					k = i;//恢复棋子坐标，为下一个方向的偏移做准备
					l = j;
				}
			}
		}
	}
	return MoveCount;//返回生成的步法总数
}

void CreatePossibleBar(int Board[12][12], int depth, int Side, int FromX, int FromY, int ToX, int ToY)
{
	int k, l;
	//模拟行棋落子的过程，便于对放置障碍判断
	Board[FromX][FromY] = EMPTY;
	Board[ToX][ToY] = Side;
	k = ToX;
	l = ToY;
	for (int t = 0; t < 8; t++)
	{
		k += Offset[t][0];
		l += Offset[t][1];
		while (Board[k][l] == EMPTY)
		{
			//存放步法
			MoveList[depth][MoveCount].From.x = FromX;//存放起始位置坐标
			MoveList[depth][MoveCount].From.y = FromY;
			MoveList[depth][MoveCount].To.x = ToX;//存放落子位置坐标
			MoveList[depth][MoveCount].To.y = ToY;
			MoveList[depth][MoveCount].Bar.x = k;//存放障碍的坐标
			MoveList[depth][MoveCount].Bar.y = l;
			MoveList[depth][MoveCount].ChessID = Side;
			MoveList[depth][MoveCount].score = 0;
			//计步器计数更新
			MoveCount++;
			
			//继续沿该路径偏移
			k += Offset[t][0];
			l += Offset[t][1];
		}
		k = ToX;//恢复坐标，为下一个方向的偏移做准备
		l = ToY;
	}
	//恢复棋盘，撤销模拟的行棋
	Board[ToX][ToY] = EMPTY;
	Board[FromX][FromY] = Side;
}
