#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024

int main(int argc, char * argv[]){



 
  int clientSocket, read_size;
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
    
  int i = 0;
  while(1){
    scanf("%s", buffer);



    send(clientSocket,buffer,13,0);
   

    if( !strcmp(buffer, "quit") || !strcmp(buffer, "kill") ){
      close(clientSocket);
      return 0;
    }
     memset(buffer,'\0', BUFFER_SIZE);
    if( recv(clientSocket, buffer , 1024 , 0) < 0){
        //puts("recv failed");
        break;
      }
         
        puts("Server reply :");
        puts(buffer);
  }


  close(clientSocket);
  

  return 0;
}