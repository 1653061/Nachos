#include "syscall.h"
#include "copyright.h"

int main(){
	char str[255];
	char write[255];
	int id, size=0, i=0;
	char c;
	print("Input file name: ");
	ReadString(str,255);
	id = Open(str,0);
	if ( id == 0){
		print("It's stdin mode\nWrong!!!!!");
	}
	if (id == -1){
		print("File doesn't exist\n");
		Halt();
	}
	print("What do you want to write:\n");
	ReadString(write,255);
	while (write[i] != 0){
		size++;
		i++;
	}
	Write(write,size,id);
	print("Write Success\n");
	Close(id);
	Halt();
}
