#include "syscall.h"
#include "copyright.h"

int main(){

	char* str;
	ReadString(str,255);
	print(str);
	Halt();

}
