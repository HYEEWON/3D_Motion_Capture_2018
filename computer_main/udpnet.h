#ifndef UDPNET_H
#define UDPNET_H

#pragma once
#include<stdio.h>
#include<winsock2.h>
#include <opencv2/opencv.hpp>
#include <mutex>
#include "config.h"
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

extern cv::Point2d cam_2d[MAX_CLIENT][MAX_POINT];
extern std::mutex point_lock;

struct udpparameter udpsocket();
struct udpparameter udpreceive(struct udpparameter *p);
void upd_thread(struct udpparameter *p);

#endif