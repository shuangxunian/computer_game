#include "stdafx.h"
#include "windows.h"

#pragma  once


enum ChainType
{
	NotDefine, SingleChain, LongChain, ShortChain, Circle, PreCircle, DeadChain, DeadCircle
};

//BoxType类是一个用于鉴定后期格子类型的类
class BoxType
{
public:
	BoxType();
	LOC BoxLoc;
	int BoxOwner;			//格子的属主，大部分格子没有属主
	int Type;				//[0]已被占领 [1]死格 [2]链格 [3]自由格
	int BelongingChainNum;		//所属链类型的编号，会从1开始自动编号。
};

//ChainInfo是一个包含了一条链的基本信息的类
class ChainInfo
{
public:
	ChainInfo();
	ChainType Type;
	int ChainBoxNum;
	LOC StartLoc;
	LOC EndLoc;
	bool ConditionOfPreCircle;
};

//BoxBoard类是一个进阶的类
class BoxBoard :public Board
{
public:
	BoxBoard(Board NewB);						//构造函数
	BoxBoard(int Array[LEN][LEN], int step);	//构造函数
	BoxType Boxes[BOXLEN + 1][BOXLEN + 1];			//后期棋盘的基本类型，由格子组成,编号从1开始。
	ChainInfo Chains[BOXNUM];					//链的数量必定不会超过全部格子数
	int GetFirstEmptyChainNum();				//获得一个空的链编号
	void DefineBoxesType();						//将所有格子的基本信息填入。
	int GetBoxType(int bx, int by);				//得到某个格子的类型，不在坐标范围内的全部返回自由格类型[坐标限制为1到BOXLEN]。
	void ShowBoxType();							//显示所有格子的类型

	//鉴别链
	void ShowBelongingChain();									//显示所有格子所属的链与各个链的信息
	void InheritChain(int InheritorRegNum, int AncesterRegNum);	//一条链吞并另一条链
	void RegisterChain(LOC FreeBoxLoc, LOC NextLoc);			//从一个场内的自由格出发，给所有的派生链做一个定义。
	void RegisterCircle(LOC StartLoc, LOC NextLoc);				//从一个格出发，确定是否为一个环。
	void SearchingFromBox(LOC BoxLoc);							//从一个格出发，注册他的所有派生链,ChianPlus应仅在没有短链时启用。
	void SearchingCircle(LOC BoxLoc);							//从一个格出发，注册他的所有派生环。
	void ResetChainsInfo();										//重定义链与格的的信息
	void DefineAllChains(bool ChainPlus);						//定义所有的链

	//说明：链是动态注册的。当一条链被归类到另一条链时候，他本身会被重新定义为"NotDefine"，而每次需要注册链的时候会从0开始查找到一条空链。

	//类型判断
	void RegisterDeadChain(LOC FreeBoxLoc, LOC NextLoc);		//从一个场内的死格出发，给其派生链做一个定义。
	void SearchingDeadChain(LOC BoxLoc);						//从一个格出发，注册他的所有派生环。
	void DefineDeadChain();										//检查所有的死格
	bool GetDeadChainExist();
	bool GetDeadCircleExist();

	//占据链
	bool CaptualAnySingleChain(int LatterPlayer);				//随机吞并一条单链，参数为占领者。
	bool CaptualAnyShortChain(int LatterPlayer);				//随机吞并一条短链，参数为占领者。
	bool CaptualShortestChain(int LatterPlayer);				//吞并一条当前最短的长链或者环

	//打开链
	LOC GetOpenShortestChainLoc();
	LOC GetOpenSuitableChainLoc();

	//最终判定局面
	bool RationalState(LOC BoxNum);								//判断双方是否为理性状态
	LOC GetRationalStateBoxNum();								//获得当前局面下双方以理性状态可以获得的格子数量，x为先手，y为后手
	int GetBoardWinner(int LatterPlayer);						//获得本局面下的胜利者，参数为后手方。
};