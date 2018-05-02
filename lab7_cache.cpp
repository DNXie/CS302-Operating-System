#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

#define INPUT_LEN 100
#define ADDR_LEN 7
#define OFFSET_LEN 16
#define CACHE_SIZE 1024
#define NB_LINE CACHE_SIZE/OFFSET_LEN

long int cache[NB_LINE];

long int hex2int(char *hex_addr);
int search_in_cache(long int hex_addr);
void insert_into_cache(long int hex_addr);
int get_index(long int addr);
long int get_tag(long int addr);

int main(){
	int nb_hit = 0;// hit count
	int count = 0;//count of all input. miss count = count-nb_hit
	int hit_history [INPUT_LEN]; //history of hit or miss. hit:1, miss:0
	char input[ADDR_LEN]; //input buffer
	long int hex_input; //input in hex format
	int hit_flag = 0; //if cache hit on the current input
	float hit_ratio = 0.0; //hit ratio
	memset(cache, -1, sizeof(cache)); // initialize all element in cache to -1
	while(1){	
		if(!cin.getline(input, ADDR_LEN+1)){
			//illegal input or input out of bound
			cin.clear();
			cin.ignore( numeric_limits<streamsize>::max(), '\n' ); 	
		}
		else if(!strncmp(input, "END",3))
			break;
		else{
			//legal input and not "END"
			hex_input = hex2int(input);
			hit_flag = search_in_cache(hex_input);
			//if the address doesn't hit and there are still available space in cache. insert a new block in cache
			if(!hit_flag) //miss
				insert_into_cache(hex_input);
			nb_hit += hit_flag;
			hit_history[count]=hit_flag;
			count += 1;
		}
	}	

	//print history of hit and miss
	for(int i=0; i< count; i++){
		if(hit_history[i]==0)
			cout<<"MISS"<<endl;
		else
			cout<<"HIT"<<endl;
	}
	hit_ratio = ((nb_hit*1.0)/count)*100;
	printf("Hit ratio = %.2f%% \n", hit_ratio);


}
long int hex2int(char *hex_addr){
	long int result;
	stringstream ss;
	ss<<hex<<hex_addr;
	ss>>result;
	return result;
}	


int search_in_cache(long int hex_addr){
	long int tag = cache[get_index(hex_addr)];
	return tag == get_tag(hex_addr);
}

void insert_into_cache(long int hex_addr){
	// insert a block into cache
	int index = get_index(hex_addr);
	cache[index] = get_tag(hex_addr);
}

int get_index(long int addr){
	// calculate index given decimal address
	return (addr%CACHE_SIZE)/OFFSET_LEN;
}
long int get_tag(long int addr){
	// calculate tag given decimal address
	return addr/CACHE_SIZE;
}
