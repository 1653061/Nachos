#ifndef SEM_H
#define SEM_H

#include "synch.h"





class Sem{
	private:
		char name[50];
		Semaphore *sem; 	

	public:
		Sem(char* na, int i){
			strcpy(this->name,na);
			sem = new Semaphore(name,i);
		}
		~Sem(){
			delete sem;
		}

		void wait(){ // thuc hien thao tac cho
			sem->P();
		}

		void signal(){ // giai phong semaphore
			sem->V();
		}

		char* GetName(){ // tra ve ten cua semaphore
			return name;
		}			 

};


#endif
