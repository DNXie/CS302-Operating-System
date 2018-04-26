#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[123];
int len_algo = 0; 
int algo2use;

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size, int start_addr); //空闲块初始化
allocated_block *init_allocated_block(int mem_size, int start_addr);
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
allocated_block *allocate_mem(int required_memsize);
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法
void init_algo();
void set_algo();
allocated_block *first_fit_algo(int required_memsize);
allocated_block *best_fit_algo(int required_memsize);
allocated_block *allocation_precess(free_block *prev, free_block *block2allocate, int required_memsize);
void free_all();
void free_free_list();
void free_allocated_list();
void concatate();
//主函数
int main(){
	int op;
	pid = 0;
	algo2use = 0; //default algorithm is first fit
	free_block_head = init_free_block(mem_size, DEFAULT_MEM_START); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	init_algo();
	for(;;){
		//sleep(1);
		display_menu();
		
		//cin.ignore(1);
		fflush(stdin);
		cin>>op;
		if(cin.fail()){
			cin.clear();
			cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 
		}
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ set_algo(); break; }
			case 3:{ create_new_process(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); free_all();return 0; }
			defaut: break;
		}
	}
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	allocated_block *temp = allocated_block_head;
	while(temp!=NULL){
		if(temp->pid==id)
			return temp;  //block found
		temp=temp->next;
	}
	return NULL; //nothing found
}

free_block *init_free_block(int mem_size, int start_addr){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = start_addr;
	p->next = NULL;
	return p;
}
allocated_block *init_allocated_block(int mem_size, int start_addr){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	allocated_block *p;
	p = (allocated_block *)malloc(sizeof(allocated_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = start_addr;
	strcpy(p->process_name,"");
	p->data = NULL;
	p->next = NULL;
	return p;
}


void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}
void init_algo(){
	algo[0]=make_pair(0,"First Fit");
	algo[1]=make_pair(1,"Best Fit");
	len_algo=2;
}
void set_algo(){
	int algo_index;
	cout<<"Algorithm Available:"<<endl;
	cout<<"Algorithm Index\t"<<"Algorithm Name"<<endl;

	//display all algorithms
	int i=0;
	while(i<len_algo){
		cout<<algo[i].first<<'\t'<<algo[i].second<<endl;
		i++;
	}
	//input
	cout<<"Please input the algoritm index:"<<endl;
	cin>>algo_index;
	if(cin.fail()){
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 
		cout<<"Wrong Input!"<<endl; 
		return;
	}
	if(algo_index<i && algo_index>=0){
		algo2use=algo_index;
		cout<<"Set Algorithm Successfully!"<<endl;
		return;
	}
	
	cin.clear();
	cout<<"Index out of bound"<<endl;
}
allocated_block *first_fit_algo(int required_memsize){
	free_block *temp = free_block_head;
	free_block *prev = NULL;
	free_block *block2allocate = NULL;
	//find the first block that has enough size
	while(temp!=NULL){
		if(temp->size>=required_memsize){
			block2allocate = temp;
			break;
		}
		else{
			prev = temp;
			temp=temp->next;
		}
	}
	if(block2allocate!=NULL)
		return allocation_precess(prev, block2allocate, required_memsize);
		
	else{// all blocks are too small
		cout<<"No memory left! Try smaller one!"<<endl;
		return NULL;
	}
}

allocated_block *best_fit_algo(int required_memsize){
	free_block *temp = free_block_head;
	free_block *prev = NULL;
	free_block *best2fit = NULL;
	int least_waste=DEFAULT_MEM_SIZE;
	while(temp!=NULL){
		if(temp->size-required_memsize < least_waste){
			best2fit = temp;
			least_waste = temp->size-required_memsize;
		}
		if(temp->next!=NULL)
			prev = temp;
		temp=temp->next;
	}
	if (best2fit!=NULL)
		return allocation_precess(prev, best2fit, required_memsize);
	else{
		cout<<"No memory left! Try smaller one!"<<endl;
		return NULL;
	}
}

allocated_block *allocation_precess(free_block *prev, free_block *block2allocate, int required_memsize){
	free_block *next = block2allocate->next; //could be NULL
	int allocate_size;//size to allocate
	//refresh free block list
	if(block2allocate->size - required_memsize >= MIN_SLICE){
		allocate_size = required_memsize;
		free_block *new_slice = init_free_block(block2allocate->size - required_memsize, (block2allocate->start_addr)+required_memsize);
		if(prev!=NULL) // prev is NULL when the block2allocate is the head
			prev->next = new_slice;
		else
			free_block_head = new_slice;
		new_slice->next = next;
	}
	else{
		allocate_size = block2allocate->size;
		if(prev!=NULL)// prev is NULL when the block2allocate is the head
			prev->next = next;
		else
			free_block_head = next;
	}
	//refresh allocated block list
	allocated_block *ab = init_allocated_block(allocate_size, block2allocate->start_addr);
	if(allocated_block_head==NULL){
		allocated_block_head = ab;
	}
	else{
		allocated_block *temp = allocated_block_head;
		while(temp->next!=NULL){
			temp = temp->next;
		}
		temp->next = ab;
	}

	ab->pid=pid+1;
	pid+=1;
	return ab;

}
allocated_block *allocate_mem(int required_memsize){ //为块分配内存，真正的操作系统会在这里进行置换等操作
	//get memory size
	allocated_block *ab;
	
	if(algo2use==0)
		ab = first_fit_algo(required_memsize);
	else 
		ab = best_fit_algo(required_memsize);
	return ab;//could be NULL
}

int create_new_process(){ //创建新进程

	int required_memsize = 0;
	cout<<"Please input the memory size you need:"<<endl;
	cin>>required_memsize;
	if(cin.fail()){
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 
		cout<<"Wrong Input!"<<endl;
		return 0;
	}
	if(required_memsize<=0){
		cout<<"Memory size should be larger than zero."<<endl;
		return 0;
	}
	allocated_block *ab = allocate_mem(required_memsize);
	if(ab==NULL){
		cout<<"Memory allocation failed!"<<endl;
		return 0;
	}

	cout<<"Memory allocated!"<<endl;
	//get process name
	cout<<"Please input the memory name:"<<endl;
	cin.clear();
	cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 	
	if(!cin.getline(ab->process_name, PROCESS_NAME_LEN)){
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 	
	}
	// if(cin.fail()){
	// 	cout<<"Wrong Input!"<<endl;
	// 	return 0;
	// }
	flag = 1;
	//cin.getline(ab->process_name, PROCESS_NAME_LEN);
	return 1;

}

void set_mem_size(){ //更改最大内存大小
	if(flag){
		//Memory has been allocated. mem_size cannot be modified.
		cout<<"Memory has been allocated. mem_size cannot be modified."<<endl;
		return;
	}
	int input_memsize;
	cout<<"Please input the memory size:"<<endl;
	scanf("%d", &input_memsize);
	if(input_memsize>0){
		mem_size=input_memsize;
		free_block_head->size = input_memsize;
	}
	else{
		cout<<"Memory size should be larger than 0."<<endl;
	}
}



void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange(){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (tmpx->start_addr < tmp->start_addr){
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}

void concatate(){
	//将地址相连的free block合并
	free_block *prev = free_block_head;
	free_block *cur = prev->next;
	while(cur!=NULL){
		if(prev->start_addr+prev->size==cur->start_addr){
			//concatate
			prev->size += cur->size;
			prev->next = cur->next;
			free(cur);
			cur = prev->next;
		}else{
			prev = cur;
			cur = cur->next;
		}
	}
}


int free_mem(allocated_block *ab){ //释放某一块的内存
	free_block *fb = init_free_block(ab->size, ab->start_addr);
	//free_block *temp = free_block_head;
	if(free_block_head!=NULL){
		//put the block anywhere possible, the order doesn't matter
		fb->next = free_block_head->next;
		free_block_head->next = fb;
	}
	else
		free_block_head = fb ;
	// while(temp->next!=NULL){
	// 	temp = temp->next;
	// }
	// temp->next = fb;
	rearrange();
	concatate();
	return 1;
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		//free(fab->process_name);
		free(fab);
		fab=NULL;
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;

	ab->next= NULL;
	free(ab);
	ab=NULL;
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	cout<<"*********************Free Memory*********************"<<endl;
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) cout<<"No Free Memory"<<endl;
	else printf("Totaly %d free blocks\n", cnt);
	cout<<endl;
	cout<<"*******************Used Memory*********************"<<endl;
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		if(ab->pid<pid){
			ab = ab->next;
		}
		else
			break;
	}

	if (!cnt) cout<<"No allocated block"<<endl;
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	allocated_block *ab;
	int kill_pid;
	puts("Please input the pid of Killed process");
	cin>>kill_pid;
	if(cin.fail()){
		cin.clear();
		cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 
		cout<<"Wrong Input"<<endl;
		return;
	}
	if(kill_pid>pid||kill_pid<0){
		cout<<"Index out of bound"<<endl;
		return;
	}
	ab = find_process(kill_pid);
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}
	else{
		cout<<"No such process!"<<endl;
		return;
	}
}

void free_all(){
	//on exit free all memory allocated
	free_free_list();
	free_allocated_list();
}
void free_free_list(){
	//free free_block_list
	free_block *cur = free_block_head->next;
	while(cur!=NULL){
		free_block_head->next = cur->next;
		cur->next = NULL;
		free(cur);
		cur = free_block_head->next;
	}
	free(free_block_head);
	free_block_head=NULL;
}
void free_allocated_list(){
	//free allocated_block_list
	if (allocated_block_head==NULL){
		free(allocated_block_head);
		return;
	}
	allocated_block *cur = allocated_block_head->next;
	while(cur!=NULL){
		allocated_block_head->next = cur->next;
		cur->next = NULL;
		free(cur);
		cur = allocated_block_head->next;
	}
	free(allocated_block_head);
	allocated_block_head=NULL;
	
}

