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
	thread gl_s(glfmain, argc, argv); //thread gl_s(glfmain, argc, argv);
	//Point2d cam1_2d, cam2_2d;
	struct udpparameter p1;	
	
	p1 = udpsocket();
	//printf("struct s %d\n", p1.s);
	thread get_point_thread(&upd_thread, &p1);
	//get_point_thread.detach();

	//while (1) {
	//	
	//	
	//	//p1 = udpreceive(&p1);
	//	//if (p1.arr[3] == 1)
	//	//{
	//	//	cam1_2d.x = p1.arr[1];
	//	//	cam1_2d.y = p1.arr[2];
	//	//	
	//	//}
	//	//if (p1.arr[3] == 2)
	//	//{
	//	//	cam2_2d.x = p1.arr[1];
	//	//	cam2_2d.y = p1.arr[2];
	//	//}
	//	//cout << "recieve" << p1.arr[1] << p1.arr[2] << endl;
	//	
	//	//cout << cam1_2d << endl;
	//	//cout << cam2_2d << endl;
	//	//point_lock.lock();
	//	//cal_2d1.x = cam_2d[0][1].x;
	//	//cal_2d1.y = cam_2d[0][1].y;
	//	//cal_2d2.x = cam_2d[1][1].x;
	//	//cal_2d2.y = cam_2d[1][1].y;
	//	//point_lock.unlock();

	//	//Point_3D = calculation3D(cal_2d1, cal_2d2, RT[0], RT[1], mtx[0], mtx[1]);
	//	//printf("print 3dlocation %lf %lf %lf\n", Point_3D.x, Point_3D.y, Point_3D.z);
	//	/* neck[0] = Point_3D.x;
	//	 neck[1] = Point_3D.y;
	//	 neck[2] = Point_3D.z;
	//	 cout << neck[0] << " " << neck[1] << " " << neck[2] << endl;*/





	//	//if (coorpi1 == 1 && coorpi2 == 1)////////////////////////////////////////////////////////////////////////
	//	//{

	//		//cv_s.join();
	//		//gl_s.join();
	//	//{
	//	//printmenu();

	//	/*switch (tcpcommenu(argc, argv))
	//	{
	//	case 1: //pose
	//		tcpcommain(argc, argv);
	//		if (infopi[0] == 1 && infopi[1] == 1)
	//		{
	//			for (int j = 0; j < 2; ++j)
	//			{
	//				convertToMat(buf[i], mtx[i], dist[i], R[i], T[i]);
	//			}
	//			cout << "Info SUCCESS" << endl;
	//		}
	//		break;
	//	case 2: //tracking
	//		//udpÅ°°í
	//		//cv_s.join();
	//		//gl_s.join();
	//		break;
	//	case 3: //finish
	//		closesocket(s);
	//		WSACleanup();
	//		return 0;
	//	}*/
	//}
	gl_s.join();
	get_point_thread.join();
	return 0;
}
