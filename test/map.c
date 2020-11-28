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
	int *user_roles;					//������ ����. 1: �ù� 2: ���� 3: �ǻ� -1: ���Ǿ�
	int numOfmap = 0, numOfciv = 0;
	int who_kill = 0, who_save = 0;
	while (users < 4) {
		printf("how many users : ");
		scanf("%d", &users);
		if (users < 4)
			printf("���� ���� �ʹ� �����ϴ�.\n");
		else if (users > 8) {
			printf("���� ���� �ʹ� �����ϴ�.\n");
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
			printf("player %d : ���Ǿ�\n", i);
			numOfmap++;
			break;
		case 1:
			printf("player %d : �ù�\n", i);
			numOfciv++;
			break;
		case 2:
			printf("player %d : ����\n", i);
			numOfciv++;
			break;
		case 3:
			printf("player %d : �ǻ�\n", i);
			numOfciv++;
		}
	}
	printf("���� ���Ǿ� ��: %d\n���� �ù� ��: %d\n", numOfmap, numOfciv);

	choice_invest(user_roles, users);
	who_kill = choice_kill(user_roles, users);
	who_save = choice_save(user_roles, users);
	if (who_kill == who_save) {
		printf("player�� �ǻ翡 ���� ��Ƴ����ϴ�!");
	}
	else {
		user_roles[who_kill] = 0;
		printf("player %d�� �׾����ϴ�.", who_kill);
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
	int abstention = 0; // ���
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
		printf("���ǾƸ� ���ʽÿ�.(-1 ���)\n");
		scanf("%d", &vote);
		if (vote == -1) {
			abstention++;
			break;
		}
		else {
			if (vote_users[vote] == -1) {
				vote = 0;
				printf("���� ����Դϴ�. �ٽ� ����ּ���\n");
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
		printf("���� ����� ���ʽÿ�.\n");
		scanf("%d", &who_kill);
		if (!u_roles[who_kill]) {
			printf("�̹� ���� ����Դϴ�. �ٽ� ������\n");
			who_kill = -1;
		}
		else {
			printf("%d�� �÷��̾ ���Դϴ�.\n", who_kill);
			break;
		}
	}

	return who_kill;

}
void choice_invest(int u_roles[], int users) {

	int who_invest = -1;

	print_now_users(u_roles, users);

	
	printf("������ ����� ���ʽÿ�.\n");
	while (1) {
		scanf("%d", &who_invest);
		if (who_invest > 0 && who_invest <= users) {
			if (u_roles[who_invest] == 0) {
				printf("�̹� ���� ����Դϴ�.");
				who_invest = -1;
			}
			else
				break;
		}
		else {
			printf("�߸��Է��Ͽ����ϴ�.\n");
			who_invest = -1;
		}
	}

	if (u_roles[who_invest] == -1) {
		printf("player %d�� ���Ǿư� �½��ϴ�.\n", who_invest);
		return;
	}
	else {
		printf("player %d�� ���Ǿư� �ƴմϴ�.\n", who_invest);
		return;
	}


}
int choice_save(int u_roles[], int users) {

	int who_save = -1;

	print_now_users(u_roles, users);

	while (1) {
		printf("�츱 ����� ���ʽÿ�.\n");
		scanf("%d", &who_save);
		if (who_save > 0 && who_save <= users) {
			if (!u_roles[who_save]) {
				printf("�̹� ���� ����Դϴ�. �ٽ� ������");
				who_save = -1;
			}
			else {
				printf("%d�� �÷��̾ �츳�ϴ�.\n", who_save);
				break;
			}
		}
	}

	return who_save;

}
void print_now_users(int u_roles[], int users) {
	int i;
	printf("���� ������: ");
	for (i = 1; i <= users; i++) {
		if (u_roles[i] != 0)
			printf("%d ", i);
	}
	printf("\n");
}