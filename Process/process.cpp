#include"process.h"

int end=1;
//����m-n֮��������
int yield_random(int m,int n){
	int tmp;
	srand((unsigned) time(NULL));
	return tmp=rand()%(n-m+1)+m;
}
void menu()
{
	printf("--------------------------\n");
	printf("1.��������\n");
	printf("2.ɾ������\n");
	printf("3.��������\n");
	printf("4.�������\n");
	printf("5.��������\n");
	printf("6.ȫ������\n");
	printf("7.�˳�\n");
	printf("--------------------------\n");
}
void CreateProcess(priority_queue<PCB> &pList,int n)   //������������
{
	pcb q;
	printf("�Զ�������y/n\n");
	char c;
	getchar();//���ջ���
	scanf("%c",&c);
	if(c=='y'){
		for(int i=0;i<n;i++){
			q = (pcb)malloc(sizeof(PCB));
			q->name=(char *)malloc(20);
			q->num=i;
			char s[10];
			s[0]='a'+yield_random(0,25);
			s[1]='a'+yield_random(0,25);
			s[2]='a'+yield_random(0,25);
			s[3]='a'+yield_random(0,25);
			s[4]='a'+yield_random(0,25);
			s[5]='\0';
			strcpy(q->name,s);
			q->needtime=yield_random(1,10);
			q->priority=yield_random(1,10);
			q->status=����;
			q->next=NULL;
			pList.push(*q);
		}
		return ;
	}
	printf("�밴˳�����������Ϣ�����\t������\n");
	for (int i=0;i<n;i++)
	{
		q = (pcb)malloc(sizeof(PCB));
		q->name=(char *)malloc(20);
		printf("�������%d��������Ϣ��",i);
		scanf("%d %s",&(q->num),q->name);
		q->needtime=yield_random(1,10);
		q->priority=yield_random(1,10);
		q->status=����;
		q->next=NULL;
		pList.push(*q);
	}
}

void GetInfo(priority_queue<PCB>await,priority_queue<PCB>block,priority_queue<PCB>excinct)//��ȡʱ��Ƭ��Ϣ
{
	printf("��ǰʱ��Ƭ���н�����Ϣ:\n");
	while(!await.empty()){
		printf("����\t���\t��ʱ\t���ȼ�\t״̬\n");
		printf("%s\t%d\t%d\t%d\t%d\n",await.top().name,await.top().num,await.top().needtime,await.top().priority,await.top().status);
		await.pop();
	}
	while(!block.empty()){
		printf("����\t���\t��ʱ\t���ȼ�\t״̬\n");
		printf("%s\t%d\t%d\t%d\t%d\n",block.top().name,block.top().num,block.top().needtime,block.top().priority,block.top().status);
		block.pop();
	}
	while(!excinct.empty()){
		printf("����\t���\t��ʱ\t���ȼ�\t״̬\n");
		printf("%s\t%d\t%d\t%d\t%d\n",excinct.top().name,excinct.top().num,excinct.top().needtime,excinct.top().priority,excinct.top().status);
		excinct.pop();
	}
	printf("��ӡ����!\n");
}

int delete_process(priority_queue<PCB>&await,priority_queue<PCB>&excinct){
	//�˺������µ�ǰ���н��̱�Ȼ����
	PCB p;
	if(!await.empty())
		p=await.top();
	else return -1;
	p.status=����;
	excinct.push(p);
	await.pop();
	return 0;
}
int block_process(priority_queue<PCB>&await,priority_queue<PCB>&block){
	//�˺������µ�ǰ���н��̱�Ȼ����
	PCB p;
	if(!await.empty())
		p=await.top();
	else return -1;
	p.status=����;
	block.push(p);
	await.pop();
	return 0;
}
int active_process(priority_queue<PCB>&block,priority_queue<PCB>&await){
	//�˺�������������ȼ��������̱�Ȼ����
	PCB p;
	if(!block.empty())
		p=block.top();
	else return -1;
	p.status=����;
	await.push(p);
	block.pop();
	return 0;
}

void Runing(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct,int step,int end)//���й���
{
	//run
	while((!await.empty())||
						(!block.empty())&&
										end){
		PCB p;
		if(!await.empty())
			p=await.top();
		else goto active;//����ȫ����������ǰ�������
		p.priority--;
		p.needtime--;
		if(p.needtime==0){//excinct
			p.status=����;
			p.priority=0;
			excinct.push(p);
			await.pop();
		}
		else if(yield_random(1,10)>5){//block
			p.status=����;
			block.push(p);
			await.pop();
		}
		else{//carry on
			await.pop();
			await.push(p);
		}
		//random motivate the block process
active:
		if(yield_random(1,10)>5){//active process
			if(!block.empty()){
				PCB q=block.top();
				q.status=����;
				await.push(q);
				block.pop();
			}
		}
		GetInfo(await,block,excinct);//��ӡ
		if(step)//����
			idle(await,block,excinct);
	}
	return ;
}

int idle(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct){
	int item;
	menu();
	scanf("%d",&item);
	switch(item){
		case 1:int n;//������
				printf("����������� ��");
				scanf("%d",&n);CreateProcess(await,n);break;
		case 2:delete_process(await,excinct);printf("��ǰ�����ѷ�����������!\n");break;
		case 3:block_process(await,block);printf("��ǰ�����ѷ�����������!\n");break;
		case 4:active_process(block,await);printf("���������ѷ����������!\n");break;
		case 5:if((await.empty())&&(block.empty())){printf("���н���!\n");system("pause");exit(0);}
				else Runing(await,block,excinct,1,1);break;
		case 6:if((await.empty())&&(block.empty())){printf("���н���!\n");system("pause");exit(0);}
				else Runing(await,block,excinct,0,1);break;
		case 7:Runing(await,block,excinct,0,0);printf("���н���!\n");system("pause");exit(0);break;
		default:exit(0);break;
	}
	return 1;
}
int main()
{
	pcb p;
	priority_queue<PCB>await;
	priority_queue<PCB>block;
	priority_queue<PCB>excinct;
	while(idle(await,block,excinct));
	return 0;
}
