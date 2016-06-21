#include "include/stdlib.h"

int isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isNum(char c){
	return (c >= '0' && c <= '9');
}

void* malloc(int bytes){
	return int80(1,bytes,0,0,0);
}

int readData(){
	int n;
	int80(15,&n,0,0,0);
	return n;
}

void  free(){ return;}