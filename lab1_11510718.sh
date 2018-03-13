#!/bin/bash
mkdir -p ~/lab1/11510718  
echo -e "\c"  > ~/lab1/11510718/file.info  #overwrite the file
dir_array=($(find $HOME \( ! -regex '.*/\..*' \) -type d))  #display all the directories recurrently
for d in "${dir_array[@]}"; do
	tmp=${d//"/home/"/""} #replace "/home/" with ""
	echo "[ "$tmp" ]" >> ~/lab1/11510718/file.info  
	ls -1 "$d" | xargs -I {} echo /"$tmp"/{}  >> ~/lab1/11510718/file.info
	echo -e   >> ~/lab1/11510718/file.info #create a blank line
done
echo "$(echo "[ Directories Count ] : ")""$(($(find $HOME/* \( ! -regex '.*/\..*' \) -type d | wc -l)))"   >> ~/lab1/11510718/file.info  
echo -e   >> ~/lab1/11510718/file.info #create a blank line
echo "$(echo "[ Files Count ] : ")""$(find $HOME/* \( ! -regex '.*/\..*' \) -type f | wc -l)"   >> ~/lab1/11510718/file.info
