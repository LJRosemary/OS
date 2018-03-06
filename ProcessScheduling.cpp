#include<stdio.h>
#include<stdlib.h>

//资源状态：0：可分配， 1：被占用
//进程状态：0：就绪， 1：运行， 2：阻塞
//优先级：1， 2， 3， 4。。。（从小至大依次增大）
//进程与资源标识：按进入队列顺序从1-n


//进程的PCB
struct P_Node{
	int priv;//优先级
	int PID;//进程标识
	int res_id;//需要申请的资源
	int stat;//进程当前状态
	struct P_Node *next_p;//下一个进程块
};
//进程队列
struct P_list{
	struct P_Node *front;
	struct P_Node *rear;
};
//资源块
struct R_Node{
	int RID;//资源标识
	struct R_Node *next_r;//下一个资源
	struct P_list block_p;//当前资源阻塞的进程链
	int stat;//当前资源状态
};
//资源队列
struct R_list{
	struct R_Node *front;
	struct R_Node *rear;
};
//进程队列初始化
bool init_P_list(P_list &q){
	q.front = q.rear = (P_Node*)malloc(sizeof(P_Node));
	if(!q.front)
		return false;
	q.front->next_p = NULL;
	return true;
}
//资源队列初始化
bool init_R_list(R_list &q){
	q.front = q.rear = (R_Node*)malloc(sizeof(R_Node));
	if(!q.front)
		return false;
	q.front->next_r = NULL;
	//if(init_P_list(q.front->block_p))
	//	return true;
	//else
	return true;
}
//进程入队列
bool en_p_list(P_list &q, int priv, int PID, int res_id, int stat){
	P_Node *p;
	p = (P_Node*)malloc(sizeof(P_Node));
	if(!p)
		return false;
	p->priv = priv;
	p->PID = PID;
	p->res_id = res_id;
	p->stat = stat;
	p->next_p = NULL;
	//if(q.rear == q.front->next_p)
	//	printf("666\n");
	q.rear->next_p = p;
	q.rear = p;
	//if(q.front->next_p == NULL){
	//	printf("777\n");
	//}
	return true;
}
//进程出队列
bool de_q_list(P_list &q){
	P_Node *p;
	if(q.front == q.rear)
		return false;
	p = q.front->next_p;
	if(p == q.rear){
		q.rear = q.front;
		q.rear->next_p = NULL;
	}
	else
		q.front->next_p = p->next_p;
	free(p);
	return true;
}
//进程队列开头插入进程
bool insert_q_list(P_list &p, P_Node *q){
	P_Node *temp;
	temp = (P_Node*)malloc(sizeof(P_Node));
	//temp = q;
	temp->PID = q->PID;
	temp->priv = q->priv;
	temp->res_id = q->res_id;
	temp->stat = q->stat;
	
	temp->next_p = p.front->next_p;
	
	p.front->next_p = temp;
	//if(p.front->next_p->next_p == p.rear){
	//	printf("131313\n");
	//}
	return true;
}
//进程队列删除指定位置进程
bool del_q_list(P_list &p, int index){
	P_Node *q, *temp;
	temp = p.front;
	q = p.front->next_p;
	for(int i=1;i<index;i++){
		q = q->next_p;
		temp = temp->next_p;
	}
	temp->next_p = q->next_p;
	
	if(q == p.rear){
		p.rear = temp;
		//printf("141414\n");
	}	
	free(q);
	return true;
}
//资源入队列
bool en_r_list(R_list &q, int RID, int stat){
	R_Node *p;
	p = (R_Node*)malloc(sizeof(R_Node));
	if(!p)
		return false;
	p->RID = RID;
	p->stat = stat;
	init_P_list(p->block_p);
	//p->block_p = p_list;
	p->next_r = NULL;
	q.rear->next_r = p;
	q.rear = p;
	return true;
}
//资源出队列
bool de_r_list(R_list &q){
	R_Node *p;
	if(q.front == q.rear)
		return false;
	p = q.front->next_r;
	if(p == q.rear){
		q.rear = q.front;
		q.rear->next_r = NULL;
	}
	else
		q.front->next_r = p->next_r;
	free(p);
	return true;
}
//初始化
bool init(P_list &p_ready, R_list &r, P_list &p_block){
	if(init_P_list(p_ready) && init_R_list(r) && init_P_list(p_block))
		return true;
	else
		return false;
}
//创建进程
bool create_p(P_list &p, R_list &r, int priv, int PID, int res_id){
	if(r.front == r.rear){
		printf("没有资源可用！");
		return false;
	}
	R_Node *q;
	q = r.front->next_r;
	while(1){
		if(res_id == q->RID){
			if(q->stat == 0){//如果资源空闲
				en_p_list(p, priv, PID, res_id, 0);
				q->stat = 1;
			}
			else{
				en_p_list(q->block_p, priv, PID, res_id, 2);
			}
			break;
		}
		else
			if(q->next_r == NULL)
				break;
			q = q->next_r;
	}
	return true;
}
void shell(P_list &p_ready, R_list &r){
	//create_p(P_list &p, R_list &r, int priv, int PID, int res_id)
	char c;
	int PID;
	int priv;
	int res_id;
	printf("是否要创建新进程？（y/n）: ");
	scanf("%c", &c);
	if(c == '\n')
		scanf("%c", &c);
	getchar();
//	printf("%c", c);
	if(c == 'y'){
		printf("请输入进程ID(正整数)，优先级(正整数)，所申请的资源id(1-5)，例：1, 2, 3:\n");
		scanf("%d, %d, %d", &PID, &priv, &res_id);
		create_p(p_ready, r, priv, PID, res_id);
	}
}
/*
资源检查:
1、先判断资源是否有阻塞序列
2、如果有阻塞队列且资源空闲，从阻塞队列中调出进程获取资源并加入就绪队列中
*/
void re_check(R_list &r, P_list &p){
	R_Node *q;
	int priv, PID, res_id;
	q = r.front->next_r;
	while(1){
		if(q->stat==0 && (q->block_p.front != q->block_p.rear)){
			priv = q->block_p.front->next_p->priv;
			PID = q->block_p.front->next_p->PID;
			res_id = q->block_p.front->next_p->res_id;
		//	if(p.rear == NULL)
		//		printf("555\n");
			en_p_list(p, priv, PID, res_id, 0);
			
		//	if(p.rear->next_p == NULL){
		//		printf("333\n");
		//		printf("%d, %d, %d, %d\n", p.rear->PID, p.rear->priv, p.rear->res_id, p.rear->stat);
		//		printf("%d, %d, %d, %d\n", p.front->next_p->PID, p.front->next_p->priv, p.front->next_p->res_id, p.front->next_p->stat);
		//	}
				
			q->stat = 1;
			de_q_list(q->block_p);
			break;
		}
		else{
			if(q->next_r == NULL)
				break;
			q = q->next_r;
		}
	}
}
/*
进程调度:
1、遍历就绪队列，将优先级最高的放在队列开头
2、遍历阻塞队列，如果当前最高优先级的进程小于阻塞队列中进程优先级的最大值，则将阻塞队列的该进程调入就绪队列的开头
3、判断就绪队列的最大优先级与当前正在执行的进程的优先级，如果差值大于等于3，则将正在执行的进程阻塞，执行最高优先级的进程，
并将被阻塞进程的优先级增加1
*/
int p_adjust(P_list &p_ready, P_list &p_block, P_Node *p, int index){
	//1
	
	P_Node *q, *temp;
	int count = 2;
	
	int flag = 0; 
	if(index%4!=0 && index!=0){
		temp = p_ready.front->next_p;
		if(temp->priv - p->priv >= 3){
			//printf("1\n");
			p->stat = 2;
			p->priv++;
			en_p_list(p_block, p->priv, p->PID, p->res_id, p->stat);
			printf("资源%d被阻塞\n", p->PID);
			flag = 1;
			//printf("%d, %d, %d, %d\n", p_block.front->next_p->PID, p_block.front->next_p->priv, p_block.front->next_p->res_id, p_block.front->next_p->stat);
		}
	}
	
	
	
	temp = q = p_ready.front->next_p;
	
	while(temp!=NULL){
		if(temp->priv > q->priv)
			q = temp;
		temp = temp->next_p;
	}
	
	//printf("%d, %d, %d, %d\n", q->PID, q->priv, q->res_id, q->stat);
	insert_q_list(p_ready, q);	

	//if(p_ready.front->next_p == p_ready.rear)
	//	printf("111111\n");
	temp = p_ready.front->next_p->next_p;
	//if(p_ready.front->next_p->next_p == p_ready.rear)
	//	printf("111111\n");
	//if(temp->next_p != NULL)
	//	temp = temp->next_p;
	//if(temp->next_p!=NULL){
	//	temp = temp->next_p;
	//	if(temp->next_p!=NULL)
	//		temp = temp->next_p;
	//}
	
	while(1){
		if(temp->priv == q->priv){
			//printf("222222\n");
			break;
		}
		else{
			count++;
			temp = temp->next_p;
		}
	}
	del_q_list(p_ready, count);
	
	//2
	count = 1;
	temp = p_block.front->next_p;

	//if(temp == NULL){
	//	printf("shutdown\n");
	//}
	
	while(temp != NULL){
		//printf("1\n");
		//printf("%d, %d, %d, %d\n", temp->PID, temp->priv, temp->res_id, temp->stat);
		if(temp->priv > q->priv){
			insert_q_list(p_ready, temp);
			del_q_list(p_block, count);
			break;
		}
		else{
			temp = temp->next_p;
			count++;
		}
	}
	
	//3
	
	return flag;
}
/*
执行进程:
1、每次从就绪队列中选取第一个执行
2、每4轮循环默认执行一个进程
3、每轮循环结束阻塞队列中的进程优先级加一
4、每次选择下一个执行的进程之前，都要执行进程调度
5、每轮循环结束后可选择是否创建新进程
6、每个进程执行结束都要释放资源
7、每轮循环都要执行资源检测
*/
void run_p(P_list &p, P_list &p_block, P_Node *q, R_list &r){
	int count = 0;
	R_Node *t;
	P_Node *temp;
	P_Node *temp1;
	P_Node *temp2;
	int flag1;
	while(1){
		flag1 = 0;
		if(count%4 == 0){
			if(q!=NULL){ 
				t = r.front->next_r;
				while(t->next_r!=NULL){
					if(t->RID == q->res_id){
						t->stat = 0;
						break;
					}
					else{
						t = t->next_r;
					}
				}
			}
			re_check(r, p);
		//	if(p.front->next_p == NULL){
		//		printf("111\n");
		//	}
			if(p.front->next_p != NULL)
				p_adjust(p, p_block, q, count);
			//printf("%d\n", p.front->next_p->PID);

			temp1 = (P_Node*)malloc(sizeof(P_Node));
			temp1->PID = p.front->next_p->PID;
			temp1->priv = p.front->next_p->priv;
			temp1->res_id = p.front->next_p->res_id;
			temp1->stat = p.front->next_p->stat;
			//printf("%d, %d, %d, %d\n", temp1->PID, temp1->priv, temp1->res_id, temp1->stat);
			//q = p.front->next_p;
			temp2 = q;
			//if(temp2==NULL)
			//	printf("999\n");
			//printf("%d, %d, %d, %d\n", temp2->PID, temp2->priv, temp2->res_id, temp2->stat);
			free(temp2);
			q = temp1;
			//printf("%d, %d, %d, %d\n", q->PID, q->priv, q->res_id, q->stat);
			q->stat = 1;
			//printf("%d, %d, %d, %d\n", p.rear->PID, p.rear->priv, p.rear->res_id, p.rear->stat);
		//	if(p.rear == p.front->next_p)
		//		printf("101010\n");
			del_q_list(p, 1);

		//	if(p.rear->next_p == NULL)
		//		printf("888\n");
			count = 0;
		}
		if(q!=NULL)
			printf("当前正在执行程序的PID为%d\n", q->PID);
		
		temp = p_block.front->next_p;
		while(temp!=NULL){
			temp->priv++;
			temp = temp->next_p;
		}
		
		//添加用户交互
		shell(p, r);

		re_check(r, p);
		
	//	if(p.front->next_p == NULL){
	//		printf("222\n");
	//	}
		
		if((count%4!=0) && (p.front->next_p != NULL))
			flag1 = p_adjust(p, p_block, q, count);

		
		count++;
		if(flag1){
		//	printf("%d\n", count);
			count = 0;
		}
		R_Node *test;
		int flag = 0;
		test = r.front->next_r;
		while(1){
			if(test->block_p.front != test->block_p.rear){
				flag = 1;
				break;
			}
			if(test->next_r != NULL){
				test = test->next_r;
			}
			else
				break;
			
		}
		if((count%4==0) && (p.front == p.rear) && (p_block.front == p_block.rear) && (flag == 0)){
			//printf("out\n");
			//printf("%d\n", count);
			break;
		}
	//	if(count == 20)
	//		break;
		
	}
}

void main(){
	//检测队列创建
	//P_list p;
	//R_list r;
	//init_P_list(p);
	//init_R_list(r);
	//en_p_list(p, 1, 1, 0, 0);
	//en_p_list(p, 2, 3, 4, 5);
	//en_p_list(p, 3, 4, 5, 6);
	//printf("%d, %d, %d, %d\n", p.rear->priv, p.rear->PID, p.rear->res_id, p.rear->stat);
	//printf("%d, %d, %d, %d\n", p.front->next_p->priv, p.front->next_p->PID, p.front->next_p->res_id, p.front->next_p->stat);
	//en_r_list(r, 1, 0);
	//en_p_list(r.rear->block_p, 1, 2, 3, 4);
	//printf("%d, %d\n", r.rear->RID, r.rear->stat);
	//printf("%d, %d, %d, %d\n", r.rear->block_p.rear->priv, r.rear->block_p.rear->PID, r.rear->block_p.rear->res_id, r.rear->block_p.rear->stat);
	P_list p_ready;//就绪队列
	R_list r;//资源队列
	P_list p_block;//阻塞队列
	P_Node *run = NULL;//正在运行的进程
	if(!init(p_ready, r, p_block)){
		printf("初始化失败，退出程序。。。");
		exit(0);
	}
	//初始定义五个资源（除CPU）
	for(int i=1;i<=5;i++){
		en_r_list(r, i, 0);
	}
	//create_p(P_list &p, R_list &r, int priv, int PID, int res_id)
	create_p(p_ready, r, 2, 11, 1);
	create_p(p_ready, r, 1, 22, 1);
	create_p(p_ready, r, 3, 33, 1);
	create_p(p_ready, r, 4, 44, 1);
	create_p(p_ready, r, 5, 55, 1);
	//run_p(P_list &p, P_list &p_block, P_Node *q, R_list &r)
	run_p(p_ready, p_block, run, r);
	//shell(p_ready, r);
	//printf("%d, %d, %d, %d\n", p_ready.rear->priv, p_ready.rear->PID, p_ready.rear->res_id, p_ready.rear->stat);
	//re_check(R_list &r, P_list &p)
	//re_check(r, p_ready);
	//p_adjust(P_list &p_ready, P_list &p_block, P_Node *p, int index)
	//p_adjust(p_ready, p_block, run, 4);


	/*检测创建进程函数
	create_p(p_ready, r, 1, 2, 3);
	create_p(p_ready, r, 1, 4, 3);
	printf("%d, %d, %d, %d\n", p.rear->priv, p.rear->PID, p.rear->res_id, p.rear->stat);
	R_Node *q;
	q = r.front->next_r;
	while(1){
		if(q->RID == 3){
			if(q->block_p.front == q->block_p.rear)
				printf("blank\n");
			else
				printf("%d\n", q->block_p.rear->PID);
			break;
		}
		else{
			if(q->next_r == NULL)
				break;
			q = q->next_r;
		}
	}
	*/
}



