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

int main(int argc, char **argv)
{
	int i;
	const int numHosts = argc - 2;
	char *hosts[numHosts];
	int sock = atoi(argv[1]);
	for (i = 0; i < argc-2; i++)
	{
		hosts[i] = argv[i + 2];
		printf("host1: %s\n", hosts[i]);
		fflush(stdout);
	}

	//Create Block
	Block block = createBlock();

	if (broadcastBlock(block, argv[1], hosts, numHosts))
		saveBlock(block);
	return 0;
}