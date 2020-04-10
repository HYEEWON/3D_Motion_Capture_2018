#include <iostream>
#include "opencv2/opencv.hpp"
#include "tracking.h"
#include "net.h"
#include "rasnet.h"
#define MAXLINE 1024 
using namespace std;
using namespace cv;

int range_count = 0;
int hue = 0;
int low_hue1 = 0, low_hue2 = 0;
int high_hue1 = 0, high_hue2 = 0;
Point2d * xy;
void callback(int range)
{
	//cout << "hue = " << hue << endl;

	int low_hue = hue - range;
	int high_hue = hue + range;

	if (low_hue < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = low_hue + 180;
		high_hue2 = high_hue;
		low_hue2 = 0;
	}
	else if (high_hue > 170) {
		range_count = 2;

		high_hue1 = low_hue;
		low_hue1 = 180;
		high_hue2 = high_hue - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = low_hue;
		high_hue1 = high_hue;
	}

	//cout << low_hue1 << "  " << high_hue1 << endl;
	//cout << low_hue2 << "  " << high_hue2 << endl;
}

void getObjectHistogram(Mat &frame, Rect object_region, Mat &globalHistogram, Mat &objectHistogram)
{
	const int channels[] = { 0, 1 };
	const int histSize[] = { 64, 64 };
	float range[] = { 0, 256 };
	const float *ranges[] = { range, range };

	// Histogram in object region
	Mat objectROI = frame(object_region);
	calcHist(&objectROI, 1, channels, noArray(), objectHistogram, 2, histSize, ranges, true, false);


	// A priori color distribution with cumulative histogram
	calcHist(&frame, 1, channels, noArray(), globalHistogram, 2, histSize, ranges, true, true);


	// Boosting: Divide conditional probabilities in object area by a priori probabilities of colors
	for (int y = 0; y < objectHistogram.rows; y++) {
		for (int x = 0; x < objectHistogram.cols; x++) {
			objectHistogram.at<float>(y, x) /= globalHistogram.at<float>(y, x);
		}
	}
	normalize(objectHistogram, objectHistogram, 0, 255, NORM_MINMAX);
}

void backProjection(const Mat &frame, const Mat &objectHistogram, Mat &bp) {
	const int channels[] = { 0, 1 };
	float range[] = { 0, 256 };
	const float *ranges[] = { range, range };
	calcBackProject(&frame, 2, channels, objectHistogram, bp, ranges);
}

bool searchOverlapping(Point2d *p, int index, int cur_index, int width, int height)
{

	for (int i = 0; i < index; i++)
	{
		if (i == cur_index)
			break;
		if ((p[i].x + width > p[cur_index].x && p[i].x - width < p[cur_index].x) && (p[i].y + height > p[cur_index].y && p[i].y - height < p[cur_index].y))
		{
			return true;
		}
		//if (p[i] == p[cur_index])
		   //return true;
	}
	return false;


}

//int trackingmain()
//{
//	Mat img;
//	int numOfLables = 0;
//
//	Scalar red(0, 0, 255);
//	Scalar blue(255, 0, 0);
//	Scalar yellow(0, 255, 255);
//	Scalar magenta(255, 0, 255);
//
//	Mat rgb_color = Mat(1, 1, CV_8UC3, Scalar(140, 77, 0));
//	Mat hsv_color;
//
//	clock_t sTime, cTime;
//
//	cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);
//	hue = (int)hsv_color.at<Vec3b>(0, 0)[0];
//
//	int rangeH = 10;
//	int LowS = 130;
//	int LowV = 50;
//
//	callback(rangeH);
//
//	cvNamedWindow("pivideo", 1);
//	cvCreateTrackbar("rangeH(0~90)", "pivideo", &rangeH, 179, callback); //Hue (0 - 179)
//	cvCreateTrackbar("LowS(0~255)", "pivideo", &LowS, 255); //Saturation (0 - 255)
//	cvCreateTrackbar("LowV(0~255)", "pivideo", &LowV, 255); //Value (0 - 255)
//
//	VideoCapture cap;
//	Mat img_hsv;
//
//	cap.open(-1);
//	// OR advance usage: select any API backend
//	int deviceID = 0;             // 0 = open default camera
//	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
//				  // open selected camera using selected API
//	cap.open(deviceID + apiID);
//
//	if (!cap.isOpened()) {
//		cerr << "ERROR! Unable to open camera\n";
//		cin.get();
//		return -1;
//	}
//
//	Rect trackingWindow(0, 0, 30, 30);
//	int frame_index = 0;
//
//	Mat globalHistogram;
//
//	Mat * objectHistogram;
//	Rect *prev_rect;
//	int udpcheck = 1;
//	struct udpparameter p1;
//	for (;;)
//	{
//		// wait for a new frame from camera and store it into 'frame'
//		cap.read(img);
//
//		// check if we succeeded
//		if (img.empty()) {
//			cerr << "ERROR! blank frame grabbed\n";
//			break;
//		}
//
//		frame_index++;
//
//		//transform to HSV
//		cvtColor(img, img_hsv, COLOR_BGR2HSV);
//
//		//use HSV and binarize video 
//		Mat img_mask1, img_mask2;
//		inRange(img_hsv, Scalar(low_hue1, LowS, LowV), Scalar(high_hue1, 255, 255), img_mask1);
//		if (range_count == 2) {
//			inRange(img_hsv, Scalar(low_hue2, LowS, LowV), Scalar(high_hue2, 255, 255), img_mask2);
//			img_mask1 |= img_mask2;
//		}
//
//		//morphological opening, eliminate dots
//		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		//morphological closing
//		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		if (frame_index < 50) {////////////////////////////////////////////////////////////////////////////
//			Mat img_labels, stats, centroids;
//			numOfLables = connectedComponentsWithStats(img_mask1, img_labels, stats, centroids, 8, CV_32S);
//			int max = -1, idx = 0;
//			if (numOfLables == 1)
//			{
//				frame_index = 0;
//			}
//			objectHistogram = new Mat[numOfLables];
//			Rect *object_region = new Rect[numOfLables];
//			prev_rect = new Rect[numOfLables];
//			for (int i = 1; i < numOfLables; i++)
//			{
//				int left = stats.at<int>(i, CC_STAT_LEFT);
//				int top = stats.at<int>(i, CC_STAT_TOP);
//				int width = stats.at<int>(i, CC_STAT_WIDTH);
//				int height = stats.at<int>(i, CC_STAT_HEIGHT);
//				//add 
//				object_region[i - 1] = Rect(left, top, width, height);
//				getObjectHistogram(img_hsv, object_region[i - 1], globalHistogram, objectHistogram[i - 1]);
//				trackingWindow = object_region[i - 1];
//
//				rectangle(img, Point(left, top), Point(left + width, top + height),
//					Scalar(0, 0, 255), 1);
//
//				prev_rect[i - 1] = object_region[i - 1];
//			}
//		}
//		else {
//			xy = new Point2d[numOfLables];
//			for (int i = 1; i < numOfLables; i++)
//			{
//				Mat bp;
//				char number[3];
//				backProjection(img_hsv, objectHistogram[i - 1], bp);
//				bitwise_and(bp, img_mask1, bp);
//
//				RotatedRect rect = CamShift(bp, prev_rect[i - 1], cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 1));
//
//				sprintf(number, "%d", i);
//				xy[i] = rect.center;
//				
//				if (udpcheck == 1)
//				{
//					p1 = udpsock();
//					udpcheck++;
//				}
//				udpsend(&p1, pi_id, i, xy[i].x, xy[i].y);
//
//				
//				printf("id = %lf, x = %lf, y =%lf\n",(double)(i),xy[i].x,xy[i].y);
//				if (searchOverlapping(xy, numOfLables, i, rect.size.width, rect.size.height))
//				{
//					cout << "Tracking overlapping!!!" << endl;//////////////////////////////////
//					continue;
//				}
//				putText(img, number, rect.center, 2, 1.2, Scalar::all(255));
//				if (rect.size.width * rect.size.height > 0)
//				{
//					ellipse(img, rect, CV_RGB(255, 0, 0), 3, CV_AA);
//				}
//			}
//		}
//		Mat img_result;
//		cvtColor(img_mask1, img_mask1, COLOR_GRAY2BGR);
//		hconcat(img, img_mask1, img_result);
//
//		imshow("pivideo", img_result);
//
//		if (waitKey(5) >= 0)
//			break;
//	}
//	return 0;
//}



int trackingmain()
{
	Mat img;
	int numOfLables = 0;
	Point2d * xy;
	int udpcheck = 1;
	struct udpparameter p1;

	Scalar red(0, 0, 255);
	Scalar blue(255, 0, 0);
	Scalar yellow(0, 255, 255);
	Scalar magenta(255, 0, 255);


	Mat rgb_color = Mat(1, 1, CV_8UC3, Scalar(88, 35, 0));
	Mat hsv_color;

	cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);


	hue = (int)hsv_color.at<Vec3b>(0, 0)[0];


	int rangeH = 10;
	int LowS = 130;
	int LowV = 50;

	callback(rangeH);


	cvNamedWindow("Result Video", 1);
	cvCreateTrackbar("rangeH(0~90)", "Result Video", &rangeH, 179, callback); //Hue (0 - 179)
	cvCreateTrackbar("LowS(0~255)", "Result Video", &LowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("LowV(0~255)", "Result Video", &LowV, 255); //Value (0 - 255)


	VideoCapture cap;
	Mat img_frame, img_hsv;

	cap.open(0);


	Rect trackingWindow(0, 0, 30, 30);
	int frame_index = 0;

	Mat globalHistogram;

	Mat * objectHistogram;
	Rect *prev_rect;


	for (;;)
	{

		cap.read(img_frame);


		if (img_frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}



		frame_index++;


		cvtColor(img_frame, img_hsv, COLOR_BGR2HSV);



		Mat img_mask1, img_mask2;
		inRange(img_hsv, Scalar(low_hue1, LowS, LowV), Scalar(high_hue1, 255, 255), img_mask1);
		if (range_count == 2) {
			inRange(img_hsv, Scalar(low_hue2, LowS, LowV), Scalar(high_hue2, 255, 255), img_mask2);
			img_mask1 |= img_mask2;
		}


		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));



		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));



		if (frame_index < 50) {
			Mat img_labels, stats, centroids;
			numOfLables = connectedComponentsWithStats(img_mask1, img_labels, stats, centroids, 8, CV_32S);
			if (numOfLables == 1)
			{
				frame_index = 0;
			}

			objectHistogram = new Mat[numOfLables];
			Rect *object_region = new Rect[numOfLables];
			prev_rect = new Rect[numOfLables];
			for (int i = 1; i < numOfLables; i++)
			{
				int left = stats.at<int>(i, CC_STAT_LEFT);
				int top = stats.at<int>(i, CC_STAT_TOP);
				int width = stats.at<int>(i, CC_STAT_WIDTH);
				int height = stats.at<int>(i, CC_STAT_HEIGHT);
				object_region[i - 1] = Rect(left, top, width, height);
				getObjectHistogram(img_hsv, object_region[i - 1], globalHistogram, objectHistogram[i - 1]);
				trackingWindow = object_region[i - 1];


				rectangle(img_frame, Point(left, top), Point(left + width, top + height),
						Scalar(0, 0, 255), 1);

				prev_rect[i - 1] = object_region[i - 1];
			}
		}
		else {

			// Tracking
			//meanShift(bp, trackingWindow, TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 0.01));
			//rectangle(img_frame, trackingWindow, CV_RGB(0, 0, 255), 2);
			RotatedRect *ar_Rect= new RotatedRect[numOfLables];
			xy = new Point2d[numOfLables];
			for (int i = 1; i < numOfLables; i++)
			{
				Mat bp;
				char number[3];
				backProjection(img_hsv, objectHistogram[i - 1], bp);
				bitwise_and(bp, img_mask1, bp);

				RotatedRect rect = CamShift(bp, prev_rect[i - 1], cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 1));
				ar_Rect[i] = rect;
				Rect t_rect = rect.boundingRect();
				sprintf(number,"%d", i);
				xy[i] = rect.center;

				
				printf("id = %lf, x = %lf, y =%lf\n",(double)(i),xy[i].x,xy[i].y);

				if (searchOverlapping(xy, numOfLables, i, t_rect.width, t_rect.height))
				{
					Mat tmp_Mask;
					img_mask1.copyTo(tmp_Mask);
					for (int j = 1; j < i; j++)
					{
						ellipse(tmp_Mask, ar_Rect[j], Scalar(0, 0, 0), 10);
						ellipse(tmp_Mask, ar_Rect[j], Scalar(0, 0, 0), -1);
					}
					bitwise_and(bp, tmp_Mask, bp);
					RotatedRect temp = CamShift(bp, prev_rect[i - 1], cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 1));
					t_rect = rect.boundingRect();
					xy[i] = temp.center;

				}
				else {
					putText(img_frame, number, rect.center, 2, 1.2, Scalar::all(255));
					if (rect.size.width * rect.size.height > 0)
					{

						ellipse(img_frame, rect, CV_RGB(255, 0, 0), 3, CV_AA);
					}
				}
				if (udpcheck == 1)
				{
					p1 = udpsock();
					udpcheck++;
				}
				udpsend(&p1, pi_id, i, xy[i].x, xy[i].y);

			}


		}


		Mat img_result;

		cvtColor(img_mask1, img_mask1, COLOR_GRAY2BGR);
		hconcat(img_frame, img_mask1, img_result);

		imshow("Result Video", img_result);


		if (waitKey(5) >= 0)
			break;


	}
	return 0;
}

