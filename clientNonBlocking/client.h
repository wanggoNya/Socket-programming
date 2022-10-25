#include <stdio.h>
#include <stdlib.h> // atoi()
#include <string.h> // bzero()
#include <unistd.h> // read(), write(), close()
#include <sys/socket.h> // socket() connect()
#include <arpa/inet.h> // inet_addr()
#define MAX 80
#define SA struct sockaddr

#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>

void usage(char *argv[]);
void chat(int clntfd);
int socketCreation(char *argv[]);
