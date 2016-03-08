/*
 * Tracker.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "Tracker.h"

Tracker::Tracker() {
	// TODO Auto-generated constructor stub
}

Tracker::~Tracker() {
	std::cout<<"~Tracker()"<<std::endl;
	// TODO Auto-generated destructor stub
}

Tracker::Tracker(cmt::CMT* cmt_, int id_, int lf): cmt(cmt_){
	id = id_;
	i = cmt->bb_rot.center;
	c = cmt->bb_rot.center;
	lifetime = lf;
	counterId = -1;
}


Point Tracker::current() const{
	return c;
}


Point Tracker::initial() const{
	return i;
}

void Tracker::setInitial(Point& p){
	i = p;
}

void Tracker::setCurrent(Point& p){
	c = p;
}

int Tracker::getId() const{
	return id;
}

int Tracker::getLifetime() const{
	return lifetime;
}

void Tracker::setLifetime(int l){
	lifetime = l;
}

void Tracker::setCounterId(int id) {
	counterId = id;
}

int Tracker::getCounterId() const{
	return counterId;
}

void Tracker::display(Mat& frame) {
	//Visualize the output. It is ok to draw on im itself, as CMT only uses the grayscale image
	for(size_t i = 0; i < cmt->points_active.size(); i++)
		circle(frame, cmt->points_active[i], 2, Scalar(255,0,0));

	Point2f vertices[4];
	cmt->bb_rot.points(vertices);
	for (int i = 0; i < 4; i++)
		line(frame, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
}

void Tracker::processCMT(Mat& gray_frame) {
	cmt->processFrame(gray_frame);
	Point p(cmt->bb_rot.center.x, cmt->bb_rot.center.y);
	setCurrent(p);

}

CMT* Tracker::getCMT() {
	return cmt;
}
