#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define ServerIP "123"
#define PORT "4003"
#define MAXCONN 6

int main()
{
	struct addrinfo hints, *serveraddr, *p;
	int socketListenDesc;
	int err1;
	int socketReuse=1;
	

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags  = AI_PASSIVE;
	if((err1 = getaddrinfo(ServerIP, PORT, &hints, &serveraddr)) != 0)
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

	
		
	
}


