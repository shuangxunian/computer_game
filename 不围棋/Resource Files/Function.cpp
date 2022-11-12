//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//不围棋组负责人王佳俊所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//王佳俊	QQ：1601897441
#include "StdAfx.h"

#include "Function.h"
#include <math.h>
#include <stdio.h>

#define  PATCHLEVEL	2

#define  WALLY_OUTPUT  0
#define  MGT_OUTPUT    1
#define  CAT_OUTPUT    2


int outputmode = WALLY_OUTPUT;

#define SMALL 0	/*a number lower than any meaningful "urgency" */
/* or move importance*/
board Evaluate_Value;
/*Return TRUE if (x,y) corresponds to a point on the board.*/
#define on1board(x) (0<=(x)&&(x)<edge)
#define onboard(x,y) (on1board(x)&&on1board(y))  //棋子落在棋盘里

/*Return TRUE if (x,y) is on the edge of the board.*/ //如果(x,y)在棋盘边缘上就返回TURE
#define onedge1(x) (0==(x)||edge-1==(x))
#define onedge(x,y) (onedge1(x)||onedge1(y))

int flookup[]=	/*sets of flags corresponding to codes*/  
    { F_BLACK, F_WHITE, F_EMPTY };

/*Return the code for the other player, who is the next to play.*/ //下一个下棋的人
#define nextp(p) (BLACK==(p)?WHITE:(WHITE==(p)?BLACK:WHITE))
//	panic("illegal input to nextp()"))


/*Convert any uppercase letter to lowercase.*/ //大小写转换
#define lowercase(c) ('A'<=(c)&&(c)<='Z'?(c)-'A'+'a':(c))


/*other information about the game*/
struct thegame
{
	int pla;		/*code for next player to play*/ //下个棋手要走的代号
	int tur;		/*the number of this move*/ //已走的步数

} thegame;

#define PATTERN 12345	/*This integer should appear at the start of each */
#define PATTEND 7171	/*This integer should appear at the end of the table.*/ //结束的整数
//找出合适匹配的模版来供电脑调用
//#黑子 O白子 ~不确定 !边界 $当前位置*表示empty
int patterns_B[]= //BLACK方模式
	{	PATTERN, 4, 60,	/*PATTERN 代表模式的开始，有4该需要匹配的点，该模式的分值是60 */
       	-1, 0,F_WHITE|F_OFF,//*	o *
		 1, 0,F_WHITE|F_OFF,//o	$ *
		 0,-1,F_WHITE|F_OFF,//*	o *
		 0,1,F_EMPTY,

		 
		 PATTERN, 4, 50,	/*a code for the beginning of a pattern*/
       	-1, -1,F_BLACK|F_OFF,  //  #
		 1, -1,F_BLACK|F_OFF, //#  *  $
		 0,-1,F_EMPTY,        //   #
		 0,-2,F_BLACK|F_OFF,
		 
							//	!	
		 

		 PATTERN, 4, 40,	
       	 -1, 0,F_OFF,		//	!
		 0, -1,F_OFF,		//!	$ *
		 0, 1,F_EMPTY,		//	*
		 1, 0,F_EMPTY,		//


		 PATTERN, 4, 16,	/*a code for the beginning of a pattern*/
       	-1, 0,F_OFF ,                //	!
		 1, 0,F_EMPTY|F_WHITE,      //*	$ *
		 0, 1,F_EMPTY,		        //	o
		 0, -1,F_EMPTY,

	 PATTEND //模式结束
	};
int patterns_W[]=
	{	PATTERN, 4, 60,	/*a code for the beginning of a pattern*/
       	-1, 0,F_BLACK|F_OFF,
		 1, 0,F_BLACK|F_OFF,
		 0,-1,F_BLACK|F_OFF,
		 0,1,F_EMPTY,

		 
		 PATTERN, 4, 50,	
       	-1, -1,F_WHITE|F_OFF,
		 1, -1,F_WHITE|F_OFF,
		 0,-1,F_EMPTY,
		 0,-2,F_WHITE|F_OFF,
		 
		
	

		  PATTERN, 4, 40,	
       	 -1, 0,F_OFF,
		 0, -1,F_OFF,
		 0, 1,F_EMPTY,
		 1, 0,F_EMPTY,

	
		  
		  PATTERN, 4, 16,	/*a code for the beginning of a pattern*/
       	-1, 0,F_OFF ,                //	!
		 1, 0,F_EMPTY|F_BLACK,      //*	$ *
		 0, 1,F_EMPTY,		        //	o
		 0, -1,F_EMPTY,

		
	 PATTEND
	};

int patnum;


/*
a table of moves the heuristics are ambivalent about, for
random selection
*/
int goodmoves[2*EDGE*EDGE];
int *pgoodmoves;	/*&next free space in goodmoves[]*/ //goodmoves中的元素个数


int rng(int n)// 该函数很难理解
//返回一个0到n-1间的数，舍去不太好的模版
/*Return a (slightly) random number from 0 to n-1.*/
/*(This is a really crummy rng, it just keeps the
/*program's moves from all lying in a trivial pattern.)*/
{
	static int seed= 0x1231;
	int r;
	seed= (seed*11+15) & 0x7FFF;
	r= (((long)seed)*n)/(0x7FFF+1);
	return r;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Function::Function()  //构造函数
{

}

Function::~Function() //析构函数
{

}

void Function::initgame() //初始化游戏
{
	register int x, y;

	edge=EDGE;        //边长9 


	player=&thegame.pla;

	/*Clear the board.*/
	for (x=0; x<edge; ++x)  for (y=0; y<edge; ++y)
			theboard[x][y]= EMPTY;

	/*Initialize miscellaneous other stuff.*/

	thegame.tur= 1;     //已走的步数1
	
	mx=my=-1;


}

int Function::nextmove()
//下一步该谁走
{
	
	
	if (thegame.pla==ComputerColor)
		return mymove();
	else
		return enemymove();    
	
	
}

int Function::nextmove(int x, int y)
{
	mx=x;
	my=y;
	return nextmove();
}



void Function::movedone() //落子后的交换对手
/*
Do everything which must be done to complete a move after the stone is
placed.
*/
{
	thegame.pla = nextp(thegame.pla);
	++thegame.tur;
}


void Function::count(register int x, register int y,  register group *thisgroup,  board scratch,  int mark)
/*
Recursively group together connected stones.  Three things must be done: 递归相关的棋子
(1) 将thisgroup->nliberties中赋上这个棋链的气数
(2) 计算棋盘上已有棋子thisgroup->nstones, scratch[][]= mark是(x,y)和this group中的棋子数目，
因为这个组是有(x,y)为中心形成的组
so that we only count each only once.  The calling routine must
see to it that scratch[][]!=mark for all points and liberties that
it wants counted.
*/
{
	register int *bxy, *sxy; 	//  *bxy=theboard[x][y] ; *sxy=scratch[x][y] //??why int type

endrecurse:						//设置跳转标志位
	bxy= &(theboard[x][y]);                   //theboard[x][y]的地址
	sxy= &(scratch[x][y]);                    //scratch[x][y]的地址

	//计算XY点使之成为组中一员
	++(thisgroup->nstones);                  //属于这个group中的数字加‘1’
	*sxy= mark;                              //使得scratch[x][y]=mark

	y = y-1;                                  //处理scratch[x][y-1]和theboard[x][y]
	bxy -= 1;
	sxy -= 1;
	if (y>=0)
		if (thisgroup->color==*bxy)            //theboard[][]现在已有棋子，
		{
			if (*sxy!=mark)
				count(x, y, thisgroup, scratch, mark);//递归查询
		}
		else if (EMPTY==*bxy)                  //没有棋子的话
		{
			if (*sxy!=mark)
			{
				*sxy=mark;
				++(thisgroup->nliberties);
			}
		}


	y= y+2;                             //处理点 (x, y+1)
	bxy += 2;
	sxy += 2;
	if (y<edge)
		if (thisgroup->color==*bxy)
		{
			if (*sxy!=mark)
				count(x, y, thisgroup, scratch, mark);
		}
		else if (EMPTY==*bxy)
		{
			if (*sxy!=mark)
			{
				*sxy=mark;
				++(thisgroup->nliberties);
			}
		}


	y = y-1;                            //处理点(x-1, y)
	x = x-1;
	bxy -= EDGE+1;
	sxy -= EDGE+1;
	if (x>=0)
		if (thisgroup->color==*bxy)
		{
			if (*sxy!=mark)
				count(x, y, thisgroup, scratch, mark);
		}
		else if (EMPTY==*bxy)
		{
			if (*sxy!=mark)
			{
				*sxy=mark;
				++(thisgroup->nliberties);
			}
		}


	x = x+2;                           //处理点(x+1, y)
	bxy += 2*EDGE;
	sxy += 2*EDGE;

	if (x<edge)
		if (thisgroup->color==*bxy)
		{
			if (*sxy!=mark)
				goto endrecurse;//重新递归
		}
		else if (EMPTY==*bxy)
		{
			if (*sxy!=mark)
			{
				*sxy=mark;
				++(thisgroup->nliberties);
			}
		}

}


int Function::myloss(int x,int y,int p)//判断落子点的气数，如果为0则返回1，否则返回0.
{
	board scratch;	/*scratch for count() to use to say if a point is */

	register group thisgroup;
	int xb,yb,np=nextp(p);
	{
		register int *s,i;
		for (s=scratch[0],i=0; i<EDGE*EDGE; ++i)
			*s++= 0;
	}
 					thisgroup.color= p;
					thisgroup.x= x;
					thisgroup.y= y;
					thisgroup.nliberties= 0;
					//thisgroup.nstones= 0;
					count(x, y, &thisgroup, scratch, 1);
					if (thisgroup.nliberties==0)
						return 1;
				
		xb=x-1;
		yb=y;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			//thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 2);
			}
			if (thisgroup.nliberties==0)
				 return 1;
			
		xb=x+1;
		yb=y;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color= np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			//thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 3);
			}
			if (thisgroup.nliberties==0)
				 return 1;

		xb=x;
		yb=y-1;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			//thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 4);
			}
			if (thisgroup.nliberties==0)
				return 1;

		xb=x;
		yb=y+1;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 5);
		}
			if (thisgroup.nliberties==0)
				 return 1;
		

		return 0;	
}

int Function::placestone(int x, int y, int p)//落子，并判断是否输了

{
	int ncap;
 //if(onboard(x,y)&&theboard[x][y]==EMPTY)
	theboard[x][y] = p;
	ncap=myloss(x,y,p);
 
	if (1==ncap)
		return 0;//输了
			
	return 1;
}




int Function::judgement(int *ua,int x,int y,int p)//对xy处的棋子进行环境判断，该函数非常重要
{
	board scratch;	/*scratch for count() to use to say if a point is */
//	register int *bxy= &theboard[x][y], *sxy= &scratch[x][y];
	register group thisgroup;
	int xb,yb,np=nextp(p);
	if(onboard(x,y)&&theboard[x][y]==EMPTY)
			theboard[x][y]=p;
	else
		return 0;

	{
		register int *s,i;
		for (s=scratch[0],i=0; i<EDGE*EDGE; ++i)
			*s++= 0;
	}


					thisgroup.color= p;
					thisgroup.x= x;
					thisgroup.y= y;
					thisgroup.nliberties= 0;
					thisgroup.nstones= 0;
					count(x, y, &thisgroup, scratch, 1);
					if (thisgroup.nliberties==0)
						{*ua=0; theboard[x][y]=EMPTY; return 0;}
					else if (thisgroup.nliberties==1)
						*ua=*ua+3;
				
		xb=x-1;
		yb=y;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 2);
			if (thisgroup.nliberties==0)
				{*ua=0; theboard[x][y]=EMPTY; return 0;}
			else if (thisgroup.nliberties==1)
				*ua=*ua-3;
			
			}
		

		xb=x+1;
		yb=y;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color= np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 3);
			if (thisgroup.nliberties==0)
				{*ua=0; theboard[x][y]=EMPTY; return 0;}
			else if (thisgroup.nliberties==1)
				*ua=*ua-3;
		
			}
		

		xb=x;
		yb=y-1;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 4);
			if (thisgroup.nliberties==0)
				{*ua=0; theboard[x][y]=EMPTY;return 0;}
			else if (thisgroup.nliberties==1)
				*ua=*ua-3;
			
			}
		

		xb=x;
		yb=y+1;

		if (onboard(xb,yb)&& theboard[xb][yb]==np && 0==scratch[xb][yb])
			{
			thisgroup.color=np;
			thisgroup.x= xb;
			thisgroup.y= yb;
			thisgroup.nliberties= 0;
			thisgroup.nstones= 0;
			count(xb, yb, &thisgroup, scratch, 5);
			if (thisgroup.nliberties==0)
				{*ua=0; theboard[x][y]=EMPTY; return 0;}
			else if (thisgroup.nliberties==1)
				*ua=*ua-3;
			
			}
		

	

		theboard[x][y]=EMPTY;
		return 1;

}

//void Function::pattern1(int *u,  board masks, board movehere,int *patAdd)  //用来寻找匹配的模版
//这个函数很难理解，如果不理解可先不细看，只是知道它是进行模版匹配
void Function::pattern1(int *u,  board masks, board movehere,int *patAdd,int Lab)  //用来寻找匹配的模版
/*加了一个int Lab参数，目的是想知道当前调用的pattern1是在pattern中第几次调用，方便了调试。
  用来从pattern()中寻找匹配的走法，只找最佳的，对于不好的不予理会。
 只有EMPTY被检验了，所以pattern()可被用来寻找相关的走步或其他
有好的模版的限制性设置。最后的修正允许用movehere[][]来说明最佳走步的好处
*/
{
	register int *is, *iis;/*pointers into patterns[]; or scratch*/ 
	register int j;	/*& into a particular（特殊） pattern, # points remaining（剩余）*/ //指向特别的模版
	register int x, y;	/*current position we're trying to match pattern to*/  //我们试着用来匹配模版的通用情况
	register int xs, ys;	/*position of a point from a pattern*/  //模版中的一点
	int ua;		/*urgency and adjustment for this move*/ //这一步的判断
	int thispat;		/*which pattern in the table are we currently */
	/* trying to match?*/  //我们用的是哪一个模版来匹配
	int *patterns = patAdd;


	for (is=patterns,thispat=0; PATTERN==*is; is+= 3+3*(is[1]),++thispat) //is[1]=*(is+1)
	{

		for (x=0; x<edge; ++x)
			for (y=0; y<edge; ++y)
			{
				if (F_EMPTY==masks[x][y] )
				      
				{
					for (iis=is+1,j= *iis++,ua= *iis++; j; --j)//首次循环 
					{
						xs= *iis++;
						ys= *iis++;
						if (onboard(x+xs,y+ys))
						{
							if (0 == (masks[x+xs][y+ys] & *iis++)) goto mismatch;//不相同的
						}
						else
						{
							if (0 == (F_OFF & *iis++)) goto mismatch;//不是边界
						}
					}
					//如果执行到这个地方来了，我们就匹配到一个模版
					/*Compute adjusted urgency.*/
					if (judgement(&ua,x,y,*player))
					{
								if ( ua>*u) 
								{
									*u= ua;	 				/*Replace old */
									movehere[x][y]= ua;     /* urgency values.*/  //如果局势紧急，而且走步还可以，
									patnum= thispat;	    /*Record pattern # for debugging.*/
									pgoodmoves= goodmoves;  /*Reinit（恢复） goodmoves[].*/ //把它记录入goodmoves，goodmoves中下标从小到大一次放最好的走法次之再次在
									goto intogoodmoves;
								}						//先找一个比较好的走法
								else if ( ua==*u &&	      ua>movehere[x][y]	)/* and it's the best move here */
										  
								{
									movehere[x][y]= ua;	/*Mark as best move here.*/
						intogoodmoves: ;									/*Put it into goodmoves[].*/  //存入goodmoves[]最好的走法放在最前面
									*pgoodmoves++= x;
									*pgoodmoves++= y;
								}
					}
				}
mismatch:
				;
			}
	}
}

//用来对匹配的模版进行8个方向的映射
//这个函数很难理解，如果不理解可先不细看，只是知道它是进行8个方向映射
void Function::pattern(int *chosenx, int *choseny, int *urgency,  board movehere,int *patAdd)
/*
Try to find a good black move by matching to a table of patterns.
Returns the urgency of the move; returns SMALL if no match found.
寻找匹配的模式。如果找不到合适的则返回SMALL

In order to match the patterns in all orientations, we
reflect(反思) the entire（全部） table eight times, checking for a match each time.
The reflections are
(1)   y <-> edge-1-y	(across a mirror plane parallel to x-axis)
(2)	x <-> edge-1-x	( " y-axis)
(3)   y <-> edge-1-y	( " x-axis)
(4)   x <-> y		( " the line y=x)
(5)   y <-> edge-1-y	(across a mirror plane parallel to x-axis)
(6)	x <-> edge-1-x	( " y-axis)
(7)   y <-> edge-1-y	( " x-axis)
(8)   x <-> -y	( " the line y=(-x))
Draw pictures if necessary to see how this works.
*/
{
	register int x, y, t, j, *is;
	board scratch;
	int *patterns = patAdd;
	
	/*Translate the board to flags for easy comparison with pattern table.*/
	for (x=0; x<edge; ++x)
		for (y=0; y<edge; ++y)
			scratch[x][y]= flookup[theboard[x][y]];  //寻找每点的值flookup[0]=1,flookup[1]=2,flookup[2]=4
									//theboard[x][y]=0 BLACK theboard[x][y]=1 WITHE theboard[x][y]=2 EMPTY SYA2012-3-29

	*urgency= SMALL;		/*No small move so far.*/
	pgoodmoves= goodmoves;	/*No moves so far.*/
	patnum= (-1);			/*For debugging: no pattern # so far.*/

	pattern1(urgency, scratch, movehere,patAdd,1);
	/*Find matches to untransformed table.*/

	/*Invert y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			is += 1;
			*is= (-*is);
			is += 2;
		}
	}

	pattern1(urgency, scratch, movehere,patAdd,2);
	/*Find matches to transformed table.*/

	/*Invert x coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			*is= (-*is);
			is += 3;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,3);
	/*Find matches to transformed table.*/

	/*Invert y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			is += 1;
			*is= (-*is);
			is += 2;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,4);
	/*Find matches to transformed table.*/

	/*Exchange x and y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			t= *is;
			*is= is[1];
			is[1]= t;
			is += 3;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,5);
	/*Find matches to transformed table.*/

	/*Invert y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			is += 1;
			*is= (-*is);
			is += 2;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,6);
	/*Find matches to transformed table.*/

	/*Invert x coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			*is= (-*is);
			is += 3;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,7);
	/*Find matches to transformed table.*/

	/*Invert y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			is += 1;
			*is= (-*is);
			is += 2;
		}
	}
	//  assert(PATTEND==*is);

	pattern1(urgency, scratch, movehere,patAdd,8);
	/*Find matches to transformed table.*/

	/*Exchange x and -y coordinates in pattern table.*/
	for (is=patterns; PATTERN==*is; )
	{
		for (j= *++is,is+=2; j--; )
		{
			t= (-*is);
			*is= (-is[1]);
			is[1]= t;
			is += 3;
		}
	}
	//  assert(PATTEND==*is);

	/*Try to pick a move from the table of equally good ones.*///?????
	{
		int nmoves= (pgoodmoves-goodmoves)/2;
		if (0==nmoves) return;
		else
		{
			int whichmove= rng(nmoves);
			*chosenx= goodmoves[2*whichmove];
			*choseny= goodmoves[2*whichmove+1];
			//*chosenx= goodmoves[0];
			//*choseny= goodmoves[1];
		}
	}

}


int Function::mymove() //计算和执行电脑的走步
/*Calculate and execute and print out the computer's move.*/
{
	int x, y,u;			/*coords of the move selected*/
	int upattern, patternx, patterny;
	int *patt;

	/*Find the most urgent move to improve our shape.*/		 //寻找最需要的走法用来改善我们的局势
	/*First say that all moves are to be considered.*/	   	//所有的走法都要考虑
	SetBoard(Evaluate_Value,SMALL);	//给scratch赋初值10000
	/*Then call pattern search.*/  //用已有的模版搜索
	patt=(*player==WHITE?patterns_W:patterns_B);
	pattern(&patternx, &patterny, &upattern, Evaluate_Value,patt);
	
//模版匹配成功	
if (upattern>1)
	{		x= patternx;
			y= patterny;
			goto movexy;
	}
//没有匹配的摸版，则用judgement函数随机找一点
	for (x=0; x<edge; ++x)
		for (y=0; y<edge; ++y)
				
			if(EMPTY==theboard[x][y])
				if (judgement(&u,x,y,*player))
				{
					goto movexy;
				}
//用以上方法都没有找到最佳点，只好随机找一个EMPTY点
		for (x=0; x<edge; ++x)
			for (y=0; y<edge; ++y)
				
				 if(EMPTY==theboard[x][y])
				{
					goto movexy;
				}
	

	return COMPUTER_PASS;//计算机不走棋

movexy:
	//SaveScratch(Evaluate_Value,"scratch_patterns.txt");
	mx=x;
	my=y;
	if(0==placestone(mx, my, ComputerColor))//落子并判断输赢
		return COMPUTER_LOS;
	else
	{
		movedone();//交换对手
		return 0;
	}
}


int Function::enemymove() //对手的走步，（下棋人）
/*Read and execute opponent's move.*/
{

	

	if(0==placestone(mx, my, *player))
		return COMPUTER_WIN;
	else
	{
	
		movedone();
		return 0;
	}
}




//下面三个函数是将数据存储到磁盘文件中。
void Function::SetBoard(board scratch,int value)
{
	register int *ip;
	register int j;
	ip = (int*)scratch;
	for (j=EDGE*EDGE;j>0;j--)//给scratch赋初值10000
	{
		*ip = value;
		ip++;
	}
}


void Function::SaveBoard()
{
	FILE *fp;
	int i,j;

	fp = fopen("board.txt","w");
	for(i=0;i<EDGE;i++)
	{
		for(j=0;j<EDGE;j++)
		{
			fprintf(fp,"%d,",theboard[j][i]);
		}
		fprintf(fp,"-1,-1,-1,-1,\n");
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<EDGE;j++) 
			fprintf(fp,"-1,");
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void Function::SaveScratch(int (*scratch)[EDGE],char *name)//为调试写的
{
	FILE *fp;
	int i,j;

	fp = fopen(name,"w");
	for(i=0;i<EDGE;i++) //i<19
	{
		for(j=0;j<EDGE;j++) //j<19
		{
			
			if (scratch[j][i]==SMALL)  fprintf(fp,"%s,","*");
			else                      fprintf(fp,"%d,",scratch[j][i]);
		}
		fprintf(fp,"-1,-1,-1,-1,\n");
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<EDGE;j++) //j<23
			fprintf(fp,"-1,");
		fprintf(fp,"\n");
	}
	fclose(fp);
}
