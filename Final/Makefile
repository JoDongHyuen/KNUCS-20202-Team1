.PHONY: clean

game_mafia : game_mafia.o server.o client.o socklib.o
	gcc -o game_mafia game_mafia.o server.o client.o socklib.o -lpthread

game_mafia.o : game_mafia.c server.o client.o
	gcc -Wall -g -c game_mafia.c

server.o: server.c socklib.h
	gcc -Wall -g -c server.c -lpthread

client.o: client.c socklib.h
	gcc -Wall -g -c client.c -lpthread

socklib.o: socklib.c socklib.h
	gcc -Wall -g -c socklib.c 

clean:
	rm -f *.o
