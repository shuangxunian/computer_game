#pragma once
#include"head.h"
class CTranspositionTable
{
public:
	CTranspositionTable();
	~CTranspositionTable();
	void EnterHashTable(ENTRY_TYPE entry_type, double eval, short depth);
	//将当前节点的值存入哈希表
	double LookUpHashTable(double alpha, double beta, double depth);			//查询哈希表中当前节点数据
	void Hash_UnMakeMove(MOVE* move, int CurPosition[10][10]);
	//撤销所给走法的哈希值，还原成走过之前的
	void Hash_MakeMove(MOVE* move, int CurPosition[10][10]);		//根据所给走法，增量生成新的哈希值
	void CalculateInitHashKey(int CurPosition[10][10]);				//计算给定棋盘的哈希值
	void InitializeHashKey();										//初始化随机数组，创建哈希表
public:
	LONGLONG Rand64();												//产生64位的随机数
	LONG Rand32();													//产生32位的随机数
private:
	UINT m_nHashKey32[4][10][10];		 //32位随机数组，用以生成32位哈希值
	ULONGLONG m_ulHashKey64[4][10][10];	 //64位随机数组，用以生成64位哈希值
	HASHITEM *m_pTT;					 //置换表头指针
	UINT m_HashKey32;				     //32位哈希值
	LONGLONG m_HashKey64;			     //64位哈希值
};

