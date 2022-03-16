#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
	int data;
	struct node *pNext;
}linklist_d;
/**
*
*
*
*/
struct node *create_node(int a){
	struct node *p = (struct node *)malloc(sizeof(struct node));
	if(p == NULL){
		printf("create node failed!\n");
		return NULL;
	}

	//clear node
	memset(p,0,sizeof(struct node));
	p->data = a;
	p->pNext = NULL;

	return p;
}

void lseek_list(struct node *pHeader){

	struct node *p = pHeader->pNext;
	do{
		printf("%d\t",p->data);
		p = p->pNext;
	}while(NULL != p );
}
/*尾插*/
void insert_tail(struct node *pHeader,struct node *node){
	
	struct node *p = pHeader;
	while(p->pNext != NULL){
		p = p->pNext; 
	}

	p->pNext = node;

	pHeader->data +=1;


}
/*头插*/
void insert_head(struct node *pHeader,struct node *node){
	
	struct node *p = pHeader;

	node->pNext = p->pNext;
	p->pNext = node;

	pHeader->data +=1;
}
/**
*
* addr -- 待插入的下标
* str  -- head -> 前插 or 后插
*/
void insert_n(struct node *pHeader,struct node *node,int addr,char *str){
	
	struct node *p = pHeader;
	if(addr < 1){
		printf("insert %d failed! addr must >= 1 \n",addr);
	}else if(addr > pHeader->data){
		printf("insert %d failed! addr must <= %d\n",addr ,pHeader->data);
	}else {
		if(NULL !=strstr(str,"head")){
			addr--;
		}

		for (int i = 1; i <= addr; i++)
		{
			p = p->pNext;
		}

		node->pNext = p->pNext;
		p->pNext = node;
		pHeader->data += 1;
	}
}

void delate_list(struct node *pHeader,int addr){

	struct node *p = pHeader;
	struct node *dela = NULL;

	if(pHeader->data < addr){
		printf("delate %d failed! (%d , %d)\n",addr,1,pHeader->data);
	} else {
		while(addr-- > 1){
			p = p->pNext;
		}
		dela = p->pNext;
		p->pNext = p->pNext->pNext;
		free(dela);
		pHeader->data -=1;

	}
}


void reverse_order_list(struct node *pHeader){
	struct node *p = pHeader->pNext;
	struct node *pBack = NULL;


	if(p == NULL || p->pNext == NULL){

	} 

	while(NULL != p){

		pBack = p->pNext;
		if(p == pHeader->pNext){
			p->pNext = NULL;
		} else {
			p->pNext = pHeader->pNext;
		}
		pHeader->pNext = p;

		p = pBack;
	}
	
}

int main(int argc, char const *argv[])
{
	int count = 3;
	/*定义头结点*/
	struct node *pHeader = create_node(0);
	for (int i = 1; i <= count; i++)
	{
		struct node *p = create_node(i);
		insert_tail(pHeader,p);
	}
	lseek_list(pHeader);
//	printf("%d.\n",pHeader->data);
	printf("\n**************************************\n");
	reverse_order_list(pHeader);
	lseek_list(pHeader);
	printf("\n**************************************\n");
	// delate_list(pHeader,100);
	// // insert_head(pHeader,create_node(11));
	// lseek_list(pHeader);
	// printf("%d.\n",pHeader->data);
	// printf("\n**************************************\n");
	
	// insert_n(pHeader,create_node(33333),count-10,"head");
	// lseek_list(pHeader);
	// printf("%d.\n",pHeader->data);



	return 0;
}