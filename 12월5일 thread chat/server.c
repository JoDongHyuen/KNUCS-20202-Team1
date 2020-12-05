#include "socklib.h"

#define oops(msg) {perror(msg); exit(1);}
#define DAY 10								//낮은 10
#define NIGHT 20							//밤은 20
void add_usr(int);
void remove_usr(int);
void *thread_server_write(void*);


int num_user=0;											//참가자수
int user_sock[8];										//최대 8명의 참가자
char user_name[8][BUFSIZ];								//참가자들의 닉네임
int client_fd;
int read_cnt;
char chat[BUFSIZ];
int sock_id = 0;
/*							게임을 위해 추가한 변수							*/
int user_roles[8];										//유저의 역할
int vote_users[8];										//유저별 투표수 저장
int abstention = 0;										//기권
int time = 10;											//낮인지 밤인지 구분해주는 변수
int current_role = 0;									//밤에 현재 역할이 누구냐를 구분하는 변수 마피아 -1 경찰 2 의사 3
int police = -1, doctor = -1, mapia[3] = { -1 };		//경찰, 의사, 마피아로 지목된 사람 저장
int num_mapia = 0, num_civil = 0;
//
void make_user_roles(int num_user) {

	int map = 1;										//마피아와 경찰 의사는 기본 1명
	int pol = 1;
	int doc = 1;
	int civ;
	int map_count = 0;
	int pol_count = 0, doc_count = 0;
	int temp = 0;
	int map_count = 0;

	switch (num_user) {									//유저 수에 따라 마피아 수가 달라짐
	case 6: map = 2; break;
	case 7: map = 2; break;
	case 8: map = 3; break;
	}

	civ = num_user - (map + pol + doc);					//시민은 남은 인원

	srand((unsigned int)time(NULL));					//랜덤으로 분배

	while (map_count < map) {							//마피아 먼저 정하기
		temp = rand() % num_user + 1;
		if (u_roles[temp] == 0) {
			u_roles[temp] = -1;
			mapia[map_count++] = temp;
			map_count++;
			num_mapia++;
		}
		else continue;
	}

	while (!pol_count || !doc_count) {		//의사와 경찰 정하기
		temp = rand() % num_user + 1;			//if문 안에 있던 오류 수정
		if (u_roles[temp] == 0 && pol_count == 0) {
			u_roles[temp] = 2;
			police = temp;
			pol_count = 1;
		}
		temp = rand() % num_user + 1;			//if문 안에 있던 오류 수정
		if (u_roles[temp] == 0 && doc_count == 0) {
			u_roles[temp] = 3;
			doctor = temp;
			doc_count = 1;
		}
	}
	for (int i = 1; i <= num_user; i++) {		//남은 인원들은 모두 시민
		if (u_roles[i] == 0)
			u_roles[i] = 1;
	}
	num_civil = num_user - (num_mapia + 1);
}

/*--------------------------------------------------------------------night------------------------------------------------------------------------------------*/
int choice_kill() {								//마피아가 밤에 행동하는 알고리즘
	current_role = -1;
	int who_kill = -1;

	print_now_users(u_roles, num_user);									//현재 남은 유저 프린트해주고 고르게하기

	while (1) {
		printf("죽일 사람을 고르십시오.\n");
		scanf("%d", &who_kill);
		if (who_kill >= 0 && who_kill < num_user) {						//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
			if (!u_roles[who_kill]) {									//이미 죽은사람을 고르면 다시 고르게하기
				printf("이미 죽은 사람입니다. 다시 고르세요\n");
				who_kill = -1;
			}
			else {
				printf("%d번 플레이어를 죽입니다.\n", who_kill);			//맞게 고르면 출력하고
				break;
			}
		}
		else {
			printf("잘못입력하였습니다.\n");
			who_kill = -1;
		}
	}
	current_role = 0;
	return who_kill;													//죽일 유저 리턴

}
void choice_invest() {							//마피아 알고리즘과 흡사합니다.

	int who_invest = -1;
	current_role = 2;
	print_now_users(u_roles, num_user);



	while (1) {
		printf("조사할 사람을 고르십시오.\n");
		scanf("%d", &who_invest);
		if (who_invest >= 0 && who_invest < num_user) {					//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
			if (u_roles[who_invest] == 0) {
				printf("이미 죽은 사람입니다.");
				who_invest = -1;
			}
			else
				break;
		}
		else {
			printf("잘못입력하였습니다.\n");
			who_invest = -1;
		}
	}
	current_role = 0;
	if (u_roles[who_invest] == -1) {									//마피아가 맞는지 아닌지만 경찰에게 알려주는 부분
		printf("player %d는 마피아가 맞습니다.\n", who_invest);
		return;
	}
	else {
		printf("player %d는 마피아가 아닙니다.\n", who_invest);
		return;
	}


}
int choice_save() {								//마피아 알고리즘에서 변수 이름만 바뀌고 살릴 사람 리턴하는 함수

	int who_save = -1;
	current_role = 3;
	print_now_users(u_roles, num_user);

	while (1) {
		printf("살릴 사람을 고르십시오.\n");
		scanf("%d", &who_save);
		if (who_save >= 0 && who_save < num_user) {						//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
			if (!u_roles[who_save]) {
				printf("이미 죽은 사람입니다. 다시 고르세요");
				who_save = -1;
			}
			else {
				printf("%d번 플레이어를 살립니다.\n", who_save);
				break;
			}
		}
		else {
			printf("잘못입력하였습니다.\n");
			who_save = -1;
		}
	}
	current_role = 0;
	return who_save;

}
/*--------------------------------------------------------------------night------------------------------------------------------------------------------------*/

void night() {
	
	int who_kill = -1, who_save = -1;

	printf("밤이 되었습니다.");
	for (j = 0; j < num_user; j++)
	{
		write(user_sock[j], "밤이 되었습니다.\n", 18);
	}
	if (user_roles[police] != 0)
		choice_invest();
	else
		sleep(5);
	
	who_kill = choice_kill();
	if (user_roles[doctor] != 0)
		who_save = choice_save();
	else
		sleep(5);

	if (who_kill == who_save) {
		printf("player %d가 의사에 의해 살아났습니다!\n", who_save);
		for (j = 0; j < num_user; j++)
		{
				write(user_sock[j], "player %d가 의사에 의해 살아났습니다!\n", 39);
		}
	}
	else {
		user_roles[who_kill] = 0;
		printf("player %d가 죽었습니다...\n", who_kill);
		for (j = 0; j < num_user; j++)
		{
			write(user_sock[j], "player %d가 죽었습니다...\n", 27);
		}
	}

	time = DAY;

}

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


void *game_chat(void* nul){

	int i, j;
	
	while(1)
    {
		if (time == DAY) {
			if (num_user < 8)
			{
				client_fd = accept(sock_id, NULL, NULL);
				if (client_fd != -1)//유저채팅가입 처리(최대 8명)
					add_usr(client_fd);
			}

			for (i = 0; i < num_user; i++)
			{
				read_cnt = read(user_sock[i], chat, sizeof(chat));

				if (read_cnt == 0)//유저채팅탈퇴 처리
					remove_usr(i);


				/*else if(exit 입력시 유저탈퇴처리)추가*/

				else if (read_cnt > 0)//모든유저에게 메시지 발송
				{
					chat[read_cnt] = 0;

					for (j = 0; j < num_user; j++)
					{
						if (i != j)
						{
							write(user_sock[j], user_name[i], strlen(user_name[i]));
							write(user_sock[j], ">", 2);
							write(user_sock[j], chat, read_cnt);
						}
					}

					printf("%s>%s", user_name[i], chat);
				}

			}
		}
		else if (time == NIGHT) {

			night();

		}
		else {
			fprintf(stderr, "TIME ERROR\n");
			exit(1);
		}

    }

}

void server(int portnum)
{
	int gameOn = 1;
	void *pthread_server_write(void*);	
	pthread_t t1; //one thread
	pthread_t t2;

	sock_id= make_server_socket(portnum);
	if(sock_id == 0) exit(1);
	printf("\n\n******게임서버에 접속되었습니다.******\n\n");
	
	//nonblocking모드로 전환
	int flag = fcntl(sock_id,F_GETFL,0);
	fcntl(sock_id,F_SETFL,flag | O_NONBLOCK);

	//쓰레드를 이용하여 메시지를 보내는것과 받는것을 따로 수행함.
	pthread_create(&t1, NULL, thread_server_write, (void *)NULL);
	
	//게임에 이용되는 채팅을 쓰레드로 수행함.
	pthread_create(&t2, NULL, game_chat, (void *)NULL);

	while(gameOn)// if (게임이 시작 되었을 때)
	{
		/*0.직업 배정하는 파트*/
		if (gameOn == 1) {								//시작하면 직업 배정해줌
			make_user_roles(num_user);
		}
		
		/*1.낮이 되었습니다 파트*/


		/*2.투표 파트*/

		/*3.밤이 되었습니다 직업별로 사람 선택*/
		time = NIGHT;
		/*다시 낮이 되었습니다 루프 -> 이 부분은 while문 돌면서 1번으로 돌아가는 부분이라 추가할 필요 없을듯*/

		/*4.승리 조건 만족하면 루프 탈출*/
		if (num_mapia >= num_civil) {
			for (j = 0; j < num_user; j++)
			{
				if (i != j)
				{
					printf("마피아 승리!");
					write(user_sock[j], "마피아 승리!", 13);
				}
			}
			break;
		}
		else if (num_maia <= 0) {
			for (j = 0; j < num_user; j++)
			{
				if (i != j)
				{
					printf("시민 승리!");
					write(user_sock[j], "시민 승리!", 11);
				}
			}
			break;
		}
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
