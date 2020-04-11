//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//二打一组负责人陈佳雨，戴鹏所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//陈佳雨	QQ：1274162928
//戴鹏		QQ：1219095374 
#define kPlayerName "参赛选手名称"
#define kPlaMax 500
#include "DdzV200.h"

#define SUM 210
#define COF_HAND 10
#define COF_MAINPOINT -2
#define COF_TYPE 1

int check(char a[]);

enum score
{
	S_LinkDouble=19,
	S_LinkSingle=18,
	S_ThreeOneDouble=6,
	S_LinkThreeSingleDouble=10,
	
	S_Double=8,
	S_Single=3,
	S_Three=12,
	S_LinkThree=1,
	S_FourSingleDouble=0,
	S_Bomb=28,
	S_Rocket=0,
};


/*D01-START计算当前手中余牌估值,预设不拆对牌和连牌，建议进一步自行完善
最后修订者:谢文&梅险,最后修订时间:15-02-11*/
double CalCardsValue(struct Ddz * pDdz, int outcards[], int num)	
{
	int i,mainpoint=AnalyzeMainPoint(outcards),type = AnalyzeTypeCount(outcards);	  //mainpoint 计算主牌最小出牌点数		type牌型
	int count=minHandCount(array,output_card(pDdz->iPlaOnHand));//以剩余牌出完手数最低为前提，其他因素累计不得超过？分   count 最少能出完的手数
	double dSum =SUM - count*COF_HAND-2.5*num;//估值	100;// 										//*****************iPlaOnHand可能存在问题***************************/
	
	/*check_card_1("估值 初值",dSum);/*********************************************************    type的值只有-1  对所有牌进行牌型判断不合适  ********************/
	/*check_card_1("最少出牌手数",count);
	check("估值程序");*/
	int  x;

	int  Zd = 0, Dz1 = 0, Dz2 = 0;     //Zd:炸弹的数量，Dz1：统计后的单牌的数量，Dz2：为统计后的双牌的后剩余单牌的数量
	int   Car_count = 0;               //手中剩余牌的数量
	int  card_stye_deal[3][17] = { 0 };                    //处理后的牌
	for (i = 0; outcards[i] >= 0; i++)     //统计手中的牌（存入数组）
	{
		//dSum = dSum - 50;                   //每多一张牌分数减50
		if(outcards[i]==52)
			card_stye_deal[1][16]++;
		else if(outcards[i]==53)
			card_stye_deal[1][17]++;
		else
		{
		x = outcards[i] / 4 + 3;
		card_stye_deal[1][x]++;
		}
		Car_count++;
		
	}
	if(pDdz->iLastTypeCount==0)	//主动出牌
	{

		if (card_stye_deal[1][16]>0||card_stye_deal[1][17]>0)                  //王:每次加20分
		{
			if (card_stye_deal[1][16] == 1&&card_stye_deal[1][17] == 1)    //俩王
			{
				dSum = dSum +40;             
			}
		
			else                                          //一个王
			{
				dSum = dSum +20;       
			}
		}
		for (i = 3; i<16; i++)      
		{
		if (card_stye_deal[2][i]>0 && card_stye_deal[2][i+1]>0 && card_stye_deal[2][i + 2]>0 && card_stye_deal[2][i + 3]>0 && card_stye_deal[2][i + 4]>0) //单顺估值
			{
				dSum = dSum +18;		
				card_stye_deal[2][i]=card_stye_deal[2][i]-1;
				card_stye_deal[2][i+1]=card_stye_deal[2][i+1]-1;
				card_stye_deal[2][i+2]=card_stye_deal[2][i+2]-1;
				card_stye_deal[2][i+3]=card_stye_deal[2][i+3]-1;
				card_stye_deal[2][i+4]=card_stye_deal[2][i+4]-1;
				if (card_stye_deal[2][i + 5]>0)         
				{
					
					dSum =dSum +3;
					card_stye_deal[2][i+5]=card_stye_deal[2][i+5]-1;
					if (card_stye_deal[2][i + 6]>0)    
					{
						dSum = dSum +3;
						card_stye_deal[2][i+6]=card_stye_deal[2][i+6]-1;
						if(card_stye_deal[2][i + 7]>0)
						{
							
							dSum =dSum+3;
							card_stye_deal[2][i+7]=card_stye_deal[2][i+7]-1;
							if(card_stye_deal[2][i + 8]>0)
							{
								dSum = dSum +3;
								card_stye_deal[2][i+8]=card_stye_deal[2][i+8]-1;
								if(card_stye_deal[2][i + 9]>0)
								{
									dSum= dSum +3;
									card_stye_deal[2][i+9]=card_stye_deal[2][i+9]-1;
								}
							}
						}
					}
				}
			}
		if (card_stye_deal[2][i] == 4)        //炸弹的数量(满足四张牌)
			{
				
				dSum+=28;
				card_stye_deal[2][i]=0;
			}
			if (card_stye_deal[2][i] == 3)        //三张牌
			{
				dSum+=12;
				card_stye_deal[2][i]=0;
			}
			if (card_stye_deal[2][i] == 3)        //两张牌
			{
				dSum+=8;
				card_stye_deal[2][i]=0;
			}
			
			if (card_stye_deal[2][i] == 1)
			{
				
				dSum+=3;     //单独的牌                    /******************************************************************************************/
				card_stye_deal[2][i]=0;
			}
		}
	}
	else           //被动出牌
	{
		if (card_stye_deal[1][16]>0||card_stye_deal[1][17]>0)                  //王:每次加20分
		{
			if (card_stye_deal[1][16] == 1&&card_stye_deal[1][17] == 1)    //俩王
			{
				dSum = dSum +40;             
			}
		
			else                                          //一个王
			{
				dSum = dSum +20;       
			}
		}
	}

	if(Car_count==0)
	{
		dSum+=10000;
	}
	
	//
	//{
	//	check("主动出牌");
	////	dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
	//	//按照mainpoint从小到大出
	//	//顺>带>单>双	
	//	check_card_1("牌型",type);        /***************************************type值有问题*************************************************/
	//	switch(type/100)
	//	{
	//		case 11:
	//			dSum +=0;// COF_TYPE*S_FourSingleDouble;             //四带二单  四带二对
	//			check_card_1("估值 四带后",dSum);
	//			break;
	//		case 10:
	//			dSum +=10;// COF_TYPE*S_LinkThreeSingleDouble;       //三顺带单   三顺带对
	//			check_card_1("估值 三顺带后",dSum);
	//			break;
	//		case 9:
	//			dSum +=1;// COF_TYPE*S_LinkThree;                 //三顺
	//			check_card_1("估值 三顺后",dSum);
	//			break;
	//		case 8:
	//			dSum +=19;// COF_TYPE*S_LinkDouble;               //双顺
	//			check_card_1("估值 双顺后",dSum);
	//			break;
	//		case 7:
	//			dSum +=18;// COF_TYPE*S_LinkSingle;              //单顺
	//			check_card_1("估值 单顺后",dSum);
	//			break;
	//		case 6:
	//			dSum +=6;// COF_TYPE*S_ThreeOneDouble;         //三带一 三带二
	//			check_card_1("估值 三带后",dSum);
	//			break;
	//		case 5:
	//			dSum +=12;// COF_TYPE*S_Three;                  //三张
	//			check_card_1("估值 三张牌后",dSum);
	//			break;
	//		case 4:
	//			dSum +=8;// COF_TYPE*S_Double;                 //对子
	//			check_card_1("估值 对子后",dSum);
	//			break;
	//		case 3:
	//			dSum += 3;//COF_TYPE*S_Single;               //单牌
	//			check_card_1("估值 单牌后",dSum);
	//			break;
	//		case 2:
	//		case 1:
	//			dSum += 0;//优先级最低
	//			check_card_1("估值 其他后",dSum);
	//			break;
	//		default:
	//			dSum += 0;//优先级最低
	//			break;
	/*		
	//	}*/	
	//check_card_1("估值 主动出牌后",dSum);
		/*
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
		*/
	//}
	//else//被动出牌
	//{
		//check("被动出牌");
		////考虑虽然能有大过的牌，但是不能拆得过于零散，否则pass
		////手数相同时考虑剩余牌的优势
		//if(count==0)
		//{
		//	dSum = SUM;//必须出手
		//	printf("we win!\n");
		//}
		//else if(count==1)                                    // count 最少能出完的手数
		//{
		//	check("剩一手");
		//	if(type==204)                              //202 炸弹 204是炸弹
		//	{
		//		int j;
		//		for(j=0; j<3 && j!=pDdz->cDir-'A';j++)
		//		{
		//			if(pDdz->iLastCount[j]==4) break;           //当前各玩家手中牌数
		//		}
		//		if(j==3)
		//			dSum = 0;//也许能引出一个炸呢
		//		else
		//			dSum = SUM;//必须出手
		//	}
		//	else if(type==102)                             //104 王炸    102
		//	{
		//		int j;
		//		for(j=0; j<3 && j!=pDdz->cDir-'A';j++)
		//		{
		//			if(pDdz->iLastCount[j]==2) break;
		//		}
		//		if(j<3 && pDdz->iLastPointArray[j]==0)//当前有玩家手中可能只剩火箭
		//		{
		//			dSum = -1;//pass
		//		}
		//		else
		//		{
		//			int k;
		//			for(j=0; j<3 && j!=pDdz->cDir-'A';j++)
		//			{
		//				if(pDdz->iLastCount[j]==4) break;
		//			}
		//			for(k=mainpoint-3+1; k<13; k++)
		//			{
		//				if(pDdz->iLastPointArray[k]==0) break;
		//			}
		//			if(j<3 && k<13)//当前有玩家手中只剩比自己大的炸
		//			{
		//				dSum = -1;//pass
		//			}
		//			else
		//			{//从小到大
		//				dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//			}
		//		}
		//	}
		//	else if(type==301)//单
		//	{
		//		int k;
		//		for(k=mainpoint-3+1; k<13; k++)                         //mainpoint 计算主牌最小出牌点数
		//		{
		//			if(pDdz->iLastPointArray[k]<4) break;                //统计已知牌，按点数计数--------------------------
		//		}
		//		if(k==13 && pDdz->iLastPointArray[13]==2)//无敌了
		//			dSum = SUM;//必须出手
		//		else
		//		{//从小到大
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}
		//	}
		//	else if(type==402)//双
		//	{
		//		int k;
		//		for(k=mainpoint-3+1; k<13; k++)
		//		{
		//			if(pDdz->iLastPointArray[k]<3) break;
		//		}
		//		if(k==13 && pDdz->iLastPointArray[13]!=0)//无敌了
		//			dSum = SUM;//必须出手
		//		else
		//		{//从小到大
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}
		//	}
		//	else
		//	{
		//		dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//	}
		//}
		//else//剩余手数>=2
		//{
		//	check("手数大于二");
		//	if(type==102||type==204)//炸或火箭
		//	{
		//		
		//		if(pDdz->iLastTypeCount==104 && !IsPartner(pDdz) && LastPlayerLeft(pDdz)<=4 && pDdz->iLastPointArray[13]==0)//本身出的就是炸
		//		{
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}				
		//		else
		//		{
		//			dSum = -1;//pass
		//		}
		//	}
		//	else if(count-pDdz->minOutCount>=3)//拆得很零散
		//	{
		//		if(!IsPartner(pDdz) && LastPlayerLeft(pDdz)<=4)
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		else
		//			dSum = -1;//pass
		//	}
		//	else if(count-pDdz->minOutCount==1)//正好能大过 垫牌
		//	{
		//		dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//	}
		//	else//余牌的优势
		//	{
		//	}
	//	//}
	//}
	//check_card_1("估值 最终估值",dSum);
	return dSum; 
}
//D01-END

//double CalCardsValue(int iPlaOnHand[])	
//{
//	int i;
//	double dSum = 100;			//估值
//	for(i=0;iPlaOnHand[i]>=0;i++)
//	{
//		dSum=dSum-5;			//手牌越少越好没多一张牌优势减5分
//		if (i >= 1 && iPlaOnHand[i - 1] / 4 == iPlaOnHand[i] / 4)
//			dSum = dSum + 2;	//相邻两牌同点加2分
//		if (i >= 4 && iPlaOnHand[i - 4] / 4 <=7
//			&&iPlaOnHand[i - 4] / 4 + 1 ==iPlaOnHand[i - 3] / 4 
//			&&iPlaOnHand[i - 3] / 4 + 1 ==iPlaOnHand[i - 2] / 4
//			&&iPlaOnHand[i - 2] / 4 + 1 ==iPlaOnHand[i - 1] / 4 
//			&&iPlaOnHand[i - 1] / 4 + 1 ==iPlaOnHand[i] / 4)
//			dSum = dSum + 6;	//2以下相邻五牌单顺加6分
//	}
//	return dSum; 
//}

//I02-START计算己方叫牌策略:预设3分或0分，建议进一步自行完善
//最后修订者:梅险,最后修订时间:15-02-12
int CalBid(struct Ddz * pDdz)	
{
	int i;
	int iMyBid=3;			//叫牌
	for(i=0;i<3;i++)
		if(pDdz->iBid[i]>=3)
			iMyBid=0;
	if(!iMyBid)
	{

	}
	return iMyBid;
}
//I02-END


//P030601-START计算己方出牌策略
//最后修订者:夏侯有杰&梅险,最后修订时间:15-02-12
void CalPla(struct Ddz * pDdz)	
{
	int i;
	double dValueNow;
	double dValueMax=-9999;
	int iMax = 0;
	HelpPla(pDdz);				//主要计算推荐出牌pDdz->iPlaArr[],pDdz->iPlaCount
	for(i=0;i<pDdz->iPlaCount;i++)
	{
		HelpTakeOff(pDdz,i);	//假设取走了第i组牌，将剩余的牌放入pDdz->iPlaOnHand[]
		
		output_card(pDdz->iPlaOnHand);
		/*check_card("剩余的牌",pDdz->iPlaOnHand);
		check_card("模拟出的牌",pDdz->iPlaArr[i]);*/
		dValueNow = CalCardsValue(pDdz,pDdz->iPlaOnHand,CountCards(pDdz->iPlaOnHand));			//计算余牌估值
	//	check_card_1("余牌估值",dValueNow);
		if (dValueNow > dValueMax)
		{
			dValueMax = dValueNow;
			iMax = i;
		}
	}
	for (i = 0;pDdz->iPlaArr[iMax][i] >= 0; i++)
		pDdz->iToTable[i] = pDdz->iPlaArr[iMax][i];
	pDdz->iToTable[i] = -1;
}
//P030601-END

//P00-START主控模块
//最后修订者:梅险,最后修订时间:15-02-08
int	main( )	
{
	struct Ddz tDdz, *pDdz=&tDdz;
	InitTurn(pDdz);			//初始化数据
	while(pDdz->iStatus!=0)
	{
		InputMsg(pDdz);			//输入信息
		//check("01输入信息cpp");
		AnalyzeMsg(pDdz);		//分析处理信息
		//check("02分析处理信息cpp");
		OutputMsg(pDdz);		//输出信息
		//check("03输出信息cpp");
		CalOthers(pDdz);		//计算其它数据
		//check("04计算其它数据cpp");
	}
	return 0;
}
//P00-END
