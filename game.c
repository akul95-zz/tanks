#include "game_io.h"
#include "gameDef.h"
#include "game_network.h"

int MIN(int a, int b)
{
	return ((a < b)?a:b);
}

int MAX(int a, int b)
{
	return ((a > b)?a:b);
}

void add_horizontal_wall(int arena[][ARENA_WIDTH], int x1, int x2, int y)
{
	int i;
	for(i = x1; i <= x2; i++)
	{
		arena[y][i] = NONBREAKABLE_WALL;
	}
}

void add_vertical_wall(int arena[][ARENA_WIDTH], int y1, int y2, int x)
{
	int i;
	for(i = y1; i <= y2; i++)
	{
		arena[i][x] = NONBREAKABLE_WALL;
	}
}

void make_arena_1(int arena[][ARENA_WIDTH])
{
	int i, j;
	for(i = 0; i < ARENA_HEIGHT; i++)
	{
		for(j = 0; j < ARENA_WIDTH; j++)
		{
			arena[i][j] = BLANK_SPACE;
			if(i == 0 || i == ARENA_HEIGHT - 1 || j == 0 || j == ARENA_WIDTH - 1)
			{
				arena[i][j] = NONBREAKABLE_WALL;
			}
		}
	}
}

void make_arena_2(int arena[][ARENA_WIDTH])
{
	int i, j;
	for(i = 0; i < ARENA_HEIGHT; i++)
	{
		for(j = 0; j < ARENA_WIDTH; j++)
		{
			arena[i][j] = BLANK_SPACE;
			if(i == 0 || i == ARENA_HEIGHT - 1 || j == 0 || j == ARENA_WIDTH - 1)
			{
				arena[i][j] = NONBREAKABLE_WALL;
			}
		}
	}
	add_horizontal_wall(arena, 10, 110, 16);
	add_vertical_wall(arena, 7, 28, 60);
	add_horizontal_wall(arena, 1, 12, 7 );
	add_horizontal_wall(arena, 108, 119, 7 );
	add_horizontal_wall(arena, 1, 12, 26 );
	add_horizontal_wall(arena, 108, 119, 26 );
	add_vertical_wall(arena, 1, 7, 35 );
	add_vertical_wall(arena, 28, 34, 35 );
	add_vertical_wall(arena, 1, 7, 85 );
	add_vertical_wall(arena, 28, 34, 85 );
}

void make_arena_3(int arena[][ARENA_WIDTH])
{
	int i, j;
	for(i = 0; i < ARENA_HEIGHT; i++)
	{
		for(j = 0; j < ARENA_WIDTH; j++)
		{
			arena[i][j] = BLANK_SPACE;
			if(i == 0 || i == ARENA_HEIGHT - 1 || j == 0 || j == ARENA_WIDTH - 1)
			{
				arena[i][j] = NONBREAKABLE_WALL;
			}
		}
	}
	add_horizontal_wall(arena, 1, 8, 17);
	add_horizontal_wall(arena, 112, 119, 17);
	add_vertical_wall(arena, 1, 7, 45);
	add_vertical_wall(arena, 1, 7, 75);
	add_vertical_wall(arena, 28, 34, 45);
	add_vertical_wall(arena, 28, 34, 75);
	add_vertical_wall(arena, 13, 21, 55);
	add_vertical_wall(arena, 13, 21, 65);
	add_horizontal_wall(arena, 55, 65, 13);
	add_horizontal_wall(arena, 55, 65, 21);

	add_vertical_wall(arena, 8, 12, 20);
	add_vertical_wall(arena, 8, 12, 30);
	add_vertical_wall(arena, 8, 12, 100);
	add_vertical_wall(arena, 8, 12, 90);
	add_horizontal_wall(arena, 20, 30, 12);
	add_horizontal_wall(arena, 90, 100, 12);

	add_vertical_wall(arena, 24, 28, 20);
	add_vertical_wall(arena, 24, 28, 30);
	add_vertical_wall(arena, 24, 28, 100);
	add_vertical_wall(arena, 24, 28, 90);
	add_horizontal_wall(arena, 20, 30, 24);
	add_horizontal_wall(arena, 90, 100, 24);
}

void pack_tank(tank *T, char buffer[])
{
	sprintf(buffer, "%d %d %d %d %d", T->id, T->row, T->col, T->dir, T->hp);
}

void unpack_tank(tank *T, char buffer[])
{
	sscanf(buffer, "%d %d %d %d %d", &T->id, &T->row, &T->col, &T->dir, &T->hp);
}

bullet *get_new_bullet(tank *T)
{
	bullet *ret = (bullet *)malloc(sizeof(bullet));
	ret->id = T->id;
	ret->row = T->row;
	ret->col = T->col;
	ret->dir = T->dir;
	if(ret->dir == 0)
		ret->row = ret->row - 1;
	if(ret->dir == 1)
		ret->col = ret->col + 1;
	if(ret->dir == 2)
		ret->row = ret->row + 1;
	if(ret->dir == 3)
		ret->col = ret->col - 1;
	return ret;
}

tank *get_new_tank(int id)
{
	tank *ret = (tank *)malloc(sizeof(tank));
	ret->id = id;
	if(id == 0)
	{
		ret->row = 2;
		ret->col = 2;
		ret->dir = 1;
		ret->hp = 100;
	}
	else if(id == 1)
	{
		ret->row = ARENA_HEIGHT - 3;
		ret->col = ARENA_WIDTH - 3;
		ret->dir = 3;
		ret->hp = 100;
	}
	else if(id == 2)
	{
		ret->row = ARENA_HEIGHT - 3;
		ret->col = 2;
		ret->dir = 1;
		ret->hp = 100;
	}
	else if(id == 3)
	{
		ret->row = 2;
		ret->col = ARENA_WIDTH - 3;
		ret->dir = 3;
		ret->hp = 100;
	}
	else if(id == 4)
	{
		ret->row = 2;
		ret->col = ARENA_WIDTH/2;
		ret->dir = 2;
		ret->hp = 100;
	}
	else if(id == 5)
	{
		ret->row = ARENA_HEIGHT - 3;
		ret->col = ARENA_WIDTH/2;
		ret->dir = 0;
		ret->hp = 100;
	}
	return ret;
}

int apply_key_press_to_tank(tank *T, char k)
{
	int ret = 0;
	if(k == 'w')
	{
		if(T->dir == 0)
		{
			T->row--;
		}
		else
		{
			T->dir = 0;
		}
		ret = 1;
	}
	else if(k == 'a')
	{
		if(T->dir == 3)
		{
			T->col--;
		}
		else
		{
			T->dir = 3;
		}
		ret = 1;
	}
	else if(k == 's')
	{
		if(T->dir == 2)
		{
			T->row++;
		}
		else
		{
			T->dir = 2;
		}
		ret = 1;
	}
	else if(k == 'd')
	{
		if(T->dir == 1)
		{
			T->col++;
		}
		else
		{
			T->dir = 1;
		}
		ret = 1;
	}
	// printf("trying %d %c with result %d\n", T->id, k, ret);
	return ret;
}

int move_tank(gameState *curr_state, int id, char key_press)
{
	int i, j;
	// no point moving em if they are dead
	if(curr_state->tanks[id] == NULL)
		return 0;
	tank *new_tank = (tank *)malloc(sizeof(tank));
	new_tank->id = curr_state->tanks[id]->id;
	new_tank->row = curr_state->tanks[id]->row;
	new_tank->col = curr_state->tanks[id]->col;
	new_tank->dir = curr_state->tanks[id]->dir;
	new_tank->hp = curr_state->tanks[id]->hp;
	int ret = apply_key_press_to_tank(new_tank, key_press);
	// if(!ret)
	// {
	// 	printf("failed %d %c because dunno\n", id, key_press);
	// }
	// else
	// {
	// 	printf("not failed %d %c because dunno\n", id, key_press);
	// }
	for (i = 0; i < curr_state->num_tanks; ++i)
	{
		if(i == id)
			continue;
		if(curr_state->tanks[i] == NULL)
			continue;
		int rdiff = abs(new_tank->row - curr_state->tanks[i]->row);
		int cdiff = abs(new_tank->col - curr_state->tanks[i]->col);
		if(rdiff < 3 && cdiff < 3)
		{
			ret = 0;
			break;
		}
	}
	// if(!ret)
	// {
	// 	printf("failed %d %c because of %d\n", id, key_press, i);
	// 	if(curr_state->tanks[i] != NULL)
	// 		printf("%d\n", curr_state->tanks[i]->hp);
	// }
	// else
	// {
	// 	printf("not failed %d %c because of %d\n", id, key_press, i);
	// 	// if(curr_state->tanks[i] == NULL)
	// 	// 	printf("bakchodi hain\n");
	// }
	for (i = 0; i < ARENA_HEIGHT; ++i)
	{
		for (j = 0; j < ARENA_WIDTH; ++j)
		{
			if(curr_state->arena[i][j] != BLANK_SPACE)
			{
				int rdiff = abs(new_tank->row - i);
				int cdiff = abs(new_tank->col - j);
				if(rdiff < 2 && cdiff < 2)
					ret = 0;
			}
		}
	}
	if(ret)
		apply_key_press_to_tank(curr_state->tanks[id], key_press);
	// else
	// {
	// 	printf("hit wall\n");
	// }
	free(new_tank);
	return ret;
}

void load_arena(gameState *curr_state, int level, int num_tanks, tank **tank_info)
{
	curr_state->num_tanks = num_tanks;
	curr_state->tanks = tank_info;
	curr_state->num_bullets = 0;
	curr_state->bullets = (bullet **)calloc(0, sizeof(bullet*));
	if(level == 1)
	{
		make_arena_1(curr_state->arena);
	}
	else if(level == 2)
	{
		make_arena_2(curr_state->arena);
	}
	else if(level == 3)
	{
		make_arena_3(curr_state->arena);
	}
}

void fire_bullet(gameState *curr_state, int id)
{
	if(curr_state->tanks[id] == NULL)
		return;
	bullet *new_bullet = get_new_bullet(curr_state->tanks[id]);
	curr_state->num_bullets++;
	curr_state->bullets = (bullet **)realloc(curr_state->bullets, curr_state->num_bullets*sizeof(bullet*));
	curr_state->bullets[curr_state->num_bullets - 1] = new_bullet;
}

void upd_state(gameState *curr_state)
{
	int i,j;
	for (i = 0; i < curr_state->num_bullets; ++i)
	{
		int prev_r = curr_state->bullets[i]->row;
		int prev_c = curr_state->bullets[i]->col;
		switch(curr_state->bullets[i]->dir)
		{
			case 0: curr_state->bullets[i]->row = curr_state->bullets[i]->row - 2;
				break;
			case 1: curr_state->bullets[i]->col = curr_state->bullets[i]->col + 2;
				break;
			case 2: curr_state->bullets[i]->row = curr_state->bullets[i]->row + 2;
				break;
			case 3: curr_state->bullets[i]->col = curr_state->bullets[i]->col - 2;
				break;	
		}
		int r = curr_state->bullets[i]->row;
		int c = curr_state->bullets[i]->col;
		int bullet_exists = 1;
		if(r < 0 || c < 0 || r >= ARENA_HEIGHT || c >= ARENA_WIDTH)
		{
			free(curr_state->bullets[i]);
			curr_state->bullets[i] = NULL;
			bullet_exists = 0;
		}
		int min_r = MIN(prev_r, r), max_r = MAX(prev_r, r);
		int min_c = MIN(prev_c, c), max_c = MAX(prev_c, c);
		for (j = min_r; j <= max_r && bullet_exists; ++j)
		{
			if(curr_state->arena[j][prev_c] == NONBREAKABLE_WALL)
			{
				free(curr_state->bullets[i]);
				curr_state->bullets[i] = NULL;
				bullet_exists = 0;
			}
		}
		if(!bullet_exists)
			continue;
		for (j = min_c; j <= max_c && bullet_exists; ++j)
		{
			if(curr_state->arena[prev_r][j] == NONBREAKABLE_WALL)
			{
				free(curr_state->bullets[i]);
				curr_state->bullets[i] = NULL;
				bullet_exists = 0;
			}
		}
		if(!bullet_exists)
			continue;
		for (j = 0; j < curr_state->num_tanks && bullet_exists; ++j)
		{
			if(curr_state->tanks[j] == NULL)
				continue;
			int rdiff = abs(curr_state->tanks[j]->row - r);
			int cdiff = abs(curr_state->tanks[j]->col - c);
			if(rdiff < 2 && cdiff < 2)
			{
				free(curr_state->bullets[i]);
				curr_state->bullets[i] = NULL;
				curr_state->tanks[j]->hp = curr_state->tanks[j]->hp - BULLET_DAMAGE;
				if(curr_state->tanks[j]->hp <= 0)
				{
					free(curr_state->tanks[j]);
					curr_state->tanks[j] = NULL;
				}
				bullet_exists = 0;
			}
		}
	}
	int ctr = 0;
	for (i = 0; i < curr_state->num_bullets; ++i)
	{
		if(curr_state->bullets[i] != NULL)
			ctr++;
	}
	bullet **new_bullets = (bullet **)calloc(ctr, sizeof(bullet*));
	int cpos = 0;
	for (i = 0; i < curr_state->num_bullets; ++i)
	{
		if(curr_state->bullets[i] != NULL)
		{
			new_bullets[cpos] = curr_state->bullets[i];
			cpos++;
		}
	}
	free(curr_state->bullets);
	curr_state->bullets = new_bullets;
	curr_state->num_bullets = ctr;
}

void play_game(int fd)
{
	fd_set serverFD;
	FD_ZERO(&serverFD);
	FD_SET(fd, &serverFD);
	char buffer[BUFF_SIZE];
	gameState curr_state;
	int my_id;
	// game arena etc.
	while(1)
	{
		int ret_val;
		while((ret_val = listen_on_fdset(serverFD, fd, buffer)) == 0);
		if(ret_val < 0)
		{
			// error. act accordingly
		}
		else if(!strcmp(buffer, "ARENA INFO"))
		{
			// level num_players
			// tank_info 1
			// ...
			// tank_info n
			while((ret_val = listen_on_fdset(serverFD, fd, buffer)) == 0);
			if(ret_val < 0)
			{
				// error
			}
			int level, num_tanks;
			tank **tank_info;
			sscanf(buffer, "%d %d %d", &my_id, &level, &num_tanks);
			tank_info = (tank **)calloc(num_tanks, sizeof(tank*));
			int i;
			for (i = 0; i < num_tanks; ++i)
			{
				while((ret_val = listen_on_fdset(serverFD, fd, buffer)) == 0);
				if(ret_val < 0)
				{
					// error
				}
				tank *temp_tank = (tank *)malloc(sizeof(tank));
				unpack_tank(temp_tank, buffer);
				tank_info[i] = temp_tank;
			}
			load_arena(&curr_state, level, num_tanks, tank_info);
			break;
		}
	}
	while(1)
	{
		while(1)
		{
			char *inp_ptr = get_input();
			if(inp_ptr != NULL)
			{
				char inp = *inp_ptr;
				inp = tolower(inp);
				sprintf(buffer, "%d%c", my_id, inp);
				send(fd, buffer, BUFF_SIZE, 0);
				if(inp == ESCAPE)
					return;
			}
			int ret_val = listen_on_fdset(serverFD, fd, buffer);
			if(ret_val < 0)
			{
				// error
			}
			else if(ret_val == 0)
			{
				// no info
			}
			else if(!strcmp(buffer, "UPD_PLISS"))
			{
				upd_state(&curr_state);
				print_arena(curr_state);
				// printf("%d\n", my_id);
			}
			else
			{
				int tank_id;
				char key_press;
				sscanf(buffer, "%d%c", &tank_id, &key_press);
				if(key_press == FIRE_BULLET)
				{
					fire_bullet(&curr_state, tank_id);
				}
				else
				{
					move_tank(&curr_state, tank_id, key_press);
				}
			}
		}
	}
}

void print_opening_screen()
{
}

void join_room(int self)
{
	char IP[50];
	char nick[BUFF_SIZE];
	if(self)
	{
		system("stty echo");
		strcpy(IP, "127.0.0.1");
		printf("Enter your nick (at most 10 characters long)\n");
		scanf("%s", nick);
		while(strlen(nick) > 10)
		{
			printf("Invalid nick\n");
			printf("Enter your nick (at most 10 characters long)\n");
			scanf("%s", nick);
		}
		system("stty -echo");
	}
	else
	{
		system("stty echo");
		printf("Enter host IP: ");
		scanf("%s", IP);
		printf("Enter your nick (at most 10 characters long)\n");
		scanf("%s", nick);
		while(strlen(nick) > 10)
		{
			printf("Invalid nick\n");
			printf("Enter your nick (at most 10 characters long)\n");
			scanf("%s", nick);
		}
		system("stty -echo");
	}
	int fd = setup_client_socket(IP, nick);
	if(fd < 0)
		return;
	play_game(fd);
}

void* join_self_room(void *args)
{
	join_room(1);
}

void host_room()
{
	// start room
	int level;
	system("stty echo");
	printf("Enter level (1-3)\n");
	scanf("%d", &level);
	while(level < 1 || level > 3)
	{
		printf("Invalid level\n");
		printf("Enter level (1-3)\n");
		scanf("%d", &level);
	}
	system("stty -echo");

	char start_room_symbol = START_GAME;
	char close_room_symbol = CLOSE_ROOM;
	printf("Press %c to start the game\n", start_room_symbol);
	printf("Press %c to close the game\n", close_room_symbol);
	clientData client_list[MAXCONN];
	int num_clients = 0;
	
	int socket_listen_desc = setup_listen_socket();
	if(socket_listen_desc < 0)
		return;

	while(1)
	{
		listen_on_socket(socket_listen_desc, client_list, &num_clients);
		char *inp_ptr = get_input();
		if(inp_ptr != NULL)
		{
			char inp = *inp_ptr;
			if(inp == START_GAME)
			{
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_t thread;
				pthread_create(&thread, &attr, join_self_room, NULL);
				break;
			}
			if(inp == CLOSE_ROOM)
				return;
		}
	}
	int old_client_num = num_clients;
	while(num_clients == old_client_num)
	{
		listen_on_socket(socket_listen_desc, client_list, &num_clients);
	}
	fd_set client_fds;
	FD_ZERO(&client_fds);
	int i, max_fd = -1;
	for (i = 0; i < num_clients; ++i)
	{
		FD_SET(client_list[i].fd, &client_fds);
		if(client_list[i].fd > max_fd)
			max_fd = client_list[i].fd;
	}
	clock_t start_time = clock(), curr_time = clock();
	char buffer[BUFF_SIZE];
	// TODO: take arena number input
	// level num_players
	// tank_info 1
	// ...
	// tank_info n
	gameState curr_state;
	sprintf(buffer, "ARENA INFO");
	send_to_fdset(client_fds, max_fd, buffer);
	load_arena(&curr_state, level, 0, NULL);
	for (i = 0; i < num_clients; ++i)
	{
		sprintf(buffer, "%d %d %d", client_list[i].id, level, num_clients);
		send(client_list[i].fd, buffer, BUFF_SIZE, 0);
	}
	curr_state.num_tanks = num_clients;
	curr_state.tanks = (tank **)calloc(num_clients, sizeof(tank*));
	for (i = 0; i < num_clients; ++i)
	{
		tank *temp_tank = get_new_tank(i);
		curr_state.tanks[i] = temp_tank;
		pack_tank(temp_tank, buffer);
		send_to_fdset(client_fds, max_fd, buffer);
	}
	int received_movement[num_clients];
	int received_fire[num_clients];
	memset(received_movement, 0, sizeof(received_movement));
	memset(received_fire, 0, sizeof(received_fire));
	while(1)
	{
		clock_t curr_time = clock();
		double time_elapsed = (double)(curr_time - start_time)/CLOCKS_PER_SEC;
		if(time_elapsed > REFRESH_TIME)
		{
			strcpy(buffer, "UPD_PLISS");
			upd_state(&curr_state);
			send_to_fdset(client_fds, max_fd, buffer);
			start_time = clock();
			memset(received_movement, 0, sizeof(received_movement));
			memset(received_fire, 0, sizeof(received_fire));
		}
		int ret_val = listen_on_fdset(client_fds, max_fd, buffer);
		if(ret_val < 0)
		{
			// error
		}
		else if(ret_val == 0)
		{
			// do nothing
		}
		else
		{
			int from_id;
			char key_press;
			sscanf(buffer, "%d%c", &from_id, &key_press);
			if(!received_fire[from_id] && key_press == FIRE_BULLET)
			{
				fire_bullet(&curr_state, from_id);
				received_fire[from_id] = 1;
				send_to_fdset(client_fds, max_fd, buffer);
			}
			if(!received_movement[from_id] && move_tank(&curr_state, from_id, key_press))
			{
				received_movement[from_id] = 1;
				send_to_fdset(client_fds, max_fd, buffer);
			}
		}
	}
}

void run_game()
{
	system("stty -echo");
	print_opening_screen();
	while(1)
	{
		char opt1 = HOST_ROOM;
		char opt2 = JOIN_ROOM;
		char opt3 = EXIT_GAME;
		printf("Press %c to host room\n", opt1);
		printf("Press %c to join room\n", opt2);
		printf("Press %c to exit game\n", opt3);
		char inp;
		system("stty echo");
		scanf(" %c", &inp);
		system("stty -echo");
		if(inp == HOST_ROOM)
			host_room();
		if(inp == JOIN_ROOM)
			join_room(0);
		if(inp == EXIT_GAME)
			break;
		if(inp == ESCAPE)
			break;
		system("stty echo");
	}
	system("stty echo");
}