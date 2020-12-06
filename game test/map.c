#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MALLOC(p,s)\
if (!(p = malloc(s))) {\
	fprintf(stderr, "memory allocation error\n");\
	exit(1);\
}

void make_user_roles(int u_roles[], int users);
void vote_mapia(int u_roles[], int users);
int choice_kill(int u_roles[], int users);
void choice_invest(int u_roles[], int users);
int choice_save(int u_roles[], int users);
void print_now_users(int u_roles[], int users);


int main() {
	int num_user = 0;
	int *user_roles;					//유저의 역할. 1: 시민 2: 경찰 3: 의사 -1: 마피아
	int numOfmap = 0, numOfciv = 0;		//마피아의 수와 시민의 수, 경찰과 의사를 시민으로 카운트한다. 
	int who_kill = 0, who_save = 0;		//마피아가 죽이려는 유저와 의사가 살리려는 유저
	while (num_user < 4) {					// 최소 유저 수는 4명이어야 됨.
		printf("how many users : ");
		scanf("%d", &num_user);
		if (num_user < 4)
			printf("유저 수가 너무 적습니다.\n");
		else if (num_user > 8) {
			printf("유저 수가 너무 많습니다.\n");
			num_user = 0;
		}
	}

	MALLOC(user_roles, num_user * sizeof(int));		//정해진 유저 수에 따라 할당해줌
	for (int i = 0; i < num_user; i++) {					//유저 배열을 초기화
		user_roles[i] = 0;
	}
	make_user_roles(user_roles, num_user);					//유저 수에 따라 역할 배분

	for (int i = 1; i <= num_user; i++) {					//유저 배분이 잘 됐나 테스트하는 코드
		switch (user_roles[i]) {
		case -1:
			printf("player %d : 마피아\n", i);
			numOfmap++;
			break;
		case 1:
			printf("player %d : 시민\n", i);
			numOfciv++;
			break;
		case 2:
			printf("player %d : 경찰\n", i);
			numOfciv++;
			break;
		case 3:
			printf("player %d : 의사\n", i);
			numOfciv++;
		}
	}													//실제로 동작할 때는 사회자만 보이게 하거나 삭제해야될 코드임

	printf("현재 마피아 수: %d\n현재 시민 수: %d\n", numOfmap, numOfciv);	//마피아 수와 시민의 수가 잘 카운트 되는지 테스트하는 코드, 실제로는 삭제되어야 함.

	//밤이 될 때 마다 실행되어야하는 경로
	choice_invest(user_roles, num_user);					//경찰이 먼저 조사하고
	who_kill = choice_kill(user_roles, num_user);			//마피아가 죽일사람 정하고
	who_save = choice_save(user_roles, num_user);			//의사가 살릴사람 정함
	if (who_kill == who_save) {							//죽일사람을 살리면 살아나게하게 한다. 이미 죽은사람은 살릴 수 없음
		printf("player가 의사에 의해 살아났습니다!");
	}
	else {
		user_roles[who_kill] = 0;
		printf("player %d가 죽었습니다.", who_kill);		//플레이어가 죽으면 죽었다고 알리고 플레이어의 role을 0(죽은상태)로 바꿈
	}
	//여기까지


	return 0;
}

void make_user_roles(int u_roles[], int users) {

	int map = 1;							//마피아와 경찰 의사는 기본 1명
	int pol = 1;
	int doc = 1;
	int civ;
	int map_count = 0;
	int pol_count = 0, doc_count = 0;
	int temp = 0;

	switch (users) {						//유저 수에 따라 마피아 수가 달라짐
	case 6: map = 2; break;
	case 7: map = 2; break;
	case 8: map = 3; break;
	}

	civ = users - (map + pol + doc);		//시민은 남은 인원

	srand((unsigned int)time(NULL));		//랜덤으로 분배

	while (map_count < map) {				//마피아 먼저 정하기
		temp = rand() % users + 1;
		if (u_roles[temp] == 0) {
			u_roles[temp] = -1;
			map_count++;
		}
		else continue;
	}

	while (!pol_count || !doc_count) {		//의사와 경찰 정하기
		
		if (u_roles[temp] == 0 && pol_count == 0) {
			temp = rand() % users + 1;
			u_roles[temp] = 2;
			pol_count = 1;
		}
		
		if (u_roles[temp] == 0 && doc_count == 0) {
			temp = rand() % users + 1;
			u_roles[temp] = 3;
			doc_count = 1;
		}
	}
	for (int i = 1; i <= users; i++) {		//남은 인원들은 모두 시민
		if (u_roles[i] == 0)
			u_roles[i] = 1;
	}
}
void vote_mapia(int u_roles[], int users) {	//마피아일거 같은 유저 투표, 변수를 전역으로 바꿔야될거같아서 수정해야될거같음
	int *vote_users;
	int abstention = 0; // 기권
	int vote;
	int i;

	MALLOC(vote_users, users * sizeof(int));

	for (i = 1; i <= users; i++) {
		if (u_roles[i] != 0)
			vote_users[i] = 0;
		else
			vote_users[i] = -1;
	}

	print_now_users(u_roles, users);

	while (1) {
		printf("마피아를 고르십시오.(-1 기권)\n");
		scanf("%d", &vote);
		if (vote == -1) {
			abstention++;
			break;
		}
		else {
			if (vote_users[vote] == -1) {
				vote = 0;
				printf("죽은 사람입니다. 다시 골라주세요\n");
			}
			else {
				vote_users[vote]++;
				break;
			}
		}
	}
}
	
int choice_kill(int u_roles[], int users) {								//마피아가 밤에 행동하는 알고리즘

	int who_kill = -1;

	print_now_users(u_roles, users);									//현재 남은 유저 프린트해주고 고르게하기

	while (1) {
		printf("죽일 사람을 고르십시오.\n");
		scanf("%d", &who_kill);
		if (who_kill > 0 && who_kill <= users) {						//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
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

	return who_kill;													//죽일 유저 리턴

}
void choice_invest(int u_roles[], int users) {							//마피아 알고리즘과 흡사합니다.

	int who_invest = -1;

	print_now_users(u_roles, users);

	
	
	while (1) {
		printf("조사할 사람을 고르십시오.\n");
		scanf("%d", &who_invest);
		if (who_invest > 0 && who_invest <= users) {					//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
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

	if (u_roles[who_invest] == -1) {									//마피아가 맞는지 아닌지만 경찰에게 알려주는 부분
		printf("player %d는 마피아가 맞습니다.\n", who_invest);
		return;
	}
	else {
		printf("player %d는 마피아가 아닙니다.\n", who_invest);
		return;
	}


}
int choice_save(int u_roles[], int users) {								//마피아 알고리즘에서 변수 이름만 바뀌고 살릴 사람 리턴하는 함수

	int who_save = -1;

	print_now_users(u_roles, users);

	while (1) {
		printf("살릴 사람을 고르십시오.\n");
		scanf("%d", &who_save);
		if (who_save > 0 && who_save <= users) {						//1~user수 사이의 수를 입력하지 않으면 다시 입력하게함.
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

	return who_save;

}
void print_now_users(int u_roles[], int users) {						//현재 생존자만 출력해주는 함수.
	int i;
	printf("현재 생존자: ");
	for (i = 1; i <= users; i++) {
		if (u_roles[i] != 0)
			printf("%d ", i);
	}
	printf("\n");
}