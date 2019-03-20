#include"process.h"

int end=1;
//生成m-n之间的随机数
int yield_random(int m,int n){
	int tmp;
	srand((unsigned) time(NULL));
	return tmp=rand()%(n-m+1)+m;
}
void menu()
{
	printf("--------------------------\n");
	printf("1.创建进程\n");
	printf("2.删除进程\n");
	printf("3.阻塞进程\n");
	printf("4.激活进程\n");
	printf("5.单步运行\n");
	printf("6.全速运行\n");
	printf("7.退出\n");
	printf("--------------------------\n");
}
void CreateProcess(priority_queue<PCB> &pList,int n)   //创建就绪链表
{
	pcb q;
	printf("自动创建？y/n\n");
	char c;
	getchar();//吸收换行
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
			q->status=就绪;
			q->next=NULL;
			pList.push(*q);
		}
		return ;
	}
	printf("请按顺序输入进程信息：编号\t进程名\n");
	for (int i=0;i<n;i++)
	{
		q = (pcb)malloc(sizeof(PCB));
		q->name=(char *)malloc(20);
		printf("请输入第%d个进程信息：",i);
		scanf("%d %s",&(q->num),q->name);
		q->needtime=yield_random(1,10);
		q->priority=yield_random(1,10);
		q->status=就绪;
		q->next=NULL;
		pList.push(*q);
	}
}

void GetInfo(priority_queue<PCB>await,priority_queue<PCB>block,priority_queue<PCB>excinct)//获取时间片信息
{
	printf("当前时间片所有进程信息:\n");
	while(!await.empty()){
		printf("名称\t编号\t需时\t优先级\t状态\n");
		printf("%s\t%d\t%d\t%d\t%d\n",await.top().name,await.top().num,await.top().needtime,await.top().priority,await.top().status);
		await.pop();
	}
	while(!block.empty()){
		printf("名称\t编号\t需时\t优先级\t状态\n");
		printf("%s\t%d\t%d\t%d\t%d\n",block.top().name,block.top().num,block.top().needtime,block.top().priority,block.top().status);
		block.pop();
	}
	while(!excinct.empty()){
		printf("名称\t编号\t需时\t优先级\t状态\n");
		printf("%s\t%d\t%d\t%d\t%d\n",excinct.top().name,excinct.top().num,excinct.top().needtime,excinct.top().priority,excinct.top().status);
		excinct.pop();
	}
	printf("打印结束!\n");
}

int delete_process(priority_queue<PCB>&await,priority_queue<PCB>&excinct){
	//此函数导致当前运行进程必然消亡
	PCB p;
	if(!await.empty())
		p=await.top();
	else return -1;
	p.status=消亡;
	excinct.push(p);
	await.pop();
	return 0;
}
int block_process(priority_queue<PCB>&await,priority_queue<PCB>&block){
	//此函数导致当前运行进程必然阻塞
	PCB p;
	if(!await.empty())
		p=await.top();
	else return -1;
	p.status=阻塞;
	block.push(p);
	await.pop();
	return 0;
}
int active_process(priority_queue<PCB>&block,priority_queue<PCB>&await){
	//此函数导致最高优先级阻塞进程必然就绪
	PCB p;
	if(!block.empty())
		p=block.top();
	else return -1;
	p.status=就绪;
	await.push(p);
	block.pop();
	return 0;
}

void Runing(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct,int step,int end)//运行过程
{
	//run
	while((!await.empty())||
						(!block.empty())&&
										end){
		PCB p;
		if(!await.empty())
			p=await.top();
		else goto active;//进程全部阻塞，提前激活进程
		p.priority--;
		p.needtime--;
		if(p.needtime==0){//excinct
			p.status=消亡;
			p.priority=0;
			excinct.push(p);
			await.pop();
		}
		else if(yield_random(1,10)>5){//block
			p.status=阻塞;
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
				q.status=阻塞;
				await.push(q);
				block.pop();
			}
		}
		GetInfo(await,block,excinct);//打印
		if(step)//调度
			idle(await,block,excinct);
	}
	return ;
}

int idle(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct){
	int item;
	menu();
	scanf("%d",&item);
	switch(item){
		case 1:int n;//进程数
				printf("请输入进程数 ：");
				scanf("%d",&n);CreateProcess(await,n);break;
		case 2:delete_process(await,excinct);printf("当前进程已放入消亡队列!\n");break;
		case 3:block_process(await,block);printf("当前进程已放入阻塞队列!\n");break;
		case 4:active_process(block,await);printf("阻塞进程已放入就绪队列!\n");break;
		case 5:if((await.empty())&&(block.empty())){printf("运行结束!\n");system("pause");exit(0);}
				else Runing(await,block,excinct,1,1);break;
		case 6:if((await.empty())&&(block.empty())){printf("运行结束!\n");system("pause");exit(0);}
				else Runing(await,block,excinct,0,1);break;
		case 7:Runing(await,block,excinct,0,0);printf("运行结束!\n");system("pause");exit(0);break;
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
