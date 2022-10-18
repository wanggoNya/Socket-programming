#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 80
#define SA struct sockaddr

static int chat(int privfd)
{
		char buff[MAX];
		int n;
		
		while(1) {
				bzero(buff, MAX);

				read(privfd, buff, sizeof(buff));
				
				printf("From client: %s\t To client : ", buff);

				if (strncmp("exit", buff, 4) == 0) {
						printf("Client Exit...\n");
						return 1;
				}

				bzero(buff, MAX);
				n = 0;

				while ((buff[n++] = getchar()) != '\n');

				write(privfd, buff, sizeof(buff));

				if (strncmp("exit", buff, 4) == 0) {
						printf("Server Exit...\n");
						return 0;
				}
		}
		return 1;
}

int main(int argc, char *argv[])
{
		int servfd, privfd;
		struct sockaddr_in servaddr, cliaddr;
		socklen_t len;

		if(argc != 2)
		{
				printf("%s <port>\n", argv[0]);
				exit(1);
		}

		servfd = socket(PF_INET, SOCK_STREAM, 0);
		if (servfd == -1) {
				printf("socket creation failed...\n");
				exit(0);
		}
		else printf("Socket successfully created..\n");

		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(atoi(argv[1]));

		if ((bind(servfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
				printf("socket bind failed...\n");
				exit(0);
		}
		else printf("Socket successfully binded..\n");

		if ((listen(servfd, 5)) != 0) {
				printf("Listen failed...\n");
				exit(0);
		}
		else printf("Server listening..\n");
	
		int funcReturn = 1;
		while(funcReturn)
		{
				len = sizeof(cliaddr);
				
				privfd = accept(servfd, (SA*)&cliaddr, &len);
				if (privfd < 0) {
						printf("server accept failed...\n");
						exit(0);
				}
				else printf("server accept the client...\n");

				printf("\n=====client 연결=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cliaddr.sin_addr), argv[1]);

				funcReturn = chat(privfd);

				printf("\n=====client 종료=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cliaddr.sin_addr), argv[1]);

				printf("Server listening..\n");
		}

		close(servfd);
}
