/*client*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>

int main()
{
	int ret,len,pid;
	int socket_fd;
	char ch;
	struct sockaddr_un address;

	socket_fd = socket(AF_UNIX,SOCK_STREAM,0);

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path,"server_socket");
	len = sizeof(address);

	ret = connect(socket_fd,(struct sockaddr *)&address,len);
	if (ret == -1) {
		perror("error in connect:");
		exit(EXIT_FAILURE);
	}
	
	pid = getpid();
	ch = 'A';
	write(socket_fd,&ch,1);
	printf("pid=%d sent ch=%c \n",pid,ch);
	read(socket_fd,&ch,sizeof(ch));
	printf("pid=%d received ch=%c \n",pid,ch);
	close(socket_fd); 
	return EXIT_SUCCESS;
}
