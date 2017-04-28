#include "gameDef.h"
#include "game_security.h"

int setup_listen_socket()
{
	struct addrinfo hints, *serveraddr, *p;
	int socketListenDesc;
	int err1;
	int socketReuse=1;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags  = AI_PASSIVE;
	if((err1 = getaddrinfo(NULL, PORT, &hints, &serveraddr)) != 0)
	{
		printf("An error occured in trying to host a game\n");
		return -1;
	}

	for(p = serveraddr; p!= NULL; p = p->ai_next)
	{
		socketListenDesc = socket(p->ai_family, p->ai_socktype, p->ai_protocol );
		if(socketListenDesc < 0)
			continue;
		if(setsockopt(socketListenDesc, SOL_SOCKET, SO_REUSEADDR, &socketReuse, sizeof(int)) == -1)
		{
			printf("An error occured in trying to host a game\n");
			return -1;
		}
		if(bind(socketListenDesc, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(socketListenDesc);
			continue;
		}
		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "Binding failed\n" );
		return -1;
	}

	freeaddrinfo(serveraddr);

	if(listen(socketListenDesc, MAXCONN) == -1)
	{
		printf("Error in Listen.\n");
		return -1;
	}

	return socketListenDesc;
}

int setup_client_socket(char serverIP[], char nick[])
{
	struct addrinfo hints, *serveraddr, *p;
	int socketListenDesc;
	int err1;
	int socketReuse=1;
	

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags  = AI_PASSIVE;
	if((err1 = getaddrinfo(serverIP, PORT, &hints, &serveraddr)) != 0)
	{
		printf("An error occured in joining the game\n");
		return -1;
	}

	for(p = serveraddr; p!= NULL; p = p->ai_next)
	{
		socketListenDesc = socket(p->ai_family, p->ai_socktype, p->ai_protocol );
		
		if(socketListenDesc < 0)
			continue;

		if(setsockopt(socketListenDesc, SOL_SOCKET, SO_REUSEADDR, &socketReuse, sizeof(int)) == -1)
		{
			printf("Address already in use\n");
			return -1;
		}

		if(connect(socketListenDesc, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(socketListenDesc);
			continue;
		}

		break;

	}

	if(p == NULL)
	{
		printf("Client failed to connect\n" );
		return -1;
	}

	freeaddrinfo(serveraddr);
	char buffer[BUFF_SIZE], plaintext[BUFF_SIZE];
	while(recv(socketListenDesc, buffer, BUFF_SIZE, 0) <= 0);
	decrypt_scheme_1(buffer, plaintext);
	encrypt_scheme_2(plaintext, buffer);
	send(socketListenDesc, buffer, BUFF_SIZE, 0);
	send(socketListenDesc, nick, BUFF_SIZE, 0);
	return socketListenDesc;
}

int listen_on_socket(int socketListenDesc, clientData clientList[], int *numClients)
{
	fd_set mainList;
	int addr_size;
	struct sockaddr_storage their_address;
	int newFileDescr;
	FD_ZERO(&mainList);
	FD_SET(socketListenDesc, &mainList);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if(select(socketListenDesc+1, &mainList, NULL, NULL, &tv) == -1)
	{
		printf("An error occured while trying to host the room\n");
		return -1;
	}
	if(FD_ISSET(socketListenDesc, &mainList))
	{
		addr_size = sizeof their_address;
		newFileDescr = accept(socketListenDesc, (struct sockaddr *)&their_address, &addr_size);
		if(newFileDescr == -1)
		{
			printf("An error occured while trying to host the room\n");
			return -1;
		}
		else
		{
			srand(time(NULL));
			char message[BUFF_SIZE], cipher1[BUFF_SIZE], cipher2[BUFF_SIZE];
			int random_num = rand()%1000000000;
			sprintf(message, "%d", random_num);
			encrypt_scheme_1(message, cipher1);
			encrypt_scheme_2(message, cipher2);
			send(newFileDescr, cipher1, BUFF_SIZE, 0);
			while(recv(newFileDescr, message, BUFF_SIZE, 0) <= 0);
			if(strcmp(cipher2, message))
				return 1;

			*numClients = *numClients + 1;
			clientList[*numClients - 1].fd = newFileDescr;
			clientList[*numClients - 1].id = *numClients - 1;
			while(recv(newFileDescr, clientList[*numClients - 1].nick, BUFF_SIZE, 0) <= 0);
		}
		printf("%s connected\n", clientList[*numClients - 1].nick);
	}
	return 1;
}

int listen_on_fdset(fd_set fdSet, int maxVal, char buffer[])
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if(select(maxVal + 1, &fdSet, NULL, NULL, &tv) == -1)
	{
		return -1;
	}
	int i;
	strcpy(buffer, "");
	for (i = 0; i < maxVal + 1; ++i)
	{
		if(FD_ISSET(i, &fdSet))
		{
			int recvSize = recv(i, buffer, BUFF_SIZE, 0);
			if(recvSize > 0)
			{
				return recvSize;
			}
		}
	}
	return 0;
}

void send_to_fdset(fd_set fdSet, int maxVal, char buffer[])
{
	int i;
	for (i = 0; i < maxVal + 1; ++i)
	{
		if(FD_ISSET(i, &fdSet))
		{
			send(i, buffer, BUFF_SIZE, 0);
		}
	}
}