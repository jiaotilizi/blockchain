#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include "blockchain.h"

// create both a client and publisher now
int main(int argc, char **argv)
{
	int blockCount = 0;
	if (argc < 2)
		errexit("usage: TCPechod [port]\n");
	
	char *service = argv[1];
	
	int i = 2, j = 0;
	const int hostCount= argc - 2;
	char *hosts[hostCount];
	
	for (; i < argc, j < hostCount; i++, j++)
	{
		hosts[j] = argv[i];
	}
	
	(void) signal(SIGCHLD, reaper);
	
	if (fork() == 0)
	{
		//child = server
		if (server(service) == 1)
		{
			errexit("Server Error\n Closing Program\n");
		}
	}
	else {
		//parent = client
		while (1)
		{
			//system("clear");
			printf("\n\tPress ENTER to launch UI\n");
			fflush(stdout);
			getchar();
			
			//int client(const int numHosts, char **hosts, char *sock);
			client(hostCount, hosts, service);
		}
	}
}
