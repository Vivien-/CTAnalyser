/*
 * Counter.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "Counter.h"
#include <opencv.hpp>


Counter::Counter() {
	// TODO Auto-generated constructor stub

}

Counter::~Counter() {
	// TODO Auto-generated destructor stub
}

Counter::Counter(Line& l, int id_): line(l), id(id_){
	a = (l.getX().y - l.getY().y)/(l.getX().x - l.getY().x);
	b = l.getX().y - a*l.getX().x;
	in = 0;
	out = 0;
	stay = 0;
}


void Counter::addTracker(Tracker& tracker){
	trackers.push_back(tracker);
}


int Counter::removeTracker(Tracker& tracker){
	for(uint i = 0; i < trackers.size(); i++){
		if(tracker.getId() == trackers[i].getId()){
			trackers.erase(trackers.begin()+i);
			return i;
		}
	}
}


int Counter::getIn() const{
	return in;
}


int Counter::getOut() const{
	return out;
}


void Counter::updateSituation() {
	for(uint i = 0; i < trackers.size(); i++){
		cv::Point c = trackers[i].current();
		cv::Point init = trackers[i].initial();
		if(c.y >= std::min(line.getX().y, line.getY().y) && c.y <= std::max(line.getX().y, line.getY().y)) {
			if(!isInside(init) && isInside(c)) {
				in ++;
				trackers[i].setInitial(c);
			}
			else if(isInside(init) && !isInside(c)) {
				out ++;
				trackers[i].setInitial(c);
			}
			else
				stay ++;
		}
	}
}


bool Counter::isInside(const cv::Point& c) const {
	if(a > 0)
		return (a*c.x -c.y + b > 0);
	else
		return (a*c.x -c.y + b < 0);
}


int Counter::getId() const{
	return id;
}


void Counter::setLine(Line& l) {
	line = l;
}
