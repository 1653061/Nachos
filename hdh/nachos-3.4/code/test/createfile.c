#include "syscall.h"
#include "copyright.h"
#define maxlen 32
int
main()
{
	int a = ReadInt();
	PrintInt(a);
	Halt();
}
