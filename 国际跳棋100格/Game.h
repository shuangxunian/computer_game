#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include"head.h"
#include"Negascout_HH_TT.h"
using namespace std;
class Game//引擎
{
public:
	Game();
	~Game();
	CNegascout_HH_TT Computer;

	//记录错误
	int Error;
	//给出最佳着法
	MOVE Search_Good_Move(int board[][10],int Color,int Engine,int Step);
	//给出所有着法
	int Search_All_Moves(int board[][10],MOVE *All_Moves,int Color);
	//按着法走棋
	void Go_Move(int board[][10],MOVE The_Move);
	//获取当前局面估值
	CString Get_Evel(int board[][10],int Color);
	//检验着法
	bool Check_Move(MOVE The_Move,MOVE AllMoves[],int move_num);
	//给搜索时间层数赋值
	void Input_Depth_Time(int Depth,double Time);
	
};
