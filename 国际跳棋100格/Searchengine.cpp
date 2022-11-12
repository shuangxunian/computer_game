#include "stdafx.h"
#include "Searchengine.h"


CSearchengine::CSearchengine()
{
	/*PossibleMoves = new MOVE *[20]; 
	for(int i=0;i<20;++i) 
		PossibleMoves[i] = new MOVE[200]; */
}

CSearchengine::~CSearchengine()
{
	/*for(int i=0;i<20;++i) 
		delete[] PossibleMoves[i]; 
	delete[] PossibleMoves; */
}


void CSearchengine::copy(int ChessBoard[10][10], int Color)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = ChessBoard[i][j];
			nowColor = Color;
		}
}

bool CSearchengine::BING_FOUR_DIRECTION_CAN_EAT(int nowX, int nowY,int Color)
{
	if (((board[nowX + directions[0][0]][nowY + directions[0][1]] == 3 - Color || board[nowX + directions[0][0]][nowY + directions[0][1]] == 3 - Color + 2) && board[nowX + 2 * directions[0][0]][nowY + 2 * directions[0][1]] == EMPTY&&nowX + 2 * directions[0][0] >= 0 && nowX + 2 * directions[0][0]<10 && nowY + 2 * directions[0][1] >= 0 && nowY + 2 * directions[0][1]<10) || \
		((board[nowX + directions[1][0]][nowY + directions[1][1]] == 3 - Color || board[nowX + directions[1][0]][nowY + directions[1][1]] == 3 - Color + 2) && board[nowX + 2 * directions[1][0]][nowY + 2 * directions[1][1]] == EMPTY&&nowX + 2 * directions[1][0] >= 0 && nowX + 2 * directions[1][0]<10 && nowY + 2 * directions[1][1] >= 0 && nowY + 2 * directions[1][1]<10) ||
		((board[nowX + directions[2][0]][nowY + directions[2][1]] == 3 - Color || board[nowX + directions[2][0]][nowY + directions[2][1]] == 3 - Color + 2) && board[nowX + 2 * directions[2][0]][nowY + 2 * directions[2][1]] == EMPTY&&nowX + 2 * directions[2][0] >= 0 && nowX + 2 * directions[2][0]<10 && nowY + 2 * directions[2][1] >= 0 && nowY + 2 * directions[2][1]<10) ||
		((board[nowX + directions[3][0]][nowY + directions[3][1]] == 3 - Color || board[nowX + directions[3][0]][nowY + directions[3][1]] == 3 - Color + 2) && board[nowX + 2 * directions[3][0]][nowY + 2 * directions[3][1]] == EMPTY&&nowX + 2 * directions[3][0] >= 0 && nowX + 2 * directions[3][0]<10 && nowY + 2 * directions[3][1] >= 0 && nowY + 2 * directions[3][1]<10))
		return true;
	else
		return false;
}

bool CSearchengine::KING_FOUR_DIRECTION_CAN_EAT(int nowX, int nowY, int Color)
{
	int howManyDirectionsCanEat = 0;
	int recordX = nowX;
	int recordY = nowY;
	for (int i = 0; i<4; i++)
	{
		while (board[nowX + directions[i][0]][nowY + directions[i][1]] == EMPTY&&nowX + directions[i][0] >= 0 && nowX + directions[i][0]<10 && nowY + directions[i][1] >= 0 && nowY + directions[i][1]<10)
		{
			nowX += directions[i][0];
			nowY += directions[i][1];
		}
		nowX += directions[i][0];
		nowY += directions[i][1];
		if ((board[nowX][nowY] == 3 - Color || board[nowX][nowY] == 3 - Color + 2) && board[nowX + directions[i][0]][nowY + directions[i][1]] == EMPTY&&nowX + directions[i][0] >= 0 && nowX + directions[i][0]<10 && nowY + directions[i][1] >= 0 && nowY + directions[i][1]<10)
			howManyDirectionsCanEat++;
		nowX = recordX;
		nowY = recordY;
	}
	if (howManyDirectionsCanEat)
		return true;
	else
		return false;
}

bool CSearchengine::IsBingCanMove(int nowX, int nowY, int Color)
{
	if (board[nowX + directions[Color-1][0]][nowY + directions[Color-1][1]] == EMPTY&&nowX + directions[Color-1][0] >= 0 && nowX + directions[Color-1][0] < 10 && nowY + directions[Color-1][1] >= 0 && nowY + directions[Color-1][1] < 10)
		return true;
	else if (board[nowX + directions[Color + 1][0]][nowY + directions[Color + 1][1]] == EMPTY&&nowX + directions[Color + 1][0] >= 0 && nowX + directions[Color + 1][0] < 10 && nowY + directions[Color + 1][1] >= 0 && nowY + directions[Color + 1][1] < 10)
		return true;
	else return false;

}

bool CSearchengine::IsKingCanMove(int X, int Y, int Color)
{
	int nowX=X, nowY=Y;
	for (int i = 0; i < 4; i++)
	{
		if (board[nowX + directions[i][0]][nowY + directions[i][1]] == EMPTY&&nowX + directions[i][0] >= 0 && nowX + directions[i][0] < 10 && nowY + directions[i][1] >= 0 && nowY + directions[i][1] < 10)
			return true;
	}
	return false;
}

int CSearchengine::IsGameOver()
{
	int ColorWin=0, unColorWin=0;
	for (int j = 0; j < 10;j++)
		for (int i = 0; i < 10; i++)
		{
			if (board[i][j] == nowColor)
			{
				if (IsBingCanMove(i, j, nowColor) || BING_FOUR_DIRECTION_CAN_EAT(i, j, nowColor))
					ColorWin++;
			}
			if (board[i][j] == nowColor + 2)
			{
				if (IsKingCanMove(i, j, nowColor) || KING_FOUR_DIRECTION_CAN_EAT(i, j, nowColor))
					ColorWin++;
			}
			if (board[i][j] ==3- nowColor)
			{
				if (IsBingCanMove(i, j, 3-nowColor) || BING_FOUR_DIRECTION_CAN_EAT(i, j, 3-nowColor))
					unColorWin++;
			}
			if (board[i][j] == 3-nowColor + 2)
			{
				if (IsKingCanMove(i, j, 3-nowColor) || KING_FOUR_DIRECTION_CAN_EAT(i, j, 3-nowColor))
					unColorWin++;
			}
		}
	if (ColorWin == 0)
		return -99999;
	else 
		if(unColorWin ==0)
			return 99999;
	if (ColorWin > 0 && unColorWin > 0)
		return 3;  //Ã»ÓÐÖÕ¾Ö
	return 4;
	
}

void CSearchengine::goAMove(int board[10][10],MOVE aMove)
{
	int nowX=aMove.move_star_x;
	int nowY=aMove.move_star_y;
	int stopX=aMove.move_stop_x;
	int stopY=aMove.move_stop_y;
	int record=board[nowX][nowY];
	board[nowX][nowY]=EMPTY;
	if(aMove.eatHowMany>0)
	{
		for(int i=0;i<aMove.eatHowMany;i++)
		{
			board[aMove.eatPoint[i][0]][aMove.eatPoint[i][1]]=EMPTY;
		}
	}
	board[stopX][stopY]=record;
	if(stopY==0&&record==WHITE)board[stopX][stopY]=W_KING;
	if(stopY==9&&record==BLACK)board[stopX][stopY]=B_KING;
}

void CSearchengine::goBack(int board[10][10],MOVE aMove)
{
	int nowX=aMove.move_star_x;
	int nowY=aMove.move_star_y;
	int stopX=aMove.move_stop_x;
	int stopY=aMove.move_stop_y;
	int record=board[nowX][nowY];
	board[nowX][nowY]=aMove.Kind;
	if(aMove.eatHowMany>0)
	{
		for(int i=0;i<aMove.eatHowMany;i++)
		{
			board[aMove.eatPoint[i][0]][aMove.eatPoint[i][1]]=aMove.eatKind[i];
		}
	}
	board[stopX][stopY]=EMPTY;
}