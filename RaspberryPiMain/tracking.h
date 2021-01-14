#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "tcpnetwork.h"
#include "udpnetwork.h"

#define PORT     8888 
#define MAXLINE 1024 
using namespace std;
using namespace cv;


extern cv::Point2d * xy;

void callback(int range);
void getObjectHistogram(cv::Mat &frame, cv::Rect object_region, cv::Mat &globalHistogram, cv::Mat &objectHistogram);
void backProjection(const cv::Mat &frame, const cv::Mat &objectHistogram, cv::Mat &bp);
bool searchOverlapping(cv::Point2d *p, int index, int cur_index);
int trackingmain();
#endif
