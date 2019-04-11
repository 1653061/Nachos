#include "syscall.h"
#include "copyright.h"

int main(){
	char str[255];
	char write[255];
	int id;
	print("Input file name: ");
	ReadString(str,255);
	id = Open(str,1);
	if ( id == 1){
		print("It's stdout mode\nWrong!!!!!");
	}
	if (id == -1){
		print("File doesn't exist\n");
		Halt();
	}
	Read(write,255,id);
	print(write);
	Close(id);
	Halt();
}
