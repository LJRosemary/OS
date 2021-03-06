#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
//内存容量
#define MAX_MEM_SIZE 100
typedef enum {error, ok}status;
typedef char Elemtype;//可自定义内存数据类型
//模拟内存
Elemtype M[MAX_MEM_SIZE];//可根据索引访问内存，本例未对内存进行真实处理，仅处理的索引位
//进程链
typedef struct PNode {
	int ID;//进程ID
	int head_index;//占用内存首地址
	int size;//占用内存大小
	PNode *next;
}*P_list;
//空闲分区链
typedef struct EmNode {
	int head_index;
	int size;
	EmNode *next;
}*Em_list;

status check_Em(Em_list &E);//将未合并的块合并
status check_Em1(Em_list &E);//将空闲块从小到大排
status check_Em2(Em_list &E);//将空闲块从大到小排
status init(P_list &P, Em_list &E);//初始化(将空闲内存链入空闲链)
status Request_FF(P_list &P, Em_list &E, int ID, int n);//FF
status Request_BF(P_list &P, Em_list &E, int ID, int n);//BF
status Request_WF(P_list &P, Em_list &E, int ID, int n);//WF
status Free_FF(P_list &P, Em_list &E, int ID);//FF
status Free_BF(P_list &P, Em_list &E, int ID);//BF
status Free_WF(P_list &P, Em_list &E, int ID);//WF
status visit_P(PNode p);
status travel_P(P_list P, status visit(PNode));
status visit_E(EmNode e);
status travel_E(Em_list E, status visit(EmNode));
status swap(Em_list &E, EmNode *a, EmNode *b);
void show_info(P_list P, Em_list E);
void menu_FF(P_list &P, Em_list &E);
void menu_BF(P_list &P, Em_list &E);
void menu_WF(P_list &P, Em_list &E);

//进城链和空闲分区链初始化、进程链为空，空闲分区链标记整个分区
status init(P_list &P, Em_list &E) {
	P = (PNode *)malloc(sizeof(PNode));
	E = (EmNode*)malloc(sizeof(EmNode));
	if (!P && !E)
		return error;
	P->next = NULL;
	E->next = NULL;
	EmNode *p;
	p = (EmNode *)malloc(sizeof(EmNode));
	p->head_index = 0;
	p->size = MAX_MEM_SIZE;
	p->next = E->next;
	E->next = p;
	return ok;
}
//进程申请内存(FF)
status Request_FF(P_list &P, Em_list &E, int ID, int n) {
	PNode *p;
	p = (PNode *)malloc(sizeof(PNode));
	p->ID = ID;
	p->size = n;
	p->head_index = -1;
	EmNode *e, *temp;
	e = E->next;
	temp = E;
	while (e != NULL) {
		if (e->size > n) {
			//printf("11\n");
			p->head_index = e->head_index;
			e->size = e->size - n;
			e->head_index = e->head_index + n;
			//printf("%d, %d, %d\n", p->head_index, p->ID, p->size);
			break;
		}
		else if (e->size == n) {
			p->head_index = e->head_index;
			temp->next = e->next;
			free(e);
			break;
		}
		else {
			e = e->next;
			temp = temp->next;
		}
	}
	if (p->head_index != -1) {
		p->next = P->next;
		//printf("22\n");
		P->next = p;
		//check_Em1(E);
		return ok;
	}
	else {
		free(p);
		return error;
	}
}
//进程申请内存(BF)
status Request_BF(P_list &P, Em_list &E, int ID, int n) {
	PNode *p;
	p = (PNode *)malloc(sizeof(PNode));
	p->ID = ID;
	p->size = n;
	p->head_index = -1;
	EmNode *e, *temp;
	e = E->next;
	temp = E;
	while (e != NULL) {
		if (e->size > n) {
			//printf("11\n");
			p->head_index = e->head_index;
			e->size = e->size - n;
			e->head_index = e->head_index + n;
			//printf("%d, %d, %d\n", p->head_index, p->ID, p->size);
			break;
		}
		else if (e->size == n) {
			p->head_index = e->head_index;
			temp->next = e->next;
			free(e);
			break;
		}
		else {
			e = e->next;
			temp = temp->next;
		}
	}
	if (p->head_index != -1) {
		p->next = P->next;
		//printf("22\n");
		P->next = p;
		check_Em1(E);
		return ok;
	}
	else {
		free(p);
		return error;
	}
}
//进程申请内存(WF)
status Request_WF(P_list &P, Em_list &E, int ID, int n) {
	PNode *p;
	p = (PNode *)malloc(sizeof(PNode));
	p->ID = ID;
	p->size = n;
	p->head_index = -1;
	EmNode *e, *temp;
	e = E->next;
	temp = E;
	while (e != NULL) {
		if (e->size > n) {
			//printf("11\n");
			p->head_index = e->head_index;
			e->size = e->size - n;
			e->head_index = e->head_index + n;
			//printf("%d, %d, %d\n", p->head_index, p->ID, p->size);
			break;
		}
		else if (e->size == n) {
			p->head_index = e->head_index;
			temp->next = e->next;
			free(e);
			break;
		}
		else {
			e = e->next;
			temp = temp->next;
		}
	}
	if (p->head_index != -1) {
		p->next = P->next;
		//printf("22\n");
		P->next = p;
		check_Em2(E);
		return ok;
	}
	else {
		free(p);
		return error;
	}
}
//进程释放内存(FF)
status Free_FF(P_list &P, Em_list &E, int ID) {
	int head_index;
	int size;
	PNode *temp, *temp1;
	temp = P->next;
	temp1 = P;
	while (temp != NULL) {
		if (temp->ID == ID)
			break;
		temp = temp->next;
		temp1 = temp1->next;
	}
	if (temp->ID != ID)
		return error;
	head_index = temp->head_index;
	size = temp->size;
	temp1->next = temp->next;
	free(temp);
	EmNode *e;
	e = (EmNode *)malloc(sizeof(EmNode));
	e->head_index = head_index;
	e->size = size;
	EmNode *test;
	EmNode *pre, *rear;
	pre = rear = E->next;
	test = E->next;
	int flag = -1;
	if (test == NULL)
		flag = 2;
	while (test != NULL) {
		if (test->head_index == e->head_index + e->size) {//释放的内存与该空闲分区的前面相连
			if (flag != 3) {
				test->head_index = e->head_index;
				test->size = test->size + e->size;
				flag = 3;
			}
			//e->head_index = test->head_index;
			//e->size = test->size;
			test = test->next;
			//printf("%d\n", flag);
		}
		else if (test->head_index + test->size == e->head_index) {//释放的内存与该空闲分区的后面相连
			if (flag != 3) {
				//printf("%d\n", flag);
				test->size = test->size + e->size;
				flag = 3;
			}
			//e = test;
			test = test->next;
		}
		else if (e->head_index < test->head_index) {
			if (flag == -1) {
				flag = 0;
				pre = test;
			}	
			test = test->next;
		}
		else if (e->head_index > test->head_index) {
			if (flag == -1) {
				flag = 1;
				rear = test;
			}
			test = test->next;
		}
		else {
			printf("error!\n");
			test = test->next;
		}
	}
	if (flag == 1) {//后
		e->next = rear->next;
		rear->next = e;
	}
	else if (flag == 0) {//前
		EmNode *test1;
		test1 = E;
		while (test1->next != pre)
			test1 = test1->next;
		e->next = test1->next;
		test1->next = e;
	}
	else if (flag == 2) {
		e->next = E->next;
		E->next = e;
	}
	else
		free(e);
	check_Em(E);
	return ok;
}
//进程释放内存（BF）
status Free_BF(P_list &P, Em_list &E, int ID) {
	int head_index;
	int size;
	PNode *temp, *temp1;
	temp = P->next;
	temp1 = P;
	while (temp != NULL) {
		if (temp->ID == ID)
			break;
		temp = temp->next;
		temp1 = temp1->next;
	}
	if (temp->ID != ID)
		return error;
	head_index = temp->head_index;
	size = temp->size;
	temp1->next = temp->next;
	free(temp);
	EmNode *e;
	e = (EmNode *)malloc(sizeof(EmNode));
	e->head_index = head_index;
	e->size = size;
	EmNode *test;
	EmNode *pre, *rear;
	pre = rear = E->next;
	test = E->next;
	int flag = -1;
	if (test == NULL)
		flag = 2;
	while (test != NULL) {
		if (test->head_index == e->head_index + e->size) {//释放的内存与该空闲分区的前面相连
			if (flag != 3) {
				test->head_index = e->head_index;
				test->size = test->size + e->size;
				flag = 3;
			}

			//e->head_index = test->head_index;
			//e->size = test->size;
			test = test->next;
			//printf("%d\n", flag);
		}
		else if (test->head_index + test->size == e->head_index) {//释放的内存与该空闲分区的后面相连
			if (flag != 3) {
				//printf("%d\n", flag);
				test->size = test->size + e->size;
				flag = 3;
			}
			//e = test;
			test = test->next;
		}
		else if (e->size < test->size) {
			if (flag == -1) {
				flag = 0;
				pre = test;
			}
			test = test->next;
		}
		else if (e->size >= test->size) {
			if (flag == -1) {
				flag = 1;
				rear = test;
			}
			test = test->next;
		}
		else {
			printf("error!\n");
			test = test->next;
		}
	}
	if (flag == 1) {//后
		e->next = rear->next;
		rear->next = e;
	}
	else if (flag == 0) {//前
		EmNode *test1;
		test1 = E;
		while (test1->next != pre)
			test1 = test1->next;
		e->next = test1->next;
		test1->next = e;
	}
	else if (flag == 2) {
		e->next = E->next;
		E->next = e;
	}
	else
		free(e);
	check_Em(E);
	check_Em1(E);
	return ok;
}
//进程释放内存（WF）
status Free_WF(P_list &P, Em_list &E, int ID) {
	int head_index;
	int size;
	PNode *temp, *temp1;
	temp = P->next;
	temp1 = P;
	while (temp != NULL) {
		if (temp->ID == ID)
			break;
		temp = temp->next;
		temp1 = temp1->next;
	}
	if (temp->ID != ID)
		return error;
	head_index = temp->head_index;
	size = temp->size;
	temp1->next = temp->next;
	free(temp);
	EmNode *e;
	e = (EmNode *)malloc(sizeof(EmNode));
	e->head_index = head_index;
	e->size = size;
	EmNode *test;
	EmNode *pre, *rear;
	pre = rear = E->next;
	test = E->next;
	int flag = -1;
	if (test == NULL)
		flag = 2;
	while (test != NULL) {
		if (test->head_index == e->head_index + e->size) {//释放的内存与该空闲分区的前面相连
			if (flag != 3) {
				test->head_index = e->head_index;
				test->size = test->size + e->size;
				flag = 3;
			}

			//e->head_index = test->head_index;
			//e->size = test->size;
			test = test->next;
			//printf("%d\n", flag);
		}
		else if (test->head_index + test->size == e->head_index) {//释放的内存与该空闲分区的后面相连
			if (flag != 3) {
				//printf("%d\n", flag);
				test->size = test->size + e->size;
				flag = 3;
			}
			//e = test;
			test = test->next;
		}
		else if (e->size > test->size) {
			if (flag == -1) {
				flag = 0;
				pre = test;
			}
			test = test->next;
		}
		else if (e->size <= test->size) {
			if (flag == -1) {
				flag = 1;
				rear = test;
			}
			test = test->next;
		}
		else {
			printf("error!\n");
			test = test->next;
		}
	}
	if (flag == 1) {//后
		e->next = rear->next;
		rear->next = e;
	}
	else if (flag == 0) {//前
		EmNode *test1;
		test1 = E;
		while (test1->next != pre)
			test1 = test1->next;
		e->next = test1->next;
		test1->next = e;
	}
	else if (flag == 2) {
		e->next = E->next;
		E->next = e;
	}
	else
		free(e);
	check_Em(E);
	check_Em2(E);
	return ok;
}
//访问单个进程节点
status visit_P(PNode p) {
	printf("进程ID：%d, 内存起始地址：%d， 占用内存大小：%d\n", p.ID, p.head_index, p.size);
	return ok;
}
//遍历进程链
status travel_P(P_list P, status visit(PNode)) {
	PNode *p;
	p = P->next;
	while (p != NULL) {
		visit(*p);
		p = p->next;
	}
	if (P->next == NULL)
		printf("空！\n");
	return ok;
}
//访问单个空闲内存节点
status visit_E(EmNode e) {
	printf("内存起始地址：%d， 占用内存大小：%d\n", e.head_index, e.size);
	return ok;
}
//遍历空闲链
status travel_E(Em_list E, status visit(EmNode)) {
	EmNode *e;
	e = E->next;
	while (e != NULL) {
		visit(*e);
		e = e->next;
	}
	if (E->next == NULL) {
		printf("空!\n");
	}
	return ok;
}
//整理合并问题
status check_Em(Em_list &E) {
	EmNode *e, *temp;
	e = E->next;
	temp = E;
	while (e != NULL) {
		if (e->next != NULL) {
			if (e->head_index + e->size == e->next->head_index) {
				e->next->head_index = e->head_index;
				e->next->size = e->size + e->next->size;
				temp->next = e->next;
				free(e);
				e = temp->next;
			}
			else if (e->head_index == e->next->head_index + e->next->size) {
				e->next->size = e->size + e->next->size;
				temp->next = e->next;
				free(e);
				e = temp->next;
			}
		}
		e = e->next;
		temp = temp->next;
	}
	return ok;
}
//空内存块交换
status swap(Em_list &E, EmNode *a, EmNode *b) {
	/*EmNode *temp1 = NULL, *temp2 = NULL;
	EmNode *test;
	test = E->next;
	while (test != NULL) {
		if (test->next == a) {
			temp1 = test;
		}
		if (test->next == b) {
			temp2 = test;
		}
		test = test->next;
	}
	EmNode *temp;
	temp = b;
	b->next = a->next;
	temp1->next = b;
	a->next = temp->next;
	temp2->next = a;*/
	int head_index;
	int size;
	head_index = a->head_index;
	size = a->size;
	a->head_index = b->head_index;
	a->size = b->size;
	b->head_index = head_index;
	b->size = size;
	return ok;
}
//整理排序问题
status check_Em1(Em_list &E) {
	EmNode *e, *temp, *d;
	e = E->next;
	d = E->next;
	temp = E;
	while (d != NULL) {
		while (e != NULL) {
			if (d->size > e->size) {
				swap(E, d, e);
			}
			e = e->next;
		}
		d = d->next;
	}
	return ok;
}
//整理排序问题
status check_Em2(Em_list &E) {
	EmNode *e, *temp, *d;
	e = E->next;
	d = E->next;
	temp = E;
	while (d != NULL) {
		while (e != NULL) {
			if (d->size < e->size) {
				swap(E, d, e);
			}
			e = e->next;
		}
		d = d->next;
	}
	return ok;
}

void show_info(P_list P, Em_list E) {
	printf("当前进程块情况：\n");
	travel_P(P, visit_P);
	printf("当前内存块情况：\n");
	travel_E(E, visit_E);
}
void menu_FF(P_list &P, Em_list &E) {
	int ID = 0;
	int size;
	int tack;
	int ID_P = -1;
	while (true) {
		printf("请选择申请还是释放内存：\n1、申请\n2、释放\n3、退出\n");
		scanf_s("%d", &tack);
		if (tack == 1) {
			printf("请输入申请内存大小：");
			scanf_s("%d", &size);
			if (Request_FF(P, E, ID, size)) {
				printf("申请成功！\n");
				ID++;
				show_info(P, E);
			}
			else {
				printf("申请失败！\n");
			}
		}
		else if (tack == 2) {
			printf("请输入要释放进程的ID：");
			scanf_s("%d", &ID_P);
			if (Free_FF(P, E, ID_P)) {
				printf("释放成功！\n");
				show_info(P, E);
			}
			else {
				printf("释放失败！\n");
			}
		}
		else {
			printf("正在退出！\n");
			break;
		}
	}
}
void menu_BF(P_list &P, Em_list &E) {
	int ID = 0;
	int size;
	int tack;
	int ID_P=-1;
	while (true) {
		printf("请选择申请还是释放内存：\n1、申请\n2、释放\n3、退出\n");
		scanf_s("%d", &tack);
		if (tack == 1) {
			printf("请输入申请内存大小：");
			scanf_s("%d", &size);
			if (Request_BF(P, E, ID, size)) {
				printf("申请成功！\n");
				ID++;
				show_info(P, E);
			}
			else {
				printf("申请失败！\n");
			}
		}
		else if (tack == 2) {
			printf("请输入要释放进程的ID：");
			scanf_s("%d", &ID_P);
			if (Free_BF(P, E, ID_P)) {
				printf("释放成功！\n");
				show_info(P, E);
			}
			else {
				printf("释放失败！\n");
			}
		}
		else {
			printf("正在退出！\n");
			break;
		}
	}
}
void menu_WF(P_list &P, Em_list &E) {
	int ID = 0;
	int size;
	int tack;
	int ID_P = -1;
	while (true) {
		printf("请选择申请还是释放内存：\n1、申请\n2、释放\n3、退出\n");
		scanf_s("%d", &tack);
		if (tack == 1) {
			printf("请输入申请内存大小：");
			scanf_s("%d", &size);
			if (Request_WF(P, E, ID, size)) {
				printf("申请成功！\n");
				ID++;
				show_info(P, E);
			}
			else {
				printf("申请失败！\n");
			}
		}
		else if (tack == 2) {
			printf("请输入要释放进程的ID：");
			scanf_s("%d", &ID_P);
			if (Free_WF(P, E, ID_P)) {
				printf("释放成功！\n");
				show_info(P, E);
			}
			else {
				printf("释放失败！\n");
			}
		}
		else {
			printf("正在退出！\n");
			break;
		}
	}
}

////测试数据
//int main()
//{
//	P_list P;
//	Em_list E;
//	if (!init(P, E)) {
//		printf("初始化失败！\n");
//		system("pause");
//		exit(0);
//	}
//	if (P->next == NULL && E->next->next == NULL) {
//		printf("No problem!\n");
//	}
//	printf("%d, %d\n", E->next->head_index, E->next->size);
//	int ID = 0;
//	if (Request_test(P, E, ID, 10))
//		printf("ok\n");
//	printf("%d, %d, %d\n", P->next->head_index, P->next->ID, P->next->size);
//	printf("%d, %d\n", E->next->head_index, E->next->size);
//	Request_test(P, E, 1, 20);
//	printf("%d, %d\n", E->next->head_index, E->next->size);
//	Request_test(P, E, 2, 30);
//	printf("%d, %d\n", E->next->head_index, E->next->size);
//	Request_test(P, E, 3, 40);
//	if (E->next == NULL)
//		printf("all over!\n");
//	Free_test(P, E, 3);
//	travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	printf("###################################\n");
//	Free_test(P, E, 1);
//	travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	printf("###################################\n");
//	Request_test(P, E, 4, 30);
//	travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	printf("###################################\n");
//	Free_test(P, E, 0);
//	travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	printf("###################################\n");
//	Free_test(P, E, 4);
//	travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	printf("###################################\n");
//	Free_test(P, E, 2);
//	//travel_P(P, visit_P);
//	travel_E(E, visit_E);
//	system("pause");
//    return 0;
//}
//测试数据

int main()
{
	P_list P;
	Em_list E;
	int str;
	if (!init(P, E)) {
		printf("初始化失败！\n");
		system("pause");
		exit(0);
	}
	printf("请选择采用的内存分派策略：\n1、FF\n2、BF\n3、WF\n");
	scanf_s("%d", &str);
	if (str == 1) {
		menu_FF(P, E);
	}
	else if (str == 2) {
		menu_BF(P, E);
	}
	else if (str == 3) {
		menu_WF(P, E);
	}
	else {
		printf("input error, exiting now!\n");
		system("pause");
		exit(0);
	}

	system("pause");
	return 0;
}

