//����汾		2017����� 
//���������������պ����ѧ���������Э���ܸ�������Ծ��ͳһ����
//�����鸺�����������������鿪��ά��
//�������Թ�����Դ����,�����ϳ�Աά�� 
//�������������й���ѧ����������Ĵ���������Ŀ2017�潻��Э��
//��������ṩ����Э����÷�ʾ���ͼ�AI����˼·�������������иĽ����ƴ������
//��������ͽ����������Ǿ�����ϵ
//��Ծ��	QQ��836473734
//����		QQ��1755923844
#include"stdafx.h"


//全局信息初始化
void InitGame(Game *g)		//【g为Game类中的一个对象，类定义在头文件"Game.h"中】
{
	g->turn = 0;		//【轮数赋为零】
	g->num = 0;			//【顺序数赋为零】

	int i,j;
	for(i=0;i<13;i++)	
	{
		g->hand[i]=-1;					//【每张手牌的编码都赋为-1】
		g->dHand[i]=-1;					//【每张明手牌的编码都赋为-1】
		for(j=0;j<4;j++)				//【手牌中四种花色的牌的花色编码赋为-1,数目赋为0，最大牌点赋为零，是否将牌赋为否】
		{
			g->suit[j].cards[i]=-1;
			g->suit[j].cardnum=0;
			g->suit[j].bigCardNum=0;
			g->suit[j].isLong=false;
		}
	}
	g->color=-1;		//【定约花色赋为-1】
	g->position=-1;		//【引擎方位赋为-1】
	g->vulnerable=0;	//【局况赋为0】
	g->nowBid[0]=-1;	//【当前叫牌方位赋为-1】
	g->nowBid[1]=-1;	//【当前叫牌墩数赋为-1】
	g->nowBid[2]=-1;	//【当前叫牌花色赋为-1】
	g->cardPoint=0;		//【牌力，大牌点，当前轮数，总轮数，除pass叫牌数量赋为0，引擎状态赋为1】
	g->honorCardPoint=0;
	g->EngineState=1;
	g->roundNow=0;
	g->roundTotal=0;
	g->nowbidnum=0;
	g->isAverage=false;	//【未经接到定约】
	g->isTidy=false;	//【牌型不是均牌型】
	g->isContover=false;//【未整理过手牌】
}


//以下是出牌相关函数

void gameNext(Game *model, char card) {

	//记录出牌
	model->historyCard[model->turn * 4 + model->num] = card;

	//索引加一
	model->num++;
	if (model->num == 4) {
		model->num = 0;
		model->turn++;
	}
}

/**
 * whoBigger - 此前本轮出牌最大的人
 * @description: 只有2、3出牌时才能调用本函数，1、2不可以
 */
int whoBigger(Game *model) {
	if (model->num == 2) {//0、1出牌
		if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 1] % 4)) {//0、1同花色，点数大者大
			if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 1]) {
				return 1;
			}
			return 0;
		}
		else if ((model->historyCard[model->turn * 4 + 1] % 4) == model->color) {//1将吃
			return 1;
		}
		return 0;
	}
	else if (model->num == 3) {//0、1、2出牌
		int bigger = 0;
		if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 1] % 4)) {//0、1同花色
			if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 1]) {
				bigger = 1;
			}
		}
		else if ((model->historyCard[model->turn * 4 + 1] % 4) == model->color) {//1将吃  只有花色不同时才可能出现将吃
			bigger = 1;
		}
		if (bigger == 0) {
			if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 2] % 4)) {//0、2同花色
				if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 2]) {
					bigger = 2;
				}
			}
			else if ((model->historyCard[model->turn * 4 + 2] % 4) == model->color) {//2将吃
				bigger = 2;
			}
		}
		else {
			if ((model->historyCard[model->turn * 4 + 1] % 4) == (model->historyCard[model->turn * 4 + 2] % 4)) {//1、2同花色
				if (model->historyCard[model->turn * 4 + 1] < model->historyCard[model->turn * 4 + 2]) {
					bigger = 2;
				}
			}
			else if ((model->historyCard[model->turn * 4 + 2] % 4) == model->color) {//2将吃
				bigger = 2;
			}
		}
		return bigger;
	}
	return 0;
}

//领出
int GetPlayFor0(Game* pgame, char *putCards, int num) {
	int index = 0;//默认出0号牌

	//如果有大牌点领出大牌点，否则随机
	int i;
	for (i = 0; i < num; i++) {
		if ((putCards[i] / 4) > 10) {//大牌领出
			index = i;
			break;
		}
	}
	if (i == num) {
		index = rand() % num;
	}
	return index;
}

//领出下家
int GetPlayFor1(Game* pgame, char *putCards, int num) {
	int index = 0;//默认出0号牌

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//候选牌张是否跟领出同花色

	if (haveSameColor) {//有与领出人同花色的牌
		for (int i = 0; i < num; i++) {
			if (putCards[i] > pgame->historyCard[pgame->turn * 4]) {//较大牌得墩
				index = i;
				break;
			}
		}
		//不能得墩，出0
	}
	else if (pgame->historyCard[pgame->turn * 4] % 4 != pgame->color) {//领出非将牌，将吃得墩
		for (int i = 0; i < num; i++) {
			if (putCards[i] == pgame->color && (putCards[i] / 4) <= 10) {//小将牌将吃得墩
				index = i;
				break;
			}
		}
		//不能得墩，出0
	}
	return index;
}

//领出同伴
int GetPlayFor2(Game* pgame, char *putCards, int num) {
	int index = 0;//默认出0号牌

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//候选牌张是否跟领出同花色
	int bigger = whoBigger(pgame);//此前本轮谁出牌最大

	if (bigger == 1) {//对手得墩
		if (haveSameColor) {
			//对手将吃得墩，出0
			if (pgame->historyCard[pgame->turn * 4] % 4 != pgame->color) {
				for (int i = 0; i < num; i++) {
					if (putCards[i] > pgame->historyCard[pgame->turn * 4 + 1]) {//更大的牌得墩
						index = i;
						break;
					}
				}
				//不能得墩，出0
			}
		}
		else {//将吃得墩
			if (pgame->historyCard[pgame->turn * 4] % 4 == pgame->color) {//对手将吃得墩
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color && putCards[i] > pgame->historyCard[pgame->turn * 4 + 1]) {//更大的将牌得墩
						index = i;
						break;
					}
				}
				//不能得墩，出0
			}
			else {
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color) {//将牌得墩
						index = i;
						break;
					}
				}
				//不能得墩，出0
			}
		}
	}
	else {//领出人得墩
		if (haveSameColor) {
			if (pgame->historyCard[pgame->turn * 4] / 4 < 10) {//领出小牌
				int i;
				for (i = 0; i < num; i++) {
					if (putCards[i] / 4 > 10) {//更大的牌
						index = i;
						break;
					}
				}
				if (i == num) {//无大牌，随机
					index = rand() % num;
				}
			}
			//出0
		}
		else {
			if (pgame->historyCard[pgame->turn * 4] / 4 < 10) {//领出小牌
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color && putCards[i] / 4 < 10) {//小将牌
						index = i;
						break;
					}
				}
				//出0
			}
			//出0
		}
	}
	return index;
}

//收官
int GetPlayFor3(Game* pgame, char *putCards, int num) {
	int index = 0;//默认出0号牌

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//候选牌张是否跟领出同花色
	int bigger = whoBigger(pgame);//此前本轮谁出牌最大

	if (bigger == 1) {//同伴得墩，垫一张小牌
		//与领出人花色相同，出0
		if (!haveSameColor) {//与领出人花色不同
			//领出将牌，出0
			if ((pgame->historyCard[pgame->turn * 4] % 4) != pgame->color) {
				for (int i = 0; i < num; i++) {
					if ((putCards[i] % 4) != pgame->color) {//垫一张非将牌
						index = i;
						break;
					}
				}
				//候选牌张全是将牌，出0
			}
		}
	}
	else {//同伴未得墩，则尽可能得墩
		if ((pgame->historyCard[pgame->turn * 4] % 4) == pgame->color) {//领出将牌
			if (haveSameColor) {//与领出人同花色，只能用较大牌得墩
				for (int i = 0; i < num; i++) {
					if (putCards[i] > pgame->historyCard[pgame->turn * 4 + bigger]) {//更大的将牌得墩
						index = i;
						break;
					}
				}
				//不能得墩，出0
			}
			//不能利用将吃，即不能得墩，出0
		}
		else {//领出非将牌
			if (haveSameColor) {//与领出人同花色，只能用较大牌得墩
				//领出人同伴利用将牌得墩，出0
				if (bigger != 2 || (pgame->historyCard[pgame->turn * 4 + 2] % 4) != pgame->color) {
					for (int i = 0; i < num; i++) {
						if (putCards[i] > pgame->historyCard[pgame->turn * 4 + bigger]) {
							index = i;
							break;
						}
					}
					//不能得墩，出0
				}
			}
			else {//与领出人花色不同，只能利用将吃得墩
				if (bigger == 2 && (pgame->historyCard[pgame->turn * 4 + 2] % 4) == pgame->color) {//领出人同伴利用将牌得墩
					for (int i = 0; i < num; i++) {
						if (putCards[i] == pgame->color && putCards[i] > pgame->historyCard[pgame->turn * 4 + 2]) {//更大将牌得墩
							index = i;
							break;
						}
					}
					//不能得墩，出0
				}
				else {//最小将牌得墩
					for (int i = 0; i < num; i++) {
						if (putCards[i] == pgame->color) {
							index = i;
							break;
						}
					}
					//不能得墩，出0
				}
			}
		}
	}
	return index;
}

char GetPlay(Game* pgame, char *r_play, int flag)
{
	if (pgame->turn == 0 && pgame->num == 0)//首攻出长套第三张
	{
		char putCard = -1;
		for (int i = 0; i<4; i++)	//出牌
		{
			if (pgame->suit[i].isLong&&pgame->suit[i].cardnum >= 4)
			{
				int num = pgame->suit[i].cardnum;
				num -= 3;
				putCard = pgame->suit[i].cards[num];
				if (putCard<10)
					sprintf_s(r_play, 80, "PLAY %c%c", pgame->position, putCard + '0');
				else
					sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->position, putCard / 10 + '0', putCard % 10 + '0');
				break;
			}
		}
		for (int i = 0; i<13; i++)	//已出的牌初始化为-1
		{
			if (pgame->hand[i] == putCard)
				pgame->hand[i] = -1;
			if (pgame->dHand[i] == putCard)
				pgame->dHand[i] = -1;
		}
		return putCard;
	}
	else {
		char putCards[13];
		int num, index = 0;//默认出0

		//取得可行牌
		if (flag == 0)
			num = GetHUseableCard(pgame, putCards);
		else
			num = GetDUseableCard(pgame, putCards);

		if (num > 1) {//有多张可出牌
			if (pgame->num == 0) {//领出
				index = GetPlayFor0(pgame, putCards, num);
			}
			else if (pgame->num == 1) {//领出人下家
				index = GetPlayFor1(pgame, putCards, num);
			}
			else if (pgame->num == 2) {//领出人同伴
				index = GetPlayFor2(pgame, putCards, num);
			}
			else if (pgame->num == 3) {//收官
				index = GetPlayFor3(pgame, putCards, num);
			}
		}
		//只有一张可出牌，出0

		if (putCards[index]<10)
		{
			if (flag == 0)
				sprintf_s(r_play, 80, "PLAY %c%c", pgame->position, putCards[index] + '0');
			else
				sprintf_s(r_play, 80, "PLAY %c%c", pgame->dPosition, putCards[index] + '0');
		}
		else
		{
			if (flag == 0)
				sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->position, putCards[index] / 10 + '0', putCards[index] % 10 + '0');
			else
				sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->dPosition, putCards[index] / 10 + '0', putCards[index] % 10 + '0');
		}

		for (int i = 0; i<13; i++)
		{
			if (pgame->hand[i] == putCards[index])
				pgame->hand[i] = -1;
			if (pgame->dHand[i] == putCards[index])
				pgame->dHand[i] = -1;
		}
		return putCards[index];
	}
}

int GetHUseableCard(Game* pgame, char *r_chars)
{
	int num = 0;
	if (pgame->num == 0) {//领出
		for (int i = 0; i < 13; i++) {
			if (pgame->hand[i] != -1) {
				r_chars[num++] = pgame->hand[i];
			}
		}
	}
	else {
		for (int i = 0; i < 13; i++) {
			if ((pgame->hand[i] != -1) && (pgame->hand[i] % 4 == pgame->historyCard[pgame->turn * 4] % 4)) {
				r_chars[num++] = pgame->hand[i];
			}
		}
		if (num == 0) {//没有领出花色则任出一张
			for (int i = 0; i < 13; i++) {
				if (pgame->hand[i] != -1) {
					r_chars[num++] = pgame->hand[i];
				}
			}
		}
	}
	return num;
}

int GetDUseableCard(Game* pgame, char *r_chars)
{
	int num = 0;
	if (pgame->num == 0) {//领出
		for (int i = 0; i < 13; i++) {
			if (pgame->dHand[i] != -1) {
				r_chars[num++] = pgame->dHand[i];
			}
		}
	}
	else {
		for (int i = 0; i < 13; i++) {
			if ((pgame->dHand[i] != -1) && (pgame->dHand[i] % 4 == pgame->historyCard[pgame->turn * 4] % 4)) {
				r_chars[num++] = pgame->dHand[i];
			}
		}
		if (num == 0) {//没有领出花色则任出一张
			for (int i = 0; i < 13; i++) {
				if (pgame->dHand[i] != -1) {
					r_chars[num++] = pgame->dHand[i];
				}
			}
		}
	}
	return num;
}


//以下为叫牌部分函数
/**
/* GetSuit整理手牌，给均牌型和长套牌标识，按花色分别计数保存手牌便于计算牌点
/* UpdateCardPoint计算牌点，分为
*/
void GetBid(Game* pgame, char bid[80])
{
	//char *bid=new char[80];
	if (!pgame->isTidy)//如果没有整理套牌则整理
		GetSuit(pgame);
	UpdateCardPoint(pgame);//根据当前叫牌更新牌点
	if (pgame->nowbidnum == 0)
		OpenBid(pgame, bid);//开叫
	else if (pgame->nowbidnum == 1)
	{
		if (pgame->nowBid[2] == pgame->pPosition)
			RespondBid(pgame, bid);//回应战友叫牌
		else
			OverCallBid(pgame, bid);//争叫
	}
	else
	{
		if (pgame->nowBid[2] == pgame->pPosition)
			AgainBid(pgame, bid);//再叫
		else
			OverCallBid(pgame, bid);//争叫
	}
	//bid="BID THIS";
	//return bid;
}

void GetSuit(Game* pgame)
{
	for (int i = 0; i<13; i++)
	{
		pgame->suit[pgame->hand[i] % 4].cards[pgame->suit[pgame->hand[i] % 4].cardnum++] = pgame->hand[i];
		if (pgame->hand[i] / 4 - 10>0)
			pgame->suit[pgame->hand[i] % 4].bigCardNum++;
	}
	int c = 0;
	int max = 0;
	for (int i = 0; i<4; i++)
	{
		if (pgame->suit[i].cardnum == 3)
			c++;
		if (pgame->suit[i].cardnum>max)
		{
			pgame->suit[i].isLong = true;
			max = pgame->suit[i].cardnum;
			for (int j = 0; j<i; j++)
				pgame->suit[j].isLong = false;
		}
		if (pgame->suit[i].cardnum == max)
			pgame->suit[i].isLong = true;
	}
	if (c == 3)
		pgame->isAverage = true;
	//先算大牌点,只算一次
	pgame->honorCardPoint = 0;
	int temp;
	for (int i = 0; i<13; i++)
	{
		temp = pgame->hand[i] / 4 - 8;
		if (temp>0)
		{
			pgame->honorCardPoint += temp;
		}
	}
	pgame->isTidy = true;
}

void UpdateCardPoint(Game* pgame)
{
	//没有开叫或叫无将的情况，牌力为大牌点加长套点
	//开叫有将，牌力为大牌点加短套点
	pgame->cardPoint = pgame->honorCardPoint;
	if (pgame->nowBid[0] == -1 || pgame->nowBid[1] == '5')
	{
		for (int i = 0; i<4; i++)
		{
			if (pgame->suit[i].isLong)
			{
				if (pgame->suit[i].cardnum == 5)
				{
					if (pgame->suit[i].bigCardNum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].bigCardNum>2)
						pgame->cardPoint += 2;
				}
				if (pgame->suit[i].cardnum>5)
				{
					if (pgame->suit[i].bigCardNum == 2)
						pgame->cardPoint += 2;
					if (pgame->suit[i].bigCardNum>2)
						pgame->cardPoint += 3;
				}
			}
		}
	}
	else
	{
		uchar color = pgame->nowBid[1] - '0' - 1;
		for (int i = 0; i<4; i++)
		{
			if (i == color)//不计算将牌
				continue;
			if (!pgame->suit[i].isLong)
			{
				if (pgame->suit[color].cardnum == 2)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 0;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 2;
				}
				if (pgame->suit[color].cardnum == 3)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 2;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 3;
				}
				if (pgame->suit[color].cardnum>3)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 3;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 5;
				}
			}
		}
	}
}

void OpenBid(Game* pgame, char r_bid[80])
{
	//没有匹配pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//23-24，均型牌（可能有5张低花套），2NT开叫敌方干扰后的叫牌与1NT相似  开叫2NT
	if (pgame->cardPoint >= 23 && pgame->cardPoint <= 24 && pgame->isAverage)
	{
		sprintf_s(r_bid, 80, "BID %c25", pgame->position);
	}
	//16+，单套结构，高花8赢墩、低花9赢墩以上，逼叫。下次叫5张以上套；
	//21+，逼叫。下次酌情叫2NT、3NT、平叫长套花色、跳叫单缺花色   开叫2C
	if ((pgame->cardPoint >= 16 && ((pgame->suit[2].cardnum>8 && pgame->suit[2].bigCardNum >= 3) ||
		(pgame->suit[3].cardnum>8 && pgame->suit[3].bigCardNum >= 3))) ||
		(pgame->cardPoint >= 16 && ((pgame->suit[0].cardnum>9 && pgame->suit[0].bigCardNum >= 3) ||
		(pgame->suit[1].cardnum>9 && pgame->suit[1].bigCardNum >= 3))))
	{
		sprintf_s(r_bid, 80, "BID %c21", pgame->position);
	}
	if (pgame->cardPoint>21)
	{
		sprintf_s(r_bid, 80, "BID %c21", pgame->position);
	}
	//12-20，5张以上套，6张套11点可以开叫1H1S
	if ((pgame->cardPoint >= 12 && pgame->cardPoint <= 20 && pgame->suit[2].cardnum == 5) ||
		(pgame->cardPoint >= 11 && pgame->suit[2].cardnum == 6))
	{
		//r_bid[5]='1';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c13", pgame->position);
	}
	if ((pgame->cardPoint >= 12 && pgame->cardPoint <= 20 && pgame->suit[3].cardnum == 5) ||
		(pgame->cardPoint >= 11 && pgame->suit[3].cardnum == 6))
	{
		//r_bid[5]='1';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c14", pgame->position);
	}
	//11～15， 典型的4S－5H高花双套开叫2D
	if (pgame->cardPoint >= 11 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum == 5 && pgame->suit[3].cardnum == 4)
	{
		//r_bid[5]='2';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c22", pgame->position);
	}
	//12～20，3张以上，均型牌时，12～15点下次叫1NT，19～20点叫2NT 开叫1C1D
	if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage&&pgame->suit[0].cardnum>3)
	{
		//r_bid[5]='1';
		//r_bid[6]='1';
		sprintf_s(r_bid, 80, "BID %c11", pgame->position);
	}
	if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage&&pgame->suit[1].cardnum>3)
	{
		//r_bid[5]='1';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c12", pgame->position);
	}
	//16～18，均型牌（可能有5张低花套）开叫1NT
	if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
	{
		//r_bid[5]='1';
		//r_bid[6]='5';
		sprintf_s(r_bid, 80, "BID %c15", pgame->position);
	}
	//6～10,好的6张高花套，没有另外的4张高花套，最多一个A  开叫2H2S
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[2].cardnum == 6 && pgame->suit[3].cardnum<4)
	{
		//r_bid[5]='2';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c23", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[3].cardnum == 6 && pgame->suit[2].cardnum<4)
	{
		//r_bid[5]='2';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c24", pgame->position);
	}
	//6～10，7张套，点力主要在该套中，没有另外的4张高花套，最多一个A 开叫3H3S3C3D
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[2].cardnum == 7 &&
		pgame->suit[3].cardnum<4 && pgame->suit[2].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c33", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[3].cardnum == 7 &&
		pgame->suit[2].cardnum<4 && pgame->suit[3].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c34", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[0].cardnum == 7 &&
		pgame->suit[1].cardnum<4 && pgame->suit[0].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='1';
		sprintf_s(r_bid, 80, "BID %c31", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[1].cardnum == 7 &&
		pgame->suit[0].cardnum<4 && pgame->suit[1].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c32", pgame->position);
	}
	//10～12，坚强7张低花套，冒险性3NT，边牌最大为Q，没有缺门 开叫3NT
	if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && (pgame->suit[0].cardnum == 7 || pgame->suit[1].cardnum == 7) &&
		pgame->suit[0].cardnum != 0 && pgame->suit[1].cardnum != 0 && pgame->suit[2].cardnum != 0 && pgame->suit[3].cardnum != 0)
	{
		//r_bid[5]='3';
		//r_bid[6]='5';
		sprintf_s(r_bid, 80, "BID %c35", pgame->position);
	}
	//11～15，坚固的8张套，或者坚固的7张套，边花有一个A或K 开叫4C4D
	//8～10，8张不坚固的套，点力主要在该套中，最多一个A 开叫4H4S

	if (r_bid[5] != '0')
		pgame->nowbidnum++;
	//else
	//sprintf_s(r_bid,80,"BID %c45",pgame->position);
}

void RespondBid(Game* pgame, char r_bid[80])
{
	//没有匹配pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//对1H1S的应叫
	if ((pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '3') || (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '4'))
	{
		//应叫1NT:6-10,对开叫人花色不支持的任意牌型
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && (pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum<3 || pgame->isAverage))
		{
			//r_bid[5]='1';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2H2S:6-10，平加，3张以上支持（加叫应包括牌型点）
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>3 && !pgame->isAverage)
		{
			//r_bid[5]='2';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c2%c", pgame->position, pgame->nowBid[1]);
		}
		//3H3S:11-12，跳加叫，4张或好的3张支持（例如Qxx）
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 &&
			pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>3 &&
			!pgame->isAverage&&
			pgame->suit[pgame->nowBid[1] - '0' - 1].bigCardNum>1)
		{
			//r_bid[5]='3';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c3%c", pgame->position, pgame->nowBid[1]);
		}
		//4H4S:≤10，4张以上支持，牌型分布好，大牌点不超过10点，封局止叫
		if (pgame->cardPoint <= 10 &&
			pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>4 &&
			!pgame->isAverage&&
			pgame->suit[pgame->nowBid[1] - '0' - 1].bigCardNum>2 &&
			pgame->honorCardPoint <= 10)
		{
			//r_bid[5]='4';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c4%c", pgame->position, pgame->nowBid[1]);
		}
		//双跳新花:11-15大牌点，显示单缺花色，有4张以上好支持（Qxxx），逼叫进局，有满贯兴趣。4-4-4-1是典型牌型
		if (pgame->honorCardPoint <= 15 && pgame->honorCardPoint >= 11 && pgame->suit[0].cardnum>6 &&
			pgame->suit[0].bigCardNum>1 &&
			(pgame->suit[1].cardnum == 1 || pgame->suit[2].cardnum == 1 || pgame->suit[3].cardnum == 1))
		{
			//r_bid[5]='3';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->honorCardPoint <= 15 && pgame->honorCardPoint >= 11 && pgame->suit[1].cardnum>6 &&
			pgame->suit[1].bigCardNum>1 &&
			(pgame->suit[0].cardnum == 1 || pgame->suit[2].cardnum == 1 || pgame->suit[3].cardnum == 1))
		{
			//r_bid[5]='3';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//2NT:13-15，均型牌，对开叫人的高花有3张以上支持
		if (pgame->cardPoint >= 13 && pgame->cardPoint <= 15 && pgame->isAverage)
		{
			//r_bid[5]='2';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:16-18，均型牌，可能有5张弱的低花套，对开叫人的高花有3张以上支持
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
		{
			//r_bid[5]='3';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//1S:6+，4张以上S，一盖一逼叫
		if (pgame->nowBid[1] == '3'&&pgame->cardPoint >= 6 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//2C2D2H:11+，5张高花或4张低花，二盖一逼叫
		//跳新花:16+，有满贯兴趣，有好的6张以上套，逼叫
		if (pgame->cardPoint >= 16 && pgame->suit[0].cardnum>6)
		{
			//r_bid[5]='2';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum>6)
		{
			//r_bid[5]='2';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
	}
	//对2D的应叫
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '2')
	{
		//2NT:13+,约定叫，问牌型
		if (pgame->cardPoint >= 13)
		{
			//r_bid[5]='2';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//2H2S:0-10，将牌至少7张配合
		if (pgame->cardPoint <= 10 && pgame->suit[2].cardnum >= 7)
		{
			//r_bid[5]='2';
			//r_bid[6]='3';
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint <= 10 && pgame->suit[3].cardnum >= 7)
		{
			//r_bid[5]='2';
			//r_bid[6]='4';
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//3H3S:11-12，将牌8+张配合，进局邀请
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 && pgame->suit[2].cardnum >= 8)
		{
			//r_bid[5]='3';
			//r_bid[6]='3';
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 && pgame->suit[3].cardnum >= 8)
		{
			//r_bid[5]='3';
			//r_bid[6]='4';
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//4H4S:≤12，将牌配合好，满贯无望，封局止叫

		//3C3D:10-12，高花无支持，6张以上坚强CD套，希望同伴有CD止张时叫3NT
		if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && pgame->suit[0].cardnum >= 6)
		{
			//r_bid[5]='3';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && pgame->suit[0].cardnum >= 6)
		{
			//r_bid[5]='3';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//3NT:10+,高花无支持，低花有好止张，止叫
		if (pgame->cardPoint >= 10 && (pgame->suit[0].bigCardNum >= 3 || pgame->suit[1].bigCardNum >= 3))
		{
			//r_bid[5]='3';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}

	}
	//对1C1D的应叫
	if ((pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '2'))
	{
		//应叫1D1H1S:6+，高花4张以上，1D为5张以上，一盖一逼叫
		if (pgame->nowBid[1] == '1'&&pgame->cardPoint>6 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c12", pgame->position);
		}
		if (pgame->cardPoint>6 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c13", pgame->position);
		}
		if (pgame->cardPoint>6 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//1NT:6-10,均型牌，无4张高花和5张低花，不逼叫
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->isAverage&&
			pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2C2D:11+,4张以上，否认有4张高花套，逼叫（首次加叫不能加牌型点）
		if (pgame->cardPoint>11 && pgame->suit[0].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[1].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
		//1D-2C:11+,4张以上C套，一般没有4+高花，逼叫
		//3C3D:6-10,5+的支持，阻击性，不逼叫
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//2NT:13-15,均型牌，无4+高花，一般未叫花色均有止张
		if (pgame->cardPoint >= 13 && pgame->cardPoint <= 15 && pgame->isAverage
			&&pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:16-18,均型牌，无4+高花，未叫花色均有止张
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage
			&&pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//跳新花:16+，有满贯兴趣，有好的5张(通常更长）套
	}
	//对1NT的应叫
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '5')
	{
		//应叫2C斯台曼,8+,
		//2D2H:转移叫，高花5+
		if (pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		//2S:弱低花转移叫，低花6+
		if (pgame->suit[0].cardnum>6 || pgame->suit[1].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//3C3D:邀请叫，低花6+半坚强套 
		//3H3S:9+,6张+，逼叫
		if (pgame->cardPoint>9 && pgame->suit[2].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>9 && pgame->suit[3].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//4C:盖伯问叫,问A,以后叫5C为问K 
		//2NT:7-8，无4张高花套，3NT邀请 
		if (pgame->cardPoint >= 7 && pgame->cardPoint <= 8
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:9-14，无4张高花套，封局止叫 
		if (pgame->cardPoint >= 9 && pgame->cardPoint <= 14
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT:15-16，定量加叫，小满贯试探 
		if (pgame->cardPoint >= 15 && pgame->cardPoint <= 16)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//5NT:19-20，定量加叫，大满贯试探 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20)
		{
			sprintf_s(r_bid, 80, "BID %c55", pgame->position);
		}
		//7NT:21+,打大满贯定约
	}
	//对2H2S的应叫
	if ((pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '3') || (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '4'))
	{
		//2NT，13点以上，一般支持开叫花色，问牌型，逼叫；
		if (pgame->cardPoint>13)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//新花色，5+，13点以上，有成局或满贯意图，寻求支持，逼叫；
		if (pgame->cardPoint>13 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
	}
	//对3C3D的应叫
	if ((pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '2'))
	{
		//应叫人加叫为阻击；3NT止叫；新花色，寻求支持，逼叫，开叫人有2张以上支持就应加叫，
		if ((pgame->nowBid[1] == '1'&&pgame->suit[0].cardnum<2) ||
			(pgame->nowBid[1] == '2'&&pgame->suit[1].cardnum<2))
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//若不支持应叫人的花色，又无单缺，可以叫3NT,
		//否则在低于4阶开叫花色水平叫出有大牌的新花色或叫回开叫花色；
		//如果应叫人牌力更强，应注意寻求满贯，在选择好将牌花色后，应叫人可以直接问A，也可以扣叫
	}
	//对3H3S的应叫
	if ((pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '2'))
	{
		//应叫人除叫4H修改3S外，其余叫新花均为逼叫，试探对该花色的支持。
		//开叫人有支持则加1阶，在高限时，还可以在比开叫花色低的4阶水平叫有大牌的花色。
		//开叫人没有支持叫回开叫花色。
		//如果开叫人叫3S寻求支持，开叫人支持不好时，没有单缺可以叫3NT,否则叫回原开叫花色。
		//开叫人再叫以后，应叫人再叫新花色为扣叫，显示有满贯兴趣
	}
	//对2NT的应叫
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '5')
	{
		//应叫3C斯台曼,4+,
		//3D3H:转移叫，高花5+
		if (pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		//3S:弱低花转移叫，低花6+
		//4C:盖伯问叫,问A,以后叫5C为问K 
		//3NT:3-8，无4张高花套，封局止叫 
		if (pgame->cardPoint >= 3 && pgame->cardPoint <= 8
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT:9，定量加叫，小满贯试探 
		if (pgame->cardPoint == 9)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//6NT:10-12，小满贯止叫
		//5NT:13，定量加叫，大满贯试探 
		if (pgame->cardPoint == 13)
		{
			sprintf_s(r_bid, 80, "BID %c55", pgame->position);
		}
		//7NT:14+,打大满贯定约
	}
	//对2C的应叫
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '1')
	{
		//应叫2D: 0-7大牌点,示弱，下次酌情叫牌。
		if (pgame->honorCardPoint<7)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
		//2H2S3C3D: 8+,示强，5张以上套
		if (pgame->cardPoint>8 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//2NT: 8-10大牌点,均型牌，无5张套
		if (pgame->honorCardPoint >= 8 && pgame->honorCardPoint <= 10 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3H3S4C4D: 11+,示强，5张以上好套
		if (pgame->cardPoint>11 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c41", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c42", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//3NT: 11+,均型牌，无5张套
		if (pgame->cardPoint>11 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
	}
	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}

void OverCallBid(Game* pgame, char r_bid[80])
{
	//暂时争叫只叫pass
	//没有匹配pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//1NT：16-18，敌花至少有一止张；处于第四家位置可使用12-15点的弱无将争叫
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1]<'5'&&pgame->cardPoint >= 16 && pgame->cardPoint <= 18)
	{
		sprintf_s(r_bid, 80, "BID %c15", pgame->position);
	}
	//2NT
	if (pgame->nowBid[0] <= '2'&&pgame->nowBid[1]<'5'&&pgame->honorCardPoint>16 && pgame->isAverage)
	{
		sprintf_s(r_bid, 80, "BID %c25", pgame->position);
	}
	if (pgame->nowBid[0]<'4')
	{
		//平叫新花色：11-15点，5+好套，最低水平地在1阶或2阶叫出，一盖一争叫，只要8点以上，套好即可。不逼叫。
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[1].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<1)
		{
			sprintf_s(r_bid, 80, "BID %c%c2", pgame->position, pgame->nowBid[0]);
		}
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<2)
		{
			sprintf_s(r_bid, 80, "BID %c%c3", pgame->position, pgame->nowBid[0]);
		}
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<3)
		{
			sprintf_s(r_bid, 80, "BID %c%c4", pgame->position, pgame->nowBid[0]);
		}
	}
	if (pgame->nowBid[0]<'3')
	{
		//跳叫新花色：有一手符合阻击开叫的牌型，但在成局水平的叫品，有可能具有接近成局所要求的赢墩数.
		if (pgame->cardPoint >= 16 && pgame->suit[0].cardnum >= 5 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c1", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum >= 5 && pgame->suit[1].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c2", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[2].cardnum >= 5 && pgame->suit[2].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c3", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum >= 5 && pgame->suit[1].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c4", pgame->position, pgame->nowBid[0] + 1);
		}
	}

	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}

void AgainBid(Game* pgame, char r_bid[80])
{
	//暂时再叫只叫pass
	//没有匹配pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);

	//对1S 的再叫
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '4')
	{
		//1NT:15-,其他所有情况
		if (pgame->cardPoint<15)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2S:15-,示弱，4张以上S 
		if (pgame->cardPoint<15 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//2H:15-,6张以上H
		if (pgame->cardPoint<15 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		//2C2D:15-或16-17，4张以上CD好套，逼叫
		if (pgame->cardPoint<17 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint<17 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}

		//3S:16+,示强,4张以上S,邀局,应叫方9点以上进局
		if (pgame->cardPoint>16 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//3H:16+，6张以上H，邀局，应叫方9点以上进局
		if (pgame->cardPoint>16 && pgame->suit[2].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		//3C3D:18+,4 张以上 CD 好套，逼叫进局 
		if (pgame->cardPoint>18 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>18 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//4C/4D:18+,S配合,C/D单缺,逼叫进局,有望进贯
		//2NT:16-18，均型牌
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//4H/4S：19-20,封局,止叫,无其他牌型需要显示 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20 && pgame->suit[2].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c43", pgame->position);
		}
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c44", pgame->position);
		}
	}
	//对1D的再叫
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '2')
	{
		//再叫2D2H2S:12-15,对应叫方应叫花色有3张D或4张HS 
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum>3)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum>3)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//1H1S:12-15,对应叫方的1D1H，有4张以上的HS
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c13", pgame->position);
		}
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//2C:12-15,5张以上C
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		//1NT:12-15,均型牌无套
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//3H3S：16+,对应叫方的1H1S有4张支持，邀局
		//3C3D:5 张套，邀叫
		//2NT:19-20 点，逼叫进局 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
	}
	//对2D的再叫
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '2')
	{
		//再叫2NT：21-22，均型牌，允许有5张套
		if (pgame->cardPoint >= 21 && pgame->cardPoint <= 22 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT：25-27，均型牌
		if (pgame->cardPoint >= 21 && pgame->cardPoint <= 22 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//平叫花色：21/16＋，21点以上或16点以上高花8、低花9赢墩以上，非均型牌、5张以上好套，逼叫
		//跳新花：21＋，4－4－4－1牌型，跳叫单缺花色
	}
	//对2NT的再叫
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '5')
	{
		//3NT：21，均型牌
		if (pgame->cardPoint>18)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT：22，定量加叫，小满贯试探
		if (pgame->cardPoint>21)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//5NT：25-26，小满贯止叫或叫6阶花色寻求4-4配合
	}
	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}


