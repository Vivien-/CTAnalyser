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
	std::cout<<"~Counter()"<<std::endl;
}

Counter::Counter(Line& l, int id_): id(id_), line(l){
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
	return -1;
}

int Counter::getIn() const{
	return in;
}


int Counter::getOut() const{
	return out;
}

void Counter::updateSituation() {
	std::cout<<"nb of tracker in counter "<<id<<": "<<trackers.size()<<std::endl;
	for(uint i = 0; i < trackers.size(); i++){
		cv::Point c = trackers[i].current();
		cv::Point init = trackers[i].initial();
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" "<<__func__<<"] Initial pos of tracker "<<trackers[i].getId()<<" is "<<init<<std::endl;
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
		std::cout<<"["<<__FILE__<<":"<< __LINE__<<" in "<<__func__<<"] New set init position of tracker "<<trackers[i].getId()<<" is "<<trackers[i].initial()<<std::endl;
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
