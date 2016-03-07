/*
 * Main.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: mohammed
 */

#include "Line.h"
#include "Counter.h"
#include "Tracker.h"
#include "CppMT/CMT.h"
#include "Controller.h"
#include "Detector.h"
#include "LineController.h"
#include <opencv.hpp>
#include "opencv2/objdetect/detection_based_tracker.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
using cmt::CMT;

void usage(const char *s){
	cerr<<"Usage: "<<s<<" xml"<<endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
	VideoCapture capture;
	CMT* cmt;
	Mat frame;
	string window_name = "window";
	CascadeClassifier logo_cascade;
	const int LIFETIME = 10;
	LineController * lController = new LineController();
	Controller *controller = new Controller(LIFETIME);
	lController->setController(controller);
	Detector d(window_name);
	// check number of arguments
	if(argc < 2) {
		usage(argv[0]);
		return 0;
	}
	// create window
	namedWindow(window_name);
	//load xml
	if(!logo_cascade.load(argv[1]) ){ printf("--(!)Error loading face cascade\n"); return -1; };
	//get video
	capture.open( -1 );
	if (!capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

	/**
	 * Uncomment for lighter execution time
	 */
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 160);

	//for all frames
	while(capture.read(frame)){
		//check that camera is working
		if( frame.empty() ){
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		Mat im_gray;
		if (frame.channels() > 1) {
			cvtColor(frame, im_gray, CV_BGR2GRAY);
		} else {
			im_gray = frame;
		}
		//allow us to draw/display/delete lines on the frame
		lController->process(window_name, frame);

		//detect the trained object
		vector<Rect> logos;
		d.detectAndMark(frame, logo_cascade, logos);

		std::map<int,Tracker> trackers = controller->getTrackers();
		cmt = new CMT();

		//dont track an object that has not been detected for a long time
		cout<<"Identifying and deleting useless trackers..."<<endl;
		controller->deleteUselessTrackers(logos);
		cout<<"Useless trackers deleted"<<endl;

		//create and update tracker using detected object
		controller->processTrackers(im_gray, logos, cmt);
		cout<<"Trackers processed"<<endl;

		//display trackers
		controller->displayTrackers(frame);
		imshow(window_name, frame);
		cout<<"Trackers displayed"<<endl;
		cout<<"Number of trackers: "<<controller->getTrackers().size()<<endl;

		//compute number of tracker entering and leaving a line
		int entered = controller->getEntered(0);
		int left = controller->getLeft(0);

		waitKey(0);
	}
	return 0;
}

