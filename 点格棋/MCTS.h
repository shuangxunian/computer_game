#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"

#include "Define.h"
#include "BoxBoard.h"
#include "Paradox.h"

#pragma  once

#define UCT_MC_TIMES 1
#define UCT_TIMES 15000000
#define UCT_LIMIT_TIME 32
#define UCT_FILTER_RANGE 30


//UCT
class UCTNode :public Board
{
public:
	UCTNode();//构造函数
	UCTNode(int Player, int Array[LEN][LEN], int step, bool GetCBox, int Filter_Range);//构造函数

	//UCT使用的值
	int Owner;						//所属玩家
	int Times;						//被尝试过的次数
	int BoardWinner;				//这个局面的胜利者
	int ExistChild;					//子节点当前的数目
	int TotalChild;					//子节点的总数。
	float AvgValue;					//对父节点而言，这个节点的平均收益 
	LOC NodeMoves[MOVENUM];			//这个子节点所有可能的招式
	UCTNode* ChildNodes[MOVENUM];	//指向第一个子节点的指针

	//功能函数
	float RefreshAvgValue();									//更新平均收益
	float RefreshAvgValueWithSolver();
	float GetUCBValue(int Total);								//根据现在的平均收益而获得UCB值
	float GetUCBRaveValue(int Total, LOC Move, int RaveTable[LEN][LEN]);
	UCTNode* ExpandUCTNode(int MC_Times, int Filter_Range);
	UCTNode* ExpandUCTNodeRave(int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
};
float UCTProcess(UCTNode &B, int &Total, int MC_Times, int Filter_Range);
void UCTMove(Board &CB, int Player, bool Msg);
void UCTMove_1(Board &CB, int Player, bool Msg);
void UCTMove_2(Board &CB, int Player, bool Msg);
void UCTMove_T(Board &CB, int Player, bool Msg);

bool SearchingUCTNode(UCTNode* SearchingNode, int CurrentBoard[LEN][LEN], int Depth);
void DeleteUCTNode(UCTNode* Root);
void DeleteUCTTree(UCTNode Root);


void UCTMoveWithSacrifice(Board &CB, int Player, bool Msg, int branch);
void LatterSituationMove(Board &CB, int Player, bool Msg);
void GameTurnMove(Board &CB, int Player, bool Msg);
void GameTurnMove_T(Board &CB, int Player, bool Msg);
void GameTurnMove_Rave(Board &CB, int Player, bool Msg);
void RaveSolverDouble(Board &CB, int Player, bool Msg);

//Rave
float UCTProcessRave(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
void UCTRaveMove(Board &CB, int Player, bool Msg);

//RaveSolverDouble
float UCTProcessRaveSolver(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
void UCTRaveSolverDoubleMove(Board &CB, int Player, bool Msg);
