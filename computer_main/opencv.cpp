#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <math.h>
#include "opencv.h"
#include "opengl.h"

using namespace std;
using namespace cv;

//Point3d cam1, cam2;

//Mat pt3D(3, 1, CV_64FC1);

cv::Mat mtx[MAX_CLIENT], dist[MAX_CLIENT], R[MAX_CLIENT], T[MAX_CLIENT], RT[MAX_CLIENT];


Point3d calculation3D(const Point2d & Point1_2D, const Point2d& Point2_2D, const Mat & cam1_RT, const Mat & cam2_RT, const Mat & mtx1, const Mat & mtx2, cv::Point3d &outPoint)
{
	//Point3d Point_3D;
	Mat pt3D(4, 1, CV_64FC1);
	Mat Pt1 = mtx1 * cam1_RT;
	Mat Pt2 = mtx2 * cam2_RT;

	Mat P1(Point1_2D);
	Mat P2(Point2_2D);

	//cout << "rt1" << cam1_RT << endl;
	//cout << "rt2" << cam2_RT << endl;
	//cout << "mtx1" << mtx1 << endl;
	//cout << "mtx2" << mtx2 << endl;
	//cout << "pt1" << Pt1 << endl;
	//cout << "pt2" << Pt2 << endl;
	//cout << P1 << endl;
	triangulatePoints(Pt1, Pt2, P1, P2, pt3D);
	//cout << pt3D << endl;

	double w = pt3D.at<double>(3);
	outPoint.x =pt3D.at<double>(0)/ w;
	outPoint.y =pt3D.at<double>(1)/ w;
	outPoint.z =pt3D.at<double>(2)/ w;

	//cout<<"x"<<neck[0]<<endl;
	//cout<<"y"<<neck[1]<<endl;
	//cout<<"z"<<neck[2]<<endl;
	//cout << "pt3D.at<double>(0, 0)" << pt3D.at<double>(0, 0)/w << endl;/// w);
	//cout << "pt3D.at<double>(1, 0)" << pt3D.at<double>(1, 0)/w<<endl;/// w);
	//cout << "pt3D.at<double>(2, 0)" << pt3D.at<double>(2, 0)/w<<endl;
	//cout << "w" << w << endl;
	//cout << "function"<<Point_3D.x << endl;
	//cout << "Point_3D" << endl;
	//cout << outPoint << endl;

	return outPoint;
}
//Point3d * calculation3D(int num, const Point2d * Point1_2D, const Point2d * Point2_2D, const Mat & cam1_RT, const Mat & cam2_RT, const Mat & mtx1, const Mat & mtx2, static Point3d * Point_3D)
//{
//	Point_3D = new Point3d[num];
//	Mat pt3D(3, 1, CV_64FC1);
//	Mat Pt1 = mtx1 * cam1_RT;
//	Mat Pt2 = mtx2 * cam2_RT;
//	for (int i = 0; i < num; i++)
//	{
//		Mat P1(Point1_2D[i]);
//		Mat P2(Point2_2D[i]);
//		triangulatePoints(Pt1, Pt2, P1, P2, pt3D);
//
//		double w = pt3D.at<double>(3, 0);
//		Point_3D[i].x = pt3D.at<double>(0, 0) / w;
//		Point_3D[i].y = pt3D.at<double>(1, 0) / w;
//		Point_3D[i].z = pt3D.at<double>(2, 0) / w;
//	}
//	return Point_3D;
//}

void printCameraMatrix(int id)
{
	cout << "mtx" << endl << mtx[id] << endl;
	cout << "dist" << endl << dist[id] << endl;
	cout << "R" << endl << R[id] << endl;
	cout << "Y" << endl << T[id] << endl;
	cout << "RT" << endl << RT[id] << endl;
}


int convertToMat(char * buff, Mat & mtx, Mat & dist, Mat& R, Mat& T, Mat& RT)
{
	int * p_i = (int *)buff;
	double *p_d = (double *)(p_i + 1);

	if (p_i[0] != 1)
	{
		cout << "I don't want this buffer" << endl;
		return -1;
	}

	mtx = Mat(3, 3, CV_64FC1);
	dist = Mat(5, 1, CV_64FC1);
	R = Mat(3, 1, CV_64FC1);
	T = Mat(3, 1, CV_64FC1);

	for (int i = 0; i < 9; i++)
	{
		mtx.at<double>(i / 3, i % 3) = p_d[i];
		if (i < 5)
			dist.at<double>(i) = p_d[i + 9];
		if (i < 3)
		{
			R.at<double>(i) = p_d[i + 14];
			T.at<double>(i) = p_d[i + 17];
		}
	}
	Mat Rd;
	int a = 0;
	int b = 0;
	Rodrigues(R, Rd);

	//cout << mtx << endl << endl;
	//cout << dist << endl << endl;
	//cout << R << endl << endl;
	//cout << T << endl << endl;
	//cout << Rd << endl << endl;

	double * _R = (double *)Rd.data;
	double * _T = (double *)T.data;
	RT = Mat(3, 4, CV_64FC1);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			if (j == 3)
				RT.at<double>(i, j) = _T[i];
			else 
				RT.at<double>(i, j) = _R[i * 3 + j];

	//cout << RT << endl << endl;
}