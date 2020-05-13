//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//二打一组负责人陈佳雨，戴鹏所带博弈组开发维护
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//陈佳雨	QQ：1274162928
//戴鹏		QQ：1219095374 
#define kPlayerName "����ѡ������"
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


/*D01-START���㵱ǰ�������ƹ�ֵ,Ԥ�費����ƺ����ƣ������һ����������
����޶���:л��&÷��,����޶�ʱ��:15-02-11*/
double CalCardsValue(struct Ddz * pDdz, int outcards[], int num)	
{
	int i,mainpoint=AnalyzeMainPoint(outcards),type = AnalyzeTypeCount(outcards);	  //mainpoint ����������С���Ƶ���		type����
	int count=minHandCount(array,output_card(pDdz->iPlaOnHand));//��ʣ���Ƴ����������Ϊǰ�ᣬ���������ۼƲ��ó�������   count �����ܳ��������
	double dSum =SUM - count*COF_HAND-2.5*num;//��ֵ	100;// 										//*****************iPlaOnHand���ܴ�������***************************/
	
	/*check_card_1("��ֵ ��ֵ",dSum);/*********************************************************    type��ֵֻ��-1  �������ƽ��������жϲ�����  ********************/
	/*check_card_1("���ٳ�������",count);
	check("��ֵ����");*/
	int  x;

	int  Zd = 0, Dz1 = 0, Dz2 = 0;     //Zd:ը����������Dz1��ͳ�ƺ�ĵ��Ƶ�������Dz2��Ϊͳ�ƺ��˫�Ƶĺ�ʣ�൥�Ƶ�����
	int   Car_count = 0;               //����ʣ���Ƶ�����
	int  card_stye_deal[3][17] = { 0 };                    //���������
	for (i = 0; outcards[i] >= 0; i++)     //ͳ�����е��ƣ��������飩
	{
		//dSum = dSum - 50;                   //ÿ��һ���Ʒ�����50
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
	if(pDdz->iLastTypeCount==0)	//��������
	{

		if (card_stye_deal[1][16]>0||card_stye_deal[1][17]>0)                  //��:ÿ�μ�20��
		{
			if (card_stye_deal[1][16] == 1&&card_stye_deal[1][17] == 1)    //����
			{
				dSum = dSum +40;             
			}
		
			else                                          //һ����
			{
				dSum = dSum +20;       
			}
		}
		for (i = 3; i<16; i++)      
		{
		if (card_stye_deal[2][i]>0 && card_stye_deal[2][i+1]>0 && card_stye_deal[2][i + 2]>0 && card_stye_deal[2][i + 3]>0 && card_stye_deal[2][i + 4]>0) //��˳��ֵ
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
		if (card_stye_deal[2][i] == 4)        //ը��������(����������)
			{
				
				dSum+=28;
				card_stye_deal[2][i]=0;
			}
			if (card_stye_deal[2][i] == 3)        //������
			{
				dSum+=12;
				card_stye_deal[2][i]=0;
			}
			if (card_stye_deal[2][i] == 3)        //������
			{
				dSum+=8;
				card_stye_deal[2][i]=0;
			}
			
			if (card_stye_deal[2][i] == 1)
			{
				
				dSum+=3;     //��������                    /******************************************************************************************/
				card_stye_deal[2][i]=0;
			}
		}
	}
	else           //��������
	{
		if (card_stye_deal[1][16]>0||card_stye_deal[1][17]>0)                  //��:ÿ�μ�20��
		{
			if (card_stye_deal[1][16] == 1&&card_stye_deal[1][17] == 1)    //����
			{
				dSum = dSum +40;             
			}
		
			else                                          //һ����
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
	//	check("��������");
	////	dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
	//	//����mainpoint��С�����
	//	//˳>��>��>˫	
	//	check_card_1("����",type);        /***************************************typeֵ������*************************************************/
	//	switch(type/100)
	//	{
	//		case 11:
	//			dSum +=0;// COF_TYPE*S_FourSingleDouble;             //�Ĵ�����  �Ĵ�����
	//			check_card_1("��ֵ �Ĵ���",dSum);
	//			break;
	//		case 10:
	//			dSum +=10;// COF_TYPE*S_LinkThreeSingleDouble;       //��˳����   ��˳����
	//			check_card_1("��ֵ ��˳����",dSum);
	//			break;
	//		case 9:
	//			dSum +=1;// COF_TYPE*S_LinkThree;                 //��˳
	//			check_card_1("��ֵ ��˳��",dSum);
	//			break;
	//		case 8:
	//			dSum +=19;// COF_TYPE*S_LinkDouble;               //˫˳
	//			check_card_1("��ֵ ˫˳��",dSum);
	//			break;
	//		case 7:
	//			dSum +=18;// COF_TYPE*S_LinkSingle;              //��˳
	//			check_card_1("��ֵ ��˳��",dSum);
	//			break;
	//		case 6:
	//			dSum +=6;// COF_TYPE*S_ThreeOneDouble;         //����һ ������
	//			check_card_1("��ֵ ������",dSum);
	//			break;
	//		case 5:
	//			dSum +=12;// COF_TYPE*S_Three;                  //����
	//			check_card_1("��ֵ �����ƺ�",dSum);
	//			break;
	//		case 4:
	//			dSum +=8;// COF_TYPE*S_Double;                 //����
	//			check_card_1("��ֵ ���Ӻ�",dSum);
	//			break;
	//		case 3:
	//			dSum += 3;//COF_TYPE*S_Single;               //����
	//			check_card_1("��ֵ ���ƺ�",dSum);
	//			break;
	//		case 2:
	//		case 1:
	//			dSum += 0;//���ȼ����
	//			check_card_1("��ֵ ������",dSum);
	//			break;
	//		default:
	//			dSum += 0;//���ȼ����
	//			break;
	/*		
	//	}*/	
	//check_card_1("��ֵ �������ƺ�",dSum);
		/*
		if(301 == pDdz->iLastTypeCount)//����
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
	//else//��������
	//{
		//check("��������");
		////������Ȼ���д�����ƣ����ǲ��ܲ�ù�����ɢ������pass
		////������ͬʱ����ʣ���Ƶ�����
		//if(count==0)
		//{
		//	dSum = SUM;//�������
		//	printf("we win!\n");
		//}
		//else if(count==1)                                    // count �����ܳ��������
		//{
		//	check("ʣһ��");
		//	if(type==204)                              //202 ը�� 204��ը��
		//	{
		//		int j;
		//		for(j=0; j<3 && j!=pDdz->cDir-'A';j++)
		//		{
		//			if(pDdz->iLastCount[j]==4) break;           //��ǰ�������������
		//		}
		//		if(j==3)
		//			dSum = 0;//Ҳ��������һ��ը��
		//		else
		//			dSum = SUM;//�������
		//	}
		//	else if(type==102)                             //104 ��ը    102
		//	{
		//		int j;
		//		for(j=0; j<3 && j!=pDdz->cDir-'A';j++)
		//		{
		//			if(pDdz->iLastCount[j]==2) break;
		//		}
		//		if(j<3 && pDdz->iLastPointArray[j]==0)//��ǰ��������п���ֻʣ���
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
		//			if(j<3 && k<13)//��ǰ���������ֻʣ���Լ����ը
		//			{
		//				dSum = -1;//pass
		//			}
		//			else
		//			{//��С����
		//				dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//			}
		//		}
		//	}
		//	else if(type==301)//��
		//	{
		//		int k;
		//		for(k=mainpoint-3+1; k<13; k++)                         //mainpoint ����������С���Ƶ���
		//		{
		//			if(pDdz->iLastPointArray[k]<4) break;                //ͳ����֪�ƣ�����������--------------------------
		//		}
		//		if(k==13 && pDdz->iLastPointArray[13]==2)//�޵���
		//			dSum = SUM;//�������
		//		else
		//		{//��С����
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}
		//	}
		//	else if(type==402)//˫
		//	{
		//		int k;
		//		for(k=mainpoint-3+1; k<13; k++)
		//		{
		//			if(pDdz->iLastPointArray[k]<3) break;
		//		}
		//		if(k==13 && pDdz->iLastPointArray[13]!=0)//�޵���
		//			dSum = SUM;//�������
		//		else
		//		{//��С����
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}
		//	}
		//	else
		//	{
		//		dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//	}
		//}
		//else//ʣ������>=2
		//{
		//	check("�������ڶ�");
		//	if(type==102||type==204)//ը����
		//	{
		//		
		//		if(pDdz->iLastTypeCount==104 && !IsPartner(pDdz) && LastPlayerLeft(pDdz)<=4 && pDdz->iLastPointArray[13]==0)//�������ľ���ը
		//		{
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		}				
		//		else
		//		{
		//			dSum = -1;//pass
		//		}
		//	}
		//	else if(count-pDdz->minOutCount>=3)//��ú���ɢ
		//	{
		//		if(!IsPartner(pDdz) && LastPlayerLeft(pDdz)<=4)
		//			dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//		else
		//			dSum = -1;//pass
		//	}
		//	else if(count-pDdz->minOutCount==1)//�����ܴ�� ����
		//	{
		//		dSum += COF_MAINPOINT*mainpoint;//[1.5,8.5]
		//	}
		//	else//���Ƶ�����
		//	{
		//	}
	//	//}
	//}
	//check_card_1("��ֵ ���չ�ֵ",dSum);
	return dSum; 
}
//D01-END

//double CalCardsValue(int iPlaOnHand[])	
//{
//	int i;
//	double dSum = 100;			//��ֵ
//	for(i=0;iPlaOnHand[i]>=0;i++)
//	{
//		dSum=dSum-5;			//����Խ��Խ��û��һ�������Ƽ�5��
//		if (i >= 1 && iPlaOnHand[i - 1] / 4 == iPlaOnHand[i] / 4)
//			dSum = dSum + 2;	//��������ͬ���2��
//		if (i >= 4 && iPlaOnHand[i - 4] / 4 <=7
//			&&iPlaOnHand[i - 4] / 4 + 1 ==iPlaOnHand[i - 3] / 4 
//			&&iPlaOnHand[i - 3] / 4 + 1 ==iPlaOnHand[i - 2] / 4
//			&&iPlaOnHand[i - 2] / 4 + 1 ==iPlaOnHand[i - 1] / 4 
//			&&iPlaOnHand[i - 1] / 4 + 1 ==iPlaOnHand[i] / 4)
//			dSum = dSum + 6;	//2�����������Ƶ�˳��6��
//	}
//	return dSum; 
//}

//I02-START���㼺�����Ʋ���:Ԥ��3�ֻ�0�֣������һ����������
//����޶���:÷��,����޶�ʱ��:15-02-12
int CalBid(struct Ddz * pDdz)	
{
	int i;
	int iMyBid=3;			//����
	for(i=0;i<3;i++)
		if(pDdz->iBid[i]>=3)
			iMyBid=0;
	if(!iMyBid)
	{

	}
	return iMyBid;
}
//I02-END


//P030601-START���㼺�����Ʋ���
//����޶���:�ĺ��н�&÷��,����޶�ʱ��:15-02-12
void CalPla(struct Ddz * pDdz)	
{
	int i;
	double dValueNow;
	double dValueMax=-9999;
	int iMax = 0;
	HelpPla(pDdz);				//��Ҫ�����Ƽ�����pDdz->iPlaArr[],pDdz->iPlaCount
	for(i=0;i<pDdz->iPlaCount;i++)
	{
		HelpTakeOff(pDdz,i);	//����ȡ���˵�i���ƣ���ʣ����Ʒ���pDdz->iPlaOnHand[]
		
		output_card(pDdz->iPlaOnHand);
		/*check_card("ʣ�����",pDdz->iPlaOnHand);
		check_card("ģ�������",pDdz->iPlaArr[i]);*/
		dValueNow = CalCardsValue(pDdz,pDdz->iPlaOnHand,CountCards(pDdz->iPlaOnHand));			//�������ƹ�ֵ
	//	check_card_1("���ƹ�ֵ",dValueNow);
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

//P00-START����ģ��
//����޶���:÷��,����޶�ʱ��:15-02-08
int	main( )	
{
	struct Ddz tDdz, *pDdz=&tDdz;
	InitTurn(pDdz);			//��ʼ������
	while(pDdz->iStatus!=0)
	{
		InputMsg(pDdz);			//������Ϣ
		//check("01������Ϣcpp");
		AnalyzeMsg(pDdz);		//����������Ϣ
		//check("02����������Ϣcpp");
		OutputMsg(pDdz);		//�����Ϣ
		//check("03�����Ϣcpp");
		CalOthers(pDdz);		//������������
		//check("04������������cpp");
	}
	return 0;
}
//P00-END
