// tree.cpp: 主项目文件。

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "CardsCategory.h"
//#define MAX_CARD_ONE_HAND 20
#define MAX_STACK_SIZE (1024 * 128)
int check(char a[]);
extern int array[MAX_CARD_ONE_HAND];
/*
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
};
*/
enum hand_state
{
	STATE_VALID = 1,
	STATE_PENDING
};
/*
struct card_hand
{
	int type;
	int state;
	int card_num;
	int card[MAX_CARD_ONE_HAND];
	struct card_hand *next;
};
*/

int output_card(int *p)                          //读取手牌并存到文件
{
	FILE *fp;

	if((fp=fopen("d:\\myfile.txt","w"))==NULL)
	{
		printf("cannot open this file\n");
		exit(0);
	}
	
	 while(*p!=-1)
	 {
		 
		 /*if((*p/4+3)==15)
			 fprintf(fp,"%d ",2);
		 else if((*p)==52)
			 fprintf(fp,"%d ",15);
		 else if((*p)==53)
			 fprintf(fp,"%d ",16);
		 else*/
			fprintf(fp,"%d ",*p/4+3);
		 p++;
	 }
	 fprintf(fp,"\\n,");

		fclose(fp);

		FILE *f = fopen("D:\\myfile.txt", "r");     //读取手牌
	if (!f) {
		/*printf("open myfile.txt failed!\n");
		goto err;*/
		exit(0);
	}
	int num = 1;
	int flag=0;
	fscanf(f, "%d", &array[0]);                       //将手牌打印
	while (fscanf(f, " %d", &array[num]) > 0) {
		num++;
	}
	fclose(f);
	return num;
}
struct choice_node
{
	struct choice_node *father;
	struct choice_node *bro_pre;
	struct choice_node *son;
	struct choice_node *bro_next;
	struct choice_node *list;
	int index;
	int hand_num;
	int state;
	struct card_hand *hand;
	struct card_hand *last_hand;
	struct card_hand *chain_hand;

	struct card_hand *single_set;
	struct card_hand *double_set;
	struct card_hand *three_set;
	struct card_hand *four_set;
	struct card_hand *chain_set;
	struct card_hand *rocket;
};


#define MEM() 	fprintf(stderr,"low memory, while %s %d\n", __FILE__, __LINE__)
#define OVF()	fprintf(stderr,"stack or queue overflow, [%d][%d] while %s %d\n", head, tail, __FILE__, __LINE__)

int compare_card(const void *j, const void *k)
{
	int a = *((const int *)j);
	int b = *((const int *)k);
	return (a > b) ? 1 : ((a == b) ? 0 : -1);
}

void sort_card(int *array, int num)
{
	qsort(array, num, sizeof(int), compare_card);
}

int node_count;
int hand_count;

struct choice_node *level1_node_pool;
struct card_hand *level1_hand_pool;

//-------------------------mem pool---------------------------

void create_level1_pool(int node_num, int hand_num)
{
	struct choice_node *n;
	struct card_hand *h;

	for (int i = 0; i < node_num; i++) {
		if (!(n = (struct choice_node *)malloc(sizeof(*n)))) MEM();
		n->list = level1_node_pool;
		level1_node_pool = n;
	}
	for (int i = 0; i < hand_num; i++) {
		if (!(h = (struct card_hand *)malloc(sizeof(*h)))) MEM();
		h->next = level1_hand_pool;
		level1_hand_pool = h;
	}
}

void destroy_level1_pool()
{
	struct choice_node *n;
	struct card_hand *h;

	while (level1_node_pool) {
		n = level1_node_pool;
		level1_node_pool = n->list;
		free(n);
	}
	while (level1_hand_pool) {
		h = level1_hand_pool;
		level1_hand_pool = h->next;
		free(h);
	}
}

struct choice_node *level1_alloc_node()
{
	struct choice_node *n;
	n = level1_node_pool;
	level1_node_pool = n->list;
	return n;
}

struct card_hand *level1_alloc_hand()
{
	struct card_hand *h;
	h = level1_hand_pool;
	level1_hand_pool = h->next;
	return h;
}

void level1_free_node(struct choice_node *n)
{
	n->list = level1_node_pool;
	level1_node_pool = n;
}

void level1_free_hand(struct card_hand *h)
{
	h->next = level1_hand_pool;
	level1_hand_pool = h;
}







struct card_hand *get_card_hand()
{
	struct card_hand *h = level1_alloc_hand();
	if (!h) MEM();
	h->card_num = 0;
	h->next = NULL;
	h->type = 0;
	h->state = 0;
	return h;
}

int card_hand_compare(struct card_hand *h1, struct card_hand *h2)
{
	int i = 0;
	if (h1->type != h2->type) return -1;
	if (h1->state != h2->state) return -1;
	if (h1->card_num != h2->card_num) return -1;
	for (; i < h1->card_num; i++) {
		if (h1->card[i] != h2->card[i])
			return -1;
	}
	return 0;
}

void erase_card_hand(struct card_hand *h)
{
	level1_free_hand(h);
}

struct card_hand *copy_card_hand(struct card_hand *ori)
{
	struct card_hand *h = get_card_hand();
	h->card_num = ori->card_num;
	h->type = ori->type;
	h->state = ori->state;
	memcpy(h->card, ori->card, sizeof(int) * h->card_num);
	return h;
}

struct choice_node *get_choice_node()
{
	struct choice_node *n = level1_alloc_node();
	if (!n) MEM();
	n->father = NULL;
	n->bro_pre = NULL;
	n->bro_next = NULL;
	n->son = NULL;
	n->list = NULL;
	n->hand_num = 0;
	n->hand = NULL;
	n->last_hand = NULL;
	n->chain_hand = NULL;
	n->index = 0;
	n->state = 0;

	n->single_set = NULL;
	n->double_set = NULL;
	n->three_set = NULL;
	n->four_set = NULL;
	n->chain_set = NULL;
	n->rocket = NULL;
	return n;
}

void card_hand_print(FILE *f, struct card_hand *h)
{
	fprintf(f, "[");
	for (int i = 0; i < h->card_num; i++) {
		if (i != h->card_num - 1) fprintf(f, "%d,", h->card[i]);
		else fprintf(f, "%d", h->card[i]);
	}
	fprintf(f, "] ");
}

void choice_node_print(FILE *f, struct choice_node *n)
{
	struct card_hand *h = n->hand;
	while (h) {
		card_hand_print(f, h);
		h = h->next;
	}
}

int choice_node_compare(struct choice_node *n1, struct choice_node *n2)
{
	struct card_hand *h1, *h2;
	if (n1->index != n2->index) return -1;
	if (n1->hand_num != n2->hand_num) return -1;
	h1 = n1->hand;
	h2 = n2->hand;
	if (!h1 || !h2) return -1;
	while (h1 && h2) {
		if (card_hand_compare(h1, h2)) {
			return -1;
		}
		h1 = h1->next;
		h2 = h2->next;
	}
	return 0;
}

void choice_node_append_hand(struct choice_node *root, struct card_hand *hand)
{
	if (!root->hand) {
		root->hand = hand;
		root->last_hand = hand;
		root->chain_hand = hand;
		return;
	}
	root->last_hand->next  = hand;
	root->last_hand = hand;
}

struct choice_node *spawn_choice_node(struct choice_node *root)
{
	struct card_hand *i, *j;
	struct choice_node *n = get_choice_node();
	n->father = root;
	n->hand_num = root->hand_num;
	n->index = root->index + 1;
	i = root->hand;
	while (i) {
		j = copy_card_hand(i);
		choice_node_append_hand(n, j);
		i = i->next;
	}
	return n;
}

struct choice_node *spawn_choice_node_ex(struct choice_node *root, 
	struct card_hand *oh, struct card_hand **nh)
{
	struct card_hand *i, *j;
	struct choice_node *n = get_choice_node();
	n->father = root;
	n->hand_num = root->hand_num;
	n->index = root->index + 1;
	i = root->hand;
	while (i) {
		j = copy_card_hand(i);
		if (oh == i) {
			*nh = j;
		}
		choice_node_append_hand(n, j);
		i = i->next;
	}
	return n;
}

void erase_choice_node(struct choice_node *root)
{
	struct card_hand *i, *j;
	i = root->hand;

	while (i) {
		j = i->next;
		erase_card_hand(i);
		i = j;
	}
	level1_free_node(root);
}

void choice_node_add_son(struct choice_node *root, struct choice_node *son)
{/*
	struct choice_node *bro = root->son;
	if (!bro) {
		root->son = son;
		return;
	}
	while (bro->bro_next) {
		bro = bro->bro_next;
	}
	bro->bro_next = son;
	son->bro_pre = bro;*/
}

int use_stack_or_queue = 1; //0 stack/1 queue
struct choice_node *stack_or_queue[MAX_STACK_SIZE];
int head = -1;
int tail = -1;

struct choice_node *choice_level_table[MAX_CARD_ONE_HAND];

void push(struct choice_node *node)
{
	if (!use_stack_or_queue) {
		if (tail == MAX_STACK_SIZE - 1) OVF();
		tail++;
		stack_or_queue[tail] = node;
	} else {
		if (tail == MAX_STACK_SIZE - 1) {
			tail++;
			tail %= MAX_STACK_SIZE;
		} else {
			tail++;
		}
		if (head == -1) {
			head++;
		} else if (tail == head) {
			OVF();
		}
		stack_or_queue[tail] = node;
	}
}

struct choice_node *pop()
{
	int tmp;
	if (!use_stack_or_queue) {
		if (tail == -1) return NULL;
		tmp = tail--;
		return stack_or_queue[tmp];
	} else {
		if (head == -1) return NULL;
		if (head == tail) {
			tmp = head;
			head = tail = -1;
		} else if (head == MAX_STACK_SIZE - 1) {
			tmp = head++;
			head %= MAX_STACK_SIZE;
		} else {
			tmp = head++;
		}
		return stack_or_queue[tmp];
	}
}

struct choice_node *get()
{
	if (!use_stack_or_queue) {
		if (tail == -1) return NULL;
		return stack_or_queue[tail];
	} else {
		if (head == -1) return NULL;
		return stack_or_queue[head];
	}
}

void create_stack_or_queue()
{
}

void erase_stack_or_queue()
{
	head = tail = -1;
}

void clean_choice_level_table()
{
	struct choice_node *n;
	struct choice_node *tmp;
	int i = 0;
	for (; i < MAX_CARD_ONE_HAND; i++) {
		n = choice_level_table[i];
		while (n) {
			tmp = n->list;
			erase_choice_node(n);
			n = tmp;
		}
		choice_level_table[i] = NULL;
	}
}

struct choice_node *choice_node_find(struct choice_node *n)
{
	struct choice_node *list = choice_level_table[n->index];
	while (list) {
		if (!choice_node_compare(list, n)) return list;
		list = list->list;
	}
	return NULL;
}

void choice_node_list(struct choice_node *n)
{
	struct choice_node *list = choice_level_table[n->index];
	
	if (!list) {
		choice_level_table[n->index] = n;
		return;
	}
	n->list = choice_level_table[n->index];
	choice_level_table[n->index] = n;

}

struct choice_node *find_single(struct choice_node *root, int card)
{
	struct card_hand *h = get_card_hand();
	struct choice_node *n = spawn_choice_node(root);

	h->type = HAND_SINGLE;
	h->state = STATE_VALID;
	h->card_num = 1;
	h->card[0] = card;

	choice_node_append_hand(n, h);
	n->hand_num++;

	if (choice_node_find(n)) {
		erase_choice_node(n);
		return NULL;
	}
	choice_node_list(n);
	choice_node_add_son(root, n);
	return n;
}

struct choice_node *find_double(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	struct card_hand *nh;
	struct choice_node *n;

	while (h) {
		if (h->type == HAND_SINGLE) {
			if (h->card[0] == card) {
				n = spawn_choice_node_ex(root, h, &nh);
				nh->type = HAND_DOUBLE;
				nh->card_num = 2;
				nh->card[1] = card;

				if (choice_node_find(n)) {
					erase_choice_node(n);
					return NULL;
				}
				choice_node_list(n);
				choice_node_add_son(root, n);
				return n;
			}
		}
		h = h->next;
	}
	return NULL;
}

struct choice_node *find_three(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	struct card_hand *nh;
	struct choice_node *n;

	while (h) {
		if (h->type == HAND_DOUBLE) {
			if (h->card[0] == card) {
				n = spawn_choice_node_ex(root, h, &nh);
				nh->type = HAND_THREE;
				nh->card_num = 3;
				nh->card[2] = card;

				if (choice_node_find(n)) {
					erase_choice_node(n);
					return NULL;
				}
				choice_node_list(n);
				choice_node_add_son(root, n);
				return n;
			}
		}
		h = h->next;
	}
	return NULL;
}

struct choice_node *find_four(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	struct card_hand *nh;
	struct choice_node *n;

	while (h) {
		if (h->type == HAND_THREE) {
			if (h->card[0] == card) {
				n = spawn_choice_node_ex(root, h, &nh);
				nh->type = HAND_FOUR;
				nh->card_num = 4;
				nh->card[3] = card;

				if (choice_node_find(n)) {
					erase_choice_node(n);
					return NULL;
				}
				choice_node_list(n);
				choice_node_add_son(root, n);
				return n;
			}
		}
		h = h->next;
	}
	return NULL;
}

struct choice_node *find_chain(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	struct card_hand *nh;
	struct choice_node *n;
	int tmp;

	h = root->chain_hand;
	while (h) {
		if (h->type == HAND_SINGLE || h->type == HAND_CHAIN) {
			tmp = h->card[h->card_num - 1];
			if (tmp >= 3 && tmp < 14 && tmp == card - 1) {
				n = spawn_choice_node_ex(root, h, &nh);
				nh->type = HAND_CHAIN;
				nh->card_num++;
				if (nh->card_num < 5) {
					nh->state = STATE_PENDING;
				} else {
					nh->state = STATE_VALID;
				}
				nh->card[nh->card_num - 1] = card;

				while (h->next && !card_hand_compare(h, h->next)) {
					h = h->next;
				}
				root->chain_hand = h->next;

				if (choice_node_find(n)) {
					erase_choice_node(n);
				} else {
					choice_node_list(n);
					choice_node_add_son(root, n);
					return n;
				}
			}
		}
		h = h->next;
	}
	root->chain_hand = NULL;
	return NULL;
}

struct choice_node *find_rocket(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	struct card_hand *nh;
	struct choice_node *n;

	if (card != 17) return NULL;

	while (h) {
		if (h->type == HAND_SINGLE) {
			if (h->card[0] == 16 && card == 17) {
				n = spawn_choice_node_ex(root, h, &nh);
				nh->type = HAND_ROCKET;
				nh->card_num = 2;
				nh->card[1] = card;

				if (choice_node_find(n)) {
					erase_choice_node(n);
					return NULL;
				}
				choice_node_list(n);
				choice_node_add_son(root, n);
				return n;
			}
		}
		h = h->next;
	}
	return NULL;
}

int validate_ex(struct choice_node *root, int card)
{
	struct card_hand *h = root->hand;
	while (h) {
		if (h->type == HAND_CHAIN && h->state == STATE_PENDING &&
			h->card[h->card_num - 1] < card - 1) {
			return -1;
		}
		h = h->next;
	}
	return 0;
}

int validate(struct choice_node *root)
{
	struct card_hand *h = root->hand;
	while (h) {
		if (h->type == HAND_CHAIN && h->state == STATE_PENDING) {
			return -1;
		}
		h = h->next;
	}
	return 0;
}

void category(struct choice_node *root, int array[], int num)
{
	struct choice_node *node;
	int card;

	if (root->index >= num) {
		pop();
		return;
	}
	card = array[root->index];
	if (validate_ex(root, card)) {
		pop();
		return;
	}

	if (!use_stack_or_queue) {
		switch (root->state) {
		case 0:
			if ((node = find_single(root, card))) {
				root->state = HAND_SINGLE;
				push(node);
				break;
			}
		case HAND_SINGLE:
			if ((node = find_double(root, card))) {
				root->state = HAND_DOUBLE;
				push(node);
				break;
			}
		case HAND_DOUBLE:
			if ((node = find_three(root, card))) {
				root->state = HAND_THREE;
				push(node);
				break;
			}
		case HAND_THREE:
			if ((node = find_four(root, card))) {
				root->state = HAND_FOUR;
				push(node);
				break;
			}
		case HAND_FOUR:
			if ((node = find_chain(root, card))) {
				push(node);
				break;
			} else {
				root->state = HAND_CHAIN;
			}
		case HAND_CHAIN:
			if ((node = find_rocket(root, card))) {
				push(node);
			}
			root->state = HAND_ROCKET;
		default:
			break;
		}
		if (root->state == HAND_ROCKET) {
			pop();
		}
	} else {
		if ((node = find_single(root, card))) {
			push(node);
		}
		if ((node = find_double(root, card))) {
			push(node);
		}
		if ((node = find_three(root, card))) {
			push(node);
		}
		if ((node = find_four(root, card))) {
			push(node);
		}
		while ((node = find_chain(root, card))) {
			push(node);
		}
		if ((node = find_rocket(root, card))) {
			push(node);
		}
		pop();
	}
}

struct choice_node *gen_category(int array[], int num)
{
	struct choice_node *root, *list, *tmp1, *tmp2;

	if (num <= 0 || num > MAX_CARD_ONE_HAND) return NULL;

	create_stack_or_queue();

	root = get_choice_node();

	choice_node_list(root);

	push(root);

	while ((root = get())) {
		category(root, array, num);
	}

	erase_stack_or_queue();

	list = NULL;
	tmp1 = choice_level_table[num];
	while (tmp1) {
		tmp2 = tmp1->list;
		if (!validate(tmp1)) {
			tmp1->list = list;
			list = tmp1;
		} else {
			erase_choice_node(tmp1);
		}
		tmp1 = tmp2;
	}

//	list = choice_level_table[num];
	choice_level_table[num] = NULL;
	clean_choice_level_table();

	return list;
}

void sort_category(struct choice_node *list)
{
	struct choice_node *n = NULL;
	while (list) {
		list->list = n;
	}
}

void init_category()
{
	create_level1_pool(20000, 200000);
}

void fini_category()
{
	destroy_level1_pool();
}

char *usage = 
	"\n"\
	"程序名称：《斗地主AI辅助组件》\n"\
	"程序版本：1.0\n"\
	"程序功能：根据现有的牌（17～20张不等），穷举出所有合法的手牌组合\n"\
	"程序使用：程序默认读取当前目录下myfile.txt文件，以获取所有的牌\n"\
	"          文件格式为xx xx xx xx xx ...\n"\
	"          例如2 3 4 4 8 12 13 13 14 4 5 6 7 7 7 7 15\n"\
	"          3～10的牌默认为数字3～10\n"\
	"          J～2的牌默认为数字11～14\n"\
	"          大王小王为数字15和16\n"\
	"          程序运行结果最后会输出到result.txt文件中\n"\
	"\n"\
;


//int main(int argc, char *argv[])
//{	
//	int array[MAX_CARD_ONE_HAND];
//	FILE *f;
//	int num, count, start;
//	struct choice_node *root;
//	int j;
//
//	/*printf("%s", usage);*/
//	system("pause");
//	f = fopen("myfile.txt", "r");
//	//if (!f) {
//	//	/*printf("open myfile.txt failed!\n");
//	//	goto err;*/
//	//}
//
//	num = 1;
//	fscanf(f, "%d", &array[0]);
//	while (fscanf(f, " %d", &array[num]) > 0) {
//		num++;
//	}
//
//	//printf("\n经排序后牌如下：\n");
//	sort_card(array, num);
//	/*for (j = 0; j < num; j++) {
//		printf("%d ", array[j]);
//	}
//	printf("\n");*/
//
//	init_category();
//	count = 0;
//	use_stack_or_queue = 1;
//	start = time(0);
//	root = gen_category(array, num);
//	//printf("\n扫描结果如下：\n");
//	while (root) {
//		choice_node_print(stdout, root);
//		//printf("\n");
//		root = root->list;
//		count++;
//	}
//	//printf("\n共有%d种结果，用时%d秒\n\n", count, time(0) - start);
//
//	fini_category();
////err:
////	system("pause");
//    return 0;
//}


//返回手中牌所有出法集合
struct Category_Info* calHandCount(int *array, int num)
{
	return NULL;
}

//返回以最少手出完为依据的手牌分类
struct Category_Info* calminHandCount(int *array, int num)
{
	return NULL;
}

//返回最少多少手能出完
int minHandCount(int *array, int num)
{
	int count=0,min=num;
	struct choice_node *root;

	sort_card(array, num);
	init_category();
	check("最少多少手能出完程序");
	root = gen_category(array, num);	
	while (root) {
		struct card_hand *h = root->hand;
		count=0;
		while (h) {
			count++;
			h = h->next;
		}
		root = root->list;
		if(min>count) min=count;
	}
	
	fini_category();
	return min;
}