//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//军棋组负责人佟凯所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//佟凯		QQ：904674468
/* ************************************************************************ */
/* 示例代码维护：沈阳航空航天工大学 计算机博弈研究组				    	*/
/* 己方棋子编码约定:														*/
/*	a司令,b军长,c师长,d旅长,e团长,f营长,g连长,h排长,i工兵,j地雷k炸弹,l军旗	*/
/* 对方方棋子编码约定:														*/
/*	A司令,B军长,C师长,D旅长,E团长,F营长,G连长,H排长,I工兵,J地雷K炸弹,L军旗	*/
/*	X未知对方棋子,0空棋位													*/
/* 最后更新：2012-03-31  meixian@hrbust.edu.cn								*/
/* 司令的走法 ，31步碰子尽量选择较小的棋子 （碰在己方棋盘上的子）	棋盘的评估
躲避敌方的31步	 第一种碰子失败，采取第二种方法，  特殊走法的生成（炸弹  工兵）


/* ************************************************************************ */
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;
int huang1 = 0;
int huang2 = 0;
//int zz=0;
int qqqqqq = 0;         //调试时使用 ，在“1”中显示“第 n 步的开始”
int z1 = 0, z2 = 0, z3 = 0, z4 = 0;//判断己方棋子，是否为第一次进行营
int Ina_step[2] = { 2,0 };  //第一位代表谁先碰子的,1为我先碰子的，2为敌方先碰子的。第二位表示未碰子次数。
int tra = 0;
int track[100];
int jq = 0;//表示对方司令死后裁判发给我方对方军棋位置信息，1代表第一次发送，非1代表不是第一次发送
int x11, y11, x21, y21;//记录敌方上一步的行旗

int qipanfen[12][5] = {
	500 ,1000,600 ,1000,800,  //
	200 ,600 ,500 ,600 ,200,
	250 ,800 ,20  ,800 ,250,
	140 ,20  ,700 ,20  ,140,
	130 ,400 ,20  ,400 ,130,
	120 ,80  ,150 ,80  ,120,
	110 ,100 ,50 ,100 ,110,
	100 ,500 ,150  ,500 ,100,
	100 ,20  ,400 ,20  ,100,
	100 ,600 ,20  ,600 ,100,
	500 ,100 ,100 ,100 ,500,
	200 ,-1000 ,200 ,-1000 ,200 };
/* ************************************************************************ */
/* 函数功能：i,j位置是否本方棋子											*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方棋子，0空棋位或对方棋子											*/
/* ************************************************************************ */
int IsMyChess(char cMap[12][5], int i, int j)
{
	if (cMap[i][j] >= 'a'&& cMap[i][j] <= 'l')
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否本方可移动的棋子									*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1己方可移动棋子(司令,军长,...,工兵,炸弹)，0军旗,地雷,对方棋子或空棋位*/
/* ************************************************************************ */
int IsMyMovingChess(char cMap[12][5], int i, int j)
{
	if (cMap[i][j] >= 'a' && cMap[i][j] <= 'i' || cMap[i][j] == 'k')
		return 1;
	else
		return 0;
}

//用于计算我方可移动棋子总数
//返回值 棋子个数
int MyMovingChess(char cMap[12][5])
{
	int i, j;
	int n = 0;
	int N = 0;
	for (i = 0; i < 12; i++)
		for (j = 0; j < 5; j++)
			if (cMap[i][j] >= 'a'&&cMap[i][j] <= 'i' || cMap[i][j] == 'k')
			{
				if (i == 11 && j == 1)
					;
				else if (i == 11 && j == 3)
					;
				else
					n++;
			}
	return n;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否山界后的兵站										*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1处于山界后，0不处于山界后											*/
/* ************************************************************************ */
int IsAfterHill(int i, int j)
{
	if (i * 5 + j == 31 || i * 5 + j == 33)
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否行营												*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是行营，0不是行营													*/
/* ************************************************************************ */
int IsMoveCamp(int i, int j)
{
	if (i * 5 + j == 11 || i * 5 + j == 13 || i * 5 + j == 17 || i * 5 + j == 21 || i * 5 + j == 23 || i * 5 + j == 36 || i * 5 + j == 38 || i * 5 + j == 42 || i * 5 + j == 46 || i * 5 + j == 48)
		return 1;
	else
		return 0;
}

//函数功能：判断旗子是否在铁轨上
//int int （i，j）为棋盘位置行列号
int IsRailway(int i, int j)
{
	if (i == 1 || i == 5 || i == 6 || i == 10 || ((j == 0 || j == 4) && i>0 && i<11))
		return 1;
	else
		return 0;
}
/* ************************************************************************ */
/* 函数功能：i,j位置是否是对手棋子											*/
/* 接口参数：																*/
/*     char cMap[12][5]	棋盘局面											*/
/*     int i, j	棋盘位置行列号												*/
/* 返回值：																	*/
/*     1对手棋子，0空棋位或己方棋子											*/
/* ************************************************************************ */
int IsOppChess(char cMap[12][5], int i, int j)
{
	if (cMap[i][j] >= 'A' && cMap[i][j] <= 'X')
		return 1;
	else
		return 0;
}

//函数功能：i,j位置是否山界前的兵站
int IsBehindHill(int i, int j)
{
	if (i * 5 + j == 26 || i * 5 + j == 28)
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：工兵拐弯的搜索                                          		*/
/* 接口参数：																*/
/*     char cTryMap	尝试棋盘局面											*/
/*     int x	横坐标														*/
/*     int y	纵坐标														*/
/*     int iSaMove	记录工兵的铁路线走法									*/
/*     int iNum	已存走法数量												*/
/* 返回值：	初始: iNum=0															*/
/* ************************************************************************ */
int SapperRail(int iSaMove[40][2], char cTryMap[12][5], int x, int y, int iNum)
{
	int i, t;
	//可以前移:不在第一行,不在山界后,前方是空,落在铁道线上
	if (x>0 && !IsAfterHill(x, y) && (cTryMap[x - 1][y] == '0' || IsOppChess(cTryMap, x - 1, y)) && IsRailway(x - 1, y))
	{
		t = 0;
		for (i = 0; i <= iNum; i++)
		{
			if ((x - 1) == iSaMove[i][0] && y == iSaMove[i][1])//查出重复走法，跳出循环
				t = 1;
		}

		if (t == 0)
		{
			iNum++;
			iSaMove[iNum][0] = x - 1;
			iSaMove[iNum][1] = y;
			if (cTryMap[x - 1][y] == '0')
			{
				cTryMap[x][y] = '0';
				cTryMap[x - 1][y] = 'i';
				iNum = SapperRail(iSaMove, cTryMap, x - 1, y, iNum);
				cTryMap[x][y] = 'i';
				cTryMap[x - 1][y] = '0';
			}
		}
	}

	//可以左移:不在最左列,左侧是空,落在铁道线上
	if (y>0 && (cTryMap[x][y - 1] == '0' || IsOppChess(cTryMap, x, y - 1)) && IsRailway(x, y - 1))
	{
		t = 0;
		for (i = 0; i <= iNum; i++)
		{
			if (x == iSaMove[i][0] && (y - 1) == iSaMove[i][1])
				t = 1;
		}

		if (t == 0)
		{
			iNum++;
			iSaMove[iNum][0] = x;
			iSaMove[iNum][1] = y - 1;
			if (cTryMap[x][y - 1] == '0')
			{
				cTryMap[x][y] = '0';
				cTryMap[x][y - 1] = 'i';
				iNum = SapperRail(iSaMove, cTryMap, x, y - 1, iNum);
				cTryMap[x][y] = 'i';
				cTryMap[x][y - 1] = '0';
			}
		}
	}

	//可以右移://不在最右列,右侧不是己方棋子,落在铁道线上
	if (y<4 && (cTryMap[x][y + 1] == '0' || IsOppChess(cTryMap, x, y + 1)) && IsRailway(x, y + 1))
	{
		t = 0;
		for (i = 0; i <= iNum; i++)
		{
			if (x == iSaMove[i][0] && (y + 1) == iSaMove[i][1])
				t = 1;
		}

		if (t == 0)
		{
			iNum++;
			iSaMove[iNum][0] = x;
			iSaMove[iNum][1] = y + 1;
			if (cTryMap[x][y + 1] == '0')
			{
				cTryMap[x][y] = '0';
				cTryMap[x][y + 1] = 'i';
				iNum = SapperRail(iSaMove, cTryMap, x, y + 1, iNum);
				cTryMap[x][y] = 'i';
				cTryMap[x][y + 1] = '0';
			}
		}
	}

	//可以后移:不在最后一行,不在山界前,后方不是己方棋子,落在铁道线上
	if (x<11 && !IsBehindHill(x, y) && (cTryMap[x + 1][y] == '0' || IsOppChess(cTryMap, x + 1, y)) && IsRailway(x + 1, y))
	{
		t = 0;
		for (i = 0; i <= iNum; i++)
		{
			if ((x + 1) == iSaMove[i][0] && y == iSaMove[i][1])
				t = 1;
		}

		if (t == 0)
		{
			iNum++;
			iSaMove[iNum][0] = (x + 1);
			iSaMove[iNum][1] = y;
			if (cTryMap[x + 1][y] == '0')
			{
				cTryMap[x][y] = '0';
				cTryMap[x + 1][y] = 'i';
				iNum = SapperRail(iSaMove, cTryMap, x + 1, y, iNum);
				cTryMap[x][y] = 'i';
				cTryMap[x + 1][y] = '0';
			}
		}
	}

	return iNum;
}
/* ************************************************************************ */
/* 函数功能：判断是某个棋子的概率                                      		*/
/* 接口参数：																*/
/*     int i													            */
/*     int j		  所要判断棋子的位置								    */
/*     int k		  棋子编号  											*/
/*     int dz[25][14] 敌方棋子概率							                */
/* ************************************************************************ */
int Judge(int i, int j, int k, int dz[25][14])
{
	for (int n = 0; n<25; n++)
	{
		if (dz[n][12] == i && dz[n][13] == j)
			return dz[n][k];
	}
	return 0;
}
/* ************************************************************************ */
/* 函数功能：i,j位置是否大本营											*/
/* 接口参数：																*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1是大本营，0不是大本营												*/
/* ************************************************************************ */
int IsBaseCamp(int i, int j)
{
	if (i * 5 + j == 1 || i * 5 + j == 3 || i * 5 + j == 56 || i * 5 + j == 58)
		return 1;
	else
		return 0;
}

/* ************************************************************************ */
/* 函数功能：i,j位置是否有棋子占位的行营										*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     int i,j 棋盘位置行列号												*/
/* 返回值：																	*/
/*     1有棋子占位的行营,0不是行营或是空行营								*/
/* ************************************************************************ */
int IsFilledCamp(char cMap[12][5], int i, int j)
{
	if (IsMoveCamp(i, j) && cMap[i][j] != '0')
		return 1;
	else
		return 0;
}
int Iszuoqian(int i, int j)
{
	if (i * 5 + j == 19 || i * 5 + j == 29 || i * 5 + j == 27 || i * 5 + j == 44 || i * 5 + j == 52 || i * 5 + j == 54)
		return 1;
	else
		return 0;
}

//(i，j)为不在行营且可以向右前方移动的旗子
//  1为是那三点，0不是那三点
int Isyouqian(int i, int j)
{
	if (i * 5 + j == 15 || i * 5 + j == 25 || i * 5 + j == 27 || i * 5 + j == 40 || i * 5 + j == 50 || i * 5 + j == 52)
		return 1;
	else
		return 0;
}
//(i，j)为不在行营且可以向左后方移动的旗子
//          1为是那三点，0不是那三点

int Iszuohou(int i, int j)
{
	if (i * 5 + j == 7 || i * 5 + j == 9 || i * 5 + j == 19 || i * 5 + j == 32 || i * 5 + j == 34 || i * 5 + j == 44)
		return 1;
	else
		return 0;
}
//(i，j)为不在行营且可以向右后方移动的旗子
//         1为是那三点，0不是那三点
int Isyouhou(int i, int j)
{
	if (i * 5 + j == 5 || i * 5 + j == 7 || i * 5 + j == 15 || i * 5 + j == 30 || i * 5 + j == 32 || i * 5 + j == 40)
		return 1;
	else
		return 0;
}
/* ************************************************************************ */
/* 函数功能：双方布局后棋局初始化											*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     char *cOutMessage 布局字符序列										*/
/* ************************************************************************ */
void InitMap(char cMap[12][5], char *cOutMessage)
{
	int i, j, k;
	for (i = 0; i<6; i++)	//标记对方棋子
		for (j = 0; j<5; j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = 'X';
	k = 6;
	for (i = 6; i<12; i++)	//标记己方棋子
		for (j = 0; j<5; j++)
			if (IsMoveCamp(i, j))
				cMap[i][j] = '0';
			else
				cMap[i][j] = cOutMessage[k++];

}

//计算我方棋子还剩多少个
//参数 cMap 当前地图
//返回值为我方剩余棋子
int NumChess(char cMap[12][5])
{
	int n = 0; //我方棋子数
	int i, j;

	for (i = 0; i < 12; i++)
		for (j = 0; j < 5; j++)
			if (cMap[i][j] >= 'a'&&cMap[i][j] <= 'l')
				n++;
	return n;
}

//计算敌方棋子还剩多少个
//参数 cMap 当前地图
//返回值为敌方剩余棋子
int NumOppChess(char cMap[12][5])
{
	int n = 0; //我敌棋子数
	int i, j;

	for (i = 0; i < 12; i++)
		for (j = 0; j < 5; j++)
			if (cMap[i][j] >= 'A'&&cMap[i][j] <= 'X')
				n++;
	return n;
}

void souhui(int n, int i, int sfen, int dz[25][14])         //sfen 回收的分数   n为旗子位置   i为旗子大小
{
	int zong = 0;
	int n1;
	if (sfen == 0)
		return;
	if (i == 0 || i == 1 || i == 11)
		zong = 10000;
	else if (i>5 && i<10)
		zong = 30000;
	else
		zong = 20000;
	for (n1 = 0; n1<25; n1++)
	{
		if (n1 != n && (zong - dz[n][i]) != 0 && dz[n1][i]<9500)
			dz[n1][i] = 1.0*dz[n1][i] * (1 - 1.0*sfen / (zong - dz[n][i]));
	}
}

//刷新猜测概率表   //对方棋子被己方吃掉
void shuaxinG0(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{  //m=6
	int i, n, n1;
	int zong = 1, sfen = 0;
	if (i2 == 6 && j2 == 4)
		huang1 = 1;
	if (i1 != i2 && j1 != j2 && (IsRailway(i1, j1) && IsRailway(i2, j2)))    //敌方棋子为工兵.......10    50     54     14
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 8 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				souhui(n, 8, 10000 - dz[n][8], dz);
				dz[n][8] = 10000;		//工兵分值变10000
			}
		}
		return;
	}
	if (m<8 && m >= 0)//己方棋子为司令至排长
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && (i <= m || i>8) && dz[n][i] != 10000)    //不是排长或者工兵
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));    //增加棋盘上其他位置比己方棋子大的分值，     被吃掉的旗子只能为排长和工兵，所以其他地方相应其他子的分值增加
						dz[n][i] = 0;   			//敌方棋子死掉，所以此位置为比己方棋子大的分值为0
					}
				}
				zong = 1;
				for (i = m + 1; i <= 8; i++)
					zong = zong + dz[n][i];
				for (i = m + 1; i <= 8; i++) //将棋盘上其他地方比该棋子小的分值减少
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);                                                        //这个位置为排长，相应其他地方为排长的的概率减少
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}
			}
		}
	}
	if (m == 9)//己方棋子为地雷
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && (i == 8 || i == 10) && dz[n][i] != 10000)    //不是炸弹或者工兵
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				zong = 1;
				for (i = 0; i<8; i++)
					zong = zong + dz[n][i];
				for (i = 0; i<8; i++) //将棋盘上其他地方为司令至排长的分值增加
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}
			}
		}
	}
}
//对方吃掉己方棋子
void shuaxinG1(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{  //m=4 tuanzhang
	int i, n, n1;
	int zong = 1, sfen = 0;
	if (cMap[i2][j2] <= 'd')
	{
		for (n = 0; n < 25; n++)
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				track[tra] = n;
				tra++;
			}
	}
	if ((i1 != i2 && j1 != j2 && IsRailway(i1, j1) && IsRailway(i2, j2)) || (m == 9))//敌方棋子为工兵.......10    50     54     14
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = i2;
				dz[n][13] = j2;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 8 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				souhui(n, 8, 10000 - dz[n][8], dz);
				dz[n][8] = 10000;
			}
		}
		return;
	}
	if (m<9 && m >= 0)//己方棋子为军长至工兵		m>0
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = i2;
				dz[n][13] = j2;

				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && ((i >= m && i<9) || (i == 10)) && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));    //增加棋盘上其他位置比己方棋子小的分值，
						dz[n][i] = 0;            //己方棋子死掉，所以此位置为比己方棋子小的分值为0
					}
				}
				zong = 1;
				for (i = 0; i<m; i++)
					zong = zong + dz[n][i];
				for (i = 0; i<m; i++) //将棋盘上其他地方比该棋子大的分值减少
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}
			}
		}
	}
}
//敌方棋子与己方棋子对死
void shuaxinG2(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{
	int i, n, n1;
	int zong = 0, sfen = 0;
	if (i1 != i2 && j1 != j2 && IsRailway(i1, j1) && IsRailway(i2, j2))//敌方棋子为工兵.......10    50     54     14
	{

		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 8 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				souhui(n, 8, 10000 - dz[n][8], dz);
				dz[n][8] = 10000;
			}
		}
		return;
	}
	if (m<9 && m >= 0)//己方棋子为司令至工兵
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				if (m == 0)  //己方棋子为司令
				{
					if (jq == 1)//对方棋子为司令
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != 0 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9500)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}
						dz[n][0] = 10000;
					}
					else      //对方棋子为炸弹
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != 10 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9500)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}
						sfen = 10000 - dz[n][10];
						souhui(n, 10, sfen, dz);
						dz[n][10] = 10000;
					}
				}
				else   //己方棋子为军长到工兵
				{
					if (m<2)
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && (i != m && i != 10) && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9000)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}
						zong = dz[n][m] + dz[n][10] + 1;

						sfen = 1.0 * 10000 * dz[n][10] / zong - dz[n][10];
						souhui(n, 10, sfen, dz);
						dz[n][10] = 10000 * (1.0*dz[n][10] / zong);

						sfen = 1.0 * 10000 * dz[n][m] / zong - dz[n][m];
						souhui(n, m, sfen, dz);
						dz[n][m] = 10000 * (1.0*dz[n][m] / zong);
					}
					else
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != m && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9000)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}

						sfen = 10000 - dz[n][m];
						souhui(n, m, sfen, dz);
						dz[n][m] = 10000;
					}

				}
			}
		}
	}
	if (m == 9)   //己方棋子为地雷
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 10 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				sfen = 10000 - dz[n][10];
				souhui(n, 10, sfen, dz);
				dz[n][10] = 10000;
			}
		}
	}
	if (m == 10)
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = 14;
			}
		}
	}
}
//敌方仅移动棋子
void shuaxinG3(int i1, int j1, int i2, int j2, int dz[25][14]) //仅移动棋子
{
	int n, n1, i;
	int sfen, zong = 0;
	if (i1 != i2 && j1 != j2 && IsRailway(i1, j1) && IsRailway(i2, j2))//敌方棋子为工兵.......10    50     54     14
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i1 && dz[n][13] == j1)
			{
				dz[n][12] = i2;
				dz[n][13] = j2;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 8 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
				souhui(n, 8, 10000 - dz[n][8], dz);
				dz[n][8] = 10000;
			}
		}

	}
	for (n = 0; n<25; n++)
	{
		if (dz[n][12] == i1 && dz[n][13] == j1)
		{
			dz[n][12] = i2;
			dz[n][13] = j2;
			if (z1 == 1 && y21 == 2 && x21 == 1)
				dz[n][10] = 4000;
			if (z2 == 1 && y21 == 2 && x21 == 3)
				dz[n][10] = 4000;
			if (z3 == 1 && y21 == 4 && x21 == 1)
				dz[n][10] = 4000;
			if (z4 == 1 && y21 == 4 && x21 == 3)
				dz[n][10] = 4000;
		}
	}
}
//  //己方棋子被对方吃掉
void shuaxinR0(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{  //m=4 tuanzhang
	int i, n, n1;
	int zong = 1, sfen = 0;
	if (cMap[i1][j1] <= 'd')
	{
		for (n = 0; n < 25; n++)
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				track[tra] = n;
				tra++;
			}
	}
	if (m == 8)//己方棋子为工兵
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && (i == 8 || i == 9 || i == 10) && dz[n][i] != 10000)    //不是,工兵,炸弹，地雷
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));    //增加棋盘上其他位置比己方棋子小的分值，
						dz[n][i] = 0;            //己方棋子死掉，所以此位置地雷炸弹分值为0
					}
				}
				zong = 1;    //   （  没有地雷）
				for (i = 0; i<m; i++)
					zong = zong + dz[n][i];
				for (i = 0; i<m; i++) //将棋盘上其他地方比该棋子大的分值减少
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}

			}
		}
	}
	if (m<8 && m>0)//己方棋子为军长至排长
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && ((i >= m && i<9) || (i == 10)) && dz[n][i] != 10000)    //不是团长到工兵
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));    //增加棋盘上其他位置比己方棋子小的分值，
						dz[n][i] = 0;            //己方棋子死掉，所以此位置为比己方棋子小的分值为0(不可能的棋子)
					}
				}
				zong = 1;    //   （  没有地雷）
				for (i = 0; i<m; i++)
					zong = zong + dz[n][i];
				for (i = 0; i<m; i++) //将棋盘上其他地方比该棋子大的分值减少
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}

			}
		}
	}
	if (m == 0)// 己方棋子为司令，敌方为地雷
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				souhui(n, 9, 10000 - dz[n][9], dz);
				dz[n][9] = 10000;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 9 && dz[n][i] != 10000)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9500)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
			}
		}
	}
	if (i2 == 0 && j2 == 1)
	{
		dz[3][11] = 10000;
		dz[1][11] = 0;
		dz[3][7] = 0;  //另一个地方为军旗，为排长和地雷的分值为0
		dz[3][9] = 0;
		dz[1][7] = 6000;
		dz[1][9] = 4000;
	}
	if (i2 == 0 && j2 == 3)
	{
		dz[1][11] = 10000;
		dz[3][11] = 0;
		dz[1][7] = 0;
		dz[1][9] = 0;
		dz[3][7] = 6000;
		dz[3][9] = 4000;
	}
}
//己方吃掉对方棋子
void shuaxinR1(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{   //m=6
	int i, n, n1;
	int zong = 1, sfen = 0;
	if (m<8 && m >= 0)//己方棋子为司令至排长
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				dz[n][12] = 14;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && (i <= m || i>8) && dz[n][i] != 10000)    //不是排长或者工兵
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));    //增加棋盘上其他位置比己方棋子大的分值，     被吃掉的旗子只能为排长和工兵，所以其他地方相应其他子的分值增加
						dz[n][i] = 0;            //敌方棋子死掉，所以此位置为比己方棋子大的分值为0
					}
				}
				zong = 1;
				for (i = m + 1; i <= 8; i++)
					zong = zong + dz[n][i];
				for (i = m + 1; i <= 8; i++) //将棋盘上其他地方比该棋子小的分值减少
				{
					sfen = 1.0 * 10000 * dz[n][i] / zong - dz[n][i];
					souhui(n, i, sfen, dz);                                                        //这个位置为排长，相应其他地方为排长的的概率减少
					dz[n][i] = 1.0 * 10000 * dz[n][i] / zong;
				}
			}
		}
	}
	if (m == 8)//己方为工兵，敌方为地雷
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				dz[n][12] = 14;
				souhui(n, 9, 10000 - dz[n][9], dz);
				dz[n][9] = 10000;
				for (i = 0; i<12; i++)
				{
					if (dz[n][i] != 0 && i != 9)
					{
						if (i == 0 || i == 1 || i == 11)
							zong = 10000;
						else if (i>5 && i<10)
							zong = 30000;
						else
							zong = 20000;
						for (n1 = 0; n1<25; n1++)
							if (n1 != n && dz[n1][i]<9000)
								dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
						dz[n][i] = 0;
					}
				}
			}
		}

	}
	if (i2 == 0 && j2 == 1)
	{
		dz[3][11] = 10000;
		dz[1][11] = 0;
		dz[3][7] = 0;  //另一个地方为军旗，为排长和地雷的分值为0
		dz[3][9] = 0;
		dz[1][7] = 6000;
		dz[1][9] = 4000;
	}
	if (i2 == 0 && j2 == 3)
	{
		dz[1][11] = 10000;
		dz[3][11] = 0;
		dz[1][7] = 0;
		dz[1][9] = 0;
		dz[3][7] = 6000;
		dz[3][9] = 4000;
	}
}

//己方移动，双方棋子对死
void shuaxinR2(int m, char cMap[12][5], int dz[25][14], int i1, int j1, int i2, int j2)
{
	int i, n, n1;
	int zong = 1, sfen = 0;
	if (m<9 && m >= 0)//己方棋子为司令至工兵
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				dz[n][12] = 14;
				if (m == 0)  //己方棋子为司令
				{
					if (jq == 1)//对方棋子为司令
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != 0 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9500)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}
						dz[n][0] = 10000;
					}
					else      //对方棋子为炸弹
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != 10 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9500)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}
						sfen = 10000 - dz[n][10];
						souhui(n, 10, sfen, dz);
						dz[n][10] = 10000;
					}
				}
				else   //己方棋子为军长到工兵
				{
					if (m<2)//如果己方棋子为师长以上，那么敌方棋子可能为炸弹
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != m && i != 10 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9000)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}

						zong = dz[n][m] + dz[n][10] + 1;  //防止分母为0，出错。

						sfen = 1.0 * 10000 * dz[n][m] / zong - dz[n][m];
						souhui(n, m, sfen, dz);
						dz[n][m] = 1.0 * 10000 * dz[n][m] / zong;

						sfen = 1.0 * 10000 * dz[n][10] / zong - dz[n][10];
						souhui(n, 10, sfen, dz);
						dz[n][10] = 1.0 * 10000 * dz[n][10] / zong;
					}
					else//敌方棋子为师长以下，则不考虑炸弹的概率
					{
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i != m && i != 10 && dz[n][i] != 10000)
							{
								if (i == 0 || i == 1 || i == 11)
									zong = 10000;
								else if (i>5 && i<10)
									zong = 30000;
								else
									zong = 20000;
								for (n1 = 0; n1<25; n1++)
									if (n1 != n && dz[n1][i]<9000)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (zong - dz[n][i]));
								dz[n][i] = 0;
							}
						}

						sfen = 10000 - dz[n][m];
						souhui(n, m, sfen, dz);
						dz[n][m] = 10000;
					}
				}
			}
		}
	}
	if (m == 10)
	{
		for (n = 0; n<25; n++)
		{
			if (dz[n][12] == i2 && dz[n][13] == j2)
			{
				dz[n][12] = 14;
			}
		}
	}
	if (i2 == 0 && j2 == 1)
	{
		dz[3][11] = 10000;
		dz[1][11] = 0;
		dz[3][7] = 0;  //另一个地方为军旗，为排长和地雷的分值为0
		dz[3][9] = 0;
		dz[1][7] = 6000;
		dz[1][9] = 4000;
	}
	if (i2 == 0 && j2 == 3)
	{
		dz[1][11] = 10000;
		dz[3][11] = 0;
		dz[1][7] = 0;
		dz[1][9] = 0;
		dz[3][7] = 6000;
		dz[3][9] = 4000;
	}
}

/* ************************************************************************ */
/* 函数功能：根据裁判反馈刷新棋盘											*/
/* 接口参数：																*/
/*     char cMap[12][5] 棋盘局面											*/
/*     char *cInMessage 来自裁判的GO YXYX R YX命令							*/
/*     char *cOutMessage 发给裁判的BESTMOVE YXYX命令						*/
/* ************************************************************************ */
void FreshMap(char cMap[12][5], char *cInMessage, char *cOutMessage, int dz[25][14])
{
	int x1, y1;				//起点					//x1,y1,x2,y2,result由char型改为int型，没什么影响
	int x2, y2;				//落点
	int result = -1, k;			//碰子结果
	int n, i, n1, zong = 1;
	//判断横排是否就一个地方有概率
	for (n = 0; n<25; n++)
	{
		k = 0;
		for (i = 0; i<12; i++)
		{
			if (dz[n][i] <= 50)
				k++;
		}
		if (k >= 11)
		{
			for (i = 0; i<12; i++)
			{
				if (dz[n][i]>50 && dz[n][i]<10000)
				{
					if (i == 0 || i == 1 || i == 11)
						zong = 10000;           //司令，军长和军旗都只有一个
					else if (i>5 && i<10)
						zong = 10000 * 3;         //每个棋子共有3个
					else
						zong = 10000 * 2;         //每个棋子有两个
					for (n1 = 0; n1<25; n1++)
						if (n1 != n)
							dz[n1][i] = 1.0*dz[n1][i] * (1 - 1.0*(10000 - dz[n][i]) / (zong - dz[n][i]));    //确定某个地方是什么旗子
					souhui(n, i, 10000 - dz[n][i], dz);
					dz[n][i] = 10000;
				}

			}
		}

	}
	//判断竖排
	for (i = 0; i<12; i++)
	{
		k = 0;
		for (n = 0; n<25; n++)
			if (dz[n][i] >= 8000)
				k++;
		if (i == 0 || i == 1 || i == 11)
			zong = 1;
		else if (i>5 && i<10)
			zong = 3;
		else
			zong = 2;
		if (k == zong)            //确定棋子在什么地方(比如两个旅长的位置都知道以后)
		{
			for (n = 0; n<25; n++)
			{
				if (dz[n][i]<8000)
				{
					for (k = 0; k<12; k++)
						if (k != i)
						{
							dz[n][k] = 1.0*dz[n][k] * (1 + 1.0*dz[n][i] / (10000 - dz[n][i]));      //竖排每个不是旅长的位置增加相应的分数
						}

					dz[n][i] = 0;        //将不是(旅长)的位置进行能够清0
				}
			}
		}
	}

	if (cInMessage[0] == 'G')	// GO 指令
	{
		if (cInMessage[3] >= 'A' && cInMessage[3] <= 'L')																					//x,y坐标发生对换，因为不习惯
		{
			y1 = cInMessage[3] - 'A';
			x1 = cInMessage[4] - '0';
			y2 = cInMessage[5] - 'A';
			x2 = cInMessage[6] - '0';
			result = cInMessage[8] - '0';		//碰子结果
			y11 = y1;
			x11 = x1;
			y21 = y2;
			x21 = x2;
			if (y21 == 2 && x21 == 1 && y11<5)
				z1++;
			if (y21 == 2 && x21 == 3 && y11<5)
				z2++;
			if (y21 == 4 && x21 == 1 && y11<5)
				z3++;
			if (y21 == 4 && x21 == 3 && y11<5)
				z4++;
			if (cInMessage[10] >= 'A' && cInMessage[10] <= 'L') //对方司令战死后显示军旗位置
			{
				cMap[(cInMessage[10] - 'A')][cInMessage[11] - '0'] = 'L';
				jq++;
				if (cInMessage[11] - '0' == 1)
				{
					dz[1][11] = 10000;
					dz[1][7] = 0;//此地为军旗，为排长和地雷的分值为0
					dz[1][9] = 0;
					dz[3][11] = 0;
					dz[3][7] = 6000;
					dz[3][9] = 4000;
				}
				if (cInMessage[11] - '0' == 3)
				{
					dz[3][11] = 10000;
					dz[1][11] = 0;
					dz[3][7] = 0;
					dz[3][9] = 0;
					dz[1][7] = 6000;
					dz[1][9] = 4000;
				}
			}
			/////判断地雷的概率///////////
			if (y1<2)
			{
				for (n = 0; n<25; n++)
				{
					if (dz[n][12] == y1 && dz[n][13] == x1)
					{
						dz[n][12] = y2;
						dz[n][13] = x2;
						for (i = 0; i<12; i++)
						{
							if (dz[n][i] != 0 && i == 9 && dz[n][i] != 10000)
							{
								for (n1 = 0; n1<25; n1++)
								{
									if (n1 != n && dz[n1][i]<9000)
										dz[n1][i] = 1.0*dz[n1][i] * (1 + 1.0*dz[n][i] / (30000 - dz[n][i]));
								}
								dz[n][i] = 0;
							}
						}
						zong = 1;
						for (i = 0; i<9; i++)
							zong = zong + dz[n][i];
						for (i = 0; i<9; i++)
						{
							souhui(n, i, 1.0 * 10000 * dz[n][i] / (zong + 1) - dz[n][i], dz);
							dz[n][i] = 10000 * dz[n][i] / (zong + 1);
						}

					}
				}
			}
			switch (result)		//根据不同结果修改棋盘
			{
			case 0:			//对方棋子被己方吃掉
				Ina_step[0] = 2;
				Ina_step[1] = 0;
				k = cMap[y2][x2] - 'a';  //终点为己方棋子
				shuaxinG0(k, cMap, dz, y1, x1, y2, x2);
				cMap[y1][x1] = '0';
				break;
			case 1:			//对方吃掉己方棋子
				Ina_step[0] = 2;
				Ina_step[1] = 0;
				k = cMap[y2][x2] - 'a';//终点为己方棋子
				shuaxinG1(k, cMap, dz, y1, x1, y2, x2);
				cMap[y2][x2] = cMap[y1][x1];
				cMap[y1][x1] = '0';
				break;
			case 2:			//双方棋子对死
				Ina_step[0] = 2;
				Ina_step[1] = 0;
				k = cMap[y2][x2] - 'a';//终点为己方棋子
				shuaxinG2(k, cMap, dz, y1, x1, y2, x2);
				cMap[y1][x1] = '0';
				cMap[y2][x2] = '0';
				break;
			case 3:			//对方移动棋子、、、///进 行营炸弹概率增加
				Ina_step[1]++;
				shuaxinG3(y1, x1, y2, x2, dz);
				cMap[y2][x2] = cMap[y1][x1];
				cMap[y1][x1] = '0';
				break;
			}
			FILE *fp;
			fp = fopen("1", "a");
			fprintf(fp, "\n%d,%c,%c,%d,%d,%d,%d,%d\n", result, cMap[y2][x2], cMap[y1][x1], y2, x2, y1, x1, k);
			fclose(fp);
		}
	}
	if (cInMessage[0] == 'R')	// RESULT 指令
	{
		y1 = cOutMessage[9] - 'A';
		x1 = cOutMessage[10] - '0';
		y2 = cOutMessage[11] - 'A';
		x2 = cOutMessage[12] - '0';
		result = cInMessage[7] - '0';		//结果结果
		if (cInMessage[8] == ' ' && cInMessage[9] >= 'A' && cInMessage[9] <= 'L') //对方司令战死后显示军旗位置
		{
			cMap[(cInMessage[10] - 'A')][cInMessage[11] - '0'] = 'L';
			jq++;
			if (cInMessage[11] - '0' == 1)
			{
				dz[1][11] = 10000;
				dz[3][11] = 0;
				dz[1][7] = 0;//此地为军旗，为排长和地雷的分值为0
				dz[1][9] = 0;
			}
			if (cInMessage[11] - '0' == 3)
			{
				dz[3][11] = 10000;
				dz[1][11] = 0;
				dz[3][7] = 0;
				dz[3][9] = 0;
			}
		}
		switch (result)		//根据不同结果修改棋盘
		{
		case 0:			//己方棋子被对方吃掉
			Ina_step[0] = 1;
			Ina_step[1] = 0;
			k = cMap[y1][x1] - 'a';  //起点为己方棋子
			shuaxinR0(k, cMap, dz, y1, x1, y2, x2);//根据己方的棋子刷新猜测概率表
			cMap[y1][x1] = '0';
			break;
		case 1:			//己方吃掉对方棋子
			Ina_step[0] = 1;
			Ina_step[1] = 0;
			k = cMap[y1][x1] - 'a';
			shuaxinR1(k, cMap, dz, y1, x1, y2, x2);
			cMap[y2][x2] = cMap[y1][x1];
			cMap[y1][x1] = '0';
			break;
		case 2:			//双方棋子对死
			Ina_step[0] = 1;
			Ina_step[1] = 0;
			k = cMap[y1][x1] - 'a';
			shuaxinR2(k, cMap, dz, y1, x1, y2, x2);
			cMap[y1][x1] = '0';
			cMap[y2][x2] = '0';
			break;
		case 3:			//己方移动棋子
			Ina_step[1]++;
			cMap[y2][x2] = cMap[y1][x1];
			cMap[y1][x1] = '0';
			break;
		}
	}
	FILE *fp;
	fp = fopen("1", "a");
	fprintf(fp, "\n\n\n\n\n\n\n\n第%d步的开始:\n", qqqqqq + 1);
	for (int i = 0; i != 12; i++)
	{
		for (int j = 0; j != 5; j++)
		{
			if (cMap[i][j] == 'X')
				fprintf(fp, "xx ");
			if (cMap[i][j] == 'L')
				fprintf(fp, "旗 ");
			if (cMap[i][j] == '0')
				fprintf(fp, "   ");
			if (cMap[i][j] == 'a')
				fprintf(fp, "司 ");
			if (cMap[i][j] == 'b')
				fprintf(fp, "军 ");
			if (cMap[i][j] == 'c')
				fprintf(fp, "师 ");
			if (cMap[i][j] == 'd')
				fprintf(fp, "旅 ");
			if (cMap[i][j] == 'e')
				fprintf(fp, "团 ");
			if (cMap[i][j] == 'f')
				fprintf(fp, "营 ");
			if (cMap[i][j] == 'g')
				fprintf(fp, "连 ");
			if (cMap[i][j] == 'h')
				fprintf(fp, "排 ");
			if (cMap[i][j] == 'i')
				fprintf(fp, "兵 ");
			if (cMap[i][j] == 'j')
				fprintf(fp, "雷 ");
			if (cMap[i][j] == 'k')
				fprintf(fp, "炸 ");
			if (cMap[i][j] == 'l')
				fprintf(fp, "旗 ");
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}
/* ************************************************************************ */
/* 函数功能：根据INFO指令,返回参赛队名										*/
/* 接口参数：																*/
/*     char *cInMessage 接收的INFO ver指令									*/
/*     char *cOutMessage 参赛队名											*/
/* ************************************************************************ */
void CulInfo(char *cInMessage, char *cVer, char *cOutMessage)
{
	strcpy(cVer, cInMessage + 5);																												//此语句没用
	strcpy(cOutMessage, "NAME ShenYang Aerospace University!");
}

/* ************************************************************************ */
/* 函数功能：根据START指令,返回布局											*/
/* 接口参数：																*/
/*     char *cInMessage 接收的START first time steps指令					*/
/*     int *iFirst 先行权[0先行，1后行]										*/
/*     int *iTime 行棋时间限制(单位秒)[1000,3600]							*/
/*     int *iStep 进攻等待限制(单位步)[10,31]								*/
/*     char *cOutMessage 布局字符序列										*/
/* ************************************************************************ */
void CulArray(char *cInMessage, int *iFirst, int *iTime, int *iStep, char *cOutMessage)
{
	*iFirst = cInMessage[6] - '0';
	*iTime = cInMessage[8] - '0';
	*iTime = *iTime * 10 + (cInMessage[9] - '0');
	*iTime = *iTime * 10 + (cInMessage[10] - '0');
	*iTime = *iTime * 10 + (cInMessage[11] - '0');
	*iStep = cInMessage[13] - '0';
	*iStep = *iStep * 10 + (cInMessage[14] - '0');
	if (*iFirst == 0)	//先手		a司令,b军长,c师长,d旅长,e团长,f营长,g连长,h排长,i工兵,j地雷k炸弹,l军旗   cdgdckhkfiifebejagjihljhg    hegickfkcibefigjajdhdljhg
						//	strcpy(cOutMessage,"ARRAY hcgcgkikgfdfdabejiiejljhh");         chdfckgkdiefabgjjiiejlghh           cdgdckhkfiifebejagjihljhg   cdedckfkfiiegbijajjghlhhg    cdedckfkfiiegbijajjghlhhg
		strcpy(cOutMessage, "ARRAY cehiakbcdikegfgjjdifhhglj");       //1.    cgdgckfkeiiefgbjajjdhlhhi   2.cgdgckekfiifeibjajjdhlhhg

	else			//后手				chefckgkfiidgbjjidjeghhlj     hegickfkcibefigjajdhdljhg
	{
		//	strcpy(cOutMessage,"ARRAY hcgcgkikgfdfadbejiiejljhh");		chdfckgkdiefabijjgiejlghh    chdfckgkdiefabgjjiiejlghh   cdgdhkbkfiifecejagjihljhg
		strcpy(cOutMessage, "ARRAY cehiakbcdikegfgjjdifhhglj");       //1.       cgdgckfkeiiefgbjajjdhlhhi   2.cgdgckekfiifeibjajjdhlhhg
		Ina_step[0] = 1;
	}
}
//所要行走的位置对己方造成的威胁分值
int weixie(int x, int y, int x1, int y1, int dz[25][14], char cMap[12][5])
{
	int n, score = 0, k, i, j, t, t1 = 0;
	if (cMap[x1][y1] == '0')
		return 0;
	for (n = 0; n<25; n++)//在猜测概率表中找到想要行走的位置
	{
		if (dz[n][12] == x1 && dz[n][13] == y1)
			break;
	}
	k = cMap[x][y] - 'a';
	//进行分值评估
	if (k >= 0 && k != 10)  //     k==10           为炸弹，先不考虑
	{
		for (i = 0; i<9; i++)//可以行走的棋的分值
		{
			if (i<k)
				score = score - dz[n][i];
			else if (i>k)
				score = score + dz[n][i];
			else
				continue;
		}
		if (cMap[x][y] == 'i')
			score = score + dz[n][9];
		else
			score = score - dz[n][9];//地雷的分值
	}
	//统计该棋子是否为现有棋子中最大的两个
	for (i = 0; i<12; i++)
	{
		for (j = 0; j<5; j++)
		{
			if (cMap[i][j] >= 'a' && cMap[i][j] <= 'i')
			{
				if (i == x && j == y)
					continue;
				else
				{
					t = cMap[i][j] - 'a';
					if (t <= k)   //比较两个棋子的大小
						t1++;  //计数
				}
			}
		}
	}
	if (t1<3)//如果棋子是己方最大的两个减去炸弹的分值
		score = score - dz[n][10];
	return score;
}
int jweixie(int i, int j, int dz[25][14], char cMap[12][5])
{
	int i1, j1;
	int score = 0, score1 = 0;
	if (IsMoveCamp(i, j) == 1)
		score = 0;
	else//分数取最小值，因为行棋分数为行走最大分数
	{
		//向前搜索找到对己方棋子的威胁概率
		//不在第一排，不在山界后，前方不是己方棋子
		if (i>0 && !IsAfterHill(i, j) && !IsMyChess(cMap, i - 1, j))
		{
			score1 = weixie(i, j, i - 1, j, dz, cMap);
			if (score1<score)
				score = score1;
			if (i>1 && i<11 && (j == 0 || j == 4))//在铁轨上
			{
				for (i1 = i - 1; i1>0; i1--)
				{
					if (cMap[i1][j] == '0')
						continue;
					else if (cMap[i1][j] >= 'A' && cMap[i1][j] <= 'X')
					{
						score1 = weixie(i, j, i1, j, dz, cMap);
						if (score1<score)
							score = score1;
						break;
					}
					else
						break;
				}
			}
		}
		//向后搜索
		if (i<11 && !IsBehindHill(i, j) && !IsMyChess(cMap, i + 1, j))
		{
			score1 = weixie(i, j, i + 1, j, dz, cMap);
			if (score1<score)
				score = score1;
			if (i>0 && i<10 && (j == 0 || j == 4))
			{
				for (i1 = i + 1; i1<11; i1++)
				{
					if (cMap[i1][j] == '0')
						continue;
					else if (cMap[i1][j] >= 'A' && cMap[i1][j] <= 'X')
					{
						score1 = weixie(i, j, i1, j, dz, cMap);
						if (score1<score)
							score = score1;
						break;
					}
					else
						break;
				}
			}
		}
		//向左搜索
		if (j>0 && !IsMyChess(cMap, i, j - 1))
		{
			score1 = weixie(i, j, i, j - 1, dz, cMap);
			if (score1<score)
				score = score1;
			if (j>0 && j<5 && (i == 1 || i == 5 || i == 6 || i == 10))
			{
				for (j1 = j - 1; j1 >= 0; j1--)
				{
					if (cMap[i][j1] == '0')
						continue;
					else if (cMap[i][j1] >= 'A' && cMap[i][j1] <= 'X')
					{
						score1 = weixie(i, j, i, j1, dz, cMap);
						if (score1<score)
							score = score1;
						break;
					}
					else
						break;
				}
			}
		}
		//向右搜索
		if (j<4 && !IsMyChess(cMap, i, j + 1))
		{
			score1 = weixie(i, j, i, j + 1, dz, cMap);
			if (score1<score)
				score = score1;
			if (j >= 0 && j<4 && (i == 1 || i == 5 || i == 6 || i == 10))
			{
				for (j1 = j + 1; j1<5; j1++)
				{
					if (cMap[i][j1] == '0')
						continue;
					else if (cMap[i][j1] >= 'A' && cMap[i][j1] <= 'X')
					{
						score1 = weixie(i, j, i, j1, dz, cMap);
						if (score1<score)
							score = score1;
						break;
					}
					else
						break;
				}
			}
		}
	}
	return score;
}
int Oppnextmove(int i, int j, char cMap[12][5], int allmove[200][2])
{
	int k = 0, i1, j1, num = 0, num1, iSaMove[40][2];
	char cmap[12][5];
	if (IsOppChess(cMap, i, j) && !IsBaseCamp(i, j))
	{
		//可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
		if (i>0 && !IsAfterHill(i, j) && !IsOppChess(cMap, i - 1, j) && !IsFilledCamp(cMap, i - 1, j))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j;
			k++;
			if ((j == 0 || j == 4) && i>0 && i<11)
			{
				for (i1 = i - 1; i1>0; i1--)
				{
					if (cMap[i1][j] == '0')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
					}
					else if (cMap[i1][j] >= 'a' && cMap[i1][j] <= 'l')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
						break;
					}
					else break;
				}
			}
		}
		//可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
		if (j>0 && !IsOppChess(cMap, i, j - 1) && !IsFilledCamp(cMap, i, j - 1))
		{
			allmove[k][0] = i;
			allmove[k][1] = j - 1;
			k++;
			//是否在1,5,6,10铁道上
			if (i == 1 || i == 5 || i == 6 || i == 10)
			{
				for (int i1 = j - 1; i1 >= 0; i1--)
				{
					if (cMap[i][i1] == '0')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
					}
					else if (cMap[i][i1] >= 'a'&&cMap[i][i1] <= 'l')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
						break;
					}
					else break;
				}
			}
		}
		//可以右移:不在最右列,右侧不是己方棋子,右侧不是被占用的行营
		if (j<4 && !IsOppChess(cMap, i, j + 1) && !IsFilledCamp(cMap, i, j + 1))
		{
			allmove[k][0] = i;
			allmove[k][1] = j + 1;
			k++;
			//是否在1,5,6,10铁道上
			if (i == 1 || i == 5 || i == 6 || i == 10)
			{
				for (int i1 = j + 1; i1<5; i1++)
				{
					if (cMap[i][i1] == '0')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
					}
					else if (cMap[i][i1] >= 'a'&&cMap[i][i1] <= 'l')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
						break;
					}
					else break;
				}
			}
		}

		//可以后移：不在最后一列，后方不是己方旗子，后面不是已占有的行营
		if (i<11 && !IsBehindHill(i, j) && !IsOppChess(cMap, i + 1, j) && !IsFilledCamp(cMap, i + 1, j))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j;

			k++;
			//可以在铁轨两侧向后移动，后方不是己方旗子，不是第一行，不在最后一行
			if ((j == 0 || j == 4) && i>0 && i<11)
			{
				for (i1 = i + 1; i1<11; i1++)
				{
					if (cMap[i1][j] == '0')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;

						k++;
					}
					else if (cMap[i1][j] >= 'a' && cMap[i1][j] <= 'l')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
						break;
					}
					else  break;
				}

			}
		}
		//可以左前移：左前方不是已占有的行营，不是己方旗子
		if ((Iszuoqian(i, j) || IsMoveCamp(i, j)) && !IsOppChess(cMap, i - 1, j - 1) && !IsFilledCamp(cMap, i - 1, j - 1))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j - 1;

			k++;
		}
		//可以左后移：左后方不是己方旗子，不是已占有的行营
		if ((Iszuohou(i, j) || IsMoveCamp(i, j)) && !IsOppChess(cMap, i + 1, j - 1) && !IsFilledCamp(cMap, i + 1, j - 1))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j - 1;

			k++;
		}
		//可以右前移：右前方不是己方旗子，不是已占有的行营
		if ((Isyouqian(i, j) || IsMoveCamp(i, j)) && !IsOppChess(cMap, i - 1, j + 1) && !IsFilledCamp(cMap, i - 1, j + 1))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j + 1;

			k++;
		}
		//可以右后移：右后方不是己方旗子，不是已占有的行营
		if ((Isyouhou(i, j) || IsMoveCamp(i, j)) && !IsOppChess(cMap, i + 1, j + 1) && !IsFilledCamp(cMap, i + 1, j + 1))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j + 1;
			k++;
		}
	}

	return k;
}
//己方某个位置的棋子的下一步行棋的所有位置
int nextmove1(int i, int j, char cMap[12][5], int allmove[200][2])
{
	int k = 0, i1, j1, num = 0, num1, iSaMove[40][2];
	char cmap[12][5];
	if (IsMyMovingChess(cMap, i, j) && !IsBaseCamp(i, j))  //己方不在大本营的可移动棋子
	{
		//工兵形棋
		if (cMap[i][j] == 'i' && IsRailway(i, j))
		{
			iSaMove[0][0] = i;
			iSaMove[0][1] = j;
			for (i1 = 0; i1<12; i1++)
			{
				for (j1 = 0; j1<5; j1++)
					cmap[i1][j1] = cMap[i1][j1];
			}
			num = SapperRail(iSaMove, cmap, i, j, 0);
			for (num1 = 1; num1<num; num1++)
			{
				allmove[k][0] = iSaMove[num1][0];
				allmove[k][1] = iSaMove[num1][1];
				k++;
			}
		}
		//可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
		if (i>0 && !IsAfterHill(i, j) && !IsMyChess(cMap, i - 1, j) && !IsFilledCamp(cMap, i - 1, j))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j;
			k++;
			if ((j == 0 || j == 4) && i>0 && i<11)
			{
				for (i1 = i - 1; i1>0; i1--)
				{
					if (cMap[i1][j] == '0')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
					}
					else if (cMap[i1][j] >= 'A' && cMap[i1][j] == 'X')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
						break;
					}
					else break;
				}
			}
		}
		//可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
		if (j>0 && !IsMyChess(cMap, i, j - 1) && !IsFilledCamp(cMap, i, j - 1))
		{
			allmove[k][0] = i;
			allmove[k][1] = j - 1;
			k++;
			//是否在1,5,6,10铁道上
			if (i == 1 || i == 5 || i == 6 || i == 10)
			{
				for (int i1 = j - 1; i1 >= 0; i1--)
				{
					if (cMap[i][i1] == '0')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
					}
					else if (cMap[i][i1] >= 'A'&&cMap[i][i1] <= 'X')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
						break;
					}
					else break;
				}
			}
		}
		//可以右移:不在最右列,右侧不是己方棋子,右侧不是被占用的行营
		if (j<4 && !IsMyChess(cMap, i, j + 1) && !IsFilledCamp(cMap, i, j + 1))
		{
			allmove[k][0] = i;
			allmove[k][1] = j + 1;
			k++;
			//是否在1,5,6,10铁道上
			if (i == 1 || i == 5 || i == 6 || i == 10)
			{
				for (int i1 = j + 1; i1<5; i1++)
				{
					if (cMap[i][i1] == '0')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
					}
					else if (cMap[i][i1] >= 'A'&&cMap[i][i1] <= 'X')
					{
						allmove[k][0] = i;
						allmove[k][1] = i1;
						k++;
						break;
					}
					else break;
				}
			}
		}

		//可以后移：不在最后一列，后方不是己方旗子，后面不是已占有的行营
		if (i<11 && !IsBehindHill(i, j) && !IsMyChess(cMap, i + 1, j) && !IsFilledCamp(cMap, i + 1, j))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j;

			k++;
			//可以在铁轨两侧向后移动，后方不是己方旗子，不是第一行，不在最后一行
			if ((j == 0 || j == 4) && i>0 && i<11)
			{
				for (i1 = i + 1; i1<11; i1++)
				{
					if (cMap[i1][j] == '0')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;

						k++;
					}
					else if (cMap[i1][j] >= 'A' && cMap[i1][j] <= 'X')
					{
						allmove[k][0] = i1;
						allmove[k][1] = j;
						k++;
						break;
					}
					else  break;
				}

			}
		}
		//可以左前移：左前方不是已占有的行营，不是己方旗子
		if ((Iszuoqian(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i - 1, j - 1) && !IsFilledCamp(cMap, i - 1, j - 1))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j - 1;

			k++;
		}
		//可以左后移：左后方不是己方旗子，不是已占有的行营
		if ((Iszuohou(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i + 1, j - 1) && !IsFilledCamp(cMap, i + 1, j - 1))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j - 1;

			k++;
		}
		//可以右前移：右前方不是己方旗子，不是已占有的行营
		if ((Isyouqian(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i - 1, j + 1) && !IsFilledCamp(cMap, i - 1, j + 1))
		{
			allmove[k][0] = i - 1;
			allmove[k][1] = j + 1;

			k++;
		}
		//可以右后移：右后方不是己方旗子，不是已占有的行营
		if ((Isyouhou(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i + 1, j + 1) && !IsFilledCamp(cMap, i + 1, j + 1))
		{
			allmove[k][0] = i + 1;
			allmove[k][1] = j + 1;
			k++;
		}
	}
	return k;
}

void mychessdx(char cMap[12][5], char mc[25])   //将我方棋子按照棋力的大小排序
{												//找到我方棋子除了司令最大的两个
	int i = 0, l;
	for (int i2 = 0; i2<12; i2++)  // 找己方炸弹的数目
	{
		for (int j2 = 0; j2<5; j2++)
			if (cMap[i2][j2] >= 'a' && cMap[i2][j2] <= 'l')
			{
				mc[i] = cMap[i2][j2];
				i++;
				l = i;
			}
	}
	for (i = 0; i<l; i++)
	{
		for (int j = 0; j + i<l - 1; j++)
		{
			if (mc[j]>mc[j + 1])
			{
				char flag = mc[j];
				mc[j] = mc[j + 1];
				mc[j + 1] = flag;
			}
		}
	}
}

void jushipg(char cMap[12][5], int dz[25][14])
{
	if (cMap[11][1] == 'l')    ///己方军旗的位置
	{
		qipanfen[6][0] = 130;
		qipanfen[7][0] = 120;
		qipanfen[7][1] = 600;
		qipanfen[8][0] = 120;
		qipanfen[8][1] = 80;
		qipanfen[9][0] = 120;
		qipanfen[9][1] = 700;
		qipanfen[10][0] = 600;
		qipanfen[10][1] = 200;
	}
	if (cMap[11][3] == 'l')
	{
		qipanfen[6][4] = 130;
		qipanfen[7][4] = 120;
		qipanfen[7][3] = 600;
		qipanfen[8][4] = 120;
		qipanfen[8][3] = 80;
		qipanfen[9][4] = 120;
		qipanfen[9][3] = 700;
		qipanfen[10][4] = 600;
		qipanfen[10][3] = 200;
	}
	if (dz[1][11] == 10000)  //敌方军旗的位置
	{
		qipanfen[0][3] = -10000;
		qipanfen[0][4] = -10000;
		qipanfen[0][1] = 10000;
		qipanfen[0][0] = 600;
		qipanfen[1][0] = 400;
		qipanfen[1][1] = 700;
		qipanfen[2][0] = 300;
		qipanfen[2][1] = 900;
		qipanfen[3][0] = 240;
		qipanfen[3][1] = 100;
		qipanfen[4][0] = 230;
		qipanfen[4][1] = 500;
		qipanfen[5][0] = 220;
	}
	if (dz[3][11] == 10000)
	{
		qipanfen[0][1] = -10000;
		qipanfen[0][0] = -10000;
		qipanfen[0][3] = 10000;
		qipanfen[0][4] = 600;
		qipanfen[1][4] = 400;
		qipanfen[1][3] = 700;
		qipanfen[2][4] = 300;
		qipanfen[2][3] = 900;
		qipanfen[3][4] = 240;
		qipanfen[3][3] = 100;
		qipanfen[4][4] = 230;
		qipanfen[4][3] = 500;
		qipanfen[5][4] = 220;
	}
}
int walei1(char cMap[12][5], int dz[25][14])
{
	int k = 0;
	if (cMap[1][0] == '0' || (cMap[1][0] == 'X' && Judge(1, 0, 9, dz)>5000))
	{
		if (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000))
			k = 1;
		if (cMap[0][0] == '0' || (cMap[0][0] == 'X' && Judge(0, 0, 9, dz)>5000))
			k = 1;
	}
	return k;
}
int walei3(char cMap[12][5], int dz[25][14])
{
	int k = 0;
	if (cMap[1][4] == '0' || (cMap[1][4] == 'X' && Judge(1, 4, 9, dz)>5000))
	{
		if (cMap[1][3] == '0' || (cMap[1][3] == 'X' && Judge(1, 3, 9, dz)>5000))
			k = 1;
		if (cMap[0][4] == '0' || (cMap[0][4] == 'X' && Judge(0, 4, 9, dz)>5000))
			k = 1;
	}
	return k;
}
int walei(char cMap[12][5], int dz[25][14])//判断敌方军旗附近是否有地雷
{
	int k = 0;
	if (dz[1][11]>9000)
	{
		if (cMap[1][0] == '0' || (cMap[1][0] == 'X' && Judge(1, 0, 9, dz)>5000))
		{
			if (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000))
				k = 1;
			if (cMap[0][0] == '0' || (cMap[0][0] == 'X' && Judge(0, 0, 9, dz)>5000))
				k = 1;
		}
		if ((cMap[1][4] == '0' || (cMap[1][4] == 'X' && Judge(1, 4, 9, dz)>5000)) && (cMap[1][3] == '0' || (cMap[1][3] == 'X' && Judge(1, 3, 9, dz)>5000)) && (cMap[1][2] == '0' || (cMap[1][2] == 'X' && Judge(1, 2, 9, dz)>5000)) && (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000)))
			k = 1;
	}
	else if (dz[3][11]>9000)
	{
		if (cMap[1][4] == '0' || (cMap[1][4] == 'X' && Judge(1, 4, 9, dz)>5000))
		{
			if (cMap[1][4] == '0' || (cMap[1][4] == 'X' && Judge(1, 4, 9, dz)>5000))
				k = 1;
			if (cMap[0][4] == '0' || (cMap[0][4] == 'X' && Judge(0, 4, 9, dz)>5000))
				k = 1;
		}
		if ((cMap[1][0] == '0' || (cMap[1][0] == 'X' && Judge(1, 0, 9, dz)>5000)) && (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000)) && (cMap[1][2] == '0' || (cMap[1][2] == 'X' && Judge(1, 2, 9, dz)>5000)) && (cMap[1][3] == '0' || (cMap[1][3] == 'X' && Judge(1, 3, 9, dz)>5000)))
			k = 1;
	}
	else
	{
		if (walei1(cMap, dz) && walei3(cMap, dz))
			k = 1;
		if ((cMap[1][4] == '0' || (cMap[1][4] == 'X' && Judge(1, 4, 9, dz)>5000)) && (cMap[1][3] == '0' || (cMap[1][3] == 'X' && Judge(1, 3, 9, dz)>5000)) && (cMap[1][2] == '0' || (cMap[1][2] == 'X' && Judge(1, 2, 9, dz)>5000)) && (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000)))
			k = 1;
		if ((cMap[1][0] == '0' || (cMap[1][0] == 'X' && Judge(1, 0, 9, dz)>5000)) && (cMap[1][1] == '0' || (cMap[1][1] == 'X' && Judge(1, 1, 9, dz)>5000)) && (cMap[1][2] == '0' || (cMap[1][2] == 'X' && Judge(1, 2, 9, dz)>5000)) && (cMap[1][3] == '0' || (cMap[1][3] == 'X' && Judge(1, 3, 9, dz)>5000)))
			k = 1;
	}

	return k;
}
int DefendFlag(char cMap[12][5], int i0, int j0, int i1, int j1)
{
	int i = 10, j, Score = 10000000;
	char ChessRank[25];
	if (cMap[11][1] == 'l')
		j = 1;
	else
		j = 3;
	if ((IsOppChess(cMap, i, j)) && i == i1 && j == j1 || (IsOppChess(cMap, i, 2) && i1 == i && j1 == 2))
	{
		mychessdx(cMap, ChessRank);
		for (int k = 0; k < NumChess(cMap); k++)
			if (cMap[i0][j0] == ChessRank[k])
				return Score - k;
	}
	return 0;
}
int DefendLeft(int i0, int j0, int i1, int j1, char cMap[12][5])//堵左翼
{
	int i = 6, j = 0, Score = 20000;
	char ChessRanking[25];
	if (cMap[i0][j0] == 'k')
		return 0;
	if (cMap[7][1] == 'k' || cMap[7][0] == 'k')
	{
		if (i0 == i && j0 == j)
			return -50000;
		if (cMap[i][j] == '0' && i1 == i && j1 == j)
		{
			mychessdx(cMap, ChessRanking);
			for (int k = 0; k < NumChess(cMap); k++)
				if (cMap[i0][j0] == ChessRanking[k])

					return Score - k;
		}
	}
	return 0;
}
int DefendRight(int i0, int j0, int i1, int j1, char cMap[12][5])//堵右翼
{
	int i = 6, j = 4, Score = 50000;
	char ChessRanking[25];
	if (cMap[7][3] == 'k')
	{
		if (i0 == i && j0 == j)
			return -500000;
		if (cMap[i][j] == '0' && i1 == i && j1 == j)
		{
			mychessdx(cMap, ChessRanking);
			for (int k = 0; k < NumChess(cMap); k++)
				if (cMap[i0][j0] == ChessRanking[k])

					return Score - k;
		}
	}
	return 0;
}
int AttackFor31(char cMap[12][5], int i0, int j0, int i1, int j1)
{
	if (Ina_step[0] == 2 && Ina_step[1] > 15)
	{
		if (i0 == 7 && j0 == 0 && cMap[i0][j0] != 'k'&&i1 > 7 && j1 == 0)
			return 50000;
		if (IsMyChess(cMap, 6, 0))
			if (cMap[i0][j0] == 'k' && j1 == 0 && i1 == 7 && cMap[7][0] != 'X')
				return 50000;
		if (i0 == 6 && j0 == 0)
		{
			bool Flag = 0;
			for (int i = 8; i>6; i--)
				if (cMap[i][0] == 'k')
				{
					Flag = 1;
					break;
				}
			if (Flag)
			{
				if (IsOppChess(cMap, i1, j1) && j1 == 0)
					return 80000;
			}
		}
	}
	return 0;
}


int pinggu(int i, int j, int i1, int j1, int dz[25][14], char cMap[12][5])
{
	int zha = 0;
	char mchess[25];
	int score = 0, weixief = 0, qipan = 0, jweixief = 0, sanyif = 0, teshu = 0, score1 = 0;
	bool sanyi1 = false, sanyi2 = false, sanyi3 = false;
	char cmap[12][5], cmap1[12][5];
	int k, k1, k2, k3, i2, j2;
	int allmove1[200][2], allmove2[200][2];
	int x = 0;
	int i3, j3;
	int flag = 0, flag1, flag2 = 0;
	FILE *fp;
	fp = fopen("1", "a");
	for (i2 = 0; i2<12; i2++)
	{
		for (j2 = 0; j2<5; j2++)
			cmap[i2][j2] = cMap[i2][j2];
	}
	cmap[i1][j1] = cmap[i][j];
	cmap[i][j] = '0';

	if (dz[1][11] == 0)
		cMap[0][3] = 'L';
	if (dz[3][11] == 0)
		cMap[0][1] = 'L';
	//    碰军旗        。。。。。。。。。。。。。。。。。。。。。。。。。。。。。

	mychessdx(cMap, mchess);
	if (cMap[i][j] <= 'e' || cMap[i][j] == 'k')//己方的大于旅长的棋子，和炸弹不碰己方大本营的棋子
	{
		if (cMap[11][1] == 'l' && i1 == 11 && j1 == 3)
			teshu = teshu - 20000;
		if (cMap[11][3] == 'l' && i1 == 11 && j1 == 1)
			teshu = teshu - 20000;
	}
	fprintf(fp, "\n己方的大于旅长的棋子，和炸弹不碰己方大本营的棋子 %d ", teshu);
	////////////////////////////////////////////////////保护军旗

	if (cMap[11][1] == 'l')
	{
		if (cMap[10][1] == 'X' && i1 == 10 && j1 == 1)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
		if (cMap[10][0] == 'X' && cMap[11][0] == '0' && i1 == 10 && j1 == 0)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
		if (cMap[10][0] == 'X' && cMap[11][0] == 'j' && Judge(10, 0, 8, dz)>6000)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
	}
	if (cMap[11][3] == 'l')
	{
		if (cMap[10][3] == 'X' && i1 == 10 && j1 == 3)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
		if (cMap[10][4] == 'X' && cMap[11][4] == '0' && i1 == 10 && j1 == 4)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
		if (cMap[10][4] == 'X' && cMap[11][4] == 'j' && Judge(10, 4, 8, dz)>6000)
			teshu = teshu + (13 - (cMap[i][j] - 'a')) * 500;
	}
	fprintf(fp, "\n保护军旗影响后的特殊分 %d ", teshu);
	/////////////////////////////////////////////////////////////////
	//特殊行棋
//*************************************
//5.位于中路铁轨的棋子直接对敌方空缺行营的攻击     
/*int step = 0;//555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555
if (IsFilledCamp(cMap, 4, 1) == 0 && IsFilledCamp(cMap, 4, 3) == 0)
{
	if (IsMyChess(cMap, 6, 2) == 1 && i1 == 5 && j1 == 2)
	{
		teshu = teshu + 100000;
		step++;
	}
}
if (IsMyChess(cMap, 5, 2) == 1)
{
	if (IsOppChess(cMap, 5, 1) == 1 && i1 == 5 && j1 == 1)
		teshu = teshu + 50000;
}
if (IsMyChess(cMap, 5, 1) == 1)
{
	if (IsOppChess(cMap, 5, 0) == 1 && i1 == 5 && j1 == 0)
		teshu = teshu + 50000;
}
*/
//当我方棋子在对面行营的时候，求求您别动弹了，您不是他的对手
if ((IsMyChess(cMap, 4, 1) == 1 && cMap[4][1] < 'e') || (IsMyChess(cMap, 4, 3) == 1 && cMap[4][1] < 'e'))
{
	for (i1 = 3; i1 < 6; i1++)
		for (j1 = 0; j1 < 5; j1++)
			teshu = teshu - 500;
}
//6.针对大优势炸弹的利用  (防止炸弹回营不动）
/*int nomovechess = 2;
int leishu = 3;
if (IsMyChess(cMap, 0, 1) == 1 || IsMyChess(cMap, 0, 3) == 1)
nomovechess--;
nomovechess ==nomovechess+ leishu;*/
if (NumChess(cMap) - NumOppChess(cMap) >= 6)  //具有较大优势
{
	int boom = 0;//炸弹数量
	for (i = 0; i < 12; i++)
		for (j = 0; j < 5; j++)
			if (cMap[i][j] == 'k')
				boom++;

	if (cMap[7][3] == 'k'&& cMap[7][1] == 'k')  //强制炸弹出行营(我方两个炸弹都存在）
	{
		int flag = 0;    //判断最优炸弹出行营位置
		int oppnum = 0;  //判断炸弹前进的路线的敌方数量
		int oppnum1[10][2], oppnum2[10][2], oppnum3[10][2], m = 0;
		for (j = 0; j < 4; j++)
		{
			if (IsOppChess(cMap, 6, j) == 1)
			{
				oppnum++;
				oppnum1[m][0] = i;
				oppnum1[m][1] = j;
				m++;
			}

		}
		if (oppnum == 1)
		{
			if (Judge(oppnum1[0][0], oppnum1[0][1], 0, dz)>=6000)
			{
				if (oppnum1[0][1] <= 2)
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == oppnum1[0][0] && j1 == oppnum1[0][1])
						teshu = teshu + 5000;
				if (oppnum1[0][1] > 2)
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == oppnum1[0][0] && j1 == oppnum1[0][1])
						teshu = teshu + 5000;
			}
			if (Judge(oppnum1[0][0], oppnum1[0][1], 1, dz)>=6000)
			{
				if (oppnum1[0][1] <= 2)
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == oppnum1[0][0] && j1 == oppnum1[0][1])
						teshu = teshu + 5000;
				if (oppnum1[0][1] > 2)
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == oppnum1[0][0] && j1 == oppnum1[0][1])
						teshu = teshu + 5000;
			}
		}
		m = 0;
		oppnum = 0;
		flag = 0;
		for (i = 9; i >= 1; i--)
		{
			if (IsOppChess(cMap, i, 0) == 1)
			{
				oppnum++;
				oppnum2[m][0] = i;
				oppnum2[m][1] = j;
				m++;
			}
		}
		k = Oppnextmove(oppnum2[0][0], oppnum2[0][1], cMap, allmove2);
		for (k1 = 0; k1 < k; k1++)
			if (cMap[allmove2[k1][0]][allmove2[k1][1]] == 'l')
				flag = 1;  //判断对方左侧棋子对我方军旗是否存在着威胁
		if (flag == 0 && IsMyChess(cMap, 9, 1) == 1)
		{
			if (Judge(oppnum2[0][0], oppnum2[0][1], 0, dz) >= 6000)
			{
				if (oppnum2[0][0] == 6 || oppnum2[0][0] == 7 || oppnum2[0][0] == 8)
				{
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == oppnum2[0][0] && j1 == 0)
						teshu = teshu + 5000;
				}
				else
				{
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == 7 && j1 == 0)
						teshu = teshu + 4000;
				}
			}
			if (Judge(oppnum2[0][0], oppnum2[0][1], 1, dz) >= 6000)
			{
				if (oppnum2[0][0] == 6 || oppnum2[0][0] == 7 || oppnum2[0][0] == 8)
				{
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == oppnum2[0][0] && j1 == 0)
						teshu = teshu + 5000;
				}
				else
				{
					if (IsMyChess(cMap, 7, 1) == 1 && i1 == 7 && j1 == 0)
						teshu = teshu + 4000;
				}
			}
		}

		m = 0;
		oppnum = 0;
		flag = 0;
		for (i = 9; i >= 1; i--)
		{
			if (IsOppChess(cMap, i, 4) == 1)
			{
				oppnum++;
				oppnum2[m][0] = i;
				oppnum2[m][1] = j;
				m++;
			}
		}
		k = Oppnextmove(oppnum2[0][0], oppnum2[0][1], cMap, allmove2);
		for (k1 = 0; k1 < k; k1++)
			if (cMap[allmove2[k1][0]][allmove2[k1][1]] == 'l')
				flag = 1;  //判断对方右侧棋子对我方军旗是否存在着威胁
		if (flag == 0 && IsMyChess(cMap, 9, 3) == 1)
		{
			if (Judge(oppnum2[0][0], oppnum2[0][1], 0, dz) >= 6000)
			{
				if (oppnum2[0][0] == 6 || oppnum2[0][0] == 7 || oppnum2[0][0] == 8)
				{
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == oppnum2[0][0] && j1 == 4)
						teshu = teshu + 5000;
				}
				else
				{
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == 7 && j1 == 4)
						teshu = teshu + 4000;
				}
			}
			if (Judge(oppnum2[0][0], oppnum2[0][1], 1, dz) >= 6000)
			{
				if (oppnum2[0][0] == 6 || oppnum2[0][0] == 7 || oppnum2[0][0] == 8)
				{
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == oppnum2[0][0] && j1 == 4)
						teshu = teshu + 5000;
				}
				else
				{
					if (IsMyChess(cMap, 7, 3) == 1 && i1 == 7 && j1 == 4)
						teshu = teshu + 4000;
				}
			}
		}
	

	/*for (i = 1; i <= 9; i++)
	{
		if (IsOppChess(cMap, i, 0) == 1 && Judge(i, 0, 0, dz) > 7000)
			flag = 2;
		if (IsOppChess(cMap, i, 0) == 1 && Judge(i, 0, 1, dz) > 7000)
			flag = 2;
		if (flag == 2 && j1 == 0)
			teshu = teshu + 3500;
	}
	if (IsMyChess(cMap, 7, 3) == 1 && cMap[7][3] == 'k')  //强制炸弹出行营
	{
		int flag = 0;
		for (j = 0; j < 4; j++)
		{
			if (IsOppChess(cMap, 6, j) == 1 && Judge(6, j, 0, dz) > 8000)
				flag = 1;
			if (IsOppChess(cMap, 6, j) == 1 && Judge(6, j, 1, dz) > 8000)
				flag = 1;
		}
		if (flag == 1 && i1 == 6)
			teshu = teshu + 2000;
		for (i = 1; i <= 9; i++)
		{
			if (IsOppChess(cMap, i, 4) == 1 && Judge(i, 4, 0, dz) > 8000)
				flag = 2;
			if (IsOppChess(cMap, i, 4) == 1 && Judge(i, 4, 1, dz) > 8000)
				flag = 2;
		}
		if (flag == 2 && j1 == 4)
			teshu = teshu + 2500;
	}*/
	if (boom != 0)  //炸弹主动出击
	{
		flag = 0;
		for(i=0;i<12;i++)
			for (j = 0; j < 5; j++)
			{
				if(cMap[i][j]='k')
					if ((i != 7 && j != 1) && (i != 7 && j != 3) && (i != 8 && j != 2) && (i != 9 && j != 1) && (i != 9 && j != 3))
					{
						flag = 1;
					}
			}
		if(flag==1)
			for (i = 0; i<12; i++)
				for (j = 0; j < 5; j++)
					if (IsOppChess(cMap, i, j) == 1)
					{
						k1 = Oppnextmove(i, j, cMap, allmove2);
						for (k1 = 0; k1 < k; k1++)
						{
							if (cMap[allmove2[k1][0]][allmove2[k1][1]] == 'k')
								if (Judge(i, j, 0, dz) >= 5000 && i1 == 1 && j1 == j)
									teshu = teshu + 5000;
							if (cMap[allmove2[k1][0]][allmove2[k1][1]] == 'k')
								if (Judge(i, j, 1, dz) >= 5000 && i1 == 1 && j1 == j)
									teshu = teshu + 5000;
						}
					}
		/*for (i = 0; i < 12; i++)
			for (j = 0; j < 5; j++)
			{
				if (cMap[i][j] == 'k'&&i1 == 7, j1 == 1)
					teshu = teshu - 5000;
				if (cMap[i][j] == 'k'&&i1 == 7, j1 == 3)
					teshu = teshu - 5000;
				if (cMap[i][j] == 'k'&&i1 == 9, j1 == 1)
					teshu = teshu - 5000;
				if (cMap[i][j] == 'k'&&i1 == 9, j1 == 3)
					teshu = teshu - 5000;
				for (i2 = 0; i2 < 12; i2++)                         //出现炸弹让营时会夺回行营
					for (j2 = 0; j2 < 5; j2++)
					{
						k = Oppnextmove(i, j, cMap, allmove2);
						for (k1 = 0; k1 <= k; k1++)
						{

							if (i1 == 7 && j1 == 1)
								teshu = teshu + 6000;
							if (i1 == 7 && j1 == 3)
								teshu = teshu + 6000;
							if (i1 == 9 && j1 == 1)
								teshu = teshu + 6000;
							if (i1 == 9 && j1 == 3)
								teshu = teshu + 6000;
						}
					}
			}*/

	}
	}
	if (IsMyChess(cMap, 7, 1) && cMap[7][1] == 'k'&&j1 == 0)  //敌方军长司令出现在左右两侧的炸弹跟踪
	{
		for (i = 1; i < 11; i++)
		{
			if (IsOppChess(cMap, i, 0) == 1 && Judge(i, 0, 0, dz) > 6000)
				teshu = teshu + 3000;
			if (IsOppChess(cMap, i, 0) == 1 && Judge(i, 0, 1, dz) > 6000)
				teshu = teshu + 3000;

		}
	}
	if (IsMyChess(cMap, 7, 3) && cMap[7][3] == 'k'&&j1 == 4)
	{
		for (i = 1; i < 11; i++)
		{
			if (IsOppChess(cMap, i, 4) == 1 && Judge(i, 4, 0, dz) > 6000)
				teshu = teshu + 3000;
			if (IsOppChess(cMap, i, 4) == 1 && Judge(i, 4, 1, dz) > 6000)
				teshu = teshu + 3000;

		}
	}
	if (IsMyChess(cMap, 7, 1) == 1 && cMap[7][1] == 'k'&&i1 == 6)
		for (j = 0; j < 4; j++)
		{
			if (IsOppChess(cMap, 6, j) == 1 && Judge(6, 4, 0, dz) > 6000)
				teshu = teshu + 3500;
			if (IsOppChess(cMap, 6, j) == 1 && Judge(6, 4, 1, dz) > 6000)
				teshu = teshu + 3500;
		}
	//我方司令防守时，别TM给我出去浪
	//对不起，麻烦您如果里面不是司令，也别出去浪行吗，求求您了，给您跪下了
		for (i = 0; i<11; i++)
	for (j = 0; j < 4; j++)
	{
	k = Oppnextmove(i, j, cMap, allmove2);
	for (k1 = 0; k1 < k; k1++)
	{
		if (allmove2[k][0] == 9 && allmove2[k][1] == 1 && i == 8 && j == 1)

			if (IsMyChess(cMap,9,1)==1)
				for (i1 = 0; i1 < 11; i1++)
					for (j1 = 0; j1 < 5; j1++)
						teshu = teshu - 3000;
	if (allmove2[k][0] == 9 && allmove2[k][1] == 1 && i == 8 && j == 1)
		if (IsMyChess(cMap, 9, 1) == 1)
			for (i1 = 0; i1 < 11; i1++)
				for (j1 = 0; j1 < 5; j1++)
					teshu = teshu - 3000;
	if (allmove2[k][0] == 9 && allmove2[k][1] == 1 && i == 8 && j == 1)

		if (IsMyChess(cMap, 9, 1) == 1)
			for (i1 = 0; i1 < 11; i1++)
				for (j1 = 0; j1 < 5; j1++)
					teshu = teshu - 3000;
	}
	}
	}
	if (IsOppChess(cMap, 10, 0) == 1 && cMap[11][0] == '0'&&IsMyChess(cMap, 9, 1) == 1 && i1 == 10 && j1 == 0)
	teshu = teshu + 3500;
	if (IsOppChess(cMap, 10, 4) == 1 && cMap[11][4] == '0'&&IsMyChess(cMap, 9, 3) == 1 && i1 == 10 && j1 == 4)
	{
	for(i=1;i<10;i++)
	if(IsOppChess(cMap,i,4)==1&&Judge(i,4,10,dz)<3000)
	teshu = teshu + 3500;
	}


	//7.敌方军旗暴露的进攻  
	if (NumOppChess(cMap) <= 10)
	{
		if (cMap[0][1] == 'L')
		{
			k = nextmove1(i1, j1, cMap, allmove1);
			for (k1 = 0; k1 < k; k++)
			{
				if (allmove1[k][0] == 1 && allmove1[k][1] == 1)
					teshu = teshu + 20000;
				if (allmove1[k][0] == 0 && allmove1[k][1] == 1)
					teshu = teshu + 30000;
			}
		}
		if (cMap[0][3] == 'L')
		{

			k = nextmove1(i1, j1, cMap, allmove1);
			for (k1 = 0; k1 < k; k++)
			{
				if (allmove1[k][0] == 1 && allmove1[k][1] == 3)
					teshu = teshu + 20000;
				if (allmove1[k][0] == 0 && allmove1[k][1] == 3)
					teshu = teshu + 30000;
			}
		}

	}
	//不知道为啥司令和军长喜欢到最下面去的特判QAQ
	if (cMap[10][0] == 'a'&&cMap[11][0] == '0'&&i1 == 11, j1 == 0 && cMap[11][0] == '0')
		teshu = teshu - 8000;
	if (cMap[10][4] == 'b'&&cMap[11][4] == '0'&&i1 == 11, j1 == 4 && cMap[11][0] == '0')
		teshu = teshu - 8000;
	if (cMap[10][0] == 'b'&&cMap[11][0] == '0'&&i1 == 11, j1 == 0 && cMap[11][0] == '0')
		teshu = teshu - 8000;
	if (cMap[10][4] == 'a'&&cMap[11][4] == '0'&&i1 == 11, j1 == 4 && cMap[11][0] == '0')
		teshu = teshu - 8000;
	//1.司令碰子回营
	mychessdx(cMap, mchess);
	if (cMap[i][j] == 'a')
	{
		if (i == 6 && j == 4)
		{
			if ((huang1 == 1 && i1 == 7 && j1 == 3) || (mchess[1] != 'b'&& i1 == 7 && j1 == 3 && cMap[6][2] == 'X'))
				teshu += 100000;
			else
				teshu -= 100000;
		}
	}
	if (cMap[6][2] == 'X'&&cMap[7][3] == 'a'&&mchess[1] != 'b')//对方强攻，吃子，司令回1营
	{
		if (i == 7 && j == 3 && i1 == 6 && j1 == 2)
		{
			teshu += 10000;
			huang2 = 1;
		}
	}
	if (cMap[6][2] == 'a')
	{
		if (i == 6 && j == 2 && i1 == 7 && j1 == 3)
			teshu += 10000;
	}
	if (cMap[7][2] == 'X'&&cMap[7][3] == 'a'&&mchess[1] != 'b')//吃子后，司令落2营
	{
		if (i == 7 && j == 3 && i1 == 7 && j1 == 2)
		{
			teshu += 10000;
			huang2 = 1;
		}
	}
	if (cMap[7][2] == 'a'&&huang2 == 1)
	{
		if (i == 7 && j == 2 && i1 == 8 && j1 == 2)
			teshu += 10000;
	}
	if ((cMap[7][3] == '0') && (cMap[8][2] == 'a'))//司令落2营后，炸弹先行营
	{
		if (cMap[8][3] == 'k'&&i == 8 && j == 3 && i1 == 7 && j1 == 3)
			teshu += 12000;
	}
	if ((mchess[1] != 'b'&& cMap[7][3] == 'a' && cMap[6][2] != 'X' && cMap[7][2] != 'X') || (cMap[7][3] == 'a' && huang1 == 1) || (NumChess(cMap) <= 19))//回2行营
	{
		if (i == 7 && j == 3 && i1 == 8 && j1 == 2)
			teshu += 10000;
	}
	if (cMap[8][2] == 'a')//回3行营
	{
		if (i == 8 && j == 2 && i1 == 9 && j1 == 3)
			if (cMap[7][2] == 'X' && cMap[8][4] != 'X' && cMap[9][4] != 'X' && cMap[8][3] != 'X' && cMap[10][4] != 'X' && cMap[10][3] != 'X' && cMap[10][2] != 'X')
				teshu -= 8000;
			else
				teshu += 8000;
	}
	if (cMap[6][4] == 'a' || cMap[7][3] == 'a' || cMap[8][2] == 'a')//不允许其他棋子占据行营
	{
		if ((i1 == 7 && j1 == 3) || (i1 == 8 && j1 == 2) || (i1 == 9 && j1 == 3))
			teshu -= 8000;
	}
	//我是赵泯杰，我求求代码爸爸了（磨棋的时候不要放弃可以保护军旗的位置）
	if (IsMyChess(cMap, 9, 3) == 1 && i1 == 8 && j1 == 2)
		teshu = teshu - 500;
	if (IsMyChess(cMap, 9, 3) == 1 && i1 == 10 && j1 == 2)
		teshu += 4000;
	/*for (j = 0; j < 5; j++)
	if (IsOppChess(cMap, 10, j) == 1)
	for (j2 = 0; j2 < 5; j2++)
	if (IsMyChess(cMap, i, j2) == 1 && j1 == j)
	teshu = teshu + 4500;*/
	//针对司令死亡后炸弹进入中间行营造成军长无法有效回去的特殊判断
	if (mchess[0] != 'a')
	{
		if (cMap[7][1] == 'k'&&IsMyChess(cMap, 7, 1) == 1 && i1 == 8 && j1 == 2)
			teshu = teshu - 3000;
		if (cMap[7][3] == 'k'&&IsMyChess(cMap, 7, 3) == 1 && i1 == 8 && j1 == 2)
			teshu = teshu - 3000;
		if(mchess[0]=='b'&&cMap[9][3]!='b')
			if (cMap[8][2] = 'k')
			{
				if (cMap[8][1] == '0')
				{
					if (IsMyChess(cMap, 8, 2) == 1 && i1 == 8 && j1 == 1)
						teshu = teshu + 3000;
				}
				if (cMap[8][3] == '0')
				{
					if (IsMyChess(cMap, 8, 2) == 1 && i1 == 8 && j1 == 3)
						teshu = teshu + 3500;
				}
			}
	}
	//我方防守行营的子被对了之后，当对方有可以进攻的子，另一个行营的大子会占据最后一排
	if (cMap[9][3] == '0'&&mchess[0]!='a')
	{
		int flag = 0;
		int i2=0;
		int j2=0;
		for(j=0;j<5;j++)
			if (IsOppChess(cMap, 10, j) == 1)
			{
				i2 = 10;
				j2 = j;
				flag = 1;
				break;
			}
		if (flag == 0)
		{
			if (IsMyChess(cMap, 9, 1) == 1 && i1 == 10 && j1 == 2)
				teshu += 30000;
		}
		if (flag == 1)
		{
			if (IsMyChess(cMap, 9, 1) == 1 && i1 == i2 && j1 == j2)
				teshu += 30000;

		}

	}

	//模拟夺旗
	if (NumOppChess(cMap) <= 10)
	{
		if (cMap[0][1] == 'L')
		{
			k = nextmove1(i1, j1, cMap, allmove1);
			for (k1 = 0; k1 < k; k++)
			{
				if (allmove1[k][0] == 1 && allmove1[k][1] == 1)
					teshu = teshu + 20000;
				if (allmove1[k][0] == 0 && allmove1[k][1] == 1)
					teshu = teshu + 30000;
			}
		}
		if (cMap[0][3] == 'L')
		{

			k = nextmove1(i1, j1, cMap, allmove1);
			for (k1 = 0; k1 < k; k++)
			{
				if (allmove1[k][0] == 1 && allmove1[k][1] == 3)
					teshu = teshu + 20000;
				if (allmove1[k][0] == 0 && allmove1[k][1] == 3)
					teshu = teshu + 30000;
			}
		}

	}
	// 6.强制夺旗                          ////////////////////////////////////////5555555555555555555555555
	
	//2步
	if (cMap[0][1] == 'L')
	{
		if (cMap[i1][j1] == '0'&&i1 == 0 && (j1 == 0 || j1 == 2))
			teshu += 5000;//走下边要比走铁轨有优势
		else if (i1 == 1 && j1 == 1)
			teshu += 3500;//如果下边有子走右边
		else if (j1 == 0)
			teshu += 2500;//靠近铁轨
	}
	else if (cMap[0][3] == 'L')
	{
		if (cMap[i1][j1] == '0'&&i1 == 0 && (j1 == 4 || j1 == 2))
			teshu += 5000;//走下边要比走铁轨有优势
		else if (i1 == 1 && j1 == 1)
			teshu += 3500;//如果下边有子走右边
		else if (cMap[0][3] == 'L'&&j1 == 4)
			teshu += 2500;//靠近铁轨
	}
	//1步
	if (cMap[i1][j1] == 'L')
		teshu += 100000;
	//我方非前二大的子可以去试探一下行营内是否为军旗
	//被打断了。先写别的
	
    //后期三十一步送子的问题(送<?)))><<）
	if (Ina_step[0] == 2 && Ina_step[1] > 10 && Ina_step[1] <= 18&&NumOppChess(cMap)<=18)
	{
		int num;
		num = NumChess(cMap);
		if (cMap[i][j] == mchess[num - 1])     //第一步：小子移动至炸弹前
		{
			if (cMap[7][1] == 'k'&&i1 == 6 && j1 == 0&&cMap[6][0]=='0')
				teshu = teshu + 5000;
			k1 = nextmove1(i1, j1, cMap, allmove1);  //模拟一步无法到达，两步到达
			for (k2 = 0; k2 < k1; k2++)
			{
				if (allmove1[k2][0] == 6 && allmove1[k2][1] == 0&&cMap[6][0]=='0')
					teshu = teshu + 4500;
			}
		}
		int flag = 0;                                     //炸弹出营
		for (i = 7; i <= 11; i++)
			if (IsOppChess(cMap, i, 0) == 1)
				flag = 1;
		if (flag == 0 && cMap[6][0] == mchess[num - 1])
		{
			if (IsMyChess(cMap, 7, 1) == 1 && i1 == 7 & j1 == 0)
				teshu = teshu + 5000;
		}
		if (cMap[6][0] == mchess[num - 1] && cMap[7][0] == 'k')
		{
			if (cMap[1][0] == '0')
				if (IsMyChess(cMap, 6, 0) == 1 && i1 == 1 && j1 == 0)
					teshu = teshu + 5000;
		}
		if(cMap[7][0]=='k')
			for(i=1;i<7;i++)
				if (cMap[i][0] != '0')
				{
					flag = 1;
					break;
				}
		if (flag == 0 && cMap[1][0] == 'X')
			if (IsMyChess(cMap, 7, 0) == 1 && i1 == 1 && j1 == 0)
				teshu = teshu + 5000;


	}

	//2.我方最大棋子守护军棋
	if ((cMap[i][j] == 'a'&&i >= 8 && j >= 2) || (mchess[0] != 'a'))
	{
		if (cMap[i][j] == mchess[0])
		{
			if (cMap[i1][j1] == 'X' && Judge(i1, j1, 10, dz) >= 3000)
				teshu = teshu - 5000;
			if (cMap[11][1] == 'l')
			{
				if (cMap[10][2] == mchess[0] || cMap[10][1] == mchess[0] || cMap[10][0] == mchess[0] || cMap[9][2] == mchess[0] || cMap[9][1] == mchess[0] || cMap[9][0] == mchess[0] || cMap[8][0] == mchess[0] || cMap[8][1] == mchess[0])
					flag2 = 1;		//判断己方棋子是否在可以保护军棋的位置 1在。0不在
				if (i1 == 10 && j1 == 2 && cMap[i1][j1] == 'X')
					teshu = teshu + 5000;
				else if (i1 == 10 && j1 == 1 && cMap[i1][j1] == 'X')
					teshu = teshu + 15000;
				else if ((i1 == 9 || i1 == 8 || i1 == 10) && j1 == 0 && cMap[i1][j1] == 'X')
				{
					for (k1 = i1 - 1; k1 >= 1; k1--)
					{
						if (cMap[k1][j1] == '0')
							continue;
						else if (cMap[k1][j1] == 'X' && Judge(k1, j1, 10, dz) >= 3000)
						{
							flag = 1;
							break;
						}
						else
							break;
					}
					if (flag == 1)
						teshu = teshu - 5000;
					else
						teshu = teshu + 2000;
					if (cMap[10][0] == 'X')
						teshu = teshu + 5000;
				}
				else if (i1 == 8 && j1 == 1 && cMap[i1][j1] == 'X')
					teshu = teshu + 3500;
				else if (i1 == 9 && j1 == 2 && cMap[i1][j1] == 'X')
					teshu = teshu + 3500;
				else if (i1 == 9 && j1 == 1)
					teshu = teshu + 3000;
				else
					teshu = teshu - 2000;
			}
			if (cMap[11][3] == 'l')
			{
				if (cMap[10][2] == mchess[0] || cMap[10][3] == mchess[0] || cMap[10][4] == mchess[0] || cMap[9][2] == mchess[0] || cMap[9][3] == mchess[0] || cMap[9][4] == mchess[0] || cMap[8][3] == mchess[0] || cMap[8][4] == mchess[0])
					flag2 = 1;
				if (i1 == 10 && j1 == 2 && cMap[i1][j1] == 'X')
					teshu = teshu + 5000;
				else if (i1 == 10 && j1 == 3 && cMap[i1][j1] == 'X')
					teshu = teshu + 15000;
				else if (i1 == 9 && j1 == 3)
					teshu = teshu + 3000;
				else if ((i1 == 9 || i1 == 8 || i1 == 10) && j1 == 4 && cMap[i1][j1] == 'X')
				{
					for (k1 = i1 - 1; k1 >= 1; k1--)
					{
						if (cMap[k1][j1] == '0')
							continue;
						else if (cMap[k1][j1] == 'X' && Judge(k1, j1, 10, dz) >= 3000)
						{
							flag = 1;
							break;
						}
						else
							break;
					}
					if (flag == 1)
						teshu = teshu - 5000;
					else
						teshu = teshu + 2000;
					if (cMap[10][4] == 'X')
						teshu = teshu + 5000;

				}
				else if (i1 == 8 && j1 == 3 && cMap[i1][j1] == 'X')
					teshu = teshu + 3500;
				else if (i1 == 9 && j1 == 2 && cMap[i1][j1] == 'X')
					teshu = teshu + 3500;
				else
					teshu = teshu - 2000;
			}
		}

		if (flag2 == 0 && cMap[i][j] == mchess[0])//己方最大棋子不在军旗周围，将棋子走过去
		{
			sanyi1 = false;
			sanyi2 = false;
			sanyi3 = false;
			if (cMap[11][1] == 'l')
			{

				if (i1 == 10 && (j1 == 2 || j1 == 1 || j1 == 0))
				{
					teshu = teshu + 6000;
					sanyi1 = true;
				}
				if (i1 == 9 && j1 == 1)
				{
					teshu = teshu + 6000;
					sanyi1 = true;
				}
			}
			if (cMap[11][3] == 'l')
			{
				if (i1 == 10 && (j1 == 2 || j1 == 3 || j1 == 4))
				{
					teshu = teshu + 6000;
					sanyi1 = true;
				}
				if (i1 == 9 && j1 == 3)
				{
					teshu = teshu + 6000;
					sanyi1 = true;
				}
			}
			if (sanyi1 == false)
			{
				k1 = nextmove1(i1, j1, cmap, allmove1);
				for (k2 = 0; k2 < k1; k2++)
				{
					if (cMap[11][1] == 'l')
					{
						if (allmove1[k2][0] == 10 && (allmove1[k2][1] == 2 || allmove1[k2][1] == 1 || allmove1[k2][1] == 0))
						{
							teshu = teshu + 5000;
							sanyi2 = true;
						}
						if (allmove1[k2][0] == 9 && allmove1[k2][1] == 1)
						{
							teshu = teshu + 5000;
							sanyi2 = true;
						}
					}

					if (cMap[11][3] == 'l')
					{
						if (allmove1[k2][0] == 10 && (allmove1[k2][1] == 2 || allmove1[k2][1] == 3 || allmove1[k2][1] == 4))
						{
							teshu = teshu + 4000;
							sanyi2 = true;
						}
						if (allmove1[k2][0] == 9 && allmove1[k2][1] == 3)
						{
							teshu = teshu + 4000;
							sanyi2 = true;
						}
					}
				}
				if (sanyi2 == false)
				{
					for (k2 = 0; k2 < k1; k2++)
					{
						cmap[allmove1[k2][0]][allmove1[k2][1]] = cmap[i1][j1];
						cmap[i1][j1] = '0';
						k3 = nextmove1(allmove1[k2][0], allmove1[k2][1], cmap, allmove2);
						for (int k4 = 0; k4 < k3; k4++)
						{
							if (cMap[11][1] == 'l')
							{
								if (allmove2[k4][0] == 10 && (allmove2[k4][1] == 2 || allmove2[k4][1] == 1 || allmove2[k4][1] == 0))
								{
									teshu = teshu + 4000;
									sanyi2 = true;
								}
								if (allmove2[k4][0] == 9 && allmove2[k4][1] == 1)
								{
									teshu = teshu + 4000;
									sanyi2 = true;
								}
							}
							if (cMap[11][3] == 'l')
							{
								if (allmove2[k4][0] == 10 && (allmove2[k4][1] == 2 || allmove2[k4][1] == 3 || allmove2[k4][1] == 4))
								{
									teshu = teshu + 4000;
									sanyi2 = true;
								}
								if (allmove2[k4][0] == 9 && allmove2[k4][1] == 3)
								{
									teshu = teshu + 4000;
									sanyi2 = true;
								}
							}
							cmap[i1][j1] = cmap[allmove1[k2][0]][allmove1[k2][1]];
							cmap[allmove1[k2][0]][allmove1[k2][1]] = '0';
						}
					}
				}

			}

		}
	}
	fprintf(fp, "\n最大子移动影响后的特殊分值 %d ", teshu);

	//3.三十一步，炸弹跟随套路
	if (Ina_step[0] == 2 && Ina_step[1]>12 && Ina_step[1] <= 18)
	{


		if (cMap[i][j] >= 'e' && cMap[i][j] < 'i')
		{
			k1 = nextmove1(i1, j1, cmap, allmove1);	//判断棋盘上，己方大于工兵小于旅长的棋子能否在两步之内碰子，
			for (k2 = 0; k2 < k1; k2++)
			{
				if (cmap[allmove1[k2][0]][allmove1[k2][1]] >= 'A' && cmap[allmove1[k2][0]][allmove1[k2][1]] <= 'X')
					flag = 1;
			}
		}
		if (cMap[i1][j1] >= 'A' && cMap[i1][j1] <= 'X')
		{
			if (!IsMoveCamp(i, j) && cMap[i][j] >= 'e' && cMap[i][j] < 'i')//己方不再行营且小于旅长大于排长的棋子。碰子的分值最大
			{
				sanyif = 30000;
				sanyif = sanyif + (cMap[i][j] - 'a') * 10;
				sanyi1 = true;
			}
			else if (cMap[i][j] == mchess[0])//己方工兵不参与三十一步，己方最大棋子不参与三十一步
			{
				sanyif = 0;
				sanyi1 = false;
			}
			else if (cMap[i][j] == 'i')
			{
				if (walei(cMap, dz))//判断工兵是否需要挖雷，不需要则，进行。三十一步
				{
					sanyi1 = true;
					sanyif = 40000;                     //////////////////////////////////////////
														//sanyif=sanyif+(cMap[i][j]-'a')*10;
				}
				else
				{
					sanyif = 0;
					sanyi1 = false;
				}
			}
			else if (IsMoveCamp(i, j) && weixie(i, j, i1, j1, dz, cMap) < 0)//己方在行营的棋子	cMap[i][j]<'d'
			{
				sanyif = 20000;
				sanyif = sanyif + (cMap[i][j] - 'a') * 10;
				sanyi1 = false;
			}
			else if (cMap[i][j] == 'k')//己方在行营小于旅长的棋子	cMap[i][j]<'d'
			{
				sanyif = 20000;
				sanyif = sanyif + (cMap[i][j] - 'a') * 10;
				sanyi1 = false;
			}
			else//己方大于团长的棋子
			{
				sanyi1 = true;
				sanyif = 28000;
				if (Judge(i1, j1, 9, dz) > 4000)
				{
					sanyif = 20000;
					sanyi1 = false;
				}
				sanyif = sanyif + (cMap[i][j] - 'a') * 10;

			}
			//
		}
		if (sanyi1 == false)//第二步能碰到子
		{
			k1 = nextmove1(i1, j1, cmap, allmove1);
			for (k2 = 0; k2 < k1; k2++)
				if (cmap[allmove1[k2][0]][allmove1[k2][1]] >= 'A' && cmap[allmove1[k2][0]][allmove1[k2][1]] <= 'X')
				{
					sanyi2 = true;
					if (cMap[i][j] == mchess[0])//己方工兵不参与三十一步，己方最大棋子不参与三十一步
					{
						sanyif = 0;
						sanyi2 = false;
					}
					else if (cMap[i][j] == 'i')
					{
						if (walei(cMap, dz))//判断工兵是否需要挖雷，不需要则，进行。三十一步
						{
							sanyif = 28000;                   ///////////////////////////////
															  //sanyif=sanyif+(cMap[i][j]-'a')*10;
						}
						else
							sanyif = 0;
					}
					else if (cMap[i][j] == 'k')
					{
						sanyif = 15000;
						sanyif = sanyif + (cMap[i][j] - 'a') * 10;
					}
					else if (cMap[i][j] >= 'e' && cMap[i][j] < 'i')
					{
						sanyif = 25000;
						sanyif = sanyif + (cMap[i][j] - 'a') * 10;
					}
					else//己方大于团长的棋子
					{
						sanyif = 23000;
						if (Judge(allmove1[k2][0], allmove1[k2][1], 9, dz) > 4000)
						{
							sanyif = 15000;
							sanyi2 = false;
						}
						sanyif = sanyif + (cMap[i][j] - 'a') * 10;
					}

				}
			if (sanyi2 == false && flag == 0)				//第三步能碰子
			{
				for (k2 = 0; k2 < k1; k2++)
				{
					cmap[allmove1[k2][0]][allmove1[k2][1]] = cmap[i1][j1];
					cmap[i1][j1] = '0';
					k3 = nextmove1(allmove1[k2][0], allmove1[k2][1], cmap, allmove2);
					for (int k4 = 0; k4 < k3; k4++)
						if (cmap[allmove2[k4][0]][allmove2[k4][1]] >= 'A' && cmap[allmove2[k4][0]][allmove2[k4][1]] <= 'X')
						{
							if (cMap[i][j] >= 'e' && cMap[i][j] < 'i')
							{
								sanyif = 20000;
								sanyif = sanyif + (cMap[i][j] - 'a') * 10;
							}
							else if (cMap[i][j] == mchess[0])
								sanyif = 0;
							else if (cMap[i][j] == 'i')
							{
								if (walei(cMap, dz))//判断工兵是否需要挖雷，不需要则进行。三十一步
								{
									sanyif = 22000;                  //////////////////////////////////////
									sanyif = sanyif + (cMap[i][j] - 'a') * 10;
								}
								else
									sanyif = 0;
							}
							else
							{
								sanyif = 13000;
								sanyif = sanyif + (cMap[i][j] - 'a') * 10;
							}
							sanyi3 = true;

						}
					cmap[i1][j1] = cmap[allmove1[k2][0]][allmove1[k2][1]];
					cmap[allmove1[k2][0]][allmove1[k2][1]] = '0';
				}
			}
		}

		if (NumChess(cMap) < 8 && NumOppChess(cMap) < 8)
			if (cMap[i][j] == 'k')
				sanyif = sanyif + 20000;
		score1 = sanyif + teshu;

	}
	fprintf(fp, "\n三一步影响三一分 %d ", sanyif);
	if (Ina_step[0] == 2 && Ina_step[1]>18)
	{
		k1 = nextmove1(i1, j1, cmap, allmove1);
		for (k2 = 0; k2<k1; k2++)
		{
			if ((allmove1[k2][1] == 0 || allmove1[k2][1] == 4) && (allmove1[k2][0]>0 && allmove1[k2][0]<11))
				sanyif = 25000;
		}
		if (i1>0 && i1<11 && (j1 == 0 || j1 == 4))
			sanyif = 35000;
		if ((j == 0 || j == 4) && (i>0 && i<11))
		{
			if (i1 == 1 && (j1 == 0 || j1 == 4))
				sanyif = 45000;
		}

		if (cMap[i1][j1] == 'X')
		{
			sanyif = 60000;
			if (Judge(i1, j1, 9, dz)>3000)
				sanyif = sanyif - 10000;
		}
		if (((i1 == 1 && (j1 == 1 || j1 == 3)) || (i1 == 0 && (j1 == 0 || j1 == 4))) && Judge(i1, j1, 9, dz)<50)
			sanyif = 55000;
		if (cMap[i][j] == 'k')
			sanyif = sanyif - 1000;
		if ((Judge(1, 1, 9, dz)<50 || Judge(0, 0, 9, dz)<50) && j1 == 0)
			sanyif = sanyif + 500;
		if ((Judge(1, 3, 9, dz)<50 || Judge(0, 4, 9, dz)<50) && j1 == 4)
			sanyif = sanyif + 500;
		if (cMap[0][1] == 'L' && i1 == 1 && j1 == 1)
			sanyif = sanyif + 2000;
		if (cMap[0][3] == 'L' && i1 == 1 && j1 == 3)
			sanyif = sanyif + 2000;
		if (cMap[i][j]>mchess[0] && cMap[i][j]<'j')
			sanyif = sanyif + (cMap[i][j] - 'a') * 10;
		if (NumChess(cMap) < 8 && NumOppChess(cMap) < 8)
			if (cMap[i][j] == 'k')
				sanyif = sanyif + 20000;
		score1 = sanyif + teshu;
	}

	if (Ina_step[0] == 1 && Ina_step[1]>12) //敌方的三十一步进行躲闪
	{
		if (cMap[i][j] != mchess[0] && cMap[i1][j1] != 'X')
		{
			for (int m = 0; m<12; m++)
			{
				for (int n = 0; n<5; n++)
				{
					if (Judge(m, n, 9, dz)<5000)
					{
						k1 = Oppnextmove(m, n, cMap, allmove1);
						for (k2 = 0; k2<k1; k2++)
						{
							if (allmove1[k2][0] == i && allmove1[k2][1] == j)
								sanyi1 = true;
						}
					}
				}
			}
			for (int m = 0; m<12; m++)
			{
				for (int n = 0; n<5; n++)
				{
					if (Judge(m, n, 9, dz)<5000)
					{
						k1 = Oppnextmove(m, n, cmap, allmove1);
						for (k2 = 0; k2<k1; k2++)
						{
							if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
								sanyi2 = true;
						}
					}
				}
			}
			if (sanyi1 == false && sanyi2 == false)
			{
				if (IsMoveCamp(i1, j1) == 1)
					sanyif = 4000;
				else
					sanyif = 3000;
			}
		}
	}
	fprintf(fp, "\n敌方三一步躲闪影响后的三一 分 %d ", sanyif);
	jushipg(cMap, dz);    //改变棋盘的分值


						  //2.阻止大子碰地雷
	if (cMap[i1][j1] == 'X' && i1<2)                ///////////////////////////////
	{
		if (Ina_step[0] == 2 && Ina_step[1] > 12)  //该执行31步
		{
			if (cMap[i][j] < 'e' && Judge(i1, j1, 9, dz)>4000)
				teshu = teshu - 4000;       ///////////////////////////
			if ((cMap[i][j] == 'f' || cMap[i][j] == 'g' || cMap[i][j] == 'h') && Judge(i1, j1, 9, dz) > 4000)
				teshu = teshu - 2000;
		}
		else
		{
			if (cMap[i][j] < 'e' && Judge(i1, j1, 9, dz)>4000)
				teshu = teshu - 6000;       ///////////////////////////
			if ((cMap[i][j] == 'f' || cMap[i][j] == 'g' || cMap[i][j] == 'h') && Judge(i1, j1, 9, dz) > 4000)
				teshu = teshu - 5000;
		}
	}

	fprintf(fp, "\n阻止大子碰雷影响后的特殊分 %d ", teshu);
	//行营外面有敌方棋子，不让行营
	if (IsMoveCamp(i, j) && (cMap[i + 1][j] == 'X' || cMap[i + 1][j + 1] == 'X' || cMap[i + 1][j - 1] == 'X' || cMap[i][j + 1] == 'X' || cMap[i][j - 1] == 'X' || cMap[i - 1][j - 1] == 'X' || cMap[i - 1][j] == 'X' || cMap[i - 1][j + 1] == 'X'))
	{
		if (i == 9 && (j == 1 || j == 3) && cMap[i1][j1] == '0')
			teshu = teshu - 1000;
		else if (i == 8 && j == 2)
			teshu = teshu - 2000;
		else if (cMap[i][j]>'d' && cMap[i1][j1] == 'X')
			teshu = teshu - 800;
		else if (cMap[i][j] <= 'd' && cMap[i1][j1] == 'X')
			teshu = teshu + 500;
		else
			teshu = teshu - 500;
	}
	fprintf(fp, "\n行营外面有敌方棋子，不让行营影响的 特殊分 %d ", teshu);
	//空行营附近有敌方棋子，抢占行营
	if (IsMoveCamp(i1, j1) && (cMap[i1 + 1][j1] == 'X' || cMap[i1 + 1][j1 + 1] == 'X' || cMap[i1 + 1][j1 - 1] == 'X' || cMap[i1][j1 + 1] == 'X' || cMap[i1][j1 - 1] == 'X' || cMap[i1 - 1][j1 - 1] == 'X' || cMap[i1 - 1][j1] == 'X' || cMap[i1 - 1][j1 + 1] == 'X'))
	{
		teshu = teshu + 1000;
	}
	fprintf(fp, "\n空行营附近有敌方棋子，抢占行营 影响后的特殊分 %d ", teshu);
	int flag3 = 0, flag4 = 0;
	//3.炸弹保护和使用
	if (cMap[i][j] == 'k')
	{
		if (cMap[i][j] == 'k')
		{
			if (i == 8 && j == 3)
			{
				if (i1 == 9 && j == 3)
					teshu -= 10000;
				else if (i1 == 8 && j1 == 2)
					teshu -= 10000;
				else if (i1 == 7 && j1 == 3 && cMap[6][4] == 'a')
					teshu -= 100000;
			}
		}
		///////////// 补充炸弹用法
		int  NextNumberOfSteps = nextmove1(i1, j1, cmap, allmove1);
		char mc[25];
		mychessdx(cMap, mc);
		if (Judge(i1, j1, 0, dz) > 6000 || Judge(i1, j1, 1, dz) > 6000 || Judge(i1, j1, mc[0] - 'a', dz) > 6000 || Judge(i1, j1, mc[1] - 'a', dz) > 6000 || Judge(i1, j1, mc[2] - 'a', dz) > 6000)
		{
			if (cMap[i][j] == 'k')
			{
				fp = fopen("1", "a");
				fprintf(fp, "\n炸弹炸了敌方较大棋子");
				fclose(fp);
				return 800000;
			}
		}
		else
		{
			for (int ii = 0; ii != NextNumberOfSteps; ii++)
			{
				int temp1 = allmove1[ii][0], temp2 = allmove1[ii][1];
				if (Judge(i1, j1, 0, dz) > 6000 || Judge(i1, j1, 1, dz) > 6000 || Judge(temp1, temp2, mc[0] - 'a', dz) > 6000 || Judge(temp1, temp2, mc[1] - 'a', dz) > 6000 || Judge(temp1, temp2, mc[2] - 'a', dz) > 6000)
					if (cMap[i][j] == 'k')
					{
						fp = fopen("1", "a");
						fprintf(fp, "\n炸弹模拟两步炸掉敌方较大棋子");
						fclose(fp);
						return 700000;
					}
			}
		}

		/*	if(IsMoveCamp(i,j)!=1 && IsMoveCamp(i1,j1)==1)  //炸弹回行营
		teshu=teshu+2000;
		if(IsMoveCamp(i,j) && cMap[i1][j1]=='0')  //保护炸弹
		teshu=teshu-2000;*/
		///使己方的棋子师长往下依次在炸弹旁
		for (i2 = 0; i2<12; i2++)  // 找己方炸弹的数目
		{
			for (j2 = 0; j2<5; j2++)
				if (cMap[i2][j2] == 'k')
					zha++;
		}
		//mychessdx(cMap,mchess);   //将我放的棋子的棋力按照从大到小排序
		int f = 0;
		for (int m = 0; m<25; m++)  //判断敌方司令和军长是否死亡
		{
			if (dz[m][0] >= 8000 && dz[m][12]>12)
				f++;
			if (dz[m][1] >= 8000 && dz[m][12]>13)
				f++;
		}
		if (zha == 2)
		{
			if (f == 2)//敌方军长和司令全部死掉
				if (Judge(i1, j1, 2, dz) + Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) + Judge(i1, j1, 3, dz) >= 9000)  //炸弹碰司令，军长或师长
					teshu = teshu + 9000;
			if (f == 1 || f == 0)//敌方军长和司令死掉一个
				if (Judge(i1, j1, 2, dz) + Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) >= 9000)  //炸弹碰司令，军长或师长
					teshu = teshu + 9000;
		}
		if (zha == 1)
		{
			if (Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) + Judge(i1, j1, 2, dz) >= 9000)  //炸弹碰司令，军长或师长
				teshu = teshu + 9000;
		}
		flag = 0;
		flag1 = 0;
		int k4;
		if (cMap[i1][j1] == '0')
		{
			for (int m = 0; m<12; m++)
			{
				for (int n = 0; n<5; n++)
				{
					if (cMap[m][n] == 'X' && Judge(m, n, 1, dz) + Judge(m, n, 0, dz) + Judge(m, n, 2, dz)<9000)
					{
						k1 = Oppnextmove(m, n, cmap, allmove1);
						for (k2 = 0; k2<k1; k2++)
						{
							if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
								flag3 = 1;
						}
					}
				}
			}
			k1 = nextmove1(i1, j1, cmap, allmove1);//判断己方棋子能否碰大的棋子
			for (k2 = 0; k2<k1; k2++)
			{
				if (Judge(allmove1[k2][0], allmove1[k2][1], 1, dz) + Judge(allmove1[k2][0], allmove1[k2][1], 0, dz) + Judge(allmove1[k2][0], allmove1[k2][1], 2, dz) >= 7000)
					flag4 = 1;
			}
			if (flag3 == 0 && flag4 == 1)
				teshu = teshu + 3000;
		}
		flag = 0;
		flag1 = 0;
		for (int m = 0; m<12; m++)
		{
			for (int n = 0; n<5; n++)
			{
				k1 = Oppnextmove(m, n, cMap, allmove1);
				for (k2 = 0; k2<k1; k2++)
				{
					if (allmove1[k2][0] == i && allmove1[k2][1] == j)
						flag = 1;
				}
			}
		}
		for (int m = 0; m<12; m++)
		{
			for (int n = 0; n<5; n++)
			{
				k1 = Oppnextmove(m, n, cmap, allmove1);
				for (k2 = 0; k2<k1; k2++)
				{
					if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
						flag1 = 1;
				}
			}
		}
		if (flag == 1 && flag1 == 0)
			teshu = teshu + 1000;
		else
			teshu = teshu - 1000;

	}
	for (i3 = 0; i3<12; i3++)
	{
		for (j3 = 0; j3<5; j3++)
			if (cMap[i3][j3] == 'c')
				x++;
	}
	for (i2 = 0; i2<12; i2++)  // 找己方炸弹的数目
	{
		for (j2 = 0; j2<5; j2++)
			if (cMap[i2][j2] == 'k')
				zha++;
	}
	//己方师长跟随在炸弹附近
	if (cMap[i][j] == 'c' && (x == 2 || x == 1))
	{
		for (i3 = 0; i3<12; i3++)
		{
			for (j3 = 0; j3<5; j3++)
			{
				if (cMap[i3][j3] == 'k')
				{
					k1 = nextmove1(i3, j3, cMap, allmove1);
					for (k2 = 0; k2<k1; k2++)
					{
						if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
						{
							if (cMap[i1][j1] == 'X')
								teshu = teshu - 100;
							else if (((j != 0 && j1 == 0) || (j != 4 && j1 == 4)) && cMap[i1][j1] != 'X')
								teshu = teshu + 500;
							else if (((j == 0 && j1 == 0) || (j == 4 && j1 == 4)) && cMap[i1][j1] != 'X')
								teshu = teshu + 1000;
							else
								teshu = teshu + 200;
						}

					}

				}
			}
		}
	}
	if (cMap[i][j] == 'd' && (x == 1 || x == 0))      //旅长长
	{
		for (i3 = 0; i3<12; i3++)
		{
			for (j3 = 0; j3<5; j3++)
			{
				if (cMap[i3][j3] == 'k')
				{
					k1 = nextmove1(i3, j3, cMap, allmove1);
					for (k2 = 0; k2<k1; k2++)
					{
						if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
						{
							if (cMap[i1][j1] == 'X')
								teshu = teshu - 100;
							else if (((j != 0 && j1 == 0) || (j != 4 && j1 == 4)) && cMap[i1][j1] != 'X')
								teshu = teshu + 500;
							else if (((j == 0 && j1 == 0) || (j == 4 && j1 == 4)) && cMap[i1][j1] != 'X')
								teshu = teshu + 1000;
							else
								teshu = teshu + 200;
						}
					}

				}
			}
		}
	}
	fprintf(fp, "\n炸弹保护和使用影响的特殊分 %d ", teshu);
	/************************************************************/
	/************************************************************/
	//军长封堵中路
	if (cMap[i][j] == 'b')
	{
		if (i == 7 && j == 2)
		{
			if (cMap[6][2] >= 'c'&&cMap[6][2] <= 'i' || cMap[6][2] == '0')
			{
				teshu -= 3000;
				if (cMap[9][3] == 'a' || mchess[0] != 'a')
					teshu += 3000;
				if (cMap[8][1] == 'X'&&i1 == 8 && j1 == 2)
					teshu += 18000;
			}
			if (cMap[6][2] == 'X'&&Judge(6, 2, 10, dz) < 3000 && Judge(6, 2, 0, dz) < 3000 && Judge(6, 2, 1, dz) < 3000 && i1 == 6 && j1 == 2)
				teshu += 8000;
		}
		if (i == 6 && j == 2)
		{
			if (i1 == 7 && j1 == 2 && cMap[5][2] == 'X')
				teshu += 8000;
		}
	}

	//后期小子夺旗
	if (cMap[i][j] >= 'd')
	{
		int ff, difang = 0, xa, ya, zonghe = 0, jl, ui, uj, iaa, ibb, fi, opkk, opll[200][2], opf, fl22 = 0, fx, fens = 0;
		for (xa = 0; xa < 12; xa++)
			for (ya = 0; ya < 5; ya++)
			{
				if (cMap[xa][ya] != '0')
					zonghe++;
				if (cMap[xa][ya] == 'X')
					difang++;
			}
		for (ui = 0; ui < 12; ui++)
			for (uj = 0; uj < 5; uj++)
				if (cMap[ui][uj] == 'X')
				{
					fens = 0;
					fi = cMap[i][j] - 'a';
					for (fx = 0; fx <= fi; fx++)
						fens = fens + Judge(ui, uj, fx, dz);
					if (fens > 6000)
						opf = Oppnextmove(ui, uj, cMap, opll); //敌方下一步模拟
					for (opkk = 0; opkk < opf; opkk++)
					{
						if (opll[opkk][0] == i1 && opll[opkk][1] == j1)   // 敌方下一步是否能碰到我方炸弹第一步
							fl22 = 1;
					}

				}
		if (fl22 == 0)
		{
			if (zonghe <= 40 && difang <= 20 && cMap[i][j] >= 'c')
			{
				if (cMap[0][1] == 'L')
				{

					if (cMap[i1][j1] == '0'&&i1 == 0 && j1 == 0)
					{

						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 0 && j1 == 2)
					{
						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 1)
					{
						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 0 && (cMap[0][0] == '0' || cMap[1][1] == '0'))
					{
						teshu = teshu + 3000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 2 && (cMap[0][2] == '0'))
					{
						teshu = teshu + 3000;

					}

				}
				if (cMap[0][3] == 'L')
				{
					if (cMap[i1][j1] == '0'&&i1 == 0 && j1 == 2)
					{
						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 0 && j1 == 4)
					{
						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 3)
					{
						teshu = teshu + 4000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 2 && (cMap[0][2] == '0'))
					{
						teshu = teshu + 3000;

					}
					if (cMap[i1][j1] == '0'&&i1 == 1 && j1 == 4 && (cMap[0][4] == '0' || cMap[1][3] == '0'))
					{
						teshu = teshu + 8000;

					}
				}

			}
		}
	}

	fprintf(fp, "\n军长封堵中路与小子夺旗影响后的特殊分 %d ", teshu);


	//补充炸弹用法:炸弹追踪系统
	if (cMap[i][j] == 'k')
	{
		int xa, ya, difang = 0, zonghe = 0, d, q, u, allmove5[200][2], allmove4[200][2], data = 0, allmove3[200][2];
		int iq, opl1[200][2], im, opk, op = 0, almove[200][2], al, ak, fl1 = 0, ia, ib, fl2 = 0, fl3 = 1, fl4 = 0;
		int pl1 = 0;
		char cmapa[12][5], in, jn, cmap2[12][5];
		for (d = 0; d < 12; d++)
			for (q = 0; q < 5; q++)
			{
				if (cMap[d][q] == 'X'&&Judge(d, q, 2, dz) + Judge(d, q, 1, dz) + Judge(d, q, 0, dz) < 6000)
				{
					u = Oppnextmove(d, q, cMap, allmove5);
				}
			}

		for (xa = 0; xa < 12; xa++)
			for (ya = 0; ya < 5; ya++)
			{
				if (cMap[xa][ya] != '0')
					zonghe++;
				if (cMap[xa][ya] == 'X')
					difang++;
			}

		for (in = 0; in < 12; in++)
			for (jn = 0; jn < 5; jn++)
			{
				cmapa[in][jn] = cMap[in][jn];
			}
		cmapa[i1][j1] = cmapa[i][j];
		cmapa[i][j] = '0';
		if (zonghe > 20)
		{
			if (IsMoveCamp(i, j) != 1 && IsMoveCamp(i1, j1) == 1)
				teshu = teshu + 5000;
			//	if(IsMoveCamp(i,j)!=1 && IsMoveCamp(i1,j1)==1&&allmove5[u][0]==i&&allmove5[u][1]==j)  //炸弹回行营
			//		teshu=teshu+6000;
			for (k1 = 0; k1 < 12; k1++)
				for (k2 = 0; k2 < 5; k2++)
					cmap2[k1][k2] = cMap[k1][k2];
			cmap2[i1][j1] = cmap2[i][j];
			cmap2[i][j] = '0';
			k1 = nextmove1(i1, j1, cmap2, allmove3);
			for (k2 = 0; k2 < k1; k2++)
				if ((IsMoveCamp(i1, j1) != 1) && (IsMoveCamp(allmove3[k2][0], allmove3[k2][1]) == 1) && (allmove5[u][0] != allmove3[k2][0]) && (allmove5[u][1] != allmove3[k2][1]))
					teshu = teshu + 3000;
			if (IsMoveCamp(i, j) || (Judge(i1, j1, 2, dz) + Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) < 8000) && cMap[i1][j1] == 'X')  //保护炸弹
				teshu = teshu - 5000;
			for (iq = 0; iq <= tra - 1; iq++)//一步追踪
				if ((i1 == dz[track[iq]][12]) && (j1 == dz[track[iq]][13]))
				{
					teshu = teshu + 22000;
					fl1 = 1;
					break;
				}
			if (fl1 == 0)  //无法一步追踪，进行模拟，两步追踪
			{
				al = nextmove1(i1, j1, cmapa, almove); //我方第二步模拟
				for (ia = 0; ia < 12; ia++)
					for (ib = 0; ib < 5; ib++)
						if (cMap[ia][ib] == 'X')
						{
							fl3 = 0;
							for (im = 0; im <= tra - 1; im++)
								if ((ia == dz[track[im]][12]) && (ib == dz[track[im]][13]))
								{
									fl3 = 1;
								}
							if (fl3 == 0)
							{
								op = Oppnextmove(ia, ib, cmapa, opl1); //敌方下一步模拟
								for (opk = 0; opk < op; opk++)
								{
									if (opl1[opk][0] == i1 && opl1[opk][1] == j1)   // 敌方下一步是否能碰到我方炸弹第一步
										fl2 = 1;
								}
							}
						}

				if (fl2 == 0)
				{
					for (ak = 0; ak < al; ak++)
						for (iq = 0; iq <= tra - 1; iq++)//一步追踪
							if ((almove[ak][0] == dz[track[iq]][12]) && (almove[ak][1] == dz[track[iq]][13]))
							{
								teshu = teshu + 9000;
								fl4 = 1;
							}

				}
				//炸弹回营
				//****************************************
				//****************************************
				/*if (fl1 == 0 && fl4 == 0)
				{
				if (IsMoveCamp(i, j) != 1 && IsMoveCamp(i1, j1) == 1)  //炸弹一步回行营
				{
				teshu = teshu + 3000;
				pl1 = 1;
				}

				if (pl1 == 0&&IsMoveCamp(i, j) != 1)//炸弹模拟两步回营
				for (in = 0; in < 12; in++)
				for (jn = 0; jn < 5; jn++)
				{
				cmapa[in][jn] = cMap[in][jn];
				}
				cmapa[i1][j1] = cmapa[i][j];
				cmapa[i][j] = '0';
				al = nextmove1(i1, j1, cmapa, almove);
				for (ia = 0; ia < 12; ia++)
				for (ib = 0; ib < 5; ib++)
				if (cMap[ia][ib] == 'X')
				{
				fl3 = 0;
				for (i = 0; i <= tra - 1; i++)
				if ((ia != dz[track[i]][12]) && (ib != dz[track[i]][13]))
				{
				fl3 = 1;
				}
				if (fl3 == 1)
				op = op + Oppnextmove(ia, ib, cmapa, opl1); //敌方下一步模拟
				}
				for (opk = 0; opk < op; opk++)
				{
				if (opl1[opk][0] == i1&&opl1[opk][1] == j1)   // 敌方下一步是否能碰到我方炸弹第一步
				fl2 = 1;
				}
				if (fl2 == 0)
				{
				for (ak = 0; ak < al; ak++)
				if (IsMoveCamp(almove[ak][0], almove[ak][1]) == 1)
				{
				teshu = teshu + 6000;
				}
				}


				}*/

			}
		}
		else
		{
			for (k1 = 0; k1 < 12; k1++)
				for (k2 = 0; k2 < 5; k2++)
					cmap1[k1][k2] = cMap[k1][k2];
			cmap1[i1][j1] = cmap1[i][j];
			cmap1[i][j] = '0';
			if (cMap[i1][j1] == 'X' && (Judge(i1, j1, 2, dz) + Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) + Judge(i1, j1, 3, dz) + Judge(i1, j1, 4, dz) >= 8000))
			{
				data = 1;
				teshu = teshu + 22000;
			}
			else
			{
				/*	if(cMap[i1][j1]=='X'&&(Judge(i1,j1,2,dz)+Judge(i1,j1,1,dz)+Judge(i1,j1,0,dz)<7000))
				teshu=teshu-1000;*/
				if (data == 0)
				{
					k1 = nextmove1(i1, j1, cmap1, allmove4);
					for (k2 = 0; k2 < k1; k2++)
					{
						if (cmap1[allmove4[k2][0]][allmove4[k2][1]] == 'X' && (Judge(allmove4[k2][0], allmove4[k2][1], 3, dz) + Judge(allmove4[k2][0], allmove4[k2][1], 4, dz) + Judge(allmove4[k2][0], allmove4[k2][1], 2, dz) + Judge(allmove4[k2][0], allmove4[k2][1], 1, dz) + Judge(allmove4[k2][0], allmove4[k2][1], 0, dz) >= 8000) && (allmove5[u][0] != i1) && (allmove5[u][1] != j1))
							teshu = teshu + 9000;
						else
						{
							if (IsMoveCamp(i, j) != 1 && IsMoveCamp(i1, j1) == 1)  //炸弹回行营
								teshu = teshu + 5000;
							for (k1 = 0; k1 < 12; k1++)
								for (k2 = 0; k2 < 5; k2++)
									cmap2[k1][k2] = cMap[k1][k2];
							cmap2[i1][j1] = cmap2[i][j];
							cmap2[i][j] = '0';
							k1 = nextmove1(i1, j1, cmap2, allmove3);
							for (k2 = 0; k2 < k1; k2++)
								if ((IsMoveCamp(i1, j1) != 1) && (IsMoveCamp(allmove3[k2][0], allmove3[k2][1]) == 1) && (allmove5[u][0] != allmove3[k2][0]) && (allmove5[u][1] != allmove3[k2][1]))
									teshu = teshu + 1000;
							if (IsMoveCamp(i, j) || (Judge(i1, j1, 2, dz) + Judge(i1, j1, 1, dz) + Judge(i1, j1, 0, dz) < 8000) && cMap[i1][j1] == 'X')  //保护炸弹
								teshu = teshu - 2000;
						}

					}
				}
			}

		}
	}
	fprintf(fp, "\n炸弹追踪影响后的特殊分值 %d ", teshu);
	/************************************************************/
	/************************************************************/

	//4.工兵行棋(保护与挖雷)		/////////////////////////////////////////////
	flag = 0;
	flag1 = 0;
	flag3 = 0;
	flag4 = 0;
	sanyi1 = false;
	if (cMap[i][j] == 'i')
	{
		flag = walei(cMap, dz);
		if (flag == 0)
		{
			if (Judge(i1, j1, 9, dz) > 4500)   //工兵挖地雷
			{
				sanyi1 = true;
				teshu = teshu + 4000;
			}
			if (sanyi1 == false && cMap[i1][j1] == '0')
			{
				for (int m = 0; m < 12; m++)
				{
					for (int n = 0; n < 5; n++)
					{
						k1 = Oppnextmove(m, n, cmap, allmove1);
						for (k2 = 0; k2 < k1; k2++)
						{
							if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
								flag3 = 1;
						}
					}
				}
				k1 = nextmove1(i1, j1, cmap, allmove1);
				for (k2 = 0; k2 < k1; k2++)
				{
					if (Judge(allmove1[k2][0], allmove1[k2][1], 9, dz) > 4500)
						flag4 = 1;
				}
				if (flag3 == 0 && flag4 == 1)
					teshu = teshu + 2000;
			}
		}
		if (flag == 1)
		{
			if (Judge(i1, j1, 10, dz) >= 4000)   //工兵碰炸弹
			{
				sanyi1 = true;
				teshu = teshu + 4000;
			}
			if (sanyi1 == false && cMap[i1][j1] == '0')//一步碰不到，模拟两步碰子
			{
				for (int m = 0; m < 12; m++)
				{
					for (int n = 0; n < 5; n++)
					{
						k1 = Oppnextmove(m, n, cmap, allmove1);
						for (k2 = 0; k2 < k1; k2++)
						{
							if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
								flag3 = 1;
						}
					}
				}
				k1 = nextmove1(i1, j1, cmap, allmove1);
				for (k2 = 0; k2 < k1; k2++)
				{
					if (Judge(allmove1[k2][0], allmove1[k2][1], 10, dz) >= 4000)
						flag4 = 1;
				}
				if (flag3 == 0 && flag4 == 1)
					teshu = teshu + 2000;
				if (flag3 == 0)
					teshu = teshu + 2000;
			}
		}
		//工兵躲闪敌方棋子
		flag = 0;
		for (int m = 0; m < 12; m++)
		{
			for (int n = 0; n < 5; n++)
			{
				k1 = Oppnextmove(m, n, cMap, allmove1);
				for (k2 = 0; k2 < k1; k2++)
				{
					if (allmove1[k2][0] == i && allmove1[k2][1] == j)
						flag = 1;
				}
			}
		}
		for (int m = 0; m < 12; m++)
		{
			for (int n = 0; n < 5; n++)
			{
				k1 = Oppnextmove(m, n, cmap, allmove1);
				for (k2 = 0; k2 < k1; k2++)
				{
					if (allmove1[k2][0] == i1 && allmove1[k2][1] == j1)
						flag1 = 1;
				}
			}
		}
		if (flag == 1 && flag1 == 0)
			teshu = teshu + 1000;
		else
			teshu = teshu - 1000;
		if (IsMoveCamp(i1, j1))
			teshu = teshu + 300;

		//工兵后期主动挖雷
		if (NumChess(cMap) < 15 && NumOppChess(cMap) < 15)             //自杀式攻击
		{
			/*char cmap10[12][5];
			for (i2 = 0; i2 < 12; i2++)
			for (j2 = 0; j2 < 5; j2++)
			cmap10[i2][j2] == cMap[i2][j2];

			cmap10[i1][j1] = cmap10[i][j];
			cmap10[i][j] = '0';*/
			FILE *fp;
			fp = fopen("1", "a");
			fprintf(fp, "\nwwww");
			fprintf(fp, "\nwwww  %d", Judge(i1, j1, 9, dz));

			int f = 1, f21 = 0, f22 = 0, f23 = 0, f3 = 1;
			fprintf(fp, "\nwwww5  %c %c %c", cMap[9][1], cMap[9][3], mchess[0]);
			if (Judge(i1, j1, 9, dz) > 4000)
				if (cMap[9][1] == mchess[0] || cMap[9][3] == mchess[0])    //最大的在守军旗
				{
					teshu = teshu + 20000;
					f = 0;
					fprintf(fp, "\nwwww1");

				}
			if (cMap[9][1] == mchess[0] || cMap[9][3] == mchess[0])          //最大的在守军旗
				if (f == 1)
				{
					fprintf(fp, "\nwwww22 ");
					k2 = nextmove1(i1, j1, cmap, allmove1);
					fprintf(fp, "\nwwww33 %d ", k2);
					for (k1 = 0; k1 < k2; k1++)
					{
						fprintf(fp, "\nwwww22  %d %d %d", allmove1[k1][0], allmove1[k1][1], Judge(allmove1[k1][0], allmove1[k1][1], 9, dz));
						if (Judge(allmove1[k1][0], allmove1[k1][1], 9, dz) > 4000)
						{
							if (f21 == 0)
							{
								teshu = teshu + 10000;
								f21++;
								f3 = 0;
								fprintf(fp, "\nwww2  %d %d", allmove1[k1][0], allmove1[k1][1]);
							}

						}
					}
				}
			/*if (f3)
			{
			if (cMap[9][1] == mchess[0] || cMap[9][3] == mchess[0])
			{
			for (k1 = 0; k1 < k2; k1++)
			{
			int f31 = 0;
			char cMap3[12][5];
			memcpy(cMap3, cmap, sizeof(cMap3));
			cMap3[allmove1[k1][0]][allmove1[k1][1]] = cMap3[i1][j1];

			k3 = nextmove1(allmove1[k1][0], allmove1[k1][1], cMap3, allmove2);
			for (int k4 = 0; k4 < k3; k4++)
			{
			if (Judge(allmove2[k4][0], allmove2[k4][1], 9, dz)>4000);
			{
			if (f31 == 0)
			{
			teshu += 10000;
			f31++;
			}
			}
			}
			}
			}
			}*/
			fclose(fp);
		}
	}
	sanyi1 = false;
	flag3 = 0;
	flag = 0;
	if (cMap[i][j]>'e' && cMap[i][j]<'i')
	{
		if (Judge(i1, j1, 10, dz) >= 4000)   //小的棋子碰炸弹
		{
			sanyi1 = true;
			teshu = teshu + 4000;
		}
		if (sanyi1 == false && cMap[i1][j1] == '0')//一步碰不到，模拟两步碰子
		{
			/*	for(int m=0;m<12;m++)
			{
			for(int n=0;n<5;n++)
			{
			if(weixie(i,j,m,n,dz,cMap)<0)
			{
			k1=Oppnextmove(m,n,cmap,allmove1);
			for(k2=0;k2<k1;k2++)
			{
			if(allmove1[k2][0]==i1 && allmove1[k2][1]==j1)
			flag3=1;
			}
			}
			}
			}*/
			k1 = nextmove1(i1, j1, cmap, allmove1);
			for (k2 = 0; k2<k1; k2++)
			{
				if (Judge(allmove1[k2][0], allmove1[k2][1], 10, dz) >= 4000)
					flag = 1;
			}
		}
		if (flag3 == 0 && flag == 1)
			teshu = teshu + 1500;
	}
	if (i >= 10)
		teshu = teshu - 300;
	if (cMap[11][0] == 'h'&&Judge(10, 0, 8, dz) > 8000)
		if (i == 11 && j == 0 && i1 == 10 && j1 == 0)
			teshu = teshu + 50000;
	if (cMap[11][2] == 'h'&&Judge(10, 2, 8, dz) > 8000)
		if (i == 11 && j == 2 && i1 == 10 && j1 == 2)
			teshu = teshu + 50000;
	fprintf(fp, "\n工兵的用法影响后的特殊分值 %d ", teshu);
	fprintf(fp, "\n我方可移动棋子数 %d", MyMovingChess(cMap));

	/*if (MyMovingChess(cMap) == 1)
	{
	if (cMap[11][1] == 'l')
	{
	fprintf(fp, "\nsss1 ");
	if (i1 = 10 && j1 == 1)
	{
	teshu = teshu + 2000;
	fprintf(fp, "\nsss2 ");
	}
	}
	if (cMap[11][3] == 'l')
	{
	fprintf(fp, "\nsss3 ");
	if (i1 = 10 && j1 == 3)
	{
	teshu = teshu + 2000;
	fprintf(fp, "\nsss4 ");
	}
	}
	}*/
	/*if (cMap[i1][j1] == '0')
	teshu = teshu + 2000;*/
	fprintf(fp, "\n同上？？ %d ", teshu);
	fclose(fp);
	weixief = weixie(i, j, i1, j1, dz, cMap) / 10;            /////   改为和间接威胁相同/  //weixief=weixie(i,j,i1,j1,dz,cMap)/5;
	qipan = qipanfen[i1][j1] - qipanfen[i][j];
	jweixief = jweixie(i1, j1, dz, cmap) / 10;
	score = weixief + qipan + jweixief + sanyif + teshu;
	score += DefendFlag(cMap, i, j, i1, j1);
	score += DefendLeft(i, j, i1, j1, cMap);
	score += DefendRight(i, j, i1, j1, cMap);
	score += AttackFor31(cMap, i, j, i1, j1);
	fp = fopen("1", "a");
	fprintf(fp, "\n%s,%d,%s,%d,%s,%d,%s,%d,%s,%d\n%c %d,%d,%d,%d\n", "直接威胁", weixief, "间接威胁", jweixief, "三十一步分值", sanyif, "特殊分", teshu, "总分", score, cMap[i][j], i, j, i1, j1);
	fclose(fp);

	/*if (Ina_step[0] == 2 && Ina_step[1]>12)
	return score1;
	else*/
	return score;
}
void CulBestmove(char cMap[12][5], char *cInMessage, char *cOutMessage, int dz[25][14])																	//cInMessage被去除掉，没什么作用
{
	strcpy(cOutMessage, "BESTMOVE A0A0");
	int move[4];
	int allmove[200][4], iSaMove[40][2];
	int i, j, num, num1, i1, j1, k = 0;
	char cmap[12][5];
	int score1, score = -10000;
	//定式
	if (1)
	{
		//我方子在敌方军旗旁边
		if (cMap[0][1] == 'L')
		{
			if (IsMyMovingChess(cMap, 0, 0))
			{
				cOutMessage[9] = 'A';
				cOutMessage[10] = '0';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '1';
				return;
			}
			if (IsMyMovingChess(cMap, 0, 2))
			{
				cOutMessage[9] = 'A';
				cOutMessage[10] = '2';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '1';
				return;
			}
			if (IsMyMovingChess(cMap, 1, 1))
			{
				cOutMessage[9] = 'B';
				cOutMessage[10] = '1';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '1';
				return;
			}
		}
		if (cMap[0][3] == 'L')
		{
			if (IsMyMovingChess(cMap, 0, 4))
			{
				cOutMessage[9] = 'A';
				cOutMessage[10] = '4';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '3';
				return;
			}
			if (IsMyMovingChess(cMap, 0, 2))
			{
				cOutMessage[9] = 'A';
				cOutMessage[10] = '2';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '3';
				return;
			}
			if (IsMyMovingChess(cMap, 1, 3))
			{
				cOutMessage[9] = 'B';
				cOutMessage[10] = '3';
				cOutMessage[11] = 'A';
				cOutMessage[12] = '3';
				return;
			}
		}

		if (cMap[0][0] >= 'e' && cMap[0][0]<'j' && !IsMyChess(cMap, 0, 1))
		{
			cOutMessage[9] = 'A';
			cOutMessage[10] = '0';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '1';
			return;
		}
		if (cMap[0][2] >= 'e' && cMap[0][2]<'j' && !IsMyChess(cMap, 0, 1))
		{
			cOutMessage[9] = 'A';
			cOutMessage[10] = '2';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '1';
			return;
		}
		if (cMap[1][1] >= 'e' && cMap[1][1]<'j' && !IsMyChess(cMap, 0, 1))
		{
			cOutMessage[9] = 'B';
			cOutMessage[10] = '1';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '1';
			return;
		}

		if (cMap[0][4] >= 'e' && cMap[0][4]<'j' && !IsMyChess(cMap, 0, 3))
		{
			cOutMessage[9] = 'A';
			cOutMessage[10] = '4';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '3';
			return;
		}
		if (cMap[0][2] >= 'e' && cMap[0][2]<'j' && !IsMyChess(cMap, 0, 3))
		{
			cOutMessage[9] = 'A';
			cOutMessage[10] = '2';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '3';
			return;
		}
		if (cMap[1][3] >= 'e' && cMap[1][3]<'j' && !IsMyChess(cMap, 0, 3))
		{
			cOutMessage[9] = 'B';
			cOutMessage[10] = '3';
			cOutMessage[11] = 'A';
			cOutMessage[12] = '3';
			return;
		}
	}
	FILE *fp;
	fp = fopen("1", "a");
	for (int m = 0; m<12; m++)
	{
		for (int n = 0; n<5; n++)
		{
			fprintf(fp, "%c ", cMap[m][n]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n\n\n%d,%d\n", Ina_step[0], Ina_step[1]);
	for (i = 0; i<25; i++)
	{
		for (j = 0; j<14; j++)
			fprintf(fp, "%5d,", dz[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);

	for (i = 0; i<12; i++)
		for (j = 0; j<5; j++)
		{
			if (IsMyMovingChess(cMap, i, j) && !IsBaseCamp(i, j))  //己方不在大本营的可移动棋子
			{
				//工兵形棋
				if (cMap[i][j] == 'i' && IsRailway(i, j))
				{
					iSaMove[0][0] = i;
					iSaMove[0][1] = j;
					for (i1 = 0; i1<12; i1++)
					{
						for (j1 = 0; j1<5; j1++)
							cmap[i1][j1] = cMap[i1][j1];
					}
					num = SapperRail(iSaMove, cmap, i, j, 0);
					for (num1 = 1; num1<num; num1++)
					{
						allmove[k][0] = i;
						allmove[k][1] = j;
						allmove[k][2] = iSaMove[num1][0];
						allmove[k][3] = iSaMove[num1][1];
						k++;
					}
				}
				//可以前移:不在第一行,不在山界后,前方不是己方棋子,前方不是有棋子占领的行营
				if (i>0 && !IsAfterHill(i, j) && !IsMyChess(cMap, i - 1, j) && !IsFilledCamp(cMap, i - 1, j))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i - 1;
					allmove[k][3] = j;
					k++;
					if ((j == 0 || j == 4) && i>0 && i<11)
					{
						for (i1 = i - 1; i1>0; i1--)
						{
							if (cMap[i1][j] == '0')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i1;
								allmove[k][3] = j;
								k++;
							}
							else if (cMap[i1][j] >= 'A' && cMap[i1][j] == 'X')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i1;
								allmove[k][3] = j;
								k++;
								break;
							}
							else break;
						}
					}
				}
				//可以左移:不在最左列,左侧不是己方棋子,左侧不是被占用的行营
				if (j>0 && !IsMyChess(cMap, i, j - 1) && !IsFilledCamp(cMap, i, j - 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i;
					allmove[k][3] = j - 1;
					k++;
					//是否在1,5,6,10铁道上
					if (i == 1 || i == 5 || i == 6 || i == 10)
					{
						for (int i1 = j - 1; i1 >= 0; i1--)
						{
							if (cMap[i][i1] == '0')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i;
								allmove[k][3] = i1;
								k++;
							}
							else if (cMap[i][i1] >= 'A'&&cMap[i][i1] <= 'X')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i;
								allmove[k][3] = i1;
								k++;
								break;
							}
							else break;
						}
					}
				}
				//可以右移:不在最右列,右侧不是己方棋子,右侧不是被占用的行营
				if (j<4 && !IsMyChess(cMap, i, j + 1) && !IsFilledCamp(cMap, i, j + 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i;
					allmove[k][3] = j + 1;
					k++;
					//是否在1,5,6,10铁道上
					if (i == 1 || i == 5 || i == 6 || i == 10)
					{
						for (int i1 = j + 1; i1<5; i1++)
						{
							if (cMap[i][i1] == '0')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i;
								allmove[k][3] = i1;
								k++;
							}
							else if (cMap[i][i1] >= 'A'&&cMap[i][i1] <= 'X')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i;
								allmove[k][3] = i1;
								k++;
								break;
							}
							else break;
						}
					}
				}

				//可以后移：不在最后一列，后方不是己方旗子，后面不是已占有的行营
				if (i<11 && !IsBehindHill(i, j) && !IsMyChess(cMap, i + 1, j) && !IsFilledCamp(cMap, i + 1, j))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i + 1;
					allmove[k][3] = j;
					k++;
					//可以在铁轨两侧向后移动，后方不是己方旗子，不是第一行，不在最后一行
					if ((j == 0 || j == 4) && i>0 && i<11)
					{
						for (i1 = i + 1; i1<11; i1++)
						{
							if (cMap[i1][j] == '0')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i1;
								allmove[k][3] = j;
								k++;
							}
							else if (cMap[i1][j] >= 'A' && cMap[i1][j] <= 'X')
							{
								allmove[k][0] = i;
								allmove[k][1] = j;
								allmove[k][2] = i1;
								allmove[k][3] = j;
								k++;
								break;
							}
							else  break;
						}

					}
				}
				//可以左前移：左前方不是已占有的行营，不是己方旗子
				if ((Iszuoqian(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i - 1, j - 1) && !IsFilledCamp(cMap, i - 1, j - 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i - 1;
					allmove[k][3] = j - 1;
					k++;
				}
				//可以左后移：左后方不是己方旗子，不是已占有的行营
				if ((Iszuohou(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i + 1, j - 1) && !IsFilledCamp(cMap, i + 1, j - 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i + 1;
					allmove[k][3] = j - 1;
					k++;
				}
				//可以右前移：右前方不是己方旗子，不是已占有的行营
				if ((Isyouqian(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i - 1, j + 1) && !IsFilledCamp(cMap, i - 1, j + 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i - 1;
					allmove[k][3] = j + 1;
					k++;
				}
				//可以右后移：右后方不是己方旗子，不是已占有的行营
				if ((Isyouhou(i, j) || IsMoveCamp(i, j)) && !IsMyChess(cMap, i + 1, j + 1) && !IsFilledCamp(cMap, i + 1, j + 1))
				{
					allmove[k][0] = i;
					allmove[k][1] = j;
					allmove[k][2] = i + 1;
					allmove[k][3] = j + 1;
					k++;
				}
			}
		}
	for (i = 0; i<k; i++)
	{
		if ((score1 = pinggu(allmove[i][0], allmove[i][1], allmove[i][2], allmove[i][3], dz, cMap)) >= score)
		{
			if (allmove[i][0] == allmove[i][2] && allmove[i][1] == allmove[i][3])
				continue;
			else
			{
				score = score1;
				move[0] = allmove[i][0];
				move[1] = allmove[i][1];
				move[2] = allmove[i][2];
				move[3] = allmove[i][3];
			}
		}
	}
	cOutMessage[9] = move[0] + 'A';
	cOutMessage[10] = move[1] + '0';
	cOutMessage[11] = move[2] + 'A';
	cOutMessage[12] = move[3] + '0';
	fp = fopen("1", "a");
	qqqqqq++;
	fprintf(fp, "\n第%d步结束%d,%d,%d,%d,%d,%d\n", qqqqqq, move[0], move[1], move[2], move[3], score1, score);
	//	fprintf(fp,"\n%d\n",zz++);
	fclose(fp);
}
/* ************************************************************************ */
/* 函数功能：计算己方最佳着法(本程序为示例算法,请参赛选手自行改进算法)		*/
/* 接口参数：																*/
/*     char *cMap 棋盘局面													*/
/*     char *cInMessage 来自裁判的 GO 命令									*/
/*     char *cOutMessage 发给裁判的 BESTMOVE 命令							*/
/* ************************************************************************ */


int main()
{
	char cVer[200];			//协议版本
	int iFirst;				//先行权[0先行，1后行]																							//不知道iFirst、iTime、iStep有什么用？可能与平台有关系。
	int iTime;				//行棋时间限制(单位秒)[1000,3600]
	int iStep;				//进攻等待限制(单位步)[10,31]
	char cInMessage[200];   //输入通信内容
	char cOutMessage[200];  //输出通信内容
	char cIn1[200], cIn2[200];
	char cMap[12][5];		//棋盘
							//   司令 ,军长 ,师长 ,旅长 ,团长 ,营长 ,连长 ,排长 ,工兵 ,地雷 ,炸弹 ,军旗     i,j,(该子所在位置)
	int dz[25][14] = {
		100  ,100  ,250  ,500  ,500  ,700  ,2000 ,2100 ,400  ,3250 ,100  ,0    ,   0,0,//A0  10000
		0    ,0    ,0    ,0    ,0    ,0    ,0    ,3000 ,0    ,2000 ,0    ,5000 ,   0,1,//A1
		100  ,100  ,250  ,500  ,500  ,700  ,2000 ,2100 ,200  ,3250 ,100  ,0    ,   0,2,//A2
		0    ,0    ,0    ,0    ,0    ,0    ,0    ,3000 ,0    ,2000 ,0    ,5000 ,   0,3,//A3
		100  ,100  ,250  ,500  ,500  ,700  ,2000 ,2100 ,400  ,3250 ,100  ,0    ,   0,4,//A4

		500  ,500  ,850  ,700  ,600  ,500  ,600  ,500  ,1000 ,3250 ,1000 ,0    ,   1,0,//B0
		400  ,400  ,850  ,700  ,600  ,500  ,800  ,600  ,1000 ,3250 ,900  ,0    ,   1,1,//B1
		400  ,400  ,850  ,700  ,600  ,500  ,800  ,600  ,1000 ,3250 ,900  ,0    ,   1,2,//B2
		400  ,400  ,850  ,700  ,600  ,500  ,800  ,600  ,1000 ,3250 ,900  ,0    ,   1,3,//B3
		500  ,500  ,850  ,700  ,600  ,500  ,600  ,500  ,1000 ,3250 ,1000 ,0    ,   1,4,//B4

		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   2,0,//C0
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   2,2,//C2
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   2,4,//C4

		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   3,0,//D0
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   3,1,//D1
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   3,3,//D3
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   3,4,//D4

		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   4,0,//E0
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   4,2,//E2
		500  ,500  ,1000 ,1000 ,1000 ,1500 ,500  ,500  ,2000 ,0    ,1500 ,0    ,   4,4,//E4

		500  ,500  ,1000 ,1000 ,1200 ,2800 ,1500 ,1500 ,0    ,0    ,0    ,0    ,   5,0,//F0
		500  ,500  ,1000 ,1000 ,1000 ,1000 ,1500 ,1500 ,2000 ,0    ,0    ,0    ,   5,1,//F1
		500  ,500  ,1000 ,1000 ,1100 ,2800 ,1600 ,1500 ,0    ,0    ,0    ,0    ,   5,2,//F2
		500  ,500  ,1000 ,1000 ,1000 ,1000 ,1500 ,1500 ,2000 ,0    ,0    ,0    ,   5,3,//F3
		500  ,500  ,1000 ,1000 ,1200 ,2800 ,1500 ,1500 ,0    ,0    ,0    ,0    ,   5,4,//F4
	};   //猜测概率                  10000     22200  27600



		 //cin.gets(a,20)		gets(a)		cin.get(str,size).get();后面的get()用于清除前面输入的残留在缓冲区的换行符。
	cin.getline(cInMessage, 200);		//获取来自裁判系统的指令 "GO 0000 0 00"
										//	gets(cInMessage);

	while (cInMessage[0] >= 'A')
	{
		if (strcmp(cIn1, cIn2) == 0) {
			//		printf("han been gone run!\n");
		}
		else {
			//		FILE *fp;
			//		fp=fopen("D:/Debug/ceshi.txt","a");
			//	fprintf(fp,"%s%s\n","发送指令:",cInMessage);
			switch (cInMessage[0])
			{

			case 'I':								//INFO指令
				CulInfo(cInMessage, cVer, cOutMessage);
				cout << cOutMessage << endl;			//将"NAME "指令传递给裁判系统
				break;
			case 'S':								//START 指令
				CulArray(cInMessage, &iFirst, &iTime, &iStep, cOutMessage);
				InitMap(cMap, cOutMessage);
				cout << cOutMessage << endl;
				break;
			case 'G':								//GO 指令
				FreshMap(cMap, cInMessage, cOutMessage, dz);
				CulBestmove(cMap, cInMessage, cOutMessage, dz);
				cout << cOutMessage << endl;
				break;
			case 'R':								//RESULT 指令
				FreshMap(cMap, cInMessage, cOutMessage, dz);
				//	cout<<cOutMessage<<endl;
				break;
			case 'E':								//END 指令
				return 0;
			default:
				return 1;

			}
			//		fprintf(fp,"%s%s\n","接受指令:",cOutMessage);
			//	fclose(fp);
		}
		strcpy(cIn1, cInMessage);
		//	cin.getline(cInMessage,200);	//获取来自裁判系统的指令"START" 或 "RESULT" 或 "GO" 或 "END"

		while (1) {
			gets_s(cInMessage);
			if (cInMessage[0] >= 'A') {
				break;
			}
		}
		strcpy(cIn2, cInMessage);
	}
	return 0;
}

