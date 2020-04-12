#include "stdafx.h"
#include "windows.h"

#pragma  once

//定义几个常数
#define RED 1
#define BLUE -1
#define EMPTY 0
#define EDGE 10
#define BOX 20
#define RED_EDGE 1
#define BLUE_EDGE -1
#define RED_BOX 2
#define BLUE_BOX -2

//定义大小
#define LEN 11			//LEN为数组的长度，正常为格子数X2+1
#define MOVENUM 60		//MOVENUM为最大招式的大小，
#define BOXNUM 25		//总格子数
#define BOXLEN 5		//格子数组长度

//定义格子类型
#define FULLBOX	0		//满格
#define DEADBOX 1		//死格
#define CHAINBOX 2		//链格
#define FREEBOX 3		//自由格

//TURN类用于表示一个新的回合，里面包含了新回合的局面以及所落的招法
class TURN
{
public:
	TURN();
	void SetBoard(int Array[LEN][LEN]);
	void SetCaptualInfo(int OB[LEN][LEN], int NB[LEN][LEN]);
	int Owner;
	bool Captual[LEN][LEN];//被使用的招法
	int NewBoard[LEN][LEN];//新产生的局面
};

//LOC类用于表示坐标，无论是Box还是Edge都可以使用
class LOC
{
public:
	int x;
	int y;
	void Set(int lx, int ly);
	
};

//Board类是一个基本的棋盘类
class Board{
public:
	int board[LEN][LEN];
	int Step;

	Board();//构造函数
	Board(int Array[LEN][LEN], int step);			//构造函数
	void PrintBoard();								//显示函数1
	void ShowBoard();								//显示函数2
	void ShowLiberties();							//显示所有格子的自由度
	int GetEmptyEdges();							//得到未被占领的边的数目
	int GetEmptyBoxes();							//得到未被占领的格子的数目
	int GetEdgeOwner(int x,int y);					//得到某个边的所有者
	int GetBoxOwner(int x,int y);					//得到某个格子的所有者
	int GetBoxLiberties(int x,int y);				//得到某个格子的自由度
	int GetPlayerBoxes(int player);					//得到某个玩家占领的格子的总数
	int GetPlayerEdges(int player);					//得到某个玩家占领的边的总数
	bool GetEdgeLegality(int x,int y);				//得到某个边是否合法的函数
	int Winner();									//得到胜利者（-1/0/1）
	void SetBoard(int Array[LEN][LEN]);				//设置局面
	void LoadBoard();								//读取局面

	//局面估值
	bool GetFreeBoxBool(int bx, int xy);			//判断某个格子是否为自由格
	int GetFreeEdgeNum();							//得到自由边的数量
	bool GetCTypeBoxBool(int bx, int by);			//判断局面中是否有C型格
	bool GetCTypeBox(int Player,bool Msg);			//占领一个局面中的C型格
	bool GetLongCTypeBoxExist();					//判断局面中是否存在长C型链
	void GetAllCTypeBoxes(int Player,bool Msg);		//吃掉所有的C型格
	LOC FindNextBox(LOC FoundBox, LOC LastBox);		//查找某个链格

	//后期计算
	LOC GetDoubleCrossLoc(int Player);				//查找一个doublecross的制作方法，返回值为该边的坐标
	bool GetCTypeBoxLimit(int Player, bool Msg);	//限制版吃C型格

	//Move是当前最有效率的方法，也就是仅考虑最后一步，在之前均使用贪婪方法
	bool Move(int x, int y, int owner);							//占领一条边的函数
	int GetAllMoves(LOC Moves[MOVENUM]);						//得到所有的可行边
	int GetFreeMoves(LOC Moves[MOVENUM]);						//得到所有的自由边
	int GetFilterMoves(LOC Moves[MOVENUM]);						//得到所有的过滤可行边(不产生长链)
	int GetFilterMoveNum();										//得到所有的过滤可行边的数量
	bool GetLongCTypeBoxBool(int bx, int by);					//判断一个格子是否是一条长死格的起点

	//Turn被证明为低效率，比直接的考虑最后一步Move要慢大约1000倍，关键在于Turn需要构建新的Board，这一点十分耗时。
	void TurnMove(TURN Turn);									//一个回合移动
	int GetAllTurns(TURN Turns[MOVENUM], int Player);			//得到所有可行的回合
	int GetFilterTurn(TURN Turns[MOVENUM], int Player);			//获得接下来一个过滤的可行局面
};

//常用函数
void cprintf(char* str, WORD color, ...);
void cprintNum(int color, int Num);
bool LocEqual(LOC a, LOC b);
bool BoardEqual(int a[LEN][LEN], int b[LEN][LEN]);
void BoardCopy(int Source[LEN][LEN], int target[LEN][LEN]);
bool IfOddNumber(int num);
bool IfEvenNumber(int num);
void MoveMsg(int x, int y, int Player);