#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef enum{error, ok} status;
typedef struct Phy_Node {
	int page;
	int time;
	Phy_Node *next;
}*Phylist;

status init(Phylist &P);
int length(Phylist P);
status insert_p(Phylist &P, int page);
status delete_p(Phylist &P, int page);
status travel(Phylist P);
status adjust_Best(Phylist &P, int page, int *dir, int i);
status adjust_FIFO(Phylist &P, int page);
status adjust_LRU(Phylist &P, int page);
status pre_adjust(Phylist &P, int page, int str, int *dir, int i);

//初始化
status init(Phylist &P) {
	P = (Phy_Node *)malloc(sizeof(Phy_Node));
	if (!P)
		return error;
	P->next = NULL;
	return ok;
}
//清空队列
status clear(Phylist &P) {
	Phy_Node *p;
	while (P->next != NULL) {
		p = P->next;
		P->next = p->next;
		free(p);
	}
	return ok;
}
//物理块链长度
int length(Phylist P) {
	int len = 0;
	while (P->next != NULL)
	{
		len++;
		P = P->next;
	}
	return len;
}
//插入页（只在物理块链的末尾插入）
status insert_p(Phylist &P, int page) {
	Phy_Node *p, *temp;
	p = (Phy_Node *)malloc(sizeof(Phy_Node));
	if (!p)
		return error;
	p->page = page;
	p->time = 0;
	temp = P;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	p->next = NULL;
	temp->next = p;
	return ok;
}
//删除链（根据不同的分配策略，删除不同的页）
status delete_p(Phylist &P, int page) {
	Phy_Node *p, *temp;
	temp = P->next;
	p = P;
	while (temp != NULL) {
		if (temp->page == page)
			break;
		temp = temp->next;
		p = p->next;
	}
	p->next = temp->next;
	free(temp);
	return ok;
}
//遍历物理块链
status travel(Phylist P) {
	while (P->next != NULL) {
		printf("%d ", P->next->page);
		P = P->next;
	}
	printf("\n");
	return ok;
}
//最佳置换算法
status adjust_Best(Phylist &P, int page, int *dir, int i) {
	int count[5];
	Phy_Node *p;
	p = P->next;
	for (int j = 0; j < 5; j++) {
		for (int k = i; k < 30; k++) {
			if (p->page == dir[k]) {
				break;
			}
			count[j]++;
		}
		p = p->next;
	}
	int index = 0, temp;
	for (int j = 0; j < 4; j++) {
		if (count[j] > count[j + 1]) {
			temp = count[j];
			count[j] = count[j + 1];
			count[j + 1] = temp;
		}
		else {
			index = j + 1;
		}
	}
	int page_temp;
	p = P->next;
	while (index--) {
		p = p->next;
	}
	page_temp = p->page;
	insert_p(P, page);
	delete_p(P, page_temp);
	return ok;
}
//先进先出置换算法
status adjust_FIFO(Phylist &P, int page) {
	int page_temp = P->next->page;
	insert_p(P, page);
	delete_p(P, page_temp);
	return ok;
}
//最近最久未使用置换算法
status adjust_LRU(Phylist &P, int page) {
	int page_temp;
	Phy_Node *p, *temp;
	p = P->next;
	int time = p->time;
	temp = p;
	while (p != NULL) {
		if (p->time > time)
			temp = p;
		p = p->next;
	}
	page_temp = temp->page;
	insert_p(P, page);
	delete_p(P, page_temp);
	return ok;
}
//调用置换算法之前的预处理
int pre_adjust(Phylist &P, int page, int str, int *dir, int i, int &suc) {
	Phy_Node *p;
	int flag = -1;
	p = P->next;
	while (p != NULL) {
		if (p->page == page) {
			p->time = 0;
			flag = 0;
			suc++;
		}
		p = p->next;
	}
	if (flag == -1) {
		if (length(P) < 5) {
			insert_p(P, page);
		}
		else if (str == 1) {
			adjust_Best(P, page, dir, i);
		}
		else if (str == 2) {
			adjust_FIFO(P, page);
		}
		else if(str == 3){
			adjust_LRU(P, page);
		}
	}
	p = P->next;
	while (p != NULL) {
		p->time++;
		p = p->next;
	}
	travel(P);
	return suc;
}

////测试
//int main()
//{
//	Phylist P1, P2, P3;
//	
//	int str;
//	if (!init(P1)) {
//		printf("初始化错误！\n");
//		system("pause");
//		exit(0);
//	}
//	init(P2);
//	init(P3);
//	int dir[30];
//	dir[0] = 3;
//	dir[1] = 2;
//	dir[2] = 8;
//	dir[3] = 5;
//	dir[4] = 0;
//	dir[5] = 7;
//	dir[6] = 3;
//	dir[7] = 9;
//	dir[8] = 1;
//	dir[9] = 8;
//	dir[10] = 0;
//	dir[11] = 3;
//	dir[12] = 5;
//	dir[13] = 6;
//	dir[14] = 6;
//	int suc = 0;
//	printf("本次页面走向为：\n");
//	for (int i = 0; i < 15; i++) {
//		dir[i] = ((i * i * i) + 13) % 10;
//		printf("%d ", dir[i]);
//	}
//	printf("\n");
//	printf("####################################################\n");
//	printf("Best置换算法:\n");
//	for (int i = 0; i < 15; i++) {
//		pre_adjust(P1, dir[i], 1, dir, i, suc);
//	}
//	printf("成功访问页数：%d\n", suc);
//	printf("缺页率为：%2f\n", (15 - (float)suc) / 15);
//	suc = 0;
//	printf("本次页面走向为：\n");
//	for (int i = 0; i < 15; i++) {
//		dir[i] = ((i * i * i) + 13) % 10;
//		printf("%d ", dir[i]);
//	}
//	printf("\n");
//	printf("####################################################\n");
//	printf("FIFO置换算法:\n");
//	for (int i = 0; i < 15; i++) {
//		pre_adjust(P2, dir[i], 2, dir, i, suc);
//	}
//	printf("成功访问页数：%d\n", suc);
//	printf("缺页率为：%2f\n", (15 - (float)suc) / 15);
//	suc = 0;
//	printf("本次页面走向为：\n");
//	for (int i = 0; i < 15; i++) {
//		dir[i] = ((i * i * i) + 13) % 10;
//		printf("%d ", dir[i]);
//	}
//	printf("\n");
//	printf("####################################################\n");
//	printf("LRU置换算法:\n");
//	for (int i = 0; i < 15; i++) {
//		pre_adjust(P3, dir[i], 3, dir, i, suc);
//	}
//	printf("成功访问页数：%d\n", suc);
//	printf("缺页率为：%2f\n", (15 - (float)suc) / 15);
//	system("pause");
//	return 0;
//}

//正常程序运行
int main()
{
	Phylist P;
	int str;
	if (!init(P)) {
		printf("初始化错误！\n");
		system("pause");
		exit(0);
	}
	int dir[30];
	srand((unsigned)time(NULL));
	printf("本次页面走向为：\n");
	for (int i = 0; i < 30; i++) {
		dir[i] = rand() % 10;
		printf("%d ", dir[i]);
	}
	printf("\n");
	while (true) {
		printf("请选择要使用的页面置换策略：\n1、Best\n2、FIFO\n3、LRU\n>>>");
		scanf_s("%d", &str);
		if (str != 1 && str != 2 && str != 3) {
			printf("input error!\nexiting now!\n");
			system("pause");
			exit(0);
		}
		int suc = 0;
		for (int i = 0; i < 30; i++) {
			pre_adjust(P, dir[i], str, dir, i, suc);
		}
		printf("缺页率为：%2f\n", (30 - (float)suc) / 30);
		clear(P);
	}
	system("pause");
    return 0;
}

