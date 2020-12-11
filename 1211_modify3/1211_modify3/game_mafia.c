#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void client(int ,char *);
void server(int);

int main()
{
	int user,port;
	char ip[BUFSIZ];

	while(1)
	{
		printf("사용자 정보를 입력하시오(1=서버관리자, 2=유저) : ");
		scanf("%d",&user);
		
		if(user != 1 && user != 2)
		{
			fprintf(stderr,"잘못 입력하셨습니다.\n");
			continue;
		}
		
		else
			break;
	}

	switch(user)
	{
		case 1:
			printf("서버의 포트번호를 입력하십시오. : ");
			scanf("%d",&port);
			server(port);
			break;

		case 2:
			printf("서버의 포트번호를 입력하십시오. : ");
			scanf("%d",&port);
			printf("서버의 ip를 입력하십시오. : ");
			scanf("%s",ip);
			fflush(stdin);
			client(port,ip);
			break;

		default:
			fprintf(stderr,"잘못 입력하셨습니다.\n");
	}

	return 1;
}
