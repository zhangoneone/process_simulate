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
	friend bool operator <(PCB p1,PCB p2){return p1.priority<p2.priority;};//���ִ�����ȼ���
}PCB,*pcb;//PCB�ṹ��

enum process_status{
	���� = 0,
	���� = 1,
	���� = 2,
	���� = 3
};
int idle(priority_queue<PCB>&await,priority_queue<PCB>&block,priority_queue<PCB>&excinct);