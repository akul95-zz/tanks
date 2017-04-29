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

void print_indented_coloured(gameState curr_state, int k)
{
	int i;
	switch(k)
	{
		case 0: printf(COLOUR_0"%s"RESET_COLOUR, curr_state.nicks[0]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[0]); i++)
					printf(" ");
			break;
		case 1: printf(COLOUR_1"%s"RESET_COLOUR, curr_state.nicks[1]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[1]); i++)
					printf(" ");
			break;
		case 2: printf(COLOUR_2"%s"RESET_COLOUR, curr_state.nicks[2]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[2]); i++)
					printf(" ");
			break;
		case 3: printf(COLOUR_3"%s"RESET_COLOUR, curr_state.nicks[3]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[3]); i++)
					printf(" ");
			break;
		case 4: printf(COLOUR_4"%s"RESET_COLOUR, curr_state.nicks[4]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[4]); i++)
					printf(" ");		
			break;
		case 5: printf(COLOUR_5"%s"RESET_COLOUR, curr_state.nicks[5]);
				for(i = 0; i < 10 - strlen(curr_state.nicks[5]); i++)
					printf(" ");
			break;
	}
	printf(" ");
	
}

void print_arena(gameState curr_state)
{
	int i, j, k = 0, l = 0;
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
				switch(id)
				{
					case 0: printf(COLOUR_0"%s"RESET_COLOUR, solid_block);
						break;
					case 1: printf(COLOUR_1"%s"RESET_COLOUR, solid_block);
						break;
					case 2: printf(COLOUR_2"%s"RESET_COLOUR, solid_block);
						break;
					case 3: printf(COLOUR_3"%s"RESET_COLOUR, solid_block);
						break;
					case 4: printf(COLOUR_4"%s"RESET_COLOUR, solid_block);
						break;
					case 5: printf(COLOUR_5"%s"RESET_COLOUR, solid_block);
						break;
				}
			}
			else if(curr_state.arena[i][j] >= 200 && curr_state.arena[i][j] < 300)
			{
				char bullet_block = '*';
				printf(BULLET_COLOUR"%c"RESET_COLOUR, bullet_block);
			}
			else
			{
				printf(".");
			}
		}
		if(i == 0)
		{
			printf("Player     Health Score");
		}
		else if(k < curr_state.num_tanks && (i%2 == 0))
		{
			switch(k)
			{
				case 0: print_indented_coloured(curr_state, 0);
					break;
				case 1: print_indented_coloured(curr_state, 1);
					break;
				case 2: print_indented_coloured(curr_state, 2);
					break;
				case 3: print_indented_coloured(curr_state, 3);
					break;
				case 4: print_indented_coloured(curr_state, 4);		
					break;
				case 5: print_indented_coloured(curr_state, 5);
					break;
			}
			if(curr_state.tanks[k] != NULL)
				printf("%d\t  ", curr_state.tanks[k]->hp);
			else
				printf("0\t  ");
			printf("%d", curr_state.scores[k]);
			k++;
		}
		else if(k == curr_state.num_tanks && i >= 20 && (i%2 == 0))
		{
			switch(l)
			{
				case 0: printf("\tControls:");
					break;
				case 1: printf("\tW: Up");
					break;
				case 2: printf("\tA: Left");
					break;
				case 3: printf("\tS: Down");
					break;
				case 4: printf("\tD: Right");
					break;
				case 5: printf("\tSpace: Fire");
					break;
			}
			l++;
		}
		printf("\n");
	}
	// for (i = 0; i < ARENA_HEIGHT; ++i)
	// {
	// 	for (j = 0; j < ARENA_WIDTH; ++j)
	// 	{
	// 		printf("%d ", curr_state.arena[i][j]);
	// 	}
	// 	printf("\n");
	// }
}

int is_ok(char x)
{
	if('a' <= x && x <= 'z')
		return 1;
	if('A' <= x && x <= 'Z')
		return 1;
	if('0' <= x && x <= '9')
		return 1;
	if(x == ' ')
		return 1;
	return 0;
}
char* get_input()
{
	char *ret = NULL;
	changemode(1);
	if(kbhit())
	{
		ret = (char *)malloc(sizeof(char));
		*ret = getchar();
		if(!is_ok(*ret))
		{
			free(ret);
			ret = NULL;
		}
	}
	changemode(0);
	return ret;
}