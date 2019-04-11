#ifndef PCB_H_H
#define PCB_H_H

#include "synch.h"

class PCB{
	
	private:
		Semaphore* joinsem; // semaphore cho qua trinh join
		Semaphore* exitsem; // semaphore cho qua trinh exit
		Semaphore* mutex; // semaphore cho qua trinh truy xuat
		int exitcode;
		int numwait; // so tien trinh join
		Thread *t; // tien trinh
	public:
		int parentID; // id cua thread cha
		PCB();
		PCB(int id);
		~PCB();
		int Exec(char* filename, int pid); // tao thread moi voi filename va process id la pid
		int GetID(); // lay id cua tien trinh goi thuc hien
		int GetNumWait(); // lay numwait
		
		void JoinWait(); // tien trinh cha doi tien trinh con
		void ExitWait(); // tien trinh con ket thuc
		void JoinRelease(); // tien trinh cha thuc thi tiep
		void ExitRelease(); // cho phep tien trinh con ket thuc

		void IncNumWait(); // tang so tien trinh cho
		void DecNumWait(); // giam so tien trinh cho

		void SetExitCode(int ec); // dat exitcode
		int GetExitCode(); // lay exitcode

		void SetFileName(char* fn); // dat ten tien trinh
		char* GetFileName(); // lay ten tien trinh
		Thread* GetThread(); // lay tien trinh
		void setThread(Thread* curThr); // thay doi tien trinh
};

#endif
