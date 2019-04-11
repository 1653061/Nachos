#include "syscall.h"

void main(){
	int pingID, pongID;
	print("Ping-Pong test starting ...\n\n");
	pingID = Exec("./test/ping");
	pongID = Exec("./test/pong");
	Join(pingID);
	Join(pongID);	
}
