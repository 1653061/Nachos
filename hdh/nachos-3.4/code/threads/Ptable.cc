#include "Ptable.h"
#include "string.h"
#include "system.h"

Ptable::Ptable(int size){
	int i;
	pcb[0] = new PCB();
	pcb[0]->setThread(currentThread);	// Thread dau tien
	for (i = 1; i < MAX_PROCESS; i++){
		pcb[i] = NULL;
	}
	psize = size;
	bm = new BitMap(MAX_PROCESS);
	bm->Mark(0); // mark thread dau tien da su dung
	bmsem = new Semaphore("bmsem",1);
}

Ptable::~Ptable(){
	int i;
	delete bm;
	delete bmsem;
	for (i = 0; i < MAX_PROCESS; i++){
               if ( pcb[i] != NULL)
		delete pcb[i];
        }
}

int Ptable::ExecUpdate(char* filename){
	bmsem->P(); // trnh nap 2 tien trinh cung luc
	int pid;
	if (filename == NULL){ // kiem tra hop le cua tham so
		bmsem->V();
		return -1;
	}
	OpenFile *executable = fileSystem->Open(filename); // kiem tra su ton tai cua file exe
	if (executable == NULL){
		bmsem->V();
		return -1;
	}
	delete executable;
	if (bm->NumClear() == MAX_PROCESS){ // neu bitmap chua mark o nao
		pid = 0;
		bm->Mark(pid);
	} else{
		if (strcmp(currentThread->getName(),filename) == 0){ // neu thread da ton tai
			bmsem->V();
			return -1;	
		}
		
		pid = bm->Find(); // tim o chua mark
		if (pid == -1){
			bmsem->V();
			return -1;
		}
	}
	pcb[pid] = new PCB(); // new pcb moi
	int result = pcb[pid]->Exec(filename, pid); // goi thuc thi ham exec trong pcb
	if (result == -1){ // neu tao that bai
		bm->Clear(pid);
		delete pcb[pid];
		bmsem->V();
		return -1;
	}
	pcb[pid]->IncNumWait(); // tang so tien trinh cho
	bmsem->V();
	return pid;
}

int Ptable::ExitUpdate(int ec){
	int pID = -1;
	for (int i=0; i<MAX_PROCESS;i++){ // kiem tra ton tai cua tien trinh va lay id
		if (pcb[i] == NULL) continue;
		if (pcb[i]->GetThread() == currentThread){
			pID = i;
			break;
		}
	}
	
	if (pID == -1)
		return -1;

	int parent =  pcb[pID]->parentID; // lay parent id cua tien trinh
	
	
	if (pID == 0){ // neu la tien trinh dau tien
		DEBUG('a', "\n Shutdown, initiated by user program.");
                printf ("\n\n Shutdown, initiated by user program.");
		interrupt->Halt();
	} else{
	pcb[pID]->SetExitCode(ec); // dat exitcode
	pcb[parent]->JoinRelease(); // tien trinh cha cho tien trinh con
	pcb[parent]->DecNumWait(); // giam so tien trinh cho
	pcb[pID]->ExitWait();	// tien trinh con xin tien trinh cha thoat
	delete pcb[pID];
	pcb[pID] = NULL;
        bm->Clear(pID);
	delete currentThread->space;
	currentThread->Finish();
        return 0;
	}
	return -1;
}

int Ptable::JoinUpdate(int id){
	if (id <0 || id > MAX_PROCESS) 
		return -1;
	if (bm->Test(id) == -1) // kiem tra ton tai cua process id
		return -1;

	int pID = -1;
	for (int i=0; i<MAX_PROCESS; i++){ // lay id cua tien trinh goi thuc hien
		if (pcb[i] == NULL) continue;
		if (pcb[i]->GetThread() == currentThread){
			pID = i;
			break;
		}
	}
	if (pID == -1)
		return -1;
	if (pcb[id]->parentID != pID) // neu tien trinh goi thuc hien khong phai cha cua tien trinh id
		return -1;
	pcb[pID]->IncNumWait(); // tang tien trinh cho
	pcb[pID]->JoinWait(); // tien trinh cha cho
	int ec = pcb[pID]->GetExitCode(); // lay exit code
	pcb[id]->ExitRelease(); // tien trinh con duoc cho phep thoat
	return ec;	

}

int Ptable::GetFreeSlot(){
	int i;
	for (i = 0; i<MAX_PROCESS;i++){
		if (pcb[i] == NULL)
			return i;
	}
	return -1;
}

bool Ptable::IsExist(int pid){
	for (int i=0; i<MAX_PROCESS;i++){
		if (pcb[i]->GetID() == pid)
			return true;
	}
	return false;
}

void Ptable::Remove(int pid){
	delete pcb[pid];
        pcb[pid] = NULL;
        bm->Clear(pid);
}

char* Ptable::GetFileName(int id){
	return pcb[id]->GetFileName();
}
