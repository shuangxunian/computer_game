#include "stdafx.h"
#include "makeNowMoves.h"
#include<cstdlib>
bool makeNowMoves::BING_FOUR_DIRECTION_CAN_EAT(int nowX,int nowY) 
{		
	if(((board[nowX+directions[0][0]][nowY+directions[0][1]]==3-nowColor||board[nowX+directions[0][0]]
		[nowY+directions[0][1]]==3-nowColor+2)&&board[nowX+2*directions[0][0]][nowY+2*directions[0][1]]
		==EMPTY&&nowX+2*directions[0][0]>=0&&nowX+2*directions[0][0]<10&&nowY+2*directions[0][1]>=0&&
		nowY+2*directions[0][1]<10)||
		((board[nowX+directions[1][0]][nowY+directions[1][1]]==3-nowColor||board[nowX+directions[1][0]]
		   [nowY+directions[1][1]]==3-nowColor+2)&&board[nowX+2*directions[1][0]][nowY+2*directions[1]
		   [1]]==EMPTY&&nowX+2*directions[1][0]>=0&&nowX+2*directions[1][0]<10&&nowY+2*directions[1][1]>=0&&
		   nowY+2*directions[1][1]<10)||
	   ((board[nowX+directions[2][0]][nowY+directions[2][1]]==3-nowColor||board[nowX+directions[2][0]]
		   [nowY+directions[2][1]]==3-nowColor+2)&&board[nowX+2*directions[2][0]][nowY+2*directions[2]
		   [1]]==EMPTY&&nowX+2*directions[2][0]>=0&&nowX+2*directions[2][0]<10&&nowY+2*directions[2][1]>=0&&
		   nowY+2*directions[2][1]<10)||
	   ((board[nowX+directions[3][0]][nowY+directions[3][1]]==3-nowColor||board[nowX+directions[3][0]]
		   [nowY+directions[3][1]]==3-nowColor+2)&&board[nowX+2*directions[3][0]][nowY+2*directions[3][1]]==EMPTY&&
		   nowX+2*directions[3][0]>=0&&nowX+2*directions[3][0]<10&&nowY+2*directions[3][1]>=0&&nowY+2*directions[3][1]<10))
		return true;
	else 
		return false;
}

makeNowMoves::makeNowMoves(void)//初始化棋盘
{
	int i,j;
	for(j=0;j<10;j++)
		for(i=0;i<10;i++)
			board[i][j]=EMPTY;
	for(j=0;j<5;j++)
		for(i=0;i<10;i++)
			if((i+j)%2==0)
				board[i][j]=WHITE;
	for(j=6;j<=9;j++)
		for(i=0;i<10;i++)
			if((i+j)%2==0)
				board[i][j]=BLACK;
}


makeNowMoves::~makeNowMoves(void)
{
}


void makeNowMoves::copy(int Color,int Board[][10])//获取当前的棋盘信息，和当前的走子方，每次生成招法
{													//都需要初始化theBestEatNum，nowTheNumberOfMove
	for(int j=0;j<10;j++)
		for(int i=0;i<10;i++)
		board[i][j]=Board[i][j];
	nowColor=Color;
	theBestEatNum=0;
	nowTheNumberOfMove=0;
}


int makeNowMoves::allMove(int board[][10],MOVE movesCoutainer[200],int Color)
{
	//Sleep(10);
	copy(Color,board);
	for(int j=0;j<10;j++)
		for(int i=0;i<10;i++)
		{
			int nowX=i;
			int nowY=j;
			
			
			//兵的走法生成
			if(board[i][j]==nowColor)//如果当前棋子是兵
			{
				nowTheMove.Kind=nowColor;
				if(BING_FOUR_DIRECTION_CAN_EAT(nowX,nowY))
				{
					nowTheMove.move_star_x=nowX;
					nowTheMove.move_star_y=nowY;
					makeNowBingMove(i,j,0,movesCoutainer);//生成当前兵能吃的招法
				}
				else//如果兵不能吃在看这个
				{
					if(theBestEatNum==0&&board[nowX+directions[nowColor-1][0]][nowY+directions[nowColor-1][1]]==EMPTY&&(nowX+directions[nowColor-1][0])<10&&(nowX+directions[nowColor-1][0])>=0&&(nowY+directions[nowColor-1][1])<10&&(nowY+directions[nowColor-1][1])>=0)//白棋的前方是指
					{																								  //向下，而黑棋的前方是向上
						nowTheMove.move_star_x=nowX;
						nowTheMove.move_star_y=nowY;
						nowTheMove.move_stop_x=nowX+directions[nowColor-1][0];
						nowTheMove.move_stop_y=nowY+directions[nowColor-1][1];
						nowTheMove.eatHowMany=0;

						//cout<<nowTheNumberOfMove<<endl;
						//if(nowTheNumberOfMove>=50)
						//{
						//	cout<<"溢出，A处"<<nowTheNumberOfMove<<endl;
						//	system("pause");
						//}


						movesCoutainer[nowTheNumberOfMove]=nowTheMove;
						nowTheNumberOfMove++;
					}
					if(theBestEatNum==0&&board[nowX+directions[nowColor+1][0]][nowY+directions[nowColor+1][1]]==EMPTY&&(nowX+directions[nowColor+1][0])<10&&(nowX+directions[3][0])>=0&&(nowY+directions[nowColor+1][1])<10&&(nowY+directions[nowColor+1][1])>=0)
					{
						nowTheMove.move_star_x=nowX;
						nowTheMove.move_star_y=nowY;
						nowTheMove.move_stop_x=nowX+directions[nowColor+1][0];
						nowTheMove.move_stop_y=nowY+directions[nowColor+1][1];
						nowTheMove.eatHowMany=0;

						////cout<<nowTheNumberOfMove<<endl;
						//if(nowTheNumberOfMove>=50)
						//{
						//	cout<<"溢出，B处"<<nowTheNumberOfMove<<endl;
						//	system("pause");
						//}

						movesCoutainer[nowTheNumberOfMove]=nowTheMove;
						nowTheNumberOfMove++;

					}
				}
			}
		//王棋的走法生成
		if(board[i][j]==nowColor+2)//如果当前棋子是王棋
				{
					nowTheMove.Kind=nowColor+2;
					nowTheMove.move_star_x=nowX;
					nowTheMove.move_star_y=nowY;
					if(KING_FOUR_DIRECTION_CAN_EAT(nowX,nowY))
					{
						makeNowKingMove(i,j,0,movesCoutainer);//生成当前王的招法
					}
					else
					{	
						int recordTheX=nowTheMove.move_star_x=nowX;
						int recordTheY=nowTheMove.move_star_y=nowY;
						int howLong=0;
						//先判断这个方向能不能走，if后面那么长就是这个意思						
						for(int k=0;k<4;k++)
						{
							if(theBestEatNum==0&&board[nowX+directions[k][0]][nowY+directions[k][1]]==EMPTY&&nowX+directions[k][0]>=0&&nowX+directions[k][0]<10&&nowY+directions[k][1]>=0&&nowY+directions[k][1]<10)
							{
								while(board[nowX+directions[k][0]][nowY+directions[k][1]]==EMPTY&&nowX+directions[k][0]>=0&&nowX+directions[k][0]<10&&nowY+directions[k][1]>=0&&nowY+directions[k][1]<10)
								{
									nowTheMove.move_stop_x=nowX+directions[k][0];
									nowTheMove.move_stop_y=nowY+directions[k][1];
									howLong++;
									nowX+=directions[k][0];
									nowY+=directions[k][1];
									nowTheMove.eatHowMany=0;

									//if(nowTheNumberOfMove>=50)
									//{
									//	cout<<"溢出，C处"<<nowTheNumberOfMove<<endl;
									//	system("pause");
									//}


									movesCoutainer[nowTheNumberOfMove]=nowTheMove;
									nowTheNumberOfMove++;//在这个位置加一，移动到下一个位置；
								}
								nowX=i;
								nowY=j;
							}
						}
					}
				}
			
			}
	//过滤招法的函数放在这里
	return nowTheNumberOfMove;
}


void makeNowMoves::makeNowBingMove(int nowX,int nowY,int eatHowMany,MOVE movesCoutainer[200])//这个其实是只生成了兵能吃的招法
{
//	eatHowMany=0;
//	nowTheMove.Kind=nowColor;//前面这个工作做过了，但总感觉如果这样做的话感觉这个函数的功能和名字不符合
//	nowTheMove.move_star_x=nowX;
//	nowTheMove.move_star_y=nowY;
	int flagX=nowX;
	int flagY=nowY;
	int recordX,recordY;
	for(int i=0;i<4;i++)
	{
		if((board[nowX+directions[i][0]][nowY+directions[i][1]]==3-nowColor||board[nowX+directions[i][0]]
			[nowY+directions[i][1]]==3-nowColor+2)&&board[nowX+2*directions[i][0]][nowY+2*directions[i][1]]==EMPTY&&
			nowX+2*directions[i][0]>=0&&nowX+2*directions[i][0]<10&&nowY+2*directions[i][1]>=0&&nowY+2*directions[i][1]<10)
		{
			int record=board[nowX+directions[i][0]][nowY+directions[i][1]];//这个位置的状态需要记录
			board[nowX+directions[i][0]][nowY+directions[i][1]]=BAR;//将记录过的状态更新为屏障，不能通过该处行棋，或搭桥
			nowTheMove.eatKind[eatHowMany]=record;
			recordX=nowTheMove.eatPoint[eatHowMany][0]=nowX+directions[i][0];
			recordY=nowTheMove.eatPoint[eatHowMany][1]=nowY+directions[i][1];
			nowX=nowX+2*directions[i][0];
			nowY=nowY+2*directions[i][1];
			nowTheMove.move_stop_x=nowX;
			nowTheMove.move_stop_y=nowY;
			nowTheMove.eatHowMany=eatHowMany+1;
			if(BING_FOUR_DIRECTION_CAN_EAT(nowX,nowY))
			{
				makeNowBingMove(nowX,nowY,eatHowMany+1,movesCoutainer);
			}
			else
			{
				if(eatHowMany+1>theBestEatNum)//eatHowMany实际上比真正的当前吃子数要少1这样做也许并不好理解，但是呢方便给结构体里的数组定位
				{
					nowTheNumberOfMove=0;//有这个感觉之前就不用初始化了，但是呢，为了保险把初始化的那个留着吧
					movesCoutainer[nowTheNumberOfMove]=nowTheMove;
					nowTheNumberOfMove++;//在这个位置加一，移动到下一个位置；
					theBestEatNum=eatHowMany+1;
				}
				else if(eatHowMany+1==theBestEatNum)
				{
					
					
					//if(nowTheNumberOfMove>=50)
					//{
					//	cout<<"溢出，D处"<<nowTheNumberOfMove<<endl;
					//	system("pause");
					//}


					movesCoutainer[nowTheNumberOfMove]=nowTheMove;
					nowTheNumberOfMove++;//在这个位置加一，移动到下一个位置；
				}
			}
			board[recordX][recordY]=record;//将变成屏障的地方还原为原来的状态
		}
		nowX=flagX;//将坐标还原为这个棋子的原始位置
		nowY=flagY;//
	}
}


bool makeNowMoves::KING_FOUR_DIRECTION_CAN_EAT(int nowX,int nowY)
{
	int howManyDirectionsCanEat=0;
	int recordX=nowX;
	int recordY=nowY;
	for(int i=0;i<4;i++)
	{
		while(board[nowX+directions[i][0]][nowY+directions[i][1]]==EMPTY&&nowX+directions[i][0]>=0&&nowX+directions[i][0]<10&&nowY+directions[i][1]>=0&&nowY+directions[i][1]<10)
		{
			nowX+=directions[i][0];
			nowY+=directions[i][1];
		}
		nowX+=directions[i][0];
		nowY+=directions[i][1];
		if((board[nowX][nowY]==3-nowColor||board[nowX][nowY]==3-nowColor+2)&&board[nowX+directions[i][0]][nowY+directions[i][1]]==EMPTY&&nowX+directions[i][0]>=0&&nowX+directions[i][0]<10&&nowY+directions[i][1]>=0&&nowY+directions[i][1]<10)
			howManyDirectionsCanEat++;
		nowX=recordX;
		nowY=recordY;
	}
	if(howManyDirectionsCanEat)
		return true;
	else 
		return false;
}


void makeNowMoves::makeNowKingMove(int nowX,int nowY,int eatHowMany,MOVE movesCoutainer[200])
{
	int flagX=nowX;
	int flagY=nowY;
	int recordX;
	int recordY;
	int howLong;
	int record;
	for(int i=0;i<4;i++)//4个方向逐个确认；
	{
		howLong=0;
		//进入这个函数的条件是4个方向至少一个方向能吃，所以此处不必要做多余的判断
		//这个while是判断在遇到障碍之前（跳吃完的棋子，边界，自己的棋子，对方的棋子）有几个空，记录当前长度给howLong
		while(board[nowX+directions[i][0]][nowY+directions[i][1]]==EMPTY&&nowX+directions[i][0]>=0&&nowX+directions[i][0]<10&&nowY+directions[i][1]>=0&&nowY+directions[i][1]<10)
		{
			nowX+=directions[i][0];
			nowY+=directions[i][1];
			howLong++;
		}
		//如果这个方向满足跳吃的条件
		if((board[nowX+directions[i][0]][nowY+directions[i][1]]==3-nowColor||board[nowX+directions[i][0]][nowY+directions[i][1]]==3-nowColor+2)&&board[nowX+2*directions[i][0]][nowY+2*directions[i][1]]==EMPTY&&nowX+2*directions[i][0]>=0&&nowX+2*directions[i][0]<10&&nowY+2*directions[i][1]>=0&&nowY+2*directions[i][1]<10)
		{
			//先将吃掉的地方记录，并且刷新成屏障BAR，招法结束的需要还原
			record=board[nowX+directions[i][0]][nowY+directions[i][1]];
			board[nowX+directions[i][0]][nowY+directions[i][1]]=BAR;
			nowTheMove.eatKind[eatHowMany]=record;
			nowTheMove.eatPoint[eatHowMany][0]=nowX+directions[i][0];
			nowTheMove.eatPoint[eatHowMany][1]=nowY+directions[i][1];
			nowX+=directions[i][0];
			nowY+=directions[i][1];
			recordX=nowX;
			recordY=nowY;
			howLong++;
			while(board[nowX+directions[i][0]][nowY+directions[i][1]]==EMPTY&&nowX+directions[i][0]>=0&&nowX+directions[i][0]<10&&nowY+directions[i][1]>=0&&nowY+directions[i][1]<10)
			{
				nowX+=directions[i][0];//刷新当前坐标
				nowY+=directions[i][1];
				howLong++;//记录该次这个位置跳吃长度，
				nowTheMove.move_stop_x=nowX;//记录当前坐标当做目前招法终点
				nowTheMove.move_stop_y=nowY;
				nowTheMove.eatHowMany=eatHowMany+1;//记录当前吃子数量；
				if(KING_FOUR_DIRECTION_CAN_EAT(nowX,nowY))
				{
					makeNowKingMove(nowX,nowY,eatHowMany+1,movesCoutainer);//这个位置如果能吃的话递归一下到下一个位置；
				}
				else
				{
					if(eatHowMany+1>theBestEatNum)//生成的吃子数量大于当今最大吃子数量
					{
						theBestEatNum=eatHowMany+1;
						nowTheNumberOfMove=0;//存入招法容器的首位；
						movesCoutainer[nowTheNumberOfMove]=nowTheMove;
						nowTheNumberOfMove++;//移动到下一位；
					}
					else if(eatHowMany+1==theBestEatNum)//生成了和当前一样的吃子数量
					{
						
						
/*						if(nowTheNumberOfMove>=50)
						{
							cout<<"溢出，E处"<<nowTheNumberOfMove<<endl;
							system("pause");
						}
						
					*/	
						movesCoutainer[nowTheNumberOfMove]=nowTheMove;
						nowTheNumberOfMove++;//移动到下一位；
					}
				}
				//board[recordX][recordY]=record;//还原障碍为原物
			}
			board[recordX][recordY]=record;//还原障碍为原物
		}
		nowX=flagX;//将坐标还原为这个棋子的原始位置
		nowY=flagY;
	}
}