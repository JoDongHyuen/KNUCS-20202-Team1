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

int make_server_socket(int);
int make_server_socket_q(int,int);
int connect_to_server(char*,int);
