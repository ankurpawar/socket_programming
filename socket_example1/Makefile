all : server client
server : server.o
	gcc server.o -o server
server_o : server.c
	gcc server.c -c server.o
client : client.o
	gcc client.o -o client
client_o : client.c
	gcc client.c -c client.o
clean :
	rm *.o server client

