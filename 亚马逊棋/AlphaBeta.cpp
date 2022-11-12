#include"AlphaBeta.h"
#include<windows.h>

int Step = 0;
int SearchDepth = 2;   //搜索两层
int CurPosition[12][12] = { 0 };   //将实际操作的棋盘复制到这里，在这个棋盘上进行模拟行棋和评估
CHESSMOVE BestMove = { 0 };


void SearchAGoodMove(int Board[12][12])   //传入一个12*12的二维数组
{
	memcpy(CurPosition, Board, 144 * sizeof(int));   //复制传入的Board棋盘到CurPosition
	AlphaBeta(SearchDepth);    //第一次搜索传入设置的搜索深度
	MakeMove(&BestMove);
	memcpy(Board, CurPosition, 144 * sizeof(int));
	Step = 0;
}

double AlphaBeta(int depth, double alpha, double beta)
{
	double score = 0;   //定义评估分数
	int type = 0;
	if (JudgeWin(CurPosition) != 0)//分出胜负
	{
		return Evaluate();//返回评估值
	}
	if (depth == 0)//到达叶子节点
	{
		if ((SearchDepth - depth) % 2 == 0)//极大值结点
		{
			if (mode == 1)
			{
				return Evaluate();
			}
			if (mode == 0)
			{
				return -Evaluate();
			}
		}
		else
		{
			if (mode == 1)
			{
				return -Evaluate();
			}
			if (mode == 0)
			{
				return Evaluate();
			}
			
		}
	}
	///////////先后手转换/////////
	Step = Step + 1;      //每搜索一层就是棋局进行了一步
	if (mode == 1)        //电脑先手，电脑执黑
	{
		if (Step % 2 == 1)//奇数层
		{
			type = mode + 1;
		}
		else if (Step % 2 == 0)
		{
			type = mode;
			//MessageBox(NULL, "测试！", "提示", MB_OK);
		}
	}
	if (mode == 0)  //电脑后手
	{
		if (Step % 2 == 0)  //奇数层
		{
			type = mode+1;
		}
		else if (Step % 2 == 1)  //偶数层
		{
			type = mode+2;
		}
	}
	/////////////////////////////
	int count = CreatePossibleMove(CurPosition, depth, type);  //记录步法总数，生成所有符合规则的步法
	for (int i = 0; i < count; i++)
	{
		MakeMove(&MoveList[depth][i]);            //模拟行棋，在CurPosition上模拟
		score = -AlphaBeta(depth -1, -beta, -alpha);   //递归向下搜索
		UnMakeMove(&MoveList[depth][i]);              //撤销模拟行棋
		if (score > alpha)
		{
			alpha = score;
			if (depth == SearchDepth)
			{
				BestMove = MoveList[depth][i];
			}
		}
		if (alpha >= beta)
		{
			break;
		}
	}
	return alpha;
}