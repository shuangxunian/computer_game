//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//二打一组负责人陈佳雨，戴鹏所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛斗地主博弈项目2014版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//陈佳雨	QQ：1274162928
//戴鹏		QQ：1219095374 
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "CardsCategory.h"
using namespace std;
//A01-START类型定义
//最后修订者:梅险,最后修订时间:15-02-08
struct Ddz
{
	int  iStatus;			//引擎状态-1错误,0结束,1开始
	char sCommandIn[80];	//通信输入内容
	char sCommandOut[80];	//通信输出内容
	int iOnHand[21];		//手中牌（所有值初始化为-1）
	int minOutCount;		//出完手中牌的最少手数-------------------------
	int iOnTable[162][21];	//以出牌数组（所有值初始化为-2）每行是一手牌，以-1结尾，Pass记为-1
	int iToTable[21];		//要出的牌
	char sVer[80];			//协议版本号
	char sName[80];			//参赛选手称呼
	char cDir;				//玩家方位编号
	char cLandlord;			//地主玩家方位编号
	char cWinner;			//胜利者方位编号
	int iBid[3];			//叫牌过程
	int iBmax;				//当前叫牌数，值域{0,1,2,3}
	int iOTmax;				//当前出牌手数
	int iRoundNow;			//当前局次
	int iRoundTotal;		//和总局数
	int iTurnNow;			//当前轮次
	int iTurnTotal;			//总轮数
	int iLevelUp;			//晋级选手数
	int iScoreMax;			//转换得分界限
	int iVoid;				//闲暇并行计算参数
	int iLef[3];			//本局底牌
	int iLastCount[3];		//当前各玩家手中牌数
	int iLastPointArray[14];//统计已知牌，按点数计数--------------------------
	int iLastPassCount;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	int iLastTypeCount;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）
	int iLastMainPoint;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，=1时保留原值，=2时值为-1）
	int iPlaArr[kPlaMax][21];		//己方多种出牌可行解集（各出牌解由牌编号升序组成-1间隔,-2收尾）
	int iPlaCount;			//己方多种出牌可行解数量（值域[0，kPlaMax-1]）
	int iPlaOnHand[21];		//己方模拟出牌后手牌编码
};
//A01-END
int array[MAX_CARD_ONE_HAND];

//部分函数声明
int IsType0Pass(int iCs[]);						//B00
int IsType1Rocket(int iCs[]);					//B01
int IsType2Bomb(int iCs[]);						//B02
int	IsType3Single(int iCs[]);					//B03
int	IsType4Double(int iCs[]);					//B04
int	IsType5Three(int iCs[]);					//B05
int IsType6ThreeOne(int iCs[]);					//B0601
int IsType6ThreeDouble(int iCs[]);				//B0602
int IsType7LinkSingle(int iCs[]);				//B07
int IsType8LinkDouble(int iCs[]);				//B08
int	IsType9LinkThree(int iCs[]);				//B09
int	IsType10LinkThreeSingle(int iCs[]);			//B1001
int	IsType10LinkThreeDouble(int	iCs[]);			//B1002
int IsType11FourSingle(int iCs[]);				//B1101
int IsType11FourDouble(int iCs[]);				//B1102
double CalCardsValue(struct Ddz * pDdz);		//D01
int CountCards(int iCards[]);					//I01
int CalBid(struct Ddz * pDdz);					//I02
int	AnalyzeTypeCount(int iCards[]);				//I03
int	AnalyzeMainPoint(int iCards[]);				//I04
void HelpfirstPass(struct Ddz * pDdz);			//H00------------
void Help0Pass(struct Ddz * pDdz);				//H00
void Help1Rocket(struct Ddz * pDdz);			//H01
void Help2Bomb(struct Ddz * pDdz);				//H02
void Help3Single(struct Ddz * pDdz);			//H03
void Help4Double(struct Ddz * pDdz);			//H04
void Help5Three(struct Ddz * pDdz);				//H05
void Help6ThreeOne(struct Ddz * pDdz);			//H0601
void Help6ThreeDouble(struct Ddz * pDdz);		//H0602
void Help7LinkSingle(struct Ddz * pDdz);		//H07
void Help8LinkDouble(struct Ddz * pDdz);		//H08
void Help9LinkThree(struct Ddz * pDdz);			//H09
void Help10LinkThreeSingle(struct Ddz * pDdz);	//H1001
void Help10LinkThreeDouble(struct Ddz * pDdz);	//H1002
void Help11FourSingle(struct Ddz * pDdz);		//H1101
void Help11FourDouble(struct Ddz * pDdz);		//H1102
void SortById(int iCards[]);					//M01
void SortByMuch(int iCards[]);					//M02
void SortByMain(int iCards[]);					//M03
void InitRound(struct Ddz * pDdz);				//M04	
void AppendCardsToS(int iCards[],char sMsg[]);	//M05
void HelpPla(struct Ddz * pDdz);				//M06
void HelpTakeOff(struct Ddz *  pDdz,int num);	//M07
void CountPoint(struct Ddz * pDdz, int iCards[]);	//
void InitTurn(struct Ddz * pDdz);				//P01
void InputMsg(struct Ddz * pDdz);				//P02
void AnalyzeMsg(struct Ddz * pDdz);				//P03
void GetDou(struct Ddz * pDdz);					//P0301
void GetInf(struct Ddz * pDdz);					//P0302
void GetDea(struct Ddz * pDdz);					//P0303
void GetBid(struct Ddz * pDdz);					//P0304
void GetLef(struct Ddz * pDdz);					//P0305
void GetPla(struct Ddz * pDdz);					//P0306
void CalPla(struct Ddz * pDdz);					//P030601	
void UpdateMyPla(struct Ddz * pDdz);			//P030602
void UpdateHisPla(struct Ddz * pDdz);			//P030603
void GetGam(struct Ddz * pDdz);					//P0307
void OutputMsg(struct Ddz * pDdz);				//P04
void CalOthers(struct Ddz * pDdz);				//P05
int IsPartner(struct Ddz * pDdz);
int LastPlayerLeft(struct Ddz * pDdz);
int output_card(int *p);                          //读取手牌并存到文件


int check(char a[])
{
	FILE *fp;
//	int i,size;

//	size=sizeof(a)/sizeof(int);
	if((fp=fopen("d:\\check.txt","a"))==NULL)
	{
		printf("cannot open this file\n");
		exit(0);
	}
	fprintf(fp,"%s\n",a);
	fclose(fp);
	return 0;

}
void check_card(char a[],int *p)
{
	FILE *fp;

	if((fp=fopen("d:\\b.txt","a"))==NULL)
	{
		printf("cannot open this file\n");
		exit(0);
	}
	fprintf(fp,"%s：",a);
	 while(*p!=-1)
	 {
		 fprintf(fp,"%d,",*p/4+3);
		 p++;
	 }
	 fprintf(fp,"\n,");

		fclose(fp);

}
void check_card_1(char b[],int a)
{
	FILE *fp;

	if((fp=fopen("d:\\b.txt","a"))==NULL)
	{
		printf("cannot open this file\n");
		exit(0);
	}

		 fprintf(fp,"%s:%d\n",b,a);
	
	 fprintf(fp,"\n,");

		fclose(fp);

}

int IsPartner(struct Ddz * pDdz)
{
	if(pDdz->cDir==pDdz->cLandlord 
		|| (pDdz->cDir-pDdz->cLandlord+3)%3==2 && pDdz->iLastPassCount==1//地主在右手边
		|| (pDdz->cDir-pDdz->cLandlord+3)%3==1 && pDdz->iLastPassCount==0)//地主在左手边
		return 0;
	return 1;
}

int LastPlayerLeft(struct Ddz * pDdz)
{
	if(pDdz->iLastPassCount)
		return pDdz->iLastCount[(pDdz->cDir-'A'+1-3)%3];//右手边player
	else
		return pDdz->iLastCount[(pDdz->cDir-'A'-1+3)%3];//左手边player
}

//B00-START判断出牌类型是否为弃权
//最后修订者:梅险,最后修订时间:15-02-10
int IsType0Pass(int iCs[])
{
	int iCount;
	iCount = CountCards(iCs);
	if(iCount==0)
		return 1;
	return 0;
}
//B00-END

//B01-START判断出牌类型是否为火箭
//最后修订者:夏侯有杰&梅险,最后修订时间:15-02-10
int IsType1Rocket(int iCs[])
{
	if((iCs[2]==-1) && (iCs[0] + iCs[1]== 52 + 53))
		return 1;
	return 0;
}
//B01-END

//B02-START判断出牌类型是否为炸弹
//最后修订者:夏侯有杰&梅险,最后修订时间:15-03-10
//修订内容及目的:防止牌是空的
int IsType2Bomb(int iCs[])
{
	if(4 != CountCards(iCs))
		return 0;
	if((iCs[4]==-1) && ( iCs[0]/4!= -1 && iCs[0]/4==iCs[1]/4 && iCs[0]/4==iCs[2]/4  && iCs[0]/4==iCs[3]/4 ))
		return 1;
	return 0;
}
//B02-END

//B03-START判断出牌类型是否为单牌
//最后修订者:夏侯有杰,最后修订时间:15-03-10
//修订内容及目的:if判断条件错误
int	IsType3Single(int iCs[])
{
	if(iCs[0] != -1 && iCs[1] == -1)
		return 1;
	return 0;
}
//B03-END

//B04-START判断出牌类型是否为对牌
//最后修订者:夏侯有杰,最后修订时间:15-02-13
int	IsType4Double(int iCs[])
{
	if(IsType1Rocket(iCs))
		return 0;
	if(iCs[0]/4 == iCs[1]/4 && iCs[0] != -1 && iCs[2] == -1)
		return 1;
	return 0;
}
//B04-END

//B05-判断出牌类型是否为三条
//最后修订者:夏侯有杰,最后修订时间:15-02-13
int	IsType5Three(int iCs[])
{
	if(iCs[0] != -1 && iCs[0]/4 == iCs[1]/4 && iCs[0]/4 == iCs[2]/4 && iCs[3] == -1)
		return 1;
	return 0;
}
//B05-END

//B0601-START判断出牌类型是否为三带一单
//最后修订者:夏侯有杰,最后修订时间:15-02-12
int IsType6ThreeOne(int iCs[])
{
	if(IsType2Bomb(iCs) || 4 != CountCards(iCs))
		return 0;
	SortByMuch(iCs);
	if(iCs[0]/4 == iCs[1]/4 && iCs[0]/4 == iCs[2]/4)
		return 1;
	return 0;
}
//B0601-END

//B0602-START判断出牌类型是否为三带一对
//最后修订者:夏侯有杰,最后修订时间:15-02-12
int IsType6ThreeDouble(int iCs[])
{
	if(5 != CountCards(iCs) )
		return 0;
	SortByMuch(iCs);
	if(iCs[0]/4==iCs[1]/4 && iCs[0]/4==iCs[2]/4)
		if(iCs[3]/4 == iCs[4]/4)
			return 1;
	return 0;
}
//B0602-END

//B07-START判断出牌类型是否为单顺
//最后修订者:谢文,最后修订时间:15-02-12
int IsType7LinkSingle(int iCs[])
{
	int iCount;
	int i;
	iCount=CountCards(iCs);
	if(iCount>=5)
	{
		SortById(iCs);
		for(i=1;iCs[i]>=0;i++)
			if(iCs[i-1]/4+1 != iCs[i]/4 || iCs[i]>=48)
				return 0;
		return 1;
	}
	return 0;
}
//B07-END

//B08-START判断出牌类型是否为连对
//最后修订者:谢文&夏侯有杰,最后修订时间:15-03-10
int IsType8LinkDouble(int iCs[])
{   
	int iCount = CountCards(iCs);
	int i; 
	if(iCount < 6 || iCount % 2 != 0)
		return 0;
	SortById(iCs);   //把iCs牌排序
	for(i = 1 ; i < iCount ; i++)
	{
		//判断i是单数还是双数
		if(i%2)
		{
			//如果i是单数，则第i张牌的编码编码会等于上一张牌编码+1  
			if(iCs[i]/4 != iCs[i-1]/4)
				return 0;
		}
		else
		{
			//如果i是双数，则第i张牌的编码编码会等于上一张牌编码
			if(iCs[i]/4 != iCs[i-1]/4 + 1)
				return 0;
		}
	}
	return 1;
}
//B08-END

//B09-START判断出牌类型是否为三顺
//最后修订者:夏侯有杰,最后修订时间:15-02-13
int	IsType9LinkThree(int iCs[])
{
	int iTempArr[11] = {0};   //初始化数组iTempArr用来记录 3 - A 每个点数的张数
	int iCount = CountCards(iCs);
	int i, iMinNum , iMaxNum;   //iMinNum 为 iCs 最小点数,iMaxNum 为 iCs 最大点数
	if(iCount < 6 || iCount % 3 != 0)
		return 0;
	SortById(iCs);   //把iCs牌排序
	iMinNum = iCs[0]/4;
	iMaxNum = iMinNum + iCount/3 -1;
	for(i = 0 ; i < iCount ; i++)
	{
		//判断iCs[i]是否在有效点数范围内
		if (iCs[i] > 47 || iCs[i]/4 < iMinNum || iCs[i]/4 > iMaxNum) 
		{
			return 0;
		}
		iTempArr[iCs[i]/4]++;
	}

	for (i = iMinNum ;i <= iMaxNum;i++)
	{
		//判断是否每个有效点数都为3张牌
		if(iTempArr[i] != 3)
			return 0;
	}
	return 1;

}
//B09-END

//B1001-START判断三顺带单，返回1是，0不是
//最后修订者:夏侯有杰,最后修订时间:15-03-10
//修订内容及目的:防止44455556的出现时排序后顺序乱
int	IsType10LinkThreeSingle(int iCs[])
{
	int iCount = CountCards(iCs);
	int iTempArr[18];
	int i,n ,m , j , iNum , iTemp ; //iNum记录有多少个 3+1
	if(iCount < 8 || iCount % 4 != 0)
		return 0;
	memset(iTempArr,-1,18*4);   //初始化iTempArr，值都为-1
	iNum = iCount/4;  
	SortByMuch(iCs);   //排序
	//判断是不是有炸弹
	n = 1,m = 0;
	while (n)
	{
		for(i = m ; i < m+4;i++)
			iTempArr[i] = iCs[i];

		//判断iTempArr是不是炸弹，不是则跳出
		if(0 == IsType2Bomb(iTempArr))
		{	
			n = 0;
		}
		else
		{
			//如果是的话,那么这个炸弹应该是一个顺子加一个单牌，单排就应该放在iCs的后面
			iTemp = iCs[m];
			for(j = m+1 ; j < iCount ;j++)
			{
				iCs[j-1] = iCs[j];
				iCs[j] = iTemp;
				iTemp = iCs[j];
			}
			m = m+3;
		}
		memset(iTempArr,-1,18*4);
	}

	//将三顺赋值给iTempArr
	for(i = 0 ; i < 3*iNum;i++)
	{
		iTempArr[i] = iCs[i];   
	}

	//判断iTempArr是不是三顺
	if(1 == IsType9LinkThree(iTempArr))
	{
		//将iTempArr传回iCs   防止55544465这样的牌型出现
		for(i = 0 ; i < 3*iNum;i++)
		{
			iCs[i] = iTempArr[i];
		}
		return 1;
	}
	return 0;

}
//B1001-END


//B1002-START   判断三顺带对，返回1是，0不是
//最后修订者:夏侯有杰,最后修订时间:15-02-13
int	IsType10LinkThreeDouble(int	iCs[])
{
	int iCount = CountCards(iCs);
	int iTempArr[18];
	int i,n , j ,k, iNum , iTemp ; //iNum记录有多少个 3+2 
	if(iCount < 10 || iCount % 5 != 0)
		return 0;
	memset(iTempArr,-1,18*4);   //初始化iTempArr，值都为-1
	iNum = iCount/5;
	SortByMuch(iCs);   //排序
	//判断是不是有炸弹
	n = 1;
	while (n)
	{
		for(i = 0 ; i < 4;i++)
		{
			iTempArr[i] = iCs[i];
		}

		//判断iTempArr是不是炸弹，不是则跳出
		if(0 == IsType2Bomb(iTempArr))
		{	
			n = 0;
		}
		else
		{
			//如果是的话,那么这个炸弹应该是属于2个对子，就应该放在iCs的后面
			for(k = 0 ; k < 4 ; k ++)
			{
				iTemp = iCs[0];
				for(j = 1 ; j < iCount ;j++)
				{
					iCs[j-1] = iCs[j];
					iCs[j] = iTemp;
					iTemp = iCs[j];
				}
			}
		}
		memset(iTempArr,-1,18*4);
	}

	//将三顺赋值给iTempArr
	for(i = 0 ; i < 3*iNum;i++)
	{
		iTempArr[i] = iCs[i];   
	}

	//判断iTempArr是不是三顺
	if(0 == IsType9LinkThree(iTempArr))
		return 0;

	//重置iTempArr
	memset(iTempArr,-1,18*4);

	j = 0;
	for(i = 3*iNum ;i < iCount;i++)
	{

		iTempArr[j] = iCs[i];
		if(j == 1)
		{
			//判断是不是对子
			if(0 == IsType4Double(iTempArr))
				return 0;
			memset(iTempArr,-1,18*4);
			j = 0;
		}
		else
		{
			j++;
		}
	}

	return 1;
}
//B1002-END

//B1101-START判断出牌类型是否为四带二单
//最后修订者:梅险,最后修订时间:15-02-10
int IsType11FourSingle(int iCs[])
{
	int iCount;
	iCount=CountCards(iCs);
	SortByMuch(iCs);  //同点数多牌的排在前面
	if(iCount==6)  //6=4+1+1
		if(iCs[0]/4==iCs[1]/4 && iCs[0]/4==iCs[2]/4  && iCs[0]/4==iCs[3]/4 )  //有四
			return 1;
	return 0;
}
//B1101-END

//B1102-START判断出牌类型是否为四带二对
//最后修订者:梅险,最后修订时间:15-02-10
int IsType11FourDouble(int iCs[])
{
	int iCount;
	iCount=CountCards(iCs);
	SortByMuch(iCs);  //同点数多牌的排在前面
	if(iCount==8)
		if(iCs[0]/4==iCs[1]/4 && iCs[0]/4==iCs[2]/4  && iCs[0]/4==iCs[3]/4 )  //有四
			if(iCs[4]/4==iCs[5]/4 && iCs[6]/4==iCs[7]/4)  //8=4+2+2
				return 1;
	return 0;
}
//B1102-END


//I01-START计算牌数
//最后修订者:梅险,最后修订时间:15-02-11
int CountCards(int iCards[])
{
	int iCount=0;
	while(iCards[iCount]>-1)
		iCount++;
	return iCount;
}
//I01-END

//I03-START分析牌型和张数
//最后修订者:梅险,最后修订时间:15-03-01
int	AnalyzeTypeCount(int iCards[])	
{
	int iCount=0;
	iCount=CountCards(iCards);
	if(IsType0Pass(iCards))
		return 0*100+iCount;
	if(IsType1Rocket(iCards))
		return 1*100+iCount;
	if(IsType2Bomb(iCards))
		return 2*100+iCount;
	if(IsType3Single(iCards))
		return 3*100+iCount;
	if(IsType4Double(iCards))
		return 4*100+iCount;
	if(IsType5Three(iCards))
		return 5*100+iCount;
	if(IsType6ThreeOne(iCards))
		return 6*100+iCount;
	if(IsType6ThreeDouble(iCards))
		return 6*100+iCount;
	if(IsType7LinkSingle(iCards))
		return 7*100+iCount;
	if(IsType8LinkDouble(iCards))
		return 8*100+iCount;
	if(IsType9LinkThree(iCards))
		return 9*100+iCount;
	if(IsType10LinkThreeSingle(iCards))
		return 10*100+iCount;
	if(IsType10LinkThreeDouble(iCards))
		return 10*100+iCount;
	if(IsType11FourSingle(iCards))
		return 11*100+iCount;
	if(IsType11FourDouble(iCards))
		return 11*100+iCount;

	return -1;
}
//I03-END

//I04-START计算主牌最小点数
//最后修订者:梅险,最后修订时间:15-03-01
int	AnalyzeMainPoint(int iCards[])	
{
	if(IsType0Pass(iCards))
		return -1;
	SortByMain(iCards);
	return iCards[0]/4;
}
//I04-END

//H00-START从iOnHand中查询作地主时首次出牌的候选，目前双顺到三顺带牌不能首出
//最后修订者:郭娜,最后修订时间:15-05-27 16:00 
void HelpfirstPass(struct Ddz * pDdz)
{
//		Help1Rocket(pDdz);
//		Help2Bomb(pDdz);
		Help3Single(pDdz);
		Help4Double(pDdz);
		Help5Three(pDdz);
		Help6ThreeOne(pDdz);
		Help6ThreeDouble(pDdz);
		Help7LinkSingle(pDdz);
//		Help8LinkDouble(pDdz);
//		Help9LinkThree(pDdz);
//		Help10LinkThreeSingle(pDdz);
//		Help10LinkThreeDouble(pDdz);
		Help11FourSingle(pDdz);
		Help11FourDouble(pDdz);
}
//H00-END

//H00-START从iOnHand中查询弃权牌型到iPlaArr,目前双顺到三顺带牌不能首出
//最后修订者:梅险,最后修订时间:15-02-17 12:00 
void Help0Pass(struct Ddz * pDdz)
{			
		Help8LinkDouble(pDdz);
		Help7LinkSingle(pDdz);
		Help3Single(pDdz);
		Help4Double(pDdz);
		Help9LinkThree(pDdz);
		Help10LinkThreeSingle(pDdz);
		Help10LinkThreeDouble(pDdz);
		Help5Three(pDdz);
		Help6ThreeOne(pDdz);
		Help6ThreeDouble(pDdz);
		Help11FourSingle(pDdz);
		Help11FourDouble(pDdz);
		Help2Bomb(pDdz);
		Help1Rocket(pDdz);		
}
//H00-END
void Help0Passpass(struct Ddz * pDdz)
{
	int i;
	for(i=0;pDdz->iOnHand[i]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax 
			&& ( pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
				|| pDdz->iOnHand[i] == 53))
		{
			pDdz->iPlaArr[pDdz->iPlaCount][0]=-1;
			pDdz->iPlaArr[pDdz->iPlaCount][1]=-1;
			pDdz->iPlaCount++;
		}
	}
}
//H01-START从iOnHand中查询组合火箭牌型到iPlaArr
//最后修订者:杨洋&梅险,最后修订时间:15-02-17 12:00 
void Help1Rocket(struct Ddz * pDdz)
{
	int iCount;
	if(pDdz->iPlaCount+1<kPlaMax)
	{
		iCount=CountCards(pDdz->iOnHand);
		SortById(pDdz->iOnHand);
		if(iCount>=2 && pDdz->iOnHand[iCount-2] + pDdz->iOnHand[iCount-1] == 52+53)
		{
			pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[iCount-2];
			pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[iCount-1];
			pDdz->iPlaArr[pDdz->iPlaCount][2]=-1;
			pDdz->iPlaCount++;
		}
	}
}
//H01-END

//H02-START推荐出牌应对炸弹
//最后修订者: 谢文，最后修订时间: 2015-03-10
void Help2Bomb(struct Ddz * pDdz)
{
	int i,iCount;
	SortById(pDdz->iOnHand);
	iCount=CountCards(pDdz->iOnHand);
	if (102==pDdz->iLastTypeCount || iCount<4)
		return;
	if(204==pDdz->iLastTypeCount)
	{
		for(i=3;pDdz->iOnHand[i]>=0;i++)
			if(pDdz->iPlaCount+1<kPlaMax
				&& pDdz->iOnHand[i]/4==pDdz->iOnHand[i-1]/4
				&& pDdz->iOnHand[i-1]/4==pDdz->iOnHand[i-2]/4
				&& pDdz->iOnHand[i-2]/4==pDdz->iOnHand[i-3]/4
				&& pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint)
			{ 
				pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i-3];
				pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i-2];
				pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i-1];
				pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[i];
				pDdz->iPlaArr[pDdz->iPlaCount][4]=-1;
				pDdz->iPlaCount++;
			}
	}
	else
		for(i=3;pDdz->iOnHand[i]>=0;i++)					
			if(pDdz->iPlaCount+1<kPlaMax
				&& pDdz->iOnHand[i]/4==pDdz->iOnHand[i-1]/4					
				&& pDdz->iOnHand[i-1]/4==pDdz->iOnHand[i-2]/4		
				&& pDdz->iOnHand[i-2]/4==pDdz->iOnHand[i-3]/4)
			{ 
				pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i-3];
				pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i-2];
				pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i-1];
				pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[i];
				pDdz->iPlaArr[pDdz->iPlaCount][4]=-1;
				pDdz->iPlaCount++;
			}  
}
//H02-END

//H03-START从iOnHand中查询单张牌型到iPlaArr
//最后修订者:梅险&夏侯有杰,最后修订时间:15-03-22
//修订内容及目的:大王能管小王
void Help3Single(struct Ddz * pDdz)
{
	int i;
	for(i=0;pDdz->iOnHand[i]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax 
			&& ( pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
				|| pDdz->iOnHand[i] == 53))
		{
			pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
			pDdz->iPlaArr[pDdz->iPlaCount][1]=-1;
			pDdz->iPlaCount++;
		}
	}
}
//H03-END

//H04-START推荐出牌应对对牌
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00 
void Help4Double(struct Ddz * pDdz)
{
	int i;
	SortById(pDdz->iOnHand);
	for(i=0;pDdz->iOnHand[i+1]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax
			&& pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4)
		{
			pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
			pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
			pDdz->iPlaArr[pDdz->iPlaCount][2]=-1;
			pDdz->iPlaCount++;
		}
	}
}
//H04-END

//H05-START推荐出牌应对三条
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00 
void Help5Three(struct Ddz * pDdz)
{
	int i;
	SortById(pDdz->iOnHand);

	for(i=0;pDdz->iOnHand[i+2]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax
			&& pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+2]/4)
		{
			pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
			pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
			pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i+2];
			pDdz->iPlaArr[pDdz->iPlaCount][3]=-1;
			pDdz->iPlaCount++;
		}
	}
}
//H05-END

//H0601-START推荐出牌应对三带一单
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00 
void Help6ThreeOne(struct Ddz * pDdz)
{
	int i,iTemp,j;
	SortById(pDdz->iOnHand);
	for(i=0;pDdz->iOnHand[i+2]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax
			&& pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+2]/4)
		{
			iTemp = i;
			for(j = 0 ;pDdz->iOnHand[j]>=0 ;j++)
			{
				//该单牌不能是三条中的数并且将炸弹排除
				if(j == iTemp || j == iTemp+1 || j == iTemp+2 || pDdz->iOnHand[j]/4 == pDdz->iOnHand[i]/4)
					continue;

				pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
				pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
				pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i+2];
				pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[j];
				pDdz->iPlaArr[pDdz->iPlaCount][4]=-1;
				pDdz->iPlaCount++;
			}

		}
	}
}
//H0601-END

//H0602-START推荐出牌应对三带一对
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00 
void Help6ThreeDouble(struct Ddz * pDdz)
{
	int i,iTemp,j;
	SortById(pDdz->iOnHand);

	for(i=0;pDdz->iOnHand[i+2]>=0;i++)
	{
		if(pDdz->iPlaCount+1<kPlaMax
			&& pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+2]/4)
		{
			iTemp = i;
			for(j = 0 ;pDdz->iOnHand[j+1]>=0 ;j++)
			{
				//将对子在三条中的数排除
				if(j >= iTemp-1 && j <= iTemp+2 )
					continue;
				if(pDdz->iOnHand[j]/4 == pDdz->iOnHand[j+1]/4)
				{
					pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
					pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
					pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i+2];
					pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[j];
					pDdz->iPlaArr[pDdz->iPlaCount][4]=pDdz->iOnHand[j+1];
					pDdz->iPlaArr[pDdz->iPlaCount][5]=-1;
					pDdz->iPlaCount++;
				}
			}
		}
	}
}
//H0602-END

//H07-START推荐出牌应对单顺
//最后修订者:谢文&夏侯有杰，最后修订时间:15-03-20: 11:00
void Help7LinkSingle(struct Ddz * pDdz)
{   int i,j,k,iLength,iTemp,iCount,iBiaoji,iFlag;
	int iCopyCards[21];
	iCount=CountCards(pDdz->iOnHand);
	iLength=pDdz->iLastTypeCount-700;
	SortById(pDdz->iOnHand);
	for(i=0;i<21;i++)
		iCopyCards[i]=pDdz->iOnHand[i];
	for(i=0;iCopyCards[i]>=0;i++)
		if(iCopyCards[i]/4>=12)
			iCopyCards[i]=-1;
	for(i=1;iCopyCards[i]>=0;i++)
		if(iCopyCards[i]/4==iCopyCards[i-1]/4)
			iCopyCards[i-1]=-1;
	for(i=0;i<iCount;i++)
		for(j=i+1;j<iCount;j++)
			if(iCopyCards[i]<iCopyCards[j])
			{ 
				iTemp=iCopyCards[i];
				iCopyCards[i]=iCopyCards[j];
				iCopyCards[j]=iTemp;
			}
	for(i=0;iCopyCards[i]>=0;i++)
		for(j=i+1;iCopyCards[j]>=0;j++)
			if(iCopyCards[i]>iCopyCards[j])
			{ 
				iTemp=iCopyCards[i];
				iCopyCards[i]=iCopyCards[j];
				iCopyCards[j]=iTemp;
			}
			//去除重复的牌并从小到大排序
	if(pDdz->iLastTypeCount==0)
	{
		for(iLength=5;iLength<=12;iLength++)
		{
				for(i=0;iCopyCards[i+iLength-1]>=0;i++)
				{
				iBiaoji=0;
				iFlag=0;
				if(iCopyCards[i]/4>pDdz->iLastMainPoint)
				{
					for(j=i;j<iLength-1+i;j++)
					{	
						if(iCopyCards[j]/4!=iCopyCards[j+1]/4-1)
						{	
							iBiaoji=1;
							break;
						}
					}
				}
				else
					continue;
				if(pDdz->iPlaCount+1<kPlaMax
					&& iBiaoji==0)
				{
					for(k=i;k<iLength+i;k++)
					{
						pDdz->iPlaArr[pDdz->iPlaCount][iFlag]=iCopyCards[k];
						iFlag++;
						if(k==iLength-1+i)			   
							pDdz->iPlaArr[pDdz->iPlaCount][iFlag++]=-1;
					}
					pDdz->iPlaCount++;
				}
				}
		}
	}
	else
	{
		for(i=0;iCopyCards[i+iLength-1]>=0;i++)
		{
			iBiaoji=0;
			iFlag=0;
			if(iCopyCards[i]/4>pDdz->iLastMainPoint)
			{
				for(j=i;j<iLength-1+i;j++)
				{	
					if(iCopyCards[j]/4!=iCopyCards[j+1]/4-1)
					{	
						iBiaoji=1;
						break;
					}
				}
			}
			else
				continue;
			if(pDdz->iPlaCount+1<kPlaMax
				&& iBiaoji==0)
			{
				for(k=i;k<iLength+i;k++)
				{
					pDdz->iPlaArr[pDdz->iPlaCount][iFlag]=iCopyCards[k];
					iFlag++;
					if(k==iLength-1+i)			   
						pDdz->iPlaArr[pDdz->iPlaCount][iFlag++]=-1;
				}
				pDdz->iPlaCount++;
			}
		}
	}
}
//H07-END

//H08-START推荐出牌应对双顺
//最后修订者:谢文，最后修订时间:2015-03-26
void Help8LinkDouble(struct Ddz * pDdz)
{
	int i,j,k,iLength;
	int iCopyCards[21];
	int n=1;
	int iFlag=0;
	int iCardsFlag=0;
	int iCard=0;
	SortById(pDdz->iOnHand);
	iLength=(pDdz->iLastTypeCount-800)/2;
	for(i=0;i<21;i++)
		iCopyCards[i]=pDdz->iOnHand[i];
	for(i=0;iCopyCards[i]>=0;i++)
		if(iCopyCards[i]/4>=12)
			iCopyCards[i]=-1;
	for(i=0;i<CountCards(iCopyCards);i++)
	{
		if(iCopyCards[i]/4!=iCopyCards[i+1]/4)
		{
			for(j=i;j<CountCards(iCopyCards);j++)
				iCopyCards[j]=iCopyCards[j+1];
			i--;
		}

	}//四连变三连 三连变对子 对子变单排 单排变没有
	for(i=0;i<CountCards(iCopyCards);i++)
	{
		if(iCopyCards[i]/4==iCopyCards[i+1]/4)
		{
			for(j=i;j<CountCards(iCopyCards);j++)
				iCopyCards[j]=iCopyCards[j+1];
			i--;
		}
	}
	//三连和对子变成单牌
	//以上将数组iCopyCards[21]处理成能组成对子的单排数组且最大为A

	for(i=0;iCopyCards[i]>=0;i++)
	{
		if(iCopyCards[i]/4==iCopyCards[i+1]/4-1)
			n++;
		else
		{
			n=1;
			i=iFlag;
			iFlag++;
		}
		if(n==iLength)
		{
			if(pDdz->iPlaCount+1<kPlaMax
				&& iCopyCards[i+2-iLength]/4>pDdz->iLastMainPoint)
			{
				for(j=i+2-iLength;j<=i+1;j++)
				{
					for (k=0;k<CountCards(pDdz->iOnHand);k++)
					{
						if(iCopyCards[j]/4==pDdz->iOnHand[k]/4&&iCardsFlag<2)
						{
							pDdz->iPlaArr[pDdz->iPlaCount][iCard]=pDdz->iOnHand[k];
							iCard++;
							iCardsFlag++;
						}
					}
					iCardsFlag=0;
				}
				pDdz->iPlaArr[pDdz->iPlaCount][iCard]=-1;
				pDdz->iPlaCount++;
				iCard=0;		
			}
			n=1;
			i=iFlag;
			iFlag++;
		}
	}
}
//H08-END

//H09-START推荐出牌应对三顺
//最后修订者:夏侯有杰，最后修订时间:2015-03-22
void Help9LinkThree(struct Ddz * pDdz)
{
	int i,iTemp,j,k,iLength = pDdz->iLastTypeCount-900;
	int iTempArr[12][5];
	if(CountCards(pDdz->iOnHand)<6)
		return ;

	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	memset(iTempArr,-1,12*5*4);
	SortById(pDdz->iOnHand);    //先将数组进行排序
	for(i = 0 ; pDdz->iOnHand[i] > -1 ; i++)
	{
		if(pDdz->iOnHand[i] < 48)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while(-1 != iTempArr[pDdz->iOnHand[i]/4][j])
				j++;

			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[pDdz->iOnHand[i]/4][j] = pDdz->iOnHand[i];

			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[pDdz->iOnHand[i]/4][0] = j;
		}
	}
	for(i = pDdz->iLastMainPoint+1 ; i <12 ; i++)
	{
		iTemp = iLength-3;
		if(iTempArr[i][0]>=3)
		{
			j = i;
			while (iTemp)
			{
				if(iTempArr[j][0]>=3)
				{
					iTemp-=3;
					j++;
				}
				else break;
			}
			if(pDdz->iPlaCount+1<kPlaMax
				&& iTemp == 0)
			{
				k = 0;
				while (k < iLength)
				{
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][1];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][2];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][3];
					k++;
					j--;
				}
				pDdz->iPlaCount++;
			}
		}
	}
}
//H09-END

//H1001-START推荐出牌应对三顺带单
//最后修订者:夏侯有杰，最后修订时间:15-03-31
void Help10LinkThreeSingle(struct Ddz * pDdz)
{
	int i,iTemp,j,k,m,iLength = (pDdz->iLastTypeCount-1000);
	int iTempArr[12][5];
	int nFlag = 0;
	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	memset(iTempArr,-1,12*5*4);
	SortById(pDdz->iOnHand);    //先将数组进行排序
	for(i = 0 ; pDdz->iOnHand[i] > -1 ; i++)
	{
		if(pDdz->iOnHand[i] < 48)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while(-1 != iTempArr[pDdz->iOnHand[i]/4][j])
				j++;

			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[pDdz->iOnHand[i]/4][j] = pDdz->iOnHand[i];

			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[pDdz->iOnHand[i]/4][0] = j;
		}
	}

	for(i = pDdz->iLastMainPoint+1 ; i <12 ; i++)
	{
		iTemp = iLength;
		if(iTempArr[i][0]>=3)
		{
			j = i;
			while (iTemp)
			{
				if(iTempArr[j][0]>=3)
				{
					iTemp-=4;
					j++;
				}
				else break;
			}
			if(iTemp == 0)
			{
				k = 0;
				j--;
				while (k < iLength/4*3)
				{
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][1];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][2];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][3];
					k++;
					j--;
				}
				
				for(j = 0 ;pDdz->iOnHand[j]>=0 ;j++)
				{
					nFlag = 1;
					for(m = 0;m < k;m++)
					{
						if(pDdz->iOnHand[j] == pDdz->iPlaArr[pDdz->iPlaCount][m])
						{
							nFlag = 0;
							break;
						}
					}
					if(pDdz->iPlaCount+1<kPlaMax
						&& nFlag)
					{
						pDdz->iPlaArr[pDdz->iPlaCount][k] = pDdz->iOnHand[j];
						k++;
						if(k == iLength)
						{
							pDdz->iPlaCount++;
							break;
						}
					}
				}
			}			
		}
	}
}
//H1001-END

//H1002-START推荐出牌应对三顺带双
//最后修订者:夏侯有杰，最后修订时间:15-03-31
void Help10LinkThreeDouble(struct Ddz * pDdz)
{
	int i,iTemp,j,k,m,iLength = (pDdz->iLastTypeCount-1000);
	int iTempArr[12][5];
	int nFlag = 0;

	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	memset(iTempArr,-1,12*5*4);
	SortById(pDdz->iOnHand);    //先将数组进行排序
	for(i = 0 ; pDdz->iOnHand[i] > -1 ; i++)
	{
		if(pDdz->iOnHand[i] < 48)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while(-1 != iTempArr[pDdz->iOnHand[i]/4][j])
				j++;

			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[pDdz->iOnHand[i]/4][j] = pDdz->iOnHand[i];

			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[pDdz->iOnHand[i]/4][0] = j;
		}
	}

	for(i = pDdz->iLastMainPoint+1 ; i <12 ; i++)
	{
		iTemp = iLength;
		if(iTempArr[i][0]>=3)
		{
			j = i;
			while (iTemp)
			{
				if(iTempArr[j][0]>=3)
				{
					iTemp-=5;
					j++;
				}
				else break;
			}
			if(iTemp == 0)
			{
				k = 0;
				j--;
				while (k < iLength/5*3)
				{
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][1];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][2];
					k++;
					pDdz->iPlaArr[pDdz->iPlaCount][k]=iTempArr[j][3];
					k++;
					j--;
				}

				for(j = 0 ;pDdz->iOnHand[j]>=0 ;j++)
				{
					nFlag = 1;
					for(m = 0;m < k;m++)
					{
						if(pDdz->iOnHand[j] == pDdz->iPlaArr[pDdz->iPlaCount][m])
						{
							nFlag = 0;
							break;
						}
					}
					if(pDdz->iPlaCount+1<kPlaMax
						&& nFlag)
					{
						if(pDdz->iOnHand[j]/4 != pDdz->iOnHand[j+1]/4)
							continue;
						pDdz->iPlaArr[pDdz->iPlaCount][k] = pDdz->iOnHand[j];
						k++;
						pDdz->iPlaArr[pDdz->iPlaCount][k] = pDdz->iOnHand[j+1];
						k++;
						if(k == iLength)
						{
							pDdz->iPlaCount++;
							break;
						}
					}
				}
			}
		}
	}
}
//H1002-END

//H1101-START推荐出牌应对四带二单
//最后修订者:夏侯有杰，最后修订时间:15-03-10 12:00 
void Help11FourSingle(struct Ddz * pDdz)
{
	int i,iTempI,j,k;
	if(CountCards(pDdz->iOnHand)<6)
		return ;
	SortById(pDdz->iOnHand);
	for(i=0;pDdz->iOnHand[i+3]>=0;i++)
	{
		if(pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+2]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+3]/4)
		{
			iTempI = i;
			for(j = 0 ;pDdz->iOnHand[j]>=0 ;j++)
			{
				//该单牌不能是4条中的数
				if(j == iTempI || j == iTempI+1 || j == iTempI+2 || j == iTempI+3)
					continue;
				for(k = j+1 ; pDdz->iOnHand[k]>=0 ;k++)
				{
					//该单牌不能是4条中的数
					if(k == iTempI || k == iTempI+1 || k == iTempI+2 || k == iTempI+3)
						continue;
					if(pDdz->iPlaCount+1<kPlaMax)
					{
						pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
						pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
						pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i+2];
						pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[i+3];
						pDdz->iPlaArr[pDdz->iPlaCount][4]=pDdz->iOnHand[j];
						pDdz->iPlaArr[pDdz->iPlaCount][5]=pDdz->iOnHand[k];
						pDdz->iPlaArr[pDdz->iPlaCount][6]=-1;
						pDdz->iPlaCount++;
					}
				}
			}
		}
	}
}
//H1101-END

//H1102-START推荐出牌应对四带二对
//最后修订者:夏侯有杰，最后修订时间:15-03-23 12:00 
void Help11FourDouble(struct Ddz * pDdz)
{
	int i,iTempI,j,k;
	if(CountCards(pDdz->iOnHand)<8)
		return ;
	SortById(pDdz->iOnHand);

	for(i=0;pDdz->iOnHand[i+3]>=0;i++)
	{
		if(pDdz->iOnHand[i]/4 > pDdz->iLastMainPoint 
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+1]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+2]/4
			&& pDdz->iOnHand[i]/4 == pDdz->iOnHand[i+3]/4)
		{
			iTempI = i;
			for(j = 0 ;pDdz->iOnHand[j]>=0 ;j++)
			{
				if(j == iTempI || j == iTempI+1 || j == iTempI+2 || j == iTempI+3 ||pDdz->iOnHand[j]/4 != pDdz->iOnHand[j+1]/4)
					continue;
				for(k = j+2 ; pDdz->iOnHand[k]>=0 ;k++)
				{
					//该单牌不能是4条中的数且把大小王排除了
					if(k == iTempI || k == iTempI+1 || k == iTempI+2 || k == iTempI+3||pDdz->iOnHand[j] >=52 || pDdz->iOnHand[k]/4 != pDdz->iOnHand[k+1]/4)
						continue;
					if(pDdz->iPlaCount+1<kPlaMax)
					{
						pDdz->iPlaArr[pDdz->iPlaCount][0]=pDdz->iOnHand[i];
						pDdz->iPlaArr[pDdz->iPlaCount][1]=pDdz->iOnHand[i+1];
						pDdz->iPlaArr[pDdz->iPlaCount][2]=pDdz->iOnHand[i+2];
						pDdz->iPlaArr[pDdz->iPlaCount][3]=pDdz->iOnHand[i+3];
						pDdz->iPlaArr[pDdz->iPlaCount][4]=pDdz->iOnHand[j];
						pDdz->iPlaArr[pDdz->iPlaCount][5]=pDdz->iOnHand[j+1];
						pDdz->iPlaArr[pDdz->iPlaCount][6]=pDdz->iOnHand[k];
						pDdz->iPlaArr[pDdz->iPlaCount][7]=pDdz->iOnHand[k+1];
						pDdz->iPlaArr[pDdz->iPlaCount][8]=-1;
						pDdz->iPlaCount++;
					}
				}
			}
		}
	}
}
//H1102-END

//MO1-START按牌编码升序排列
//最后修订者:夏侯有杰&梅险,最后修订时间:15-02-09
void SortById(int iCards[])
{
	int i, j;
	int iTemp;
	for (i = 0; iCards[i] >= 0; i++)
	{
		for (j = i + 1; iCards[j] >= 0; j++)
			if (iCards[i]>iCards[j])
			{
				iTemp = iCards[i];
				iCards[i] = iCards[j];
				iCards[j] = iTemp;
			}
	}
}
//MO1-END

//M02-START按主牌(同点数牌)编码升序排列
//最后修订者:夏侯有杰,最后修订时间:2015-03-01
void SortByMuch(int iCards[])
{
	int i,j,k,n;
	// iTempArr[i][0]用来装点数为 i 有多少张牌，
	// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]用来装点数为 i 的牌的编码，
	int iTempArr[13][5];
	memset(iTempArr,-1,13*5*4);
	SortById(iCards);    //先将iCards数组进行排序
	for(i = 0 ; iCards[i] > -1 ; i++)
	{
		if(iCards[i] < 52)
		{
			//出点数为 iCards[i]/4 的牌有多少张，用 j 来表示
			j = 1;
			while(-1 != iTempArr[iCards[i]/4][j])
				j++;
			//将编码放入iTempArr[iCards[i]/4][j]里面，因为该编码的点数为iCards[i]/4
			iTempArr[iCards[i]/4][j] = iCards[i];
			//将点数为 iCards[i]/4 的牌的张数 改为 j ；
			iTempArr[iCards[i]/4][0] = j;
		}
	}
	n = 0;   //n为iCards的下标，重新将iTempArr中的数放入iCards中
	for(i = 4 ; i > 0 ;i--) //先找出iTempArr一样的四张牌，若有则写入原数组iCards【】中，然后再寻找三张的，一次类推	
	{
		for(j = 0 ;j < 13 ; j++)
		{
			if(iTempArr[j][0] == i)    //判断该点数的牌是不是有 i 张
			{
				for(k = 1;k <= i ; k++)    // 有的话，就把牌都放进iCards[ n ]中，然后 n++
				{
					iCards[n] = iTempArr[j][k];
					n++;
				}
			}
		}
	}
}
//MO2-END

//M03-START按管牌主要因素升序排列
//最后修订者:夏侯有杰,最后修订时间:15-03-08 18:00 
void SortByMain(int iCards[])
{
	if(IsType0Pass(iCards))
		return ;
	if(IsType1Rocket(iCards))
		return ;
	if(IsType2Bomb(iCards))
		return ;
	if(IsType3Single(iCards))
		return ;
	if(IsType4Double(iCards))
		return ;
	if(IsType5Three(iCards))
		return ;
	if(IsType6ThreeOne(iCards))
		return ;
	if(IsType6ThreeDouble(iCards)) 
		return ;
	if(IsType7LinkSingle(iCards))
		return ;
	if(IsType8LinkDouble(iCards))
		return ;
	if(IsType9LinkThree(iCards))
		return ;
	if(IsType10LinkThreeSingle(iCards))
		return ;
	if(IsType10LinkThreeDouble(iCards))
		return ;
	if(IsType11FourSingle(iCards))
		return ;
	if(IsType11FourDouble(iCards))
		return ;
	return;
}
//MO3-END

void CountPoint(struct Ddz * pDdz, int iCards[])
{
	int i;
	for(i=0; iCards[i]>=0; i++)
	{
		pDdz->iLastPointArray[iCards[i]/4]++;
	}
}
void PrintPointArray(struct Ddz * pDdz)
{
	int i;
	FILE *fp=fopen("lalala.txt","a+");

	for(i=0; i<=7; i++)
	{
		fprintf(fp,"%5d",i+3);
	}
	fprintf(fp,"%5c%5c%5c%5c%5c%5c\n",'J','Q','K','A','2','W');
	for(i=0; i<14; i++)
	{
		fprintf(fp,"%5d",pDdz->iLastPointArray[i]);
	}
	fprintf(fp,"\n\n");

	fclose(fp);
}


//M04-START重置本局数据初始值
//最后修订者:梅险&夏侯有杰,最后修订时间:15-04-05
//修改目的:iLastPasCount,iLastTypeCount,iLastMainPoint初始化
void InitRound(struct Ddz * pDdz)	
{
	int i, j;
	pDdz->iStatus = 1;					//初始化本局引擎状态
	strcpy(pDdz->sCommandIn,"");		//初始化本局通信输入内容
	strcpy(pDdz->sCommandOut,"");		//初始化本局通信输出内容
	for (i = 0; i < 21; i++)			//初始化本局手牌
		pDdz->iOnHand[i] = -1;
	pDdz->minOutCount=20;
	for (i = 0; i<162; i++)				//初始化本局桌面牌
		for (j = 0; j<21; j++)
			pDdz->iOnTable[i][j] = -2;
	for (i = 0; i < 21; i++)			//初始化本局出牌
		pDdz->iToTable[i] = -1;
	strcpy(pDdz->sVer,"");				//初始化本局协议版本号
	strcpy(pDdz->sVer,kPlayerName);		//初始化本局参赛选手称呼
	pDdz->cDir='B';						//初始化本局玩家方位编号
	pDdz->cLandlord='B';				//初始化本局地主方位编号
	pDdz->cWinner='B';					//初始化本局胜者方位编号
	for (i = 0; i < 3; i++)				//初始化本局叫牌
		pDdz->iBid[i] = -1;
	for (i = 0; i < 3; i++)				//初始化各玩家手中牌数
		pDdz->iLastCount[i] = 17;
	for (i = 0; i < 14; i++)			//初始化已知牌点数统计数量 
		pDdz->iLastPointArray[i] = 0;
	pDdz->iLastPassCount=2;
	pDdz->iLastPassCount=2;		//当前桌面连续PASS数（值域[0,2],初值2，正常出牌取0，一家PASS取1，两家PASS取2）
	pDdz->iLastTypeCount=0;		//当前桌面牌型张数（值域[0,1108],初值0，iLastPassCount=0时更新值，=1时保留原值，=2时值为0）
	pDdz->iLastMainPoint=-1;		//当前桌面主牌点数（值域[0,15],初值-1，iLastPassCount=0时更新值，，=1时保留原值，=2时值为-1）
	pDdz->iBmax = 0;					//初始化本局叫牌分数
	pDdz->iOTmax = 0;					//初始化本局桌面牌手数
}
//MO4-END

//M05-START将牌码数组转换为字符串追加(-1或)
//最后修订者:梅险,最后修订时间:15-03-01
void AppendCardsToS(int iCards[],char sMsg[])
{
	int i;
	char sCard[4]="";
	char sCardString[90]="";
	if (iCards[0] == -1)	// PASS
		strcat(sCardString, "-1");
	else					// 不是PASS
	{
		for (i = 0; iCards[i] >= 0; i++)
		{
			if (iCards[i] >= 10)
			{
				sCard[0] = iCards[i] / 10 + '0';
				sCard[1] = iCards[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else
			{
				sCard[0] = iCards[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat(sCardString, sCard);
		}
		sCardString[strlen(sCardString) - 1] = '\0';	//去掉多余的尾部逗号
		
	}
	strcat(sMsg, sCardString);
}
//MO5-END

//M06-START计算推荐出牌
//最后修订者:郭娜，最后修订时间:15-05-27
void HelpPla(struct Ddz * pDdz)
{
	int i,j;
	for(i=0;i<kPlaMax;i++)		//清理
		for(j=0;j<21;j++)
			pDdz->iPlaArr[i][j]=-1;
	pDdz->iPlaCount=0;
	if(pDdz->iLastTypeCount==0)	//连续2家PASS,出任意牌型
	{
		if(pDdz->iOTmax==0)
			HelpfirstPass(pDdz);
		else
			Help0Pass(pDdz);
		return;
	}
	else
	{	//Help0Passpass(pDdz);	
		if(301 == pDdz->iLastTypeCount)//单张
			Help3Single(pDdz);
		else if(402 == pDdz->iLastTypeCount)
			Help4Double( pDdz);
		else if(503 == pDdz->iLastTypeCount)
			Help5Three(pDdz);
		else if(604 == pDdz->iLastTypeCount)
			Help6ThreeOne(pDdz);
		else if(605 == pDdz->iLastTypeCount)
			Help6ThreeDouble(pDdz);
		else if(pDdz->iLastTypeCount > 700 && pDdz->iLastTypeCount < 800)
			Help7LinkSingle(pDdz);
		else if(pDdz->iLastTypeCount > 800 && pDdz->iLastTypeCount < 900)
			Help8LinkDouble(pDdz);
		else if(pDdz->iLastTypeCount > 900 && pDdz->iLastTypeCount < 1000)
			Help9LinkThree(pDdz);
		else if(pDdz->iLastTypeCount > 1000 && pDdz->iLastTypeCount < 1100)
		{
			if((pDdz->iLastTypeCount-1000)%4 == 0)
				Help10LinkThreeSingle(pDdz);
			else
				Help10LinkThreeDouble(pDdz);
		}
		else if(1106 == pDdz->iLastTypeCount)
			Help11FourSingle(pDdz);
		else if(1108 == pDdz->iLastTypeCount)
			Help11FourDouble(pDdz);
		Help2Bomb(pDdz);
		Help1Rocket(pDdz);		
		return;
	}
}
//MO6-END

//M07-START从手牌模拟移除一组牌
//最后修订者:梅险,最后修订时间:15-03-01
void HelpTakeOff(struct Ddz *  pDdz,int num)
{
	int i,j = 0,k,same;
	for(i = 0; pDdz->iOnHand[i]>=0 ;i++)
	{
		same = 0;
		for(k = 0 ;pDdz->iPlaArr[num][k] >=0 ; k++)
		{
			if(pDdz->iOnHand[i] == pDdz->iPlaArr[num][k])
			{
				same = 1;
			}
		}
		if(same!=1)
		{
			pDdz->iPlaOnHand[j] = pDdz->iOnHand[i];
			j++;
		}
	}
	pDdz->iPlaOnHand[j] = -1;
}
//MO7-END


//P01-START重置本轮数据初始值
//最后修订者:梅险,最后修订时间:15-02-08 00:13 
void InitTurn(struct Ddz * pDdz)	
{
	pDdz->iTurnTotal=1;				//初始化总轮数
	pDdz->iTurnNow=1;					//初始化当前轮次
	pDdz->iRoundTotal=1;				//初始化总局数
	pDdz->iRoundNow=1;				//初始化当前局次
	InitRound(pDdz);					//初始化本局数据
}
//P01-END

//P02-START输入信息
//最后修订者:梅险,最后修订时间:15-02-08
void InputMsg(struct Ddz * pDdz)	
{
	cin.getline(pDdz->sCommandIn,80);
}
//P02-END

//P03-START分析处理信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13 
void AnalyzeMsg(struct Ddz * pDdz)	
{
	char sShort[4];
	int i;
	for(i=0;i<3;i++)
		sShort[i]=pDdz->sCommandIn[i];
	sShort[3]='\0';
	if(strcmp(sShort,"DOU")==0)					//版本信息
	{
		GetDou(pDdz);
		check("A版本信息");
		return;
	}
	if(strcmp(sShort,"INF")==0)					//轮局信息
	{
		GetInf(pDdz);
		check("B轮局信息");
		return;
	}
	if(strcmp(sShort,"DEA")==0)					//牌套信息
	{
		GetDea(pDdz);
		check("C牌套信息");
		return;
	}
	if(strcmp(sShort,"BID")==0)					//叫牌过程
	{
		GetBid(pDdz);
		check("D叫牌过程");
		return;
	}
	if(strcmp(sShort,"LEF")==0)					//底牌信息
	{
		GetLef(pDdz);
		check("E底牌信息");
		return;
	}
	if(strcmp(sShort,"PLA")==0)					//出牌过程
	{
		GetPla(pDdz);
		check("F出牌过程");
		return;
	}
	if(strcmp(sShort,"GAM")==0)					//胜负信息
	{
		GetGam(pDdz);
		check("G胜负信息");
		return;
	}
	if(strcmp(sShort,"EXI")==0)					//强制退出
	{
		check("H强制退出");
		exit(0);
	}
	strcpy(pDdz->sCommandOut,"ERROR at module AnalyzeMsg,sCommandIn without match");
	return ;
}
//P03-END

//P0301-START获取并处理版本信息DOU
//最后修订者:张洪民&梅险,最后修订时间:15-02-10 21:04 
//修订内容及目的:修订kPlayerNmae
void GetDou(struct Ddz * pDdz)
{
	int i;								//简单循环变量
	for (i = 0; pDdz->sCommandIn[i] != '\0'; i++)
		pDdz->sVer[i] = pDdz->sCommandIn[i];
	pDdz->sVer[i] = '\0';
	strcpy(pDdz->sCommandOut,"NAME ");
	strcat(pDdz->sCommandOut,kPlayerName);
}
//P0301-END

//P0302-START获取并处理轮局信息INF
//最后修订者:张晨&梅险,最后修订时间:15-02-10 
void GetInf(struct Ddz * pDdz)		//轮次信息处理函数(处理数字信息，对应写入对象成员变量中):输入INFO 1/4 1/9 9 2450     输出OK INFO
{
	char c;					//存当前字节信息		
	int iCount=0;			//记录数据个数
	int iTemp=0;			//中间变量
	int iMessage[6]={0};		//记录数据数组
	int i;
	for(i=5;pDdz->sCommandIn[i] != '\0';i++)
	{
		c= pDdz->sCommandIn[i];	
		if(c>='0' && c<='9')											//当前字符为0-9
		{
			iTemp = iTemp * 10 + c - '0';
			iMessage[iCount] = iTemp;								//晋级选手数
		}
		if(c==',')														//当前字符为逗号,
		{
			iCount++;
			iTemp=0;
		}
	}
	pDdz->iTurnNow = iMessage[0];						//当前轮次
	pDdz->iTurnTotal = iMessage[1];						//总轮数
	pDdz->iRoundNow = iMessage[2];						//当前局次
	pDdz->iRoundTotal =iMessage[3];						//总局数
	pDdz->iLevelUp=iMessage[4];							//晋级数量
	pDdz->iScoreMax =iMessage[5];						//得分界限
	strcpy(pDdz->sCommandOut, "OK INFO");
}
//P0302-END

//P0303-START获取并处理牌套信息DEA
//最后修订者:杨洋&梅险,最后修订时间:15-02-09 22:55 
//修订内容及目的:修改读牌算法
void GetDea(struct Ddz * pDdz)	
{
	int i;			      //简单循环变量
	int iNow = 0;		  //当前处理牌序号
	int iCardId = 0;	//当前处理牌编码
	char c;			      //当前指令字符
	pDdz->cDir = pDdz->sCommandIn[5];     //获取本家AI方位编号
	for(i=0 ; i<21 ; i++)				          //清理iOnhand[]
		pDdz->iOnHand[i] = -1;
	for(i=6 ; pDdz->sCommandIn[i] != '\0'; i++)	//依次读取牌码指令
	{
		c = pDdz->sCommandIn[i];			      //c为当前指令字符
		if(c>='0' && c<='9')				        //当前字符为0-9
		{
			iCardId = iCardId * 10 + c - '0';
			pDdz->iOnHand[iNow] = iCardId;
		}
		if(c==',')							            //当前字符为逗号,
		{
			iCardId = 0;
			iNow++;
		}
	}
	strcpy(pDdz->sCommandOut, "OK DEAL");  //回复信息
	SortById(pDdz->iOnHand);  //iOnHand[]从小到大排序
	CountPoint(pDdz, pDdz->iOnHand);
}
//P0303-END

//P0304-START获取并处理叫牌信息BID
//最后修订者:李思寒&梅险,最后修订时间:15-02-08
void GetBid(struct Ddz * pDdz)	
{
	if(pDdz->sCommandIn[4]=='W')					//如果输入信息为BID WHAT
	{
		strcpy(pDdz->sCommandOut,"BID _0");
		pDdz->sCommandOut[4]=pDdz->cDir;
		pDdz->iBid[pDdz->cDir-'A']=CalBid(pDdz);
		pDdz->sCommandOut[5]=pDdz->iBid[pDdz->cDir-'A']+'0';		//调用叫牌函数
		pDdz->sCommandOut[6]='\0';
		return ;
	}
	if(pDdz->sCommandIn[4]>='A'&&pDdz->sCommandIn[4]<='C')  //输入信息为BID **
	{
		pDdz->iBid[pDdz->sCommandIn[4]-'A']=pDdz->sCommandIn[5]-'0';
		strcpy(pDdz->sCommandOut,"OK BID");
		return;
	}
}
//P0304-END

//将手中牌id转化成牌面数字
int idToNum(int *id,int *num) 
{
	int i;
	for (i = 0; id[i] >= 0; i++)
	{
		if(id[i]<52)
			num[i]=id[i]/4+3;
		else if(id[i]==52)
			num[i]=16;
		else if(id[i]==53)
			num[i]=17;
		else return -1;
	}
	return 0;
}


//P0305-START获取并处理底牌信息LEF
//最后修订者:杨洋&梅险,最后修订时间:15-02-08
void GetLef(struct Ddz * pDdz)	
{
	int i, iCount = 0;
	char c;
	pDdz->cLandlord = pDdz->sCommandIn[9];    //确定地主方
	pDdz->iLef[0]=0;
	pDdz->iLef[1]=0;
	pDdz->iLef[2]=0;
	for (i = 10; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c=pDdz->sCommandIn[i];
		if (c>='0' && c<='9')
			pDdz->iLef[iCount] = pDdz->iLef[iCount] * 10 + c - '0';
		else
			iCount++;
	}
	if (pDdz->sCommandIn[9] == pDdz->cDir)
	{
		pDdz->iOnHand[17] = pDdz->iLef[0];
		pDdz->iOnHand[18] = pDdz->iLef[1];
		pDdz->iOnHand[19] = pDdz->iLef[2];
		pDdz->iOnHand[20] = -1;		
	}
	CountPoint(pDdz, pDdz->iOnHand+17);//注意顺序SortById之前  */                                 
	strcpy(pDdz->sCommandOut, "OK LEFTOVER");
	SortById(pDdz->iOnHand);					//iOnHand[]从小到大排序
//	pDdz->iLastCount[pDdz->sCommandIn[9]-'A']+=3;//更新手中牌的数量                                /************************造成不能正常运行***************/
	pDdz->minOutCount=minHandCount(pDdz->iOnHand,pDdz->iLastCount[pDdz->sCommandIn[9]-'A']);    
}
//P0305-END

//P0306-START获取并处理出牌信息PLA
//最后修订者:梅险,最后修订时间:15-02-08 
void GetPla(struct Ddz * pDdz)	
{
	if(pDdz->sCommandIn[5]=='W')					//接收信息为PLAY WHAT：应调用出牌计算函数计算出牌
	{
		CalPla(pDdz);					//调用出牌计算函数计算出牌
		strcpy(pDdz->sCommandOut,"PLAY _");
		pDdz->sCommandOut[5]=pDdz->cDir;		//输出命令的预备信息准备到sCommandOut数组
		AppendCardsToS(pDdz->iToTable,pDdz->sCommandOut);		//要出牌数组iToTable[]中的数字转化为字符并连接到sCommandOut中
		UpdateMyPla(pDdz);		//根据己方出牌更新数据
	}
	else										//否则收到信息为其它玩家出牌
	{
		UpdateHisPla(pDdz);		//根据他人出牌更新数据       
		strcpy(pDdz->sCommandOut,"OK PLAY");//回复收到
	}
	//当前手数加1
	pDdz->iOTmax++;
}
//P0306-END

//P030602-START根据己方出牌更新数据
//最后修订者:梅险&夏侯有杰,最后修订时间:15-03-01
//修订内容及目的:修改减少手中牌
void UpdateMyPla(struct Ddz * pDdz)	
{
	int i,j,k;
	if(pDdz->iToTable[0]==-1)	//Pass
	{
		pDdz->iOnTable[pDdz->iOTmax][0]=-1;
		pDdz->iLastPassCount++;
		if(pDdz->iLastPassCount>=2)	//连续两家PASS
		{
			pDdz->iLastPassCount=0;
			pDdz->iLastTypeCount=0;
			pDdz->iLastMainPoint=-1;
		}
		pDdz->iOTmax++;
	}
	else						//不是PASS
	{
		//增加桌面牌
		for(i=0;pDdz->iToTable[i]>=0;i++)
			pDdz->iOnTable[pDdz->iOTmax][i]=pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i]=-1;
		pDdz->iOTmax++;
		//减少手中牌
	
		for(j=0 ; pDdz->iToTable[j]>=0 ; j++)
		{	
			for(i=0;pDdz->iOnHand[i]>=0;i++)
				if(pDdz->iOnHand[i]==pDdz->iToTable[j])
				{
					for(k=i;pDdz->iOnHand[k]>=0;k++)
						pDdz->iOnHand[k]=pDdz->iOnHand[k+1];
					break;
				}
		}
		pDdz->iLastPassCount=0;
		pDdz->iLastTypeCount=AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint=AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iLastCount[pDdz->cDir-'A']-=j;//更新手中牌的数量
	}
}
//PO30602-END

//P030603-START根据他人出牌更新数据
//最后修订者:梅险,最后修订时间:15-03-01
void UpdateHisPla(struct Ddz * pDdz)	
{
	int i;
	int iCardId;
	int iNow;
	char cDir;
	char c;

	if(pDdz->sCommandIn[6]=='-')// PLAY ?-1 即PASS
	{
		pDdz->iOnTable[pDdz->iOTmax][0]=-1;
		pDdz->iLastPassCount++;
		if(pDdz->iLastPassCount>=2)	//连续两家PASS
		{
			pDdz->iLastPassCount=0;
			pDdz->iLastTypeCount=0;
			pDdz->iLastMainPoint=-1;
		}
		pDdz->iOTmax++;
	}
	else						// PLAY 出牌
	{
		for(i=0 ; i<21 ; i++)							//清理iToTable[]
			pDdz->iToTable[i] = -1;
		iCardId=0;
		iNow=0;
		cDir=pDdz->sCommandIn[5];						//获取当前出牌者方位
		for(i=6 ; pDdz->sCommandIn[i] != '\0'; i++)		//依次读取牌码
		{
			c = pDdz->sCommandIn[i];					//c为当前指令字符
			if(c>='0' && c<='9')				        //当前字符为0-9
			{
				iCardId = iCardId * 10 + c - '0';
				pDdz->iToTable[iNow] = iCardId;
			}
			if(c==',')									//当前字符为逗号,
			{
				iCardId = 0;
				iNow++;
			}
		}
		//增加桌面牌
		for(i=0;pDdz->iToTable[i]>=0;i++)
			pDdz->iOnTable[pDdz->iOTmax][i]=pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i]=-1;
		pDdz->iLastPassCount=0;
		pDdz->iLastTypeCount=AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint=AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iOTmax++;
		pDdz->iLastCount[cDir-'A']-=iNow;//更新手中牌的数量
		CountPoint(pDdz, pDdz->iOnTable[pDdz->iOTmax]);//更新已知牌点数数量array
	}
}
//PO30603-END


//P0307-START获取并处理胜负信息GAM
//最后修订者:梅险,最后修订时间:15-02-08 00:13 
void GetGam(struct Ddz * pDdz)	
{
	pDdz->cWinner = pDdz->sCommandIn[9];			//胜利者方位编号
	if(pDdz->iRoundNow == pDdz->iRoundTotal)		//如果当前局数与每轮局相等时
	{
		pDdz->iStatus = 0;							//引擎状态变量设为结束
	}
	else											//否则
	{
		pDdz->iRoundNow++;							//当前局次加1
		InitRound(pDdz);							//引擎状态变量设为重新开始下一局
	}
	strcpy(pDdz->sCommandOut, "OK GAMEOVER");
}
//PO307-END

//P04-START输出信息
//最后修订者:梅险,最后修订时间:15-02-08 00:13 
void OutputMsg(struct Ddz * pDdz)	
{
	cout<<pDdz->sCommandOut<<endl;
}
//P04-END

//P05-START计算其它数据
//最后修订者:梅险,最后修订时间:15-02-08
void CalOthers(struct Ddz * pDdz)	
{
	pDdz->iVoid=0;
	PrintPointArray(pDdz);/********************************************************************************************************************************/
}
//P05-END
