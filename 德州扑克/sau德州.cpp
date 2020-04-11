//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//德州扑克组负责人唐家杰所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//唐家杰	QQ：1714527126 
#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define NumberOfMatches 70//对局回合
int AllCards[9][2]; //所有牌 可能不真实
int HandCards[2][2];//手牌
int FieldCards[5][2];	//场牌
int KeepCards[7][2];//虚假的这也是
int BlindNote=0;   //小盲0大盲1
int Stage;   //第几阶段  如preflop,turn,river
int Increase;
int SUMbj;
int SUMbh;
int bujie=0;
	char sendBuf[100]="\0";
	char recvBuf[100]="\0";
	char opBuf[100]="\0";
#pragma comment(lib,"ws2_32.lib")
void zhenghe(int nnn);	
void observe(char Colour,char Size,int Round,int Number);
void observes(char Colour,char Size1,char Size2,int Round,int Number);
void oppo();
void pref();
void flop();
void turn();
void rive();
void chec();
void call();
void alli();
void rais();
int PAIXING;
int PAIDIAN;
void zuida();
int pdsy();
void jsjz();
void sjfp(int stage);
int shenglishu(int nnn);
void delay()
{
	int i,j,k;
	int n=0;
	for(i=0;i<300;i++)
	{
		for(j=0;j<600;j++)
	{
		for(k=0;k<600;k++)
	{
		n++;
	}
	}
	}
}
void main()
{
	WSADATA wsaData;
    SOCKET sockClient;//客户端Socket
    SOCKADDR_IN addrServer;//服务端地址
    WSAStartup(MAKEWORD(2,2),&wsaData);
    //新建客户端socket
    sockClient=socket(AF_INET,SOCK_STREAM,0);
    //定义要连接的服务端地址
    addrServer.sin_addr.S_un.S_addr=inet_addr("192.168.43.240");//目标IP(127.0.0.1是回送地址)
    addrServer.sin_family=AF_INET;
    addrServer.sin_port=htons(10001);//连接端口号
    //连接到服务端
    connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
	int i,j,k,n,m;
	int tiaozhuan=0;//0关 1开
	int duiju=0;	//当前对局数
	int sl;
	SUMbh=0;
	srand(time(NULL));	
	while(duiju<=NumberOfMatches)
	{	
	
		
		//gets(recvBuf);
		if(bujie==0)
		{
		memset(recvBuf,0,100);		
		recv(sockClient,recvBuf,100,0);
		}
		printf("收到的消息为:%s\n",recvBuf);


	for(i=0;i<15;i++)		//判断回合以及对手行为
	{
		if(recvBuf[i]=='|')
		{
			switch(recvBuf[3])
			{
				case 'f':
					Stage=1;
					break;
				case 'p':
					Stage=2;
					break;
				case 'n':
					Stage=3;
					break;
				case 'e':
					Stage=4;
					break;
				case 'o':
					Stage=5;
					break;
			}
			break;
		}
	}
	int	number=0;
	for(i=0;i<30;i++)	//分析所发牌
	{	
		if(recvBuf[i]=='<')
		{
			number++;
			if(recvBuf[i+4]=='>')
			observe(recvBuf[i+1],recvBuf[i+3],Stage,number);
			else
			observes(recvBuf[i+1],recvBuf[i+3],recvBuf[i+4],Stage,number);
		}
	}
		if((recvBuf[0]=='n')&&(recvBuf[1]=='a'))//返还名字
		{			
			strcpy(sendBuf,"watermalen");								
		}
		else if((recvBuf[0]=='p')&&(recvBuf[1]=='r'))//preflop阶段
		{
			SUMbh=0;
			SUMbj=20000;
			printf("剩余筹码量%d\n",SUMbj);
			duiju++;
			pref();			
		}
		else if((recvBuf[0]=='f')&&(recvBuf[1]=='l'))//flop阶段
		{
			SUMbj=SUMbj-SUMbh;
			printf("剩余筹码量%d\n",SUMbj);
			SUMbh=0;
			flop();						
		}
		else if((recvBuf[0]=='t')&&(recvBuf[1]=='u'))//turn阶段
		{
			SUMbj=SUMbj-SUMbh;
			printf("剩余筹码量%d\n",SUMbj);
			SUMbh=0;
			turn();					
		}
		else if((recvBuf[0]=='r')&&(recvBuf[1]=='i'))//river阶段
		{
			SUMbj=SUMbj-SUMbh;
			printf("剩余筹码量%d\n",SUMbj);
			SUMbh=0;
			rive();						
		}
		else if((recvBuf[0]=='c')&&(recvBuf[1]=='h'))//check
		{
			chec();						
		}
		else if((recvBuf[0]=='c')&&(recvBuf[1]=='a'))//call
		{								
			call();					
		}
		else if((recvBuf[0]=='o')&&(recvBuf[1]=='p'))//oppo
		{	
	
			if(strlen(recvBuf)<30)
			sendBuf[0]=0;
			else
			{
				oppo();
			}
								
		}
		else if((recvBuf[0]=='e')&&(recvBuf[1]=='a'))//earn
		{
		
			sendBuf[0]=0;
		}
		else if((recvBuf[0]=='a')&&(recvBuf[1]=='l'))//allin
		{
			alli();
		}
		else if((recvBuf[0]=='r')&&(recvBuf[1]=='a'))//raise
		{
			jsjz();
			rais();	
		}
		delay();
		if(sendBuf[0]!=0)
		{
			send(sockClient,sendBuf,100,0);
			puts(sendBuf);
		
			memset(sendBuf,0,100);
		}

		if(bujie>0)
		{
			bujie--;
		}
				
	}
	//关闭socket
   closesocket(sockClient);
   WSACleanup();

}
void observe(char Colour,char Size,int Round,int Number)
{
	switch(Round)
	{
	case 1:
		if(Number==1)
		{
			HandCards[0][0]=int(Colour)-48;
			HandCards[0][1]=int(Size)-48;
		}
		if(Number==2)
		{
			HandCards[1][0]=int(Colour)-48;
			HandCards[1][1]=int(Size)-48;
		}
		break;
	case 2:
		if(Number==1)
		{
			FieldCards[0][0]=int(Colour)-48;
			FieldCards[0][1]=int(Size)-48;
		}
		if(Number==2)
		{
			FieldCards[1][0]=int(Colour)-48;
			FieldCards[1][1]=int(Size)-48;
		}
		if(Number==3)
		{
			FieldCards[2][0]=int(Colour)-48;
			FieldCards[2][1]=int(Size)-48;
		}
		break;
	case 3:
		FieldCards[3][0]=int(Colour)-48;
		FieldCards[3][1]=int(Size)-48;
		break;
	case 4:
		FieldCards[4][0]=int(Colour)-48;
		FieldCards[4][1]=int(Size)-48;
		break;
	case 5:
		break;
	}

} 
void observes(char Colour,char Size1,char Size2,int Round,int Number)
{
	switch(Round)
	{
	case 1:
		if(Number==1)
		{
			HandCards[0][0]=int(Colour)-48;
			HandCards[0][1]=10*(int(Size1)-48)+int(Size2)-48;
		}
		if(Number==2)
		{
			HandCards[1][0]=int(Colour)-48;
			HandCards[1][1]=10*(int(Size1)-48)+int(Size2)-48;
		}
		break;
	case 2:
		if(Number==1)
		{
			FieldCards[0][0]=int(Colour)-48;
			FieldCards[0][1]=10*(int(Size1)-48)+int(Size2)-48;
		}
		if(Number==2)
		{
			FieldCards[1][0]=int(Colour)-48;
			FieldCards[1][1]=10*(int(Size1)-48)+int(Size2)-48;
		}
		if(Number==3)
		{
			FieldCards[2][0]=int(Colour)-48;
			FieldCards[2][1]=10*(int(Size1)-48)+int(Size2)-48;
		}
		break;
	case 3:
		FieldCards[3][0]=int(Colour)-48;
		FieldCards[3][1]=10*(int(Size1)-48)+int(Size2)-48;
		break;
	case 4:
		FieldCards[4][0]=int(Colour)-48;
		FieldCards[4][1]=10*(int(Size1)-48)+int(Size2)-48;
		break;
	case 5:
		break;
	}

}
void jsjz()
{
	if(strlen(recvBuf)==11)
	{

		Increase=(int(recvBuf[6])-48)*10000+(int(recvBuf[7])-48)*1000+(int(recvBuf[8])-48)*100+(int(recvBuf[9])-48)*10+(int(recvBuf[10])-48);
	}
	else if(strlen(recvBuf)==10)
	{
		Increase=(int(recvBuf[6])-48)*1000+(int(recvBuf[7])-48)*100+(int(recvBuf[8])-48)*10+(int(recvBuf[9])-48);
	}
	else if(strlen(recvBuf)==9)
	{
		Increase=(int(recvBuf[6])-48)*100+(int(recvBuf[7])-48)*10+(int(recvBuf[8])-48);
	}
	else if(strlen(recvBuf)==8)
	{
		Increase=(int(recvBuf[6])-48)*10+(int(recvBuf[7])-48);
	}
	else if(strlen(recvBuf)==7)
	{
		Increase=(int(recvBuf[6])-48);
	}
}
int  pdsy()
{
	int a1,b1,a2,b2,c;
	int i,j;
		int ap[7][2],
			bp[7][2];

	for(i=0;i<7;i++)
	{		
		for(j=0;j<2;j++)
		{
			ap[i][j]=AllCards[i][j];
		}

	}
	for(i=2;i<9;i++)
	{		
		for(j=0;j<2;j++)
		{
			bp[i-2][j]=AllCards[i][j];
		}

	}

	for(i=1;i<7;i++)
	{
		for(j=i;j<7;j++)
		{
			if(ap[i-1][1]<=ap[j][1])
			{
				c=ap[i-1][1];
				ap[i-1][1]=ap[j][1];
				ap[j][1]=c;
				c=ap[i-1][0];
				ap[i-1][0]=ap[j][0];
				ap[j][0]=c;
			}
			if(bp[i-1][1]<=bp[j][1])
			{
				c=bp[i-1][1];
				bp[i-1][1]=bp[j][1];
				bp[j][1]=c;
				c=bp[i-1][0];
				bp[i-1][0]=bp[j][0];
				bp[j][0]=c;
			}
		}
	}
	for(i=0;i<7;i++)
	{		
		for(j=0;j<2;j++)
		{
			KeepCards[i][j]=ap[i][j];
		}

	}
	zuida();
	a1=PAIXING;
	a2=PAIDIAN;
	for(i=0;i<7;i++)
	{		
		for(j=0;j<2;j++)
		{
			KeepCards[i][j]=bp[i][j];
		}

	}
	zuida();
	b1=PAIXING;
	b2=PAIDIAN;

if(a1>b1)
	return 1;
else if(a1==b1)
{
	if(a2>b2)
		return 1;
	else if(a2==b2)
		return 0;
		else
		return -1;
}
else 
	return -1;
}
void zuida()
{
	int i,j;
	int c,d,e;
	PAIXING=0;
	PAIDIAN=KeepCards[0][1];
	for(i=0;i<6;i++)
	{
		for(j=i+1;j<7;j++)
		{
			if(KeepCards[i][1]==KeepCards[j][1])
			{
				PAIXING=1;
				PAIDIAN=KeepCards[i][1];
				break;
			}

		}
		if(	PAIXING==1)
			break;
	}
	c=0;
	for(i=0;i<6;i++)
	{
		if(c==0)
		{
		for(j=i+1;j<7;j++)
		{
			if(KeepCards[i][1]==KeepCards[j][1])
			{
				
				i=i+1;
				c=1;
				break;
			}
		}
		}
		else
		{
			
		for(j=i+1;j<7;j++)
		{
			if(KeepCards[i][1]==KeepCards[j][1])
			{
				
				PAIXING=2;
				break;
			}
		}
		}
		if(	PAIXING==2)
			break;
	}
	for(i=0;i<5;i++)
	{
		for(j=i+1;j<6;j++)
		{
			if((KeepCards[i][1]==KeepCards[j][1])&&(KeepCards[i][1]==KeepCards[j+1][1]))
			{
				PAIXING=3;
				PAIDIAN=KeepCards[i][1];
				break;
			}
	
		}
		if(	PAIXING==3)
			break;
	}
	for(i=0;i<3;i++)
	{
		d=0;
		c=KeepCards[i][1];
		for(j=i+1;j<7;j++)
		{
			if(c==(KeepCards[j][1]+1))
			{
				c--;
				d++;
			}
			
		}
		if(d>3)
		{
			PAIXING=4;
			PAIDIAN=KeepCards[i][1];
			break;
		}
		
	
	}
	for(i=0;i<3;i++)
	{
		
		c=0;		
		for(j=i+1;j<7;j++)
		{
			if(KeepCards[i][0]==KeepCards[j][0])
				c++;
		}
		if(c>3)
		{
				PAIXING=5;
				PAIDIAN=KeepCards[i][1];
				break;
		}
		
	}
	c=0;
	for(i=0;i<5;i++)
	{
	
		if(c==0)
		{	
			if((KeepCards[i][1]==KeepCards[i+1][1])&&(KeepCards[i][1]==KeepCards[i+2][1]))
			{
				
				c=1;
				PAIDIAN=KeepCards[i][1];
			
				i=0;
			}		
		}
		else
		{			
		for(j=0;j<6;j++)
		{
			if((KeepCards[j][1]==KeepCards[j+1][1])&&(KeepCards[i][1]!=PAIDIAN))
			{
				PAIXING=6;
				break;
			}
		}
		
		}
		if(	PAIXING==6)
			break;
	}
	
for(i=0;i<4;i++)
	{			
			if((KeepCards[i][1]==KeepCards[i+1][1])&&(KeepCards[i][1]==KeepCards[i+2][1])&&(KeepCards[i][1]==KeepCards[i+3][1]))
			{				
				PAIDIAN=KeepCards[i][1];			
				PAIXING=7;
			}		
		if(	PAIXING==7)
			break;
	}
for(i=0;i<3;i++)
	{
		d=0;
		c=KeepCards[i][1];
		e=KeepCards[i][0];
		for(j=i+1;j<7;j++)
		{
			if((c==(KeepCards[j][1]+1))&&(e==KeepCards[j][0]))
			{
				c--;
				d++;
			}
			
		}
		if(d>3)
		{
			PAIXING=8;
			PAIDIAN=KeepCards[i][1];
			break;
		}
		
	
	}



}
void sjfp(int stage)
{
	int i,j,k;
	int a,b,c;
	AllCards[0][0]=HandCards[0][0];
	AllCards[0][1]=HandCards[0][1];
	AllCards[1][0]=HandCards[1][0];
	AllCards[1][1]=HandCards[1][1];
//	srand(time(NULL));
	if(stage==1)
	{
		for(i=2;i<9;i++)
		{
			c=0;
			a=rand()%4;
			b=rand()%13;
			for(j=0;j<i;j++)
			{
				if((a==AllCards[j][0])&&(b==AllCards[j][1]))
				{
					c=1;
					i--;
					break;
				}
			}
			if(c==0)
			{
				AllCards[i][0]=a;
				AllCards[i][1]=b;
			}

		}
	
	}
	else if(stage==2)
	{

		for(i=0;i<3;i++)
		{
			for(j=0;j<2;j++)
			{
				AllCards[i+2][j]=FieldCards[i][j];			
			}
		}
	for(i=5;i<9;i++)
		{
			c=0;
			a=rand()%4;
			b=rand()%13;
			for(j=0;j<i;j++)
			{
				if((a==AllCards[j][0])&&(b==AllCards[j][1]))
				{
					c=1;
					i--;
					break;
				}
			}
			if(c==0)
			{
				AllCards[i][0]=a;
				AllCards[i][1]=b;
			}

		}
	
	
	}
	else if(stage==3)
	{
		for(i=0;i<4;i++)
		{
			for(j=0;j<2;j++)
			{
				AllCards[i+2][j]=FieldCards[i][j];			
			}
		}
	for(i=6;i<9;i++)
		{
			c=0;
			a=rand()%4;
			b=rand()%13;
			for(j=0;j<i;j++)
			{
				if((a==AllCards[j][0])&&(b==AllCards[j][1]))
				{
					c=1;
					i--;
					break;
				}
			}
			if(c==0)
			{
				AllCards[i][0]=a;
				AllCards[i][1]=b;
			}

		}
	
	
	}
	
	else if(stage==4)
	{

		for(i=0;i<5;i++)
		{
			for(j=0;j<2;j++)
			{
				AllCards[i+2][j]=FieldCards[i][j];			
			}
		}
	for(i=7;i<9;i++)
		{
			c=0;
			a=rand()%4;
			b=rand()%13;
			for(j=0;j<i;j++)
			{
				if((a==AllCards[j][0])&&(b==AllCards[j][1]))
				{
					c=1;
					i--;
					break;
				}
			}
			if(c==0)
			{
				AllCards[i][0]=a;
				AllCards[i][1]=b;
			}

		}
	
	
	}
}
int shenglishu(int nnn)
{
	int shengli=0;
	int i,nou;
	srand(time(NULL));
	for(i=0;i<10000;i++)
	{
		sjfp(nnn);
		nou=pdsy();
		shengli=shengli+nou;
	}
	return shengli;
} 
void oppo()
{
			int i;
			int n=0;
			int m=0;
				memset(opBuf,0,100);
			for(i=0;i<100;i++)
			{
				if(n<2)
				{
				if(recvBuf[i]=='>')
					n++;
				}
				else
				{
					opBuf[m]=recvBuf[i];
					m++;
				}
			
			}

			for(i=0;i<100;i++)
			{
			recvBuf[i]=opBuf[i];
			}
			bujie=2;
			sendBuf[0]=0;

}
void pref()
{
	int i;
	int n=0;
	int m=0;
				if(recvBuf[8]=='B')
				{
					sendBuf[0]=0;
					BlindNote=1;
					SUMbh=100;
					if(strlen(recvBuf)>30)
					{
					memset(opBuf,0,100);
					bujie=2;
			for(i=0;i<100;i++)
			{
				if(n<2)
				{
				if(recvBuf[i]=='>')
					n++;
				}
				else
				{
					opBuf[m]=recvBuf[i];
					m++;
				}
			
			}

			for(i=0;i<100;i++)
			{
			recvBuf[i]=opBuf[i];
			
			}
					}



				}
				else 
				{	
					BlindNote=0;	
					int sl;
					srand(time(NULL));
					sl=shenglishu(1);									
					if(sl>4000)
					{
					strcpy(sendBuf,"raise 2000");
					SUMbh=3000;
					}
					else if(sl>2000)
					{
					strcpy(sendBuf,"raise 1000");
					SUMbh=1000;
					}
					else
					{
					strcpy(sendBuf,"call");
					SUMbh=100;
					}
				}

}
void flop()
{
		if(BlindNote==1)
				{
					srand(time(NULL));
					int sl;
					sl=shenglishu(2);
					if(sl>8000)
					{
						if(SUMbj<10000)
						{
							strcpy(sendBuf,"allin");	
						}
						else
						{
							strcpy(sendBuf,"raise 8000");
							SUMbh=8000;
						}
					}
					else if(sl>6000)
					{
						if(SUMbj>10000)
						{
							zhenghe(SUMbj-10000);
							SUMbh=10000;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}
					else if(sl>4000)
					{
					
						if(SUMbj>12000)
						{
							zhenghe(SUMbj-12000);
							SUMbh=8000;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}

					else if(sl>2000)
					{
					
						if(SUMbj>15000)
						{
							zhenghe(SUMbj-15000);
							SUMbh=SUMbj-15000;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}
					else 
					{
						strcpy(sendBuf,"check");					
					}									
				}
				else 
				{
	
					sendBuf[0]=0;
				}	
	
}
void turn()
{
				if(BlindNote==1)
				{							
				srand(time(NULL));
					int sl;
					sl=shenglishu(3);
					if(sl>6000)
					{				
						strcpy(sendBuf,"allin");						
					}
					else if(sl>4000)
					{
					
						if(SUMbj>5000)
						{
							zhenghe(SUMbj-5000);
							SUMbh=SUMbj-5000;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}
					else if(sl>2000)
					{
					
						if(SUMbj>10000)
						{
							zhenghe(SUMbj-10000);
							SUMbh=SUMbj-10000;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}
					else 
					{
						strcpy(sendBuf,"check");					
					}	
				}
				else 
				{	
						sendBuf[0]=0;
				}		

}
void rive()
{
				if(BlindNote==1)
				{											
					int sl;
					srand(time(NULL));
					sl=shenglishu(4);									
					if(sl>7000)
					{
					strcpy(sendBuf,"allin");
					}
					else if(sl>2000)
					{
					
						if(SUMbj>10000)
						{
							zhenghe(SUMbj-10000);
							SUMbh=SUMbj-10000;;
						}
						else
						{
							strcpy(sendBuf,"check");
						}
					}
					else
					strcpy(sendBuf,"check");
				}
				else 
				{	
					sendBuf[0]=0;
				}	

}
void chec()
{
					int sl;
					srand(time(NULL));
					sl=shenglishu(Stage);									
					if(sl>7000)
					{
					strcpy(sendBuf,"allin");
					}
					else if(sl>4000)
					{
					if(SUMbj>10000)
					{
					zhenghe(SUMbj-10000);
						SUMbh=SUMbj-10000;
					}
					else
					strcpy(sendBuf,"call");
					}
					else if(sl>2000)
					{
					if(SUMbj>18000)
					{
					zhenghe(SUMbj-18000);
						SUMbh=SUMbj-18000;
					}
					else
					strcpy(sendBuf,"call");
					}
					else
					strcpy(sendBuf,"call");


}
void call()
{
					int sl;
					srand(time(NULL));
					sl=shenglishu(1);									
					if(sl>4000)
					{
					strcpy(sendBuf,"raise 2000");
					SUMbh=2000;
					}
					else if(sl>2000)
					{
					strcpy(sendBuf,"raise 1000");
					SUMbh=1000;
					}
					else
					{
					strcpy(sendBuf,"check");
					}

}
void rais()
{
	
					int sl;
					srand(time(NULL));
					sl=shenglishu(Stage);									
					if(sl>8000)
					{
					strcpy(sendBuf,"allin");
					}
					else if(sl>6000)
					{
						if(Increase>9999)
						{
							strcpy(sendBuf,"call");
							SUMbh=Increase;
						}
						else if(Increase>4999)
						{
							zhenghe(2*Increase);
							SUMbh=2*Increase;												
						}
						else 
						{
							zhenghe(10000);
							SUMbh=10000;
						}
					
					}
					else if(sl>4000)
					{
						if(Increase>8000)
						{
							strcpy(sendBuf,"fold");
						}
						else if(Increase>2000)
						{
							strcpy(sendBuf,"call");	
							SUMbh=Increase;
						}
						else 
						{
							strcpy(sendBuf,"raise 2000");	
							SUMbh=2000;
						}
					
					}
					else if(sl>2000)
					{
						if(Increase>5000)
						{
							strcpy(sendBuf,"fold");
						}
						else if(Increase>1000)
						{
							strcpy(sendBuf,"call");	
							SUMbh=Increase;
						}
						else 
						{
							zhenghe(2*Increase);	
							SUMbh=2*Increase;
						}
					
					}
					else if(sl>0)
					{
						if(Increase>3000)
						{
							strcpy(sendBuf,"fold");
						}
						else 
						{
							strcpy(sendBuf,"call");	
							SUMbh=Increase;
						}
					}
					else
					{
						if(Increase>1000)
						{
							strcpy(sendBuf,"fold");
						}
						else 
						{
							strcpy(sendBuf,"call");	
							SUMbh=Increase;
						}
					}
						
}
void alli()
{
	
					int sl;
					srand(time(NULL));
					sl=shenglishu(Stage);	
					if(sl>3000)
					{
					strcpy(sendBuf,"call");
					}
					else
					{
					strcpy(sendBuf,"fold");	
					}
}
void zhenghe(int nnn)
{
	if(nnn>=10000)
	{
		strcpy(sendBuf,"raise ");
		sendBuf[6]=nnn/10000+48;
		sendBuf[7]=(nnn%10000)/1000+48;
		sendBuf[8]=(nnn%1000)/100+48;
		sendBuf[9]=(nnn%100)/10+48;
		sendBuf[10]=nnn%10+48;
		
	}
	else if(nnn>1000)
	{
		strcpy(sendBuf,"raise ");
		sendBuf[6]=nnn/1000+48;
		sendBuf[7]=(nnn%1000)/100+48;
		sendBuf[8]=(nnn%100)/10+48;
		sendBuf[9]=nnn%10+48;
	}
	else if(nnn>100)
	{
		strcpy(sendBuf,"raise ");
		sendBuf[6]=nnn/100+48;
		sendBuf[7]=(nnn%100)/10+48;
		sendBuf[8]=nnn%10+48;
	
	}
	else if(nnn>10)
	{
		strcpy(sendBuf,"raise ");
		sendBuf[6]=nnn/10+48;
		sendBuf[7]=nnn%10+48;

	}
	else if(nnn==0)
	{
		strcpy(sendBuf,"call");	
	}
	

}
