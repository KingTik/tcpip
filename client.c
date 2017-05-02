#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


#define BUFFER_SIZE 1024

int main(int argc, char * argv[]){

//printf("%s \n", argv[1]);

  if (argc < 2){
    printf ("Usage: ./clinet message\n");
    return -1;
  }
 
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  size_t status;

  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  
  addr_size = sizeof serverAddr;
  
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    

  strcpy(buffer,argv[1]);
  

  while(1){
  
    send(clientSocket,buffer,13,0);
    status = recv(clientSocket, buffer, 1024, 0);
    
    if(status > 0){
      printf("Odpowiedz servera: %s",buffer); 
    }
    
    printf("%s\n" ,buffer);
    if(!strcmp(buffer, "quit")){
      return 0;
    }  
    memset(buffer,'\0', BUFFER_SIZE);
    scanf("%s", buffer);
    //
  }

  return 0;
}