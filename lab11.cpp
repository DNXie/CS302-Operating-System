#include <vector>
#include <sstream>
#include <iterator>
#include <limits>
#include <iterator>
#include <string>
#include <iostream>
#include <queue>

using namespace std;

struct file;
struct directory;
struct user;

// 删文件夹 关里面的文件


template<typename Out>
void split(const std::string &s, char delim, Out result);
vector<string> split(const std::string &s, char delim);
void init_sys();
string display_path(vector<directory *> *path);
void display_term();
void display_demand();
int process_demand(string demand);
void reg_user(const string & username, const string & password);
void ls(directory *dir);
vector<directory *> parse_path_str(const string & path_str);
void make_dir(directory * parent_dir, const string & dir_name);
void delete_dir(directory * parent_dir, const string & dir_name);
void delete_user(const string & username);
void display_user();
file *init_file(const string & filename, int omode, directory * parent_dir);
directory *init_dir(const string & dir_name);
int find_dir(directory * parent_dir, const string & dir_name);
int find_file(directory * parent_dir, const string & filename);
void login(const string & username, const string & password);
user *init_user(const string & username, const string & password);
int find_user(vector<user *> *user_list,const string & username);
int root_check();
void create_file(vector<directory *> *dir_list, const string & filename, int omode);
void delete_file(directory * parent_dir, const string & filename);
void chmod(directory * parent_dir, const string & filename, int new_mode);
void cat(directory * parent_dir, const string & filename);
void open_file(directory * parent_dir, const string & filename);
void close_file(directory * parent_dir, const string & filename);
void lsfile();
void truncate(directory * parent_dir, const string & filename, int cnt);
void reposition(directory * parent_dir, const string & filename, int new_pos);
void write_file(directory * parent_dir, const string & filename, string buff, int wmode);
void search_file(const string & filename);
int permission_check(file *f, int mode);

struct file {
	string filename;
	string path;
	directory * parent_dir;
	string content;
	int omode; // 1 exe, 2 write, 4 read 
	int ptr;  //position
	bool if_open;
	user * creater;
	user * opener;
};

struct directory {
	string dir_name;
	vector<directory *> subdir_list;
	vector<file *> file_list;
};

struct user {
	string username;
	string password;
};

user *cur_user;
vector<directory *> cur_path;
vector<user *> user_list;
directory * root_dir;
vector<file *> opened_file;

int main(){
	init_sys();
	string demand;
	while(1){
		display_term();
		getline(cin, demand);
		if(-1 == process_demand(demand))
			break;
		
	}
}

void init_sys(){
	user *root = init_user("root", "111");
	user_list.push_back(root);
	cur_user = root;
	
	root_dir = init_dir("");
	cur_path.push_back(root_dir);
}
int process_demand(string demand){
	vector<string> tokens = split(demand, ' ');


	if(tokens[0].compare("man") == 0){
		display_demand();
	}
	else if(tokens[0].compare("pwd") == 0){
		cout<<display_path(&cur_path);
		cout<<endl;
	}
	else if(tokens[0].compare("mkdirp") == 0){
		
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		make_dir(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("mkdir") == 0){
		make_dir(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("dedirp") == 0){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		delete_dir(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("dedir") == 0){
		delete_dir(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("cdp") == 0){
		cur_path = parse_path_str(tokens[1]);
	}
	else if(tokens[0].compare("cd") == 0){
		int index= find_dir(cur_path.back(), tokens[1]);
		cur_path.push_back(cur_path.back()->subdir_list[index]);
	}
	else if(tokens[0].compare("lsp") == 0){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		ls(dir_obj_list.back());
	}
	else if(tokens[0].compare("ls") == 0){
		ls(cur_path.back());
	}
	else if(tokens[0].compare("regusr") == 0){
	   	reg_user(tokens[1], tokens[2]);
	}
	else if(tokens[0].compare("delusr") == 0){
	   	delete_user(tokens[1]);
	}
	else if(tokens[0].compare("disusr") == 0){
	   	display_user();
	}
	else if(tokens[0].compare("login") == 0){
		if(tokens.size()==2){
			login(tokens[1], "");
		}else
			login(tokens[1], tokens[2]);
	   	
	}
	else if(tokens[0].compare("create")== 0 ){
		create_file(&cur_path, tokens[1], stoi(tokens[2]));
	}
	else if(tokens[0].compare("createp")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		create_file(&dir_obj_list, tokens[2], stoi(tokens[3]));
	}
	else if(tokens[0].compare("delete")== 0 ){
		delete_file(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("deletep")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		delete_file(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("chmod")== 0 ){
		chmod(cur_path.back(), tokens[1], stoi(tokens[2]));
	}
	else if(tokens[0].compare("chmodp")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		chmod(dir_obj_list.back(), tokens[2], stoi(tokens[3]));
	}
	else if(tokens[0].compare("cat")== 0 ){
		cat(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("catp")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		cat(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("open")== 0 ){
		open_file(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("openp")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		open_file(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("close")== 0 ){
		close_file(cur_path.back(), tokens[1]);
	}
	else if(tokens[0].compare("closep")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		close_file(dir_obj_list.back(), tokens[2]);
	}
	else if(tokens[0].compare("truncate")== 0 ){
		truncate(cur_path.back(), tokens[1], stoi(tokens[2]));
	}
	else if(tokens[0].compare("truncatep")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		truncate(dir_obj_list.back(), tokens[2], stoi(tokens[3]));
	}
	else if(tokens[0].compare("reposition")== 0 ){
		reposition(cur_path.back(), tokens[1], stoi(tokens[2]));
	}
	else if(tokens[0].compare("repositionp")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		reposition(dir_obj_list.back(), tokens[2], stoi(tokens[3]));
	}
	else if(tokens[0].compare("write")== 0 ){
		write_file(cur_path.back(), tokens[1], tokens[2], stoi(tokens[3]));
	}
	else if(tokens[0].compare("writep")== 0 ){
		vector<directory *> dir_obj_list = parse_path_str(tokens[1]);
		write_file(dir_obj_list.back(), tokens[2], tokens[3], stoi(tokens[4]));
	}
	else if(tokens[0].compare("lsfile")== 0 ){
		lsfile();
	}
	else if(tokens[0].compare("search")== 0 ){
		search_file(tokens[1]);
	}

	else if(tokens[0].compare("exit") == 0){
		return -1;  //exit the program
	}
	return 0;
}
int permission_check(file *f, int mode){
	return f->omode & mode;
}
void search_file(const string & filename){
	queue<directory *> dir_queue;
	directory * tmp_dir;
	dir_queue.push(root_dir);
	int cnt=0;
	while(!dir_queue.empty()){
		tmp_dir = dir_queue.front();
		dir_queue.pop();
		for(file * f : tmp_dir->file_list){
			if(f->filename == filename){
				cout<<f->path<<filename<<endl;
				cnt += 1;
			}
		}
		for(directory * d : tmp_dir->subdir_list){
			dir_queue.push(d);
		}
	}
	if(cnt == 0)
		cout<<"file does not found"<<endl;
}
void write_file(directory * parent_dir, const string & filename, string buff, int wmode){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		file *f = parent_dir->file_list[index];
		if(f->if_open){
			if(2 == permission_check(parent_dir->file_list[index], 2)){
				if(wmode == 0)      //write from the bottom
					f->content += buff;
				else if (wmode == 1) {    // replace from the file pointer position
					string tmp= f->content.substr(0, f->ptr) + buff ;
					if(f->ptr+buff.length()<f->content.length())
						f->content = tmp += f->content.substr(f->ptr+buff.length());
					else
						f->content = tmp;
				}
				else if (wmode == 2)     // insert from the file pointer position
					f->content = f->content.substr(0, f->ptr) + buff + f->content.substr(f->ptr);

				f->ptr = f->content.length();
				cout<<"write ok"<<endl;
			}
			else
				cout<<"permission denied"<<endl;
		}
		else
			cout<<"file does not open"<<endl;
		
	}
}
void reposition(directory * parent_dir, const string & filename, int new_pos){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		parent_dir->file_list[index]->ptr = new_pos;
		cout<<"reposition ok"<<endl;
	}
}
void truncate(directory * parent_dir, const string & filename, int cnt){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		parent_dir->file_list[index]->content = parent_dir->file_list[index]->content.substr(0, cnt);
		parent_dir->file_list[index]->ptr = parent_dir->file_list[index]->content.length();
	}
}
void lsfile(){
	for (file * f : opened_file){
		cout<<f->path<<" "<<f->filename<<" "<<f->creater->username<<" "<<f->opener->username<<" "<<f->omode<<endl;
	}
}
void close_file(directory * parent_dir, const string & filename){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		if(parent_dir->file_list[index]->if_open == false){
			cout<<"file does not open"<<endl;
		}
		else{
			parent_dir->file_list[index]->if_open = false;
			// find this file in vector 'opened_file' and remove
			vector<file *>::iterator it = find (opened_file.begin(), opened_file.end(), parent_dir->file_list[index]);
			if(it!=opened_file.end())
				opened_file.erase(it);
		}
	}
}
void open_file(directory * parent_dir, const string & filename){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		if(parent_dir->file_list[index]->if_open == true){
			cout<<"file already open"<<endl;
		}
		else{
			parent_dir->file_list[index]->if_open = true;
			opened_file.push_back(parent_dir->file_list[index]);
			parent_dir->file_list[index]->opener = cur_user;
		}
		
	}
}
void cat(directory * parent_dir, const string & filename){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		if(4 == permission_check(parent_dir->file_list[index], 4)){
			if(parent_dir->file_list[index]->if_open)
				cout<<parent_dir->file_list[index]->content<<endl;
			else
				cout<<"file does not open"<<endl;
		}
		else
			cout<<"permission denied"<<endl;
	}
}
void chmod(directory * parent_dir, const string & filename, int new_mode){
	int index = find_file(parent_dir, filename);
	if(-1 == index){
		cout<<"file does not exist"<<endl;
	}
	else{
		parent_dir->file_list[index]->omode = new_mode;
	}
}
void delete_file(directory * parent_dir, const string & filename){
	int index = find_file(parent_dir, filename);
	if(-1 != index){
		//delete it in opened_file
		if(parent_dir->file_list[index]->if_open){
			vector<file *>::iterator it = find (opened_file.begin(), opened_file.end(), parent_dir->file_list[index]);
			if(it!=opened_file.end())
				opened_file.erase(it);
		}

		parent_dir->file_list.erase(parent_dir->file_list.begin() + index);
	}else{
		cout<<"file does not exist"<<endl;
	}
}
int root_check(){
	if(cur_user->username.compare("root") == 0)
		return 1;
	else {
		cout<<"permission denied"<<endl;
		return 0;
	}
}

void create_file(vector<directory *> *dir_list, const string & filename, int omode){
	// create a file with mode in parent_dir
	if(-1 == find_file(dir_list->back(), filename)){
		file * f = init_file(filename, omode, dir_list->back());
		f->path = display_path(dir_list);
		f->creater = cur_user;
		dir_list->back()->file_list.push_back(f);
	}else{
		cout<<"file already exists"<<endl;
	}
}

void make_dir(directory * parent_dir, const string & dir_name){
	int index = find_dir(parent_dir, dir_name);
	if(-1 != index){
		cout<<"directory already exists"<<endl;
	}
	else{
		directory *d = init_dir(dir_name);
		parent_dir->subdir_list.push_back(d);
	}
}
void delete_dir(directory * parent_dir, const string & dir_name){
	// delete directory named dir_name from its parent directory
	int index = find_dir(parent_dir, dir_name);
	if(index==-1){
		cout<<"directory does not exist"<<endl;
	}
	else{
		for(file * f : parent_dir->subdir_list[index]->file_list){
			if (f->if_open){
				vector<file *>::iterator it = find (opened_file.begin(), opened_file.end(), f);
				if(it!=opened_file.end())
					opened_file.erase(it);
			}
		}
		parent_dir->subdir_list.erase(parent_dir->subdir_list.begin() + index);

	}
}
user *init_user(const string & username, const string & password){ 
	//initialize a user
	user *u = new user;
	u->username = username;
	u->password = password;
	return u;
}

int find_user(vector<user *> *user_list,const string & username){
	//find user in user_list, return NULL if user not found
	for (size_t i = 0; i != user_list->size(); ++i){
		if(user_list->at(i)->username.compare(username) == 0){
			return i;
		}
	}
	return -1; // not found
}




void reg_user(const string & username, const string & password){
	if(find_user(&user_list, username) != -1){
		//user exists
		cout<<"user already exists"<<endl;
	}
	else{
		user *u = init_user(username, password);
		if(u!=NULL){
			user_list.push_back(u);
			cout<<"register ok"<<endl;
		}
	}
}
void login(const string & username, const string & password){
	int index = find_user(&user_list, username);
	if(-1 == index){
		cout<<"user does not exist"<<endl;
		return;
	}
	// get password
	cout<<"password:";
	string pw_input;
	if(password.empty()){
		cin>>pw_input;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}else{
		pw_input = password;
	}
	
	if(user_list[index]->password.compare(pw_input) == 0 ){
		cur_user = user_list[index];
		cout<<"login ok"<<endl;
	}
	else{
		cout<<"password wrong"<<endl;
	}
}
void display_user(){
	if(root_check()){
		for (auto u : user_list){
			cout<<u->username<<endl;
		}
	}
}
void delete_user(const string & username){
	if(root_check()){
		int index = find_user(&user_list, username);
		if(index == -1){
			cout<<"user does not exist"<<endl;
		}
		else{
			user_list.erase(user_list.begin()+index);
		}
	}
}
void ls(directory *dir){
	for (directory * d: dir->subdir_list) {
        cout<<d->dir_name<<endl;
    }
    for (file * f: dir->file_list) {
        cout<<f->filename<<endl;
    }
}

file *init_file(const string & filename, int omode, directory * parent_dir){ 
	//initialize a file
	file *f = new file;
	f->filename.assign(filename);
	f->omode = omode;
	f->if_open=false;
	f->parent_dir = parent_dir;
	return f;
}

directory *init_dir(const string & dir_name){ 
	//initialize a file
	// return NULL if fail
	directory *d = new directory;
	d->dir_name.assign(dir_name);
	// subdir_list and file_list are initialized to be empty be default
	return d;
}

int find_dir(directory * parent_dir, const string & dir_name){
	for (size_t i = 0; i != parent_dir->subdir_list.size(); ++i){
		if(parent_dir->subdir_list[i]->dir_name.compare(dir_name)==0){
			//found the dir
			return i;
		}
	}
	return -1; //not found
}

int find_file(directory * parent_dir, const string & filename){
	for (size_t i = 0; i != parent_dir->file_list.size(); ++i){
		if(parent_dir->file_list[i]->filename.compare(filename)==0){
			//found the dir
			return i;
		}
	}
	return -1; //not found
}

vector<directory *> parse_path_str(const string & path_str){
	// convert a path string into a directory linked list
	vector<string> dir_list = split(path_str,'/');

	vector<directory *> dir_obj_list;
	dir_obj_list.push_back(root_dir); //result

	vector<directory *> search_dir_list; //temp dir list for search usage

	for (size_t i = 1; i != dir_list.size(); ++i){
		// for each dir name in the path_str
		// find it in subdir_list in the last element in dir_obj_list 
		search_dir_list = dir_obj_list.back()->subdir_list;
		for(size_t j = 0; j != search_dir_list.size(); ++j){
			if(search_dir_list[j]->dir_name.compare(dir_list[i])==0){
				// find the object
				dir_obj_list.push_back(search_dir_list[j]);
			}
		}
	}
	// for (auto s : dir_obj_list){
	// 	cout<<s->dir_name<<endl;
	// }
	return dir_obj_list;
}
string display_path(vector<directory *> *path){
	//display a path
	string result = "";
	vector<directory *>::iterator it = path->begin();
	while(it!=path->end()){
		result += (*it)->dir_name;
		result += "/";
		it++;
	}
	return result;
	//cout<<endl;
}

void display_term(){
	//display terminal title
	cout<<cur_user->username<<"$";
	cout<<display_path(&cur_path);
	cout<<"%:";
}

void display_demand(){
	cout<<"Manual:"<<endl;
	cout<<"0) pwd (current work directory)"<<endl;
	cout<<"1) mkdir(p) (path) directory_name"<<endl;
	cout<<"2) dedir(p) (path) directory_name"<<endl;
	cout<<"3) search file_name"<<endl;
	cout<<"4) create(p) (path) file_name omode"<<endl;
	cout<<"5) cat(p) (path) file_name"<<endl;
	cout<<"6) delete(p) (path) file_name"<<endl;
	cout<<"7) write(p) (path) file_name buff wmode"<<endl;
	cout<<"8) reposition(p) (path) file_name pos"<<endl;
	cout<<"9) truncate(p) (path) file_name cnt"<<endl;
	cout<<"10) ls(p) (path)"<<endl;
	cout<<"11) open(p) (path) file_name"<<endl;
	cout<<"12) close(p) (path) file_name"<<endl;
	cout<<"13) regusr user_name password"<<endl;
	cout<<"14) delusr user_name [root only]"<<endl;
	cout<<"15) disusr"<<endl;
	cout<<"16) login user_name"<<endl;
	cout<<"17) lsfile"<<endl;
	cout<<"18) chmod(p) (path) file_name mode"<<endl;
	cout<<"19) cd(p) (path) dir_name"<<endl;
	cout<<"20) exit"<<endl;
}


// split
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

vector<string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
};
