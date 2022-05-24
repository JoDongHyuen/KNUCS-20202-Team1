#include "socklib.h"

#define oops(msg) {perror(msg); exit(1);}

void add_usr(int);
void remove_usr(int);
void *thread_server_write(void*);


int num_user=0; //참가자수
int user_sock[8]; //최대 8명의 참가자
char user_name[8][BUFSIZ];//참가자들의 닉네임

void *thread_server_write(void* nul) //thread function
{
	int j;
	char my_chat[BUFSIZ];

	while(1)
	{
		if(fgets(my_chat,BUFSIZ,stdin) != NULL) //내 메시지를 모든유저에게 발송
		{
			for(j=0; j<num_user; j++)
			{
				write(user_sock[j],"server>",8);
				write(user_sock[j],my_chat,strlen(my_chat));		
			}

		}
	}
}

void server(int portnum)
{

	int client_fd;
	int i,j,read_cnt;
	char chat[BUFSIZ];

	void *pthread_server_write(void*);	
	pthread_t t1; //one thread

	int sock_id = 0;
	sock_id= make_server_socket(portnum);
	if(sock_id == 0) exit(1);
	printf("\n\n******게임서버에 접속되었습니다.******\n\n");
	
	//nonblocking모드로 전환
	int flag = fcntl(sock_id,F_GETFL,0);
	fcntl(sock_id,F_SETFL,flag | O_NONBLOCK);

	//쓰레드를 이용하여 메시지를 보내는것과 받는것을 따로 수행함.
	pthread_create(&t1, NULL, thread_server_write, (void *)NULL);

	while(1)
	{
		if(num_user < 8)
		{
			client_fd = accept(sock_id,NULL,NULL);
			if( client_fd != -1)//유저채팅가입 처리(최대 8명)
				add_usr(client_fd);
		}		

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
				{
					if(i != j)
					{
						write(user_sock[j],user_name[i],strlen(user_name[i]));
						write(user_sock[j],">",2);
						write(user_sock[j],chat,read_cnt);
					}
				}

				printf("%s>%s",user_name[i],chat);
			}		
			
		}
		/*마피아 게임 시작후 프로세스 흐름 추가*/

	}


}

void add_usr(int s)
{
	int read_cnt=0;
	char name[BUFSIZ];

	//client_socket 저장, nonblocking 모드 전환
	user_sock[num_user] = s;

	//유저닉네임을 받음
	read_cnt = read(user_sock[num_user],name,sizeof(name));
	name[read_cnt] = 0;
	strcpy(user_name[num_user],name);	
	printf("%s\n",user_name[num_user]);

	//socket을 nonblocking모드로 전환
	int flag = fcntl(s,F_GETFL,0);
	fcntl(s,F_SETFL,flag | O_NONBLOCK);
	
	num_user++;
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
		strcpy(user_name[j-1],user_name[j]);
	}

	/*서버에 유저나갔다는 메시지추가*/
	printf("유저가 나갔습니다. 현재 유저수 : %d\n",num_user);
}
