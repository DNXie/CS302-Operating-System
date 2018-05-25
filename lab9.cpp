#include <string>
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm>  

using namespace std;

void fcfs(vector<int> request, int init_post);
void sstf(vector<int> request, int init_post, int tail);
void scan(vector<int> request, int init_post);
void c_scan(vector<int> request, int init_post, int tail);
void look(vector<int> request, int init_post);
void c_look(vector<int> request, int init_post);
void push_if_not_exist(vector<int> *list, int value);
void display(vector<int> *result, string title, int offset);

int main(){
	string input_buffer;

	int init_post; //initial position
	int tail;  //track tail
	vector<int> request;  //request list

	int max_input_len;
	int input_count = 0;  


	getline(cin, input_buffer, ' ');
	init_post = atoi(input_buffer.c_str());
	getline(cin, input_buffer, ' ');
	tail = atoi(input_buffer.c_str())-1;
	cin>>max_input_len;

	//read requests
	while(input_count<max_input_len){
		getline(cin, input_buffer, ' ');
		request.push_back(atoi(input_buffer.c_str()));
		input_count+=1;
	}

	fcfs(request, init_post);
	sstf(request, init_post, tail);
	scan(request, init_post);
	c_scan(request, init_post, tail);
	look(request, init_post);
	c_look(request, init_post);
	return 0;
}

void fcfs(vector<int> request, int init_post){
	//first come first serve algorithm
	request.insert(request.begin(), init_post);
	display(&request, "FCFS", 0);
}
void sstf(vector<int> request, int init_post, int tail){
	//shortest seek time first algorithm
	vector<int> result;
	int cur = init_post;  //last examined 
	result.push_back(init_post);
	int min_dist;
	int min_dist_elem;
	int index;
	while(!request.empty()){
		min_dist = tail;
		for(int i=0; i<request.size(); i++){
			if(abs(request[i]-cur)<min_dist){
				min_dist = abs(request[i]-cur);
				min_dist_elem = request[i];
				index = i;
			}
		}
		request.erase(request.begin()+index);
		cur = min_dist_elem;
		if(min_dist!=0){
			result.push_back(min_dist_elem);
		}
	}
	display(&result, "SSTF", 0);
}
void scan(vector<int> request, int init_post){
	//elevator algorithm
	push_if_not_exist(&request, init_post);
	push_if_not_exist(&request, 0);
	//request.push_back(tail);
	vector<int> result;
	sort(request.begin(), request.end());
	int post = find(request.begin(), request.end(), init_post)-request.begin();
	for(int i=post; i>=0; i--){
		result.push_back(request[i]);
	}
	for(int i=post+1; i<request.size();i++){
		result.push_back(request[i]);
	}
	display(&result, "SCAN", 0);

}


void c_scan(vector<int> request, int init_post, int tail){
	//Restricts scanning to one direction only
	push_if_not_exist(&request, init_post);
	push_if_not_exist(&request, 0);
	push_if_not_exist(&request, tail);
	vector<int> result;
	sort(request.begin(), request.end());
	int post = find(request.begin(), request.end(), init_post)-request.begin();
	for(int i=post; i>=0; i--){
		result.push_back(request[i]);
	}
	for(int i=request.size()-1; i>post; i--){
		result.push_back(request[i]);
	}
	display(&result, "C-SCAN", -1*tail);
}

void look(vector<int> request, int init_post){
	//The magnetic arm only moves to the furthest requested position.
	push_if_not_exist(&request, init_post);
	//request.push_back(tail);
	vector<int> result;
	sort(request.begin(), request.end());
	int post = find(request.begin(), request.end(), init_post)-request.begin();
	for(int i=post; i>=0; i--){
		result.push_back(request[i]);
	}
	for(int i=post+1; i<request.size();i++){
		result.push_back(request[i]);
	}
	display(&result, "LOOK", 0);

}

void c_look(vector<int> request, int init_post){
	//look algorithm with only one direction
	push_if_not_exist(&request, init_post);
	vector<int> result;
	sort(request.begin(), request.end());
	int post = find(request.begin(), request.end(), init_post)-request.begin();
	for(int i=post; i>=0; i--){
		result.push_back(request[i]);
	}
	for(int i=request.size()-1; i>post; i--){
		result.push_back(request[i]);
	}
	display(&result, "C-LOOK", -1*(request[request.size()-1]-request[0]));
}

void push_if_not_exist(vector<int> *list, int value){
	//push value back to the list if it doesn't exist
	vector<int>::iterator it = find(list->begin(), list->end(), value);
	if(it==list->end()){
		//not exist
		list->push_back(value);
	}
}
void display(vector<int> *result, string title, int offset){
	//display the visit order and the total distance
	int dist = 0;

	cout<<title<<endl;
	for(int i = 0; i<result->size(); i++){
		cout<<result->at(i);
		if(i!=result->size()-1){
			cout<<" -> ";
			dist += abs(result->at(i)-result->at(i+1));
		}
	}
	dist += offset;
	cout<<endl;
	cout<<dist<<endl;
}
