// Function.h: interface for the Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTION_H__8FDE5B4D_B014_47EF_9866_3A5F052CA8BB__INCLUDED_)
#define AFX_FUNCTION_H__8FDE5B4D_B014_47EF_9866_3A5F052CA8BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define EDGE  9 //23		/*max # intersections on the edge of a go board*/

#define RESIGN (-2)	/*codes for resigning */
#define PASS (-1)	/* passing moves*/
#define BOTHPASS (-3)	/*code for both sides passed, game is over*/
#define FAIL (-4)            //放子错误
#define COMPUTER_WIN   1000 //计算机赢了。
#define COMPUTER_LOS  -1000//计算机输了。
#define COMPUTER_PASS -1//计算机不走了
/*codes for players; and a code for something (e.g. a point on the board) */
/* which belongs to neither*///棋子和选手代号
#define BLACK 0
#define WHITE 1
#define EMPTY 2

/*flags in shape table entries for types of points*///入口标准用于模式匹配
#define F_BLACK 1	/*the point has a black stone*///(0001)B
#define F_WHITE 2	/*the point has a white stone*///(00010)B
#define F_EMPTY 4	/*the point is empty*///(0100)B
#define F_OFF   8	/*the point is off the board*///提子点(1000)B//边界点



typedef struct group
{
	int	color,	/*the color of stones in this group*/ //棋子的颜色
	nliberties,	/*the number of liberties(自由) that this group has*/
	nstones,	/*the number of stones in this group*/ //组里有棋子的数目
	x, y;		/*one point in this group*/ //盘上的点

} group;

/*a struct for holding information on every square of the board*/ //存储棋盘上的数据
typedef int board[EDGE][EDGE];


class Function  
{
public:
	Function();
	virtual ~Function();

	int edge;		/*the number of intersections which we are using (a command line parameter)*/
	board theboard;	/*current position of stones on the board*/ //
	int mx,my;		// move of the game 步数
	int *player;		// next player to play
	int ComputerColor;//计算机棋子颜色



	void SaveBoard();	//用于保存当前棋盘
	void SaveScratch(int (*scratch)[EDGE],char *);//用于保存scratch，供调试使用。Liuc:2011-05-28

	void initgame();//初始化游戏
	int nextmove();//判断下一步走棋
	int nextmove(int x, int y);

	void movedone();
	int placestone(int x, int y, int p);//落子
	
	void count(register int x, register int y,  register group *thisgroup,  board scratch,  int mark);//计算机棋链的气

	
	void pattern1(int *u,  board masks, board movehere,int *patAdd,int);//模式匹配
	void pattern(int *chosenx, int *choseny, int *urgency,  board movehere,int *patAdd);//将模式映射成8种形式
	int mymove();//计算机走棋
	int enemymove();//对手走棋
	void SetBoard(board scratch,int value);
	int judgement(int *ua,int x,int y,int p);//判断(x,y)点处的环境
	int myloss(int x,int y,int p);//判断(x,y)落子点输赢

};

#endif // !defined(AFX_FUNCTION_H__8FDE5B4D_B014_47EF_9866_3A5F052CA8BB__INCLUDED_)
