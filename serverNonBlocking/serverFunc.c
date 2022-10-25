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
		int read_rtn;

		while(1) {
				printf("\t\t\tread wait\n");

				bzero(buff, MAX);

				read_rtn = read(privfd, buff, sizeof(buff));

				if (read_rtn < 0)
				{
						if(errno = EAGAIN)
						{
								printf("\t\t\tread EAGAIN\n");
						}
						else
						{
								printf("----- read error\n");
								printf("----- Socket close\n");
								//	close(privfd);
								return 0;
						}
				}
				else if(read_rtn == 0)
				{

						printf("----- Socket close\n");
						//		close(privfd);
						//		break;
						return 0;
				}
				else
				{
						printf("From client: %s\t To client : ", buff);

						if (strncmp("exit", buff, 4) == 0) 
						{
								printf("Client Exit...\n");
								return 1;
						}

						bzero(buff, MAX);
						n = 0;

						while ((buff[n++] = getchar()) != '\n');

						write(privfd, buff, sizeof(buff));

						if (strncmp("exit", buff, 4) == 0) 
						{
								printf("Server Exit...\n");
								return 0;
						}
				}
				routine();
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

		int flag; 
		flag = fcntl(servfd, F_GETFL, 0 );
		fcntl(servfd, F_SETFL, flag | O_NONBLOCK );


		int funcReturn = 1;


		while(funcReturn)
		{
				len = sizeof(cliaddr);

				privfd = accept(servfd, (SA*)&cliaddr, &len);

				flag = fcntl(privfd, F_GETFL, 0 );
				fcntl(privfd, F_SETFL, flag | O_NONBLOCK );


				if (privfd < 0) {
						printf("server accept failed...\n");
						funcReturn = 1;
						routine();
						continue;
						//return 1;
				}
				else printf("server accept the client...\n");


				printf("\n=====client 연결=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cliaddr.sin_addr), argv[1]);

				funcReturn = chat(privfd);

				if(funcReturn)
				{
						printf("\n=====client 종료=====\nip : %s\nport : %s\n=====================\n\n", inet_ntoa(cliaddr.sin_addr), argv[1]);
						printf("Server listening..\n");
				}
				else printf("Server Closing..\n");
		}
		close(servfd);
		return 0;
}

void routine(void)
{
		printf("\t\t\t\t routine..........\n");
		sleep(1);
}
