//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//国际跳棋组负责人黄倩所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//黄倩		QQ：2505649975
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

int a[8][8];             //棋盘
int player = 1;				//player=1为白方  player=-1 为黑方
int state = 0;				//state=0 为无子可吃   ，1 为有子必吃
int record[100][2];			//记录必吃路径
int record1[20][2];         //暂时记录必吃路径
int length;                  //必吃的长度
int length1;				//暂时记录必吃的长度
int count;                   //必吃的有几种情况

// 以下函数只在find 函数中调用，或者这些函数互相调用。
//----------------------------------------------------------------
void Soilder(int, int);
void King(int, int);
void SoilderLeftUp(int, int);
void SoilderRightUp(int, int);
void SoilderLeftDown(int, int);
void SoilderRightDown(int, int);
void KingLeftUp(int, int);
void KingLeftDown(int, int);
void KingRightUp(int, int);
void KingRightDown(int, int);
//-----------------------------------------------------------------

void draw();                //画棋盘
void start();				//开始时 进行布局
void go();				//下棋
void change();	        //黑白棋交换位置		
void change1();
void find();            //找 是否有必吃的棋子
void findlongest();     //找到必吃中吃子数最多的吃法

int main()
{
	start();
	draw();
}

void start()
{
	a[0][1] = -1;	a[0][3] = -1;	a[0][5] = -1;	a[0][7] = -1;
	a[1][0] = -1;   a[1][2] = -1;   a[1][4] = -1;   a[1][6] = -1;
	a[2][1] = -1;   a[2][3] = -1;   a[2][5] = -1;   a[2][7] = -1;
	a[3][0] = 0;   a[3][2] = 0;   a[3][4] = 0;   a[3][6] = 0;
	a[4][1] = 0;   a[4][3] = 0;   a[4][5] = 0;	 a[4][7] = 0;
	a[5][0] = 1;   a[5][2] = 1;   a[5][4] = 1;   a[5][6] = 1;
	a[6][1] = 1;   a[6][3] = 1;   a[6][5] = 1;   a[6][7] = 1;
	a[7][0] = 1;   a[7][2] = 1;   a[7][4] = 1;   a[7][6] = 1;
}

void draw()
{
	printf("—————————————————————\n ");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (a[i][j] == 1)
				printf("| ● ");
			if (a[i][j] == -1)
				printf("| ○ ");
			if (a[i][j] == 0)
				printf("|    ");
			if (a[i][j] == 2)
				printf("| ▼ ");
			if (a[i][j] == -2)
				printf("| ▽ ");
		}
		printf("|  %d\n—————————————————————\n ", i+1);
	}
	printf("  1    2    3    4    5    6     7    8   \n\n\n");
	if (player==1)
	find();
	if (player == -1)
	change();
}

void change()
{
	int b[4][8];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
		{
			b[i][j] = a[i][j];
			a[i][j] = -a[7 - i][7 - j];
			a[7 - i][7 - j] = -b[i][j];
		}
	find();
}

void change1()
{
	int b[4][8];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 8; j++)
		{
			b[i][j] = a[i][j];
            a[i][j] = -a[7 - i][7 - j];
			a[7 - i][7 - j] = -b[i][j];
}
	draw();
}

void go()
{
	if (player == 1)
		printf("请白方落子 格式为： ");
	else
		printf("请黑方落子 格式为： ");

	if (state == 0)      //不吃 走
	{
		int X1, Y1, X2, Y2, k;
		printf("\n  X1（点）Y1（空格）X2（点）Y2     如（1.1 2.2）\n\n");
		scanf("%d.%d %d.%d", &X1, &Y1, &X2, &Y2);

		X1 = X1 - 1;
		Y1 = Y1 - 1;
		X2 = X2 - 1;
		Y2 = Y2 - 1;

		if (player == -1)            // 交换位置
		{
			X1 = 7 - X1;
			Y1 = 7 - Y1;
			X2 = 7 - X2;
			Y2 = 7 - Y2;
		}


		if (X1 >= 0 && X1 < 8 && Y1 >= 0 && Y1 < 8 && X2 >= 0 && X2 < 8 && Y2 >= 0 && Y2 < 8 && a[X2][Y2] == 0 && (a[X1][Y1] == 1 || a[X1][Y1] == 2))
		{
			if (a[X1][Y1] == 1 && X2 == X1 - 1 && (Y2 == Y1 + 1 || Y2 == Y1 - 1))        //兵棋
			{
				a[X1][Y1] = 0;
				a[X2][Y2] = 1;
			}

			else if (a[X1][Y1] == 2 && (X1 -X2==Y1-Y2 || X1-X2==Y2-Y1))															//王棋
			{
				if (X1 < X2)
				{
					int X = X1;
					X1 = X2;
					X2 = X;
				}

				k = 0;
				for (int K=1;K<X1-X2;K++)
					{
						if (a[X2+K][Y2+K] != 0)
						{
							k = k + 1;
						}
					}

				if (k != 0)
				{
					printf("请重新输入.\n");
					go();
				}
				else
				{
					a[X1][Y1] = 0;
					a[X2][Y2] = 2;
				}
			}

			else
			{
				printf("请重新输入：");
				go();
			}

		}
		else
		{
			printf("请重新输入：");
			go();
		}
	}

	if (state == 1)   //必吃
	{
		int N;
		printf("请输入序号：\n");
		for (int i = 1; i <= count; i++)
		{
			printf("%d.  ", i);
			printf("%d.", record[(length + 1)*(i-1)][0] + 1);
			printf("%d.", record[(length + 1)*(i-1)][1] + 1);
			for (int j = 1; j <= length; j++)
			{
				printf("----%d.", record[(length + 1)*(i - 1) + j][0] + 1);
				printf("%d", record[(length + 1)*(i - 1) + j][1] + 1);
			}
			printf("\n");
		}

		scanf("%d", &N);
		if (N > count)
		{
			printf("请重新输入：\n");
			go();
		}
		else
		{
			if (player == -1)
				for (int k = 0; k <= (length + 1)*(count - 1) + length; k++)
				{
					record[k][0] = 7 - record[k][0];
					record[k][1] = 7 - record[k][1];
				}

			if (a[record[(length + 1)*(N - 1)][0]][record[(length + 1)*(N - 1)][1]] == 1) //兵棋吃
				{
					a[record[(length + 1)*(N - 1)][0]][record[(length + 1)*(N - 1)][1]] = 0;
					a[record[(length + 1)*(N - 1) + length][0]][record[(length + 1)*(N - 1) + length][1]] = 1;
					for (int j = 1; j <= length; j++)
						{
							a[(record[(length + 1)*(N - 1) + (j - 1)][0] + record[(length + 1)*(N - 1) + j][0]) / 2][(record[(length + 1)*(N - 1) + (j - 1)][1] + record[(length + 1)*(N - 1) + j][1]) / 2] = 0;
						}
				}

			else if (a[record[(length + 1)*(N - 1)][0]][record[(length + 1)*(N - 1)][1]] == 2)         //王棋吃
				{
					a[record[(length + 1)*(N - 1)][0]][record[(length + 1)*(N - 1)][1]] = 0;
					a[record[(length + 1)*(N - 1) + length][0]][record[(length + 1)*(N - 1) + length][1]] = 2;
					for (int j = 1; j <= length; j++)
					{
						if (record[(length + 1)*(N - 1) + (j - 1)][0] > record[(length + 1)*(N - 1) + j][0] && record[(length + 1)*(N - 1) + (j - 1)][1] > record[(length + 1)*(N - 1) + j][1])
						{       //右下到左上 换位
							int temp;
							temp=record[(length + 1)*(N - 1) + (j - 1)][0];
							record[(length + 1)*(N - 1) + (j-1)][0] = record[(length + 1)*(N - 1) + j][0];
							record[(length + 1)*(N - 1) + j][0] = temp;

							temp = record[(length + 1)*(N - 1) + (j - 1)][1];
							record[(length + 1)*(N - 1) + (j - 1)][1] = record[(length + 1)*(N - 1) + j][1];
							record[(length + 1)*(N - 1) + j][1] = temp;
						}
						else if (record[(length + 1)*(N - 1) + (j - 1)][0] > record[(length + 1)*(N - 1) + j][0] && record[(length + 1)*(N - 1) + (j - 1)][1] < record[(length + 1)*(N - 1) + j][1])
						{       //左下到右上 换位;
							int temp;
							temp = record[(length + 1)*(N - 1) + (j - 1)][0];
							record[(length + 1)*(N - 1) + (j - 1)][0] = record[(length + 1)*(N - 1) + j][0];
							record[(length + 1)*(N - 1) + j][0] = temp;

							temp = record[(length + 1)*(N - 1) + (j - 1)][1];
							record[(length + 1)*(N - 1) + (j - 1)][1] = record[(length + 1)*(N - 1) + j][1];
							record[(length + 1)*(N - 1) + j][1] = temp;
						}

						if (record[(length + 1)*(N - 1) + (j - 1)][0] < record[(length + 1)*(N - 1) + j][0] && record[(length + 1)*(N - 1) + (j - 1)][1] < record[(length + 1)*(N - 1) + j][1])
						{		//左上到右下 + +

							for (int k = 1; k < record[(length + 1)*(N - 1) + j][0] - record[(length + 1)*(N - 1) + (j - 1)][0]; k++)
							{
								if (a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] + k] == -1 || a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] + k] == -2)
								{
									a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] + k] = 0;
									break;
								}

							}
						}
						else if (record[(length + 1)*(N - 1) + (j - 1)][0] < record[(length + 1)*(N - 1) + j][0] && record[(length + 1)*(N - 1) + (j - 1)][1] > record[(length + 1)*(N - 1) + j][1])
						{		//右上到左下   + -
							for (int k = 1; k < record[(length + 1)*(N - 1) + j][0] - record[(length + 1)*(N - 1) + (j - 1)][0]; k++)
							{
								if (a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] - k] == -1 || a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] - k] == -2)
								{
									a[(record[(length + 1)*(N - 1) + (j - 1)][0]) + k][record[(length + 1)*(N - 1) + (j - 1)][1] - k] = 0;
									break;
								}

							}
						}
					}
				}
		}
	}
	

	for (int i = 0; i < 100; i++)                       //还原记录
		for (int j = 0; j < 2; j++)
			record[i][j] = 0;
	length = 0;
	count = 0;

	for (int j = 1; j <= 7; j++)                           //成王
		if (a[0][j] == 1)
			a[0][j] = 2;

	int number = 0;                                //检验另一个棋子个数
	for (int i = 0; i <= 7; i++)
		for (int j = 0; j <= 7; j++)
			if (a[i][j] == -1 || a[i][j] == -2)
				number = number + 1;

	if (number == 0)
	{
		printf("结束");
		if (player == 1)
			printf("白方胜");
		else if (player == -1)
			printf("黑方胜");
		system("pause");
	}

	else
	{
		player = -player;
		if (player == -1)
			draw();
		else
			change1();
	}
}

void find()
{
	state = 0;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (a[i][j] == 1)
			{
				for (int i1 = 0; i1 < 20; i1++)
					for (int j1=0; j1 < 2; j1++)
						record1[i1][j1] = 0;
				length1 = 0;

				record1[0][0] = i;
				record1[0][1] =j;
				Soilder(i, j);
			}
		}

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (a[i][j] == 2)
			{
				for (int i1 = 0; i1 < 20; i1++)
					for (int j1 = 0; j1 <= 2; j1++)
						record1[i1][j1] = 0;
				length1 = 0;

				record1[0][0] = i;
				record1[0][1] = j;
				King(i, j);
			}
		}
		go();
}

void findlongest()
{
	if (length1 > length)
	{
		length = length1;
		count = 1;

		if (player == -1)
		{
			record[0][0] = 7 - record1[0][0];
			record[0][1] = 7 - record1[0][1];
		}
		else
		{
			record[0][0] = record1[0][0];
			record[0][1] = record1[0][1];
		}

		for (int i = 1; i <= length; i++)
		{
			record[i][0] = record1[i][0];
			record[i][1] = record1[i][1];
		}
	}

	else if (length1 == length)
	{
		if (player == -1)
		{
			record[(length + 1)*count][0] = 7-record1[0][0];
			record[(length + 1)*count][1] = 7-record1[0][1];
		}
		else
		{
			record[(length + 1)*count][0] = record1[0][0];
			record[(length + 1)*count][1] = record1[0][1];
		}

		for (int i = 1; i <=length; i++)
		{
			record[(length + 1)*count + i][0] = record1[i][0];
			record[(length + 1)*count + i][1] = record1[i][1];
		}
		count = count + 1;
	}
	length1 = length1 - 1;
}

void Soilder(int i, int j)
{
	if (i < 2)
	{
		if (j <= 1)
		{
			if ((a[i + 1][j + 1] == -1 || a[i + 1][j + 1] == -2) && a[i + 2][j + 2] == 0)
			{
				SoilderRightDown(i, j);
				findlongest();
			}
		}
		else if (j > 1 && j < 6)
		{
			if ((a[i + 1][j + 1] == -1 || a[i + 1][j + 1] == -2) && a[i + 2][j + 2] == 0)
			{
				SoilderRightDown(i, j);
				findlongest();
			}
			if ((a[i + 1][j - 1] == -1 || a[i + 1][j - 1] == -2) && a[i + 2][j - 2] == 0)
			{
				SoilderLeftDown(i, j);
				findlongest();
			}
		}
		else
		{
			if ((a[i + 1][j - 1] == -1 || a[i + 1][j - 1] == -2) && a[i + 2][j - 2] == 0)
			{
				SoilderLeftDown(i, j);
				findlongest();
			}
		}
	}

	else if (i >= 2 && i <= 5)
	{
		if ((j == 0 || j == 1))
		{
			if ((a[i - 1][j + 1] == -1 || a[i - 1][j + 1] == -2) && a[i - 2][j + 2] == 0)
			{
				SoilderRightUp(i, j);
				findlongest();
			}

			if ((a[i + 1][j + 1] == -1 || a[i + 1][j + 1] == -2) && a[i + 2][j + 2] == 0)
			{
				SoilderRightDown(i, j);
				findlongest();
			}
		}

		else if (j > 1 && j < 6)
		{
			if ((a[i - 1][j - 1] == -1 || a[i - 1][j - 1] == -2) && a[i - 2][j - 2] == 0)
			{
				SoilderLeftUp(i, j);
				findlongest();
			}
			if ((a[i - 1][j + 1] == -1 || a[i - 1][j + 1] == -2) && a[i - 2][j + 2] == 0)
			{
				SoilderRightUp(i, j);
				findlongest();
			}
			if ((a[i + 1][j - 1] == -1 || a[i + 1][j - 1] == -2) && a[i + 2][j - 2] == 0)
			{
				SoilderLeftDown(i, j);
				findlongest();
			}
			if ((a[i + 1][j + 1] == -1 || a[i + 1][j + 1] == -2) && a[i + 2][j + 2] == 0)
			{
				SoilderRightDown(i, j);
				findlongest();
			}
		}

		else if ((j == 6 || j == 7))
		{
			if ((a[i - 1][j - 1] == -1 || a[i - 1][j - 1] == -2) && a[i - 2][j - 2] == 0)
			{
			
			SoilderLeftUp(i, j);
			findlongest();
			}
			if ((a[i + 1][j - 1] == -1 || a[i + 1][j - 1] == -2) && a[i + 2][j - 2] == 0)
			{
				SoilderLeftDown(i, j);
				findlongest();
			}
		}
	}

	else
	{
		if (j <= 1)
		{
			if ((a[i - 1][j + 1] == -1 || a[i - 1][j + 1] == -2) && a[i - 2][j + 2] == 0)
			{
				SoilderRightUp(i, j);
				printf("SoilderRightUp\n");
				findlongest();
			}
		}
		else if (j > 1 && j < 6)
		{
			if ((a[i - 1][j + 1] == -1 || a[i - 1][j + 1] == -2) && a[i - 2][j + 2] == 0)
			{
				SoilderRightUp(i, j);
				printf("SoilderRightUp\n");
				findlongest();
			}
			if ((a[i - 1][j - 1] == -1 || a[i - 1][j - 1] == -2) && a[i - 2][j - 2] == 0)
			{
				SoilderLeftUp(i, j);
				printf("SoilderLeftUp\n");
				findlongest();
			}
		}
		else
		{
			if ((a[i - 1][j - 1] == -1 || a[i - 1][j - 1] == -2) && a[i - 2][j - 2] == 0)
			{
				SoilderLeftUp(i, j);
				printf("SoilderLeftUp\n");
				findlongest();
			}
		}
	}
}

void SoilderLeftUp(int i, int j)
{
	length1 = length1 + 1;
	record1[length1][0] = i - 2;
	record1[length1][1] = j - 2;

	if (player == -1)
	{
		record1[length1][0] = 7 - (i - 2);
		record1[length1][1] = 7 - (j - 2);
	}

	int k;
	a[i][j] = 0;
	k = a[i - 1][j - 1];
	a[i - 1][j - 1] = 0;
	a[i - 2][j - 2] = 1;
	state = 1;                                     //改变状态
	Soilder(i - 2, j - 2);

	a[i][j] = 1;									  //还原
	a[i - 1][j - 1] = k;
	a[i - 2][j - 2] = 0;
}

void SoilderRightUp(int i, int j)
{
	length1 = length1 + 1;
	record1[length1][0] = i - 2;
	record1[length1][1] = j + 2;

	if (player == -1)
	{
		record1[length1][0] = 7 - (i - 2);
		record1[length1][1] = 7 - (j + 2);
	}

	int k;
	a[i][j] = 0;
	k = a[i - 1][j + 1];
	a[i - 1][j + 1] = 0;
	a[i - 2][j + 2] = 1;
	state = 1;                                          //改变状态
	Soilder(i - 2, j + 2);

	a[i][j] = 1;                                        //还原
	a[i - 1][j + 1] = k;
	a[i - 2][j + 2] = 0;
}

void SoilderLeftDown(int i, int j)
{
	length1 = length1 + 1;
	record1[length1][0] = i + 2;
	record1[length1][1] = j - 2;

	if (player == -1)
	{
		record1[length1][0] = 7 - (i + 2);
		record1[length1][1] = 7 - (j - 2);
	}

	int k;
	a[i][j] = 0;
	k = a[i + 1][j - 1];
	a[i + 1][j - 1] = 0;
	a[i + 2][j - 2] = 1;
	state = 1;                                     //改变状态
	Soilder(i + 2, j - 2);

	a[i][j] = 1;									  //还原
	a[i + 1][j - 1] = k;
	a[i + 2][j - 2] = 0;
}

void SoilderRightDown(int i, int j)
{
	length1 = length1 + 1;
	record1[length1][0] = i + 2;
	record1[length1][1] = j + 2;

	if (player == -1)
	{
		record1[length1][0] = 7 - (i + 2);
		record1[length1][1] = 7 - (j + 2);
	}

	int k;
	a[i][j] = 0;
	k = a[i + 1][j + 1];
	a[i + 1][j + 1] = 0;
	a[i + 2][j + 2] = 1;
	state = 1;                                          //改变状态
	Soilder(i + 2, j + 2);

	a[i][j] = 1;                                        //还原
	a[i + 1][j + 1] = k;
	a[i + 2][j + 2] = 0;
}

void King(int i, int j)
{
	if (i < 2)
	{
		if (j < 2)
			KingRightDown(i, j);
		else if (j < 6)
		{
			KingLeftDown(i, j);
			KingRightDown(i, j);
		}
		else
			KingLeftDown(i, j);
	}

	else if (i < 6)
	{
		if (j < 2)
		{
			KingRightUp(i, j);
			KingRightDown(i, j);
		}
		else if (j < 6)
		{
			KingLeftUp(i, j);
			KingRightUp(i, j);
			KingLeftDown(i, j);
			KingRightDown(i, j);
		}
		else
		{
			KingLeftUp(i, j);
			KingLeftDown(i, j);
		}
	}

	else
	{
		if (j < 2)
			KingRightUp(i, j);
		else if (j < 6)
		{
			KingLeftUp(i, j);
			KingRightUp(i, j);
		}
		else
			KingLeftUp(i, j);
	}
}

void KingLeftUp(int i, int j)
{
	for(int k=1;i-k>=1 && j-k>=1;k++)
		if ((a[i - k][j - k] == -1 || a[i - k][j - k] == -2) && a[i - k - 1][j - k - 1] == 0)
		{
			a[i][j] = 0;
			int i1 = i - k;                            //a[i1][j1]是被吃的子
			int j1 = j - k;
			k = a[i1][j1];
			a[i1][j1] = 0;

			for (int k1 = 1; i1 - k1 >= 0 && j1 - k1 >= 0; k1++)
				if (a[i1 - k1][j1 - k1] == 0)
				{
					
					int i2 = i1 - k1;
					int j2 = j1 - k1;
					a[i2][j2] = 2;

					length1 = length1 + 1;
					record1[length1][0] = i2;
					record1[length1][1] = j2;
					if (player == -1)
					{
						record1[length1][0] = 7 - i2;
						record1[length1][1] = 7 - j2;
					}

					state = 1;                          //改变状态
					King(i2, j2);

					a[i2][j2] = 0;

					findlongest();
				}
			a[i][j] = 2;						//还原
			a[i1][j1] = k;
		break;
		}
}

void KingLeftDown(int i, int j)
{
	for (int k = 1; i + k <= 6 && j - k >= 1; k++)
		if ((a[i + k][j - k] == -1 || a[i + k][j - k] == -2) && a[i + k + 1][j - k - 1] == 0)
		{
			a[i][j] = 0;
			int i1 = i + k;                            //a[i1][j1]是被吃的子
			int j1 = j - k;
			k = a[i1][j1];
			a[i1][j1] = 0;

			for (int k1 = 1; i1 + k1 <=7 && j1 - k1 >= 0; k1++)
				if (a[i1 + k1][j1 - k1] == 0)
				{

					int i2 = i1 + k1;
					int j2 = j1 - k1;
					a[i2][j2] = 2;

					length1 = length1 + 1;
					record1[length1][0] = i2;
					record1[length1][1] = j2;
					if (player == -1)
					{
						record1[length1][0] = 7 - i2;
						record1[length1][1] = 7 - j2;
					}

					state = 1;                          //改变状态
					King(i2, j2);

					a[i2][j2] = 0;

					findlongest();
				}
			a[i][j] = 2;						//还原
			a[i1][j1] = k;
			break;
		}

}

void KingRightUp(int i, int j)
{
	for (int k = 1; i - k >= 1 && j + k <= 6; k++)
		if ((a[i - k][j + k] == -1 || a[i - k][j + k] == -2) && a[i - k - 1][j + k + 1] == 0)
		{
			a[i][j] = 0;
			int i1 = i - k;                            //a[i1][j1]是被吃的子
			int j1 = j + k;
			k = a[i1][j1];
			a[i1][j1] = 0;

			for (int k1 = 1; i1 - k1 >= 0 && j1 + k1 <= 7; k1++)
				if (a[i1 - k1][j1 + k1] == 0)
				{

					int i2 = i1 - k1;
					int j2 = j1 + k1;
					a[i2][j2] = 2;

					length1 = length1 + 1;
					record1[length1][0] = i2;
					record1[length1][1] = j2;
					if (player == -1)
					{
						record1[length1][0] = 7 - i2;
						record1[length1][1] = 7 - j2;
					}

					state = 1;                          //改变状态
					King(i2, j2);

					a[i2][j2] = 0;

					findlongest();
				}
			a[i][j] = 2;						//还原
			a[i1][j1] = k;
			break;
		}
}

void KingRightDown(int i, int j)
{
	for (int k = 1; i + k <= 6 && j + k <=6; k++)
		if ((a[i + k][j + k] == -1 || a[i + k][j + k] == -2) && a[i + k + 1][j + k + 1] == 0)
		{
			a[i][j] = 0;
			int i1 = i + k;                            //a[i1][j1]是被吃的子
			int j1 = j + k;
			k = a[i1][j1];
			a[i1][j1] = 0;

			for (int k1 = 1; i1 + k1 <=7 && j1 + k1 <=7; k1++)
				if (a[i1 + k1][j1 + k1] == 0)
				{

					int i2 = i1 + k1;
					int j2 = j1 + k1;
					a[i2][j2] = 2;

					length1 = length1 + 1;
					record1[length1][0] = i2;
					record1[length1][1] = j2;
					if (player == -1)
					{
						record1[length1][0] = 7 - i2;
						record1[length1][1] = 7 - j2;
					}

					state = 1;                          //改变状态
					King(i2, j2);

					a[i2][j2] = 0;            //还原

					findlongest();
				}
			a[i][j] = 2;						//还原
			a[i1][j1] = k;
			break;
		}
}
