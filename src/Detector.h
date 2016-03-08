/*
 * Detector.h
 *
 *  Created on: 12 févr. 2016
 *      Author: vivien
 */

#ifndef SRC_DETECTOR_H_
#define SRC_DETECTOR_H_

#include <opencv.hpp>

using namespace std;
using namespace cv;

class Detector {
public:
	Detector();
	Detector(string& s);
	virtual ~Detector();
	void detectAndMark(Mat &frame, CascadeClassifier& logo_cascade, vector<Rect>& logos);
private:
	String window_name;
	void displayObject(vector<Rect> & logos, Mat &frame);
};

#endif /* SRC_DETECTOR_H_ */
