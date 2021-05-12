/* Cody Girouard
 * client - server-client communication using UDP
 * simulate ping test over UDP
 */
 #include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>   /* inet(3) functions */
#include <sys/time.h>

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 3) // incorrect command line format
	{
		printf("usage : ./client <hostname> <port>\n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	// get ip from host name
	struct hostent *hosten;
	hosten = gethostbyname(argv[1]);
	if (hosten == NULL) // ip to correspond to hostname not found
	{
		printf("hostname not found\n");
		exit(EXIT_FAILURE);
	}
	
	memcpy(&servaddr.sin_addr, hosten->h_addr_list[0], hosten->h_length); // insert ip into sockaddr_in's sin_addr
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket using internet and UDP
	if (sockfd == 0)
	{
		perror("socket failed\n");
		exit(EXIT_FAILURE);
	}
	
	// set timeout for client receive
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) 
	{
		perror("setsockopt error\n");
		exit(EXIT_FAILURE);
	}

	dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	exit(0);
}

void dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
	int n, i;
	char sendline[] = "PING\n", recvline[21];
	suseconds_t min = 9000000, max = 0, avg = 0;
	double sent = 0, received = 0;

	for (i = 0; i < 10; i++)
	{
		struct timeval begin, end; // used to calculate RTT
		gettimeofday(&begin, NULL); // start time
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen); // send ping to server
		sent++;
		n = recvfrom(sockfd, recvline, 20, 0, NULL, NULL); // get response from server, n is length of string received (-1 if no response)
		gettimeofday(&end, NULL); //  end time
		recvline[n] = '\0';    /* null terminate */

		printf("%2d: Sent... ", i+1);
		struct timeval diff; //  store RTT is microseconds, convert to milliseconds in printf
		diff.tv_sec = 0;
		diff.tv_usec = (end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec;
		suseconds_t usec = diff.tv_usec;
		if (n >= 0)
		{
			// not lost
			printf("RTT=0.%lu000 ms\n", usec);
			received++;
			min = (usec < min)? usec : min;
			max = (usec > max)? usec : max;
			avg = avg + usec;
		}
		else
		{
			// lost
			printf("Timed out\n");
		}
	}
	double average = (double)avg/received;
	printf("%d pkts xmited, %d pkts rcvd, %d%% pkt loss\nmin: %.6f ms, max: %.6f ms, avg: %.6f ms\n", (int)sent, (int)received, (int)(100*(sent-received)/sent), (double)min/1000, (double)max/1000, average/1000);
} 
