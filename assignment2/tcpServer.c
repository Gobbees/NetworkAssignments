#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "myfunction.h"

#define MAX_BUF_SIZE 1024 

int main(int argc, char *argv[]){
	int pid = getpid();
  struct sockaddr_in client_addr; // struct containing client address information
  int i;
  ssize_t byteRecv; // Number of bytes received
  ssize_t byteSent; // Number of bytes to be sent
  socklen_t cli_size;
  char receivedData [MAX_BUF_SIZE]; // Data to be received
  char sendData [MAX_BUF_SIZE]; // Data to be sent
  
  cli_size = sizeof(client_addr);
  
  for(;;){
	recv(0, receivedData, MAX_BUF_SIZE, 0);

  	byteRecv = 7;
	sprintf(receivedData, "%d", pid);
	
	byteSent = send(1, receivedData, byteRecv, 0);
  }
  
  return 0;
}
