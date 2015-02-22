#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define SERV_PORT	23456
/*Received code
 * 1 name of file 
 *
 *Sent code
 *  -1 error
 *   2 file data
 * 
 * */
struct buf{
	   int code;
	   char buff[100]; 
          };

int main()
{
    int	listenfd, connfd, i = 0, ret, bytes,fd;
    char connected, buffer[100] ;
    struct buf servBuf;
    //   pid_t   childpid;
    socklen_t	clilen;
    struct sockaddr_in	cliaddr, servaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket %d is created\n", listenfd) ; 
   
    if(listenfd == -1)
    {
	perror("Error in socket creation") ; 
	exit(-1) ; 
    }
        
    memset(&servaddr, 0 , sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);
    
    if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))
       == -1)
    {
	perror("Error in bind") ; 
	exit(-2) ; 
    }
    
    while(1)
    {
	if(listen(listenfd, 10) == -1) 
	{
	    perror("Error in listen") ; 
	    exit(-3) ; 
	}
	
    	clilen = sizeof(cliaddr);
	
	connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
	
	if ( connfd == -1) 
	{
	    perror("Error in accept") ;
	    exit(-4) ;
	}

	printf("client is connected using socket %d\n", connfd) ; 
	
	connected = 1 ; 
	
	while(connected)
	{
	    connected ++ ; 

	    memset(&servBuf,0,sizeof(struct buf));
	    
	    ret = recv(connfd,&servBuf,sizeof(struct buf), 0) ; 

	    
	    if(ret == -1)
	    {
		perror("Error in recv") ; 
		exit(-5) ;
	    }
	    
	    if(ret == 0)
	    {
		close(connfd) ; 
		connected = 0 ; 
		continue ; 
	    }
	    
	    printf("%d Server received \n ",connected) ; 

	    i = 0 ; 
	    
            if(servBuf.code==1)
            {
	       fd=open(servBuf.buff,O_RDONLY);
               /*Error in opening file*/
	       if(fd<0)
               {
                   servBuf.code=-1;
                   bytes = send(connfd,&servBuf,ret,0);
                   if(bytes == -1)
                   {   perror("Error in send") ;exit(-5) ; }
               }
               /*Send the file*/
               servBuf.code=2; 
               while(read(fd,&servBuf.buff,100))
               {
                    bytes=send(connfd,&servBuf,ret,0);
                    if(bytes == -1) 
                    {   perror("Error in send") ;exit(-5) ; }
               }
               close(fd);
            }

         } 
		
	    
    }
}


