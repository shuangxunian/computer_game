#include "stdafx.h"
#include "CEvelation.h"

CEvelation::CEvelation()	
{
	
}
CEvelation::~CEvelation()
{
}
/////////////////////////////////新估值////////////////////////////////////////////

//用于储存兵位置附加值的常量矩阵
/*
const int White_Pawn_Pre_Value_A[10][10] =
{
	{ 0, 400, 0, 400, 0, 400, 0, 400, 0, 400 },
	{ 200, 0, 200, 0, 200, 0, 200, 0, 200, 0 },
	{ 0, 180, 0, 180, 0, 180, 0, 180, 0, 210 },//积极的边兵15赋值较高
	{ 160, 0, 160, 0, 160, 0, 160, 0, 160, 0 },
	{ 0, 150, 0, 150, 0, 150, 0, 150, 0, 150 },
	{ 120, 0, 140, 0, 140, 0, 140, 0, 140, 0 },//落后兵26、36、46、35、45  Value-20
	{ 0, 130, 0, 130, 0, 130, 0, 130, 0, 110 },
	{ 100, 0, 120, 0, 120, 0, 120, 0, 120, 0 },
	{ 0, 110, 0, 110, 0, 110, 0, 110, 0, 90 },
	{ 80, 0, 100, 0, 100, 0, 100, 0, 100, 0 },
}; 
const int Black_Pawn_Pre_Value_A[10][10] =
{
	{ 0, 100, 0, 100, 0, 100, 0, 100, 0, 80 },
	{ 90, 0, 110, 0, 110, 0, 110, 0, 110, 0 },//落后兵5、15、25、6、16   Value-20
	{ 0, 120, 0, 120, 0, 120, 0, 120, 0, 100 },
	{ 110, 0, 130, 0, 130, 0, 130, 0, 130, 0 },
	{ 0, 140, 0, 140, 0, 140, 0, 140, 0, 120 },
	{ 150, 0, 150, 0, 150, 0, 150, 0, 150, 0 },
	{ 0, 160, 0, 160, 0, 160, 0, 160, 0, 160 },
	{ 210, 0, 180, 0, 180, 0, 180, 0, 180, 0 },//积极的边兵36赋值较高
	{ 0, 190, 0, 190, 0, 190, 0, 190, 0, 190 },
	{ 400, 0, 400, 0, 400, 0, 400, 0, 400, 0 },
};
*/
const int White_Pawn_Pre_Value_B[10][10] =
{
	//1   2   3   4   5   6    7   8   9   10
	{ 0, 400, 0, 400, 0, 400, 0, 400, 0, 400 },//1
	{ 200, 0, 200, 0, 200, 0, 200, 0, 200, 0 },//2
	{ 0, 180, 0, 190, 0, 190, 0, 190, 0, 210 },//3   //积极的边兵15赋值较高
	{ 160, 0, 180, 0, 180, 0, 180, 0, 160, 0 },//4
	{ 0, 150, 0, 175, 0, 175, 0, 175, 0, 150 },//5
	{ 120, 0, 170, 0, 170, 0, 170, 0, 140, 0 },//6      //落后兵26、36、46、35、45  Value-20
	{ 0, 130, 0, 160, 0, 160, 0, 130, 0, 110 },//7
	{ 100, 0, 140, 0, 140, 0, 140, 0, 120, 0 },//8
	{ 0, 110, 0, 110, 0, 110, 0, 110, 0, 90 }, //9
	{ 70, 0, 100, 0, 100, 0, 100, 0, 100, 0 }, //10
};
const int Black_Pawn_Pre_Value_B[10][10] =
{
	// 1   2   3   4   5   6    7   8   9   10
	{ 0, 100, 0, 100, 0, 100, 0, 100, 0, 70 }, //1
	{ 90, 0, 110, 0, 110, 0, 110, 0, 110, 0 }, //2   //落后兵5、15、25、6、16   Value-20
	{ 0, 120, 0, 140, 0, 140, 0, 140, 0, 100 },//3
	{ 110, 0, 160, 0, 160, 0, 160, 0, 130, 0 },//4
	{ 0, 140, 0, 170, 0, 170, 0, 170, 0, 120 },//5
	{ 150, 0, 175, 0, 175, 0, 175, 0, 150, 0 },//6
	{ 0, 160, 0, 180, 0, 180, 0, 180, 0, 160 },//7
	{ 210, 0, 190, 0, 190, 0, 190, 0, 180, 0 },//8       //积极的边兵36赋值较高
	{ 0, 200, 0, 200, 0, 200, 0, 200, 0, 200 },//9
	{ 400, 0, 400, 0, 400, 0, 400, 0, 400, 0 },//10
};

//计算当前局面上每个兵的附加值
int CEvelation::Get_Pawn_Pre_Value(int x, int y, int position[10][10])
{
	/*int Count_W=0;
	int Count_B=0;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(position[i][j]==WHITE||position[i][j]==W_KING)
			{
					Count_W++;
			}	
			if(position[i][j]==BLACK||position[i][j]==B_KING)
			{
					Count_B++;
			}
		}
	}
	if(Count_W>10||Count_B>10)
	{
		if (position[x][y] == WHITE)
			return White_Pawn_Pre_Value_A[x][y];
		if (position[x][y] == BLACK)
			return Black_Pawn_Pre_Value_A[x][y];
	}
	else
	{
	*/
		if (position[x][y] == WHITE)
			return White_Pawn_Pre_Value_B[y][x];
		if (position[x][y] == BLACK)
			return Black_Pawn_Pre_Value_B[y][x];
	return 0;
}

//用于判断当前棋子是否处于被保护的状态
int CEvelation::Is_Guard_Pos(int x, int y, int position[10][10])
{
	switch (position[x][y])
	{
	case WHITE:
	{
		if (x == 1 || x == 8)
		{
			if ((x == 1) && (position[x - 1][y + 1] == WHITE) && (position[x + 1][y + 1] == WHITE) && 
				(position[x + 2][y + 2] == WHITE))//边界位置形成双兵兵链形成防御
				return 1;
			if ((x == 8) && (position[x - 1][y + 1] == WHITE) && (position[x - 2][y + 2] == WHITE) &&
				(position[x + 1][y + 1] == WHITE))
				return 1;
		}
		else
			if (((position[x - 1][y + 1] == WHITE) && (position[x - 2][y + 2] == WHITE)) &&
				((position[x + 1][y + 1] == WHITE) && (position[x + 2][y + 2] == WHITE)))//中间位置形成三兵兵链构成防御
				return 1;
		return 0;
	}
	case BLACK:
	{
		if (x == 1 || x == 8)
		{
			if ((x == 1) && (position[x - 1][y - 1] == BLACK) && (position[x + 1][y - 1] == BLACK) && 
				(position[x + 2][y - 2] == BLACK))//边界位置形成双兵兵链形成防御
				return 1;
			if ((x == 8) && (position[x - 1][y - 1] == BLACK) && (position[x - 2][y - 2] == BLACK) &&
				(position[x + 1][y - 1] == BLACK))
				return 1;
		}
		else
			if (((position[x - 1][y - 1] == BLACK) && (position[x - 2][y - 2] == BLACK)) &&
				((position[x + 1][y - 1] == BLACK) && (position[x + 2][y - 2] == BLACK)))//中间位置形成三兵兵链构成防御
				return 1;
		return 0;
	}
	case W_KING:
	{
		if (x == 1 || x == 8)//边界情况
		{
			if ((x == 1) && (position[x - 1][y + 1] == W_KING) && (position[x + 1][y + 1] == WHITE) && 
				(position[x + 2][y + 2] == WHITE))
				return 1;//左下为王、右下为双兵
			if ((x == 8) && (position[x - 1][y + 1] == WHITE) && (position[x - 2][y + 2] == WHITE) && 
				(position[x + 1][y + 1] == W_KING))
				return 1;//右下为王、左下为双兵
		}
		else
			if (((position[x - 1][y + 1] == W_KING) && (position[x - 2][y + 2] == WHITE)) &&
				((position[x + 1][y + 1] == W_KING) && (position[x + 2][y + 2] == WHITE)))//左下、右下均为王兵组合兵链
				return 1;
		return 0;
	}
	case B_KING:
	{
		if (x == 1 || x == 8)
		{
			if ((x == 1) && (position[x - 1][y - 1] == B_KING) && (position[x + 1][y + 1] == BLACK) &&
				(position[x + 2][y + 2] == BLACK))
				return 1;//左上为王、右上为双兵
			if ((x == 8) && (position[x + 1][y - 1] == B_KING) && (position[x - 1][y - 1] == BLACK) &&
				(position[x - 2][y - 2] == BLACK))
				return 1;//右上为王、左上为双兵
		}
		else
			if (((position[x - 1][y - 1] == B_KING) && (position[x - 2][y - 2] == WHITE)) &&
				((position[x + 1][y - 1] == B_KING) && (position[x + 2][y - 2] == WHITE)))//左上、右上为王兵组合

				return 1;
		return 0;
	}
	}
	return 0;
}

//判断当前棋子是否被对方棋子威胁
int CEvelation::Is_Attack_Pos(int x, int y, int position[10][10], int Type)
{
	int i = 2;
	int j = 2;
	if (Is_Guard_Pos(x, y, position))
		return 0;
	else
	{
		switch (position[x][y])
		{
		case WHITE:
		{
			if ((position[x - 1][y - 1] == BLACK|| (position[x - 1][y - 1] == B_KING)) &&
				(position[x + 1][y + 1] == EMPTY) && Type == 1)//右上被兵威胁
				return 1;
			if ((position[x - 1][y + 1] == BLACK || (position[x - 1][y + 1] == B_KING)) && (position[x + 1][y - 1] == EMPTY)
				&& Type == 1)//左上被兵威胁
				return 1;
			if ((position[x + 1][y - 1] == BLACK || (position[x + 1][y - 1] == B_KING)) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//左下被兵威胁
				return 1;
			if ((position[x + 1][y + 1] == BLACK || (position[x + 1][y + 1] == B_KING)) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下被兵威胁
				return 1;
			/*while((x - i >= 0) && (y - j >= 0))*/
			if((position[x - i][y - j] == B_KING) && position[x - 1][y - 1] == 0 &&(position[x + 1][y + 1] == EMPTY) && 
				Type == 1)//被左上相邻的王威胁
				return 1;
			else
				if(position[x - 1][y - 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y - j >= 0))//遍历左上方的相关位置
					{
						if ((position[x - i][y - j] == B_KING) && (position[x + 1][y + 1] == EMPTY) && Type == 1)//左上被王威胁
							return 1;
						else
							if(position[x - i][y - j] == BLACK)//如果左上方通道存在对方的兵
								break;
							else//继续沿该方向遍历其余位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x - 1][y + 1] == B_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 1)//被左下方相邻王威胁
				return 1;
			else
				if(position[x - 1][y + 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y + j <= 9))//遍历该方向上相关位置
					{
						if ((position[x - i][y + j] == B_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 1)//左下被王威胁
							return 1;
						else
							if(position[x - i][y + j] == BLACK)//当左下方存在对方兵
								break;
							else//继续遍历该方向上其余相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y - 1] == B_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//右上方被相邻位置王威胁
				return 1;
			else
				if(position[x + 1][y - 1] == EMPTY)//右上方相邻位置为空格
				{
					while ((x + i <= 9) && (y - j >= 0))//遍历右上方所有相关位置
					{
						if ((position[x + i][y - j] == B_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//右上被王威胁
							return 1;
						else
							if(position[x + i][y - j] == BLACK)//当右上方不相邻位置出现对方兵时
								break;
							else//继续遍历右上方相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y + 1] == B_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下方被相邻王威胁
				return 1;
			else
				if(position[x + 1][y + 1] == EMPTY)//右下方相邻位置是空格
				{
					while ((x + i <= 9) && (y + j <= 9))//遍历空格后该方向上所有相邻位置
					{
						if ((position[x + i][y + j] == B_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下被王威胁
							return 1;
						else
							if(position[x + i][y + j] == BLACK)//在该方向上存在不相邻的对方兵
								break;
							else//继续遍历该方向相关位置
							{
								i++;
								j++;
							}
					}
				}
			return 0;
		}
		case BLACK:
		{
			if ((position[x - 1][y - 1] == WHITE||(position[x - 1][y - 1] == W_KING)) && (position[x + 1][y + 1] == EMPTY) && Type == 2)//右上被兵威胁
				return 1;
			if ((position[x - 1][y + 1] == WHITE || (position[x - 1][y + 1] == W_KING)) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//左上被兵威胁
				return 1;
			if ((position[x + 1][y - 1] == WHITE || (position[x + 1][y - 1] == W_KING)) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//左下被兵威胁
				return 1;
			if ((position[x + 1][y + 1] == WHITE || (position[x + 1][y + 1] == W_KING)) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下被兵威胁
				return 1;
			if((position[x - i][y - j] == W_KING) && position[x - 1][y - 1] == 0 &&(position[x + 1][y + 1] == EMPTY) && Type == 2)//被左上相邻的王威胁
				return 1;
			else
				if(position[x - 1][y - 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y - j >= 0))//遍历左上方的相关位置
					{
						if ((position[x - i][y - j] == W_KING) && (position[x + 1][y + 1] == EMPTY) && Type == 2)//左上被王威胁
							return 1;
						else
							if(position[x - i][y - j] == WHITE)//如果左上方通道存在对方的兵
								break;
							else//继续沿该方向遍历其余位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x - 1][y + 1] == W_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//被左下方相邻王威胁
				return 1;
			else
				if(position[x - 1][y + 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y + j <= 9))//遍历该方向上相关位置
					{
						if ((position[x - i][y + j] == W_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//左下被王威胁
							return 1;
						else
							if(position[x - i][y + j] == WHITE)//当左下方存在对方兵
								break;
							else//继续遍历该方向上其余相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y - 1] == W_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//右上方被相邻位置王威胁
				return 1;
			else
				if(position[x + 1][y - 1] == EMPTY)//右上方相邻位置为空格
				{
					while ((x + i <= 9) && (y - j >= 0))//遍历右上方所有相关位置
					{
						if ((position[x + i][y - j] == W_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//右上被王威胁
							return 1;
						else
							if(position[x + i][y - j] == WHITE)//当右上方不相邻位置出现对方兵时
								break;
							else//继续遍历右上方相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y + 1] == W_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下方被相邻王威胁
				return 1;
			else
				if(position[x + 1][y + 1] == EMPTY)//右下方相邻位置是空格
				{
					while ((x + i <= 9) && (y + j <= 9))//遍历空格后该方向上所有相邻位置
					{
						if ((position[x + i][y + j] == W_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下被王威胁
							return 1;
						else
							if(position[x + i][y + j] == WHITE)//在该方向上存在不相邻的对方兵
								break;
							else//继续遍历该方向相关位置
							{
								i++;
								j++;
							}
					}
				}
			return 0;
		}
		case W_KING:
		{
			if ((position[x - 1][y - 1] == BLACK||(position[x - 1][y - 1] == B_KING)) && (position[x + 1][y + 1] == EMPTY) && Type == 1)//右上被兵威胁
				return 1;
			if ((position[x - 1][y + 1] == BLACK || (position[x - 1][y + 1] == B_KING)) && (position[x + 1][y - 1] == EMPTY) && Type == 1)//左上被兵威胁
				return 1;
			if ((position[x + 1][y - 1] == BLACK || (position[x + 1][y - 1] == B_KING)) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//左下被兵威胁
				return 1;
			if ((position[x + 1][y + 1] == BLACK || (position[x + 1][y + 1] == B_KING)) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下被兵威胁
				return 1;
			if((position[x - i][y - j] == B_KING) && position[x - 1][y - 1] == EMPTY &&(position[x + 1][y + 1] == EMPTY) && Type == 1)//被左上相邻的王威胁
				return 1;
			else
				if(position[x - 1][y - 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y - j >= 0))//遍历左上方的相关位置
					{
						if ((position[x - i][y - j] == B_KING) && (position[x + 1][y + 1] == EMPTY) && Type == 1)//左上被王威胁
							return 1;
						else
							if(position[x - i][y - j] == BLACK)//如果左上方通道存在对方的兵
								break;
							else//继续沿该方向遍历其余位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x - 1][y + 1] == B_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 1)//被左下方相邻王威胁
				return 1;
			else
				if(position[x - 1][y + 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y + j <= 9))//遍历该方向上相关位置
					{
						if ((position[x - i][y + j] == B_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 1)//左下被王威胁
							return 1;
						else
							if(position[x - i][y + j] == BLACK)//当左下方存在对方兵
								break;
							else//继续遍历该方向上其余相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y - 1] == B_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//右上方被相邻位置王威胁
				return 1;
			else
				if(position[x + 1][y - 1] == EMPTY)//右上方相邻位置为空格
				{
					while ((x + i <= 9) && (y - j >= 0))//遍历右上方所有相关位置
					{
						if ((position[x + i][y - j] == B_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 1)//右上被王威胁
							return 1;
						else
							if(position[x + i][y - j] == BLACK)//当右上方不相邻位置出现对方兵时
								break;
							else//继续遍历右上方相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y + 1] == B_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下方被相邻王威胁
				return 1;
			else
				if(position[x + 1][y + 1] == EMPTY)//右下方相邻位置是空格
				{
					while ((x + i <= 9) && (y + j <= 9))//遍历空格后该方向上所有相邻位置
					{
						if ((position[x + i][y + j] == B_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 1)//右下被王威胁
							return 1;
						else
							if(position[x + i][y + j] == BLACK)//在该方向上存在不相邻的对方兵
								break;
							else//继续遍历该方向相关位置
							{
								i++;
								j++;
							}
					}
				}
			return 0;
		}
		case B_KING:
		{
			if ((position[x - 1][y - 1] == WHITE||(position[x - 1][y - 1] == W_KING)) && (position[x + 1][y + 1] == EMPTY) && Type == 2)//右上被兵威胁
				return 1;
			if ((position[x - 1][y + 1] == WHITE || (position[x - 1][y + 1] == W_KING)) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//左上被兵威胁
				return 1;
			if ((position[x + 1][y - 1] == WHITE || (position[x + 1][y - 1] == W_KING)) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//左下被兵威胁
				return 1;
			if ((position[x + 1][y + 1] == WHITE || (position[x + 1][y + 1] == W_KING)) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下被兵威胁
				return 1;
			if((position[x - i][y - j] == W_KING) && (position[x - 1][y - 1] == EMPTY )&& (position[x + 1][y + 1] == EMPTY) && Type == 2)//被左上相邻的王威胁
				return 1;
			else
				if(position[x - 1][y - 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y - j >= 0))//遍历左上方的相关位置
					{
						if ((position[x - i][y - j] == W_KING) && (position[x + 1][y + 1] == EMPTY) && Type == 2)//左上被王威胁
							return 1;
						else
							if(position[x - i][y - j] == WHITE)//如果左上方通道存在对方的兵
								break;
							else//继续沿该方向遍历其余位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x - 1][y + 1] == W_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//被左下方相邻王威胁
				return 1;
			else
				if(position[x - 1][y + 1] == EMPTY)//如果相邻位置为空
				{
					while ((x - i >= 0) && (y + j <= 9))//遍历该方向上相关位置
					{
						if ((position[x - i][y + j] == W_KING) && (position[x + 1][y - 1] == EMPTY) && Type == 2)//左下被王威胁
							return 1;
						else
							if(position[x - i][y + j] == WHITE)//当左下方存在对方兵
								break;
							else//继续遍历该方向上其余相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y - 1] == W_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//右上方被相邻位置王威胁
				return 1;
			else
				if(position[x + 1][y - 1] == EMPTY)//右上方相邻位置为空格
				{
					while ((x + i <= 9) && (y - j >= 0))//遍历右上方所有相关位置
					{
						if ((position[x + i][y - j] == W_KING) && (position[x - 1][y + 1] == EMPTY) && Type == 2)//右上被王威胁
							return 1;
						else
							if(position[x + i][y - j] == WHITE)//当右上方不相邻位置出现对方兵时
								break;
							else//继续遍历右上方相关位置
							{
								i++;
								j++;
							}
					}
				}
				i=2;
				j=2;
			if((position[x + 1][y + 1] == W_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下方被相邻王威胁
				return 1;
			else
				if(position[x + 1][y + 1] == EMPTY)//右下方相邻位置是空格
				{
					while ((x + i <= 9) && (y + j <= 9))//遍历空格后该方向上所有相邻位置
					{
						if ((position[x + i][y + j] == W_KING) && (position[x - 1][y - 1] == EMPTY) && Type == 2)//右下被王威胁
							return 1;
						else
							if(position[x + i][y + j] == WHITE)//在该方向上存在不相邻的对方兵
								break;
							else//继续遍历该方向相关位置
							{
								i++;
								j++;
							}
					}
				}
			return 0;
		}
		default:
			return 0;
		}
	}
}

//计算当前棋子的灵活度
int CEvelation::Fiexiblity_Count(int x, int y, int position[10][10])
{
	int Fiexible_Num = 0;
	int i = 1;
	int j = 1;
	switch (position[x][y])
	{
	case WHITE://白兵
	{
		if (x>0&&position[x - 1][y - 1] == EMPTY)//左上灵活
			Fiexible_Num++;
		if (x<9&&position[x - 1][y + 1] == EMPTY)//右上灵活
			Fiexible_Num++;
	}
	break;
	case BLACK://黑兵
	{
		if (x>0&&position[x + 1][y - 1] == EMPTY)//左下灵活
			Fiexible_Num++;
		if (x<9&&position[x + 1][y + 1] == EMPTY)//右下灵活
			Fiexible_Num++;
	}
	break;
	case W_KING://白王
	{
		while ((x - i >= 0) && (y - j >= 0))
		{
			if ((position[x - i][y - j] == EMPTY))//左上灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x - i >= 0) && (y + j <= 9))
		{
			if ((position[x - i][y + j] == EMPTY))//左下灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x + i <= 9) && (y - j >= 0))
		{
			if ((position[x + i][y - j] == EMPTY))//右上灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x + i <= 9) && (y + j <= 9))
		{
			if ((position[x + i][y + j] == EMPTY))//右下灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
	}
	break;
	case B_KING://黑王
	{
		while ((x - i >= 0) && (y - j >= 0))
		{
			if ((position[x - i][y - j] == EMPTY))//左上灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x - i >= 0) && (y + j <= 9))
		{
			if ((position[x - i][y + j] == EMPTY))//左下灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x + i <= 9) && (y - j >= 0))
		{
			if ((position[x + i][y - j] == EMPTY))//右上灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
		i=1;
		j=1;
		while ((x + i <= 9) && (y + j <= 9))
		{
			if ((position[x + i][y + j] == EMPTY))//右下灵活
			{
				Fiexible_Num++;
				i++;
				j++;
			}
			else
				break;
		}
	}
	break;
	}
	return Fiexible_Num;
}

//计算当前棋子是否处于具有潜在称王位置
int CEvelation::Is_Being_King(int x,int y,int position[10][10],int Type)//逻辑未改
{
	switch (position[x][y])
	{
	case WHITE:
		{
			if(x==0&&y==3)
			{
			if(position[1][0]==EMPTY&&position[0][1]==EMPTY&&position[2][1]==EMPTY
				&&position[1][2]==EMPTY&&position[3][0]!=B_KING)
					return 1;
			}
			if(x==1&&y==2)
			{
				if(!Is_Attack_Pos(x,y,position,Type)&&position[1][0]==EMPTY)
					return 1;
			}
			if(x==8&&y==3)
			{
				if(!Is_Attack_Pos(x,y,position,Type)&&position[7][0]==EMPTY&&position[9][0]==EMPTY)
					return 1;
			}
			if(x==9&&y==2)
			{
				if(position[8][1]!=2&&position[7][0]==EMPTY&&position[9][0]==EMPTY)
					return 1;
			}
			return 0;
		}
	case BLACK:
		{
			if(x==9&&y==6)
			{
			if(position[8][7]==EMPTY&&position[7][8]==EMPTY&&position[9][8]==EMPTY
				&&position[8][9]==EMPTY&&position[6][9]!=W_KING)
					return 1;
			}
			if(x==8&&y==7)
			{
				if(!Is_Attack_Pos(x,y,position,Type)&&position[8][9]==EMPTY)
					return 1;
			}
			if(x==1&&y==6)
			{
				if(!Is_Attack_Pos(x,y,position,Type)&&position[0][9]==EMPTY&&position[2][9]==EMPTY)
					return 1;
			}
			if(x==0&&y==7)
			{
				if(position[1][8]!=1&&position[0][9]==EMPTY&&position[2][9]==EMPTY)
					return 1;
			}
			return 0;
		}
	default:
		break;
	}
	return 0;
}

double CEvelation::Eval(int position[10][10], int Type)
{
	return 0.0;
}

double CEvelation::Eval(int position[10][10], int Type,int Step)
{
	//设定子力的基本价值
	int WHITE_Basic_Value = 100;
	int BLACK_Basic_Value = 100;
	int W_KING_Basic_Value = 800;
	int B_KING_Basic_Value = 800;
	
	//设定黑、白双方的总价值的初始值
	double n_WHITE_Value = 0;
	double n_BLACK_Value = 0;
	//基本价值指标变量的初始化:
	double W_Pos_Value=0;//白方位置附加值
	double W_Fiexiblity_Value=0;//白方灵活度
	double W_Attack_Value=0;//白方威胁
	double W_Guard_Value=0;//白方保护
	double W_Being_King_Value=0;//白方潜在称王位置奖励
	double B_Pos_Value=0;//白方位置附加值
	double B_Fiexiblity_Value=0;//白方灵活度
	double B_Attack_Value=0;//白方威胁
	double B_Guard_Value=0;//白方保护
	double B_Being_King_Value=0;//白方潜在称王位置奖励
	//扫描棋盘对当前局面的棋子的价值进行计算
	for (int i = 0; i <= 9; i++)
		for (int j = 0; j <= 9; j++)
		{
		if (position[i][j] != EMPTY)//当前局面有子的情况下
		{
			switch (position[i][j])
			{
			case WHITE:
			{
				//一、将兵的“位置附加值”计入总价值
				W_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
				//二、将棋子的“灵活度”计入总价值
				W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*WHITE_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					W_Guard_Value += (0.5*WHITE_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					W_Attack_Value -= (0.7*WHITE_Basic_Value);
				//五、具备“称王潜在条件位置”的兵进行奖励
				if(Is_Being_King(i,j,position,Type))
					W_Being_King_Value +=100;
				break;
			}
			case BLACK:
			{
				//一、将兵的“位置附加值”计入总价值
				B_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
				//二、将棋子的“灵活度”计入总价值
				B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*BLACK_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					B_Guard_Value += (0.5*BLACK_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					B_Attack_Value -= (0.7*BLACK_Basic_Value);
				//五、具备“称王潜在条件位置”的兵进行奖励
				if(Is_Being_King(i,j,position,Type))
					B_Being_King_Value +=100;
				break;
			}
			case W_KING:
			{
				//一、将王的“位置附加值”计入总价值
				W_Pos_Value += W_KING_Basic_Value;
				//二、将棋子的“灵活度”计入总价值
				W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					W_Guard_Value += (0.5*W_KING_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					W_Attack_Value -= (0.7*W_KING_Basic_Value);
				break;
			}
			case B_KING:
			{
				//一、将王的“位置附加值”计入总价值
				B_Pos_Value += B_KING_Basic_Value;
				//二、将棋子的“灵活度”计入总价值
				B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					B_Guard_Value += (0.5*W_KING_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i,j,position,Type))
					B_Attack_Value -= (0.7*W_KING_Basic_Value);
				break;
			}
			}
		}
		}
		//初始化各指标的权重参数
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	int parameter[5];
	if (Step > 40)
	{
		 parameter[0] = 4;
		 parameter[1] = 1; 
		 parameter[2] = 6;
		 parameter[3] = 6;
		 parameter[4] = 0;
	}
	else
	{
		parameter[0] = 4;
		parameter[1] = 1;
		parameter[2] = 5;
		parameter[3] = 5;
		parameter[4] = 3;
	}
		//int parameter[5] = { 4,1,5,5,3 };
	//综合计算出黑、白双方的总价值
		//            第一个：       位置附加值  第二个：       灵活度         第三个：       威胁           第四个：       保护          第五个：       潜在称王奖励
		n_WHITE_Value=parameter[0]*W_Pos_Value+parameter[1]*W_Fiexiblity_Value+parameter[2]*W_Attack_Value+parameter[3]*W_Guard_Value+parameter[4]*W_Being_King_Value;
		n_BLACK_Value=parameter[0]*B_Pos_Value+parameter[1]*B_Fiexiblity_Value+parameter[2]*B_Attack_Value+parameter[3]*B_Guard_Value+parameter[4]*B_Being_King_Value;
	//设置估值核心的初始价值
		Tree++;
		if(Tree%1000==0)
			::PostMessage(AfxGetMainWnd()->m_hWnd,UPDATE_TREE,0,Tree);
	if (Type % 2 == 1)
		return n_BLACK_Value - n_WHITE_Value;
	else
		return n_WHITE_Value - n_BLACK_Value;
}
//double CEvelation::Eval(int position[10][10], int Type)//原始估值系统
//{
//	//设定子力的基本价值
//	int WHITE_Basic_Value = 100;
//	int BLACK_Basic_Value = 100;
//	int W_KING_Basic_Value = 800;
//	int B_KING_Basic_Value = 800;
//
//
//	//设定黑、白双方的总价值的初始值
//	double n_WHITE_Value = 0;
//	double n_BLACK_Value = 0;
//	//基本价值指标变量的初始化:
//	double W_Pos_Value = 0;//白方位置附加值
//	double W_Fiexiblity_Value = 0;//白方灵活度
//	double W_Attack_Value = 0;//白方威胁
//	double W_Guard_Value = 0;//白方保护
//	double W_Being_King_Value = 0;//白方潜在称王位置奖励
//	double B_Pos_Value = 0;//白方位置附加值
//	double B_Fiexiblity_Value = 0;//白方灵活度
//	double B_Attack_Value = 0;//白方威胁
//	double B_Guard_Value = 0;//白方保护
//	double B_Being_King_Value = 0;//白方潜在称王位置奖励
//								  //扫描棋盘对当前局面的棋子的价值进行计算
//	for (int i = 0; i <= 9; i++)
//		for (int j = 0; j <= 9; j++)
//		{
//			if (position[i][j] != EMPTY)//当前局面有子的情况下
//			{
//				switch (position[i][j])
//				{
//				case WHITE:
//				{
//					//一、将兵的“位置附加值”计入总价值
//					W_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
//					//二、将棋子的“灵活度”计入总价值
//					W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*WHITE_Basic_Value);
//					//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
//					if (Is_Guard_Pos(i, j, position))
//						W_Guard_Value += (0.5*WHITE_Basic_Value);
//					//四、如果被对方“威胁”：价值减去自身基本价值的一半
//					if (Is_Attack_Pos(i, j, position, Type))
//						W_Attack_Value -= (0.6*WHITE_Basic_Value);
//					//五、具备“称王潜在条件位置”的兵进行奖励
//					if (Is_Being_King(i, j, position, Type))
//						W_Being_King_Value += 100;
//					break;
//				}
//				case BLACK:
//				{
//					//一、将兵的“位置附加值”计入总价值
//					B_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
//					//二、将棋子的“灵活度”计入总价值
//					B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*BLACK_Basic_Value);
//					//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
//					if (Is_Guard_Pos(i, j, position))
//						B_Guard_Value += (0.5*BLACK_Basic_Value);
//					//四、如果被对方“威胁”：价值减去自身基本价值的一半
//					if (Is_Attack_Pos(i, j, position, Type))
//						B_Attack_Value -= (0.6*BLACK_Basic_Value);
//					//五、具备“称王潜在条件位置”的兵进行奖励
//					if (Is_Being_King(i, j, position, Type))
//						B_Being_King_Value += 100;
//					break;
//				}
//				case W_KING:
//				{
//					//一、将王的“位置附加值”计入总价值
//					W_Pos_Value += W_KING_Basic_Value;
//					//二、将棋子的“灵活度”计入总价值
//					W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
//					//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
//					if (Is_Guard_Pos(i, j, position))
//						W_Guard_Value += (0.5*W_KING_Basic_Value);
//					//四、如果被对方“威胁”：价值减去自身基本价值的一半
//					if (Is_Attack_Pos(i, j, position, Type))
//						W_Attack_Value -= (0.6*W_KING_Basic_Value);
//					break;
//				}
//				case B_KING:
//				{
//					//一、将王的“位置附加值”计入总价值
//					B_Pos_Value += B_KING_Basic_Value;
//					//二、将棋子的“灵活度”计入总价值
//					B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
//					//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
//					if (Is_Guard_Pos(i, j, position))
//						B_Guard_Value += (0.5*W_KING_Basic_Value);
//					//四、如果被对方“威胁”：价值减去自身基本价值的一半
//					if (Is_Attack_Pos(i, j, position, Type))
//						B_Attack_Value -= (0.6*W_KING_Basic_Value);
//					break;
//				}
//				}
//			}
//		}
//	//初始化各指标的权重参数
//	int parameter[5] = { 4,1,5,5,3 };
//	//综合计算出黑、白双方的总价值
//	//            第一个：       位置附加值  第二个：       灵活度         第三个：       威胁           第四个：       保护          第五个：       潜在称王奖励
//	n_WHITE_Value = parameter[0] * W_Pos_Value + parameter[1] * W_Fiexiblity_Value + parameter[2] * W_Attack_Value + parameter[3] * W_Guard_Value + parameter[4] * W_Being_King_Value;
//	n_BLACK_Value = parameter[0] * B_Pos_Value + parameter[1] * B_Fiexiblity_Value + parameter[2] * B_Attack_Value + parameter[3] * B_Guard_Value + parameter[4] * B_Being_King_Value;
//	//设置估值核心的初始价值
//	Tree++;
//	if (Tree % 1000 == 0)
//		::PostMessage(AfxGetMainWnd()->m_hWnd, UPDATE_TREE, 0, Tree);
//	if (Type % 2 == 1)
//		return n_BLACK_Value - n_WHITE_Value;
//	else
//		return n_WHITE_Value - n_BLACK_Value;
//}

CString CEvelation::MFC_Eval(int position[10][10], int Type)
{
	Tree=0;
	//设定子力的基本价值
	int WHITE_Basic_Value = 100;
	int BLACK_Basic_Value = 100;
	int W_KING_Basic_Value = 800;
	int B_KING_Basic_Value = 800;
	
	//设定黑、白双方的总价值的初始值
	double n_WHITE_Value = 0;
	double n_BLACK_Value = 0;
	//基本价值指标变量的初始化:
	double W_Pos_Value=0;//白方位置附加值
	double W_Fiexiblity_Value=0;//白方灵活度
	double W_Attack_Value=0;//白方威胁
	double W_Guard_Value=0;//白方保护
	double W_Being_King_Value=0;//白方潜在称王位置奖励
	double B_Pos_Value=0;//白方位置附加值
	double B_Fiexiblity_Value=0;//白方灵活度
	double B_Attack_Value=0;//白方威胁
	double B_Guard_Value=0;//白方保护
	double B_Being_King_Value=0;//白方潜在称王位置奖励
	//扫描棋盘对当前局面的棋子的价值进行计算
	for (int i = 0; i <= 9; i++)
		for (int j = 0; j <= 9; j++)
		{
		if (position[i][j] != EMPTY)//当前局面有子的情况下
		{
			switch (position[i][j])
			{
			case WHITE:
			{
				//一、将兵的“位置附加值”计入总价值
				W_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
				//二、将棋子的“灵活度”计入总价值
				W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*WHITE_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					W_Guard_Value += (0.5*WHITE_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					W_Attack_Value -= (0.6*WHITE_Basic_Value);
				//五、具备“称王潜在条件位置”的兵进行奖励
				if(Is_Being_King(i,j,position,Type))
					W_Being_King_Value +=100;
				break;
			}
			case BLACK:
			{
				//一、将兵的“位置附加值”计入总价值
				B_Pos_Value += Get_Pawn_Pre_Value(i, j, position);
				//二、将棋子的“灵活度”计入总价值
				B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*BLACK_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					B_Guard_Value += (0.5*BLACK_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					B_Attack_Value -= (0.6*BLACK_Basic_Value);
				//五、具备“称王潜在条件位置”的兵进行奖励
				if(Is_Being_King(i,j,position,Type))
					B_Being_King_Value +=100;
				break;
			}
			case W_KING:
			{
				//一、将王的“位置附加值”计入总价值
				W_Pos_Value += W_KING_Basic_Value;
				//二、将棋子的“灵活度”计入总价值
				W_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					W_Guard_Value += (0.5*W_KING_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i, j, position, Type))
					W_Attack_Value -= (0.6*W_KING_Basic_Value);
				break;
			}
			case B_KING:
			{
				//一、将王的“位置附加值”计入总价值
				B_Pos_Value += B_KING_Basic_Value;
				//二、将棋子的“灵活度”计入总价值
				B_Fiexiblity_Value += (Fiexiblity_Count(i, j, position)*0.1*W_KING_Basic_Value);
				//三、如果被自己方棋子“保护”：价值加上基本价值的0.1
				if (Is_Guard_Pos(i, j, position))
					B_Guard_Value += (0.5*W_KING_Basic_Value);
				//四、如果被对方“威胁”：价值减去自身基本价值的一半
				if (Is_Attack_Pos(i,j,position,Type))
					B_Attack_Value -= (0.6*W_KING_Basic_Value);
				break;
			}
			}
		}
		}
		//初始化各指标的权重参数
		float parameter[5]={4.0,1.0,5.0,5.0,3.0};
		n_WHITE_Value=parameter[0]*W_Pos_Value+parameter[1]*W_Fiexiblity_Value+parameter[2]*W_Attack_Value+parameter[3]*W_Guard_Value+parameter[4]*W_Being_King_Value;
		n_BLACK_Value=parameter[0]*B_Pos_Value+parameter[1]*B_Fiexiblity_Value+parameter[2]*B_Attack_Value+parameter[3]*B_Guard_Value+parameter[4]*B_Being_King_Value;
		CString Eval_Mess;
		if(W_Attack_Value!=0)W_Attack_Value=-W_Attack_Value;
		if(B_Attack_Value!=0)B_Attack_Value=-B_Attack_Value;
		Eval_Mess.Format(
			"白方总分:%05.f  位置:%05.f  灵活:%05.f  威胁:-%06.f  保护:%05.f  称王:%.0f\r\n黑方总分:%05.f  位置:%05.f  灵活:%05.f  威胁:-%05.f  保护:%05.f  称王:%.0f\r\n",
			n_WHITE_Value,
			parameter[0]*W_Pos_Value,
			parameter[1]*W_Fiexiblity_Value,
			parameter[2]*W_Attack_Value,
			parameter[3]*W_Guard_Value,
			parameter[4]*W_Being_King_Value,
			n_BLACK_Value,
			parameter[0]*B_Pos_Value,
			parameter[1]*B_Fiexiblity_Value,
			parameter[2]*B_Attack_Value,
			parameter[3]*B_Guard_Value,
			parameter[4]*B_Being_King_Value);
		return Eval_Mess;
}