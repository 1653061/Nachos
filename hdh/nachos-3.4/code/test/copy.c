#include "syscall.h"
#include "copyright.h"

int main(){

	char copy[255];
	char des[255];
	int read, write;
	int IDF1, IDF2, size, i;
	char c;
	// nhap file copy va lay id
	print("Input file copy: ");
	ReadString(copy,255);
	IDF1 = Open(copy,1);
	// nhap file copy den va lay id
	print("Input file des: ");
	ReadString(des,255);
	IDF2 = Open(des,0);
	// neu file copy khong ton tai thi bao loi
	if (IDF1 == -1){
		print("File doesn't exist\n");
		Halt();
	}
	// neu file copy den khong ton tai thi tao ra
	if (IDF2 == -1){
		Create(des);
		IDF2 = Open(des,0);
	}
	
	size = Seek(-1,IDF1); // lay do dai cua file copy
	Seek(0,IDF1);
	Seek(0,IDF2); // dua tro file ve dau
	for (i = 0 ; i < size; i++){
		Read(&c,1,IDF1);
		Write(&c,1,IDF2); // doc va ghi
	}
	
	// close file
	Close(IDF1);
	Close(IDF2);	
	Halt();
}
