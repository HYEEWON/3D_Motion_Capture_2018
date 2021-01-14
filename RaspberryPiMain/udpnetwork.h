#ifndef RASNET_H
#define RASNET_H

#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT     8888 
#define MAXLINE 1024 

struct udpparameter
{
	int sockfd;
	double buffer[MAXLINE];
	struct sockaddr_in servaddr;
	double array[MAXLINE];
};


struct udpparameter udpsock();
void udpsend(struct udpparameter *p, int, int, double, double);
#endif
