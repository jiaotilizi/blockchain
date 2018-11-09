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

#define	LINELEN	128
#define	BUFSIZE	4096
#define	QLEN 5

int server(char *service)
{
	struct	sockaddr_in fsin;	/* the address of a client	*/
	int	alen;			/* length of client's address	*/
	int	msock;			/* master server socket		*/
	int	ssock;			/* slave server socket		*/
	
	Block block;

	msock = passiveTCP(service, QLEN);

	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0) {
			if (errno == EINTR)   //system call was interrupted permaturely with a signal before it was able to complete
				continue;     //should retry the syscall 
			errexit("accept: %s\n", strerror(errno));
		}
		switch (fork()) {
		case 0:		/* child */
			(void) close(msock);
			exit(recieveBlock(&block, ssock) == false);
		default:	/* parent */
			(void) close(ssock);
			break;
		case -1:
			errexit("fork: %s\n", strerror(errno));
		}
	}
	
	return 1;
}

int client(const int numHosts, char **hosts, char *sock)
{
	
	//Create Block
	Block block = createBlock();
	if (broadcastBlock(block, sock, hosts, numHosts))
	{
		printf("Attempting to save block: %s\n", block.blockTitle);
		saveBlock(block);
	}
	return 0;
}

Block createBlock()
{
	Block block = { "LF2nPWzQe68RDLiO3BPIYlV2NlBNT4vk22hdXj2V07BFgIulxkfJyybT2yClRUTcm7AYqn8RSS9cfedbzN4Dddpyv912ZQc2DDsVpdja0HwllsLltEeFhE5MAkRJ39BJz1r60JykCEHlbJAATkIDrc4LlPuqQ2bZsnPVwiYaiBStwKMTudioLNzNtUgg9swtyb2erxtPKlIbgIibZbXPOSKkas0uV5hoKxKnuW9UxAv5hRSumHMirIYSmfuuecX1",
					"LF2nPWzQe68RDLiO3BPIYlV2NlBNT4vk22hdXj2V07BFgIulxkfJyybT2yClRUTcm7AYqn8RSS9cfedbzN4Dddpyv912ZQc2DDsVpdja0HwllsLltEeFhE5MAkRJ39BJz1r60JykCEHlbJAATkIDrc4LlPuqQ2bZsnPVwiYaiBStwKMTudioLNzNtUgg9swtyb2erxtPKlIbgIibZbXPOSKkas0uV5hoKxKnuW9UxAv5hRSumHMirIYSmfuuecX1",
					"LF2nPWzQe68RDLiO3BPIYlV2NlBNT4vk22hdXj2V07BFgIulxkfJyybT2yClRUTcm7AYqn8RSS9cfedbzN4Dddpyv912ZQc2DDsVpdja0HwllsLltEeFhE5MAkRJ39BJz1r60JykCEHlbJAATkIDrc4LlPuqQ2bZsnPVwiYaiBStwKMTudioLNzNtUgg9swtyb2erxtPKlIbgIibZbXPOSKkas0uV5hoKxKnuW9UxAv5hRSumHMirIYSmfuuecX1",
					"0",
					0,
					"LF2nPWzQe68RDLiO3BPIYlV2NlBNT4vk22hdXj2V07BFgIulxkfJyybT2yClRUTcm7AYqn8RSS9cfedbzN4Dddpyv912ZQc2DDsVpdja0HwllsLltEeFhE5MAkRJ39BJz1r60JykCEHlbJAATkIDrc4LlPuqQ2bZsnPVwiYaiBStwKMTudioLNzNtUgg9swtyb2erxtPKlIbgIibZbXPOSKkas0uV5hoKxKnuW9UxAv5hRSumHMirIYSmfuuecX1",
					time(NULL)
	};
					
	return block;
}

void saveBlock(Block block)
{
	FILE *file;
	file = fopen(block.blockTitle, "w");
	//printf("Attempting to save block: %s\n", block.blockTitle);
	if (file == NULL) 
    { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }
	fwrite (&block, sizeof(Block), 1, file); 
	if(fwrite == 0)  
	{
        printf("Error writing file\n"); 
		exit (1);
	}
	fclose(file);
}

Block loadBlock(char *str)
{
	Block block;
	FILE *file;
	file = fopen(str, "r");
	if (file == NULL) 
    { 
        fprintf(stderr, "\nError opening file\n"); 
        exit (1); 
    }
	while(fread(&block, sizeof(Block), 1, file));
    // close file 
    fclose(file); 
	return block;
}

bool broadcastBlock(Block block, char* sock, char** hosts, int numHosts)
{
	//make multiple TCPsend calls
	char buf[LINELEN+1];		/* buffer for one line of text	*/
    char endoffile[]="End of file\n";
	int	s, i, n, cc;			/* socket descriptor, read count*/
	int	outchars, inchars;	/* characters sent and received	*/
	char validMssg[]="Block Valid\n";
	for (i = 0; i < numHosts; i++)
	{
		char *host = hosts[i];
		s = connectTCP(host, sock);
		if (write(s, &block, sizeof(Block)) < 0)
			errexit("echo write: %s\n", strerror(errno));
		
		while (cc = read(s, buf, sizeof buf)) {
			if (cc < 0)
				errexit("echo read: %s\n", strerror(errno));
			if(strncmp(buf,validMssg,strlen(validMssg))==0) {
				printf("File Recieved and Verified\n");
				break;
			}
		}
	}
	return 1;
}

bool recieveBlock(Block *block, int fd)
{
	char buf[BUFSIZ];
	char endOfFile_Indicator[]="End of file\n";
	char validMssg[]="Block Valid\n";
	int	cc;
	
	//use a while loop to determine if the block is getting filled
	cc = read(fd, block, sizeof(Block));
		//printf("recieved: %s, %d\n", buf,cc);
	if (cc < 0)
		errexit("echo read: %s\n", strerror(errno));

	//printf("Block recieved, verifying block...\n");
	fflush(stdout);
	if (validateBlock(*block))
	{
		saveBlock(*block);
		if (write(fd, validMssg, strlen(validMssg)) < 0)
			errexit("echo write: %s\n", strerror(errno));
		return true;
	}
	return false;
}

bool validateBlock(Block block)
{
	return true;
}

void reaper(int sig)
{
	int	status;

	while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
		/* empty */;
}

// int broadcastBlock()
// int verifyBlock()
// int updateBlock()
// int createTransaction()
// int verifyTransaction()
// int saveTransaction()
// int broadcastTransaction()
