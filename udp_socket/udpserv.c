#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERV_PORT	22222
#define MAXLINE		80

int main()
{
    int i, len, sock, bytes, ret;
    struct sockaddr_in	servaddr, cliaddr;
    
    char buffer[100] ;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(sock == -1)
    {
	    perror("Error in socket creation") ;
	    exit(-1) ;
    }
    
    memset(&servaddr, 0,  sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);
    
    
    if(bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr))
       == -1)
    {
	perror("Error in bind") ;
	exit(-2) ;
    }
    
    while(1)
    {
	memset(buffer, 0, 100) ; 
	len = sizeof(cliaddr) ;
	ret = recvfrom(sock, buffer, MAXLINE, 0,  
		       (struct sockaddr *)&cliaddr,(socklen_t *) &len);
	
	if(ret == -1)
	{
	    perror("Error in recvfrom") ;
	    exit(-3) ;
	}
	
	/*if(ret == 0)
	  {
	  close(connfd) ;
	  
	  exit(-4) ;
	  }*/
	
	printf ("Client says: %s\n", buffer) ;	
	
	i = 0 ;
	
	while(buffer[i])
	{
	    if(buffer[i] >= 'a' && buffer[i]<='z')
	    {
		buffer[i] = buffer[i] - ('a'-'A') ;
	    }
	    
	    i++ ;
	}
	
	bytes = sendto(sock, buffer, ret, 0,
		       (struct sockaddr *)&cliaddr, len);
	
	if(bytes == -1)
	{
	    perror("Error in send") ;
	    exit(-5) ;
	}
	
	printf("return value of sendto %d\n", bytes) ; 
	
    }
}
