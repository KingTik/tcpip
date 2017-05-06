

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <libconfig.h>
#include <pthread.h>
#include <arpa/inet.h>
//#include <unistd.h>

/**
* funkcja zwraca wartosci z pliku konfiguracyjnego
* @param const chat *c:
    - port
    -max_client
*/
int get_int_from_settings(config_t cfg, const char *c){
  int ret_value;

  if (config_lookup_int(&cfg, c, &ret_value))
  {
    return ret_value;
  }else{
    return -1;
  }

}

/*
3 - list of commands
2 - send back hello
1 - quit
0 - display

*/

int message_handle(const char *message){

  if( !strcmp(message, "quit") ){
    return 1;
  }else if(!strcmp(message, "hi")){
    return 2;
  }else if(!strcmp(message, "help")){
    return 3;
  }else{
    return 0;
  }

}


void *connection_handle(void *newSocket){

  int socket = *(int*) newSocket;
  int read_size;
  char buffer[1024];

  while( (read_size = recv(socket, buffer, 1024, 0)) >0 ){
    printf("%s\n", buffer);
    if( !strcmp(buffer, "quit") ){
        close(socket);
        free(newSocket);
      
    }else if(!strcmp(buffer, "hi")){
      write(socket,"hi yourself\n",13,0);
      
    }else if(!strcmp(buffer, "help")){
      char msg[] = "quit\nkill\nhi\nhelp\n*";
      write(socket,msg ,strlen(msg)+1,0);
      
    }else if(!strcmp(buffer, "kill")){
        close(socket);
        free(newSocket);
        exit(1);
    }else{
      write(socket,"thats nice sweety\n",17,0);
      
    }

  }
   

}


int main(){

  config_t cfg;
  config_setting_t *setting;
  const char *str;
  int max_client, tmp, port, read_size;
  int run = 1;

  config_init(&cfg);

  // sprawdzenie czy jest plik konfiguracjny
  if(! config_read_file(&cfg, "konfiguracja.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    printf("nie ma pliku konfiguracyjnego");
    //jak nie ma to wartosci domyslne
    port = 7891;
    max_client = 3;
  }else{
    //jak jest to pobieramy wartosci z pliku konfiguracyjnego
    max_client = get_int_from_settings(cfg, "max_client");
    port = get_int_from_settings(cfg, "port");
    
  }

  printf ("port: %d   \nmaksymalna liczba klientow: %d\n", port, max_client);


  
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  int *thread_sock;

  
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  
    if(listen(welcomeSocket,max_client)==0)
      printf("Listening\n");
    else
      printf("Error\n");
 
  



    //newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    addr_size = sizeof(struct sockaddr_in);
    

    while( (newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, (socklen_t*)&addr_size)) )
    {
        
         puts("Connection accepted");
        pthread_t new_thread;
        thread_sock = malloc(1);
        *thread_sock = newSocket;
         
        if( pthread_create( &new_thread , NULL ,  connection_handle , (void*) thread_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         

        
    }
  
  
  return 0;
}


/*
TO DO:


- protokol
- wystawienie interface'ow
- zobaczyc na warningi
- logi
- zrobic server jako demon
- 

*/