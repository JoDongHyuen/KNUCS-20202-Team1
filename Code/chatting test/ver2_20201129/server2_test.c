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

#define oops(msg) {perror(msg); exit(1);}
#define HOSTLEN 256

void add_usr(int);
void remove_usr(int);

int num_user=0; //참가자수
int user_sock[8]; //최대 8명의 참가자
FILE* users[8];

int main(int argc,char *argv[])
{
	
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];

	int sock_id,client_fd;
	int i,j,read_cnt;
	char chat[BUFSIZ],*read_rt;

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

	while(1)
	{
		client_fd = accept(sock_id,NULL,NULL);
		if(client_fd != -1)//유저채팅가입 처리
			add_usr(client_fd);
		/*유저 이름 받아오기 및 인원수 제한처리 추가*/

		for(i=0; i<num_user; i++)
		{				 
			read_cnt=read(user_sock[i],chat,sizeof(chat));

			if(read_cnt == 0)//유저채팅탈퇴 처리
				remove_usr(i);
			

			/*else if(exit 입력시 유저탈퇴처리)추가*/	

			else if(read_cnt > 0)//모든유저에게 메시지 발송
			{
				chat[read_cnt] = 0;
						
				for(j=0; j<num_user; j++)
					write(user_sock[j],chat,read_cnt);
					//fprintf(users[i],"%s\n",chat);
						
				printf("%s",chat);
			}		
			
		}
		/*마피아 게임 시작후 프로세스 흐름 추가*/

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
	fclose(users[i]);	

	num_user--;

	for(j=num_user; j>i; j--)
	{
		user_sock[j-1] = user_sock[j];
	}

	/*서버에 유저나갔다는 메시지추가*/
	printf("유저가 나갔습니다. 현재 유저수 : %d\n",num_user);
}
