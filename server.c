

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <libconfig.h>

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
 
  



    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    addr_size = sizeof serverStorage;
    
    
    while( (read_size = recv(newSocket, buffer, 1024, 0)) >0 ){

      switch (message_handle(buffer))
      {
      case 0:
        write(newSocket,"thats nice sweety\n",17,0);
        printf("Data received: %s \n",buffer);
        break;
      case 1:
        close(welcomeSocket);
        config_destroy(&cfg);
        return 0;
        break; 
      case 2:
       
        write(newSocket,"hi yourself\n",13,0);
        break;
      case 3:
        write(newSocket,"quit\nhi\nhelp\n*",17,0);
        break;
      default:
        printf("wait what\n");// nie powinno nigdy to wejsc
        break;
      }
    }

  
  
  return 0;
}


/*
TO DO:

- kilku klientow
- protokol
- wystawienie interface'ow
- zobaczyc na warningi
- zrobic server jako demon (+logi)
- 

*/