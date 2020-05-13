//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//不围棋组负责人王佳俊所带博弈组开发维护 
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
#define onboard(x,y) (on1board(x)&&on1board(y))  //��������������

/*Return TRUE if (x,y) is on the edge of the board.*/ //���(x,y)�����̱�Ե�Ͼͷ���TURE
#define onedge1(x) (0==(x)||edge-1==(x))
#define onedge(x,y) (onedge1(x)||onedge1(y))

int flookup[]=	/*sets of flags corresponding to codes*/  
    { F_BLACK, F_WHITE, F_EMPTY };

/*Return the code for the other player, who is the next to play.*/ //��һ���������
#define nextp(p) (BLACK==(p)?WHITE:(WHITE==(p)?BLACK:WHITE))
//	panic("illegal input to nextp()"))


/*Convert any uppercase letter to lowercase.*/ //��Сдת��
#define lowercase(c) ('A'<=(c)&&(c)<='Z'?(c)-'A'+'a':(c))


/*other information about the game*/
struct thegame
{
	int pla;		/*code for next player to play*/ //�¸�����Ҫ�ߵĴ���
	int tur;		/*the number of this move*/ //���ߵĲ���

} thegame;

#define PATTERN 12345	/*This integer should appear at the start of each */
#define PATTEND 7171	/*This integer should appear at the end of the table.*/ //����������
//�ҳ�����ƥ���ģ���������Ե���
//#���� O���� ~��ȷ�� !�߽� $��ǰλ��*��ʾempty
int patterns_B[]= //BLACK��ģʽ
	{	PATTERN, 4, 60,	/*PATTERN ����ģʽ�Ŀ�ʼ����4����Ҫƥ��ĵ㣬��ģʽ�ķ�ֵ��60 */
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

	 PATTEND //ģʽ����
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
int *pgoodmoves;	/*&next free space in goodmoves[]*/ //goodmoves�е�Ԫ�ظ���


int rng(int n)// �ú�����������
//����һ��0��n-1���������ȥ��̫�õ�ģ��
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

Function::Function()  //���캯��
{

}

Function::~Function() //��������
{

}

void Function::initgame() //��ʼ����Ϸ
{
	register int x, y;

	edge=EDGE;        //�߳�9 


	player=&thegame.pla;

	/*Clear the board.*/
	for (x=0; x<edge; ++x)  for (y=0; y<edge; ++y)
			theboard[x][y]= EMPTY;

	/*Initialize miscellaneous other stuff.*/

	thegame.tur= 1;     //���ߵĲ���1
	
	mx=my=-1;


}

int Function::nextmove()
//��һ����˭��
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



void Function::movedone() //���Ӻ�Ľ�������
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
Recursively group together connected stones.  Three things must be done: �ݹ���ص�����
(1) ��thisgroup->nliberties�и����������������
(2) ������������������thisgroup->nstones, scratch[][]= mark��(x,y)��this group�е�������Ŀ��
��Ϊ���������(x,y)Ϊ�����γɵ���
so that we only count each only once.  The calling routine must
see to it that scratch[][]!=mark for all points and liberties that
it wants counted.
*/
{
	register int *bxy, *sxy; 	//  *bxy=theboard[x][y] ; *sxy=scratch[x][y] //??why int type

endrecurse:						//������ת��־λ
	bxy= &(theboard[x][y]);                   //theboard[x][y]�ĵ�ַ
	sxy= &(scratch[x][y]);                    //scratch[x][y]�ĵ�ַ

	//����XY��ʹ֮��Ϊ����һԱ
	++(thisgroup->nstones);                  //�������group�е����ּӡ�1��
	*sxy= mark;                              //ʹ��scratch[x][y]=mark

	y = y-1;                                  //����scratch[x][y-1]��theboard[x][y]
	bxy -= 1;
	sxy -= 1;
	if (y>=0)
		if (thisgroup->color==*bxy)            //theboard[][]�����������ӣ�
		{
			if (*sxy!=mark)
				count(x, y, thisgroup, scratch, mark);//�ݹ��ѯ
		}
		else if (EMPTY==*bxy)                  //û�����ӵĻ�
		{
			if (*sxy!=mark)
			{
				*sxy=mark;
				++(thisgroup->nliberties);
			}
		}


	y= y+2;                             //������ (x, y+1)
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


	y = y-1;                            //������(x-1, y)
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


	x = x+2;                           //������(x+1, y)
	bxy += 2*EDGE;
	sxy += 2*EDGE;

	if (x<edge)
		if (thisgroup->color==*bxy)
		{
			if (*sxy!=mark)
				goto endrecurse;//���µݹ�
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


int Function::myloss(int x,int y,int p)//�ж����ӵ�����������Ϊ0�򷵻�1�����򷵻�0.
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

int Function::placestone(int x, int y, int p)//���ӣ����ж��Ƿ�����

{
	int ncap;
 //if(onboard(x,y)&&theboard[x][y]==EMPTY)
	theboard[x][y] = p;
	ncap=myloss(x,y,p);
 
	if (1==ncap)
		return 0;//����
			
	return 1;
}




int Function::judgement(int *ua,int x,int y,int p)//��xy�������ӽ��л����жϣ��ú����ǳ���Ҫ
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

//void Function::pattern1(int *u,  board masks, board movehere,int *patAdd)  //����Ѱ��ƥ���ģ��
//��������������⣬�����������Ȳ�ϸ����ֻ��֪�����ǽ���ģ��ƥ��
void Function::pattern1(int *u,  board masks, board movehere,int *patAdd,int Lab)  //����Ѱ��ƥ���ģ��
/*����һ��int Lab������Ŀ������֪����ǰ���õ�pattern1����pattern�еڼ��ε��ã������˵��ԡ�
  ������pattern()��Ѱ��ƥ����߷���ֻ����ѵģ����ڲ��õĲ������ᡣ
 ֻ��EMPTY�������ˣ�����pattern()�ɱ�����Ѱ����ص��߲�������
�кõ�ģ������������á���������������movehere[][]��˵������߲��ĺô�
*/
{
	register int *is, *iis;/*pointers into patterns[]; or scratch*/ 
	register int j;	/*& into a particular�����⣩ pattern, # points remaining��ʣ�ࣩ*/ //ָ���ر��ģ��
	register int x, y;	/*current position we're trying to match pattern to*/  //������������ƥ��ģ���ͨ�����
	register int xs, ys;	/*position of a point from a pattern*/  //ģ���е�һ��
	int ua;		/*urgency and adjustment for this move*/ //��һ�����ж�
	int thispat;		/*which pattern in the table are we currently */
	/* trying to match?*/  //�����õ�����һ��ģ����ƥ��
	int *patterns = patAdd;


	for (is=patterns,thispat=0; PATTERN==*is; is+= 3+3*(is[1]),++thispat) //is[1]=*(is+1)
	{

		for (x=0; x<edge; ++x)
			for (y=0; y<edge; ++y)
			{
				if (F_EMPTY==masks[x][y] )
				      
				{
					for (iis=is+1,j= *iis++,ua= *iis++; j; --j)//�״�ѭ�� 
					{
						xs= *iis++;
						ys= *iis++;
						if (onboard(x+xs,y+ys))
						{
							if (0 == (masks[x+xs][y+ys] & *iis++)) goto mismatch;//����ͬ��
						}
						else
						{
							if (0 == (F_OFF & *iis++)) goto mismatch;//���Ǳ߽�
						}
					}
					//���ִ�е�����ط����ˣ����Ǿ�ƥ�䵽һ��ģ��
					/*Compute adjusted urgency.*/
					if (judgement(&ua,x,y,*player))
					{
								if ( ua>*u) 
								{
									*u= ua;	 				/*Replace old */
									movehere[x][y]= ua;     /* urgency values.*/  //������ƽ����������߲������ԣ�
									patnum= thispat;	    /*Record pattern # for debugging.*/
									pgoodmoves= goodmoves;  /*Reinit���ָ��� goodmoves[].*/ //������¼��goodmoves��goodmoves���±��С����һ�η���õ��߷���֮�ٴ���
									goto intogoodmoves;
								}						//����һ���ȽϺõ��߷�
								else if ( ua==*u &&	      ua>movehere[x][y]	)/* and it's the best move here */
										  
								{
									movehere[x][y]= ua;	/*Mark as best move here.*/
						intogoodmoves: ;									/*Put it into goodmoves[].*/  //����goodmoves[]��õ��߷�������ǰ��
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

//������ƥ���ģ�����8�������ӳ��
//��������������⣬�����������Ȳ�ϸ����ֻ��֪�����ǽ���8������ӳ��
void Function::pattern(int *chosenx, int *choseny, int *urgency,  board movehere,int *patAdd)
/*
Try to find a good black move by matching to a table of patterns.
Returns the urgency of the move; returns SMALL if no match found.
Ѱ��ƥ���ģʽ������Ҳ������ʵ��򷵻�SMALL

In order to match the patterns in all orientations, we
reflect(��˼) the entire��ȫ���� table eight times, checking for a match each time.
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
			scratch[x][y]= flookup[theboard[x][y]];  //Ѱ��ÿ���ֵflookup[0]=1,flookup[1]=2,flookup[2]=4
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


int Function::mymove() //�����ִ�е��Ե��߲�
/*Calculate and execute and print out the computer's move.*/
{
	int x, y,u;			/*coords of the move selected*/
	int upattern, patternx, patterny;
	int *patt;

	/*Find the most urgent move to improve our shape.*/		 //Ѱ������Ҫ���߷������������ǵľ���
	/*First say that all moves are to be considered.*/	   	//���е��߷���Ҫ����
	SetBoard(Evaluate_Value,SMALL);	//��scratch����ֵ10000
	/*Then call pattern search.*/  //�����е�ģ������
	patt=(*player==WHITE?patterns_W:patterns_B);
	pattern(&patternx, &patterny, &upattern, Evaluate_Value,patt);
	
//ģ��ƥ��ɹ�	
if (upattern>1)
	{		x= patternx;
			y= patterny;
			goto movexy;
	}
//û��ƥ������棬����judgement���������һ��
	for (x=0; x<edge; ++x)
		for (y=0; y<edge; ++y)
				
			if(EMPTY==theboard[x][y])
				if (judgement(&u,x,y,*player))
				{
					goto movexy;
				}
//�����Ϸ�����û���ҵ���ѵ㣬ֻ�������һ��EMPTY��
		for (x=0; x<edge; ++x)
			for (y=0; y<edge; ++y)
				
				 if(EMPTY==theboard[x][y])
				{
					goto movexy;
				}
	

	return COMPUTER_PASS;//�����������

movexy:
	//SaveScratch(Evaluate_Value,"scratch_patterns.txt");
	mx=x;
	my=y;
	if(0==placestone(mx, my, ComputerColor))//���Ӳ��ж���Ӯ
		return COMPUTER_LOS;
	else
	{
		movedone();//��������
		return 0;
	}
}


int Function::enemymove() //���ֵ��߲����������ˣ�
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




//�������������ǽ����ݴ洢�������ļ��С�
void Function::SetBoard(board scratch,int value)
{
	register int *ip;
	register int j;
	ip = (int*)scratch;
	for (j=EDGE*EDGE;j>0;j--)//��scratch����ֵ10000
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

void Function::SaveScratch(int (*scratch)[EDGE],char *name)//Ϊ����д��
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
