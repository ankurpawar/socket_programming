/*Server code*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>

int main()
{
	int server_fd,client_fd;
	struct sockaddr_un server_add;
	struct sockaddr_un client_add;
	int server_len,client_len;
	int ret,len;

	unlink("server_socket");

	server_fd = socket(AF_UNIX,SOCK_STREAM,0);
	if (server_fd == -1) {
		perror("error in creating socket:");
		exit(EXIT_FAILURE);	
	}

	server_add.sun_family = AF_UNIX;
	strcpy(server_add.sun_path,"server_socket");
	server_len = sizeof(server_add);
	ret = bind(server_fd,(struct sockaddr*)&server_add,server_len);
	if (ret == -1) {
		perror("error in bind:");
		exit(EXIT_FAILURE);	
	}

	listen(server_fd,5);

	while (1) {
		char ch;
		printf("in server\n");
		
		client_len = sizeof(client_add);
		ret = accept(server_fd,(struct sockaddr*)&client_add,&client_len);
		if (ret == -1) {
			perror("error in accepting:");
			exit(EXIT_FAILURE);
		}

		read(client_fd,&ch,1);
		printf("client send ch = %c\n",ch);
		ch = 'B';
		write(client_fd,&ch,sizeof(char));
		printf("server sent ch =%c\n",ch);
		close(client_fd);
	}

	return EXIT_SUCCESS;
}

