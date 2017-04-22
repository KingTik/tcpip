all: server client


server: server.c
	gcc server.c -o server

client: client.c
	gcc client.c -o client


clean:
	rm -f server client
