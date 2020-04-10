#include<stdio.h>
#include<winsock2.h>
#include<opencv2/opencv.hpp>
#include<thread>
#include<iostream>
#include<mutex>
#include "config.h"
#include "opencv.h"
#include "opengl.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of arrfer
//#define PORT 8888   //The port on which to listen for incoming data

cv::Point2d cam_2d[MAX_CLIENT][MAX_POINT];

std::mutex point_lock;
std::mutex point_lock2;

struct udpparameter
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	double arr[4];
	WSADATA wsa;
};

void upd_thread(struct udpparameter *p)
{
	double arr[4];
	int *int_p;
	int pi_id;
	int point_id;
	double x;
	double y;
	cv::Point3d Point_3D[7];


	printf("Waiting for data...");
	fflush(stdout);


	while (1) {
		//try to receive some data, this is a blocking call
		if ((p->recv_len = recvfrom(p->s, (char*)arr, BUFLEN, 0, (struct sockaddr *) &p->si_other, &p->slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//printf("recv success\n");

		int_p = (int *)arr;
		//printf("Received packet from %s:%d\n", inet_ntoa(p->si_other.sin_addr), ntohs(p->si_other.sin_port));

		pi_id = int_p[0];
		point_id = int_p[1];
		x = arr[1];
		y = arr[2];

		//printf("Pi_id:%d,Point_id:%d,x:%lf,y:%lf\n", pi_id, point_id, x, y);

		point_lock.lock();
		cam_2d[pi_id][point_id].x = x;
		cam_2d[pi_id][point_id].y = y;
		calculation3D(cam_2d[0][point_id], cam_2d[1][point_id], RT[0], RT[1], mtx[0], mtx[1], Point_3D[point_id - 1]);
		
		//left_shoulder[0] = Point_3D[0].x; left_shoulder[1] =-Point_3D[0].y; left_shoulder[2] =-Point_3D[0].z;
		//right_shoulder[0] = Point_3D[1].x; right_shoulder[1= -Point_3D[1].y; right_shoulder[2] = -Point_3D[1].z;
		//left_elbow[0] =Point_3D[0].x; left_elbow[1] = -Point_3D[0].y; left_elbow[2] = -Point_3D[0].z;
	//	std::cout <<"1"<< "(" << left_elbow[0] << "," << left_elbow[1] << "," << left_elbow[2] <<")"<< "\n";
		//right_elbow[0] = Point_3D[1].x; right_elbow[1] = -Point_3D[1].y; right_elbow[2] = -Point_3D[1].z;


	//	std::cout <<"2"<< "(" << right_elbow[0] << "," << right_elbow[1] << "," << right_elbow[2] << ")" << "\n";
		//neck[0] = -Point_3D[0].y; neck[1] = Point_3D[0].z; neck[2] = -Point_3D[0].x;
		right_hand[0] = -Point_3D[0].x;  right_hand[1] = Point_3D[0].z; right_hand[2] = -Point_3D[0].y;
		//neck[0] = Point_3D[0].x; neck[1] = -Point_3D[0].y; neck[2] = Point_3D[0].z;
		right_elbow[0] = -Point_3D[1].x; right_elbow[1] = Point_3D[1].z; right_elbow[2] = -Point_3D[1].y;
		right_shoulder[0] = -Point_3D[2].x; right_shoulder[1] = Point_3D[2].z; right_shoulder[2] = -Point_3D[2].y;
		neck[0] = -Point_3D[3].x; neck[1] = Point_3D[3].z; neck[2] = -Point_3D[3].y;
		left_shoulder[0] = -Point_3D[4].x; left_shoulder[1] = Point_3D[4].z; left_shoulder[2] = -Point_3D[4].y;
		left_elbow[0] = -Point_3D[5].x; left_elbow[1] = Point_3D[5].z; left_elbow[2] = -Point_3D[5].y;
		left_hand[0] = -Point_3D[6].x; left_hand[1] = Point_3D[6].z; left_hand[2] = -Point_3D[6].y;





		point_lock.unlock();

	}
}

struct udpparameter udpreceive(struct udpparameter *p)
{
	int *int_p;

	printf("Waiting for data...");
	fflush(stdout);

	//clear the arrfer by filling null, it might have previously received data
	memset(p->arr, '\0', BUFLEN);
	printf("hello\n");
	//try to receive some data, this is a blocking call
	if ((p->recv_len = recvfrom(p->s, (char*)p->arr, BUFLEN, 0, (struct sockaddr *) &p->si_other, &p->slen)) == SOCKET_ERROR)
	{
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("recv success\n");

	int_p = (int *)p->arr;

	//if (p->si_other.sin_addr.S_un.S_addr == inet_addr("192.168.1.100"))
	//	p->arr[3] = 1.0;
	//else if (p->si_other.sin_addr.S_un.S_addr == inet_addr("192.168.1.101"))
	//	p->arr[3] = 2.0;



	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(p->si_other.sin_addr), ntohs(p->si_other.sin_port));
	/*for (int i = 0; i < 4; i++)
	{
		printf("%d = %lf\n", i + 1, p->arr[i]);
	}*/
	//printf("Pi_id:%d,Point_id:%d,x:%lf,y:%lf,pi:%lf\n", int_p[0], int_p[1], p->arr[1], p->arr[2], p->arr[3]);
	printf("Pi_id:%d,Point_id:%d,x:%lf,y:%lf\n", int_p[0], int_p[1], p->arr[1], p->arr[2]);
	return *p;

}


struct udpparameter udpsocket()
{
	/*SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	double arr[100];
	WSADATA wsa;*/

	struct udpparameter p;
	p.slen = sizeof(p.si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &p.wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((p.s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	p.server.sin_family = AF_INET;
	p.server.sin_addr.s_addr = INADDR_ANY;
	p.server.sin_port = htons(SERVER_UDP_PORT);

	//Bind
	if (bind(p.s, (struct sockaddr *)&p.server, sizeof(p.server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data

	//closesocket(p.s);
	//WSACleanup();

	return p;
}
