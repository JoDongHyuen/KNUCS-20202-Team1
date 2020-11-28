#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>

#define PORTNUM  13000 
#define HOSTLEN  256
#define oops(msg) { perror(msg) ; exit(1) ; }

void *thread_test(void* nul) //thread function
{
	int i;
	for (i = 5; i >= 1; i--){
		printf("Hello I'm Thread %dsecond\n", i);
		sleep(1);
	}
}


int main(int argc, char *argv[])
{
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id, sock_fd;
	FILE *sock_fp;
	char *ctime(); 
	time_t thetime;
	int i;
	void *pthread_test(void*);
	int s;
	pthread_t t1; //one thread

	sock_id = socket(PF_INET, SOCK_STREAM, 0); 
	if (sock_id == -1) 
		oops("socket");

	bzero((void*)&saddr, sizeof(saddr) );
	gethostname(hostname, HOSTLEN); 
	hp = gethostbyname(hostname);
	                              
	bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET ;

	if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0 )
	       oops("bind");


	if (listen(sock_id, 3) != 0 ) 
		oops("listen");

	while ( 1 ){
	       sock_fd = accept(sock_id, NULL, NULL);
	       printf("Wow! got a call!\n");
	       
	       if (sock_fd == -1)
		       oops("accept");
	       
	       sock_fp = fdopen(sock_fd,"w");
	       if (sock_fp == NULL)
		       oops("fdopen");
	       
	       /*new command for thread create*/
               pthread_create(&t1, NULL, thread_test, NULL);

	       
	       //setbuf(sock_fp, 0);
	       thetime = time(NULL);
	       fprintf(sock_fp, "The time here is ..");
	       fprintf(sock_fp, "%s", ctime(&thetime));
	       sleep(1);

	      
	       for (i = 5; i >= 1; i--){
		       fprintf(sock_fp, "I'm sleeping now Remain : %d\n", i);
		       //fflush(sock_fp);
		       sleep(1);
	       }

	      

	       fclose(sock_fp);
	       /*new command for pthread_join(exit?)*/
	       pthread_join(t1, NULL);
	       //fclose(sock_fp);
	}
}
