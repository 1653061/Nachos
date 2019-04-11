#include "syscall.h"

void main(){
	int id, c_index, value, seek, voi1, voi2, out, tab, r_index;
	char c[255];
	char result[255];
	c_index = 0;
	voi1 = 0;
	voi2 = 0;
	value = 1;
	tab = 0;
	seek = 0;
	r_index = 0;
	Create("output_tmp.txt"); // luu ket qua tra ve tam
	out = Open("output_tmp.txt",0);
	while (1){
		value = 0;
		Down("voinuoc"); // cho sinhvien tinh toan va Up voinuoc
		id = Open("voinuoc.txt",1); // Doc gia tri la 1 so nguyen trong file voinuoc.txt
		seek = Seek(-1,id);
		Seek(0,id);
		Read(c,255,id);
		Close(id);
		// Neu la 0 thi dung loop
		if (c[0] == '0')
			break;
		// cu phap file output
		if (tab == 1){
			result[r_index] = '\t';
			r_index++;
		}
		// chuyen thanh so
		while (c_index < seek+1){
			value = value*10 + c[c_index] - 48;
			c_index++;
		}
		// kiem tra voi nao van con dang su dung
		if (voi1 <= voi2){
			voi1 = voi1+value;
			result[r_index] = '1';
		} else{
			voi2 = voi2+value;
			result[r_index] = '2';
		}
		r_index++;
		c_index=0;
		tab = 1;
		Up("sinhvien"); // wakeup sinhvien chay tiep
	}
	Write(result,r_index,out); // ghi ket qua vao file out tmp
	Close(out);
}
