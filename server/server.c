#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

static int func(int connfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
   
        // and send that buffer to client
        write(connfd, buff, sizeof(buff));
   
        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
			return 0;
        }
		return 0;
}

int main(int argc, char *argv[])
{
		int serv_sock;
		int clint_sock;
		struct sockaddr_in serv_addr;
		struct sockaddr_in clint_addr;
		socklen_t clnt_addr_size;

		if(argc != 2)  
		{
				printf("%s <port>\n", argv[0]);
				exit(1);
		}

		serv_sock = socket(PF_INET, SOCK_STREAM,0); //1번
		if(serv_sock == -1)
				printf("socket error\n");

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(atoi(argv[1]));

		if(bind(serv_sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) //2번
				printf("bind error\n");
		if(listen(serv_sock,5)==-1) //3번
				printf("listen error\n");

		clnt_addr_size = sizeof(clint_addr);

		char message[] = "hello world";
		while(1)
		{
				clint_sock = accept(serv_sock,(struct sockaddr*)&clint_addr,&clnt_addr_size); //4번
				if(clint_sock == -1)
						printf("accept error\n");
				printf("\n=====client 연결=====\nip : %s\nport : %s\n=====================\n\n", 
								inet_ntoa(clint_addr.sin_addr), argv[1]);
				write(clint_sock, message, sizeof(message)); //5번
				func(serv_sock);
				close(clint_sock);
		}
		close(serv_sock); //6번
		return 0;	
}
