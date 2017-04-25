#include "gameDef.h"

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
		fprintf(stderr, "GetAddrinfo: %s\n", gai_strerror(err1));
		exit(1);
	}

	for(p = serveraddr; p!= NULL; p = p->ai_next)
	{
		socketListenDesc = socket(p->ai_family, p->ai_socktype, p->ai_protocol );
		if(socketListenDesc < 0)
			continue;
		if(setsockopt(socketListenDesc, SOL_SOCKET, SO_REUSEADDR, &socketReuse, sizeof(int)) == -1)
		{
			perror("Address already in use ");
			exit(1);
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
		exit(2);
	}

	freeaddrinfo(serveraddr);

	if(listen(socketListenDesc, MAXCONN) == -1)
	{
		perror("Error in Listen.");
		exit(3);
	}

	return socketListenDesc;
}

int setup_client_socket(char serverIP[])
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
		fprintf(stderr, "GetAddrinfo: %s\n", gai_strerror(err1));
		exit(1);
	}

	for(p = serveraddr; p!= NULL; p = p->ai_next)
	{
		socketListenDesc = socket(p->ai_family, p->ai_socktype, p->ai_protocol );
		
		if(socketListenDesc < 0)
			continue;

		if(setsockopt(socketListenDesc, SOL_SOCKET, SO_REUSEADDR, &socketReuse, sizeof(int)) == -1)
		{
			perror("Address already in use ");
			exit(1);
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
		fprintf(stderr, "Client failed to connect\n" );
		exit(2);
	}

	freeaddrinfo(serveraddr);
	return socketListenDesc;
}

void listen_on_socket(int socketListenDesc, clientData clientList[], int *numClients)
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
		perror("Select statement");
		exit(4);
	}
	if(FD_ISSET(socketListenDesc, &mainList))
	{
		addr_size = sizeof their_address;
		newFileDescr = accept(socketListenDesc, (struct sockaddr *)&their_address, &addr_size);
		printf("someone trying to join at fd = %d, will be given id = %d\n", newFileDescr, *numClients);
		if(newFileDescr == -1)
		{
			perror("Error while accepting");
		}
		else
		{
			*numClients = *numClients + 1;
			clientList[*numClients - 1].fd = newFileDescr;
			clientList[*numClients - 1].id = *numClients - 1;
		}
	}
}

int listen_on_fdset(fd_set fdSet, int maxVal, char buffer[])
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if(select(maxVal + 1, &fdSet, NULL, NULL, &tv) == -1)
	{
		perror("Select statement");
		exit(4);
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