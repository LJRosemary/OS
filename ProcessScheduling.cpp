#include<stdio.h>
#include<stdlib.h>

//��Դ״̬��0���ɷ��䣬 1����ռ��
//����״̬��0�������� 1�����У� 2������
//���ȼ���1�� 2�� 3�� 4����������С������������
//��������Դ��ʶ�����������˳���1-n


//���̵�PCB
struct P_Node{
	int priv;//���ȼ�
	int PID;//���̱�ʶ
	int res_id;//��Ҫ�������Դ
	int stat;//���̵�ǰ״̬
	struct P_Node *next_p;//��һ�����̿�
};
//���̶���
struct P_list{
	struct P_Node *front;
	struct P_Node *rear;
};
//��Դ��
struct R_Node{
	int RID;//��Դ��ʶ
	struct R_Node *next_r;//��һ����Դ
	struct P_list block_p;//��ǰ��Դ�����Ľ�����
	int stat;//��ǰ��Դ״̬
};
//��Դ����
struct R_list{
	struct R_Node *front;
	struct R_Node *rear;
};
//���̶��г�ʼ��
bool init_P_list(P_list &q){
	q.front = q.rear = (P_Node*)malloc(sizeof(P_Node));
	if(!q.front)
		return false;
	q.front->next_p = NULL;
	return true;
}
//��Դ���г�ʼ��
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
//���������
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
//���̳�����
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
//���̶��п�ͷ�������
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
//���̶���ɾ��ָ��λ�ý���
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
//��Դ�����
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
//��Դ������
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
//��ʼ��
bool init(P_list &p_ready, R_list &r, P_list &p_block){
	if(init_P_list(p_ready) && init_R_list(r) && init_P_list(p_block))
		return true;
	else
		return false;
}
//��������
bool create_p(P_list &p, R_list &r, int priv, int PID, int res_id){
	if(r.front == r.rear){
		printf("û����Դ���ã�");
		return false;
	}
	R_Node *q;
	q = r.front->next_r;
	while(1){
		if(res_id == q->RID){
			if(q->stat == 0){//�����Դ����
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
	printf("�Ƿ�Ҫ�����½��̣���y/n��: ");
	scanf("%c", &c);
	if(c == '\n')
		scanf("%c", &c);
	getchar();
//	printf("%c", c);
	if(c == 'y'){
		printf("���������ID(������)�����ȼ�(������)�����������Դid(1-5)������1, 2, 3:\n");
		scanf("%d, %d, %d", &PID, &priv, &res_id);
		create_p(p_ready, r, priv, PID, res_id);
	}
}
/*
��Դ���:
1�����ж���Դ�Ƿ�����������
2�������������������Դ���У������������е������̻�ȡ��Դ���������������
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
���̵���:
1�������������У������ȼ���ߵķ��ڶ��п�ͷ
2�������������У������ǰ������ȼ��Ľ���С�����������н������ȼ������ֵ�����������еĸý��̵���������еĿ�ͷ
3���жϾ������е�������ȼ��뵱ǰ����ִ�еĽ��̵����ȼ��������ֵ���ڵ���3��������ִ�еĽ���������ִ��������ȼ��Ľ��̣�
�������������̵����ȼ�����1
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
			printf("��Դ%d������\n", p->PID);
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
ִ�н���:
1��ÿ�δӾ���������ѡȡ��һ��ִ��
2��ÿ4��ѭ��Ĭ��ִ��һ������
3��ÿ��ѭ���������������еĽ������ȼ���һ
4��ÿ��ѡ����һ��ִ�еĽ���֮ǰ����Ҫִ�н��̵���
5��ÿ��ѭ���������ѡ���Ƿ񴴽��½���
6��ÿ������ִ�н�����Ҫ�ͷ���Դ
7��ÿ��ѭ����Ҫִ����Դ���
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
			printf("��ǰ����ִ�г����PIDΪ%d\n", q->PID);
		
		temp = p_block.front->next_p;
		while(temp!=NULL){
			temp->priv++;
			temp = temp->next_p;
		}
		
		//����û�����
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
	//�����д���
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
	P_list p_ready;//��������
	R_list r;//��Դ����
	P_list p_block;//��������
	P_Node *run = NULL;//�������еĽ���
	if(!init(p_ready, r, p_block)){
		printf("��ʼ��ʧ�ܣ��˳����򡣡���");
		exit(0);
	}
	//��ʼ���������Դ����CPU��
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


	/*��ⴴ�����̺���
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



