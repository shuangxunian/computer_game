

#pragma once
#include"head.h"
class CHistory_TT
{
public:
	CHistory_TT(void);
	~CHistory_TT(void);
	void InitializeHistoryTable();//生成历史表，并初始化
	double LookUpHHTable(MOVE *move);//返回值用来排序
	void EnterHHTable(MOVE *move,int depth);//存入历史表
	void qrs(MOVE *a,int depth, int left, int right);//快速排序
protected:
	LONGLONG Rand32();//产生64位的随机数//64->32
	LONG Rand16();	  //产生32位的随机数//32->16
private:
	UINT m_nHashKey16[4][10][10];	   //32位随机数组，用以生成32位哈希值//32->16
	ULONGLONG m_ulHashKey32[4][10][10];//64位随机数组，用以生成64位哈希值//64->32
	HH_HASHITEM *m_pHH;

};

