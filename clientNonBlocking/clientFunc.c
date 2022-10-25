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

		//u_long isNonBlocking = 1;
//		int flag = fcntl(socket, F_GETFL,0);
//		fcntl(socket, F_SETFL, flag | O_NONBLOCK);
	
//		ioctlsocket(socket,        //Non-Blocking으로 변경할 소켓
//						FIONBIO,       //변경할 소켓의 입출력 모드
//						&isNonBlocking //넘기는 인자, 여기서는 nonblocking설정 값
//				   );

		if (clntfd == -1) {
				printf("socket creation failed...\n");
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

		other_routine();
		close(clntfd);

		return 0;
}

void other_routine(void)
{
		printf("----- Other routine processing\n");
}
