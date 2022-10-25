#include "client.h"

void usage(char *argv[])
{
		printf("%s <IP> <PORT>\n", argv[0]);
}

void chat(int clntfd)
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

int socketCreation(char *argv[])
{
		int clntfd;
		struct sockaddr_in servaddr;

		clntfd = socket(PF_INET, SOCK_STREAM, 0);
		if (clntfd == -1) {
				printf("socket creation failed...\n");
				usage(argv);
				return 1;
		}
		else printf("Socket successfully created..\n");

		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);
		servaddr.sin_port = htons(atoi(argv[2]));

		if (connect(clntfd, (SA*)&servaddr, sizeof(servaddr))
						!= 0) {
				printf("connection with the server failed...\n");
				return 1;
		}
		else printf("connected to the server..\n");

		chat(clntfd);

		close(clntfd);

		return 0;
}
