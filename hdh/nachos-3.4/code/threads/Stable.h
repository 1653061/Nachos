#ifndef STABLE_H
#define STABLE_H


#define MAX_SEMAPHORE 10

#include "bitmap.h"
#include "sem.h"
#include "copyright.h"

class Stable 
{
	public:
		Stable();
		~Stable();
		int Create(char* name, int init); //Tao semaphore

		int Wait(char* name); // goi this->P() de thuc thi

		int Signal(char* name); // goi this->V() de thuc thi

		int FindFreeSlot();  // tim slot trong
		
		Sem* GetSem(int i);
	private:
                BitMap* bm;
                Sem* semTab[MAX_SEMAPHORE];


};
#endif
