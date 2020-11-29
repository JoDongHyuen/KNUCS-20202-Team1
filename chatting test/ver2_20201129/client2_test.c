#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>
#include<fcntl.h>
#include<ctype.h>
#include <pthread.h>

#define oops(msg) {perror(msg); exit(1);}

int sock_id;

void *thread_write(void* nul) //thread function
{
	char my_chat[BUFSIZ];

	while(1)
	{
		if(fgets(my_chat,BUFSIZ,stdin) != NULL)
		{
			write(sock_id,my_chat,strlen(my_chat));
		}
	}
}

int main(int argc,char *argv[])
{
	
	struct sockaddr_in caddr;
	struct hostent *hp;
	
	int i,j,read_cnt,pid;
	char chat[BUFSIZ];

	void *pthread_write(void*);	
	pthread_t t1; //one thread

	if(argc!=4)
	{
		fprintf(stderr,"실행방법 :./실행파일 [서버ip] [서버포트번호] [이름] -lpthread\n");
		exit(1);
	}
	
	//socket설정 및 connect 요청
	sock_id = socket(PF_INET,SOCK_STREAM,0);
	if(sock_id == -1) oops("socket");

	bzero((char*)&caddr,sizeof(caddr));
	hp = gethostbyname(argv[1]);
	bcopy((void*)hp->h_addr,(void*)&caddr.sin_addr,hp->h_length);
	caddr.sin_port = htons(atoi(argv[2]));
	caddr.sin_family = AF_INET;
	
	if(connect(sock_id,(struct sockaddr*)&caddr,sizeof(caddr))!=0) oops("connect");
	
	printf("게임서버에 접속되었습니다.\n");
	
	//nonblocking모드로 전환
	int flag = fcntl(sock_id,F_GETFL,0);
	fcntl(sock_id,F_SETFL,flag | O_NONBLOCK);
	
	//쓰레드를 이용하여 메시지를 보내는것과 받는것을 따로 수행함.
	pthread_create(&t1, NULL, thread_write, (void *)NULL);

	while(1)
	{
		read_cnt = read(sock_id,chat,sizeof(chat));
		if(read_cnt > 0)//서버로부터 메시지 받아옴
		{
			chat[read_cnt] = 0;
			write(1,chat,read_cnt);
			//fflush(sock_id);
		}

	}



	return 0;

}
