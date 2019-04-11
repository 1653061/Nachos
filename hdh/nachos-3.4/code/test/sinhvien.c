#include "syscall.h"

void main(){
	char c[255];
	char tmp[255];
	int exe, id, c_index, t_index;
	id = Open("sinhvien.txt",1); // doc value trong file
	Read(c,255,id);
	Close(id);
	c_index = 0;
	t_index = 0;
	exe = Exec("./test/voinuoc");// goi thuc thi voinuoc
	while (c[c_index] != 0){ // iem tra da het chuoi
		while (c[c_index] != '\t' && c[c_index] != 32){ // doc tung so trong chuoi cach nhau boi dau cach or dau tab
			if (c[c_index] == 0){
				c_index--;
				break;
			}
			tmp[t_index] = c[c_index];
			c_index++;
			t_index++;
		}
		Create("voinuoc.txt"); //tao file voinuoc luu tru 1 so nguyen
		id = Open("voinuoc.txt",0);
		Write(tmp,t_index,id);
		Close(id);
		Up("voinuoc"); // wakeup voinuoc
		Down("sinhvien"); // cho voinuoc	
		c_index++;
		t_index = 0;	
	}
	id = Open("voinuoc.txt",0); // danh dau ket thuc cho voinuoc
	Write("0",1,id);
	Close(id);
	Up("voinuoc"); // wakeup voinuoc
	Join(exe); // cho voinuoc ket thuc
}
