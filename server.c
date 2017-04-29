

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <libconfig.h>

int get_int_from_settings(config_t cfg, const char *c){
  int ret_value;

  if (config_lookup_int(&cfg, c, &ret_value))
  {
    return ret_value;
  }else{
    return -1;
  }

}

int main(){

  config_t cfg;
  config_setting_t *setting;
  const char *str;
  int max_client, tmp, port;

  config_init(&cfg);


  if(! config_read_file(&cfg, "konfiguracja.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    printf("nie ma pliku konfiguracyjnego");
    port = 7891;
    max_client = 3;
  }

  max_client = get_int_from_settings(cfg, "max_client");
  port = get_int_from_settings(cfg, "port");
  printf ("port: %d   \nmaksymalna liczba klientow: %d\n", port, max_client);





  
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
  config_destroy(&cfg);
  return 0;
}


/*
TO DO:
- komunikacja w 2 strony
- kilku klientow
- plik config DONE
- protokol
- wystawienie interface'ow
- zobaczyc na warningi
- zrobic server jako demon (+logi)
- 

*/