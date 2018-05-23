#include <iostream>
#include <list>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <vector>


using namespace std;

int FIFO(int max_input_len, int cache_size);
int LRU(int max_input_len, int cache_size);
int Min(int max_input_len, int cache_size);
int second_chance(int max_input_len, int cache_size);
int clock(int max_input_len, int cache_size);
int insert_into_FIFO(list<int> *fifo_list, int value, int max_len);
int insert_into_LRU(list<int> *fifo_list, int value, int max_len);

int main(){

	//read from input
	int cache_size;
	cin>>cache_size;
	int option;
	cin>>option;
	int max_input_len;
	cin>>max_input_len;

	int hit = 0;
	switch (option){
		case 0:{ hit = FIFO(max_input_len, cache_size); break; }
		case 1:{ hit = LRU(max_input_len, cache_size); break; }
		case 2:{ hit = Min(max_input_len, cache_size); break; }
		case 3:{ hit = clock(max_input_len, cache_size); break; }
		case 4:{ hit = second_chance(max_input_len, cache_size);break; }
		defaut: break;
	}
	printf("Hit ratio = %.2f%% \n", ((hit*1.0*10000)/(max_input_len*10000))*100);
}
int clock(int max_input_len, int cache_size){
	int input_count = 0;
	string input_str;
	int input_int;

	vector<int> cache;
	vector<int> valid;
	int hand = 0;
	int hit = 0;

	vector<int>::iterator it;
	int dist;
	while(input_count<max_input_len){
		getline(cin, input_str, ' ');
		input_int = atoi(input_str.c_str());
		it = find(cache.begin(), cache.end(), input_int);
		if(it == cache.end()){
			//miss
			if(cache.size() == cache_size){
				// cache is full
				while(1){
					if(valid[hand]==0){
						//replace
						cache[hand] = input_int;
						valid[hand] = 1;
						hand = hand<cache.size()-1 ? hand+1 : 0;
						break;
					}
					valid[hand] = 0;
					hand = hand<cache.size()-1 ? hand+1 : 0;
				}//end while
			}//end if
			else{
				//cache is not full
				cache.push_back(input_int);
				valid.push_back(1);
				hand  = hand<cache_size-1 ? hand+1 : 0;
			}
		}
		else{
			//hit
			dist = distance(cache.begin(), it);
			valid[dist] = 1;
			hit+= 1;
		}
		input_count += 1;

	}
	return hit;
}

int FIFO(int max_input_len, int cache_size){
	int input_count = 0;
	string input_str;
	int input_int;
	list<int> cache;

	int hit = 0;

	while(input_count<max_input_len){
		getline(cin, input_str, ' ');
		input_int = atoi(input_str.c_str());
		hit += insert_into_FIFO(&cache, input_int, cache_size);
		input_count+=1;	
	}
	return hit;
}
int LRU(int max_input_len, int cache_size){
	int input_count = 0;
	string input_str;
	int input_int;
	list<int> cache;
	
	int hit = 0;

	while(input_count<max_input_len){
		getline(cin, input_str, ' ');
		input_int = atoi(input_str.c_str());
		hit += insert_into_LRU(&cache, input_int, cache_size);
		input_count+=1;		
	}
	return hit;
}

int Min(int max_input_len, int cache_size){
	int input_count = 0;
	string input_str;
	int input_int;
	int pages[max_input_len];
	list<int> cache;

	int hit = 0;
	list<int>::iterator it;

	int farthest_page = 0;
	int farthest_distance = 0;
	int j;
	//read all the pages and store them all in int array "pages"
	for (int i=0; i<max_input_len; i++){
		getline(cin, input_str, ' ');
		input_int = atoi(input_str.c_str());
		pages[i] = input_int;
	}

	for(int i=0; i<max_input_len; i++){
		it = find(cache.begin(), cache.end(), pages[i]);
		if(it != cache.end()){
			// hit
			hit+=1;
		}
		else{
			//miss
			if(cache.size() >= cache_size){
				// miss and full, have to give up an element
				farthest_distance = i;
				for(auto &c : cache){
					//for each element c in cache 
					
					for(j=i; j<max_input_len; j++){
						//for each future input 
						if(pages[j]==c){
							if(j>farthest_distance){
								farthest_distance = j;
								farthest_page = c;
							}
							break;
						}
					}
					if(j==max_input_len){
						//the page is never been referenced in the future
						farthest_page = c;
						break;
					}
				}//end prediction

				// remove the farthest page that would be used
				cache.remove(farthest_page);
			}//end else (miss and full)
			cache.push_back(pages[i]);
		}//end else (miss)
	}

	return hit;
}

int second_chance(int max_input_len, int cache_size){
	int input_count = 0;
	string input_str;
	int input_int;
	list<int> fifo;
	list<int> lru;

	int fifo_size = (int)cache_size/2;
	int lru_size = cache_size - fifo_size;

	int hit = 0;
	list<int>::iterator it;

	int temp_front = 0;
	int temp_size = 0;

	while(input_count<max_input_len){
		getline(cin, input_str, ' ');
		input_int = atoi(input_str.c_str());
		it = find(lru.begin(), lru.end(), input_int);
		temp_front = fifo.front();
		temp_size = fifo.size();
		if(it != lru.end()){
			//found in LRU
			lru.remove(input_int);
			hit+=1;		
			insert_into_FIFO(&fifo, input_int, fifo_size);	
		}
		else{
			// not in LRU
			if(1==insert_into_FIFO(&fifo, input_int, fifo_size)){
				//hit in fifo
				hit+=1;
				temp_size = 0;
			}
		}
		if(temp_size == fifo_size)
			//miss in fifo and fifo was full, which means an element has been poped
			insert_into_LRU(&lru, temp_front, lru_size);
		
		input_count+=1;

	}
	return hit;

}

int insert_into_FIFO(list<int> *fifo_list, int value, int max_len){

	list<int>::iterator it = find(fifo_list->begin(), fifo_list->end(), value);
	if(it == fifo_list->end()){ 			//miss
		// if it's not found
		
		if(fifo_list->size()>=max_len)  			// cache is full
			fifo_list->pop_front();
		fifo_list->push_back(value);
		return 0;
	}
	return 1;							//hit
}

int insert_into_LRU(list<int> *fifo_list, int value, int max_len){
	list<int>::iterator it = find(fifo_list->begin(), fifo_list->end(), value);
	if(it == fifo_list->end()){ 			//miss
		// if it's not found
		if(fifo_list->size()>=max_len)   	// cache is full
			fifo_list->pop_front();

		fifo_list->push_back(value);	
		return 0;
	}
	else{						 	 //hit
		fifo_list->remove(value);
		fifo_list->push_back(value);
		return 1;
	}
}