#ifndef GAMEDEF_H
#define GAMEDEF_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>


#define PORT "1025"
#define MAXCONN 6

#define ARENA_HEIGHT 35
#define ARENA_WIDTH 120

#define HOST_ROOM '1'
#define JOIN_ROOM '2'
#define EXIT_GAME '3'

#define START_GAME '1'
#define CLOSE_ROOM '2'

#define ESCAPE (char)(27)

#define BUFF_SIZE 50

#define REFRESH_TIME 0.2

#define BLANK_SPACE 0
#define NONBREAKABLE_WALL 1

#define FIRE_BULLET ' '

#define COLOUR_0 "\x1b[31m"
#define COLOUR_1 "\x1B[32m"
#define COLOUR_2 "\x1B[33m"
#define COLOUR_3 "\x1B[34m"
#define COLOUR_4 "\x1B[35m"
#define COLOUR_5 "\x1B[36m"
#define BULLET_COLOUR "\x1b[37m"
#define RESET_COLOUR "\x1b[0m"

#define BULLET_DAMAGE 20
#define HIT_BONUS 50

#define PUBLIC_KEY 1000000801

typedef struct clientData
{
	int fd, id;
	char nick[BUFF_SIZE];
}clientData;


typedef struct tank
{
	int id, row, col, dir, hp;
}tank;

typedef struct bullet
{
	int id, row, col, dir;
}bullet;

typedef struct gameState
{
	int num_tanks;
	tank **tanks;
	char **nicks;
	int *scores;
	int num_bullets;
	bullet **bullets;
	int arena[ARENA_HEIGHT][ARENA_WIDTH];
}gameState;

#endif