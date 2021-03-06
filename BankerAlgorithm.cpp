#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>

#define NUM_PROC 5
#define NUM_RES 3
typedef enum {error, ok} status;

status Security_test(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES]);
status Travel(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES]);
status Dijkstra(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES], int proc, int *Request);

//安全检测算法
status Security_test(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES]) {
	int Available_test[NUM_RES];
	int Max_test[NUM_PROC][NUM_RES];
	int Allocation_test[NUM_PROC][NUM_RES];
	int Need_test[NUM_PROC][NUM_RES];
	for(int i = 0; i<NUM_PROC; i++)
		for (int j = 0; j < NUM_RES; j++) {
			Max_test[i][j] = Max[i][j];
			Allocation_test[i][j] = Allocation[i][j];
			Need_test[i][j] = Need[i][j];
		}
	int Work[NUM_RES];
	int Finish[NUM_PROC] = {0, 0, 0, 0, 0};
	for (int i = 0; i < NUM_RES; i++) {
		Work[i] = Available[i];
		Available_test[i] = Available[i];
	}
	printf("进程     Work    Need    Allocation    Work+Allocation    Finish\n");
	for (int k = 0; k < NUM_PROC; k++) {
		int proc = -1;
		for (int i = 0; i < NUM_PROC; i++) {
			int flag = 1;
			for (int j = 0; j < NUM_RES; j++) {
				if (Need_test[i][j] > Work[j]) {
					flag = 0;
					break;
				}
			}
			if (flag == 1) {
				proc = i;
				break;
			}
		}
		if (proc == -1)
			break;
		else {
			for (int i = 0; i < NUM_RES; i++) {
				Work[i] = Work[i] + Allocation_test[proc][i];
				Need_test[proc][i] = 999;
			}
			Finish[proc] = 1;
		}
		if (proc != -1) {
			printf(" %d\t", proc);
			for (int i = 0; i < NUM_RES; i++) {
				printf("%d ", Work[i] - Allocation_test[proc][i]);
			}
			printf("\t ");
			for (int i = 0; i < NUM_RES; i++) {
				printf("%d ", Need[proc][i]);
			}
			printf("\t  ");
			for (int i = 0; i < NUM_RES; i++) {
				printf("%d ", Allocation[proc][i]);
			}
			printf("\t    ");
			for (int i = 0; i < NUM_RES; i++) {
				printf("%d ", Work[i]);
			}
			printf("\t    %d\n", Finish[proc]);
		}

	}
	int flag = 1;
	for (int i = 0; i < NUM_PROC; i++) {
		if (Finish[i] != 1) {
			flag = 0;
			break;
		}
	}
	if (flag == 1)
		return ok;
	else
		return error;
}
//银行家算法
status Dijkstra(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES], int proc, int *Request) {
	int flag = 1;
	for (int i = 0; i < NUM_RES; i++) {
		if (Request[i] > Need[proc][i])
			flag = 0;
	}
	//Need[0][0] = 0;
	//Need[0][1] = 0;
	//Need[0][2] = 0;
	if (flag == 0)
		return error;
	flag = 1;
	for (int i = 0; i < NUM_RES; i++) {
		if (Request[i] > Available[i])
			flag = 0;
	}
	if (flag == 0)
		return error;
	for (int i = 0; i < NUM_RES; i++) {
		Available[i] = Available[i] - Request[i];
		Allocation[proc][i] = Allocation[proc][i] + Request[i];
		Need[proc][i] = Need[proc][i] - Request[i];
	}
	//printf("安全检测前：\n");
	//Travel(Available, Max, Allocation, Need);
	status test;
	test = Security_test(Available, Max, Allocation, Need);
	if (test == ok)
		return ok;
	else {
		for (int i = 0; i < NUM_RES; i++) {
			Available[i] = Available[i] + Request[i];
			Allocation[proc][i] = Allocation[proc][i] - Request[i];
			Need[proc][i] = Need[proc][i] + Request[i];
		}
		return error;
	}
}
//遍历资源
status Travel(int Available[NUM_RES], int Max[NUM_PROC][NUM_RES], int Allocation[NUM_PROC][NUM_RES], int Need[NUM_PROC][NUM_RES]) {
	printf("可利用资源：\n");
	for (int i = 0; i < NUM_RES; i++) {
		printf("%d ", Available[i]);
	}
	printf("\n");
	/*printf("最大需求矩阵：\n");
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Max[i][j]);
		}
		printf("\n");
	}
	printf("分配矩阵：\n");
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Allocation[i][j]);
		}
		printf("\n");
	}
	printf("需求矩阵：\n");
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Need[i][j]);
		}
		printf("\n");
	}*/
	printf("进程号 最大需求矩阵   分配矩阵 需求矩阵\n");
	for (int i = 0; i < NUM_PROC; i++) {
		printf("  %d\t", i);
		printf("  ");
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Max[i][j]);
		}
		printf("\t");
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Allocation[i][j]);
		}
		printf("\t");
		for (int j = 0; j < NUM_RES; j++) {
			printf("%d ", Need[i][j]);
		}
		printf("\n");
	}
	return ok;
}

////测试
//int main()
//{
//	int Available[NUM_RES] = { 3, 3, 2 };
//	int Max[NUM_PROC][NUM_RES] = { { 7, 5, 3 },{ 3, 2, 2 },{ 9, 0, 2 },{ 2, 2, 2 },{ 4, 3, 3 } };
//	int Allocation[NUM_PROC][NUM_RES] = { { 0, 1, 0 },{ 2, 0, 0 },{ 3, 0, 2 },{ 2, 1, 1 },{ 0, 0, 2 } };
//	int Need[NUM_PROC][NUM_RES] = { { 7, 4, 3 },{ 1, 2, 2 },{ 6, 0, 0 },{ 0, 1, 1 },{ 4, 3, 1 } };
//	//int Request[3] = { 0, 1, 2 };
//	//printf("%d, %d, %d\n", Need[0][0], Need[0][1], Need[0][2]);
//	//Dijkstra(Available, Max, Allocation, Need, 0, Request);
//	//printf("%d, %d, %d\n", Need[0][0], Need[0][1], Need[0][2]);
//	if (Security_test(Available, Max, Allocation, Need))
//		printf("当前系统处于安全状态！\n");
//	else
//		printf("不安全！\n");
//	Travel(Available, Max, Allocation, Need);
//	int Request[3] = { 1, 0, 2 };
//	int proc = 1;
//	printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
//	if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
//		printf("分配成功！\n");
//	else
//		printf("分配失败！\n");
//	Travel(Available, Max, Allocation, Need);
//	printf("####################################################\n");
//	Request[0] = 4;
//	Request[1] = 3;
//	Request[2] = 1;
//	proc = 4;
//	printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
//	if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
//		printf("分配成功！\n");
//	else
//		printf("分配失败！\n");
//	Travel(Available, Max, Allocation, Need);
//	printf("####################################################\n");
//	Request[0] = 0;
//	Request[1] = 2;
//	Request[2] = 0;
//	proc = 0;
//	printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
//	if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
//		printf("分配成功！\n");
//	else
//		printf("分配失败！\n");
//	Travel(Available, Max, Allocation, Need);
//	printf("####################################################\n");
//	Request[0] = 0;
//	Request[1] = 1;
//	Request[2] = 0;
//	proc = 0;
//	printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
//	if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
//		printf("分配成功！\n");
//	else
//		printf("分配失败！\n");
//	Travel(Available, Max, Allocation, Need);
//	printf("####################################################\n");
//	Request[0] = 1;
//	Request[1] = 0;
//	Request[2] = 0;
//	proc = 0;
//	printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
//	if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
//		printf("分配成功！\n");
//	else
//		printf("分配失败！\n");
//	Travel(Available, Max, Allocation, Need);
//	if (Security_test(Available, Max, Allocation, Need))
//		printf("当前系统处于安全状态！\n");
//	else
//		printf("不安全！\n");
//	system("pause");
//    return 0;
//}

int main()
{
	//int Available[NUM_RES] = { 3, 3, 2 };
	//int Max[NUM_PROC][NUM_RES] = { { 7, 5, 3 },{ 3, 2, 2 },{ 9, 0, 2 },{ 2, 2, 2 },{ 4, 3, 3 } };
	//int Allocation[NUM_PROC][NUM_RES] = { { 0, 1, 0 },{ 2, 0, 0 },{ 3, 0, 2 },{ 2, 1, 1 },{ 0, 0, 2 } };
	//int Need[NUM_PROC][NUM_RES] = { { 7, 4, 3 },{ 1, 2, 2 },{ 6, 0, 0 },{ 0, 1, 1 },{ 4, 3, 1 } };
	//int Request[3] = { 0, 1, 2 };
	//printf("%d, %d, %d\n", Need[0][0], Need[0][1], Need[0][2]);
	//Dijkstra(Available, Max, Allocation, Need, 0, Request);
	//printf("%d, %d, %d\n", Need[0][0], Need[0][1], Need[0][2]);
	int Available[NUM_RES];
	int Max[NUM_PROC][NUM_RES];
	int Allocation[NUM_PROC][NUM_RES];
	int Need[NUM_PROC][NUM_RES];
	FILE *fp;
	fopen_s(&fp, "1.txt", "r");
	if (!fp) {
		printf("打开文件失败！\n");
		system("pause");
		exit(0);
	}
	for (int i = 0; i < NUM_RES; i++) {
		fscanf_s(fp, "%d", &Available[i]);
	}
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			fscanf_s(fp, "%d", &Max[i][j]);
		}
	}
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			fscanf_s(fp, "%d", &Allocation[i][j]);
		}
	}
	for (int i = 0; i < NUM_PROC; i++) {
		for (int j = 0; j < NUM_RES; j++) {
			fscanf_s(fp, "%d", &Need[i][j]);
		}
	}
	if (Security_test(Available, Max, Allocation, Need))
		printf("当前系统处于安全状态！\n可以分配内存\n");
	else
		printf("不安全!\n此时不能分配内存！\n");
	while (true) {
		Travel(Available, Max, Allocation, Need);
		int proc;
		int Request[3];
		printf("请输入要申请内存的进程号：\n>>>");
		scanf_s("%d", &proc);
		if (proc > 4 || proc < 0) {
			printf("进程号错误！\n");
			break;
		}
		printf("请输入A资源的申请数目：\n>>>");
		scanf_s("%d", &Request[0]);
		printf("请输入B资源的申请数目：\n>>>");
		scanf_s("%d", &Request[1]);
		printf("请输入C资源的申请数目：\n>>>");
		scanf_s("%d", &Request[2]);
		//scanf_s("%d, %d %d %d", &proc, &Request[0], &Request[1], &Request[1]);
		printf("申请资源：进程：%d, 资源数：%d %d %d\n", proc, Request[0], Request[1], Request[2]);
		if (Dijkstra(Available, Max, Allocation, Need, proc, Request))
			printf("分配成功！\n");
		else
			printf("分配失败！\n");
		//Travel(Available, Max, Allocation, Need);
	}
	
	
	system("pause");
	return 0;
}

////文件测试
//int main() {
//	int Allocation[NUM_PROC][NUM_RES];
//	FILE *fp;
//	fopen_s(&fp, "1.txt", "r");
//	if (!fp) {
//		printf("打开文件失败！\n");
//		system("pause");
//		exit(0);
//	}
//	for (int i = 0; i < NUM_PROC; i++) {
//		for (int j = 0; j < NUM_RES; j++) {
//			fscanf_s(fp, "%d", &Allocation[i][j]);
//		}
//	}
//	for (int i = 0; i < NUM_PROC; i++) {
//		for (int j = 0; j < NUM_RES; j++) {
//			printf("%d, ", Allocation[i][j]);
//		}
//		printf("\n");
//	}
//	system("pause");
//	return 0;
//}