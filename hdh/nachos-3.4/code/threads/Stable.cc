#include "Stable.h"
#include "copyright.h"

Stable::Stable(){
	int i;
	bm = new BitMap(MAX_SEMAPHORE);
	for (i=0; i<MAX_SEMAPHORE;i++){
		semTab[i] = NULL;
	}
}

Stable::~Stable(){
	int i;
	delete bm;
	for (i=0; i<MAX_SEMAPHORE;i++){
               if( semTab[i] != NULL)
		delete semTab[i];
        }

}

int Stable::Create(char* name, int init){
	if (name == NULL || init < 0) // kiem tra hop le cua tham so
		return -1;
	int ID;
	for (int i=0; i<MAX_SEMAPHORE;i++){ // neu semaphore da ton tai thi bao loi
		if (semTab[i] == NULL) continue;
		else{
			if (strcmp(semTab[i]->GetName(), name) == 0)
				return -1;
		}
	}
	ID = bm->Find(); // tim vi tri chua mark
	if (ID == -1)
		return -1;
	semTab[ID] = new Sem(name,init); // tao semaphore moi
	
	return 0;
}

int Stable::Wait(char* name){
	if (name == NULL) //  kiem tra hop le cua tham so
		return -1;

	int i;
	for (i=0; i<MAX_SEMAPHORE;i++){ // tim vi tri neu co thi goi wait va tra ve vi tri
		if (semTab[i] == NULL)
			continue;
		if (strcmp(semTab[i]->GetName(), name) == 0){
			semTab[i]->wait();
			return i;
		}
	}

	return -1;
} 

int Stable::Signal(char* name){
	if (name == NULL) 
                return -1;
	int i;
        for (i=0; i<MAX_SEMAPHORE;i++){
                if (semTab[i] == NULL)
                        continue;
                if (strcmp(semTab[i]->GetName(), name) == 0){ // tim vi tri neu co thi goi signal va tra ve vi tri
                        semTab[i]->signal();
                        return i;
                }
        }

        return -1;

}

int Stable::FindFreeSlot(){
	for (int i=0; i<MAX_SEMAPHORE;i++){
		if (semTab[i] == NULL)
			return i;
	}

	return -1;
}

Sem* Stable::GetSem(int i){
	return semTab[i];
}
