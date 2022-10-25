#include <stdio.h>
#include <stdlib.h> // atoi()
#include <string.h> // bzero()
#include <unistd.h> // read(), write(), close()
#include <netinet/in.h> // in_addr, sockaddr_in //#include <sys/socket.h> 
#include <arpa/inet.h> // inet_addr()
#define MAX 80
#define SA struct sockaddr


#include <errno.h>
#include <fcntl.h>

void usage(char *argv[]);
int chat(int privfd);
int socketCreation(char *argv[]);

void routine(void);
