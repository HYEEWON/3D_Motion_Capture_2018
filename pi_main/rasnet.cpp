
// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "rasnet.h"
#include "net.h"
#include "config.h"


struct udpparameter udpsock()
{
	struct udpparameter p;
	// Creating socket file descriptor 
	if ((p.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&p.servaddr, 0, sizeof(p.servaddr));

	// Filling server information 
	p.servaddr.sin_family = AF_INET;
	p.servaddr.sin_port = htons(SERVER_UDP_PORT);
	p.servaddr.sin_addr.s_addr = inet_addr(server_address);

	return p;
}

void udpsend(struct udpparameter *p, int id, int point_id, double x, double y)
{
	int *int_p;

	int_p = (int *)p->array;

	int_p[0] = id;
	int_p[1] = point_id;

//	p->array[0] = id;
	p->array[1] = x;
	p->array[2] = y;
	sendto(p->sockfd, (char*)p->array, sizeof(double) * 3,
		MSG_CONFIRM, (const struct sockaddr *) &p->servaddr,
		sizeof(p->servaddr));
//	printf("Hello message sent.\n");
}
// Driver code 
//void udpmain(double id, double x, double y)
//{
//	
//	
//	udpsock(array,servaddr,sockfd,buffer);
//	udpsend(array, servaddr, sockfd, buffer);
//	
//	/*recieve
//	int n, len;
//	//printf("Enter message : ");
//	////gets_s(message);
//	//for (int i = 0; i < 3; i++)
//	//	scanf("%lf", array + i);
//	
//	
//
//	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//		MSG_WAITALL, (struct sockaddr *) &servaddr,
//		&len);
//
//	buffer[n] = '\0';
//	for (int i = 0; i < 3; i++)
//		printf("Data: %f\n", *(array + i));
//		*/
//	close(sockfd);
//	
//}
//void udpsock(double array[MAXLINE], struct sockaddr_in servaddr, int sockfd, double buffer[MAXLINE])
//
//
//
//
//
//
