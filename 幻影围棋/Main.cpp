//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//幻影围棋组负责人赵浩宇，田景所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//赵浩宇	QQ：1366160976
//田景		QQ：282940270
#ifndef	_MAIN_CPP_
#define	_MAIN_CPP_

#include "Define.h"
#include<iostream>
char undo_board[GRID_NUM][GRID_NUM];
char undo_board_know[GRID_NUM][GRID_NUM];
SPlayer undo_player[3];	

int main()
{
	char msg[100];
	int	numTi = 0;
	int	x,y;

	InitGame();
	while(1)
	{
		Sleep(50);
		fflush(stdin);
		Sleep(50);
		fflush(stdout);
		printf("开局：'new' or 'quit'\n");
		scanf("%s",msg);  
		if (strcmp(msg,"quit") == 0)                                 //结束游戏
		{	
			break;
		}
		else if (strcmp(msg,"new") == 0)                	         //开始新棋局
		{
			InitGame();
			PrintBoard();
			printf("棋子：'black' or other\n");
			scanf("%s",msg);
			if (strcmp(msg,"black") == 0)                            //如果msg是black，自己是黑子
			{
				chessColor = BLACK;
				SearchMove(&bestMove);
			    printf("-->%d %c",bestMove.x,bestMove.y+'@');        //输出一个最好招法
				printf("   可走：'m' or 'i'\n");
				while(1)
				{ 	
			           scanf("%s",msg);                              //读取黑子信息
				       if (strcmp(msg,"m") == 0)                     //自己上次走的为合法招法
					   {	     
					     board[bestMove.x][bestMove.y] = BLACK;
					     PrintBoard();
					     record();
				       	 player[BLACK].total++;
					     player[WHITE].total++;                      //  ////
                         printf("继续：'go' / 'take' / 'taked' / 'undo' / 'passed'\n");
					   }
					   else if (strcmp(msg,"go")==0)
					   {
						   if (SearchMove(&bestMove))
						   {
						      printf("-->%d %c",bestMove.x,bestMove.y+'@');     //产生招法成功，返回平台 
							  printf("   可走：'m' or 'i'\n");
						   }
						   else
						   {	   
						       printf("passed\n");                   //不成功，自己pass
						   }
					   }	   
				       else if (strcmp(msg,"i") == 0)                //自己上次走的为非法招法
					   {  
					      board[bestMove.x][bestMove.y] = WHITE;
					      PrintBoard();
					      player[WHITE].know++;
					      board_know[bestMove.x][bestMove.y] ++;     //  ???
					      if (SearchMove(&bestMove))
						  {
						     printf("-->%d %c",bestMove.x,bestMove.y+'@');      //产生招法成功，返回平台
							 printf("   可走：'m' or 'i'\n");
						  }
					      else
						  {
						      printf("passed\n");			         //不成功，自己pass
						  }
					   }
				       else if (strcmp(msg,"take") == 0)            //自己提子,读取提子数目
					   {
					      SMove moveList[81] ;
						  printf("提子数目：");
					      scanf("%d",&numTi);
					      player[WHITE].total -= numTi;
					      for(int k = 0 ; k < numTi ; k++)
						  {
                             printf("提子坐标：");
						     Sleep(50);
						     fflush(stdin);
						     scanf("%d%d",&x,&y);
						     moveList[k].x = x;
						     moveList[k].y = y;
						  }
					      for(int i = 0 ; i < numTi ; i++)
						  {
							  board[moveList[i].x][moveList[i].y] = NOSTONE;
						  }
					      PrintBoard();
                          printf("继续：'go' / 'passed' / 'taked'\n");
					   }
			           else if (strcmp(msg,"taked") == 0)            //对方提子,读取提子数目
					   {					      
					      SMove moveList[81] ;
						  printf("被提子数目：");
					      scanf("%d",&numTi);
					      player[BLACK].total -= numTi;					      
					      for(int k = 0 ; k < numTi ; k++)
						  {
                             printf("被提子坐标：");
						     Sleep(50);
						     fflush(stdin);
						     scanf("%d %d",&x,&y);
						     moveList[k].x = x;
						     moveList[k].y = y;     
						     if (board[x-1][y] == NOSTONE)           //对所提的子的边界进行探测，如果是空，肯定是对方的子
							 {
							     board[x-1][y] = WHITE;
							     player[WHITE].know++;
							 }
						     if (board[x+1][y] == NOSTONE)
							 {
							    board[x+1][y] = WHITE;
							    player[WHITE].know++;
							 }
						     if (board[x][y-1] == NOSTONE)
							 {
							    board[x][y-1] = WHITE;
							    player[WHITE].know++;
							 }
						     if (board[x][y+1] == NOSTONE)
							 {
							    board[x][y+1] = WHITE;
							    player[WHITE].know++;
							 } 
						  }
					      for(int i = 0 ; i< numTi ; i++)
						  {
						     board[moveList[i].x][moveList[i].y] = NOSTONE;
						  }
						  PrintBoard();
                          printf("继续：'go' / 'passed'\n");
					   }
			           else if (strcmp(msg,"passed") == 0)           //对方pass
					   {    
					     player[WHITE].know  = player[WHITE].total;
					   }
		               else if (strcmp(msg,"undo") == 0)
					   {
			               Undo();
                           printf("继续：'i' or 'm'\n");
					   }
		               else
					   {
			             printf("Error Input!!!\n");
			             PrintBoard();
					   }                       
                 } 
			}
			else
			{
				chessColor = WHITE;
				while(1)
				{
					scanf("%s",msg);
			        chessColor = WHITE;			                     //自己是白子		    
				    if (strcmp(msg,"m") == 0)					     //自己上次走的为合法招法
					{
					   board[bestMove.x][bestMove.y] = WHITE;
					   PrintBoard();
					   record();
					   player[WHITE].total++;
					   player[BLACK].total++;
					   printf("继续：'go' / 'take' / 'taked' / 'undo' / 'passed'\n");
					}
				    else if (strcmp(msg,"go")==0)
					{
					   if (SearchMove(&bestMove))
					   {
					      printf("%d %c",bestMove.x,bestMove.y+'@');          //产生招法成功，返回平台
						  printf("   可走：'m' or 'i'\n");
					   }
					   else                                          //不成功，自己pass
					   {
						   printf("passed\n");
					   }
					}
				    else if (strcmp(msg,"i") == 0)                   //自己上次走的为非法招法
					{	    
					    board[bestMove.x][bestMove.y] = BLACK;
                        PrintBoard();
					    player[BLACK].know++;
					    board_know[bestMove.x][bestMove.y] ++;
					    if (SearchMove(&bestMove))
						{
						    printf("%d %c",bestMove.x,bestMove.y+'@');          //产生招法成功，返回平台
							printf("   可走：'m' or 'i'\n");
						}
					    else                                         //不成功，自己pass
						{  
						    printf("passed\n");
						}
					}
				    else if (strcmp(msg,"take") == 0)
					{
					    SMove moveList[81] ;                         //自己提子,读取提子数目
						printf("提子数目：");
					    scanf("%d",&numTi);
					    player[BLACK].total -= numTi;
					    for(int k = 0 ; k < numTi ; k++)
						{
							printf("提子坐标：");
						    Sleep(50);
						    fflush(stdin);							
						    scanf("%d%d",&x,&y);
						    moveList[k].x = x;
						    moveList[k].y = y;
						}
					    for(int i = 0 ; i < numTi ; i++)
						{
						    board[moveList[i].x][moveList[i].y] = NOSTONE;
						}
					                                                 //自己知道的子也应该修正
						PrintBoard();
                        printf("继续：'go' or 'passed' or 'taked'\n");
					}
				    else if (strcmp(msg,"taked") == 0)               //对方提子,读取提子数目
					{					    
					    SMove moveList[81] ;
						printf("被提子数目：");
					    scanf("%d",&numTi);
					    player[WHITE].total -= numTi;                //读取自己所有被提的子	    
					    for(int k = 0 ; k < numTi ; k++)
						{
							printf("被提子坐标：");
						    Sleep(50);
						    fflush(stdin);
						    scanf("%d%d",&x,&y);
						    moveList[k].x = x;
						    moveList[k].y = y;			    
						    if (board[x-1][y] == NOSTONE)            //对所提的子的边界进行探测，如果是空，肯定是对方的子
							{
						     	board[x-1][y] = BLACK;
						    	player[BLACK].know++;
							}
						    if (board[x+1][y] == NOSTONE)
							{
						    	board[x+1][y] = BLACK;
						    	player[BLACK].know++;
							}
						    if (board[x][y-1] == NOSTONE)
							{
						    	board[x][y-1] = BLACK;
						    	player[BLACK].know++;
							}
						    if (board[x][y+1] == NOSTONE)
							{
							    board[x][y+1] = BLACK;
						    	player[BLACK].know++;
							}
						}
					    for(int i = 0 ; i < numTi ; i++)
						{
						    board[moveList[i].x][moveList[i].y] = NOSTONE;
						}
						PrintBoard();
                        printf("继续：'go' or 'passed'\n");
					}
				    else if (strcmp(msg,"passed") == 0)              //对方pass
					{	    
					    player[WHITE].know  = player[BLACK].total;
					}
		            else if (strcmp(msg,"undo") == 0)
					{
			             Undo();
						 printf("继续：'i' or 'm'\n");
					}
		            else
					{
			             printf("Error Input!!!\n");
			             PrintBoard();
					}

				 }
			}
		}
	}
	return 0;
}

void record()
{
	long i, j;
	for (i=1; i<10; i++)
	{
		for (j=1; j<10; j++)
		{
			undo_board[i][j] = board[i][j];
			undo_board_know[i][j] = board_know[i][j];
		}
	}
	for (i=0; i<3; i++){
		undo_player[i] = player[i];
	}
}

void Undo()
{
	long i, j;
	for (i=1; i<10; i++)
	{
		for (j=1; j<10; j++)
		{
			board[i][j] = undo_board[i][j];
			board_know[i][j] = undo_board_know[i][j];
		}
	}
	for (i=0; i<3; i++)
		player[i] = undo_player[i];
}


void InitGame()
{
	memset(player, 0, sizeof(player));
	memset(board, 0, sizeof(board));
	memset(board_know, 0, sizeof(board_know));
	memset(&bestMove, 0, sizeof(bestMove));
	for (long i = 0 ; i < 11 ; i++)
	{
		board[i][0] = BORDER;
		board[0][i] = BORDER;
		board[i][10] = BORDER;
		board[10][i] = BORDER;
	}
}

void PrintBoard()
{
	printf("  ");
	char ch='A';
	for(long j = 1 ; j < 10 ; j++)
	{
		printf("  %c%d",ch,j);
		ch++;
	}
    printf("%s","\n  ┌─┬─┬─┬─┬─┬─┬─┬─┬─┐\n");
	for (long i=0;i<9;i++)
	{
		printf("%d %s",i+1,"│");
		for(long j=0;j<9;j++)
		{
			if(board[i+1][j+1]==1)
				printf("%s","╳│");
			else if(board[i+1][j+1]==2)
				printf("%s","○│");
			else
				printf("%s","  │");
		}
		printf("%s","\n");
		if(i==8)
			printf("%s","  └─┴─┴─┴─┴─┴─┴─┴─┴─┘\n");
		else
			printf("%s","  ├─┼─┼─┼─┼─┼─┼─┼─┼─┤\n");
	}
}


#endif
