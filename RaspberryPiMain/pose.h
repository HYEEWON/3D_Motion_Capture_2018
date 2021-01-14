#ifndef POS_H
#define POS_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <opencv2/opencv.hpp>

extern cv::Mat rvec, tvec;
extern cv::Mat mtx, dist;

void draw(cv::Mat & img, cv::Point2d corner, std::vector<cv::Point2d> img_pt);
void save(cv::Mat & R, cv::Mat & T, cv::Point3d pt_cam);
int posmain();
void pose(int time);

#endif
