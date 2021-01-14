#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "pose.h"
#include "tcpnetwork.h"
#include "config.h"


int s, new_socket;
struct sockaddr_in server, client;
char mes[10000];
char hostname[100];

int pi_id;

char server_address[16] = SERVER_IP_DEFAULT;

void print_camera()
{
	std::cout << "Camera Matrix" << std::endl;
	std::cout << mtx << std::endl;
	std::cout << "Dist Coeffi" << std::endl;
	std::cout << dist << std::endl;
	std::cout << "R vector" << std::endl;
	std::cout << rvec << std::endl;
	std::cout << "T vector" << std::endl;
	std::cout << tvec << std::endl;

}

void print_packet(char buf[])
{
	int i;
	int *intp;
	double *doublep;
	int result;
	
	intp = (int *)buf;
	doublep = (double *)(buf + 4);

	for(i = 0; i < 20; i++) {
		printf("%lf\t", doublep[i]);
	}
	printf("\n");
}

int read_camera_file(char *filename)
{
	/*
1:camera matrix, co effi, 2: rvec, tvect
	 */
	cv::FileStorage fs(CAMERA_INFO_FILENAME, cv::FileStorage::READ);
	fs["camera_matrix"] >> mtx;
	fs["distortion_coefficients"] >> dist;

	return true;
}

int tcpmain()
{
	int c;
	int result;

	read_camera_file(CAMERA_INFO_FILENAME);

	print_camera();

	if(gethostname(hostname, 99) == -1)
	{
		perror("gethostname");
		return false;
	}
	
	printf("Hostname: %s\n", hostname);
	// Set Pi ID
	pi_id = hostname[17] - '0' - 1;
	printf("Pi ID: %d\n", pi_id);

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("socket");
		return false;
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	inet_pton(AF_INET, server_address, &server.sin_addr);
	//server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);

	c = sizeof(struct sockaddr_in);
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("connect");
		return false;
	}
	puts("Connection accepted");


	// Send Hostname to identify
	result = send(s, hostname, strlen(hostname) + 1, 0);
	if(result <= 0) {
		printf("Cannot send hostname\n");
		close(s);
		return false;
	}

	// Receive reply
	result = recv(s, mes, 10000, 0);
	if(result <= 0) {
		printf("Cannot recv\n");
		close(s);
		return false;
	}
	printf("Reply: %s\n", mes);

	//Reply to client

	char buf[10000];
	int *p = (int*)buf;
	*p = 1;

	double *p2 = (double *)(p + 1);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			*p2 = mtx.at<double>(i, j);
			p2 += 1;
		}
	}

	double *dp;
	dp = (double*)(p + 19);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 1; ++j)
		{
			*dp = dist.at<double>(i, j);
			dp += 1;
		}
	}
//	double r[3] = { 1.5, 2.3, 6.7 };
//	double t[3] = { 3.4, 7.8, 9.0 };
	for (int i = 0; i < 3; ++i)
	{
		//*dp = r[i];
		*dp = rvec.at<double>(i);
		dp += 1;
	}

	for (int j = 0; j < 3; ++j)
	{
		//*dp = t[j];
		*dp = tvec.at<double>(j);
		dp += 1;
	}

	print_packet(buf);

	int numbuf = 0, num;
	while (numbuf < 164) {
		num = send(s, buf + numbuf, 164 - numbuf, 0);
		numbuf += num;
	}
//	for (int y = 0; y <= 163; y++)
//	{
//		printf("%d\n", buf[y]);
//	}

//	close(s);

	return true;
}

int tcpmenu()
{
	int size;

	size = recv(s, mes, 10000, 0);

	if (size <= 0) {
		perror("recv");
		return false;
	}
	printf("%s\n", mes);


	//closesocket(s);
	//WSACleanup();
	return 0;
}
