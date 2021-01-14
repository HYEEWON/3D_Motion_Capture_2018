#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include "pose.h"
#include "config.h"

using namespace cv;
using namespace std;

Mat rvec, tvec;
Mat mtx, dist;

char camera_filename[] = "out_camera_data.xml";

void draw(Mat & img, Point2d corner, vector<Point2d> img_pt)
{
	line(img, corner, img_pt[1], Scalar(255, 0, 0), 5); 
	line(img, corner, img_pt[2], Scalar(0, 255, 0), 5);
	line(img, corner, img_pt[3], Scalar(0, 0, 255), 5);
}

void save(Mat & R, Mat & T, Point3d pt_cam)
{
	FileStorage fs(CAMERA_OUT_FILENAME, FileStorage::WRITE);
	fs << "R" << R;
	fs << "T" << T;
	fs << "Cam_location" << pt_cam;
	int a = 0;
	int b = 0;
	double * _R = (double *)R.data;
	double * _T = (double *)T.data;
	vector<double> Rt;
	for (int i = 0; i < 12; i++)
	{
		if (i % 4 == 3)
			Rt.push_back(_T[a++]);
		else
			Rt.push_back(_R[b++]);
	}

	Mat temp(Rt);
	Mat RT = temp.reshape(1, 3);
	fs << "RT" << RT;
}

void pose(int time)
{
	Mat img;
	Point3d Cam_pose;
	bool found = false;
	Size boardSize;
	double sizes;
	FileStorage fs(CAMERA_INFO_FILENAME, FileStorage::READ);
	fs["camera_matrix"] >> mtx;
	fs["distortion_coefficients"] >> dist;
	fs["board_width"] >> boardSize.width;
	fs["board_height"] >> boardSize.height;
	fs["square_size"] >> sizes;

	double start = 0, end = 0;
	bool first = true;
	vector<Point2d> imagePoints;
	vector<Point2d> image_Points;
	vector<Point3d> objectPoints;
	vector<Point3d> axis = { Point3d(0.0,0.0,0.0),Point3d(3.0 * sizes,0.0,0.0),Point3d(0.0,3.0 * sizes,0.0),Point3d(0.0,0.0,3.0 * sizes) };


	Size Reverse(boardSize.height, boardSize.width);

	VideoCapture cap(0);
	Mat gray;

	Mat R, R_inv, P;
	Mat RT;
	double *p;
	for (int i = 0; i < boardSize.width; i++)
		for (int j = 0; j < boardSize.height; j++)
			objectPoints.push_back(Point3d(double(i) *sizes, double(j) *sizes, 0.0));
	while (end - start < time)
	{
		if (found)
		{
			end = clock();
		}
		else
			first = true;
		cap.read(img);
		cvtColor(img, gray, COLOR_BGR2GRAY);
		found = findChessboardCorners(gray, Reverse, imagePoints, CALIB_CB_FAST_CHECK);

		if (found)
		{
			if (first)
			{
				start = clock();
				first = false;
			}


			solvePnP(objectPoints, imagePoints, mtx, dist, rvec, tvec, false);
			projectPoints(axis, rvec, tvec, mtx, dist, image_Points);

			draw(img, image_Points[0], image_Points);
			Rodrigues(rvec, R);
			R_inv = R.inv();
			P = -R_inv * tvec;
			p = (double *)P.data;
			Cam_pose = Point3d(p[0], p[1], p[2]);

			save(R, tvec, Cam_pose);
		}
		imshow("test", img);
		if (waitKey(10) == 27) break;

	}
}

int posmain()
{
	pose(5000);
	return 1;
}
