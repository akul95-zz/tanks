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


#define PORT "4003"
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

#define TANK_COLOUR "\x1b[31m"
#define BULLET_COLOUR "\x1b[32m"
#define RESET_COLOUR "\x1b[0m"

#define BULLET_DAMAGE 50

typedef struct clientData
{
	int fd, id;
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
	int num_bullets;
	bullet **bullets;
	int arena[ARENA_HEIGHT][ARENA_WIDTH];
}gameState;

#endif