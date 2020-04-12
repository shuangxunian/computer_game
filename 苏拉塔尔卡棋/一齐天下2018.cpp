#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
int T = -1;
int main()
{
	extern int T;
    void print(int a[6][6]);
    void zouchizi(int x,int y,int c,int d,int a[6][6]);
	void zong(int a[6][6]);
    int b[6][6]={
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1 };
	int s;
    int x,y,c,d,i;
	int BBQ;
    print(b);
	puts("输入1选择上方，输入2选择下方 ");
	scanf("%d", &BBQ);
	printf("  * 开始游戏 *\n");
	printf("  1 人类先手\n");
	printf("  2 电脑先手\n");
	////////////////////////////////////////////////////
	
	
	if (BBQ == 1)
	{
		T = 1;
	printf("请输入“1”或“2”来选择先手：\n");
	scanf("%d",&s);
	if(s==1)
	{
		printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
		scanf("%d,%d",&x,&y);
		printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
		scanf("%d,%d",&c,&d);
		zouchizi(x,y,c,d,b);
		print(b);
		for(i=0;i<=100;i++)
		{
			zong(b);
		    print(b);
			printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
	     	scanf("%d,%d",&x,&y);
	    	printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
	    	scanf("%d,%d",&c,&d);
	    	zouchizi(x,y,c,d,b);
	    	print(b);

		}

	}
	else if(s==2)
	{
		zong(b);
		print(b);
        for(i=0;i<=100;i++)
		{
			printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
	     	scanf("%d,%d",&x,&y);
	    	printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
	    	scanf("%d,%d",&c,&d);
	    	zouchizi(x,y,c,d,b);
	    	print(b);
            zong(b);
		    print(b);
		}
	}
	}
	else if (BBQ == 2)
	{
		T = -1;
		printf("请输入“1”或“2”来选择先手：\n");
		scanf("%d", &s);
		if (s == 1)
		{
			printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
			scanf("%d,%d", &x, &y);
			printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
			scanf("%d,%d", &c, &d);
			zouchizi(x, y, c, d, b);
			print(b);
			for (i = 0; i <= 100; i++)
			{
				zong(b);
				print(b);
				printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
				scanf("%d,%d", &x, &y);
				printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
				scanf("%d,%d", &c, &d);
				zouchizi(x, y, c, d, b);
				print(b);

			}

		}
		else if (s == 2)
		{
			zong(b);
			print(b);
			for (i = 0; i <= 100; i++)
			{
				printf("请输入你想走的棋子的坐标(用逗号隔开):\n");
				scanf("%d,%d", &x, &y);
				printf("请输入你要走的棋子到达的坐标(用逗号隔开):\n");
				scanf("%d,%d", &c, &d);
				zouchizi(x, y, c, d, b);
				print(b);
				zong(b);
				print(b);
			}
		}
	}
    return 0;

}
void zouchizi(int x,int y,int c,int d,int a[6][6])
{
	a[c-1][d-1]=a[x-1][y-1];
	a[x-1][y-1]=0;
}
void zong(int a[6][6])
{
    void pinggu11(int a[6][6]);
	void pinggu000(int a[6][6]);
	void pinggu111(int a[6][6]);
	void pinggu1(int a[6][6]);
	int i,j,jifang=0,difang=0;                      //分别表示己方、敌方棋子个数
	int t=T;
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)
		{
			if(a[i][j]==t)
			{
				jifang++;
			}
			if(a[i][j]==-t)
			{
				difang++;
			}
		}
	}
	if((jifang==2)&&(difang>2))
	{
		pinggu11(a);                             //四层
	}
	else if((difang==2)&&(jifang>2))
	{
		pinggu111(a);
	}
	else if(jifang==1||difang==1)
	{
	    pinggu000(a);
	}
	else
	{
		pinggu1(a);
	}
}
void pinggu111(int a[6][6])              //第一层
{
    int max(int a[12]);
    int pinggu222(int a[6][6],int H[1]);
    int max11(int a[8]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int z,h=0;
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int f[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int x[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int y[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
	int H[1]={0};
	int Q,W,E,R;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu222(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu222(p,H);
					p[i-1][j]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu222(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;

				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu222(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;

				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu222(p,H);
					p[i][j+1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu222(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu222(p,H);
					p[i+1][j]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu222(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;

				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=3;
					c[8]=pinggu222(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=3;
					c[9]=pinggu222(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=3;
					c[10]=pinggu222(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=3;
					c[11]=pinggu222(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}

                d[h]=max(c);           //记录某个棋子的最大值
                f[h]=max11(c);         //记录该棋子走哪一步
                y[h]=j;x[h]=i;          //记录这个棋子的坐标
				h++;
			}
		}
	}
    z=max11(d); //记录展现最大值的是第几个棋子
	i=x[z];
	j=y[z];
	if(p[i][j]==t)
	{
		if(f[z]==0)
		{
			p[i-1][j-1]=t;
		    p[i][j]=0;
		}
		if(f[z]==1)
		{
		    p[i-1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==2)
		{
			p[i-1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==3)
		{
			p[i][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==4)
		{
			p[i][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==5)
		{
			p[i+1][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==6)
		{
			p[i+1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==7)
		{
			p[i+1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==8)
		{
			chizi01(p,i,j);
		}
		if(f[z]==9)
		{
			chizi02(p,i,j);
		}
		if(f[z]==10)
		{
			chizi03(p,i,j);
		}
		if(f[z]==11)
		{
		    chizi04(p,i,j);
		}
	}
    zhuanhuan(p,a);
}
int pinggu222(int a[6][6],int H[1])                          //第二层
{
    int min(int a[12]);
    int pinggu333(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu333(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu333(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu333(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu333(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu333(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu333(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu333(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu333(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]-2;
					c[8]=pinggu333(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]-2;
					c[9]=pinggu333(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]-2;
					c[10]=pinggu333(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]-2;
					c[11]=pinggu333(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

				d[h]=min(c);
                h++;
			}
		}
	}
    z=min(d);
	return z;
}
int pinggu333(int a[6][6],int H[1])                        //第三层
{
    int pinggu0(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int max(int a[12]);
    int z,h=0;
    int J=H[0];
	int p[6][6];
	int P[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu0(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu0(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu0(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu0(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu0(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu0(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu0(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu0(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]+3;
					c[8]=pinggu0(p,H);
					 H[0]=J;
					zhuanhuan(P,p);
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]+3;
					c[9]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]+3;
					c[10]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]+3;
					c[11]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

                d[h]=max(c);
				h++;
			}
		}
	}
	z=max(d);
	return z;
}
void pinggu000(int a[6][6])              //第一层
{
    void pinggu001(int a[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);
    int i,j;
	int Q,W,E,R;
	int t=T;
    int flag=0;
	for(i=0;i<=5;i++)
	{

	   	for(j=0;j<=5;j++)
		{
			if(a[i][j]==t)
			{
				Q=chizi1(a,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(a,i,j);
					flag=1;
					break;
				}
				W=chizi2(a,i,j);
				if(W==30)
				{
					chizi02(a,i,j);
					flag=1;
					break;
				}
				E=chizi3(a,i,j);
				if(E==30)
				{
					chizi03(a,i,j);
					flag=1;
					break;
				}
				R=chizi4(a,i,j);
				if(R==30)
				{
					chizi04(a,i,j);
					flag=1;
					break;
				}
			}
		}
		if(flag==1)
        {
            break;
        }
	}
	if(flag==0)
	{
	    pinggu001(a);
	    flag=1;
	}
}
void pinggu001(int a[6][6])              //第一层
{
    int max(int a[12]);
    int pinggu002(int a[6][6],int H[1]);
    int max11(int a[8]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int z,h=0;
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int f[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int x[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int y[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
	int H[1]={0};
	int Q,W,E,R;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu002(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu002(p,H);
					p[i-1][j]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu002(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;

				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu002(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;

				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu002(p,H);
					p[i][j+1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu002(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu002(p,H);
					p[i+1][j]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu002(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;

				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=3;
					c[8]=pinggu002(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=3;
					c[9]=pinggu002(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=3;
					c[10]=pinggu002(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=3;
					c[11]=pinggu002(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}

                d[h]=max(c);           //记录某个棋子的最大值
                f[h]=max11(c);         //记录该棋子走哪一步
                y[h]=j;x[h]=i;          //记录这个棋子的坐标
				h++;
			}
		}
	}
    z=max11(d); //记录展现最大值的是第几个棋子
	i=x[z];
	j=y[z];
	if(p[i][j]==t)
	{
		if(f[z]==0)
		{
			p[i-1][j-1]=t;
		    p[i][j]=0;
		}
		if(f[z]==1)
		{
		    p[i-1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==2)
		{
			p[i-1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==3)
		{
			p[i][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==4)
		{
			p[i][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==5)
		{
			p[i+1][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==6)
		{
			p[i+1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==7)
		{
			p[i+1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==8)
		{
			chizi01(p,i,j);
		}
		if(f[z]==9)
		{
			chizi02(p,i,j);
		}
		if(f[z]==10)
		{
			chizi03(p,i,j);
		}
		if(f[z]==11)
		{
		    chizi04(p,i,j);
		}
	}
    zhuanhuan(p,a);
}
int pinggu002(int a[6][6],int H[1])                          //第二层
{
    int min(int a[12]);
    int pinggu0(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu0(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu0(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu0(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu0(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu0(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu0(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu0(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu0(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]-2;
					c[8]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]-2;
					c[9]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]-2;
					c[10]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]-2;
					c[11]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

				d[h]=min(c);
                h++;
			}
		}
	}
    z=min(d);
	return z;
}
void pinggu11(int a[6][6])              //第一层
{
    int max(int a[12]);
    int pinggu22(int a[6][6],int H[1]);
    int max11(int a[8]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int z,h=0;
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int f[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int x[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int y[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
	int H[1]={0};
	int Q,W,E,R;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu22(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu22(p,H);
					p[i-1][j]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu22(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;

				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu22(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;

				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu22(p,H);
					p[i][j+1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu22(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu22(p,H);
					p[i+1][j]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu22(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;

				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=3;
					c[8]=pinggu22(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=3;
					c[9]=pinggu22(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=3;
					c[10]=pinggu22(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=3;
					c[11]=pinggu22(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}

                d[h]=max(c);           //记录某个棋子的最大值
                f[h]=max11(c);         //记录该棋子走哪一步
                y[h]=j;x[h]=i;          //记录这个棋子的坐标
				h++;
			}
		}
	}
    z=max11(d); //记录展现最大值的是第几个棋子
	i=x[z];
	j=y[z];
	if(p[i][j]==t)
	{
		if(f[z]==0)
		{
			p[i-1][j-1]=t;
		    p[i][j]=0;
		}
		if(f[z]==1)
		{
		    p[i-1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==2)
		{
			p[i-1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==3)
		{
			p[i][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==4)
		{
			p[i][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==5)
		{
			p[i+1][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==6)
		{
			p[i+1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==7)
		{
			p[i+1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==8)
		{
			chizi01(p,i,j);
		}
		if(f[z]==9)
		{
			chizi02(p,i,j);
		}
		if(f[z]==10)
		{
			chizi03(p,i,j);
		}
		if(f[z]==11)
		{
		    chizi04(p,i,j);
		}
	}
    zhuanhuan(p,a);
}
int pinggu22(int a[6][6],int H[1])                          //第二层
{
    int min(int a[12]);
    int pinggu33(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu33(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu33(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu33(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu33(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu33(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu33(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu33(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu33(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]-2;
					c[8]=pinggu33(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]-2;
					c[9]=pinggu33(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]-2;
					c[10]=pinggu33(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]-2;
					c[11]=pinggu33(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

				d[h]=min(c);
                h++;
			}
		}
	}
    z=min(d);
	return z;
}
int pinggu33(int a[6][6],int H[1])                        //第三层
{
    int pinggu44(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int max(int a[12]);
    int z,h=0;
    int J=H[0];
	int p[6][6];
	int P[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu44(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu44(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu44(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu44(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu44(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu44(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu44(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu44(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]+3;
					c[8]=pinggu44(p,H);
					 H[0]=J;
					zhuanhuan(P,p);
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]+3;
					c[9]=pinggu44(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]+3;
					c[10]=pinggu44(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]+3;
					c[11]=pinggu44(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

                d[h]=max(c);
				h++;
			}
		}
	}
	z=max(d);
	return z;
}
int pinggu44(int a[6][6],int H[1])                          //第4层
{
    int min(int a[12]);
    int pinggu0(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu0(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu0(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu0(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu0(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu0(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu0(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu0(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu0(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=H[0]-2;
					c[8]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=H[0]-2;
					c[9]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=H[0]-2;
					c[10]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=H[0]-2;
					c[11]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}

				d[h]=min(c);
                h++;
			}
		}
	}
    z=min(d);
	return z;
}
void pinggu1(int a[6][6])              //第一层
{
    int max(int a[8]);
    int pinggu2(int a[6][6],int H[1]);
    int max11(int a[8]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int z,h=0;
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int f[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int x[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int y[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
	int H[1]={0};
	int Q,W,E,R;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu2(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu2(p,H);
					p[i-1][j]=0;
					p[i][j]=t;

				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu2(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;

				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu2(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;

				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu2(p,H);
					p[i][j+1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu2(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu2(p,H);
					p[i+1][j]=0;
					p[i][j]=t;

				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu2(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;

				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					chizi01(p,i,j);
					H[0]=2;
					c[8]=pinggu2(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					chizi02(p,i,j);
					H[0]=2;
					c[9]=pinggu2(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					chizi03(p,i,j);
					H[0]=2;
					c[10]=pinggu2(p,H);
					zhuanhuan(P,p);
                    H[0]=0;

				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					chizi04(p,i,j);
					H[0]=2;
					c[11]=pinggu2(p,H);
					zhuanhuan(P,p);
                    H[0]=0;
				}

                d[h]=max(c);           //记录某个棋子的最大值
                f[h]=max11(c);         //记录该棋子走哪一步
                y[h]=j;x[h]=i;          //记录这个棋子的坐标
				h++;
			}
		}
	}
    z=max11(d); //记录展现最大值的是第几个棋子
	i=x[z];
	j=y[z];
	if(p[i][j]==t)
	{
		if(f[z]==0)
		{
			p[i-1][j-1]=t;
		    p[i][j]=0;
		}
		if(f[z]==1)
		{
		    p[i-1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==2)
		{
			p[i-1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==3)
		{
			p[i][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==4)
		{
			p[i][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==5)
		{
			p[i+1][j-1]=t;
			p[i][j]=0;
		}
		if(f[z]==6)
		{
			p[i+1][j]=t;
			p[i][j]=0;
		}
		if(f[z]==7)
		{
			p[i+1][j+1]=t;
			p[i][j]=0;
		}
		if(f[z]==8)
		{
			chizi01(p,i,j);
		}
		if(f[z]==9)
		{
			chizi02(p,i,j);
		}
		if(f[z]==10)
		{
			chizi03(p,i,j);
		}
		if(f[z]==11)
		{
		    chizi04(p,i,j);
		}
	}
    zhuanhuan(p,a);
}
int pinggu2(int a[6][6],int H[1])                          //第2层
{
    int min(int a[12]);
    int pinggu3(int a[6][6],int H[1]);
	int pinggu03(int a[6][6],int H[1],int C[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[0]=pinggu3(p,H);
						B[0]=c[0];
					}
					else
					{
						c[0]=pinggu03(p,H,B);
						B[0]=c[0];
					}
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[1]=pinggu3(p,H);
						B[0]=c[1];
					}
					else
					{
						c[1]=pinggu03(p,H,B);
						B[0]=c[1];
					}
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[2]=pinggu3(p,H);
						B[0]=c[2];
					}
					else
					{
						c[2]=pinggu03(p,H,B);
						B[0]=c[2];
					}
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[3]=pinggu3(p,H);
						B[0]=c[3];
					}
					else
					{
						c[3]=pinggu03(p,H,B);
						B[0]=c[3];
					}
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[4]=pinggu3(p,H);
						B[0]=c[4];
					}
					else
					{
						c[4]=pinggu03(p,H,B);
						B[0]=c[4];
					}
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[5]=pinggu3(p,H);
						B[0]=c[5];
					}
					else
					{
						c[5]=pinggu03(p,H,B);
						B[0]=c[5];
					}
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[6]=pinggu3(p,H);
						B[0]=c[6];
					}
					else
					{
						c[6]=pinggu03(p,H,B);
						B[0]=c[6];
					}
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[7]=pinggu3(p,H);
						B[0]=c[7];
					}
					else
					{
						c[7]=pinggu03(p,H,B);
						B[0]=c[7];
					}
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[8]=pinggu3(p,H);
						B[0]=c[8];
					}
					else
					{
						c[8]=pinggu03(p,H,B);
						B[0]=c[8];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[9]=pinggu3(p,H);
						B[0]=c[9];
					}
					else
					{
						c[9]=pinggu03(p,H,B);
						B[0]=c[9];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[10]=pinggu3(p,H);
						B[0]=c[10];
					}
					else
					{
						c[10]=pinggu03(p,H,B);
						B[0]=c[10];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[11]=pinggu3(p,H);
						B[0]=c[11];
					}
					else
					{
						c[11]=pinggu03(p,H,B);
						B[0]=c[11];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				d[h]=min(c);
                h++;
			}
		}
	}
    z=min(d);
	return z;
}
int pinggu3(int a[6][6],int H[1])                        //第3层
{
    int pinggu4(int a[6][6],int H[1]);
	int pinggu04(int a[6][6],int H[1],int C[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int max(int a[12]);
    int z,h=0;
    int J=H[0];
	int p[6][6];
	int P[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[0]=pinggu4(p,H);
						B[0]=c[0];
					}
					else
					{
						c[0]=pinggu04(p,H,B);
						B[0]=c[0];
					}
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[1]=pinggu4(p,H);
						B[0]=c[1];
					}
					else
					{
						c[1]=pinggu04(p,H,B);
						B[0]=c[1];
					}
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[2]=pinggu4(p,H);
						B[0]=c[2];
					}
					else
					{
						c[2]=pinggu04(p,H,B);
						B[0]=c[2];
					}
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[3]=pinggu4(p,H);
						B[0]=c[3];
					}
					else
					{
						c[3]=pinggu04(p,H,B);
						B[0]=c[3];
					}
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[4]=pinggu4(p,H);
						B[0]=c[4];
					}
					else
					{
						c[4]=pinggu04(p,H,B);
						B[0]=c[4];
					}
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[5]=pinggu4(p,H);
						B[0]=c[5];
					}
					else
					{
						c[5]=pinggu04(p,H,B);
						B[0]=c[5];
					}
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[6]=pinggu4(p,H);
						B[0]=c[6];
					}
					else
					{
						c[6]=pinggu04(p,H,B);
						B[0]=c[6];
					}
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[7]=pinggu4(p,H);
						B[0]=c[7];
					}
					else
					{
						c[7]=pinggu04(p,H,B);
						B[0]=c[7];
					}
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[8]=pinggu4(p,H);
						B[0]=c[8];
					}
					else
					{
						c[8]=pinggu04(p,H,B);
						B[0]=c[8];
					}
					 H[0]=J;
					zhuanhuan(P,p);
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[9]=pinggu4(p,H);
						B[0]=c[9];
					}
					else
					{
						c[9]=pinggu04(p,H,B);
						B[0]=c[9];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[10]=pinggu4(p,H);
						B[0]=c[10];
					}
					else
					{
						c[10]=pinggu04(p,H,B);
						B[0]=c[10];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[11]=pinggu4(p,H);
						B[0]=c[11];
					}
					else
					{
						c[11]=pinggu04(p,H,B);
						B[0]=c[11];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
                d[h]=max(c);
				h++;
			}
		}
	}
	z=max(d);
	return z;
}
int pinggu03(int a[6][6],int H[1],int C[1])                          //第3层
{
	int max2(int a[120]);
    int pinggu4(int a[6][6],int H[1]);
    int pinggu04(int a[6][6],int H[1],int C[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int P[6][6];
	int MAX[120]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int K;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[0]=pinggu4(p,H);
						B[0]=c[0];
					}
					else
					{
						c[0]=pinggu04(p,H,B);
						B[0]=c[0];
					}
					p[i-1][j-1]=0;
					p[i][j]=t;
					if(c[0]<=C[0])
					{
						MAX[A]=c[0];
					}
					else
					{
						return C[0];                                                                               //返回C【0】
					}
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[1]=pinggu4(p,H);
						B[0]=c[1];
					}
					else
					{
						c[1]=pinggu04(p,H,B);
						B[0]=c[1];
					}
					p[i-1][j]=0;
					p[i][j]=t;
					if(c[1]<=C[0])
					{
						MAX[A]=c[1];
					}
					else
					{
						return C[0];
					}
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[2]=pinggu4(p,H);
						B[0]=c[2];
					}
					else
					{
						c[2]=pinggu04(p,H,B);
						B[0]=c[2];
					}
                    p[i-1][j+1]=0;
					p[i][j]=t;
					if(c[2]<=C[0])
					{
						MAX[A]=c[2];
					}
					else
					{
						return C[0];
					}
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[3]=pinggu4(p,H);
						B[0]=c[3];
					}
					else
					{
						c[3]=pinggu04(p,H,B);
						B[0]=c[3];
					}
                    p[i][j-1]=0;
					p[i][j]=t;
					if(c[3]<=C[0])
					{
						MAX[A]=c[3];
					}
					else
					{
						return C[0];
					}
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[4]=pinggu4(p,H);
						B[0]=c[4];
					}
					else
					{
						c[4]=pinggu04(p,H,B);
						B[0]=c[4];
					}
					p[i][j+1]=0;
					p[i][j]=t;
					if(c[4]<=C[0])
					{
						MAX[A]=c[4];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[5]=pinggu4(p,H);
						B[0]=c[5];
					}
					else
					{
						c[5]=pinggu04(p,H,B);
						B[0]=c[5];
					}
					p[i+1][j-1]=0;
					p[i][j]=t;
					if(c[5]<=C[0])
					{
						MAX[A]=c[5];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[6]=pinggu4(p,H);
						B[0]=c[6];
					}
					else
					{
						c[6]=pinggu04(p,H,B);
						B[0]=c[6];
					}
					p[i+1][j]=0;
					p[i][j]=t;
					if(c[6]<=C[0])
					{
						MAX[A]=c[6];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[7]=pinggu4(p,H);
						B[0]=c[7];
					}
					else
					{
						c[7]=pinggu04(p,H,B);
						B[0]=c[7];
					}
					p[i+1][j+1]=0;
					p[i][j]=t;
					if(c[7]<=C[0])
					{
						MAX[A]=c[7];
					}
					else
					{
						return C[0];
					}
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[8]=pinggu4(p,H);
						B[0]=c[8];
					}
					else
					{
						c[8]=pinggu04(p,H,B);
						B[0]=c[8];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[8]<=C[0])
					{
						MAX[A]=c[8];
					}
					else
					{
						return C[0];
					}
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[9]=pinggu4(p,H);
						B[0]=c[9];
					}
					else
					{
						c[9]=pinggu04(p,H,B);
						B[0]=c[9];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[9]<=C[0])
					{
						MAX[A]=c[9];
					}
					else
					{
						return C[0];
					}
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[10]=pinggu4(p,H);
						B[0]=c[10];
					}
					else
					{
						c[10]=pinggu04(p,H,B);
						B[0]=c[10];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[10]<=C[0])
					{
						MAX[A]=c[10];
					}
					else
					{
						return C[0];
					}
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]+3;
					if(A==1)
					{
						c[11]=pinggu4(p,H);
						B[0]=c[11];
					}
					else
					{
						c[11]=pinggu04(p,H,B);
						B[0]=c[11];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[11]<=C[0])
					{
						MAX[A]=c[11];
					}
					else
					{
						return C[0];
					}
				}
                h++;
			}
		}
	}
	K=max2(MAX);
	return K;
}
int pinggu4(int a[6][6],int H[1])                          //第4层
{
    int min(int a[12]);
    int pinggu5(int a[6][6],int H[1]);
	int pinggu05(int a[6][6],int H[1],int C[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int z,h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[0]=pinggu5(p,H);
						B[0]=c[0];
					}
					else
					{
						c[0]=pinggu05(p,H,B);
						B[0]=c[0];
					}
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[1]=pinggu5(p,H);
						B[0]=c[1];
					}
					else
					{
						c[1]=pinggu05(p,H,B);
						B[0]=c[1];
					}
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[2]=pinggu5(p,H);
						B[0]=c[2];
					}
					else
					{
						c[2]=pinggu05(p,H,B);
						B[0]=c[2];
					}
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[3]=pinggu5(p,H);
						B[0]=c[3];
					}
					else
					{
						c[3]=pinggu05(p,H,B);
						B[0]=c[3];
					}
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[4]=pinggu5(p,H);
						B[0]=c[4];
					}
					else
					{
						c[4]=pinggu05(p,H,B);
						B[0]=c[4];
					}
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[5]=pinggu5(p,H);
						B[0]=c[5];
					}
					else
					{
						c[5]=pinggu05(p,H,B);
						B[0]=c[5];
					}
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[6]=pinggu5(p,H);
						B[0]=c[6];
					}
					else
					{
						c[6]=pinggu05(p,H,B);
						B[0]=c[6];
					}
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[7]=pinggu5(p,H);
						B[0]=c[7];
					}
					else
					{
						c[7]=pinggu05(p,H,B);
						B[0]=c[7];
					}
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[8]=pinggu5(p,H);
						B[0]=c[8];
					}
					else
					{
						c[8]=pinggu05(p,H,B);
						B[0]=c[8];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[9]=pinggu5(p,H);
						B[0]=c[9];
					}
					else
					{
						c[9]=pinggu05(p,H,B);
						B[0]=c[9];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[10]=pinggu5(p,H);
						B[0]=c[10];
					}
					else
					{
						c[10]=pinggu05(p,H,B);
						B[0]=c[10];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[11]=pinggu5(p,H);
						B[0]=c[11];
					}
					else
					{
						c[11]=pinggu05(p,H,B);
						B[0]=c[11];
					}
					zhuanhuan(P,p);
					H[0]=J;
				}
                h++;
			}
		}
	}
    z=B[0];
	return z;
}
int pinggu04(int a[6][6],int H[1],int C[1])                          //第4层
{
	int min2(int a[120]);
    int pinggu5(int a[6][6],int H[1]);
	int pinggu05(int a[6][6],int H[1],int C[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int K;
    int MAX[120]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int P[6][6];
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=-T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[0]=pinggu5(p,H);
						B[0]=c[0];
					}
					else
					{
						c[0]=pinggu05(p,H,B);
						B[0]=c[0];
					}
					p[i-1][j-1]=0;
					p[i][j]=t;
					if(c[0]>=C[0])
					{
						MAX[A]=c[0];
					}
					else
					{
						return C[0];
					}
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[1]=pinggu5(p,H);
						B[0]=c[1];
					}
					else
					{
						c[1]=pinggu05(p,H,B);
						B[0]=c[1];
					}
					p[i-1][j]=0;
					p[i][j]=t;
					if(c[1]>=C[0])
					{
						MAX[A]=c[1];
					}
					else
					{
						return C[0];
					}
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[2]=pinggu5(p,H);
						B[0]=c[2];
					}
					else
					{
						c[2]=pinggu05(p,H,B);
						B[0]=c[2];
					}
                    p[i-1][j+1]=0;
					p[i][j]=t;
					if(c[2]>=C[0])
					{
						MAX[A]=c[2];
					}
					else
					{
						return C[0];
					}
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[3]=pinggu5(p,H);
						B[0]=c[3];
					}
					else
					{
						c[3]=pinggu05(p,H,B);
						B[0]=c[3];
					}
                    p[i][j-1]=0;
					p[i][j]=t;
					if(c[3]>=C[0])
					{
						MAX[A]=c[3];
					}
					else
					{
						return C[0];
					}
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[4]=pinggu5(p,H);
						B[0]=c[4];
					}
					else
					{
						c[4]=pinggu05(p,H,B);
						B[0]=c[4];
					}
					p[i][j+1]=0;
					p[i][j]=t;
					if(c[4]>=C[0])
					{
						MAX[A]=c[4];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[5]=pinggu5(p,H);
						B[0]=c[5];
					}
					else
					{
						c[5]=pinggu05(p,H,B);
						B[0]=c[5];
					}
					p[i+1][j-1]=0;
					p[i][j]=t;
					if(c[5]>=C[0])
					{
						MAX[A]=c[5];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[6]=pinggu5(p,H);
						B[0]=c[6];
					}
					else
					{
						c[6]=pinggu05(p,H,B);
						B[0]=c[6];
					}
					p[i+1][j]=0;
					p[i][j]=t;
					if(c[6]>=C[0])
					{
						MAX[A]=c[6];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					if(A==1)
					{
						c[7]=pinggu5(p,H);
						B[0]=c[7];
					}
					else
					{
						c[7]=pinggu05(p,H,B);
						B[0]=c[7];
					}


					p[i+1][j+1]=0;
					p[i][j]=t;
					if(c[7]>=C[0])
					{
						MAX[A]=c[7];
					}
					else
				    {
					   	return C[0];
				   	}

				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[8]=pinggu5(p,H);
						B[0]=c[8];
					}
					else
					{
						c[8]=pinggu05(p,H,B);
						B[0]=c[8];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[8]>=C[0])
					{
						MAX[A]=c[8];
					}
					else
					{
						return C[0];
					}
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[9]=pinggu5(p,H);
						B[0]=c[9];
					}
					else
					{
						c[9]=pinggu05(p,H,B);
						B[0]=c[9];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[9]>=C[0])
					{
						MAX[A]=c[9];
					}
					else
					{
						return C[0];
					}
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[10]=pinggu5(p,H);
						B[0]=c[10];
					}
					else
					{
						c[10]=pinggu05(p,H,B);
						B[0]=c[10];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[10]>=C[0])
					{
						MAX[A]=c[10];
					}
					else
					{
						return C[0];
					}
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]-2;
					if(A==1)
					{
						c[11]=pinggu5(p,H);
						B[0]=c[11];
					}
					else
					{
						c[11]=pinggu05(p,H,B);
						B[0]=c[11];
					}
					zhuanhuan(P,p);
					H[0]=J;
					if(c[11]>=C[0])
					{
						MAX[A]=c[11];
					}
					else
					{
						return C[0];
					}
				}
                h++;
			}
		}
	}
    K=min2(MAX);
	return K;
}
int pinggu5(int a[6][6],int H[1])                        //第5层
{
    int pinggu0(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int max(int a[12]);
    int z,h=0;
    int J=H[0];
	int p[6][6];
	int P[6][6];
    int i,j;
	int d[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu0(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu0(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu0(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu0(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu0(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu0(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu0(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu0(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]+3;
					c[8]=pinggu0(p,H);
					 H[0]=J;
					zhuanhuan(P,p);
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]+3;
					c[9]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]+3;
					c[10]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]+3;
					c[11]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
				}
                d[h]=max(c);
				h++;
			}
		}
	}
	z=max(d);
	return z;
}
int pinggu05(int a[6][6],int H[1],int C[1])                          //第5层
{
	int max2(int a[120]);
    int pinggu0(int a[6][6],int H[1]);
    void zhuanhuan(int a[6][6],int b[6][6]);
    int chizi1(int a[6][6],int x,int y);
    int chizi2(int a[6][6],int x,int y);
    int chizi3(int a[6][6],int x,int y);
    int chizi4(int a[6][6],int x,int y);
    void chizi01(int a[6][6],int x,int y);
    void chizi02(int a[6][6],int x,int y);
    void chizi03(int a[6][6],int x,int y);
    void chizi04(int a[6][6],int x,int y);

    int Q,W,E,R;
    int h=0;
    int J=H[0];
	int p[6][6];
    int i,j;
	int P[6][6];
	int MAX[120]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int K;
    for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			p[i][j]=a[i][j];
		}
	}
	int t=T;
	int A=0,B[1]={0};
	for(i=0;i<=5;i++)
	{
	   	for(j=0;j<=5;j++)
		{
			if(p[i][j]==t)
			{
                int c[12]={0,0,0,0,0,0,0,0,0,0,0,0};
				if(((i-1)>=0&&(j-1)>=0)&&p[i-1][j-1]==0)
				{
					A++;
					p[i-1][j-1]=t;
					p[i][j]=0;
					c[0]=pinggu0(p,H);
					p[i-1][j-1]=0;
					p[i][j]=t;
					if(c[0]<=C[0])
					{
						MAX[A]=c[0];
					}
					else
					{
						return C[0];                                                                               //返回C【0】
					}
				}
				if(((i-1)>=0)&&p[i-1][j]==0)
				{
					A++;
					p[i-1][j]=t;
					p[i][j]=0;
					c[1]=pinggu0(p,H);
					p[i-1][j]=0;
					p[i][j]=t;
					if(c[1]<=C[0])
					{
						MAX[A]=c[1];
					}
					else
					{
						return C[0];
					}
				}
				if(((i-1)>=0&&(j+1)<=5)&&p[i-1][j+1]==0)
				{
					A++;
					p[i-1][j+1]=t;
					p[i][j]=0;
					c[2]=pinggu0(p,H);
                    p[i-1][j+1]=0;
					p[i][j]=t;
					if(c[2]<=C[0])
					{
						MAX[A]=c[2];
					}
					else
					{
						return C[0];
					}
				}
				if((j-1)>=0&&p[i][j-1]==0)
				{
					A++;
					p[i][j-1]=t;
					p[i][j]=0;
					c[3]=pinggu0(p,H);
                    p[i][j-1]=0;
					p[i][j]=t;
					if(c[3]<=C[0])
					{
						MAX[A]=c[3];
					}
					else
					{
						return C[0];
					}
				}
				if(((j+1)<=5)&&p[i][j+1]==0)
				{
					A++;
					p[i][j+1]=t;
					p[i][j]=0;
					c[4]=pinggu0(p,H);
					p[i][j+1]=0;
					p[i][j]=t;
					if(c[4]<=C[0])
					{
						MAX[A]=c[4];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j-1)>=0)&&p[i+1][j-1]==0)
				{
					A++;
					p[i+1][j-1]=t;
					p[i][j]=0;
					c[5]=pinggu0(p,H);
					p[i+1][j-1]=0;
					p[i][j]=t;
					if(c[5]<=C[0])
					{
						MAX[A]=c[5];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5)&&p[i+1][j]==0)
				{
					A++;
					p[i+1][j]=t;
					p[i][j]=0;
					c[6]=pinggu0(p,H);
					p[i+1][j]=0;
					p[i][j]=t;
					if(c[6]<=C[0])
					{
						MAX[A]=c[6];
					}
					else
					{
						return C[0];
					}
				}
				if(((i+1)<=5&&(j+1)<=5)&&p[i+1][j+1]==0)
				{
					A++;
					p[i+1][j+1]=t;
					p[i][j]=0;
					c[7]=pinggu0(p,H);
					p[i+1][j+1]=0;
					p[i][j]=t;
					if(c[7]<=C[0])
					{
						MAX[A]=c[7];
					}
					else
					{
						return C[0];
					}
				}

				zhuanhuan(p,P);                              //储存
				Q=chizi1(p,i,j);                            //判断吃子*4
				if(Q==30)
				{
					A++;
					chizi01(p,i,j);
					H[0]=H[0]+3;
					c[8]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
					if(c[8]<=C[0])
					{
						MAX[A]=c[8];
					}
					else
					{
						return C[0];
					}
				}
				W=chizi2(p,i,j);
				if(W==30)
				{
					A++;
					chizi02(p,i,j);
					H[0]=H[0]+3;
					c[9]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
					if(c[9]<=C[0])
					{
						MAX[A]=c[9];
					}
					else
					{
						return C[0];
					}
				}
				E=chizi3(p,i,j);
				if(E==30)
				{
					A++;
					chizi03(p,i,j);
					H[0]=H[0]+3;
					c[10]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
					if(c[10]<=C[0])
					{
						MAX[A]=c[10];
					}
					else
					{
						return C[0];
					}
				}
				R=chizi4(p,i,j);
				if(R==30)
				{
					A++;
					chizi04(p,i,j);
					H[0]=H[0]+3;
					c[11]=pinggu0(p,H);
					zhuanhuan(P,p);
					H[0]=J;
					if(c[11]<=C[0])
					{
						MAX[A]=c[11];
					}
					else
					{
						return C[0];
					}
				}
                h++;
			}
		}
	}
	K=max2(MAX);
	return K;
}
int max(int a[12])                     //判断评估最大值                                //正确
{
    int max,i;
	for(i=0;i<=11;i++)
	{
		if(a[i]!=0)
		{
			max=a[i];
			break;
		}
        else
		{
			max=0;
		}
	}
	for(;i<=11;i++)
	{
		if((a[i]>=max)&&a[i]!=0)
		{
			max=a[i];
		}
		else continue;
	}
	return max;
}
int max2(int a[60])
{
    int max,i;
	for(i=0;i<60;i++)
	{
		if(a[i]!=0)
		{
			max=a[i];
			break;
		}
        else
		{
			max=0;
		}
	}
	for(;i<60;i++)
	{
		if((a[i]>=max)&&a[i]!=0)
		{
			max=a[i];
		}
		else continue;
	}
	return max;
}
int min(int a[12])                     //判断评估最小值                                //正确
{
    int min,i;
	for(i=0;i<=11;i++)
	{
		if(a[i]!=0)
		{
			min=a[i];
			break;
		}
        else
		{
			min=0;
		}
	}
	for(;i<=11;i++)
	{
		if((a[i]<=min)&&a[i]!=0)
		{
			min=a[i];
		}
		else continue;
	}
	return min;
}
int max11(int a[12])                   //返回最大值对应的数组                            //正确
{
    int max,i,t=0;
	for(i=0;i<=11;i++)
	{
	    if(a[i]!=0)
	    {
			max=a[i];
		    t=i;
			break;
	    }
        else
	  	{
		   	max=0;
		   	t=i;
	   	}
   	}
	for(;i<=11;i++)
	{
		if((a[i]>=max)&&a[i]!=0)
		{
			max=a[i];
			t=i;
		}
		else continue;
	}

	return t;
}
int min2(int a[120])
{
    int min,i;
	for(i=0;i<120;i++)
	{
		if(a[i]!=0)
		{
			min=a[i];
			break;
		}
        else
		{
			min=0;
		}
	}
	for(;i<120;i++)
	{
		if((a[i]<=min)&&a[i]!=0)
		{
			min=a[i];
		}
		else continue;
	}
	return min;
}
int pinggu0(int a[6][6],int H[1])          //评估函数    7.19
{
	//统计敌方和己方棋子数
    int i,j,jifang=0,difang=0;                      //分别表示己方、敌方棋子个数
	int t=T;                                        //T为初始设置的己方棋子，是全局变量
	int z=0,h=0;                                    //移动范围。 z表示己方，h表示敌方
    int b[6][6]={{25,35,35,35,35,25},{35,70,50,50,70,35},{35,50,60,60,50,35},{35,50,60,60,50,35},{35,70,50,50,70,35},{25,35,35,35,35,25}};
	                                                //前期棋盘盘面价值
	int c[6][6]={{15,35,35,35,35,15},{35,60,50,50,60,35},{35,50,60,60,50,35},{35,50,60,60,50,35},{35,60,50,50,60,35},{15,35,35,35,35,15}};
	                                                //中期棋盘盘面价值
    int d[6][6]={{8,35,35,35,35,8},{35,50,70,70,50,35},{35,70,60,60,70,35},{35,70,60,60,70,35},{35,50,70,70,50,35},{8,35,35,35,35,8}};
                                                    //后期棋盘盘面价值
	int v=0,u=0;                                    //盘面价值估分,v表示己方，u表示敌方
	int k;
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)
		{
			if(a[i][j]==t)
			{
				jifang++;
			}
			if(a[i][j]==-t)
			{
				difang++;
			}
		}
	}
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)
		{
			if(a[i][j]==t)
			{
				if((i-1)>=0)
				{
					if(a[i-1][j]==0)
					{
						z++;
					}
				}
				if((i+1)<=5)
				{
					if(a[i+1][j]==0)
					{
						z++;
					}
				}
				if((j-1)>=0)
				{
					if(a[i][j-1]==0)
					{
						z++;
					}
				}
				if((j+1)<=5)
				{
					if(a[i][j+1]==0)
					{
						z++;
					}
				}
	            if((i-1)>=0&&(j-1)>=0)
				{
					if(a[i-1][j-1]==0)
					{
						z++;
					}
				}
				if((i-1)>=0&&(j+1)<=5)
				{
					if(a[i-1][j+1]==0)
					{
						z++;
					}
				}
				if((i+1)<=5&&(j+1)<=5)
				{
					if(a[i+1][j+1]==0)
					{
						z++;
					}
				}
				if((i+1)<=5&&(j-1)>=0)
				{
					if(a[i+1][j-1]==0)
					{
						z++;
					}
				}                                       //己方移动范围
				if(jifang>8)                            //己方盘面价值
				{
					v=v+b[i][j];
				}
				if(jifang<=8&&jifang>4)
				{
					v=v+c[i][j];
				}
				if(jifang<=4)
				{
					v=v+d[i][j];
				}
			}
			if(a[i][j]==-t)
			{
				if((i-1)>=0)
				{
					if(a[i-1][j]==0)
					{
						h++;
					}
				}
				if((i+1)<=5)
				{
					if(a[i+1][j]==0)
					{
						h++;
					}
				}
				if((j-1)>=0)
				{
					if(a[i][j-1]==0)
					{
						h++;
					}
				}
				if((j+1)<=5)
				{
					if(a[i][j+1]==0)
					{
						h++;
					}
				}
	            if((i-1)>=0&&(j-1)>=0)
				{
					if(a[i-1][j-1]==0)
					{
						h++;
					}
				}
				if((i-1)>=0&&(j+1)<=5)
				{
					if(a[i-1][j+1]==0)
					{
						h++;
					}
				}
				if((i+1)<=5&&(j+1)<=5)
				{
					if(a[i+1][j+1]==0)
					{
						h++;
					}
				}
				if((i+1)<=5&&(j-1)>=0)
				{
					if(a[i+1][j-1]==0)
					{
						h++;
					}
				}
				if(difang>8)
				{
					u=u+b[i][j];
				}
				if(difang<=8&&difang>4)
				{
					u=u+c[i][j];
				}
				if(jifang<=4)
				{
					u=u+d[i][j];
				}
			}
		}
	}
	if((jifang+difang)>18)
	{
		k=(jifang*200+z*6+v*4)-(difang*100+h*3+u*2)+H[0]*60;
	}
	if((jifang+difang)>8&&(jifang+difang)<=18)
	{
		k=(jifang*200+z*6+v*4)-(difang*100+h*3+u*2)+H[0]*150;
	}
	if((jifang+difang)<=8)
	{
	    k=(jifang*240+z*6+v*4)-(difang*100+h*3+u*2)+H[0]*150;
	}
	return k;
}
void zhuanhuan(int a[6][6],int b[6][6])           //把a赋值到b（转移棋盘）
{
	int i,j;
	for(i=0;i<=5;i++)
	{
		for(j=0;j<=5;j++)
		{
			b[i][j]=a[i][j];
		}
	}
}

