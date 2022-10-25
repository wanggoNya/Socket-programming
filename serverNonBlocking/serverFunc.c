#include "server.h"

void usage(char *argv[])
{
		printf("%s <PORT>\n", argv[0]);
		exit(1);
}

int chat(int privfd)
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

int socketCreation(char *argv[])
{
		int servfd, privfd;
		struct sockaddr_in servaddr, cliaddr;
		socklen_t len;

		servfd = socket(PF_INET, SOCK_STREAM, 0);
		if (servfd == -1) {
				printf("socket creation failed...\n");
				return 1;	
		}

		if( fcntl( servfd, F_SETFL, O_NONBLOCK) == -1 ) {
				return -1; // error
		}

		int error = 0;
		socklen_t len2 = sizeof( error );
		if( getsockopt(servfd, SOL_SOCKET, SO_ERROR, &error, &len2) < 0 ) {
				// 값을 가져오는데 에러 발생
				// errno을 가지고 에러 값을 출력
				// 연결 오류로 처리
		}

		else printf("Socket successfully created..\n");

		bzero(&servaddr, sizeof(servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(atoi(argv[1]));

		if ((bind(servfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
				printf("socket bind failed...\n");
				return 1;	
		}
		else printf("Socket successfully binded..\n");

		if ((listen(servfd, 5)) != 0) {
				printf("Listen failed...\n");
				return 1;	
		}
		else printf("Server listening..\n");

		int funcReturn = 1;
		while(funcReturn)
		{
				len = sizeof(cliaddr);

				privfd = accept(servfd, (SA*)&cliaddr, &len);
				if (privfd < 0) {
						printf("server accept failed...\n");
						return 1;
				}
				else printf("server accept the client...\n");

				printf("\n=====client 연결=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cliaddr.sin_addr), argv[1]);

				funcReturn = chat(privfd);

				printf("\n=====client 종료=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cliaddr.sin_addr), argv[1]);

				if(funcReturn) printf("Server listening..\n");
				else printf("Server Closing..\n");
		}

		close(servfd);
		return 0;
}

void other_routine(void)
{
		printf("----- Other routine processing\n");
}
