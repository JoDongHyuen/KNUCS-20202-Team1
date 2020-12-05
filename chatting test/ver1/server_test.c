#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>
#include<ctype.h>

#define PORTNUM  15000  
#define HOSTLEN  256
#define oops(msg) { perror(msg); exit(1); }

void sanitize(char* str);
int main(int argc, char *argv[])
{
	struct  sockaddr_in   saddr;   
	struct	hostent	*hp;   
	char hostname[HOSTLEN];

	char chat[BUFSIZ];     
	int sock_id,sock_fd[10];
	int i=-1,j,k,len;      	//i = 참가자수-1
	//FILE* sock_fpi[10], *sock_fpo[10];               

	sock_id = socket(AF_INET, SOCK_STREAM, 0);   
	if (sock_id == -1 ) 
		oops( "socket" );

	bzero( (void *)&saddr, sizeof(saddr) ); 
	gethostname( hostname, HOSTLEN );     
	hp = gethostbyname( hostname );      
	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);     
	saddr.sin_family = AF_INET ;          
	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

	if ( listen(sock_id, 5) != 0 ) //queue 사이즈 =5
		oops( "listen" );

	while ( 1 )
	{
	       	sock_fd[++i] = accept(sock_id, NULL, NULL);
	       	if( sock_fd[i] == -1 ) oops("accept"); 

		//read(sockfd, buf, len) == recv(sockfd, buf, len, 0)
		//send(clisock_list[j], buf, nbyte, 0);
	      
		for(j=0; j<=i; j++)
		{
			
			len = read(sock_fd[j], chat, BUFSIZ);
			chat[len] = 0;			
			
			if (strstr(chat, "exit") != NULL) {
				close(sock_fd[j]);	// 클라이언트의 종료
				continue;
			}

			
			for(k=0; k<=i; k++)
				write(sock_fd[k],chat,len);
			printf("%s\n",chat);
			
		}

	    	//fclose(sock_fpo);
	     	//fclose(sock_fpi);
       }
}

