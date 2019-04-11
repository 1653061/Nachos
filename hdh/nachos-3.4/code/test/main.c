#include "syscall.h"

void main(){
/*	int s1;
	int s2;
	int ping;
	int pong;
	s1 = CreateSemaphore("A",1);
	s2 = CreateSemaphore("B", 0);
	
	ping = Exec("./test/ping");
	pong = Exec("./test/pong");
	
	Join(ping);
	Join(pong);
*/
	char c[255];
	char tmp[255];
	char result[255];
	int s1, s2;
	int loop, out, seek, id, step, c_index, exe, t_index, size;
	Create("output.txt");
	id = Open("input.txt",1);
	Read(c,255,id);
	Close(id);
	c_index = 0;
	t_index = 0;
	step = 0;
	loop = 0;
	size = 0;
	seek = 0;
	while (c[c_index] != '\n'){// doc so dot sinh vien
		loop = loop*10 + c[c_index] - 48;
		c_index++;
	}
	c_index++;
	s1 = CreateSemaphore("sinhvien",0); // semaphore cho sinhvien
	s2 = CreateSemaphore("voinuoc",0); // semaphore cho voinuoc
	while (step < loop){ 
		Create("sinhvien.txt"); // luu gia tri moi dot
		id = Open("sinhvien.txt",0);
		while (c[c_index] != '\n'){
			tmp[t_index] = c[c_index];
			c_index++;
			t_index++;
		}
		Write(tmp,t_index,id);
		Close(id);
		exe = Exec("./test/sinhvien"); // goi thuc thi sinhvien
		Join(exe); // wait sinhvien
		out = Open("output_tmp.txt",1);
		seek = Seek(-1,out);
		Seek(0,out);
		Read(tmp,seek+1,out); // ghi ket qua thu duoc vao chuoi tmp
		Close(out);
		t_index = 0;
		while (t_index < seek+1){ // ghi tmp vao chuoi result de giu ket qua cuoi cung
			result[size] = tmp[t_index];
			size++;
			t_index++;
		}
		result[size] = '\n';
		size++;
		t_index = 0;
		c_index++;
		step++;
	}
	out = Open("output.txt",0);
	Write(result,size,out); // ghi vao file output
	Close(out);

}
