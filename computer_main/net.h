#ifndef NET_H
#define NET_H

#include<stdio.h>
#include<winsock2.h>
#include <thread>
#include "config.h"

extern int infopi1;
extern int infopi2;

extern int coorpi1;
extern int coorpi2;

extern char buf[4][10000];
extern const char *message;
extern int num;
extern int infopi[10];
extern int index_info;

extern int pi_id[MAX_CLIENT];

extern SOCKET s, new_socket1, new_socket2, new_socket[10];
extern struct sockaddr_in server, client1, client2, client[10];

//int getDirectKey();
int tcpcommain();
void thread_s(int i);
int tcpcommenu(int argc, char *argv[]);
void print_infopi(char buf[]);
//int udpmain();
#endif
