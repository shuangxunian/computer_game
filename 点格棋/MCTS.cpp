#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "time.h"
#include "windows.h"

#include "Define.h"
#include "Paradox.h"
#include "MCTS.h"

using namespace std;

//UCT for DAB

//请默念一句话：每一个子节点的收益值，都是对于父节点所有者而言的收益！


UCTNode::UCTNode()
{
	//初始化棋盘
	Step = 0;
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[j][i] = 0;
		}
	}
	ExistChild = 0;
	TotalChild = 0;
}
UCTNode::UCTNode(int Player, int Array[LEN][LEN], int step,bool GetCBox, int Filter_Range)
{
	//初始化棋盘
	Step = step;
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[j][i] = Array[j][i];
		}
	}
	if (GetCBox)
		GetAllCTypeBoxes(Player, false);		//先全部吃掉C型格，此时的Node自动跃迁为Gotten Node


	//然后开始初始化数据
	Owner = Player;								//这个节点的拥有者是传入的数据
	Times = 1;									//创建时候这个节点已经被访问一次了
	Board Win(board, Step);						//创建当前局面的判断点
	BoardWinner = GetBoardWinner(Win, -Player);	//这个局面的胜利者，注意此时的先手是我方
	ExistChild = 0;								//已经存在的子节点数为0

	int F = Win.GetFreeEdgeNum();
	if (F>=Filter_Range)
		TotalChild = GetFreeMoves(NodeMoves);		//提前计算总节点数
	else
		TotalChild = GetFilterMoves(NodeMoves);		//提前计算总节点数
}
UCTNode* UCTNode::ExpandUCTNode(int MC_Times, int Filter_Range)
{
	//创建新节点
	Board CB(board, Step);
	CB.Move(NodeMoves[ExistChild].x, NodeMoves[ExistChild].y, Owner);
	UCTNode *NewB = new UCTNode(-Owner, CB.board, CB.Step, true, Filter_Range);

	//做一次MC评估
	CB.SetBoard(NewB->board);
	NewB->AvgValue = GetFilterMCEvalution(CB, -Owner, Owner, MC_Times, Filter_Range);
	return NewB;//返回NewB的地址
}
UCTNode* UCTNode::ExpandUCTNodeRave(int MC_Times, int Filter_Range,int RaveTable[LEN][LEN])
{
	//创建新节点
	Board CB(board, Step);
	CB.Move(NodeMoves[ExistChild].x, NodeMoves[ExistChild].y, Owner);
	UCTNode *NewB = new UCTNode(-Owner, CB.board, CB.Step, true, Filter_Range);

	//做一次MC评估
	CB.SetBoard(NewB->board);
	NewB->AvgValue = GetRaveFilterMCEvalution(CB, -Owner, Owner, MC_Times, Filter_Range,RaveTable);
	return NewB;//返回NewB的地址
}
float UCTNode::RefreshAvgValue()
{
	float Value = 0;
	//首先计算平均收益
	for (int i = 0; i < ExistChild; i++)
	{
		/*if (ChildNodes[i]->AvgValue == 1)//如果任意一个子节点的收益为1
		{
			AvgValue = 0;	//则本节点的收益必定为0
			return 0;		//返回0
		}*/
		Value += ChildNodes[i]->AvgValue;
	}
	AvgValue = 1 - (Value / ExistChild);//当前节点的平均收益必定是(1 - 子节点的平均收益 )
	return AvgValue;
}
float UCTNode::RefreshAvgValueWithSolver()
{
	float Value = 0;
	//首先计算平均收益
	bool Fail = true;
	for (int i = 0; i < ExistChild; i++)
	{
		if (ChildNodes[i]->BoardWinner != -Owner)
		{
			Fail = false;
			break;
		}
	}
	if (Fail)
	{
		BoardWinner = -Owner;
		AvgValue = 1;
		return 1;
	}
	int VaildChildNum = ExistChild;
	for (int i = 0; i < ExistChild; i++)
	{
		if (ChildNodes[i]->BoardWinner == Owner)//如果任意一个子节点的收益为1
		{
			BoardWinner = Owner;	//那么这个节点的胜利者就是其所有者
			AvgValue = 0;			//那么对于父节点来说本节点的收益为0
			return 0;
		}
		if (ChildNodes[i]->BoardWinner == -Owner)
			VaildChildNum--;
		else
			Value += ChildNodes[i]->AvgValue;
	}
	AvgValue = 1 - (Value / VaildChildNum);//当前节点的平均收益必定是(1 - 有效子节点的平均收益 )
	return AvgValue;
}
float UCTNode::GetUCBValue(int Total)
{
	//double UCBValue = AvgValue + sqrt((((log10((double)Total))*2)/(double)Times));
	return AvgValue + sqrt((((log10((float)Total)) * 2) / (float)Times));
}
float UCTNode::GetUCBRaveValue(int Total,LOC Move,int RaveTable[LEN][LEN])
{
	float k = 100;
	float UCB = AvgValue + sqrt((((log10((float)Total)) * 2) / (float)Times));
	float RAVE = (float)RaveTable[Move.x][Move.y];
	float ravek = sqrt(k / (3 * Total) + k);
	float ucbk = 1 - ravek;
	float Final = ravek*RAVE + ucbk*UCB;
	return Final;
}
float UCTProcess(UCTNode &B, int &Total, int MC_Times, int Filter_Range)
{
	B.Times++;//访问的次数增加一次
	if (B.BoardWinner != 0)//如果游戏已经结束了
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//如果在这个节点游戏结束了，判定收益。
		else
			B.AvgValue = 1;//如果在这个节点游戏结束了，判定收益
		Total++;//如果搜索到游戏结束的节点，则本次迭代结束。
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//如果还有未尝试过的节点
	{
		Total++;//基准情形，本次迭代结束，尝试次数+1。
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNode(MC_Times,Filter_Range);//扩展一个子节点
		B.ExistChild++;//子节点的数目自增1
		B.RefreshAvgValue();//刷新收益
		return B.AvgValue;
	}
	else//说明没有未尝试过的节点
	{
		int BestNodeNum = 0;
		double BestUCBValue = 0;
		double UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBValue(Total);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcess(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}

void UCTMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step,true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT的次数函数
		clock_t start;	//设置计时器的变量
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//判定最佳收益
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("最大访问与最佳收益相同！\n", 10);
			else
				cprintf("最大访问不等同于最佳收益！\n", 14);
			cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "最多访问节点访问为" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "最多访问节点收益为" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "本次UCT总迭代次数为" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//释放内存
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player,true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
	
	
}
void UCTMove_2(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT的次数函数
		clock_t start;	//设置计时器的变量
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//判定最佳收益
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("最大访问与最佳收益相同！\n", 10);
			else
				cprintf("最大访问不等同于最佳收益！\n", 14);
			cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "最多访问节点访问为" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "最多访问节点收益为" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "本次UCT总迭代次数为" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//释放内存
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void UCTMove_T(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT的次数函数
		clock_t start;	//设置计时器的变量
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//判定最佳收益
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("最大访问与最佳收益相同！\n", 10);
			else
				cprintf("最大访问不等同于最佳收益！\n", 14);
			cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "最多访问节点访问为" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "最多访问节点收益为" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "本次UCT总迭代次数为" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//释放内存
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}

bool SearchingUCTNode(UCTNode* SearchingNode, int CurrentBoard[LEN][LEN], int Player, int Depth)
{
	if (BoardEqual(SearchingNode->board, CurrentBoard) && Player == SearchingNode->Owner)//符合查找条件
	{
		//CurrentUCTNode = SearchingNode;
		return true;
	}
	else if (SearchingNode->BoardWinner != 0 && Depth > 0 && SearchingNode->ExistChild > 0)//仍未胜利 and 深度未到 and 有子节点
	{
		for (int i = 0; i < SearchingNode->ExistChild; i++)
		{
			if (SearchingUCTNode(SearchingNode->ChildNodes[i], CurrentBoard, Player, Depth - 1))
				return true;
		}
	}
	return false;
}
void DeleteUCTNode(UCTNode* Root)
{
	if (Root->ExistChild > 0)
	{
		for (int i = 0; i < Root->ExistChild; i++)
		{
			DeleteUCTNode(Root->ChildNodes[i]);
			delete Root->ChildNodes[i];
		}
	}
}
void DeleteUCTTree(UCTNode Root)
{
	for (int i = 0; i < Root.ExistChild; i++)
	{
		DeleteUCTNode(Root.ChildNodes[i]);
		delete Root.ChildNodes[i];
	}
}

void UCTMoveWithSacrifice(Board &CB,int Player,bool Msg,int branch)
{
	BoxBoard Dead(CB);
	bool DeadChain = Dead.GetDeadChainExist();
	bool DeadCircle = Dead.GetDeadCircleExist();
	if (DeadCircle||DeadChain)//有环的情况优先处理
	{
		LOC DCMove;
		if (DeadChain)//然后处理死链
		{
			//首先吃到贪婪的临界点
			for (;;)
			{
				Board Test = CB;
				Test.GetCTypeBox(Player,false);
				BoxBoard Dead(Test);
				if (Dead.GetDeadChainExist())
					CB.GetCTypeBox(Player, Msg);
				else
					break;
			}
			//然后开始考虑DoubleCross
			DCMove = CB.GetDoubleCrossLoc(Player);
		}
		else
		{
			//首先吃到贪婪的临界点
			for (;;)
			{
				Board Test = CB;
				Test.GetCTypeBox(Player, false);
				BoxBoard Dead(Test);
				if (Dead.GetDeadCircleExist())
					CB.GetCTypeBox(Player, Msg);
				else
					break;
			}
			//然后开始考虑DoubleCross
			DCMove = CB.GetDoubleCrossLoc(Player);
		}
		//然后开始制造UCTProcess
		UCTNode GreedyChild = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
		UCTNode SacrificeChild = UCTNode(-Player, CB.board, CB.Step, false, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
		SacrificeChild.Move(DCMove.x, DCMove.y, Player);
		for (;;)//直到无法占据CTypeBox了就结束
		{
			if (!SacrificeChild.GetCTypeBoxLimit(Player, false))
				break;
		}
		//此时的Sacrifice是准备牺牲的局面了
		SacrificeChild.TotalChild = SacrificeChild.GetFilterMoves(SacrificeChild.NodeMoves);
		Board Win(SacrificeChild.board, SacrificeChild.Step);		//创建当前局面的判断点
		SacrificeChild.BoardWinner = GetBoardWinner(Win, Player);	//这个局面的胜利者，注意此时的先手是对方
		if (SacrificeChild.BoardWinner == 0)
		{
			//
			int Total = 0;//UCT的次数函数
			int Total2 = 0;
			clock_t start;	//设置计时器的变量
			start = clock();
			for (int i = 0; i < UCT_TIMES / 2; i++)//迭代一定次数
			{
				UCTProcess(SacrificeChild, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);
				UCTProcess(GreedyChild, Total2, UCT_MC_TIMES, UCT_FILTER_RANGE);
				//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
				double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
				if (totaltime >= UCT_LIMIT_TIME)
					break;
			}
			//

			//判定最佳收益
			int BestNodeNum = 0;
			float BestAvgValue = 0;
			for (int i = 0; i < GreedyChild.ExistChild; i++)
			{
				if (GreedyChild.ChildNodes[i]->AvgValue >= BestAvgValue)
				{
					BestNodeNum = i;
					BestAvgValue = GreedyChild.ChildNodes[i]->AvgValue;
				}
			}

			//用贪婪
			if (BestAvgValue >= SacrificeChild.AvgValue)
			{
				CB.GetAllCTypeBoxes(Player, Msg);
				CB.Move(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
				if (Msg)
					MoveMsg(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
			}
			//使用牺牲
			else
			{
				CB.Move(DCMove.x, DCMove.y, Player);
				for (;;)//直到无法占据CTypeBox了就结束
				{
					if (!CB.GetCTypeBoxLimit(Player, Msg))
						break;
				}
				if (Msg)
					MoveMsg(DCMove.x, DCMove.y, Player);
			}
			DeleteUCTTree(GreedyChild);
			DeleteUCTTree(SacrificeChild);
			if (Msg)
			{
				cprintf("========================Infomation==========================\n", 8);
				cout << "牺牲的节点收益为" << SacrificeChild.AvgValue << endl;
				cout << "贪婪最佳节点收益为" << GreedyChild.ChildNodes[BestNodeNum]->AvgValue << endl;
				cprintf("============================================================\n", 8);
			}
		}
		else
		{

		}
	}

	else//正常UCT移动
	{
		CB.GetAllCTypeBoxes(Player, Msg);
		if (branch ==1)
			UCTMove_T(CB, Player, true);
		else if (branch == 2)
			UCTMove_2(CB, Player, true);
		else if (branch == 3)
			UCTRaveMove(CB, Player, true);
		else if (branch == 4)
			UCTRaveSolverDoubleMove(CB, Player, true);
		else
			UCTMove(CB, Player, true);
	}
}
void LatterSituationMove(Board &CB, int Player, bool Msg)
{
	//后期算法，此时只有长链和环。
	if (CB.GetLongCTypeBoxExist())
	{
		//已有打开的长链，根据牺牲与不牺牲之后的理性状态决定是否牺牲。
		BoxBoard Dead(CB);
		int SacrificeBoxNum;
		if (Dead.GetDeadChainExist())
			SacrificeBoxNum = 2;
		if (Dead.GetDeadCircleExist())
			SacrificeBoxNum = 4;
		
		//假设全部都吃掉了
		Dead.GetAllCTypeBoxes(Player,false);
		LOC BoxNum = Dead.GetRationalStateBoxNum();//x是先手的，y是后手的

		//现在根据接下来局面能得到的格子数来进行分析

		//假设在当前链全被消灭后后手可以拿到x个，先手可以拿到y个。我方全吃后可以拿到y+n，对方拿到x个。若x-y<n则我方全吃

		if (BoxNum.x - BoxNum.y <= SacrificeBoxNum||Dead.Winner()!=0)
		{
			CB.GetAllCTypeBoxes(Player, Msg);
			if (Dead.Winner()==0)
				LatterSituationMove(CB, Player, Msg);
		}
		else
		{
			//牺牲，此时必有死长链或死环
			LOC DCMove;
			if (SacrificeBoxNum==2)//然后处理死链
			{
				//首先吃到贪婪的临界点
				for (;;)
				{
					Board Test = CB;
					Test.GetCTypeBox(Player, false);
					BoxBoard Dead(Test);
					if (Dead.GetDeadChainExist())
						CB.GetCTypeBox(Player, Msg);
					else
						break;
				}
				//然后开始考虑DoubleCross
				DCMove = CB.GetDoubleCrossLoc(Player);
			}
			else
			{
				//首先吃到贪婪的临界点
				for (;;)
				{
					Board Test = CB;
					Test.GetCTypeBox(Player, false);
					BoxBoard Dead(Test);
					if (Dead.GetDeadCircleExist())
						CB.GetCTypeBox(Player, Msg);
					else
						break;
				}
				//然后开始考虑DoubleCross
				DCMove = CB.GetDoubleCrossLoc(Player);
			}

			CB.Move(DCMove.x, DCMove.y, Player);
			for (;;)//直到无法占据CTypeBox了就结束
			{
				if (!CB.GetCTypeBoxLimit(Player, Msg))
					break;
			}
			MoveMsg(DCMove.x, DCMove.y, Player);
			//牺牲结束
		}

	}
	else
	{
		//选择打开哪一条长链。根据理性状态决定是打开最短的长链还是如何
		CB.GetAllCTypeBoxes(Player, Msg);
		BoxBoard Test(CB);
		if (Test.RationalState(Test.GetRationalStateBoxNum()))
		{
			//如果是理性局面的话,依次打开链
			LOC M = Test.GetOpenSuitableChainLoc();
			CB.Move(M.x, M.y, Player);
			if (Msg)
				MoveMsg(M.x, M.y, Player);
		}
		else
		{
			//如果不是理性局面的话，打开最短的链
			//如果是理性局面的话
			LOC M = Test.GetOpenShortestChainLoc();
			CB.Move(M.x, M.y, Player);
			if (Msg)
				MoveMsg(M.x, M.y, Player);
		}
	}
}

//游戏移动，会根据前中后期自动移动
void GameTurnMove(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player,false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "使用UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg,0);
	}
	else//也就是后期局面了
	{
		//也就是Filter都已经无能为力的情况下，只有LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "使用后期" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}
void GameTurnMove_T(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "使用UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg,1);
	}
	else//也就是后期局面了
	{
		//也就是Filter都已经无能为力的情况下，只有LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "使用后期" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}

//Rave
float UCTProcessRave(UCTNode &B, int &Total, int MC_Times, int Filter_Range,int RaveTable[LEN][LEN])
{
	B.Times++;//访问的次数增加一次
	if (B.BoardWinner != 0)//如果游戏已经结束了
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//如果在这个节点游戏结束了，判定收益。
		else
			B.AvgValue = 1;//如果在这个节点游戏结束了，判定收益
		Total++;//如果搜索到游戏结束的节点，则本次迭代结束。
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//如果还有未尝试过的节点
	{
		Total++;//基准情形，本次迭代结束，尝试次数+1。
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNodeRave(MC_Times, Filter_Range,RaveTable);//扩展一个子节点
		B.ExistChild++;//子节点的数目自增1
		B.RefreshAvgValue();//刷新收益
		return B.AvgValue;
	}
	else//说明没有未尝试过的节点
	{
		int BestNodeNum = 0;
		float BestUCBValue = 0;
		float UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBRaveValue(Total,B.NodeMoves[i],RaveTable);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcessRave(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range,RaveTable);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}
void UCTRaveMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT的次数函数
		clock_t start;	//设置计时器的变量
		start = clock();
		int RaveTable[LEN][LEN];
		for (int y = 0; y < LEN; y++)
		{
			for (int x = 0; x < 0; x++)
			{
				RaveTable[x][y] = 0;
			}
		}
		for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
		{
			UCTProcessRave(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE,RaveTable);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//判定最佳收益
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("最大访问与最佳收益相同！\n", 10);
			else
				cprintf("最大访问不等同于最佳收益！\n", 14);
			cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "最多访问节点访问为" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "最多访问节点收益为" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "本次UCT总迭代次数为" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//释放内存
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void GameTurnMove_Rave(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "使用UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg, 3);
	}
	else//也就是后期局面了
	{
		//也就是Filter都已经无能为力的情况下，只有LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "使用后期" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}

//Rave Double Solver
float UCTProcessRaveSolver(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN])
{
	B.Times++;//访问的次数增加一次
	if (B.BoardWinner != 0)//如果游戏已经结束了
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//如果在这个节点游戏结束了，判定收益。
		else
			B.AvgValue = 1;//如果在这个节点游戏结束了，判定收益
		Total++;//如果搜索到游戏结束的节点，则本次迭代结束。
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//如果还有未尝试过的节点
	{
		Total++;//基准情形，本次迭代结束，尝试次数+1。
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNodeRave(MC_Times, Filter_Range, RaveTable);//扩展一个子节点
		B.ExistChild++;//子节点的数目自增1
		B.RefreshAvgValueWithSolver();//刷新收益
		return B.AvgValue;
	}
	else//说明没有未尝试过的节点
	{
		int BestNodeNum = 0;
		float BestUCBValue = 0;
		float UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBRaveValue(Total, B.NodeMoves[i], RaveTable);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcessRaveSolver(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range,RaveTable);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}
void UCTRaveSolverDoubleMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT的次数函数
		clock_t start;	//设置计时器的变量
		start = clock();
		int RaveTable[LEN][LEN];
		for (int y = 0; y < LEN; y++)
		{
			for (int x = 0; x < 0; x++)
			{
				RaveTable[x][y] = 0;
			}
		}
		for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
		{
			UCTProcessRaveSolver(UCTB, Total, 2, UCT_FILTER_RANGE, RaveTable);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//判定最佳收益
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "当前节点平均收益为" << 1 - UCTB.AvgValue << endl;
			cout << "最佳收益节点访问为" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "最佳收益节点收益为" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "本次UCT总迭代次数为" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//释放内存
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void RaveSolverDouble(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "使用UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg, 4);
	}
	else//也就是后期局面了
	{
		//也就是Filter都已经无能为力的情况下，只有LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "使用后期" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}
