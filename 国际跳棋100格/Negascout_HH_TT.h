#pragma once
#include"Searchengine.h"
#include"TranspositionTable.h"
#include"History_TT.h"
#include"head.h"

//#include"CEvelation.h"
class CNegascout_HH_TT:
	public CSearchengine,
	public CTranspositionTable,
	public CHistory_TT
{
private:
								  //�з�����
	int m_iMaxDepth;
	int First_In_N;
	double Negascout_HH_TT(int Color,int depth,double alpha,double beta,int Step);
	double alphabeta(int iDepth,int Color,double alpha,double beta);
public:
	
	int m_nMaxDepth;
	double m_fTimeLimit;
	CNegascout_HH_TT();
	~CNegascout_HH_TT();
	void Input_Depth_Time(int Depth,double Time);
	MOVE SearchAGoodMove(int board[10][10],int Color,int Step);
	MOVE SearchAGoodMove_2(int board[10][10],int Color);

};
