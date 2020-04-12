#include "stdafx.h"
#include "Game.h"
#include "Searchengine.h"
#include "Negascout_HH_TT.h"

Game::Game()
{
	
}

Game::~Game()
{

}

MOVE Game::Search_Good_Move(int board[][10],int Color,int Engine,int Step)
{
	int Move_Num;
	MOVE All_Moves_Temp[200];
	MOVE The_Good_Move;

	Move_Num=Computer.allMove(board,All_Moves_Temp, Color);
	
	if(Engine&Negascout&&Engine&AlphaBeta)
	{	
		The_Good_Move=Computer.SearchAGoodMove( board, Color,Step);
	}
	else if(Engine&AlphaBeta)
	{	
		The_Good_Move=Computer.SearchAGoodMove_2( board, Color);
	}
	//检验走法正确性
	int Move_Fault_Times=0;
	while(Check_Move(The_Good_Move, All_Moves_Temp,Move_Num))
	{
		Error=Error|(Engine&Err_Engine_N?Err_Engine_N:Err_Engine_AB);
		Move_Fault_Times++;
		if(Move_Fault_Times>3)//着法错误3次跳出
		{
			Error=Error|Err_Engine_Check;
			The_Good_Move=All_Moves_Temp[0];
			break;
		}
		The_Good_Move=Computer.SearchAGoodMove_2( board, Color);
	}
	return  The_Good_Move;
}

int  Game::Search_All_Moves(int board[][10],MOVE *All_Moves,int Color)
{
	return Computer.allMove(board,All_Moves, Color);
}

void Game::Go_Move(int board[][10],MOVE The_Move)
{
	Computer.goAMove(board,The_Move);
}

CString Game::Get_Evel(int board[][10],int Color)
{
	//CString Evel_Mess;
	//Evel_Mess.Format("dfgh");
	return  Computer.MFC_Eval(board,Color);
}


bool Game::Check_Move(MOVE nowMove,MOVE AllMoves[],int move_num)
{

	for(int i=0;i<move_num;i++)
	{				
		if(AllMoves[i].eatHowMany==nowMove.eatHowMany
			&&AllMoves[i].move_star_x==nowMove.move_star_x
			&&AllMoves[i].move_star_y==nowMove.move_star_y
			&&AllMoves[i].move_stop_x==nowMove.move_stop_x
			&&AllMoves[i].move_stop_y==nowMove.move_stop_y)
		{
			for(int j=0;j<AllMoves[i].eatHowMany;j++)
			{
				if(AllMoves[i].eatPoint[j][0]==nowMove.eatPoint[j][0]&&AllMoves[i].eatPoint[j][1]==nowMove.eatPoint[j][1])
					return 0;
				else 
					return 1;
			}
			return 0;
		}

	}
	return 1;
}

void Game::Input_Depth_Time(int Depth,double Time)
{
	Computer.Input_Depth_Time(Depth,Time);
}