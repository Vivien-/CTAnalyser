/*
 * Detector.cpp
 *
 *  Created on: 12 févr. 2016
 *      Author: vivien
 */

#include "Detector.h"

Detector::Detector() {}

Detector::Detector(string& s): window_name(s) {}

Detector::~Detector() {}

void Detector::detectAndMark(Mat& frame, CascadeClassifier& logo_cascade, vector<Rect>& logos ) {
	Mat frame_gray;
	Mat ROI;
	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	//equalizeHist( frame_gray, frame_gray );
	//-- Detect logos
	logo_cascade.detectMultiScale( frame_gray, logos, 1.2, 1, 0|CASCADE_SCALE_IMAGE);
	displayObject(logos, frame);
}

void Detector::displayObject(vector<Rect> & logos, Mat &frame) {
	for(uint i = 0; i<logos.size(); i++){
		Point center( logos[i].x + logos[i].width/2, logos[i].y + logos[i].height/2 );
		circle(frame, center, 5, Scalar( 255, 0, 255 ), 4, 8, 0);
	}
}
