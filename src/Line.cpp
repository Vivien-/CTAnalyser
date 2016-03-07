/*
 * Line.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "Line.h"

Line::Line() {
	// TODO Auto-generated constructor stub

}

Line::Line(cv::Point a, cv::Point b, int id_) {
	X = a;
	Y = b;
	id = id_;
}

Line::~Line() {
	// TODO Auto-generated destructor stub
}

cv::Point Line::getX(void) const{
	return X;
}

cv::Point Line::getY(void) const{
	return Y;
}

int Line::getId(void) const{
	return id;
}

void Line::setX(int _x, int _y){
	X.x = _x; X.y = _y;
}

void Line::setY(int _x, int _y){
	Y.x = _x; Y.y = _y;
}
