#include <iostream>
#include <math.h>
#include <thread>
#include <winsock2.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
//#include <GL/glut.h>
#include "opencv.h"
#include "opengl.h"
#include "net.h"
#include "udpnet.h"
//#include "menu.h"

using namespace std;
using namespace cv;
struct udpparameter
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	double arr[100];
	WSADATA wsa;
};
int printmenu()
{
	cout << "-----MENU-----" << endl;
	cout << "1. List" << endl;;
	cout << "2. Pose Estimation" << endl;
	cout << "3. Tracking" << endl;
	cout << "4. Stop" << endl;
	cout << ". Finish" << endl;
	return 0;
}

int main(int argc, char *argv[])
{
	Mat img;
	

	tcpcommain();
	cout << "TCP finished" << endl;

	Point2d cal_2d1, cal_2d2;

	if ((infopi[0] == 1) && infopi[1] == 1)//////////////////////////////////////////////////////////////////
	{
		for (int j = 0; j <= 1; ++j)//////////////////////////////////////////////////////////////////////////
		{
			//convertToMat(buf[j], mtx[j], dist[j], R[j], T[j], RT[j]);
		}
		cout << "Info SUCCESS" << endl;
	}

	
	/*for (int k = 0; k < 2; ++k)/////////////////////////////////////////////////////////////////////////////////
	{
		infopi[k] = 0;
	}*/
	//Point2d cam1_2d, cam2_2d	
	Point3d Point_3D;


	//thread cv_s(calculation3D, cam1_2d, cam2_2d, RT[0], RT[1], mtx[0], mtx[1]);//thread cv_s(transformation); //thread cv_s(opencvmain);
	thread gl_s(glfmain, argc, argv); 
	//Point2d cam1_2d, cam2_2d;
	struct udpparameter p1;	
	
	p1 = udpsocket();
	//printf("struct s %d\n", p1.s);
	thread get_point_thread(&upd_thread, &p1);
	gl_s.join();
	get_point_thread.join();
	return 0;
}
