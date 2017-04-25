#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>


#define PORT "4003"
#define MAXCONN 6

int main()
{
	
	fd_set mainList;
	fd_set tempList;
	fd_set clientList;
	FD_ZERO(&mainList);
	FD_ZERO(&tempList);
	FD_ZERO(&clientList);
	struct addrinfo hints, *serveraddr, *p;
	int maxFD;
	int socketListenDesc;
	int err1;
	int socketReuse=1;
	int i;
	int addr_size;
	struct sockaddr_storage their_address;
	int newFileDescr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags  = AI_PASSIVE;
	if((err1 = getaddrinfo(NULL, PORT, &hints, &serveraddr)) != 0)
	{
		fprintf(stderr, "GetAddrinfo: %s\n", gai_strerror(err1));
		exit(1);
	}

	for(p = serveraddr; p!= NULL; p->ai_next)
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

	FD_SET(socketListenDesc, &mainList);


	maxFD = socketListenDesc;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	while(1)
	{
		
		tempList = mainList;
		if(select(socketListenDesc+1,&tempList,NULL, NULL, &tv) == -1)
		{
			perror("Select statement");
			exit(4);
		}
				
		if(FD_ISSET(socketListenDesc, &tempList))
		{
			addr_size = sizeof their_address;
			newFileDescr = accept(socketListenDesc, (struct sockaddr *)&their_address, &addr_size);
			// write printf to tell the ip of client
			if(newFileDescr == -1)
			{
				perror("Error while accepting");
			}
			else
			{
				FD_SET(newFileDescr, &clientList);
				if(newFileDescr > maxFD)
					maxFD = newFileDescr;
				
			}		
		}
		
	}
}


