#pragma once
#include"head.h"
class makeNowMoves
{
public:
	makeNowMoves();//初始化当前招法，及棋盘；
	~makeNowMoves();//不知道什么用，习惯的给定义成虚函数了
	//与兵相关的成员函数
private:
	void copy(int Color,int board[10][10]);//获取当前走子方，及当前局面
	void makeNowBingMove(int starX,int starY,int eatHowMany,MOVE movesCoutainer[200]);//生成当前棋子(兵)的招法
	//与王相关的成员函数
	void makeNowKingMove(int starX,int starY,int eatHowMany,MOVE movesCoutainer[200]);//生成当前棋子(王)的招法
	bool KING_FOUR_DIRECTION_CAN_EAT(int X,int Y);//这个是判断王的能不能吃
	bool BING_FOUR_DIRECTION_CAN_EAT(int X,int Y);
	MOVE nowTheMove;//正在生成的结构体，如果满足条件加入到招法记录的数组中
	int board[10][10];//需要对该局面生成招法
	int nowColor;//对该种棋子生成招法
	int theBestEatNum;//记录生成时当前的最大吃子数量（这个成员真的很坑啊，差个1就会出问题）
	int nowTheNumberOfMove;//招法的记录到第几的位置了，最后在接口函数返回这个值，作为招法总数；
public:
	int allMove(int board[10][10],MOVE movesCoutainer[200],int Color);//接口函数
};

