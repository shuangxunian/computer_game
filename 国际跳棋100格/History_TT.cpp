#include "stdafx.h"
#include "History_TT.h"


CHistory_TT::CHistory_TT(void)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
			{
				m_nHashKey16[i][j][k] = Rand16();//
				m_ulHashKey32[i][j][k] = Rand32();//
			}
	m_pHH = new HH_HASHITEM[2048*2048];//
}

CHistory_TT::~CHistory_TT(void)
{
	delete [] m_pHH;
}

void CHistory_TT::InitializeHistoryTable()
{
//	m_HashKey32=Rand32();
//	m_HashKey64=Rand64();
	for (int i = 0; i < 2048 * 2048; i++)//
		m_pHH[i].score = 0.0;
}

double CHistory_TT::LookUpHHTable(MOVE *move)
{
	BYTE nFromID,nToID;
	UINT m_HashKey16=0;			//	   //32位哈希值
	LONGLONG m_HashKey32=0;     //       //64位哈希值
	__int32 x;

	nToID=nFromID = move->Kind;
	if(move->move_stop_y==0&&move->Kind==WHITE)
		nToID=move->Kind+2;
	if(move->move_stop_y==9&&move->Kind==BLACK)
		nToID=move->Kind+2;

	//将要移动的棋子从中去除
	m_HashKey16=m_nHashKey16[nFromID-1][move->move_star_x][move->move_star_y];//////
	m_HashKey32=m_ulHashKey32[nFromID-1][move->move_star_x][move->move_star_y];////////

	m_HashKey16 = m_HashKey16^m_nHashKey16[nToID - 1][move->move_stop_x][move->move_stop_y];///////
	m_HashKey32 = m_HashKey32^m_nHashKey16[nToID - 1][move->move_stop_x][move->move_stop_y];//////
	//m_HashKey64 = m_HashKey64^m_nHashKey32[nToID - 1][move->move_stop_x][move->move_stop_y];//原

	if(move->eatHowMany!=0)//有棋子被吃，也要从中去除
	{
		for(int k=0;k<move->eatHowMany;k++)
		{
			m_HashKey16=m_HashKey16^m_nHashKey16[move->eatKind[k]-1][move->eatPoint[k][0]][move->eatPoint[k][1]];///////
			m_HashKey32 = m_HashKey32^m_ulHashKey32[move->eatKind[k] - 1][move->eatPoint[k][0]][move->eatPoint[k][1]];/////////
		}
	}
	x = m_HashKey16 & 0x3FFFFF;///////生成21位哈希地址/////////////
	if(m_HashKey32==m_pHH[x].checksum)//////////////////////
		return m_pHH[x].score;
	else 
		return 0;
}

void CHistory_TT::EnterHHTable(MOVE *move,int depth)
{
	BYTE nFromID,nToID;
	UINT m_HashKey16=0;		////		     //32位哈希值
	LONGLONG m_HashKey32=0; ////             //64位哈希值
	__int32 x;

	nToID=nFromID = move->Kind;
	if(move->move_stop_y==0&&move->Kind==WHITE)
		nToID=move->Kind+2;
	if(move->move_stop_y==9&&move->Kind==BLACK)
		nToID=move->Kind+2;

	//将要移动的棋子从中去除
	m_HashKey16=m_nHashKey16[nFromID-1][move->move_star_x][move->move_star_y];///////////
	m_HashKey32=m_ulHashKey32[nFromID-1][move->move_star_x][move->move_star_y];///////////

	m_HashKey16 = m_HashKey16^m_nHashKey16[nToID - 1][move->move_stop_x][move->move_stop_y];//////////
	m_HashKey32 = m_HashKey32^m_nHashKey16[nToID - 1][move->move_stop_x][move->move_stop_y];/////////

	if(move->eatHowMany!=0)//有棋子被吃，也要从中去除
	{
		for(int k=0;k<move->eatHowMany;k++)
		{
			m_HashKey16=m_HashKey16^m_nHashKey16[move->eatKind[k]-1][move->eatPoint[k][0]][move->eatPoint[k][1]];///////////
			m_HashKey32 = m_HashKey32^m_ulHashKey32[move->eatKind[k] - 1][move->eatPoint[k][0]][move->eatPoint[k][1]];//////
		}
	}
	x = m_HashKey16 & 0x3FFFFF;///////生成21位哈希地址/////
	m_pHH[x].score+=2<<depth;
	if(m_pHH[x].checksum==m_HashKey32)////////////////
		m_pHH[x].score+=2<<depth;
	else
	{
		m_pHH[x].checksum=m_HashKey32;//////////////
		m_pHH[x].score=2<<depth;
	}

}

void CHistory_TT::qrs(MOVE *a,int depth, int left, int right)
{
	register int i,j;
	  double middle;
	  MOVE temp;
	  i = left; 
	  j = right;
	  middle = a[(left+right)/2].score; 
	  
	  do{ 
		  while((a[i].score > middle) && (i<right))//从左扫描大于中值的数 
			  i++;      
		  while((a[j].score <middle) && (j>left))//从右扫描大于中值的数 
			  j--; 
		  if(i<=j)//找到了一对值 
		  { 
			  //交换 
			  temp =a[i]; 
			  a[i]= a[j]; 
			  a[j] = temp; 
			  i++; 
			  j--; 
		  } 
	  }while(i<=j); 
	  
	  if(left<j) 
		  qrs(a,depth,left,j); 
	  if(right>i) 
		  qrs(a,depth,i,right); 
}

//生成64位随机数
LONGLONG CHistory_TT::Rand32()//64->32
{
    return rand()^((LONGLONG)rand()<<15)^((LONGLONG)rand()<<30)^
		((LONGLONG)rand()<<45)^((LONGLONG)rand()<<60);
}

//生成32位随机数
LONG CHistory_TT::Rand16()//32->16
{
    return rand()^((LONG)rand()<<15)^((LONG)rand()<<30);
}
