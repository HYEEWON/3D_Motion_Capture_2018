#ifndef OPENCV_H
#define OPENCV_H
//#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "config.h"

//extern cv::Mat pt3D;

extern cv::Mat mtx[MAX_CLIENT], dist[MAX_CLIENT], R[MAX_CLIENT], T[MAX_CLIENT], RT[MAX_CLIENT];

cv::Point3d calculation3D(const cv::Point2d & Point1_2D, const cv::Point2d& Point2_2D, const cv::Mat & cam1_RT, const cv::Mat & cam2_RT, const cv::Mat & mtx1, const cv::Mat & mtx2, cv::Point3d &outPoint);
//cv::Point3d * calculation3D(int num, const Point2d * Point1_2D, const Point2d * Point2_2D, const Mat & cam1_RT, const Mat & cam2_RT, const Mat & mtx1, const Mat & mtx2, static Point3d * Point_3D);

void printCameraMatrix(int id);
int convertToMat(char * buff, cv::Mat & mtx, cv::Mat & dist, cv::Mat& R, cv::Mat& T,cv::Mat& RT);
#endif
