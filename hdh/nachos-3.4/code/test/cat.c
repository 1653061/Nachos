#include "syscall.h"
#include "copyright.h"

int main(){
	char str[255];
	int id, size, i;
	char c;
	print("Input file name: ");
	ReadString(str,255); // ten file name
	id = Open(str,0); // mo file
	if (id == -1){ // neu file ko ton tai bao loi
		print("File doesn't exist\n");
		Halt();
	}
	size = Seek(-1,id); // lay do dai cua file
	Seek(0,id); // dua con tro file ve lai dau
	for (i=0; i<size;i++){ // xuat noi dung file
		Read(&c,1,id);
		PrintChar(c);
	}
	Close(id);
	
	Halt();
}
