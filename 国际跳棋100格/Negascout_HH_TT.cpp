#include "stdafx.h"
#include "Negascout_HH_TT.h"

CNegascout_HH_TT::CNegascout_HH_TT()
{
	 m_fTimeLimit =15;
	 m_nMaxDepth=7;
}

CNegascout_HH_TT::~CNegascout_HH_TT()
{
}

void CNegascout_HH_TT::Input_Depth_Time(int Depth,double Time)
{ 
	m_fTimeLimit =Time;
	m_nMaxDepth=Depth;
}

MOVE CNegascout_HH_TT::SearchAGoodMove(int board[10][10], int Color,int Step)
{
	CSearchengine::copy(board, Color); 
	CalculateInitHashKey(board);
	InitializeHistoryTable();
	First_In_N=0;
	timeStart = clock();
	for (m_iMaxDepth = 3; m_iMaxDepth <20; m_iMaxDepth++)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,UPDATE_TREE,m_iMaxDepth,0);
	
		if (Negascout_HH_TT(nowColor, m_iMaxDepth, -200000, 200000,Step) != 66666)//如果本次搜索没有被中止，保存最佳走法
			theBestMove = BestMove;
		else
			break;
	}
	return theBestMove;


}

MOVE CNegascout_HH_TT::SearchAGoodMove_2(int board[10][10], int Color)
{
	CSearchengine::copy(board, Color);
	alphabeta(m_nMaxDepth,Color,-100000,100000);//alphabeta
	theBestMove = BestMove;
	return theBestMove;
}

double CNegascout_HH_TT::alphabeta(int iDepth,int Color,double alpha,double beta)//深度优先
{
	int is;
	int iCount;
	double score;
	is=CSearchengine::IsGameOver();
	int iBestMove = -1;
	if(is==99999||is==-99999)//当前棋局已经结束
		{
			if (Color == nowColor)
				return is;
			else if(Color != nowColor)
				return -is;
	}
	if (iDepth <= 0)//叶子节点取估值
	{
		if (Color == nowColor)
			score = Eval(board, nowColor);
		if (Color != nowColor)
			score = -Eval(board, nowColor);
		return score;
	}
	//调用走法生成器函数产生当前局面的所有走法返回走法数量
	iCount = allMove(board, PossibleMoves[iDepth],Color);

	if(iDepth==m_nMaxDepth&&iCount==1)
	{
		BestMove = PossibleMoves[iDepth][0];
		return 66666;
	}
	for (int i = 0; i < iCount; i++)                           //遍历这些走法
	{
		CSearchengine::goAMove(board, PossibleMoves[iDepth][i]);
		//print_Board(position);
		score=-alphabeta(iDepth-1,3-Color,-beta,-alpha);//递归调用alphabeta函数进行下一层搜索
		CSearchengine::goBack(board, PossibleMoves[iDepth][i]);
		//print_Board(position);
		if(score>alpha)
		{
			alpha=score;
			if(iDepth==m_nMaxDepth)          //当搜索深度达到人为设置的最大搜索深度,m_nMaxDepth为人工设置的最大搜素深度
			{
					iBestMove = i;//直接 i no iBestMove
					BestMove = PossibleMoves[iDepth][iBestMove];
					//return i;
			}//将当前走法作为最佳走法并且放置于走法列表中
		}
		if(alpha>=beta)                          
			//return beta;
			break;
	}
	return alpha;
}

double CNegascout_HH_TT::Negascout_HH_TT(int Color,int iDepth, double iAlpha, double iBeta,int Step)
{
	int iCount, iGameOverRet;
	double a, b, t;
	double score;
	int i;

	iGameOverRet =CSearchengine:: IsGameOver();
	if(iGameOverRet==99999||iGameOverRet==-99999)//当前棋局已经结束
		{
			if (Color == nowColor)
				return iGameOverRet;
			else if(Color != nowColor)
				return -iGameOverRet;
	}
	
	if(First_In_N!=0)
	{
		score = LookUpHashTable(iAlpha, iBeta, iDepth);//查找置换表
		if (score != 66666)
			return score;
	}
	else
	First_In_N=1;

	if (iDepth <= 0)//叶子节点取估值
	{
		if (Color == nowColor)
			score = Eval(board, nowColor,Step);
		if (Color != nowColor)
			score = -Eval(board, nowColor,Step);
		EnterHashTable(exact, score, iDepth);//将估值存入置换表
			return score;
	}

	iCount = allMove(board, PossibleMoves[iDepth],Color);

	if(iCount==1&&iDepth==m_iMaxDepth)
	{
		theBestMove=PossibleMoves[iDepth][0];
		return 66666;
	}

	for (i = 0; i < iCount; i++)
		PossibleMoves[iDepth][i].score = LookUpHHTable(&PossibleMoves[iDepth][i]);
	qrs(PossibleMoves[iDepth], iDepth, 0, iCount - 1);

	int iBestMove = -1;
	a = iAlpha;
	b = iBeta;
	int eval_is_exact = 0;

	for (i = 0; i<iCount; i++)
	{
		if ((double)(clock() - timeStart) / CLOCKS_PER_SEC >= m_fTimeLimit)
			return 66666;//返回中止标记


		Hash_MakeMove(&PossibleMoves[iDepth][i], board);
		CSearchengine::goAMove(board, PossibleMoves[iDepth][i]);
		t = -Negascout_HH_TT(3-Color,iDepth - 1, -b, -a,Step);
		Hash_UnMakeMove(&PossibleMoves[iDepth][i], board);
		//Tree++;
		//if(Tree%1000==0)
		//	::PostMessage(AfxGetMainWnd()->m_hWnd,UPDATE_TREE,m_iMaxDepth,Tree);
		CSearchengine::goBack(board, PossibleMoves[iDepth][i]);
		 
		if (t>a && t<iBeta && i>0)
		{
			//对于第一个后的节点，如果上面的搜索failhigh
			Hash_MakeMove(&PossibleMoves[iDepth][i], board);
			CSearchengine::goAMove(board, PossibleMoves[iDepth][i]);
			a = -Negascout_HH_TT(3-Color,iDepth - 1, -iBeta, -t,Step);//递归搜索子节点
			eval_is_exact = 1;//设数据类型为精确值
			Hash_UnMakeMove(&PossibleMoves[iDepth][i], board);
			CSearchengine::goBack(board, PossibleMoves[iDepth][i]);
			if (iDepth == m_iMaxDepth)
			{
				iBestMove = i;
				BestMove = PossibleMoves[iDepth][iBestMove];
			}
		}
		
		if (a<t)
		{
			eval_is_exact = 1;
			a = t;
			if (iDepth == m_iMaxDepth)
			{
				iBestMove = i;
				BestMove = PossibleMoves[iDepth][iBestMove];
			}
		}
		if (a >= iBeta)
		{
			EnterHashTable(lowerBound, a, iDepth);
			EnterHHTable(&PossibleMoves[iDepth][i], iDepth);
			return a;
		}
		b = a +1;//set new null window
	}
	if (iBestMove != -1)
		EnterHHTable(&PossibleMoves[iDepth][iBestMove], iDepth);
	if (eval_is_exact)
		EnterHashTable(exact, a, iDepth);
	else
		EnterHashTable(UpperBound, a, iDepth);
	return a;
}
