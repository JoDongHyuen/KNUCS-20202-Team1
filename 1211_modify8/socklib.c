
#include "socklib.h"

#define oops(msg) {perror(msg); exit(1);}

#define   HOSTLEN  256
#define	  BACKLOG  5

int make_server_socket(int portnum)
{
	return make_server_socket_q(portnum, BACKLOG);
}
int make_server_socket_q(int portnum, int backlog)
{
	struct sockaddr_in saddr;   
	struct hostent	*hp;   
	char hostname[HOSTLEN];     
	int sock_id;	       
	
	//socket설정 및 bind설정
	sock_id = socket(PF_INET, SOCK_STREAM, 0);  
	if ( sock_id == -1) 
		oops("socket");

	bzero((void *)&saddr, sizeof(saddr));   
	gethostname(hostname, HOSTLEN);         
	hp = gethostbyname(hostname);          
	                                       
	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(portnum);       
	saddr.sin_family = AF_INET ;           
	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)	
	       oops("bind");
	
	//listen, 클라이언트로 부터 수연결요청 기다림
	if (listen(sock_id, backlog) != 0) oops("listen");

	return sock_id;
}

int connect_to_server(char *host, int portnum)
{
	int sock;
	struct sockaddr_in servadd;      
	struct hostent *hp;            

	//socket설정 및 connect 요청
	sock = socket(PF_INET, SOCK_STREAM, 0); 
	if ( sock == -1) 
		oops("socket");

	bzero(&servadd, sizeof(servadd));     
	hp = gethostbyname(host);             
	if (hp == NULL) 
		return -1;
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(portnum);     
	servadd.sin_family = AF_INET ;          

	if (connect(sock,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
	{
		fprintf(stderr,"서버 연결에 실패했거나, 인원이 초과되었습니다.\n" );
		exit(1);
	}

	return sock;
}
