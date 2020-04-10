#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <cstring>
#include "pos.h"
#include "tracking.h"
#include "net.h"
#include "rasnet.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	if(argc == 2) {
		strcpy(server_address, argv[1]);
	}
	cout << "Server IP; " << server_address << endl;

	// pose estimation
	posmain();
	cout << "Pose Estimation Success" << endl;

	// send camera infomation
	if(!tcpmain()) {
		return 0;
	}

	while (1)
	{
		//tcpmenu(argc, argv);
		trackingmain();
	
		//udpmain(argc, argv);
	}

	//tcpmain(argc, argv);
	//tcpmenu(argc, argv);
	return 0;
}

