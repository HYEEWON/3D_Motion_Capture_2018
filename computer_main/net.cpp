#include<stdio.h>
#include <iostream>
#include <thread>
#include<winsock2.h>
#include<ws2tcpip.h>
#include "config.h"
#include "net.h"
#include "opencv.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

SOCKET s, new_socket[10];
struct sockaddr_in server, client[10];

int pi_id[MAX_CLIENT];

char buf[4][10000];
int infopi[10];

char coor[10000];
//int coorinfo[10];
int coorpi1 = 0;
int coorpi2 = 0;

int index_info;
int num = 0;
const char *message;
void thread_s(int index_info);

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int tcpcommain()
{
	WSADATA wsa;
	std::thread pis[4];

	int c;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return false;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		closesocket(s);
		return false;
	}

	printf("Socket created.\n");

	int opt = 1;
	int result = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int));
	if (result < 0) {
		printf("setsockopt Error: %d", WSAGetLastError());
		closesocket(s);
		return false;
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_TCP_PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		closesocket(s);
		return false;
	}

	puts("Bind done");

	//Listen to incoming connections
	if (listen(s, 5) == SOCKET_ERROR)
	{
		printf("Listen failed with error code : %d", WSAGetLastError());
		closesocket(s);
		return false;
	}

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);


	for (index_info = 0; index_info < MAX_CLIENT; ++index_info)
	{
		new_socket[index_info] = accept(s, (struct sockaddr *)&client[index_info], &c);

		if (new_socket[index_info] == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d", WSAGetLastError());
			closesocket(s);
			return false;
		}
		puts("Connection accepted");
		pis[index_info] = std::thread(thread_s, index_info);

	}
	for (index_info = 0; index_info < MAX_CLIENT; ++index_info)
	{
		pis[index_info].join();
	}


	//closesocket(s);
	//WSACleanup();
	return 0;
}

void thread_s(int index_info)
{
	//Reply to client

	int recvnum;
	int sendnum;
	int totalnum = 0;
	char buf[10000];

	// Receive hostname
	recvnum = recv(new_socket[index_info], buf, 10000, 0);
	if (recvnum <= 0) {
		printf("Recv failed with error code : %d", WSAGetLastError());
		closesocket(s);
		exit(EXIT_FAILURE);
	}
	int id = buf[17] - '0' - 1;
	if (id >= MAX_CLIENT || id < 0) {
		printf("Wrong Pi ID: %d", id);
		closesocket(s);
		exit(EXIT_FAILURE);
	}

	// Reply
	sendnum = send(new_socket[index_info], "hostnameok", 11, 0);
	if (sendnum <= 0) {
		printf("Send failed with error code : %d", WSAGetLastError());
		closesocket(s);
		exit(EXIT_FAILURE);
	}

	// Get camera info
	while (totalnum < 164) {
		recvnum = recv(new_socket[index_info], buf + totalnum, 10000, 0);
		if (recvnum == SOCKET_ERROR) {
			printf("Recv failed with error code : %d", WSAGetLastError());
			closesocket(s);
			exit(EXIT_FAILURE);
		}

		totalnum += recvnum;
	}


	print_infopi(buf);
	//if (*(int*)buf == 1 && recvnum >= 164)
	//{
	//	infopi[index_info] = 1;
	//}
	/*if (*(int*)buf[index_info] == 1 && recvnum[index_info] >= 164)
	{
		infopi[index_info] = 1;
	}
	if (*(int*)buf[index_info] == 2 && recvnum[index_info] >= 164)
	{
		infopi[index_info] = 1;
	}*/
	printf("char1: %d\n", buf[0]);
	printf("ID: %d\n", id);

	convertToMat(buf, mtx[id], dist[id], R[id], T[id], RT[id]);
	printCameraMatrix(id);
}

void print_infopi(char buf[])
{
	int q;
	int* intp;
	double* doublelep;
	intp = (int*)buf;
	doublelep = (double*)(buf + 4);
	for (q = 0; q < 20; q++)
	{
		printf("%lf\t", doublelep[q]);
	}
	printf("\n");
}

//closesocket(s);
//WSACleanup();

int tcpcommenu(int argc, char *argv[])
{
	std::cout << "num: ";
	std::cin >> num;
	switch (num)
	{
	case 1:
		message = "Pose_Estimation";
		infopi[0] = 0; infopi[1] = 0;
		break;
	case 2:
		message = "Tracking";
		break;
	case 3:
		message = "Finish";
		break;
	default:
		message = " ";
		break;
	}
	for (int h = 0; h <= 1; h++)
	{
		int numbuf = 0, numf;
		while (numbuf < strlen(message)) {
			numf = send(new_socket[h], message + numbuf, strlen(message)+1-numbuf, 0);
			numbuf += numf;
		}
	}
	printf("%s\n", message);
	int temp = num;
	num = 100;
	return temp;
}
//getchar();

/*for (int y = 0; y <= 163; y++)
{
   printf("%d\n", buf[y]);
}*/
//closesocket(s);
//WSACleanup();
