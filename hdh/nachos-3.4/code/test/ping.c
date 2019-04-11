#include "syscall.h"

void main(){
	int i;
	for (i=0; i<1000;i++){
		Down("A");	
		PrintChar('A');
		Up("B");
	}

//	int id = Open("a",0);
//	Write("\na",2, id);
//	Close(id);
}

