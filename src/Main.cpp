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
#include "LineControllerBasic.h"
#include <opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/contrib/detection_based_tracker.hpp>
//
//


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
	LineController * lController = new LineControllerBasic();
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
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 160);
	//for all frames
	double detected = 0;
	double undetected = 0;
	double moy = 0.0;
	double error = 0.0;
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
		clock_t tStart = clock();
		d.detectAndMark(frame, logo_cascade, logos);
		if(logos.size() == 1)
			detected ++;
		else
			undetected ++;
		error = detected/(detected+undetected);
		moy += (double)(clock() - tStart)/CLOCKS_PER_SEC;
		printf("Moyenne: %fs",moy/(detected+undetected));
		printf("\terror: %f \%\n", 100-error*100);
		//		DetectionBasedTracker::Parameters param;
		//
		//		//param.maxObjectSize = 400;
		//		param.maxTrackLifetime = 20;
		//		param.minDetectionPeriod = 7;
		//		DetectionBasedTracker obj = DetectionBasedTracker(fn, param) //("/home/vivien/CT/xml/cascade.xml",param);
		//				obj.run();
		//		obj.process(im_gray);
		//		vector< Rect_<int> > faces;
		//		obj.getObjects(faces);
		//		for(int i = 0; i < faces.size(); i++){
		//			Rect face_i = faces[i];
		//			// Make a rectangle around the detected object
		//			rectangle(frame, face_i, CV_RGB(0, 255,0), 3);
		//
		//		}
		//		param.minNeighbors = 3;
		//		param.minObjectSize = 20;
		//		param.scaleFactor = 1.1;

		std::map<int,Tracker> trackers = controller->getTrackers();
		cmt = new CMT();
		//dont track an object that has not been detected for a long time
		cout<<"deleting.."<<endl;
		controller->deleteUselessTrackers(logos);
		cout<<"deleted"<<endl;
		//create and update tracker using detected object
		controller->processTrackers(im_gray, logos, cmt);
		cout<<"processed"<<endl;
		//display trackers
		controller->displayTrackers(frame);
		cout<<"displayed"<<endl;
		//show frame
		cout<<controller->getTrackers().size()<<endl;
		imshow(window_name, frame);
		waitKey(0);
	}
	return 0;
}

