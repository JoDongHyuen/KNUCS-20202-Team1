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
#define HOSTLEN 256

void add_usr(int);
void remove_usr(int);

int num_user=0; //참가자수
int user_sock[8]; //최대 8명의 참가자
FILE* users[8];
int sock_id, client_fd;
int read_cnt;
char chat[BUFSIZ], *read_rt;


void *waiting_room(void* nul)
{
	int k, l;
	
	while(1)
	{
		client_fd = accept(sock_id,NULL,NULL);
		if(client_fd != -1)//유저채팅가입 처리
			add_usr(client_fd);

		for(k=0; k<num_user; k++)
		{
			read_cnt=read(user_sock[k],chat,sizeof(chat));

			if(read_cnt == 0)//유저채팅탈퇴 처리
				remove_usr(k);
		
			else if(read_cnt > 0)//모든유저에게 메시지 발송
			{
				chat[read_cnt] = 0;
				for(l=0; l<num_user; l++)
					write(user_sock[l],chat,read_cnt);
				//fprintf(users[i],"%s\n",chat);
				printf("%s",chat);
			}
		}
	}
}

int main(int argc,char *argv[])
{
	
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];

	char command[BUFSIZ];
	int i,j;

	pthread_t room;

	if(argc!=2)
	{
		fprintf(stderr,"실행방법 : ./실행파일 [포트번호]\n");
		exit(1);
	}
	
	//socket설정 및 bind설정
	sock_id = socket(PF_INET,SOCK_STREAM,0);
	if(sock_id == -1) oops("socket");

	bzero((char*)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void*)hp->h_addr,(void*)&saddr.sin_addr,hp->h_length);
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_family = AF_INET;
	
	if(bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");
	
	//listen, 클라이언트로 부터 수연결요청 기다림
	if(listen(sock_id,5)!=0) oops("listen");
	
	//nonblocking모드로 전환
	int flag = fcntl(sock_id,F_GETFL,0);
	fcntl(sock_id,F_SETFL,flag | O_NONBLOCK);

	pthread_create(&room, NULL, waiting_room, (void *)NULL);
	while(1)
	{
		/*before game start*/
		command = fgets();
		if (strcmp(command, "start") == 0){

		}

		while(1)
		{

		/*직업 배정하는 파트*/

		/*낮이 되었습니다 파트*/

		/*투표 파트*/

		/*밤이 되었습니다 직업별로 사람 선택*/

		/*다시 낮이 되었습니다 루프*/

		/*승리 조건 만들어서 루프 탈출*/
		}

	}


	return 0;

}

void add_usr(int s)
{
	//client_socket 저장, nonblocking 모드 전환
	user_sock[num_user] = s;
	
	int flag = fcntl(s,F_GETFL,0);
	fcntl(s,F_SETFL,flag | O_NONBLOCK);

	
	if((users[num_user++] = fdopen(s,"rw")) == NULL)
		oops("fdopen");
	
	/*서버에 유저수 출력 및 유저 들어왔다는 메시지추가*/
	printf("유저가 들어왔습니다. 현재 유저수 : %d\n",num_user);
}

void remove_usr(int i)
{
	int j;
	close(user_sock[i]);

	num_user--;

	for(j=num_user; j>i; j--)
	{
		user_sock[j-1] = user_sock[j];
	}

	/*서버에 유저나갔다는 메시지추가*/
	printf("유저가 나갔습니다. 현재 유저수 : %d\n",num_user);
}
