#include "gameDef.h"

void changemode(int dir)
{
	static struct termios oldt, newt;
	if(dir == 1)
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}
	else
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit(void)
{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}

void put_tank_arena(int id, int row, int col, int dir, int arena[][ARENA_WIDTH])
{
	if(dir == 0) // up
	{
		arena[row - 1][col] = 100 + id;
		arena[row][col - 1] = 100 + id;
		arena[row][col] = 100 + id;
		arena[row][col + 1] = 100 + id;
		arena[row + 1][col - 1] = 100 + id;
		arena[row + 1][col + 1] = 100 + id;
	}
	else if(dir == 1) //rt
	{
		arena[row - 1][col - 1] = 100 + id;
		arena[row - 1][col] = 100 + id;
		arena[row][col] = 100 + id;
		arena[row][col + 1] = 100 + id;
		arena[row + 1][col - 1] = 100 + id;
		arena[row + 1][col] = 100 + id;
	}
	else if(dir == 2) //dn
	{
		arena[row - 1][col - 1] = 100 + id;
		arena[row - 1][col + 1] = 100 + id;
		arena[row][col - 1] = 100 + id;
		arena[row][col] = 100 + id;
		arena[row][col + 1] = 100 + id;
		arena[row + 1][col] = 100 + id;
	}
	else if(dir == 3) //lt
	{
		arena[row - 1][col] = 100 + id;
		arena[row - 1][col + 1] = 100 + id;
		arena[row][col - 1] = 100 + id;
		arena[row][col] = 100 + id;
		arena[row + 1][col] = 100 + id;
		arena[row + 1][col + 1] = 100 + id;
	}

	return;
}

void put_bullet_arena(int id, int row, int col, int arena[][ARENA_WIDTH])
{
	arena[row][col] = 200 + id;
}

void print_arena(gameState curr_state)
{
	int i, j;
	system("clear");
	for (i = 0; i < curr_state.num_tanks; ++i)
	{
		if(curr_state.tanks[i] == NULL)
			continue;
		int id = curr_state.tanks[i]->id;
		int r = curr_state.tanks[i]->row;
		int c = curr_state.tanks[i]->col;
		int dir = curr_state.tanks[i]->dir;
		put_tank_arena(id, r, c, dir, curr_state.arena);
	}
	for (i = 0; i < curr_state.num_bullets; ++i)
	{
		int id = curr_state.bullets[i]->id;
		int r = curr_state.bullets[i]->row;
		int c = curr_state.bullets[i]->col;
		put_bullet_arena(id, r, c, curr_state.arena);
	}
	for (i = 0; i < ARENA_HEIGHT; ++i)
	{
		for (j = 0; j < ARENA_WIDTH; ++j)
		{
			if(curr_state.arena[i][j] == NONBREAKABLE_WALL)
			{
				char solid_block[4];
				solid_block[0] = (char)-30;
				solid_block[1] = (char)-106;
				solid_block[2] = (char)-120;
				solid_block[3] = (char)0;
				printf("%s", solid_block);
			}
			else if(curr_state.arena[i][j] == BLANK_SPACE)
			{
				printf(" ");
			}
			else if(curr_state.arena[i][j] >= 100 && curr_state.arena[i][j] < 200)
			{
				char solid_block[4];
				solid_block[0] = (char)-30;
				solid_block[1] = (char)-106;
				solid_block[2] = (char)-120;
				solid_block[3] = (char)0;
				int id = curr_state.arena[i][j] - 100;
				printf(TANK_COLOUR"%s"RESET_COLOUR, solid_block);
			}
			else if(curr_state.arena[i][j] >= 200 && curr_state.arena[i][j] < 300)
			{
				char bullet_block = '*';
				printf(BULLET_COLOUR"%c"RESET_COLOUR, bullet_block);
			}
		}
		printf("\n");
	}
	printf("Controls:\n");
	printf("W: up\n");
	printf("A: left\n");
	printf("S: down\n");
	printf("D: right\n");
	printf("space: fire\n");
	// for (i = 0; i < ARENA_HEIGHT; ++i)
	// {
	// 	for (j = 0; j < ARENA_WIDTH; ++j)
	// 	{
	// 		printf("%d ", curr_state.arena[i][j]);
	// 	}
	// 	printf("\n");
	// }
}

char* get_input()
{
	char *ret = NULL;
	changemode(1);
	if(kbhit())
	{
		ret = (char *)malloc(sizeof(char));
		*ret = getchar();
	}
	changemode(0);
	return ret;
}