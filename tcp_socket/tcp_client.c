#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERV_PORT 23456
#define MAXLINE 80

int
main(int argc, char **argv)
{
    int	sockfd;
    struct sockaddr_in	servaddr;
    char   sendline[MAXLINE];
    char   string[MAXLINE];
    
    
    if (argc != 2) {
	printf("usage: tcpcli <IPaddress>");
	exit (0);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Client socket created %d\n", sockfd) ; 
    
    if(sockfd == -1)
    {
	perror("error in socket") ;
	exit(-1) ;
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
	
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
    {
	perror("error in connect") ;
	exit(-2) ;
    }
     while(1)
    {
 
    scanf("%s", sendline) ; 

    if(write(sockfd, sendline, sizeof(sendline)) == -1)
    {
	perror("error in write") ; 
	exit(-3) ;
    }

    if (read(sockfd, string, sizeof(string)) == 0) 
    {
	printf("Client: server terminated prematurely");
	exit (0);
    }

    printf("Server returned %s\n", string);
    }
    close(sockfd) ;
    return 0 ; 
}

