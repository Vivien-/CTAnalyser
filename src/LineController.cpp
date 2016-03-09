/*
 * LineController.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "LineController.h"
#include "patch.hpp"

using namespace std;
using namespace cv;

/**
 * Delete line by changing corresponding check button state
 * TODO Would probably throw a segfault, some work is needed here
 */
void buttonCallback(int , void *instance){
	LineController * that = static_cast<LineController*>(instance);
	int id = that->getNextId() - 2;
	int position = that->getLinePositionById(id);
	that->removeLine(id);
	that->controller->removeCounter(id);
	that->destroy_window = true;
}

/**
 * Create a line every two clicks on the image screen
 * TODO Might throw a segfault as well, some work is probably needed here
 */
void mouseCallback(int event, int _x, int _y, int , void* instance){
	LineController * that = static_cast<LineController*>(instance);
	int* click_nb = &(that->click_nb);
	Point* X = &(that->X);
	Point* Y = &(that->Y);
	if(event == EVENT_LBUTTONDOWN ){
		if((*click_nb)%2 == 0) {
			X->x = _x;
			X->y = _y;
		} else {
			Y->y = _y;
			Y->x = _x;
			Line l = that->addLine();
			Counter counter(l, l.getId());
			that->controller->addLine(l);
			int this_id = that->getNextId() - 1;
			that->buttons_id[that->new_button] = this_id;
			createButton(patch::to_string(this_id), buttonCallback, that, CV_CHECKBOX);
		}
		(*click_nb)++;
	}
}

LineController::LineController() {
	next_id = 1;
	destroy_window = false;
	destroyed = false;
	click_nb = 0;
}

LineController::~LineController() {
	// TODO Auto-generated destructor stub
}

Line LineController::addLine() {
	Line l = Line(X, Y, next_id);
	lines.push_back(l);
	next_id++;
	return l;
}

void LineController::removeLine(int id){
	int i;
	if((i = getLinePositionById(id)) != -1){
		lines.erase(lines.begin()+i);
		cout<<"Removed line "<<id<<" at position "<<i<<endl;
	} else
		cout<<"Line "<<id<<" not in lines collection"<<endl;
}

//as lines is sorted we can do a log search TODO
int LineController::getLinePositionById(int id) {
	for(uint i = 0; i< lines.size(); i++)
		if(lines[i].getId() == id)
			return i;
	return -1;
}

void LineController::draw(){
	for(uint i = 0; i < lines.size(); i++) {
		double a = 255 - i * (255 / lines.size());
		drawLine(lines[i], Scalar(a,a,0));
	}
}

void LineController::setBackground (const Mat & m){
	currentBackground = m;
}

vector<Line>& LineController::getLines(void) {
	return lines;
}

Mat& LineController::frame() {
	return currentBackground;
}

int LineController::getNextId() {
	return next_id;
}

void LineController::drawLine(const Line &l, Scalar color){
	line(currentBackground, l.getX(), l.getY(), color, 2, 8);
}


void LineController::process(std::string windowName, Mat & frame){
	setMouseCallback(windowName, mouseCallback, this);
	if(destroy_window) {
		destroy_window = false;
		destroyed = true;
		destroyWindow(windowName);
	}

	setBackground(frame);
	draw();
//	imshow(windowName, frame);
	if(destroyed) {
		for(uint i = 0; i < lines.size(); ++i){
			int this_id = lines[i].getId();
			buttons_id[new_button] = this_id;
			createButton(patch::to_string(this_id), buttonCallback, &buttons_id[new_button++], CV_CHECKBOX);
		}
		setMouseCallback(windowName, mouseCallback, NULL);
		destroyed = false;
//		waitKey(0);
	}
}


void LineController::setController(Controller *c){
	controller = c;
}

