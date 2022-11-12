#include"EvaluateEngine.h"
#include"Data.h"
#include<windows.h>
#include<math.h>
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

double t1 =0;
double t2 = 0;
double c1 = 0;
double c2 = 0;
double w = 0;
double s = 0;
int KingmoveBlack[12][12] = { 0 };
int KingmoveWhite[12][12] = { 0 };
int QueenmoveBlack[12][12] = { 0 };
int QueenmoveWhite[12][12] = { 0 };
int Mobility[12][12] = { 0 };
int aBlack[4] = { 0 };
int aWhite[4] = { 0 };

///////计算KingMove//////
void SearchKingMove()
{
	register int i, j; //定义寄存器变量，提高运算效率
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			KingmoveBlack[i][j] = 0;  //对模拟的KingMove棋盘初始化
			KingmoveWhite[i][j] = 0;
		}
	}
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == BLACKCHESS)//遍历棋盘找到黑棋，用KingmoveBlack棋盘存储KingMove走法
			{
				if (CurPosition[i][j - 1] == EMPTY)//正左方
				{
					KingmoveBlack[i][j - 1] = 1;
				}
				if (CurPosition[i - 1][j - 1] == EMPTY)//左上方
				{
					KingmoveBlack[i - 1][j - 1] = 1;
				}
				if (CurPosition[i - 1][j] == EMPTY)//正上方
				{
					KingmoveBlack[i - 1][j] = 1;
				}
				if (CurPosition[i - 1][j + 1] == EMPTY)//右上方
				{
					KingmoveBlack[i - 1][j + 1] = 1;
				}
				if (CurPosition[i][j + 1] == EMPTY)//正右方
				{
					KingmoveBlack[i][j + 1] = 1;
				}
				if (CurPosition[i + 1][j + 1] == EMPTY)//右下方
				{
					KingmoveBlack[i + 1][j + 1] = 1;
				}
				if (CurPosition[i + 1][j] == EMPTY)//正下方
				{
					KingmoveBlack[i + 1][j] = 1;
				}
				if (CurPosition[i + 1][j - 1] == EMPTY)//左下方
				{
					KingmoveBlack[i + 1][j - 1] = 1;
				}
			}
			if (CurPosition[i][j] == WHITECHESS)//遍历棋盘找到白棋，用KingmoveWhite棋盘存储KingMove走法
			{
				if (CurPosition[i][j - 1] == EMPTY)//正左方
				{
					KingmoveWhite[i][j - 1] = 1;
				}
				if (CurPosition[i - 1][j - 1] == EMPTY)//左上方
				{
					KingmoveWhite[i - 1][j - 1] = 1;
				}
				if (CurPosition[i - 1][j] == EMPTY)//正上方
				{
					KingmoveWhite[i - 1][j] = 1;
				}
				if (CurPosition[i - 1][j + 1] == EMPTY)//右上方
				{
					KingmoveWhite[i - 1][j + 1] = 1;
				}
				if (CurPosition[i][j + 1] == EMPTY)//正右方
				{
					KingmoveWhite[i][j + 1] = 1;
				}
				if (CurPosition[i + 1][j + 1] == EMPTY)//右下方
				{
					KingmoveWhite[i + 1][j + 1] = 1;
				}
				if (CurPosition[i + 1][j] == EMPTY)//正下方
				{
					KingmoveWhite[i + 1][j] = 1;
				}
				if (CurPosition[i + 1][j - 1] == EMPTY)//左下方
				{
					KingmoveWhite[i + 1][j - 1] = 1;
				}
			}
		}
	}
	for (register int n = 1; n < 11; n++)//棋子在棋盘上通过KingMove走法最多是10步
	{
		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < 12; j++)
			{
				if (KingmoveBlack[i][j] == n)//找到黑棋KingMove对应的步法数
				{
					if (CurPosition[i][j - 1] == EMPTY && KingmoveBlack[i][j - 1] == 0)
					{
						KingmoveBlack[i][j - 1] = n + 1;
					}
					if (CurPosition[i - 1][j - 1] == EMPTY&&KingmoveBlack[i - 1][j - 1] == 0)
					{
						KingmoveBlack[i - 1][j - 1] = n + 1;
					}
					if (CurPosition[i - 1][j] == EMPTY&&KingmoveBlack[i - 1][j] == 0)
					{
						KingmoveBlack[i - 1][j] = n + 1;
					}
					if (CurPosition[i - 1][j + 1] == EMPTY&&KingmoveBlack[i - 1][j + 1] == 0)
					{
						KingmoveBlack[i - 1][j + 1] = n + 1;
					}
					if (CurPosition[i][j + 1] == EMPTY&&KingmoveBlack[i][j + 1] == 0)
					{
						KingmoveBlack[i][j + 1] = n + 1;
					}
					if (CurPosition[i + 1][j + 1] == EMPTY&&KingmoveBlack[i + 1][j + 1] == 0)
					{
						KingmoveBlack[i + 1][j + 1] = n + 1;
					}
					if (CurPosition[i + 1][j] == EMPTY&&KingmoveBlack[i + 1][j] == 0)
					{
						KingmoveBlack[i + 1][j] = n + 1;
					}
					if (CurPosition[i + 1][j - 1] == EMPTY&&KingmoveBlack[i + 1][j - 1] == 0)
					{
						KingmoveBlack[i + 1][j - 1] = n + 1;
					}
				}
				if (KingmoveWhite[i][j] == n) //找到白棋KingMove对应的步法数
				{
					if (CurPosition[i][j - 1] == EMPTY && KingmoveWhite[i][j - 1] == 0)
					{
						KingmoveWhite[i][j - 1] = n + 1;
					}
					if (CurPosition[i - 1][j - 1] == EMPTY&&KingmoveWhite[i - 1][j - 1] == 0)
					{
						KingmoveWhite[i - 1][j - 1] = n + 1;
					}
					if (CurPosition[i - 1][j] == EMPTY&&KingmoveWhite[i - 1][j] == 0)
					{
						KingmoveWhite[i - 1][j] = n + 1;
					}
					if (CurPosition[i - 1][j + 1] == EMPTY&&KingmoveWhite[i - 1][j + 1] == 0)
					{
						KingmoveWhite[i - 1][j + 1] = n + 1;
					}
					if (CurPosition[i][j + 1] == EMPTY&&KingmoveWhite[i][j + 1] == 0)
					{
						KingmoveWhite[i][j + 1] = n + 1;
					}
					if (CurPosition[i + 1][j + 1] == EMPTY&&KingmoveWhite[i + 1][j + 1] == 0)
					{
						KingmoveWhite[i + 1][j + 1] = n + 1;
					}
					if (CurPosition[i + 1][j] == EMPTY&&KingmoveWhite[i + 1][j] == 0)
					{
						KingmoveWhite[i + 1][j] = n + 1;
					}
					if (CurPosition[i + 1][j - 1] == EMPTY&&KingmoveWhite[i + 1][j - 1] == 0)
					{
						KingmoveWhite[i + 1][j - 1] = n + 1;
					}
				}
			}
		}
	}
}

///////计算QueenMove/////
void SearchQueenMove()
{
	register int i, j, k, l;
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			QueenmoveBlack[i][j] = 0;//对queenmove棋盘初始化
			QueenmoveWhite[i][j] = 0;
		}
	}
	for (k = 0; k < 12; k++)
	{
		for (l = 0; l < 12; l++)
		{
			if (CurPosition[k][l] == BLACKCHESS)//黑棋的Queenmove
			{
				i = k;
				j = l;
				while (CurPosition[i][j - 1] == EMPTY)
				{
					QueenmoveBlack[i][j - 1] = 1;
					j--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j - 1] == EMPTY)
				{
					QueenmoveBlack[i - 1][j - 1] = 1;
					i--;
					j--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j] == EMPTY)
				{
					QueenmoveBlack[i - 1][j] = 1;
					i--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j + 1] == EMPTY)
				{
					QueenmoveBlack[i - 1][j + 1] = 1;
					i--; 
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i][j + 1] == EMPTY)
				{
					QueenmoveBlack[i][j + 1] = 1;
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j + 1] == EMPTY)
				{
					QueenmoveBlack[i + 1][j + 1] = 1;
					i++;
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j] == EMPTY)
				{
					QueenmoveBlack[i + 1][j] = 1;
					i++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j - 1] == EMPTY)
				{
					QueenmoveBlack[i + 1][j - 1] = 1;
					i++;
					j--;
				}
			}
			if (CurPosition[k][l] == WHITECHESS)//白棋的QueenMove
			{
				i = k;
				j = l;
				while (CurPosition[i][j - 1] == EMPTY)
				{
					QueenmoveWhite[i][j - 1] = 1;
					j--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j - 1] == EMPTY)
				{
					QueenmoveWhite[i - 1][j - 1] = 1;
					i--;
					j--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j] == EMPTY)
				{
					QueenmoveWhite[i - 1][j] = 1;
					i--;
				}
				i = k;
				j = l;
				while (CurPosition[i - 1][j + 1] == EMPTY)
				{
					QueenmoveWhite[i - 1][j + 1] = 1;
					i--;
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i][j + 1] == EMPTY)
				{
					QueenmoveWhite[i][j + 1] = 1;
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j + 1] == EMPTY)
				{
					QueenmoveWhite[i + 1][j + 1] = 1;
					i++;
					j++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j] == EMPTY)
				{
					QueenmoveWhite[i + 1][j] = 1;
					i++;
				}
				i = k;
				j = l;
				while (CurPosition[i + 1][j - 1] == EMPTY)
				{
					QueenmoveWhite[i + 1][j - 1] = 1;
					i++;
					j--;
				}
			}
		}
	}
	for (register int n = 1; n < 6; n++)
	{
		for (k = 0; k < 12; k++)
		{
			for (l = 0; l < 12; l++)
			{
				if (QueenmoveBlack[k][l] == n)
				{
					i = k;
					j = l;
					while (CurPosition[i][j - 1] == EMPTY&&QueenmoveBlack[i][j - 1] == 0)
					{
						QueenmoveBlack[i][j - 1] = n + 1;
						j--;
					}
					i = k;
					j = l;
					while (CurPosition[i - 1][j - 1] == EMPTY&&QueenmoveBlack[i - 1][j - 1] == 0)
					{
						QueenmoveBlack[i - 1][j - 1] = n + 1;
						i--;
						j--;
					}
					while (CurPosition[i - 1][j] == EMPTY&&QueenmoveBlack[i - 1][j] == 0)
					{
						QueenmoveBlack[i - 1][j] = n + 1;
						i--;
					}
					i = k;
					j = l;
					while (CurPosition[i - 1][j + 1] == EMPTY&&QueenmoveBlack[i - 1][j + 1] == 0)
					{
						QueenmoveBlack[i - 1][j + 1] = n + 1;
						i--;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i][j + 1] == EMPTY&&QueenmoveBlack[i][j + 1] == 0)
					{
						QueenmoveBlack[i][j + 1] = n + 1;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j + 1] == EMPTY&&QueenmoveBlack[i + 1][j + 1] == 0)
					{
						QueenmoveBlack[i + 1][j + 1] = n + 1;
						i++;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j] == EMPTY&&QueenmoveBlack[i + 1][j] == 0)
					{
						QueenmoveBlack[i + 1][j] = n + 1;
						i++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j - 1] == EMPTY&&QueenmoveBlack[i + 1][j - 1] == 0)
					{
						QueenmoveBlack[i = 1][j - 1] = n + 1;
						i++;
						j--;
					}
				}
				if (QueenmoveWhite[k][l] == n)//白棋
				{
					i = k;
					j = l;
					while (CurPosition[i][j - 1] == EMPTY&&QueenmoveWhite[i][j - 1] == 0)
					{
						QueenmoveWhite[i][j - 1] = n + 1;
						j--;
					}
					i = k;
					j = l;
					while (CurPosition[i - 1][j - 1] == EMPTY&&QueenmoveWhite[i - 1][j - 1] == 0)
					{
						QueenmoveWhite[i - 1][j - 1] = n + 1;
						i--;
						j--;
					}
					while (CurPosition[i - 1][j] == EMPTY&&QueenmoveWhite[i - 1][j] == 0)
					{
						QueenmoveWhite[i - 1][j] = n + 1;
						i--;
					}
					i = k;
					j = l;
					while (CurPosition[i - 1][j + 1] == EMPTY&&QueenmoveWhite[i - 1][j + 1] == 0)
					{
						QueenmoveWhite[i - 1][j + 1] = n + 1;
						i--;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i][j + 1] == EMPTY&&QueenmoveWhite[i][j + 1] == 0)
					{
						QueenmoveWhite[i][j + 1] = n + 1;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j + 1] == EMPTY&&QueenmoveWhite[i + 1][j + 1] == 0)
					{
						QueenmoveWhite[i + 1][j + 1] = n + 1;
						i++;
						j++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j] == EMPTY&&QueenmoveWhite[i + 1][j] == 0)
					{
						QueenmoveWhite[i + 1][j] = n + 1;
						i++;
					}
					i = k;
					j = l;
					while (CurPosition[i + 1][j - 1] == EMPTY&&QueenmoveWhite[i + 1][j - 1] == 0)
					{
						QueenmoveWhite[i = 1][j - 1] = n + 1;
						i++;
						j--;
					}
				}
			}
		}
	}
}
/////计算评估t1////////
void t1account()
{
	t1 = 0;//先归零
	int i, j;

	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)//扫描棋盘，通过queenmove对棋盘上的空格的控制权进行比较
			{
				if (QueenmoveBlack[i][j] == QueenmoveWhite[i][j] != 0)//对该空格的控制权相等且双方都能到达
				{
					t1 = t1 + 0.1;   //这时对该空格的控制权在于行棋方，该值在-1到1之间，代表行棋方的优势，可以进行调整。黑方先行
				}
				if (QueenmoveWhite[i][j] < QueenmoveBlack[i][j])//白方到达该空格的步数更少
				{
					if (QueenmoveWhite[i][j] == 0)   //白方进不去
					{
						t1 = t1 + 1;     //给黑方加一分
					}
					if (QueenmoveWhite[i][j] != 0)
					{
						t1 = t1 - 1;
					}
				}
				if (QueenmoveWhite[i][j] > QueenmoveBlack[i][j])    //黑方到达该空格的步数更少
				{
					if (QueenmoveBlack[i][j] == 0)//黑方进不去
					{
						t1 = t1 - 1;  //给黑方减一分
					}
					if (QueenmoveBlack[i][j] != 0)
					{
						t1 = t1 + 1;
					}
				}
			}
		}
	}
}
/////计算评估t2////////
void t2account()//计算kingmove,与t1计算原理相似
{
	t2 = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (KingmoveBlack[i][j] == KingmoveWhite[i][j] != 0)
				{
					t2 = t2 + 0.1;
				}
				if (KingmoveWhite[i][j] < KingmoveBlack[i][j])//白方到达该空格步数更少
				{
					if (KingmoveWhite[i][j] == 0)
					{
						t2 = t2 + 1;  //给黑方加一分
					}
					if (KingmoveWhite[i][j] != 0)
					{
						t2 = t2 - 1;
					}
				}
				if (KingmoveBlack[i][j] < KingmoveWhite[i][j])//黑方到达该空格的步数更少
				{
					if (KingmoveBlack[i][j] == 0)//黑方进不去
					{
						t2 = t2 - 1;  //黑方减一分
					}
					if (KingmoveBlack[i][j] != 0)
					{
						t2 = t2 + 1;
					}
				}
			}
		}
	}
}
/////计算评估C1////////
void c1account()//根据公式进行计算
{
	c1 = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (QueenmoveBlack[i][j] != 0 && QueenmoveWhite[i][j] != 0)
				{
					c1 = c1 + pow((double)2, -(QueenmoveBlack[i][j])) - pow((double)2, -(QueenmoveWhite[i][j]));
				}
				if (QueenmoveBlack[i][j] != 0 && QueenmoveWhite[i][j] == 0)
				{
					c1 = c1 + pow((double)2, -(QueenmoveBlack[i][j]));
				}
				if (QueenmoveBlack[i][j] == 0 && QueenmoveWhite[i][j] != 0)
				{
					c1 = c1 - pow((double)2, -(QueenmoveWhite[i][j]));
				}
			}
		}
	}
	c1 = c1 * 2;
}
/////计算评估C2////////
void c2account()
{
	c2 = 0;
	double a = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (KingmoveBlack[i][j] != 0 && KingmoveWhite[i][j] != 0)
				{
					a = KingmoveWhite[i][j] - KingmoveBlack[i][j];
					c2 = c2 + min(1.0, (double)max(-1.0, a / 6.0));
				}
				if (KingmoveBlack[i][j] != 0 && KingmoveWhite[i][j] == 0)
				{
					c2 = c2 + 1;
				}
				if (KingmoveBlack[i][j] == 0 && KingmoveWhite[i][j] != 0)
				{
					c2 = c2 - 1;
				}
			}
		}
	}
}
///////计算空格的灵活度/////
void MobilitySearch()
{
	int i, j;
	for ( i = 0; i < 12; i++)
	{
		for ( j = 0; j < 12; j++)
		{
			Mobility[i][j] = 0;
		}
	}
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (CurPosition[i][j - 1] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i - 1][j - 1] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i - 1][j] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i - 1][j + 1] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i][j + 1] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i + 1][j + 1] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i + 1][j] == EMPTY)
					Mobility[i][j] += 1;
				if (CurPosition[i + 1][j - 1] == EMPTY)
					Mobility[i][j] += 1;
			}
		}
	}
}
//////计算每个棋子的灵活度//////
void aaccount()
{
	register int k, l, m, i, j;
	register int n = 0;
	register int h = 0;
	aWhite[4] = { 0 };//存储每个棋子通过queenMove获得的灵活度
	aBlack[4] = { 0 };
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == BLACKCHESS)//找到一个黑子
			{
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k][l - 1] == EMPTY&&QueenmoveBlack[k][l - 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k][l - 1]/m;
					l--;
					m=m+1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l - 1] == EMPTY&&QueenmoveBlack[k - 1][l - 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k - 1][l - 1] / m;
					k--;
					l--;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l] == EMPTY&&QueenmoveBlack[k - 1][l] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k - 1][l] / m;
					k--;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l + 1] == EMPTY&&QueenmoveBlack[k - 1][l + 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k - 1][l + 1] / m;
					k--;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k][l + 1] == EMPTY&&QueenmoveBlack[k][l + 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k][l + 1] / m;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l + 1] == EMPTY&&QueenmoveBlack[k + 1][l + 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k + 1][l + 1] / m;
					k++;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l] == EMPTY&&QueenmoveBlack[k + 1][l] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k + 1][l] / m;
					k++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l - 1] == EMPTY&&QueenmoveBlack[k + 1][l - 1] != 0)
				{
					aBlack[n] = aBlack[n] + Mobility[k + 1][l - 1] / m;
					k++;
					l--;
					m = m + 1;
				}
				n++;
			}
			if (CurPosition[i][j] == WHITECHESS)//找到白棋
			{
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k][l - 1] == EMPTY&&QueenmoveWhite[k][l - 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k][l - 1] / m;
					l--;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l - 1] == EMPTY&&QueenmoveWhite[k - 1][l - 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k - 1][l - 1] / m;
					k--;
					l--;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l] == EMPTY&&QueenmoveWhite[k - 1][l] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k - 1][l] / m;
					k--;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k - 1][l + 1] == EMPTY&&QueenmoveWhite[k - 1][l + 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k - 1][l + 1] / m;
					k--;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k][l + 1] == EMPTY&&QueenmoveWhite[k][l + 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k][l + 1] / m;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l + 1] == EMPTY&&QueenmoveWhite[k + 1][l + 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k + 1][l + 1] / m;
					k++;
					l++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l] == EMPTY&&QueenmoveWhite[k + 1][l] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k + 1][l] / m;
					k++;
					m = m + 1;
				}
				k = i;
				l = j;
				m = 1;
				while (CurPosition[k + 1][l - 1] == EMPTY&&QueenmoveWhite[k + 1][l - 1] != 0)
				{
					aWhite[h] = aWhite[h] + Mobility[k + 1][l - 1] / m;
					k++;
					l--;
					m = m + 1;
				}
				h++;
			}
		}
	}
}
//////计算W参数//////
void waccount()/////计算公式///////
{
	w = 0;
	int i, j;
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (QueenmoveBlack[i][j] != 0 && QueenmoveWhite[i][j] != 0)
				{
					w = w + pow(2, -(fabs((double)(QueenmoveBlack[i][j] - QueenmoveWhite[i][j]))));
				}
			}
		}
	}
}
/////计算S参数///////
void saccount()
{
	s = 0;
	int M1, M2, N1, N2, i, j;
	M1 = M2 = N1 = N2 = 0;
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 12; j++)
		{
			if (CurPosition[i][j] == EMPTY)
			{
				if (QueenmoveBlack[i][j] < QueenmoveWhite[i][j])
				{
					if (QueenmoveBlack[i][j] == 0)
					{
						M2++;
						N2 += Mobility[i][j];
					}
					else
					{
						M1++;
						N1 += Mobility[i][j];
					}
				}
				if (QueenmoveBlack[i][j] > QueenmoveWhite[i][j])
				{
					if (QueenmoveWhite[i][j] == 0)
					{
						M1++;
						N1 += Mobility[i][j];
					}
					else
					{
						M2++;
						N2 += Mobility[i][j];
					}
				}
			}
		}
	}
	double s1 = M2 - M1;
	double s2 = N1 - N2;
	s = s1 / 1000 + s2 / 10000;
}
//////计算最终局面的评估/////
double Evaluate()////////////////////////////有待改进////////////////////////////////////
{
	double Value;
	//生成KingMove，QueenMove棋盘
	SearchKingMove();
	SearchQueenMove();
	//MobilitySearch();
	/////计算各个评估值//////
	t1account();
	t2account();
	c1account();
	c2account();
	//saccount();
	//////计算权重////////
	waccount();
	double a = (17 / (w + 17));
	double b = (w / (w + 49.03));
	double c = (1 - (a + b)) / 2;
	Value = a*t1 + b*t2 / 2 + c*((c1 + c2) / 2);
	return Value;
}