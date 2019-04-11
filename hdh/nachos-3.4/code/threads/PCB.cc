#include "PCB.h"
#include "system.h"
#include "filesys.h"
void processCreator(int arg){
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	machine->Run();
	ASSERT(FALSE);
}

PCB::PCB(){
	joinsem = new Semaphore("joinsem",0);
	exitsem = new Semaphore("exitsem",0);
	mutex = new Semaphore("mutex",1);
	exitcode = 0;
	numwait = 0;
	parentID = -1;
	t = NULL;
}

PCB::~PCB(){
	delete joinsem;
	delete exitsem;
	delete mutex;
}

int PCB::Exec(char* filename, int pid){
	mutex->P(); // tranh nap nhieu tien trinh cung mot luc
	if (filename == NULL || pid < 0 || t != NULL){ // kiem tra hop le cua tham so
		printf("Not enough memory\n");
		mutex->V();
		return -1;
	}
	OpenFile* executable = fileSystem->Open(filename); // kiem tra ton tai cua file exe
	if (executable == NULL){
		mutex->V();
		return -1;
	}
	t = new Thread(filename); // tao mot thread moi
	t->space = new AddrSpace(executable); // cap space can thiet
	parentID = currentThread->getID(); // set parrent id la id cua tien trinh goi thuc thi
	t->setID(pid); // set process id cua tien trinh vua tao
	delete executable;
	t->Fork(processCreator,0); // Fork de t duoc len ke hoach thuc thi
	mutex->V();
	return pid;	
}

int PCB::GetID(){
	return t->getID();
}

int PCB::GetNumWait(){
	return numwait;
}

void PCB::JoinWait(){
	joinsem->P();
}

void PCB::ExitWait(){
	exitsem->P();
}

void PCB::JoinRelease(){
	joinsem->V();
}

void PCB::ExitRelease(){
	exitsem->V();
}

void PCB::IncNumWait(){
	numwait++;
}

void PCB::DecNumWait(){
	numwait--;
}

void PCB::SetExitCode(int ec){
	exitcode = ec;
}

int PCB::GetExitCode(){
	return exitcode;
}

void PCB::SetFileName(char* fn){
	t->setName(fn);
}

char* PCB::GetFileName(){
	return t->getName();
}

Thread* PCB::GetThread(){
	return t;
}

void PCB::setThread(Thread* curThr){
	t = curThr;
}
