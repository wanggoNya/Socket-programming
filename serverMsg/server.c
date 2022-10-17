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


// Function designed for chat between client and server.
static int func(int connfd)
{
		char buff[MAX];
		int n;
		// infinite loop for chat
		while(1) {
				bzero(buff, MAX);

				// read the message from client and copy it in buffer
				read(connfd, buff, sizeof(buff));
				// print buffer which contains the client contents
				printf("From client: %s\t To client : ", buff);

				if (strncmp("exit", buff, 4) == 0) {
						printf("Client Exit...\n");
						return 1;
						break;
				}

				bzero(buff, MAX);
				n = 0;
				// copy server message in the buffer
				while ((buff[n++] = getchar()) != '\n');

				// and send that buffer to client
				write(connfd, buff, sizeof(buff));

				// if msg contains "Exit" then server exit and chat ended.
				if (strncmp("exit", buff, 4) == 0) {
						printf("Server Exit...\n");
						return 0;
						break;
				}
		}
		return 1;
}

// Driver function
int main(int argc, char *argv[])
{
		int sockfd, connfd;
		struct sockaddr_in servaddr, cli;
		socklen_t len;

		if(argc != 2)
		{
				printf("%s <port>\n", argv[0]);
				exit(1);
		}

		// socket create and verification
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
				printf("socket creation failed...\n");
				exit(0);
		}
		else
				printf("Socket successfully created..\n");
		bzero(&servaddr, sizeof(servaddr));

		// assign IP, PORT
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(atoi(argv[1]));

		// Binding newly created socket to given IP and verification
		if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
				printf("socket bind failed...\n");
				exit(0);
		}
		else
				printf("Socket successfully binded..\n");

		// Now server is ready to listen and verification
		if ((listen(sockfd, 5)) != 0) {
				printf("Listen failed...\n");
				exit(0);
		}
		else
				printf("Server listening..\n");
	
		int funcReturn = 1;
		while(funcReturn)
		{
				len = sizeof(cli);

				// Accept the data packet from client and verification
				connfd = accept(sockfd, (SA*)&cli, &len);
				if (connfd < 0) {
						printf("server accept failed...\n");
						exit(0);
				}
				else
						printf("server accept the client...\n");

				// Function for chatting between client and server
				printf("\n=====client 연결=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cli.sin_addr), argv[1]);

				funcReturn = func(connfd);

				printf("\n=====client 종료=====\nip : %s\nport : %s\n=====================\n\n",
								inet_ntoa(cli.sin_addr), argv[1]);

				printf("Server listening..\n");
		}

		// After chatting close the socket
		close(sockfd);
}
