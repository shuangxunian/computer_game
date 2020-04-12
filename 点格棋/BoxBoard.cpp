#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"
#include "BoxBoard.h"

using namespace std;

//BoxType
BoxType::BoxType()
{
	Type = 0;//默认类型
	BelongingChainNum = 0;//默认为必空的0号空间
}

//ChainInfo
ChainInfo::ChainInfo()
{
	Type = NotDefine;				//类型为为定义
	ChainBoxNum = 0;				//所属格子数在开始时候为0
	ConditionOfPreCircle = false;	//是否预备环的先决条件
}

//BoxBoard
BoxBoard::BoxBoard(Board NewB)
{
	//复制基本属性
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[i][j] = NewB.board[i][j];
		}
	}
	Step = NewB.Step;
	//定义格子属性
	//DefineBoxesType();
}
BoxBoard::BoxBoard(int Array[LEN][LEN], int step)
{
	//复制基本属性
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[i][j] = Array[i][j];
		}
	}
	Step = step;
	//定义格子属性
	//DefineBoxesType();
}
void BoxBoard::DefineBoxesType()
{
	for (int y = 1; y <= BOXLEN; y++)//格子数
	{
		for (int x = 1; x <= BOXLEN; x++)//格子数
		{
			int bx = (x * 2) - 1;//Board中的坐标
			int by = (y * 2) - 1;//Board中的坐标
			Boxes[x][y].BoxLoc.Set(bx, by);//存入坐标
			//定义Owner
			if (board[bx][by] == BOX)//如果还没被占领
				Boxes[x][y].BoxOwner = 0;//未被占领
			else
				Boxes[x][y].BoxOwner = board[bx][by] / 2;//除以二之后的数值等同于玩家编号
			//定义Type
			int bl = GetBoxLiberties(bx, by);//得到格子的自由度
			if (bl == 4)
				Boxes[x][y].Type = FREEBOX;	//是自由格
			else
				Boxes[x][y].Type = bl;	//不然就跟自由边的数量是相同的。
		}
	}
}
int BoxBoard::GetBoxType(int bx, int by)
{
	if (bx >= 1 && by >= 1 && bx <= BOXLEN&& by <= BOXLEN)//满足格子要求
		return Boxes[bx][by].Type;//返回格子类型
	return FREEBOX;//其余超出范围的均返回自由格
}
void BoxBoard::ShowBoxType()
{
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			if (Boxes[i][j].Type == FULLBOX)
				cprintf("■", 8);
			else if (Boxes[i][j].Type == DEADBOX)
				cprintf("■", 2);
			else if (Boxes[i][j].Type == CHAINBOX)
				cprintf("■", 14);
			else if (Boxes[i][j].Type == FREEBOX)
				cprintf("■", 5);
		}
		cout << endl;
	}
	cprintf("\n自由格", 5);
	cprintf("\n链格", 14);
	cprintf("\n死格", 2);
	cprintf("\n已被占领\n", 8);
}
int BoxBoard::GetFirstEmptyChainNum()
{
	for (int i = 1; i < BOXNUM; i++)//不采用0号空间
	{
		if (Chains[i].Type == NotDefine)
			return i;
	}
	cout << "链空间不足！" << endl;
	system("pause");
	return 0;
}

//鉴别链
void BoxBoard::ShowBelongingChain()
{
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
			if (board[i][j] == BOX || board[i][j] == RED_BOX || board[i][j] == BLUE_BOX)
			{
				if (Boxes[(i + 1) / 2][(j + 1) / 2].BelongingChainNum != 0)
				{
					cprintf(" ", 8);
					cprintNum(14, Boxes[(i + 1) / 2][(j + 1) / 2].BelongingChainNum);
				}
				else
				{
					if (Boxes[(i + 1) / 2][(j + 1) / 2].BoxOwner != 0)
					{
						if (Boxes[(i + 1) / 2][(j + 1) / 2].BoxOwner == RED)
							cprintf("■", 12);
						else
							cprintf("■", 9);
					}
					else
						cprintf("  ", 8);
				}

			}
			else if (board[i][j] == EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
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
				cprintf("○", 15);
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

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type != NotDefine)
		{
			cout << "Chain [" << i << "] Box [" << Chains[i].ChainBoxNum << "] Type [";
			if (Chains[i].Type == SingleChain)
				cout << "SingleChain]";
			else if (Chains[i].Type == ShortChain)
				cout << "ShortChain]";
			else if (Chains[i].Type == LongChain)
				cout << "LongChain]";
			else if (Chains[i].Type == PreCircle)
				cout << "PreCircle]";
			else if (Chains[i].Type == Circle)
				cout << "Circle]";
			else
			{
				cout << "Wrong";
				system("pause");
			}

			cout << "  PreChain = ";
			if (Chains[i].ConditionOfPreCircle)
				cout << "yes" << endl;
			else
				cout << "no" << endl;
		}

	}
}
void BoxBoard::InheritChain(int InheritorRegNum, int AncesterRegNum)
{
	//cout <<InheritorRegNum<< " inherit " << AncesterRegNum << endl;
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			if (Boxes[i][j].BelongingChainNum == AncesterRegNum)
				Boxes[i][j].BelongingChainNum = InheritorRegNum;//被继承
		}
	}
	Chains[InheritorRegNum].ChainBoxNum += Chains[AncesterRegNum].ChainBoxNum;//格子数累加
	Chains[InheritorRegNum].Type = LongChain;//必定是长链。

	//销毁被继承者的注册空间
	Chains[AncesterRegNum].ChainBoxNum = 0;
	Chains[AncesterRegNum].Type = NotDefine;
}
void BoxBoard::RegisterChain(LOC FreeBoxLoc, LOC FirstLoc)
{
	//动态注册链。从一个格子出发 ，向一个格子开始进行链的注册。一般仅从自由格出发

	int x = FreeBoxLoc.x;				//初始X值
	int y = FreeBoxLoc.y;				//初始Y值
	LOC Loc = FreeBoxLoc;				//设置起点坐标
	LOC NewLoc = FirstLoc;				//设置新坐标
	int Re = GetFirstEmptyChainNum();	//获取一个空白的链空间
	Chains[Re].StartLoc = Loc;			//起点为某个自由格
	Chains[Re].ChainBoxNum = 0;			//更新起点
	Chains[Re].Type = NotDefine;

	//在如下过程中，NewLoc才是实际链的每一个格子的坐标。链的类型在搜索结束时候判断
	for (int i = 0; i < BOXNUM; i++)
	{
		if (GetBoxType(NewLoc.x, NewLoc.y) != CHAINBOX)//当搜索结束的时候，判定链的类型。此时，NewLoc为搜索到的终点格坐标
		{
			Chains[Re].EndLoc = NewLoc;//搜索到达终点
			if (NewLoc.x == FreeBoxLoc.x&&NewLoc.y == FreeBoxLoc.y&&Boxes[NewLoc.x][NewLoc.y].BelongingChainNum == EMPTY)
			{
				//若终点格的坐标与起点格相同并且该格还未被认定类型，则认定为PreCircle
				Chains[Re].Type = PreCircle;
				Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;//包括该格在内也需要被注册
				Chains[Re].ChainBoxNum++;
			}
			else//若起终点不一样，则为链。根据数目判定其类型
			{
				if (Chains[Re].ChainBoxNum == 1)
					Chains[Re].Type = SingleChain;
				else if (Chains[Re].ChainBoxNum == 2)
					Chains[Re].Type = ShortChain;
				else
					Chains[Re].Type = LongChain;
			}
			break;
		}
		else//每搜到一个新的格子
		{
			LOC mLoc = NewLoc;
			//定义格子的信息
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//与链挂钩
			Chains[Re].ChainBoxNum++;							//链的格子数目自增1
			NewLoc = FindNextBox(NewLoc, Loc);					//寻找下一个。
			Loc = mLoc;
		}
	}
}
void BoxBoard::RegisterCircle(LOC StartLoc, LOC NextLoc)
{
	//动态注册链。从一个格子出发 ，向一个格子开始进行链的注册。一般仅从自由格出发

	int x = StartLoc.x;//初始X值
	int y = StartLoc.y;//初始Y值
	LOC Loc = StartLoc;//设置起点坐标
	LOC NewLoc = NextLoc;//设置新坐标
	int Re = GetFirstEmptyChainNum();//获取一个空白的链空间
	Chains[Re].ChainBoxNum = 0;//改为0
	Chains[Re].Type = NotDefine;
	//在如下过程中，NewLoc才是实际链的每一个格子的坐标。链的类型在搜索结束时候判断
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Boxes[NewLoc.x][NewLoc.y].Type != 2)//若发现自由格，则取消注册。
		{
			InheritChain(EMPTY, Re);//0号链吞并所有已经注册的链
			break;
		}
		else if (NewLoc.x == StartLoc.x&&NewLoc.y == StartLoc.y)//搜到起点，确定为Circle
		{
			Chains[Re].Type = Circle;
			Chains[Re].StartLoc = NewLoc;						//起点与终点都标记为这个格子
			Chains[Re].EndLoc = NewLoc;							//起点与终点都标记为这个格子
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//包括该格在内也需要被注册
			Chains[Re].ChainBoxNum++;							//链的格子数目自增1
			break;
		}
		else//每搜到一个新的格子
		{
			LOC mLoc = NewLoc;
			//定义格子的信息
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//与链挂钩
			Chains[Re].ChainBoxNum++;							//链的格子数目自增1
			NewLoc = FindNextBox(NewLoc, Loc);					//寻找下一个。
			Loc = mLoc;
		}
	}
}
void BoxBoard::SearchingFromBox(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//邻边为空，目标格子为链格
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//必须为未归属的格子，避免环重复从不同方向出发。
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterChain(BoxLoc, NewLoc);
			}
		}
	}

}
void BoxBoard::SearchingCircle(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//邻边为空，目标格子为链格
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//必须为未归属的格子，避免环重复从不同方向出发。
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterCircle(BoxLoc, NewLoc);
			}
		}
	}
}
void BoxBoard::ResetChainsInfo()
{
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			Boxes[x][y].BelongingChainNum = 0;
		}
	}
	for (int i = 1; i < BOXNUM; i++)
	{
		Chains[i].ChainBoxNum = 0;
		Chains[i].Type = NotDefine;
		Chains[i].ConditionOfPreCircle = false;
	}
}
void BoxBoard::DefineAllChains(bool ChainPlus)
{
	DefineBoxesType();//首先定义所有格子的类型
	ResetChainsInfo();//重置链的数据

	//首先从自由格出发搜索所有长链
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == 3)
			{
				LOC k;
				k.Set(i, j);
				SearchingFromBox(k);
			}
		}
	}

	//然后在边缘搜索还没有被排查的链
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(i, 0);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(0, i);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(i, BOXLEN + 1);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(BOXLEN + 1, i);
		SearchingFromBox(k);
	}

	/*然后在边缘搜索还没有被排查的链
	for (int i = 1; i <= BOXLEN; i++)
	{
	//搜索上边
	if (board[(i * 2) - 1][0] == EDGE&&Boxes[i][1].BelongingChainNum == EMPTY&&Boxes[i][1].Type == CHAINBOX)
	{
	LOC BoxLoc,NewLoc;
	BoxLoc.Set(i, 0);
	NewLoc.Set(i, 1);
	RegisterChain(BoxLoc, NewLoc);
	}
	//搜索右边
	if (board[LEN-1][(i * 2) - 1] == EDGE&&Boxes[BOXLEN][i].BelongingChainNum == EMPTY&&Boxes[BOXLEN][i].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(BOXLEN+1, i);
	NewLoc.Set(BOXLEN, i);
	RegisterChain(BoxLoc, NewLoc);
	}
	//搜索下边
	if (board[(i * 2) - 1][LEN - 1] == EDGE&&Boxes[i][BOXLEN].BelongingChainNum == EMPTY&&Boxes[i][BOXLEN].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(i,BOXLEN + 1);
	NewLoc.Set(i,BOXLEN);
	RegisterChain(BoxLoc, NewLoc);
	}
	//搜索左边
	if (board[0][(i * 2) - 1] == EDGE&&Boxes[1][i].BelongingChainNum == EMPTY&&Boxes[1][i].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(0, i);
	NewLoc.Set(1, i);
	RegisterChain(BoxLoc, NewLoc);
	}
	}*/


	//然后搜索还没有被搜索到的链格，可能为环
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == CHAINBOX&&Boxes[i][j].BelongingChainNum == EMPTY)
			{
				LOC BoxLoc;
				BoxLoc.Set(i, j);
				SearchingCircle(BoxLoc);
			}
		}
	}

	//最后开始ChainPlus
	if (ChainPlus)
	{
		//首先查找是否有首尾相同的链，定义为PreChain
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain)
			{
				for (int j = i + 1; j <= BOXNUM; j++)
				{
					if (Chains[j].Type == LongChain&&LocEqual(Chains[i].StartLoc, Chains[j].StartLoc) && LocEqual(Chains[i].EndLoc, Chains[j].EndLoc))
					{
						if (Boxes[Chains[j].StartLoc.x][Chains[j].StartLoc.y].BelongingChainNum == EMPTY&&Boxes[Chains[j].EndLoc.x][Chains[j].EndLoc.y].BelongingChainNum == EMPTY)
						{
							//此时，i与j同为长链而且首尾相同且首尾的格子都未被认定，可以认定他们为PreChain
							InheritChain(i, j);//链i继承了链j
							Boxes[Chains[i].StartLoc.x][Chains[i].StartLoc.y].BelongingChainNum = i;
							Boxes[Chains[i].EndLoc.x][Chains[i].EndLoc.y].BelongingChainNum = i;
							Chains[i].ChainBoxNum = Chains[i].ChainBoxNum + 2;
							Chains[i].StartLoc = Chains[i].EndLoc;
							Chains[i].Type = PreCircle;
						}
					}
				}
			}
		}

		//然后把所有PreCircle的PreChain定义好
		for (int y = 1; y <= BOXLEN; y++)
		{
			for (int x = 1; x <= BOXLEN; x++)
			{
				if (Boxes[x][y].Type == 3 && Chains[Boxes[x][y].BelongingChainNum].Type == PreCircle)
				{
					int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
					int bx = x * 2 - 1;
					int by = y * 2 - 1;
					//先手先产生一个死格
					for (int n = 0; n < 4; n++)
					{
						int ex = bx + Dir[n][0];
						int ey = by + Dir[n][1];
						int nbx = x + Dir[n][0];
						int nby = y + Dir[n][1];
						if (board[ex][ey] == EDGE&&Chains[Boxes[nbx][nby].BelongingChainNum].Type == LongChain)
						{
							Chains[Boxes[nbx][nby].BelongingChainNum].ConditionOfPreCircle = true;
						}
					}
				}
			}
		}
		//最后把所有还没定义的自由格归属到他最长的两条派生链的合并中
		for (int y = 1; y < BOXNUM; y++)
		{
			for (int x = 1; x <= BOXNUM; x++)
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				if (Boxes[x][y].Type == FREEBOX&&Boxes[x][y].BelongingChainNum == EMPTY)//若本自由格还未确定
				{
					int ChainRegNum[4];
					int Total = 0;
					for (int n = 0; n < 4; n++)
					{
						int bx = x + Dir[n][0];
						int by = y + Dir[n][1];
						int lx = (x * 2) - 1 + Dir[n][0];
						int ly = (y * 2) - 1 + Dir[n][1];
						if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//邻边为空，目标格子为链格
						{
							ChainRegNum[Total] = Boxes[bx][by].BelongingChainNum;
							Total++;
						}
					}
					//先找出最长的
					int FirstChainNum = 0;
					int FirstChainBoxes = 0;
					for (int i = 0; i < Total; i++)
					{
						if (Chains[ChainRegNum[i]].ChainBoxNum > FirstChainBoxes)
						{
							FirstChainNum = ChainRegNum[i];
							FirstChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
						}
					}
					//再找出次长的
					int SecChainNum = 0;
					int SecChainBoxes = 0;
					for (int i = 0; i < Total; i++)
					{
						if (Chains[ChainRegNum[i]].ChainBoxNum > SecChainBoxes&&ChainRegNum[i] != FirstChainNum)
						{
							SecChainNum = ChainRegNum[i];
							SecChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
						}
					}
					//将两个链，以及本格累加在一起
					Boxes[x][y].BelongingChainNum = FirstChainNum;
					Chains[FirstChainNum].ChainBoxNum++;
					InheritChain(FirstChainNum, SecChainNum);
				}
			}
		}
	}
}

//类型判断
void BoxBoard::RegisterDeadChain(LOC FreeBoxLoc, LOC FirstLoc)
{
	//动态注册链。从一个格子出发 ，向一个格子开始进行链的注册。一般仅从自由格出发

	int x = FreeBoxLoc.x;				//初始X值
	int y = FreeBoxLoc.y;				//初始Y值
	LOC Loc = FreeBoxLoc;				//设置起点坐标
	LOC NewLoc = FirstLoc;				//设置新坐标
	int Re = GetFirstEmptyChainNum();	//获取一个空白的链空间
	Chains[Re].StartLoc = Loc;			//起点为某个自由格
	Chains[Re].ChainBoxNum = 1;			//更新起点
	Chains[Re].Type = NotDefine;

	//在如下过程中，NewLoc才是实际链的每一个格子的坐标。链的类型在搜索结束时候判断
	for (int i = 0; i < BOXNUM; i++)
	{
		if (GetBoxType(NewLoc.x, NewLoc.y) != CHAINBOX)//当搜索结束的时候，判定链的类型。此时，NewLoc为搜索到的终点格坐标
		{
			Chains[Re].EndLoc = NewLoc;//搜索到达终点
			if (GetBoxType(NewLoc.x, NewLoc.y) == DEADBOX)//如果终端是一个死格的话，这就是一个死循环(DeadCircle)
			{
				Chains[Re].Type = DeadCircle;
				Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;//包括该格在内也需要被注册
				Chains[Re].ChainBoxNum++;
			}
			else//若起终点不一样，则为链。根据数目判定其类型
			{
				Chains[Re].Type = DeadChain;
			}
			break;
		}
		else//每搜到一个新的格子
		{
			LOC mLoc = NewLoc;
			//定义格子的信息
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//与链挂钩
			Chains[Re].ChainBoxNum++;							//链的格子数目自增1
			NewLoc = FindNextBox(NewLoc, Loc);					//寻找下一个。
			Loc = mLoc;
		}
	}
}
void BoxBoard::SearchingDeadChain(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//邻边为空，目标格子为链格
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//必须为未归属的格子，避免环重复从不同方向出发。
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterDeadChain(BoxLoc, NewLoc);
			}
		}
	}
}
void BoxBoard::DefineDeadChain()
{
	DefineBoxesType();//首先定义所有格子的类型
	ResetChainsInfo();//重置链的数据
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == DEADBOX)
			{
				LOC k;
				k.Set(i, j);
				SearchingDeadChain(k);
			}
		}
	}
}
bool BoxBoard::GetDeadChainExist()
{
	DefineDeadChain();
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Chains[i].Type == DeadChain)
			return true;
	}
	return false;
}
bool BoxBoard::GetDeadCircleExist()
{
	DefineDeadChain();
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Chains[i].Type == DeadCircle&&Chains[i].ChainBoxNum>3)
			return true;
	}
	return false;
}

//占据链
bool BoxBoard::CaptualAnySingleChain(int LatterPlayer)
{
	DefineAllChains(false);

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == SingleChain)//找到的任意一条单链
		{
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{

						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;
						//先手先产生一个死格
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, -LatterPlayer);
								break;
							}
						}
						//后手占据这个格子
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, LatterPlayer);
								break;
							}
						}
						//占据结束，刷新链信息，结束。
						Boxes[x][y].BelongingChainNum = 0;
						InheritChain(EMPTY, i);
						DefineBoxesType();
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool BoxBoard::CaptualAnyShortChain(int LatterPlayer)
{
	DefineAllChains(false);

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == ShortChain)//找到的任意一条单链
		{
			//先手先占领短链两个格子中间的那个边
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{

						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;

						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							int nbx = x + Dir[n][0];
							int nby = y + Dir[n][1];
							if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == i)//找到两个格子交叉的那个格子
							{
								Move(ex, ey, -LatterPlayer);
								break;
							}
						}

					}
				}
			}

			//后手占领短链两个格子剩余的边
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{
						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, LatterPlayer);
								break;
							}
						}
						//占据结束，刷新链信息，结束。
						Boxes[x][y].BelongingChainNum = 0;
					}
				}
			}
			//结束
			InheritChain(EMPTY, i);
			DefineBoxesType();
			return true;

		}
	}
	return false;
}
bool BoxBoard::CaptualShortestChain(int LatterPlayer)
{
	DefineAllChains(false);

	//先找到最短的那条长链/环
	int Least = 0;
	int LeastBoxNum = BOXNUM;//假设最短的链有25个
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == LongChain || Chains[i].Type == Circle)
		{
			if (Chains[i].ChainBoxNum < LeastBoxNum)
			{
				LeastBoxNum = Chains[i].ChainBoxNum;
				Least = i;
			}
		}
	}
	if (Least == 0)
		return false;


	//先手先占领任意两个格子之间的边
	bool Finish = false;
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least&&!Finish)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == Least)//找到两个格子交叉的那个格子
					{
						Move(ex, ey, -LatterPlayer);
						Finish = true;
						break;
					}
				}
			}
		}
	}

	//后手占领这条链所有其他格子空余的边
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least)
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					if (board[ex][ey] == EDGE)
					{
						Move(ex, ey, LatterPlayer);
						break;
					}
				}
				//占据结束，刷新链信息，结束。
				Boxes[x][y].BelongingChainNum = 0;
			}
		}
	}
	//结束
	InheritChain(EMPTY, Least);
	DefineBoxesType();
	return true;
}

//打开链
LOC BoxBoard::GetOpenShortestChainLoc()
{
	DefineAllChains(false);

	//先找到最短的那条长链/环
	int Least = 0;
	int LeastBoxNum = BOXNUM;//假设最短的链有25个
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == LongChain || Chains[i].Type == Circle)
		{
			if (Chains[i].ChainBoxNum < LeastBoxNum)
			{
				LeastBoxNum = Chains[i].ChainBoxNum;
				Least = i;
			}
		}
	}


	//先手先占领任意两个格子之间的边
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == Least)//找到两个格子交叉的那个格子
					{
						LOC k;
						k.Set(ex, ey);
						return k;
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}
LOC BoxBoard::GetOpenSuitableChainLoc()
{
	DefineAllChains(false);

	//先找到最短的那条长链/环
	int SuitChainNum = 99;
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == Circle)
		{
			SuitChainNum = i;
		}
	}

	if (SuitChainNum == 99)
	{
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain&&Chains[i].ConditionOfPreCircle)
			{
				SuitChainNum = i;
			}
		}
	}

	if (SuitChainNum == 99)
	{
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain)
			{
				SuitChainNum = i;
			}
		}
	}

	//=======
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == SuitChainNum)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == SuitChainNum)//找到两个格子交叉的那个格子
					{
						LOC k;
						k.Set(ex, ey);
						return k;
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}

//判断函数
bool BoxBoard::RationalState(LOC BoxNum)
{
	if (BoxNum.x >= BoxNum.y)
		return true;
	return false;
}
LOC BoxBoard::GetRationalStateBoxNum()
{
	//获知当前局面是否是一个理性局面，首先需要判断当前局面的数目。
	DefineAllChains(true);

	//首先清算各种链的数目
	int LCNum = 0;
	int LCBox = 0;
	int CNum = 0;
	int CBox = 0;
	int PCNum = 0;
	int PCBox = 0;
	bool OnlyPreChain = true;//是否仅有预备链，也就是预备环的先决条件。是的话，最后一个必定是预备环
	for (int i = 1; i <= BOXNUM; i++)
	{
		if (Chains[i].Type != NotDefine)
		{
			if (Chains[i].Type == LongChain)
			{
				if (Chains[i].ConditionOfPreCircle == false)
					OnlyPreChain = false;
				LCNum++;
				LCBox += Chains[i].ChainBoxNum;
			}
			else if (Chains[i].Type == Circle)
			{
				CNum++;
				CBox += Chains[i].ChainBoxNum;
			}
			else if (Chains[i].Type == PreCircle)
			{
				PCNum++;
				PCBox += Chains[i].ChainBoxNum;
			}
		}
	}

	//开始计算牺牲数目
	int Total = LCBox + PCBox + CBox;//总格子数
	int Sacrifice = 0;
	if (OnlyPreChain)
	{
		if (LCNum == 0)//但这个状况下不存在长链，则该情况都是环
		{
			Sacrifice = (CNum - 1) * 4;
		}
		else//存在长链，但都是预备链。最后一个必定为预备环
		{
			Sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 4;
		}
	}
	else
	{
		//有长链的时候，最后一个必定是长链
		Sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 2;
	}
	//ShowBelongingChain();
	//cout << "Total [" << Total << "]  Sacrifice [" << Sacrifice << "]" << endl;
	LOC num;
	num.Set(Total - Sacrifice, Sacrifice);
	return num;
}
int BoxBoard::GetBoardWinner(int LatterPlayer)
{
	if (GetFilterMoveNum() > 0)
		cout << "Wrong";
	bool debug = false;
	int player = LatterPlayer;
	DefineBoxesType();
	//ShowBelongingChain();
	//system("pause");
	/*
	for (;;)//先吞并所有的单链
	{
		if (CaptualAnySingleChain(player) == false)
			break;
		else
		{
			player = -player;
			//ShowBelongingChain();
			//system("pause");
		}
	}
	if (debug)
	{
		PrintBoard();
		cout << "Single Chain Finish , The Latter Player This Time is " << player << endl;
	}

	for (;;)//再吞并所有的短链
	{
		if (CaptualAnyShortChain(player) == false)
			break;
		else
		{
			player = -player;
			//ShowBelongingChain();
			//system("pause");
		}
	}
	if (debug)
	{
		PrintBoard();
		cout << "Short Chain Finish , The Latter Player This Time is " << player << endl;
	}
	*/
	LOC BoxNum;
	for (;;)//非理性情况下吞并所有
	{
		DefineAllChains(true);//先定义为完全状态判定一次
		BoxNum = GetRationalStateBoxNum();//然后再判定一次理性情况
		if (RationalState(BoxNum))
			break;
		else
		{
			if (!CaptualShortestChain(player))//如果吃不下去了也退出
				break;
			else
				player = -player;
		}
	}
	//ShowBelongingChain();
	//system("pause");
	if (Winner() == 0)//也就是还没胜利
	{
		int r, b;
		if (player == RED)
		{
			r = BoxNum.x + GetPlayerBoxes(RED);
			b = BoxNum.y + GetPlayerBoxes(BLUE);
		}
		else
		{
			r = BoxNum.y + GetPlayerBoxes(RED);
			b = BoxNum.x + GetPlayerBoxes(BLUE);
		}
		//cout << "Game Finish!\n Red [" << r << "] Blue [" << b << "]" << endl;
		if (r > b)
			return RED;
		else
			return BLUE;
	}
	else
		return Winner();
}

