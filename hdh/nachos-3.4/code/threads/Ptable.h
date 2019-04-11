#ifndef PTABLE_H
#define PTABLE_H


#include "bitmap.h"
#include "synch.h"
#include "PCB.h"

#define MAX_PROCESS 10



class Ptable{
	private:
		BitMap *bm; // danh dau vi tri da dung trong pcb
		PCB* pcb[MAX_PROCESS];
		int psize;
		Semaphore *bmsem; // ngan nap nhieu tien trinh cung luc

	public:
		Ptable(int size);
		~Ptable();

		int ExecUpdate(char* name); // xy ly syscall exec
		int ExitUpdate(int ec); // xu ly syscall exit
		int JoinUpdate(int id); // xu ly syscall join
		int GetFreeSlot(); // tim slot trong de luu tien trinh moi
		bool IsExist(int pid); // kiem tra ton tai process id khong
		void Remove(int pid); // khi tien trinh ket thuc delete process id ra khoi mang
		char* GetFileName(int id); // lay ten cua tieu trinh

};


#endif
