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
	int users = 0;
	int *user_roles;					//유저의 역할. 1: 시민 2: 경찰 3: 의사 -1: 마피아
	int numOfmap = 0, numOfciv = 0;
	int who_kill = 0, who_save = 0;
	while (users < 4) {
		printf("how many users : ");
		scanf("%d", &users);
		if (users < 4)
			printf("유저 수가 너무 적습니다.\n");
		else if (users > 8) {
			printf("유저 수가 너무 많습니다.\n");
			users = 0;
		}
	}

	MALLOC(user_roles, (users+1) * sizeof(int));
	for (int i = 0; i <= users; i++) {
		user_roles[i] = 0;
	}
	make_user_roles(user_roles, users);

	for (int i = 1; i <= users; i++) {
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
	}
	printf("현재 마피아 수: %d\n현재 시민 수: %d\n", numOfmap, numOfciv);

	choice_invest(user_roles, users);
	who_kill = choice_kill(user_roles, users);
	who_save = choice_save(user_roles, users);
	if (who_kill == who_save) {
		printf("player가 의사에 의해 살아났습니다!");
	}
	else {
		user_roles[who_kill] = 0;
		printf("player %d가 죽었습니다.", who_kill);
	}
	return 0;
}

void make_user_roles(int u_roles[], int users) {

	int map = 1;
	int pol = 1;
	int doc = 1;
	int civ;
	int map_count = 0;
	int pol_count = 0, doc_count = 0;
	int temp = 0;

	switch (users) {
	case 6: map = 2; break;
	case 7: map = 2; break;
	case 8: map = 3; break;
	}

	civ = users - (map + pol + doc);

	srand((unsigned int)time(NULL));

	while (map_count < map) {
		temp = rand() % users + 1;
		if (u_roles[temp] == 0) {
			u_roles[temp] = -1;
			map_count++;
		}
		else continue;
	}

	while (!pol_count || !doc_count) {
		temp = rand() % users + 1;
		if (u_roles[temp] == 0 && pol_count == 0) {
			u_roles[temp] = 2;
			pol_count = 1;
		}
		temp = rand() % users;
		if (u_roles[temp] == 0 && doc_count == 0) {
			u_roles[temp] = 3;
			doc_count++;
		}
	}
	for (int i = 1; i <= users; i++) {
		if (u_roles[i] == 0)
			u_roles[i] = 1;
	}
}
void vote_mapia(int u_roles[], int users) {
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

int choice_kill(int u_roles[], int users) {

	int who_kill = -1;

	print_now_users(u_roles, users);

	while (1) {
		printf("죽일 사람을 고르십시오.\n");
		scanf("%d", &who_kill);
		if (!u_roles[who_kill]) {
			printf("이미 죽은 사람입니다. 다시 고르세요\n");
			who_kill = -1;
		}
		else {
			printf("%d번 플레이어를 죽입니다.\n", who_kill);
			break;
		}
	}

	return who_kill;

}
void choice_invest(int u_roles[], int users) {

	int who_invest = -1;

	print_now_users(u_roles, users);

	
	printf("조사할 사람을 고르십시오.\n");
	while (1) {
		scanf("%d", &who_invest);
		if (who_invest > 0 && who_invest <= users) {
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

	if (u_roles[who_invest] == -1) {
		printf("player %d는 마피아가 맞습니다.\n", who_invest);
		return;
	}
	else {
		printf("player %d는 마피아가 아닙니다.\n", who_invest);
		return;
	}


}
int choice_save(int u_roles[], int users) {

	int who_save = -1;

	print_now_users(u_roles, users);

	while (1) {
		printf("살릴 사람을 고르십시오.\n");
		scanf("%d", &who_save);
		if (who_save > 0 && who_save <= users) {
			if (!u_roles[who_save]) {
				printf("이미 죽은 사람입니다. 다시 고르세요");
				who_save = -1;
			}
			else {
				printf("%d번 플레이어를 살립니다.\n", who_save);
				break;
			}
		}
	}

	return who_save;

}
void print_now_users(int u_roles[], int users) {
	int i;
	printf("현재 생존자: ");
	for (i = 1; i <= users; i++) {
		if (u_roles[i] != 0)
			printf("%d ", i);
	}
	printf("\n");
}