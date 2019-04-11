// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

//  Input: - User spaceaddress (int)
 // - Limit of buffer (int)
 // Output:- Buffer (char*)
 // Purpose: Copy buffer from User memory space to System memory space
 
void updatePC() // tang thanh ghi pc
{
	int pc, nextpc, prevpc;
	prevpc = machine->ReadRegister(PrevPCReg);
	pc = machine->ReadRegister(PCReg);
	nextpc = machine->ReadRegister(NextPCReg); 														     prevpc = pc;
	pc = nextpc;
	nextpc = nextpc + 4;																	     machine->WriteRegister(PrevPCReg, prevpc);														    	  machine->WriteRegister(PCReg, pc);															       machine->WriteRegister(NextPCReg, nextpc);
}
 char* User2System(int virtAddr,int limit)
 {
  int i;// index
   int oneChar;
    char* kernelBuf = NULL;
     kernelBuf = new char[limit +1];//need for terminal string
      if (kernelBuf == NULL)
       return kernelBuf;
        memset(kernelBuf,0,limit+1);
         //printf("\n Filename u2s:");
          for (i = 0 ; i < limit ;i++)
           {
            machine->ReadMem(virtAddr+i,1,&oneChar);
             kernelBuf[i] = (char)oneChar;
              //printf("%c",kernelBuf[i]);
               if (oneChar == 0)
                break;
                 }
                  return kernelBuf;
  }

// Input: - User space address (int)
 // - Limit of buffer (int)
 // - Buffer (char[])
 // Output:- Number of bytes copied (int)
 // Purpose: Copy buffer from System memory space to User memory space
 int System2User(int virtAddr,int len,char* buffer)
 {
  if (len < 0) return -1;
   if (len == 0)return len;
    int i = 0;
     int oneChar = 0 ;
      do{
       oneChar= (int) buffer[i];
        machine->WriteMem(virtAddr+i,1,oneChar);
         i ++;
          }while(i < len && oneChar != 0);
           return i; 
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    
    // Input: reg4 -filename (string)
    // // Output: reg2 -1: error and 0: success
    // // Purpose: process the event SC_Create of System call
    // // mã system call sẽ được đưa vào thanh ghi r2 (có thể xem lại phần xử lý cho
    // // system call Halt trong tập tin start.s ở trên)
    // // tham số thứ 1 sẽ được đưa vào thanh ghi r4
    // // tham số thứ 2 sẽ được đưa vào thanh ghi r5
    // // tham số thứ 3 sẽ được đưa vào thanh ghi r6
    // // tham số thứ 4 sẽ được đưa vào thanh ghi r7
    // kết quả thực hiện của system call sẽ được đưa vào thanh ghi r2
    switch (which) {
    	 case NoException:
     		return;
    	 case SyscallException:
      		switch (type){
       			case SC_Halt:{
        			DEBUG('a', "\n Shutdown, initiated by user program.");
         			printf ("\n\n Shutdown, initiated by user program.");
          			interrupt->Halt();
          			break;
			}
			case SC_Create:{
				int virtAddr;
				char* filename;
				DEBUG('a',"\n SC_Create call ...");
				DEBUG('a',"\n Reading virtual address of filename");
				// Lấy tham số tên tập tin từ thanh ghi r4
 				virtAddr = machine->ReadRegister(4);
	 			DEBUG ('a',"\n Reading filename.");
				// MaxFileLength là = 32
 				filename = User2System(virtAddr,MaxFileLength+1);
 				if (filename == NULL)
 				{
  					printf("\n Not enough memory in system");
					DEBUG('a',"\n Not enough memory in system");
		    			machine->WriteRegister(2,-1); // trả về lỗi cho chương
     				// trình người dùng
      					delete filename;
  		     			return;
      				}			
       				DEBUG('a',"\n Finish reading filename.");
      				//DEBUG(‘a’,"\n File name : '"<<filename<<"'");
      				 // Create file with size = 0
       				// Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
       				// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
       				// hành Linux, chúng ta không quản ly trực tiếp các block trên
       				// đĩa cứng cấp phát cho file, việc quản ly các block của file
       				// trên ổ đĩa là một đồ án khác
       				if (!fileSystem->Create(filename,0))
       				{
     		   			printf("\n Error create file '%s'",filename);
         				machine->WriteRegister(2,-1);
          				delete filename;
        	   			return;
	           		}
           			machine->WriteRegister(2,0); // trả về cho chương trình
            			// người dùng thành công
            			delete filename;
				updatePC();
        	    		break;
             		}
			case SC_ReadString:
			{	// lay tham so chuoi tren thanh ghi r4
				int vaAddr = machine->ReadRegister(4);
				// lay do dai cua chuoi
				int len = machine->ReadRegister(5);
				
				char* str = new char[len];
				// nguoi dung nhap vao chuoi tra ve so byte ghi
				int size = gSynchConsole->Read(str,len);
				// dua chuoi buf tu system space vao userspace
				System2User(vaAddr,size--,str);
				delete[] str;
				updatePC();
				break;
			}
			case SC_Print:
			{	// Lay tham so chuoi can xuat tren thanh ghi r4
				int vaAddr = machine->ReadRegister(4);
				// dua chuoi tu User space sang System space
				char* va = User2System(vaAddr, 255);
				int i = 0;
				// viet tung ki tu trong chuoi va ra ngoai console
				while ( va[i] != 0){
					gSynchConsole->Write(va+i ,1);
					i++;
				}
				delete[] va;
				updatePC();
				break;
			}
			case SC_Open:
			{
				int vaAddr = machine->ReadRegister(4); // ten cua file
				int type = machine->ReadRegister(5); // type cua file

				char* va = User2System(vaAddr,255);

				int ind = fileSystem->Open(va,type); // lay index cua file
				// Neu la input or output thi tra ve 0 or 1
				if (strcmp(va,"stdin") == 0){
					machine->WriteRegister(2,0);
					delete[] va;
					updatePC();
					break;
				}

				if (strcmp(va,"stdout") == 0){
                                        machine->WriteRegister(2,1);
                                        delete[] va;
                                        updatePC();
                                        break;
                                }
				// Neu file ton tai tra ve vi tri file
				if (ind != -1){
					machine->WriteRegister(2,ind); // index la vi tri cua file
				} else{ // Neu file khong ton tai tra ve -1
					machine->WriteRegister(2,-1);
				}	
				updatePC();
				break;
			}
			case SC_Close:
			{	// Doc id cua file tu thanh ghi r4
				int vaAddr = machine->ReadRegister(4);
				// Neu id = -1 thi bao loi
				if (vaAddr == -1){
					printf("Close failed\n");
					updatePC();
					break;
				}
				// xoa file tai id tuong ung va set ve bang NULL
				delete fileSystem->o[vaAddr];
				fileSystem->o[vaAddr] = NULL;
				updatePC();
				break;
			}
			case SC_Read:
			{
				int vaAddr = machine->ReadRegister(4); // Tham so luu tru noi dung file
				int size = machine->ReadRegister(5); // so ki tu doc
				int ID = machine->ReadRegister(6); // ID cua file can doc
				
				int byt; // so byt tra ve
				int prev; // vi tri bat dau cua offset
				int next; // vi tri sau khi doc cua offset
				char* va = new char[size];
				if (ID > 10 || ID < 0){
					machine->WriteRegister(2,-1);
					delete[] va;
					updatePC();
					break;
				}

				if (fileSystem->o[ID] == NULL){
					machine->WriteRegister(2,-1);
					delete[] va;
                                        updatePC();
                                        break;
				}
				prev = fileSystem->o[ID]->CurrentOffset(); // vi tri bat dau
				if (ID == 0){ // stdin mode
					next = gSynchConsole->Read(va,size); // ghi va tra ve so byte ghi that su
					byt = next - prev + 1; // so byt tra ve
					machine->WriteRegister(2,byt);
					System2User(vaAddr,next,va); // copy tu System space sang User space

					delete[] va;
					updatePC();
					break;
				}
				fileSystem->o[ID]->Read(va,size); // doc file
				next = fileSystem->o[ID]->CurrentOffset(); // tra ve vi tri hien tai cua con tro file
				byt = next - prev + 1; // so byte ghi thuc su
				System2User(vaAddr,byt,va); // dua chuoi tu system space vao lai user space
				machine->WriteRegister(2,byt); // ghi ket qua so byte tra ve
				delete[] va;
				updatePC();
				break;
			}
			case SC_Write:
			{
				int vaAddr = machine->ReadRegister(4); // doc chuoi can ghi
				int size = machine->ReadRegister(5); // kich thuoc chuoi ghi
				int ID = machine->ReadRegister(6); // file can ghi
				
				int byt, prev, next;
				int i;
				char* va = new char[size];
				// kiem tra ID
				if (ID > 10 || ID < 0){
                                        machine->WriteRegister(2,-1);
                                        delete[] va;
                                        updatePC();
                                        break;
                                }

				if (fileSystem->o[ID] == NULL){
					machine->WriteRegister(2,-1);
                                        delete[] va;
                                        updatePC();
                                        break;

				}
				prev = fileSystem->o[ID]->CurrentOffset();	// vi tri con tro dau
				va = User2System(vaAddr,size); // chuyen user space sang system space
				if (fileSystem->o[ID]->Write(va,size) != -1){ // Kiem tra xem file co phai read-only hay khong
					next = fileSystem->o[ID]->CurrentOffset(); // vi tri con tro sau khi viet
					byt = next - prev +1; // so byt ghi that su
					machine->WriteRegister(2,byt); // tra ve so byt
				}
				else{
					machine->WriteRegister(2,-1);
					delete[] va;
					updatePC();
					break;
				}

				if (ID == 1){ // stdout mode
					i = 0;
					while (va[i] != 0){
						gSynchConsole->Write(va+i,1);
						i++;
					}
					va[i] = '\n';
					gSynchConsole->Write(va+i,1);
					machine->WriteRegister(2,i-1);
				}

				delete[] va;
				updatePC();
				break;
			}
			case SC_Seek:
			{
				int position = machine->ReadRegister(4); // vi tri can dua con tro file
				int ID = machine->ReadRegister(5); // ID cua file can thao tac

				if (ID > 10 || ID < 0){ // kiem tra id file
                                        machine->WriteRegister(2,-1);
                                        updatePC();
                                        break;
                                }

                                if (fileSystem->o[ID] == NULL){
                                        machine->WriteRegister(2,-1);
                                        updatePC();
                                        break;
                                }

				if (position == -1){ // neu position can tro = -1 thi dua toi cuoi file
                                        position = fileSystem->o[ID]->Length() - 1; // vi tri cuoi file
                                        fileSystem->o[ID]->Seek(position); // dua con tro toi
                                        machine->WriteRegister(2,position); // tra ve vi tri
                                        updatePC();
                                        break;
                                }

				if (position < 0 || position > fileSystem->o[ID]->Length()){ // kiem tra vi tri dua vao hop le hay khong
                                        machine->WriteRegister(2,-1);
                                        updatePC();
                                        break;
                                }
				
				fileSystem->o[ID]->Seek(position); // dua con tro toi vi tri
				machine->WriteRegister(2,position); // tra ve vi tri
				updatePC();
				break;

			}
			case SC_ReadInt:
			{
				char* va = new char[10];
				int size, i;
				int num = 0;
				size = gSynchConsole->Read(va,10);// nhap chuoi so ra ve do dai
				for (i=0; i<size;i++){ // chuyen chuoi sang so
					if (va[i] != '-'){
						num = num*10 + (int)(va[i] - 48);	
					} 
				}
				if (va[0] == '-'){
					num = num*(-1);
				}
				machine->WriteRegister(2,num);
				delete[] va;
				updatePC();
				break;
			}
			case SC_PrintInt:
			{
				int vaAddr = machine->ReadRegister(4); // load so tu thanh ghi r4
				char minus = '-';
				char* num = new char[255];
				int i = 0;
				int j = 0;
				if (vaAddr < 0){ // kiem tra so am hay duong
					gSynchConsole->Write(&minus,1);
					vaAddr= -vaAddr;
				}
				while (vaAddr > 0){ // chuyen so thanh chuoi
					if (i == 0){
						num[0] = vaAddr%10 + '0'; 
						i++;
						vaAddr = vaAddr / 10;
					} else{
						for (j = i; j > 0; j--){
							num[j] = num[j-1];
						}
						num[0] = vaAddr%10 + '0';
						i++;
						vaAddr = vaAddr / 10; 
					}
				}
				j = 0;
				while (j < i){ // viet so ra console
					gSynchConsole->Write(num+j,1);
					j++;
				}
				delete[] num;
				updatePC();
				break;
			}
			case SC_PrintChar:{ // in ki tu
				char c = machine->ReadRegister(4);
				gSynchConsole->Write(&c,1);
				updatePC();
				break;
			}
			case SC_Exec:{
				int vaAddr = machine->ReadRegister(4); // lay tham so ten chuoi tu thanh ghi so 4
				int result;
				char* va = new char[255];
				
				va = User2System(vaAddr,255); // chuyen tu user space sang kernel space
				
				if (va == NULL){ // neu ten bi loi
					printf("Error open file\n");
					machine->WriteRegister(2,-1);
				}

				result = pTab->ExecUpdate(va); // goi execupdate de kiem tra va tao tien trinh
				machine->WriteRegister(2,result);
				delete [] va;
				updatePC();
				break;
			}
			case SC_Join:{
				int id = machine->ReadRegister(4); // lay tham so id tu thanh ghi so 4
				int result = pTab->JoinUpdate(id); // goi joinupdate de thuc thi
				machine->WriteRegister(2,result);
				updatePC();
				break;
			}
			case SC_Exit:{
				int exitStatus = machine->ReadRegister(4); // lay tham so ec tu thanh ghi so 4
				int result = pTab->ExitUpdate(exitStatus); // kiem tra va thuc thi giai phong tien trinh va bo nho
				machine->WriteRegister(2,result);
				updatePC();
				break;
			}
			case SC_CreateSemaphore:{
				int vaAddr = machine->ReadRegister(4); // lay ten tu thanh ghi so 4
				int semval = machine->ReadRegister(5); // lay val tu thanh ghi so 5
				int check;
				char* name = new char[255];
				name = User2System(vaAddr,255); // chuyen tu Userspace sang kernel space

				check = semTab->Create(name, semval); // goi ham tao semaphore
				if (check == -1){ // neu tao that bai bao loi
					printf("Error\n");
					machine->WriteRegister(2,-1);
				}
				else{
					machine->WriteRegister(2,check);
				}

				delete [] name;
				updatePC();
				break;
			}
			case SC_Up:{
				int vaAddr = machine->ReadRegister(4); // lay ten tu thanh ghi so 4
				char* va = new char[255];
				int i;
				int result;
				va = User2System(vaAddr,255); // chuyen tu Userspace sang kernel space
				for (i=0; i<10;i++){ // kiem tra ten co ton tai trong mang khong. Neu khong thi bao loi
					if (strcmp(semTab->GetSem(i)->GetName(),va) == 0)
						break;
				}
				if (i == 10)
					machine->WriteRegister(2,-1);
				else {
					result = semTab->Signal(va); // neu ton tai goi Signal de thuc thi
					machine->WriteRegister(2,result);
				}
				delete[] va;
				updatePC();
				break;
			}
			case SC_Down:{
				 int vaAddr = machine->ReadRegister(4); // lay ten tu thanh ghi so 4
                                char* va = new char[255];
                                int i;
                                int result;
                                va = User2System(vaAddr,255); // chuyen tu Userspace sang kernel space
                                for (i=0; i<10;i++){ // kiem tra ton tai hay khong. Neu khong thi bao loi
                                        if (strcmp(semTab->GetSem(i)->GetName(),va) == 0)
                                                break;
                                }
                                if (i == 10)
                                        machine->WriteRegister(2,-1);
                                else {
                                        result = semTab->Wait(va); // Neu ton tai goi Wait de thuc thi
                                        machine->WriteRegister(2,result);
                                }
                                delete[] va;
                                updatePC();
                                break;

			}
			default:
			printf("\n Unexpected user mode exception (%d %d)", which,type);
 			interrupt->Halt();
 		} break;  
		case PageFaultException:
		{
			printf("No valid translation found\n");
			
			interrupt->Halt();
			break;
		}
		case ReadOnlyException:
		{
			printf("Write attempted to page marked read-only\n");

			interrupt->Halt();
			break;
		}
		case BusErrorException:
		{
			printf("Translation resulted in an invalid physical address\n");

			interrupt->Halt();
			break;
		}
		case AddressErrorException:
		{
			printf("Unaligned reference or one that was beyondthe end of the address space\n");
			
			interrupt->Halt();
			break;
		}
		case OverflowException:
		{
			printf("Interger overflow in add or sub\n");

			interrupt->Halt();
			break;
		}
		case IllegalInstrException:
		{
			printf("Unimplemented or reserved instr\n");

			interrupt->Halt();
			break;
		}
	}
}
