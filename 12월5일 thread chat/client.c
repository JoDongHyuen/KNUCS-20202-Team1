#include "socklib.h"

#define oops(msg) {perror(msg); exit(1);}

void *thread_client_write(void*);

int sock_id1=0;

void *thread_client_write(void* nul) //thread function
{
	char my_chat[BUFSIZ];

	while(1)
	{
		if(fgets(my_chat,BUFSIZ,stdin) != NULL)
		{
			write(sock_id1,my_chat,strlen(my_chat));
		}
	}
}

void client(int portnum,char *ip)
{

	int read_cnt;
	char chat[BUFSIZ];

	void *pthread_client_rite(void*);	
	pthread_t t1; //one thread
	
	//서버에게 닉네임을 보냄
	printf("닉네임을 입력하십시오. : ");
	scanf("%s",chat);
	fflush(stdin);

	sock_id1 = connect_to_server(ip, portnum);
	if(sock_id1 == 0) exit(1);

	write(sock_id1,chat,strlen(chat));

	printf("\n\n******게임서버에 접속되었습니다.******\n\n");
	
	//nonblocking모드로 전환
	int flag = fcntl(sock_id1,F_GETFL,0);
	fcntl(sock_id1,F_SETFL,flag | O_NONBLOCK);
	
	//쓰레드를 이용하여 메시지를 보내는것과 받는것을 따로 수행함.
	pthread_create(&t1, NULL, thread_client_write, (void *)NULL);

	while(1)
	{
		read_cnt = read(sock_id1,chat,sizeof(chat));
		if(read_cnt > 0)//서버로부터 메시지 받아옴
		{
			chat[read_cnt] = 0;
			write(1,chat,read_cnt);
		}

	}

}
