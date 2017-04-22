

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  while(1){
    if(listen(welcomeSocket,5)==0)
      printf("Listening\n");
    else
      printf("Error\n");

    
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    recv(newSocket, buffer, 1024, 0);

    if( !strcmp(buffer, "quit") ){
      printf("bye bye \n");
      break;
    }else{

      printf("Data received: %s \n",buffer);  
    }

  }
  close(welcomeSocket);

  return 0;
}


/*
TO DO:
- komunikacja w 2 strony
- kilku klientow
- plik config
- protokol
- wystawienie interface'ow
- zobaczyc na warningi
- zrobic server jako demon (+logi)
- 

*/