#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#include<queue>
#include<vector>
#include<iostream>
using namespace std;
typedef struct PCB
{
	int num;
	char* name;
	int needtime;
	int priority;
	int status;
	struct PCB *next;
	friend bool operator <(PCB p1,PCB p2){return p1.priority<p2.priority;};//数字大的优先级高
}PCB,*pcb;//PCB结构体

enum process_status{
	运行 = 0,
	就绪 = 1,
	阻塞 = 2,
	消亡 = 3
};
int idle(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct);