
all: bib.o libbib.so server.o server clean

bib.o: server_library.c
	gcc -c -fpic -Werror -Wall -o bib.o server_library.c

libbib.so: bib.o
	gcc -shared -o libbib.so bib.o

server.o: server.c 
	gcc -c -o server.o server.c 

server: server.o libbib.so
	gcc -L/home/tomek/zaawansowane_programowanie_linux/lab2/tcpip -Wall -o server server.o -lbib -l:libconfig.a -lpthread

clean:
	-rm -f bib.o server.o 

