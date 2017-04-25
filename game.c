#include "game_io.h"
#include "gameDef.h"
#include "game_network.h"

void pack_tank(tank *T, char buffer[])
{
	sprintf(buffer, "%d %d %d %d %d", T->id, T->row, T->col, T->dir, T->hp);
}

void unpack_tank(tank *T, char buffer[])
{
	sscanf(buffer, "%d %d %d %d %d", &T->id, &T->row, &T->col, &T->dir, &T->hp);
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
	if('A' <= k && k <= 'Z')
	{
		k+=('a' - 'A');
	}
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
	if(curr_state->tanks[id]->hp <= 0)
		return 0;
	tank *new_tank = (tank *)malloc(sizeof(tank));
	new_tank->id = curr_state->tanks[id]->id;
	new_tank->row = curr_state->tanks[id]->row;
	new_tank->col = curr_state->tanks[id]->col;
	new_tank->dir = curr_state->tanks[id]->dir;
	new_tank->hp = curr_state->tanks[id]->hp;
	int ret = apply_key_press_to_tank(new_tank, key_press);
	for (i = 0; i < curr_state->num_tanks; ++i)
	{
		if(i == id)
			continue;
		int rdiff = abs(new_tank->row - curr_state->tanks[i]->row);
		int cdiff = abs(new_tank->col - curr_state->tanks[i]->col);
		if(rdiff < 3 && cdiff < 3)
			ret = 0;
	}
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
		int i,j;
		for (i = 0; i < ARENA_HEIGHT; ++i)
		{
			for (j = 0; j < ARENA_WIDTH; ++j)
			{
				if(i == 0 || i == ARENA_HEIGHT - 1 || j == 0 || j == ARENA_WIDTH - 1)
				{
					curr_state->arena[i][j] = NONBREAKABLE_WALL;
				}
				else
				{
					curr_state->arena[i][j] = BLANK_SPACE;
				}
			}
		}
	}
}

void play_game(int fd)
{
	fd_set serverFD;
	FD_ZERO(&serverFD);
	FD_SET(fd, &serverFD);
	char buffer[BUFF_SIZE];
	gameState curr_state;
	int my_id;
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
				sprintf(buffer, "%d %c", my_id, inp);
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
				print_arena(curr_state);
				printf("%d\n", my_id);
			}
			else
			{
				int tank_id;
				char key_press;
				sscanf(buffer, "%d %c", &tank_id, &key_press);
				move_tank(&curr_state, tank_id, key_press);
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
	if(self)
	{
		// connect to 127.0.0.1
		strcpy(IP, "127.0.0.1");
	}
	else
	{
		// TODO: take input for IP
		strcpy(IP, "127.0.0.1");
	}
	int fd = setup_client_socket(IP);
	play_game(fd);
}

void* join_self_room(void *args)
{
	join_room(1);
}

void host_room()
{
	// start room
	clientData client_list[MAXCONN];
	int num_clients = 0;
	
	int socket_listen_desc = setup_listen_socket();

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
				usleep(5000);
				break;
			}
			if(inp == CLOSE_ROOM)
				return;
		}
	}

	listen_on_socket(socket_listen_desc, client_list, &num_clients);
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
	int level = 1;
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
	while(1)
	{
		clock_t curr_time = clock();
		double time_elapsed = (double)(curr_time - start_time)/CLOCKS_PER_SEC;
		if(time_elapsed > REFRESH_TIME)
		{
			strcpy(buffer, "UPD_PLISS");
			send_to_fdset(client_fds, max_fd, buffer);
			start_time = clock();
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
			sscanf(buffer, "%d %c", &from_id, &key_press);
			// printf("%s\n", buffer);
			if(move_tank(&curr_state, from_id, key_press))
			{
				// printf("approved %s\n", buffer);
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
		char *inp_ptr = get_input();
		if(inp_ptr == NULL)
			continue;
		char inp = *inp_ptr;
		if(inp == HOST_ROOM)
			host_room();
		if(inp == JOIN_ROOM)
			join_room(0);
		if(inp == EXIT_GAME)
			break;
		if(inp == ESCAPE)
			break;
	}
	system("stty echo");
}