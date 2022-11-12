#include "stdafx.h"
#include "TranspositionTable.h"


CTranspositionTable::CTranspositionTable(void)
{
	InitializeHashKey();
}

CTranspositionTable::~CTranspositionTable(void)
{
	delete [] m_pTT;
}

////生成置换表
void CTranspositionTable::InitializeHashKey()
{
	srand((unsigned int)time(NULL));  	  
    for (int i = 0; i < 4; i++)
	  for (int j = 0; j < 10; j++)
		  for (int k = 0; k < 10; k++)
		  {
			  m_nHashKey32[i][j][k] = Rand32();
			  m_ulHashKey64[i][j][k] = Rand64();
		  }
	m_pTT = new HASHITEM[2048*2048];
}

void CTranspositionTable::CalculateInitHashKey(int position[10][10])
{
	BYTE nStoneType;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	for (int i = 0; i < 10; i++)		//改
		for (int j = 0; j< 10; j++)		//
	//for (int j = 0; j < 10; j++)
	//  for (int i = 0; i< 10; i++)//原来的
	  {
		  nStoneType = position[i][j];
		  if (nStoneType != 0) 
		  {                        
			  m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nStoneType-1][i][j]; 
			  m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nStoneType-1][i][j]; 
		  }
	  }	  
}

void CTranspositionTable::Hash_MakeMove(MOVE *move, int CurPosition[][10])
{
	BYTE nFromID,nToID;
	nFromID = move->Kind;//改
	nToID = nFromID;//改
	//nToID=nFromID=move->Kind;//原来的
	if(move->move_stop_y==0&&move->Kind==WHITE)
		nToID=move->Kind+2;
	if(move->move_stop_y==9&&move->Kind==BLACK)
		nToID = move->Kind + 1;//改 
		//nToID=move->Kind+2;//原来的

	//要移动的棋子
	m_HashKey32=m_HashKey32^m_nHashKey32[nFromID-1][move->move_star_x][move->move_star_y];
	m_HashKey64=m_HashKey64^m_ulHashKey64[nFromID-1][move->move_star_x][move->move_star_y];

	if(move->eatHowMany!=0)//有棋子被吃
	{
		for(int k=0;k<move->eatHowMany;k++)
		{
			m_HashKey32=m_HashKey32^m_nHashKey32[move->eatKind[k]-1][move->eatPoint[k][0]][move->eatPoint[k][1]];
			m_HashKey64=m_HashKey64^m_ulHashKey64[move->eatKind[k]-1][move->move_star_x][move->move_star_y];
		}
	}
	
	//将棋子在目标位置上的随机数填入
	m_HashKey32=m_HashKey32^m_nHashKey32[nToID-1][move->move_stop_x][move->move_stop_y];
	m_HashKey64=m_HashKey64^m_ulHashKey64[nToID-1][move->move_stop_x][move->move_stop_y];
}

void CTranspositionTable::Hash_UnMakeMove(MOVE *move,int CurPosition[][10])
{
	BYTE nFromID,nToID;
	nFromID = move->Kind;//改
	nToID = nFromID;//改
				
	//nToID=nFromID=move->Kind;//原来的
	if(move->move_stop_y==0&&move->Kind==WHITE)
		nToID=move->Kind+2;
	if(move->move_stop_y==9&&move->Kind==BLACK)
		nToID = move->Kind + 1;//改
		//nToID=move->Kind+2;//原来的
	
	//将移动棋子在移动前位置上的随机数添入
	m_HashKey32=m_HashKey32^m_nHashKey32[nToID-1][move->move_stop_x][move->move_stop_y];
	m_HashKey64=m_HashKey64^m_ulHashKey64[nToID-1][move->move_stop_x][move->move_stop_y];

	if(move->eatHowMany!=0)//有棋子被吃，也要填入
	{
		for(int k=0;k< move->eatHowMany;k++)
		{
			//m_HashKey32=m_HashKey32^m_nHashKey32[move->eatKind[move->eatHowMany-k-1]-1][move->eatPoint[k][0]][move->eatPoint[k][1]];
			//m_HashKey64=m_HashKey64^m_ulHashKey64[move->eatKind[move->eatHowMany-k-1]-1][move->move_star_x][move->move_star_y];
			m_HashKey32 = m_HashKey32^m_nHashKey32[move->eatKind[k] - 1][move->eatPoint[k][0]][move->eatPoint[k][1]];
			m_HashKey64 = m_HashKey64^m_ulHashKey64[move->eatKind[k] - 1][move->move_star_x][move->move_star_y];
		}
	}
	//
	m_HashKey32=m_HashKey32^m_nHashKey32[nFromID-1][move->move_star_x][move->move_star_y];
	m_HashKey64=m_HashKey64^m_ulHashKey64[nFromID-1][move->move_star_x][move->move_star_y];
	
}

double CTranspositionTable::LookUpHashTable(double alpha, double beta, double depth)
{
	__int32 x;
	HASHITEM *pht;///////////////置换表指针
	x = m_HashKey32 & 0x3FFFFF;///////21位哈希地址
	pht = &m_pTT[x];
	if(pht->checksum != m_HashKey64 )
	{
		return 66666;///没有找到相同局面
	}		
//    hash_move=pht->hashmove;
	if(pht->depth >= depth)//深度优先覆盖
	{  
		switch (pht->enterType)
	  {
		case exact:
		  return pht->eval;
	  case lowerBound:
		  if (pht->eval >= beta)
			  return (pht->eval);
		  else 
			  break;
	  case UpperBound:
		  if (pht->eval <= alpha)
			  return (pht->eval);
		  else 
			  break;
	  }
	}
	return 66666;///没有找到相同局面
}

void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type,double eval,short depth)
{
	__int32 x;
	HASHITEM *pht;
	x=m_HashKey32 & 0x3FFFFF;////取二十一位二进制数
	pht = &m_pTT[x];
	if(pht->depth >= depth)
		return;
	pht->checksum = m_HashKey64;
	pht->enterType = entry_type;
	pht->eval = eval;
	pht->depth = depth;
//	pht->hashmove=mv;
}

//生成64位随机数
LONGLONG CTranspositionTable::Rand64()
{
    return rand()^((LONGLONG)rand()<<15)^((LONGLONG)rand()<<30)^
		((LONGLONG)rand()<<45)^((LONGLONG)rand()<<60);
}

//生成32位随机数
LONG CTranspositionTable::Rand32()
{
    return rand()^((LONG)rand()<<15)^((LONG)rand()<<30);
}