#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"

using namespace std;

//基础类
void LOC::Set(int lx, int ly)
{
	x = lx;
	y = ly;
}
TURN::TURN()
{
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			Captual[i][j] = false;
			if (IfOddNumber(i) && IfOddNumber(j))//xy均为奇数，为格子
			{
				NewBoard[i][j] = BOX;
			}
			else if (IfEvenNumber(i) && IfOddNumber(j))//x偶数，y奇数，为行
			{
				NewBoard[i][j] = EDGE;
			}
			else if (IfOddNumber(i) && IfEvenNumber(j))//x奇数，y偶数，为行
			{
				NewBoard[i][j] = EDGE;
			}
			else//剩下的就是xy均偶数，为空点。
			{
				NewBoard[i][j] = EMPTY;
			}
		}
	}
}
void TURN::SetBoard(int Array[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			NewBoard[i][j] = Array[i][j];
		}
	}
}
void TURN::SetCaptualInfo(int OB[LEN][LEN], int NB[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			if (OB[i][j] == EDGE&&NB[i][j] != EDGE)
				Captual[i][j] = true;
		}
	}
}

//定义棋盘类
Board::Board()
{
	Step = 0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(IfOddNumber(i)&&IfOddNumber(j))//xy均为奇数，为格子
			{
				board[i][j] = BOX;
			}
			else if(IfEvenNumber(i)&&IfOddNumber(j))//x偶数，y奇数，为行
			{
				board[i][j] = EDGE;
			}
			else if(IfOddNumber(i)&&IfEvenNumber(j))//x奇数，y偶数，为行
			{
				board[i][j] = EDGE;
			}
			else//剩下的就是xy均偶数，为空点。
			{
				board[i][j] = EMPTY;
			}
		}
	}
}
Board::Board(int Array[LEN][LEN],int step)
{
	Step = step;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			board[i][j]=Array[i][j];
		}
	}
}
int Board::GetEmptyEdges()
{
	int lines=0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(board[i][j]==EDGE)
				lines++;
		}
	}
	return lines;
}
int Board::GetEmptyBoxes()
{
	int boxes=0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(board[i][j]==BOX)
				boxes++;
		}
	}
	return boxes;
}
void Board::PrintBoard()
{
	//打印棋盘函数
	cout<<"  ";
	for(int i = 0;i<=(LEN-1);i++)
	{
		printf("%2d",i);
	}
	cout<<"\n";
	for(int j = 0;j<LEN;j++)
	{
		printf("%2d",j);
		for(int i = 0;i<LEN;i++)
		{
			if(board[i][j]==BOX)
			{
				cprintf("  ",8);
			}
			else if(board[i][j]==RED_BOX)
			{
				cprintf("■",12);
			}
			else if(board[i][j]==BLUE_BOX)
			{
				cprintf("■",9);
			}
			else if(board[i][j]==EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
				{
					cout << "  ";
					//cprintf("┃",15);
				}
				else
				{
					cout << "  ";
					//cprintf("━",15);
				}
			}
			else if(board[i][j]==RED_EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
				{
					cprintf("┃",12);
				}
				else
				{
					cprintf("━",12);
				}
			}
			else if(board[i][j]==BLUE_EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
				{
					cprintf("┃",9);
				}
				else
				{
					cprintf("━",9);
				}
			}
			else if(board[i][j]==EMPTY)
			{
				cprintf("○", 15);
			}
		}
		printf("%2d",j);
		cout<<"\n";
	}
	cout<<"  ";
	for(int i = 0;i<=(LEN-1);i++)
	{
		printf("%2d",i);
	}
	cout<<"\n";
}
void Board::ShowBoard()
{
	//打印棋盘函数
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
	for (int j = 0; j<LEN; j++)
	{
		printf("%2d", j);
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == BOX)
			{
				cprintf("  ", 8);
			}
			else if (board[i][j] == RED_BOX)
			{
				cprintf("■", 12);
			}
			else if (board[i][j] == BLUE_BOX)
			{
				cprintf("■", 9);
			}
			else if (board[i][j] == EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("┃", 15);
				}
				else
				{
					cprintf("━", 15);
				}
			}
			else if (board[i][j] == RED_EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("┃", 12);
				}
				else
				{
					cprintf("━", 12);
				}
			}
			else if (board[i][j] == BLUE_EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("┃", 9);
				}
				else
				{
					cprintf("━", 9);
				}
			}
			else if (board[i][j] == EMPTY)
			{
				if (i == 0 && j == 0)
				{
					cprintf("┌", 8);
				}
				else if (i == (LEN - 1) && j == 0)
				{
					cprintf("┐", 8);
				}
				else if (i == 0 && j == (LEN - 1))
				{
					cprintf("└", 8);
				}
				else if (i == (LEN - 1) && j == (LEN - 1))
				{
					cprintf("┘", 8);
				}
				else if (i == 0 && j != (LEN - 1) && j != 0)
				{
					cprintf("├", 8);
				}
				else if (i == (LEN - 1) && j != (LEN - 1) && j != 0)
				{
					cprintf("┤", 8);
				}
				else if (j == 0 && i != (LEN - 1) && i != 0)
				{
					cprintf("┬", 8);
				}
				else if (j == (LEN - 1) && i != (LEN - 1) && i != 0)
				{
					cprintf("┴", 8);
				}
				else
				{
					cprintf("┼", 8);
				}
			}
		}
		printf("%2d", j);
		cout << "\n";
	}
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
}
void Board::ShowLiberties()
{
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			int bl = GetBoxLiberties(i * 2 - 1, j * 2 - 1);
			if (bl == 0)
			{
				cprintNum(8, bl);
				cout << " ";
			}
			else if (bl == 1)
			{
				cprintNum(2,bl);
				cout << " ";
			}
			else if (bl == 2)
			{
				cprintNum(14,bl);
				cout << " ";
			}
			else
			{
				cprintNum(5,bl);
				cout << " ";
			}
		}
		cout << endl;
	}
}
int Board::GetEdgeOwner(int x,int y)
{
	int num = board[x][y];
	if(num!=EDGE&&num!=RED_EDGE&&num!=BLUE_EDGE)
	{
		cout<<"GetEdgeOwnerPlayer函数输入了一个错误的地址";
		return 0;
	}
	if(num>0)
	{
		return RED;
		//cout<<x<<" "<<y<<" EdgeOwner = RED"<<endl;
	}
	if(num<0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" EdgeOwner = BLUE"<<endl;
	}
	return EMPTY;
}
int Board::GetBoxOwner(int x,int y)
{
	int num = board[x][y];
	if(num!=BOX&&num!=RED_BOX&&num!=BLUE_BOX)
	{
		cout<<"GetEdgeOwnerPlayer函数输入了一个错误的地址";
		return 0;
	}
	if(num>0)
	{
		return RED;
		//cout<<x<<" "<<y<<" BoxOwner = RED"<<endl;
	}
	if(num<0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" BoxOwner = BLUE"<<endl;
	}
	return EMPTY;
}
int Board::GetBoxLiberties(int x,int y)
{
	int num = board[x][y];
	if(num!=BOX&&num!=RED_BOX&&num!=BLUE_BOX)
	{
		cout<<"GetBoxLiberties函数输入了一个错误的地址";
		return 0;
	}
	int lib = 0;
	if(board[x-1][y]==EDGE)
		lib++;
	if(board[x][y+1]==EDGE)
		lib++;
	if(board[x+1][y]==EDGE)
		lib++;
	if(board[x][y-1]==EDGE)
		lib++;
	return lib;
}
bool Board::GetEdgeLegality(int x,int y)
{
	if(board[x][y]==EDGE)
		return true;
	return false;
}
int Board::GetPlayerBoxes(int player)
{
	int boxes=0;
	if(player==RED)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==RED_BOX)
					boxes++;
			}
		}
	}
	if(player==BLUE)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==BLUE_BOX)
					boxes++;
			}
		}
	}
	return boxes;
}
int Board::GetPlayerEdges(int player)
{
	int lines=0;
	if(player==RED)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==RED_EDGE)
					lines++;
			}
		}
	}
	if(player==BLUE)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==BLUE_EDGE)
					lines++;
			}
		}
	}
	return lines;
}
void Board::SetBoard(int Array[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			board[i][j] = Array[i][j];
		}
	}
}
int Board::Winner()
{
	int RedBoxes = GetPlayerBoxes(RED);
	int BlueBoxes = GetPlayerBoxes(BLUE);
	if((RedBoxes+BlueBoxes)>=BOXNUM)
	{
		if(RedBoxes>BlueBoxes)
			return RED;
		else
			return BLUE;
	}
	return 0;
}
void Board::LoadBoard()
{
	for (int j = 0; j < LEN; j++)
	{
		for (int i = 0; i < LEN; i++)
		{
			if (board[i][j] != 0 && ((IfOddNumber(j) && IfEvenNumber(i)) || (IfEvenNumber(j) && IfOddNumber(i))))
			{
				system("cls");
				PrintBoard();
				cout << "请输入[" << i << " , " << j << "]这条边的属主【0，1，2】" << endl;
				int Num;
				cin >> Num;
				if (Num == 1)
					board[i][j] = 1;
				else if (Num == 2)
					board[i][j] = -1;
				else if (Num == 99)
					goto end;
				else
					board[i][j] = 10;
			}
		}
	}
	for (int j = 0; j < LEN; j++)
	{
		for (int i = 0; i < LEN; i++)
		{
			if (board[i][j] != 0 &&IfOddNumber(j) && IfOddNumber(i))
			{
				system("cls");
				PrintBoard();
				cout << "请输入[" << i << " , " << j << "]这个格子的属主【0，1，2】" << endl;
				int Num;
				cin >> Num;
				if (Num == 1)
					board[i][j] = 2;
				else if (Num == 2)
					board[i][j] = -2;
				else if (Num == 99)
					goto end;
				else
					board[i][j] = 20;
			}
		}
	}
	end:
	PrintBoard();
	cout << "Load Board Finish!" << endl;
}

//局面估值
bool Board::GetFreeBoxBool(int bx, int by)
{
	//注意的是，输入的X与Y是Box实际地址
	if (1<=bx&&LEN-2>=bx&&1<=by&&LEN-2>=by&&IfOddNumber(bx)&&IfOddNumber(by))//Box位置编号必须正确
	{
		if (GetBoxLiberties(bx, by) >= 3)
			return true;
		else
			return false;
	}
	else
	{
		cout << "Wrong Number In <GetFreeBoxBool> Function";
		system("pause");
	}
	return false;
}
int Board::GetFreeEdgeNum()
{
	int EdgeNum = 0;
	//得到所有的自由边
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		//先判定头部第一个格子与外界的边是否自由边
		if (GetFreeBoxBool(1, y) && GetEdgeLegality(0, y))//第一个为交格而且与外界交互的边为空边
		{
			EdgeNum++;//总自由边数目自增1
		}
		//循环判定中间的几个格子
		for (int x = 1; x < LEN - 3; x = x + 2)//x轴
		{
			if (GetFreeBoxBool(x, y) && GetFreeBoxBool(x + 2, y) && GetEdgeLegality(x + 1, y))
			{
				EdgeNum++;//总自由边数目自增1
			}
		}
		//判断末尾的格子
		if (GetFreeBoxBool(LEN - 2, y) && GetEdgeLegality(LEN - 1, y))//最后一个为交格且与外界交互的边为空边
		{
			EdgeNum++;//总自由边数目自增1
		}

		//XY替换，再进行一次判定

		//先判定头部第一个格子与外界的边是否自由边
		if (GetFreeBoxBool(y, 1) && GetEdgeLegality(y, 0))//第一个为交格而且与外界交互的边为空边
		{
			EdgeNum++;//总自由边数目自增1
		}
		//循环判定中间的几个格子
		for (int x = 1; x < LEN - 3; x = x + 2)//x轴
		{
			if (GetFreeBoxBool(y, x) && GetFreeBoxBool(y, x + 2) && GetEdgeLegality(y, x + 1))
			{
				EdgeNum++;//总自由边数目自增1
			}
		}
		//判断末尾的格子
		if (GetFreeBoxBool(y, LEN - 2) && GetEdgeLegality(y, LEN - 1))//最后一个为交格且与外界交互的边为空边
		{
			EdgeNum++;//总自由边数目自增1
		}
	}
	return EdgeNum;//返回自由边的总数
}
bool Board::GetCTypeBoxBool(int bx, int by)
{
	//注意的是，输入的X与Y是Box实际地址
	if (bx >= 1 && bx <= LEN - 2 && by >= 1 && by <= LEN - 2 && IfOddNumber(bx) && IfOddNumber(by))//Box位置编号必须正确
	{
		if (GetBoxLiberties(bx, by) ==DEADBOX)
			return true;
		else
			return false;
	}
	return false;
}
bool Board::GetCTypeBox(int Player,bool Msg)
{
	//仅用在前期，用于在搜索的时候占领所有C型格
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		for (int x = 1; x < LEN - 1; x = x + 2)//x轴
		{
			if (GetCTypeBoxBool(x,y))
			{
				int lx, ly;
				if (board[x + 1][y] == EDGE)
				{
					Move(x + 1, y, Player);
					lx = x + 1;
					ly = y;
				}
				else if (board[x - 1][y] == EDGE)
				{
					Move(x - 1, y, Player);
					lx = x - 1;
					ly = y;
				}
				else if (board[x][y + 1] == EDGE)
				{
					Move(x, y + 1, Player);
					lx = x;
					ly = y+1;
				}
				else if (board[x][y - 1] == EDGE)
				{
					Move(x, y - 1, Player);
					lx = x;
					ly = y-1;
				}
				if (Msg)
				{
					MoveMsg(lx, ly, Player);
				}
				return true;//占据之后就返回真
			}
		}
	}
	return false;//返回假
}
bool Board::GetLongCTypeBoxExist()
{
	for (int by = 1; by < LEN - 1; by = by + 2)
	{
		for (int bx = 1; bx < LEN - 1; bx = bx + 2)
		{
			if (GetBoxLiberties(bx, by) == DEADBOX)//如果存在自由度为1的格子
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = bx + Dir[n][0] + Dir[n][0];
					int nby = by + Dir[n][1] + Dir[n][1];
					if (board[ex][ey] == EDGE&&nbx>=1&&nbx<=LEN-2&&nby>=1&&nby<=LEN-2)
					{
						if (GetBoxLiberties(nbx, nby) == CHAINBOX)
							return true;
					}
				}
			}
		}
	}
	return false;
}
void Board::GetAllCTypeBoxes(int Player,bool Msg)
{
	for (;;)//直到无法占据CTypeBox了就结束
	{
		if (!GetCTypeBox(Player, Msg))
			break;
	}
}
LOC Board::FindNextBox(LOC FoundBox, LOC LastBox)
{
	//查找某个链格,给出的坐标可能包含超区坐标。请使用前利用GetBoxType鉴定
	int fbx = FoundBox.x * 2 - 1;
	int fby = FoundBox.y * 2 - 1;
	LOC AimBox;
	if (board[fbx + 1][fby] == EDGE && ((FoundBox.x + 1) != LastBox.x || (FoundBox.y) != LastBox.y))//方向为0度
	{
		AimBox.Set(FoundBox.x + 1, FoundBox.y);
		return AimBox;
	}
	else if (board[fbx][fby + 1] == EDGE && ((FoundBox.x) != LastBox.x || (FoundBox.y + 1) != LastBox.y))//方向为90度
	{
		AimBox.Set(FoundBox.x, FoundBox.y + 1);
		return AimBox;
	}
	else if (board[fbx - 1][fby] == EDGE && ((FoundBox.x - 1) != LastBox.x || (FoundBox.y) != LastBox.y))//方向为180度
	{
		AimBox.Set(FoundBox.x - 1, FoundBox.y);
		return AimBox;
	}
	else if (board[fbx][fby - 1] == EDGE && ((FoundBox.x) != LastBox.x || (FoundBox.y - 1) != LastBox.y))//方向为270度
	{
		AimBox.Set(FoundBox.x, FoundBox.y - 1);
		return AimBox;
	}
	AimBox.Set(0, 0);//代表错误
	return AimBox;
}

//后期计算
LOC Board::GetDoubleCrossLoc(int Player)
{
	//得到可以制造双交的那个边的坐标
	for (int by = 1; by < LEN - 1; by = by + 2)
	{
		for (int bx = 1; bx < LEN - 1; bx = bx + 2)
		{
			if (GetLongCTypeBoxBool(bx, by))
			{
				//现在的bx和by就是实际的格子
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = bx + Dir[n][0] + Dir[n][0];
					int nby = by + Dir[n][1] + Dir[n][1];
					if (board[ex][ey] == EDGE&&nbx >= 1 && nbx <= LEN - 2 && nby >= 1 && nby <= LEN - 2)
					{
						//现在ex,ey是其公共边，nbx,nby是doublecross的末端CHAINBOX
						for (int n = 0; n < 4; n++)
						{
							int nex = nbx + Dir[n][0];
							int ney = nby + Dir[n][1];
							if (board[nex][ney] == EDGE && (nex != ex || ney != ey))//空边而且不是中间的公共边(ex,ey)
							{
								LOC k;
								k.Set(nex, ney);
								return k;
							}
						}
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}
bool Board::GetCTypeBoxLimit(int Player,bool Msg)
{
	//仅用在前期，用于在搜索的时候占领所有C型格
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		for (int x = 1; x < LEN - 1; x = x + 2)//x轴
		{
			if (GetCTypeBoxBool(x, y))
			{
				int lx, ly;
				if (board[x + 1][y] == EDGE&&!GetCTypeBoxBool(x+2, y))
				{
					Move(x + 1, y, Player);
					lx = x + 1;
					ly = y;
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//占据之后就返回真
				}
				else if (board[x - 1][y] == EDGE&&!GetCTypeBoxBool(x - 2, y))
				{
					Move(x - 1, y, Player);
					lx = x - 1;
					ly = y;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//占据之后就返回真
				}
				else if (board[x][y + 1] == EDGE &&!GetCTypeBoxBool(x , y+2))
				{
					Move(x, y + 1, Player);
					lx = x;
					ly = y + 1;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//占据之后就返回真
				}
				else if (board[x][y - 1] == EDGE &&!GetCTypeBoxBool(x, y-2))
				{
					Move(x, y - 1, Player);
					lx = x;
					ly = y - 1;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//占据之后就返回真
				}
			}
		}
	}
	return false;//返回假
}

//招法
bool Board::Move(int x, int y, int owner)
{
	bool GetBoxes = false;
	Step++;
	if (GetEdgeLegality(x, y) && (owner == RED || owner == BLUE) && x >= 0 && x <= (LEN - 1) && y >= 0 && y <= (LEN - 1))
	{
		int ex, ey;
		board[x][y] = owner;//把这个边填满
		//检查一下四个方向有没有被填满但还没有被占领的各自，占领他。
		if (IfOddNumber(x) && IfEvenNumber(y))//X奇数Y偶数，横行
		{
			if (y == 0)
			{
				ex = x; ey = y + 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else if (y == LEN - 1)
			{
				ex = x; ey = y - 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else
			{
				ex = x; ey = y + 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
				ex = x; ey = y - 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
		}
		else if (IfEvenNumber(x) && IfOddNumber(y))//X偶数Y奇数，竖行
		{
			if (x == 0)
			{
				ex = x + 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else if (x == LEN - 1)
			{
				ex = x - 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else
			{
				ex = x + 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
				ex = x - 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
		}
		return GetBoxes;
	}
	return GetBoxes;
}
int Board::GetAllMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	for (int j = 0; j<LEN; j++)
	{
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == EDGE)
			{
				Moves[MoveNum].Set(i, j);
				MoveNum++;											//总数目自增
			}
		}
	}
	return MoveNum;
}
int Board::GetFreeMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	//得到所有的自由边
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		//先判定头部第一个格子与外界的边是否自由边
		if (GetFreeBoxBool(1, y) && GetEdgeLegality(0, y))//第一个为交格而且与外界交互的边为空边
		{
			Moves[MoveNum].Set(0, y);//保存坐标
			MoveNum++;//总自由边数目自增1
		}
		//循环判定中间的几个格子
		for (int x = 1; x < LEN - 3; x = x + 2)//x轴
		{
			if (GetFreeBoxBool(x, y) && GetFreeBoxBool(x + 2, y) && GetEdgeLegality(x + 1, y))
			{
				Moves[MoveNum].Set(x + 1, y);//保存坐标
				MoveNum++;//总自由边数目自增1
			}
		}
		//判断末尾的格子
		if (GetFreeBoxBool(LEN - 2, y) && GetEdgeLegality(LEN - 1, y))//最后一个为交格且与外界交互的边为空边
		{
			Moves[MoveNum].Set(LEN - 1, y);//保存坐标
			MoveNum++;//总自由边数目自增1
		}

		//XY替换，再进行一次判定

		//先判定头部第一个格子与外界的边是否自由边
		if (GetFreeBoxBool(y, 1) && GetEdgeLegality(y, 0))//第一个为交格而且与外界交互的边为空边
		{
			Moves[MoveNum].Set(y, 0);//保存坐标
			MoveNum++;//总自由边数目自增1
		}
		//循环判定中间的几个格子
		for (int x = 1; x < LEN - 3; x = x + 2)//x轴
		{
			if (GetFreeBoxBool(y, x) && GetFreeBoxBool(y, x + 2) && GetEdgeLegality(y, x + 1))
			{
				Moves[MoveNum].Set(y, x + 1);//保存坐标
				MoveNum++;//总自由边数目自增1
			}
		}
		//判断末尾的格子
		if (GetFreeBoxBool(y, LEN - 2) && GetEdgeLegality(y, LEN - 1))//最后一个为交格且与外界交互的边为空边
		{
			Moves[MoveNum].Set(y, LEN - 1);//保存坐标
			MoveNum++;//总自由边数目自增1
		}
	}
	return MoveNum;//返回自由边的总数
}
int Board::GetFilterMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//若为空白边
			{
				//Board Test(board,Step);
				int BoardSave[LEN][LEN];
				int CurrentStep = Step;
				BoardCopy(board, BoardSave);	//保存一下
				Move(x, y, RED);				//玩家模拟走一步试试

				if (IfOddNumber(x) && IfEvenNumber(y))//X奇数Y偶数，横行
				{
					if (y == 0)
					{
						if (!GetLongCTypeBoxBool(x, y + 1))//如果下面的那个格子没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
					else if (y == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x, y - 1))//如果上面的那个格子没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x, y + 1) && !GetLongCTypeBoxBool(x, y - 1))//如果上下的格子都没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
				}
				else//竖行
				{
					if (x == 0)
					{
						if (!GetLongCTypeBoxBool(x + 1, y))//如果右边的那个格子没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
					else if (x == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x - 1, y))//如果左边的那个格子没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x + 1, y) && !GetLongCTypeBoxBool(x - 1, y))//如果左右两边的格子都没问题的话，这个招法也没问题
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//总数目自增
						}
					}
				}

				/*if (!GetLongCTypeBoxExist())	//若不存在长死格，也就是只有单链或者没有死格
				{
				//存入TURN
				Moves[MoveNum].Set(x, y);
				MoveNum++;//总数目自增
				}*/
				SetBoard(BoardSave);			//还原
				Step = CurrentStep;				//还原
			}
		}
	}

	return MoveNum;
}
int Board::GetFilterMoveNum()
{
	int MoveNum = 0;
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//若为空白边
			{
				//Board Test(board,Step);
				int BoardSave[LEN][LEN];
				int CurrentStep = Step;
				BoardCopy(board, BoardSave);	//保存一下
				Move(x, y, RED);				//玩家模拟走一步试试

				if (IfOddNumber(x) && IfEvenNumber(y))//X奇数Y偶数，横行
				{
					if (y == 0)
					{
						if (!GetLongCTypeBoxBool(x, y + 1))//如果下面的那个格子没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
					else if (y == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x, y - 1))//如果上面的那个格子没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x, y + 1) && !GetLongCTypeBoxBool(x, y - 1))//如果上下的格子都没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
				}
				else//竖行
				{
					if (x == 0)
					{
						if (!GetLongCTypeBoxBool(x + 1, y))//如果右边的那个格子没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
					else if (x == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x - 1, y))//如果左边的那个格子没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x + 1, y) && !GetLongCTypeBoxBool(x - 1, y))//如果左右两边的格子都没问题的话，这个招法也没问题
						{
							MoveNum++;//总数目自增
						}
					}
				}

				/*if (!GetLongCTypeBoxExist())	//若不存在长死格，也就是只有单链或者没有死格
				{
				//存入TURN
				Moves[MoveNum].Set(x, y);
				MoveNum++;//总数目自增
				}*/
				SetBoard(BoardSave);			//还原
				Step = CurrentStep;				//还原
			}
		}
	}

	return MoveNum;
}
bool Board::GetLongCTypeBoxBool(int bx, int by)
{
	if (GetBoxLiberties(bx, by) == DEADBOX)//首先这个格子必须本身是一个C型格
	{
		int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		for (int n = 0; n < 4; n++)
		{
			int ex = bx + Dir[n][0];
			int ey = by + Dir[n][1];
			int nbx = bx + Dir[n][0] + Dir[n][0];//下一个格子的实际地址
			int nby = by + Dir[n][1] + Dir[n][1];//下一个格子的实际地址
			if (board[ex][ey] == EDGE&&nbx >= 1 && nbx <= LEN - 2 && nby >= 1 && nby <= LEN - 2)
			{
				if (GetBoxLiberties(nbx, nby) == CHAINBOX)
					return true;
			}
		}
	}
	return false;
}


//Turn被证明为低效率，比直接的考虑最后一步Move要慢大约1000倍，关键在于Turn需要构建新的Board，这一点十分耗时。
void Board::TurnMove(TURN Turn)
{
	SetBoard(Turn.NewBoard);//设置为新局面
}
int Board::GetAllTurns(TURN Turns[MOVENUM], int Player)
{
	int TurnNum = 0;
	Board ChosenBoard(board, Step);//建立模拟棋盘
	GetAllCTypeBoxes(Player,false);
	for (int j = 0; j<LEN; j++)
	{
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == EDGE)
			{
				Board Test(ChosenBoard.board, ChosenBoard.Step);
				//Test.Move(i, j, Player);							//玩家就走这一步试试
				Turns[TurnNum].Owner = Player;						//这一轮的属主是该玩家
				Turns[TurnNum].SetBoard(board);						//存入新局面
				Turns[TurnNum].NewBoard[i][j] = Player;
				//Turns[TurnNum].SetCaptualInfo(board, Test.board);	//存入招法
				TurnNum++;											//总数目自增
			}
		}
	}
	return TurnNum;
}
int Board::GetFilterTurn(TURN Turns[MOVENUM], int Player)
{
	int TurnNum = 0;
	Board ChosenBoard(board, Step);//建立模拟棋盘
	GetAllCTypeBoxes(Player,false);
	//此时的ChosenBoard是标准局面，需要做一个选择
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//若为空白边
			{
				Board Test(ChosenBoard.board, ChosenBoard.Step);
				Test.Move(x, y, Player);//玩家模拟走一步试试
				if (!Test.GetLongCTypeBoxExist())//若不存在长死格，也就是只有单链或者没有死格
				{
					//存入TURN
					Turns[TurnNum].Owner = Player;						//这一轮的属主是该玩家
					Turns[TurnNum].SetBoard(Test.board);				//存入新局面
					Turns[TurnNum].SetCaptualInfo(board, Test.board);	//存入招法
					TurnNum++;//总数目自增
				}
			}
		}
	}

	return TurnNum;
}

//常用函数
void cprintf(char* str, WORD color, ...) {
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	cout << str;
	SetConsoleTextAttribute(handle, colorOld);
	/*
	cprintf("H", 10);//绿色
	cprintf("e", 9);//蓝色
	cprintf("l", 12);//红色
	cprintf("l", 11);//青色
	cprintf("o", 13);//粉色
	cprintf("W", 15);//白
	cprintf("o", 2);//军绿
	cprintf("r", 5);//紫
	cprintf("l", 8);//灰
	cprintf("d", 14);//黄
	cprintf("!", 4);//棕
	*/
}
void cprintNum(int color, int Num)
{
	char str[4];
	sprintf_s(str, "%d", Num);
	cprintf(str, color);
}
bool LocEqual(LOC a, LOC b)
{
	if (a.x == b.x&&a.y == b.y)
		return true;
	return false;
}
bool BoardEqual(int a[LEN][LEN], int b[LEN][LEN])
{
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (a[x][y] != b[x][y])
				return false;
		}
	}
	return true;
}
bool IfOddNumber(int num)
{
	//判断一个数字是否是奇数
	if (num % 2 != 0)
		return true;
	return false;
}
bool IfEvenNumber(int num)
{
	//判断一个数字是否是偶数
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}
void BoardCopy(int Source[LEN][LEN], int Target[LEN][LEN])
{
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			Target[x][y] = Source[x][y];
		}
	}
}
void MoveMsg(int x, int y, int Player)
{
	cout << "Msg: ";
	if (Player == RED)
		cprintf("Red", 12);
	else
		cprintf("Blue ", 9);
	cout << " Captual Edge";
	cprintf("(", 10);
	cprintNum(10, x);
	cprintf(",", 10);
	cprintNum(10, y);
	cprintf(")\n", 10);
}









