#include "syscall.h"
#include "copyright.h"

int main(){
	char str[255];
	int id, type;
	print("Input file name: ");	
	ReadString(str,255);
	print("Input type: ");
	ReadInt(type);	
	id = Open(str,type);
	if (id != -1){
		print("Open file success\n");
	} else{
		print("File doesn't exist or wrong type");
		Halt();
	}
	Close(id);
	Halt();
}
