/* Cody Girouard
 * server - server-client communication using UDP
 * simulate ping test over UDP
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>   /* inet(3) functions */


#define MAXLINE 4096

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
	if (argc != 2) // incorrect command line format
	{
		printf("usage: ./server <port>\n");
		exit(EXIT_FAILURE);
	}
		
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket using internet and UDP
	if (sockfd == 0)
	{
		perror("socket failed\n");
		exit(EXIT_FAILURE);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) // bind socket to address and port number
	{
		perror("bind failure\n");
		exit(EXIT_FAILURE);
	}

	dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[] = "PONG\n";
	
	printf("[server]: ready to accept data...\n");
	srand(time(0)); //  seed random number

	for( ; ; ) // inf loop
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

		printf("[client]: %s", mesg);
		
		if ((rand()%10) < 3) // 30% chance to drop packet
		{
			printf("[server]: dropped packet\n");
		}
		else
		{
			sendto(sockfd, mesg, n, 0, pcliaddr, len); // not dropped, send response
		}
	}
} 

