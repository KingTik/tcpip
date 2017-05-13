#include <stdio.h>
#include <stdlib.h>
#include "server_library.h"
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
char * get_ip(){

	
        struct ifaddrs *ifaddr, *ifa;
        int family, s;
        char host[NI_MAXHOST];
        char *msg = (char *)malloc(sizeof(char)*80);
        memset(msg,'\0', 80);
        if (getifaddrs(&ifaddr) == -1) {
            perror("getifaddrs");
            exit(EXIT_FAILURE);
        }

        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            family = ifa->ifa_addr->sa_family;

            if (family == AF_INET) {
                s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                                              host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                if (s != 0) {
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }
                
              strcat(msg,(char*)host);
              strcat(msg,"\n");
            }
            
        }

          
    return msg;
}