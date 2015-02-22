#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERV_PORT 23456
#define MAXLINE 100

struct buf{
            int code;
            char buff[100];
          }

int main(int argc, char **argv)
{
    int	sockfd,ret,fd;
    struct sockaddr_in	servaddr;
    char   sendline[MAXLINE];
    char   string[MAXLINE];
    struct buf cliBuf;
    
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
 
        
        printf("Enter the file name\n");
        scanf("%s",cliBuf.buff) ; 
        strcpy(string,cliBuf.buff);

        cliBuf.code=1;
        /*Send the file name*/ 
        if(write(sockfd, cliBuf, sizeof(struct buf)) == -1)
        {
           perror("error in write") ; 
	   exit(-3) ;
        }

        ret=read(sockfd,cliBuf,sizeof(struct buf));
        if (ret == 0) 
        {
	   printf("Client: server terminated prematurely");
	   exit (0);
        }
        
        /*received the file*/
        if(cliBuf.code==2)
        {
           fd=open(string,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
           while(cliBuf.code==2)
           {
               write(fd,cliBuf.buff,100);  
               
           }        
           close(fd);
        } 
        

        printf("Server returned %s\n", string);
    }
    close(sockfd) ;
    return 0 ; 
}

