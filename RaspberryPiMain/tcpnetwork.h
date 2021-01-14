#ifndef NET_H
#define NET_H

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

extern const char *message;
extern int server_s, client_s;
extern struct sockaddr_in server, client;
extern char mes[10000];
extern char hostname[100];
extern int pi_id;

int tcpmain();
int read_camera_file(char *filename);
//int main(int argc, char *argv[]);
//int udpmain(int argc, char *argv[]);
//int tcpmenu(int argc, char *argv[]);

extern char server_address[16];

#endif

