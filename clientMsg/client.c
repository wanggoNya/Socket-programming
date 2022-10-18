#include <stdio.h>
#include <stdlib.h> // atoi()
#include <string.h> // bzero()
#include <unistd.h> // read(), write(), close()
#include <sys/socket.h> // socket() connect()
#include <arpa/inet.h> // inet_addr()
#define MAX 80
#define SA struct sockaddr

static void chat(int clntfd)
{
		char buff[MAX];
		int n;
		while(1) {
				bzero(buff, sizeof(buff));
				printf("Enter the string : ");

				n = 0;
				while ((buff[n++] = getchar()) != '\n');

				write(clntfd, buff, sizeof(buff));

				if ((strncmp(buff, "exit", 4)) == 0) {
						printf("Client Exit...\n");
						break;
				}

				bzero(buff, sizeof(buff));
				read(clntfd, buff, sizeof(buff));
				printf("From Server : %s", buff);
		}
}

int main(int argc, char* argv[])
{
		int clntfd;
		struct sockaddr_in servaddr;

		if(argc != 3)
		{
				printf("%s <IP> <PORT>\n", argv[0]);
				exit(1);
		}

		clntfd = socket(PF_INET, SOCK_STREAM, 0);
		if (clntfd == -1) {
				printf("socket creation failed...\n");
				exit(0);
		}
		else printf("Socket successfully created..\n");

		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);
		servaddr.sin_port = htons(atoi(argv[2]));

		if (connect(clntfd, (SA*)&servaddr, sizeof(servaddr))
						!= 0) {
				printf("connection with the server failed...\n");
				exit(0);
		}
		else printf("connected to the server..\n");

		chat(clntfd);

		close(clntfd);
}
