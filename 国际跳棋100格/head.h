#pragma once

	const int directions[4][2]={{1,1},{1,-1},{-1,1},{-1,-1},};//这个每个方向的位置不能变，现在是黑棋在上白棋在下的顺序

	#define File_Path	"d:\\对战记录.txt"
	#define TIMER		1
	#define Blocked		1
	#define Open		2
	#define Chose		4
	#define Finish		8

	#define Flip		1
	#define Normal		0

	#define EMPTY		0
	#define WHITE		2
	#define BLACK		1
	#define W_KING		4
	#define B_KING		3
	#define BAR			5
	#define depthMax	9

	#define D_Pieces                        1
	#define D_Board                         2
	#define D_Prompt                        4

	#define Negascout                       1
	#define AlphaBeta                       2
	#define Minimax                         4

	#define Input							8
	#define Dele							16
	#define GetAllMove						32

	#define All_Move_Finish					1
	#define AI_Finish						1
	#define Human_Finish					2

	#define HvsH                            1
	#define HvsAI                           2	
	#define AIvsH                           4

	#define Err_Engine_AB					2
	#define Err_Engine_N					1
	#define Err_Engine_Check				4

	#define WM_MY_MESSAGE					WM_USER + 1
	#define UpdateMoveDataMess				WM_USER + 2
	#define Get_All_Move_Finish				WM_USER + 3
	#define Input_Depth_Time_Finish			WM_USER + 4
	#define UPDATE_TREE						WM_USER + 5
	typedef __int64 LONGLONG;
	typedef unsigned __int64 ULONGLONG;
	typedef int                 INT;
	typedef unsigned int        UINT;
	typedef long LONG;
	typedef unsigned char       BYTE;
	typedef __int64 LONGLONG;

	struct MOVE
	{
		int Kind;//记录种类;
		int move_star_x;//起点坐标
		int move_star_y;
		int move_stop_x;//终点坐标
		int move_stop_y;
		int eatHowMany;//吃子数量，同时也是eatHowMany的长度
		int eatPoint[20][2];
		int eatKind[20];//记录吃掉棋子的种类
		double score;//待用，迭代深化模型有可能有用上
	};

	struct MOVE_MESS
	{
		MOVE The_Good_Move;
		int Add_Mess;//附加消息
	};//用于线程间传递消息

	struct BOARD_MESS
	{
		int Board[10][10];
		int Color;
		int Add_Mess;//附加消息通常是引擎
	};//用于线程间传递消息

	/*struct MOVE_PT
	{
	
		POINT pt[10][20];
		int move_num;
		int eat_num[10];
		int i[10];
	
	};*/

	enum ENTRY_TYPE{ exact, lowerBound, UpperBound };//哈希表中解决哈希冲突的一个方法	

	struct HASHITEM{
	LONGLONG checksum;
	double eval;
	short depth;
	ENTRY_TYPE enterType;
};//哈希元素结构体

	struct HH_HASHITEM{
	LONGLONG checksum;	  //64位校验码
	double score;		  //历史分
	short depth;
};//历史表元素结构体(哈希)
	

