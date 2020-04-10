#include "stdafx.h"
#include "CBoard.h"

using namespace std;

static double RED_VALUE[5][5] = { {0,1,1,1,1},{1,2,2,2,2.5},{1,2,4,4,5},{1,2,4,8,10},{1,2.5,5,10,100} };
static double BLUE_VALUE[5][5] = { {100,10,5,2.5,1} ,{10,8,4,2,1},{5,4,4,2,1},{2.5,2,2,2,1},{1,1,1,1,0} };

void Loc::Set(int x, int y)
{
	Loc::x = x;
	Loc::y = y;
}

Board::Board(int arr[5][5])
{
	RedState[0] = 6;
	BlueState[0] = 6;
	for (int i = 1; i <= 6; i++)
	{
		RedState[i] = 1;
		BlueState[i] = 1;
	}
	step = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			board[i][j] = arr[i][j];
}

void Board::show()
{
	system("cls");
	cout << "     ----------欢迎来到命运之石----------" << endl;
	cout << "\n     步数:" << step << endl;
	cout << "\n             1   2   3   4   5            " << endl;
	cout << "          ┌───┬───┬───┬───┬───┐          " << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "         " << (i + 1) << "│";
		for (int j = 0; j < 5; j++)
		{
			if (board[i][j] > 0)
				cout << " R" << board[i][j] << "│";
			else if(board[i][j] < 0)
				cout << " B" << -board[i][j] << "│";
			else cout << "   │";

		}
		cout << endl;
		if (i == 4) 
			cout << "          └───┴───┴───┴───┴───┘ " << endl;
		else       
			cout << "          ├───┼───┼───┼───┼───┤" << endl;
	}
	cout << endl;
}

Loc Board::GetPiece(int piece)
{
	Loc loc;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (piece == board[i][j])
			{
				loc.Set(i, j);
				break;
			}
	return loc;
}

bool Board::IsAlive(int piece)
{
	if (piece > 0)
		return RedState[piece];
	else
		return BlueState[-piece];
}

int Board::GetPieceFaction(int	piece)
{
	if (piece > 0)
		return 1;              //红方为1
	else if (piece < 0)
		return -1;			   //蓝方为-1
	else
		return 0;
}

bool Board::GetPieceLegality(Loc ploc)
{
	if (ploc.x >= 0 && ploc.x < 5 && ploc.y >= 0 && ploc.y < 5)
		return true;
	return false;
}

int Board::GetLargerPiece(int piece)
{
	if (piece > 0)
	{
		for (int i = piece + 1; i <= 6; i++)
			if (IsAlive(i))
				return i;
	}
	else if(piece < 0)
	{
		for (int i = piece - 1; i >= -6; i--)
			if (IsAlive(i))
				return i;
	}
	return 0;
}

int Board::GetSmallerPiece(int piece)
{
	if (piece > 0)
	{
		for (int i = piece - 1; i > 0; i--)
			if (IsAlive(i))
				return i;
	}
	else if (piece < 0)
	{
		for (int i = piece + 1; i < 0; i++)
			if (IsAlive(i))
				return i;
	}
	return 0;
}

bool Board::Is_Waning_Zone(Loc loc)
{
	int f=GetPieceFaction(board[loc.x][loc.y]);
	if ((f*loc.x >= 2 * f + 1) && (f*loc.y >= 2 * f + 1))				//禁区公式
		return true;
	else
		return false;
}

int Board::GetAllMoves(MovePiece* Moves, int piece)
{
	int MoveNum = 0;
	int Direction[3][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 } };				//三个走子方向
	int f = GetPieceFaction(piece);										//向量(棋子阵营)
	Loc nloc;															//移动后坐标
	Loc ploc;															//棋子坐标
	if (IsAlive(piece))
	{
		ploc = GetPiece(piece);

/////////////////////////////////////////////////////////////
		if (Is_Waning_Zone(ploc))					     ////
		{											     ////
			nloc.Set(2 + 2 * f, 2 + 2 * f);			     ////
			Moves[0] = { nloc,piece };					 ////禁区定式
			return 1;									 ////
		}												 ////
/////////////////////////////////////////////////////////////
		for (int i = 0; i < 3; i++)
		{
			nloc.Set(ploc.x + f*Direction[i][0], ploc.y + f*Direction[i][1]);
			if (GetPieceLegality(nloc))
			{
				Moves[MoveNum] = { nloc,piece };
				MoveNum++;
			}
		}
	}
	else
	{
		int L = GetLargerPiece(piece);
		int S = GetSmallerPiece(piece);
		if (L != 0)
		{

			ploc = GetPiece(L);
/////////////////////////////////////////////////////////////
			if (Is_Waning_Zone(ploc))					 ////
			{											 ////
				nloc.Set(2 + 2 * f, 2 + 2 * f);		     ////
				Moves[0] = { nloc,L };				     ////禁区定式
				return 1;								 ////
			}											 ////
/////////////////////////////////////////////////////////////
			for (int i = 0; i < 3; i++)
			{
				nloc.Set(ploc.x + f*Direction[i][0], ploc.y + f*Direction[i][1]);
				if (GetPieceLegality(nloc))
				{
					Moves[MoveNum] = { nloc,L };
					MoveNum++;
				}
			}
		}
		if (S != 0)
		{
			ploc = GetPiece(S);
/////////////////////////////////////////////////////////////
			if (Is_Waning_Zone(ploc))					 ////
			{											 ////
				nloc.Set(2 + 2 * f, 2 + 2 * f);		     ////
				Moves[0] = { nloc,S };					 ////禁区定式
				return 1;								 ////
			}											 ////
/////////////////////////////////////////////////////////////
			for (int i = 0; i < 3; i++)
			{
				nloc.Set(ploc.x + f*Direction[i][0], ploc.y + f*Direction[i][1]);
				if (GetPieceLegality(nloc))
				{
					Moves[MoveNum] = { nloc,S };
					MoveNum++;
				}
			}
		}
	}
	return MoveNum;
}

int Board::Winner()
{
	if (BlueState[0] == 0 || board[4][4] > 0)
		return 1;				//红胜
	else if (RedState[0] == 0 || board[0][0] < 0)
		return -1;				//蓝胜
	else
		return 0;               //未分出胜负
}

void Board::Move(Loc nloc, int piece)
{
	Loc oloc = GetPiece(piece);
	int f = board[nloc.x][nloc.y];
	if (f > 0)
	{
		RedState[0]--;
		RedState[f]--;
	}
	if(f<0)
	{
		BlueState[0]--;
		BlueState[-f]--;
	}
	board[oloc.x][oloc.y] = 0;
	board[nloc.x][nloc.y] = piece;
	step++;
}

void print()
{
	cout << "\n             1   2   3   4   5            " << endl;
	cout << "          ┌───┬───┬───┬───┬───┐          " << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "         " << (i + 1) << "│";
		for (int j = 0; j < 5; j++)
			cout << "   │";
		cout << endl;
		if (i == 4)
			cout << "          └───┴───┴───┴───┴───┘ " << endl;
		else
			cout << "          ├───┼───┼───┼───┼───┤" << endl;
	}
	cout << endl;
}

double Board::GameState()											//进攻值：威胁值=1:0.17
{
	float Red_w[7] = { 0 }, Blue_w[7] = { 0 };
	double Red_Value = 0, Blue_Value = 0;
////////////////////////////////////////////////////////////////////////////////////////////////
//计算棋子移动概率（结果×6）
	for (int i = 1; i <= 6; i++)
	{
		if (RedState[i] == 0)
			continue;
		Red_w[i]++;
		for (int j = i - 1; RedState[j] == 0 && j >= 1; j--)
			Red_w[i]++;
		for (int j = i + 1; RedState[j] == 0 && j <= 6; j++)
			Red_w[i]++;
	}
	for (int i = 1; i <= 6; i++)
	{
		if (BlueState[i] == 0)
			continue;
		Blue_w[i]--;
		for (int j = i - 1; BlueState[j] == 0 && j >= 1; j--)
			Blue_w[i]--;
		for (int j = i + 1; BlueState[j] == 0 && j <= 6; j++)
			Blue_w[i]--;
	}
///////////////////////////////////////////////////////////////////////////////////////////////
//计算进攻值
	for(int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			if (board[i][j] > 0)
				Red_Value += Red_w[board[i][j]] * RED_VALUE[i][j];
			else if (board[i][j] < 0)
				Blue_Value += Blue_w[-board[i][j]] * BLUE_VALUE[i][j];
			//cout << Blue_w[-board[i][j]] << BLUE_VALUE[i][j] << Blue_Value << endl;
		}
	//cout << Red_Value << endl << Blue_Value << endl;
//////////////////////////////////////////////////////////////////////////////////////////////
//计算威胁值
	Loc loc, point_temp;
	double max, value_temp;
	int f;                        //阵营
	int Direction[3][2] = { { 1, 0 },{ 1, 1 },{ 0, 1 } };
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			max = 0;
			loc.Set(i, j);
			f = GetPieceFaction(GetPiece(loc));
			if (f > 0)             //红子
			{
				for (int i = 0; i < 3; i++)
				{
					point_temp.Set(loc.x + f*Direction[i][0], loc.y + f*Direction[i][1]);
					if (GetPiece(point_temp) < 0)
					{
						value_temp = Blue_w[-GetPiece(point_temp)] * BLUE_VALUE[point_temp.x][point_temp.y];
						max = (max < value_temp) ? max : value_temp;
					}
				}
				Red_Value -= (0.17 * max);
			}
			if (f < 0)				   //蓝子
			{
				for (int i = 0; i < 3; i++)
				{
					point_temp.Set(loc.x + f*Direction[i][0], loc.y + f*Direction[i][1]);
					if (GetPiece(point_temp) > 0)
					{
						value_temp = Red_w[GetPiece(point_temp)] * RED_VALUE[point_temp.x][point_temp.y];
						max = (max > value_temp) ? max : value_temp;
					}
				}
				Blue_Value -= (0.17 * max);
			}
		}
	return (Red_Value + Blue_Value);
}

int Board::GetPiece(Loc loc)
{
	if (GetPieceLegality(loc))
		return board[loc.x][loc.y];
	return 0;
}

int Board::GetStep()
{
	return step;
}

void Board::Regret_Game(stack<Move_Recording> &Recording, int step)
{
	Board::step -= step;
	while (step != 0)
	{
		Move_Recording temp = Recording.top();
		board[temp.start_move_piece.x][temp.start_move_piece.y] = board[temp.end_move_piece.x][temp.end_move_piece.y];
		board[temp.end_move_piece.x][temp.end_move_piece.y] = temp.covered_piece;
		step--;
		if (temp.covered_piece > 0)
		{
			RedState[0]++;
			RedState[temp.covered_piece] = 1;
		}
		else if (temp.covered_piece < 0)
		{
			BlueState[0]++;
			BlueState[-temp.covered_piece] = 1;
		}
		Recording.pop();
	}
}