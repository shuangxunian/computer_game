#ifndef CARDSCATEGORY_H
#define CARDSCATEGORY_H

#include <stdio.h>

#define MAX_CARD_ONE_HAND 20

enum hand_type
{
	HAND_SINGLE = 1,
	
	HAND_DOUBLE,
	HAND_THREE,
	HAND_FOUR,
	HAND_CHAIN,
	HAND_ROCKET,

	HAND_CHAIN_DOUBLE,
	HAND_CHAIN_THREE,
	HAND_CHAIN_THREE_SINGLE,
	HAND_CHAIN_THREE_DOUBLE,

	HAND_THREE_SINGLE,
	HAND_THREE_DOUBLE,

	HAND_FOUR_SINGLE_SINGLE,
	HAND_FOUR_DOUBLE_DOUBLE,
	HAND_TYPE_MAX
};

struct card_hand
{
	int type;
	int state;
	int card_num;
	int card[MAX_CARD_ONE_HAND];//enum hand_type 为下标
	struct card_hand *next;
};

struct Category_Info
{
	int HandCount;
	struct card_hand cards_type[HAND_TYPE_MAX];
	struct Category_Info *next;
};

//返回手中牌所有出法集合
struct Category_Info* calHandCount(int *array, int num);

//返回以最少手出完为依据的手牌分类
struct Category_Info* calminHandCount(int *array, int num);

//返回最少多少手能出完
int minHandCount(int *array, int num);
#endif