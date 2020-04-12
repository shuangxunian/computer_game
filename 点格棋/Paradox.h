#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"
#include "BoxBoard.h"

#pragma  once


#define MC_TIMES 10000
#define MCE_TIMES 100000

#define FILTER_RANGE 18


//Some Game Function
void MakeLaterSituation(Board &NewB,int StartPlayer,bool Show);
int GetBoardWinner(Board &CB, int LatterPlayer);

//Random Move
void RndTurn(Board &CB, int Player, bool Msg);
int RndFilterTurn(Board &CB, int Player, bool Msg, int Filter_Range);

//MC
int GetMCWinner(Board &CB,int NextPlayer);
float GetMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES);
void MonteCarloMove(Board &CB,int Player,bool Msg);

//FilterMC
int GetFilterMCWinner(Board &CB, int NextPlayer);
float GetFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range);
float GetFilterMCSingleEva(Board &CB, int NextPlayer, int Winner, int Filter_Range);
void FilterMCMove(Board &CB, int Player, bool Msg);

float GetRaveFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range, int RaveTable[LEN][LEN]);
