/*
 * Controller.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */
#include "Controller.h"

Controller::Controller(int lf) {
	next_id_counter = 0;
	next_id_line = 0;
	lifetime = lf;
	next_id_tracker = 0;
}

Controller::~Controller() {
	std::cout<<"~Controller()"<<std::endl;
	// TODO Auto-generated destructor stub
}

void Controller::addLine(Line& l){
	lines.insert(std::pair<int,Line>(l.getId(), l));
	//A counter is linked to each new line
	Counter c(l,next_id_line);
	addCounter(c);
	next_id_line++;
}

void Controller::removeLine(int id){
	lines.erase(id);
}

void Controller::addTracker(Tracker& t){
	trackers.insert(std::pair<int,Tracker>(t.getId(), t));
	next_id_tracker++;
}

void Controller::removeTracker(int id){
	trackers.erase(id);
}

void Controller::addCounter(Counter& c){
	counters.insert(std::pair<int, Counter>(c.getId(), c));
	next_id_counter++;
}

void Controller::removeCounter(int id){
	counters.erase(id);
}

void Controller::setTrackerToCounter(int idTracker, int idCounter){
	int oldcounterid = trackers[idTracker].getCounterId();
	if(oldcounterid != -1)
		counters[oldcounterid].removeTracker(trackers[idTracker]);
	trackers[idTracker].setCounterId(idCounter);
	counters[idCounter].addTracker(trackers[idTracker]);
}

std::map<int, Line>& Controller::getLines() {
	return lines;
}

Line& Controller::getLine(int id) {
	return lines[id];
}

std::map<int, Counter>& Controller::getCounters() {
	return counters;
}

Counter& Controller::getCounter(int id) {
	return counters[id];
}

std::map<int, Tracker>& Controller::getTrackers() {
	return trackers;
}

Tracker& Controller::getTracker(int id) {
	return trackers[id];
}

int Controller::getNextIdTracker() {
	return next_id_tracker;
}

int Controller::getNextIdLine(){
	return next_id_line;
}

int Controller::getNextIdCounter() {
	return next_id_counter;
}

void Controller::displayTrackers(Mat& frame){
	for(std::map<int,Tracker>::iterator it = trackers.begin(); it != trackers.end(); ++it) {
		trackers[it->first].display(frame);
	}
}

int Controller::getEntered(int lineId) {
	return getCounter(lineId).getIn();
}

int Controller::getLeft(int lineId) {
	return getCounter(lineId).getOut();
}

void Controller::updateCountersSituation() {
	for(auto it = trackers.begin(); it != trackers.end(); ++it) {
		int bestLineId = 0;	// TODO bestLineId = someComputationToGetTheBestLineId();
		setTrackerToCounter(trackers[it->first].getId(), bestLineId);
	}
	for(auto it = counters.begin(); it != counters.end(); ++it)
		counters[it->first].updateSituation();

	for(auto it = counters.begin(); it != counters.end(); ++it) {
		std::vector<Tracker> trackers = counters[it->first].trackers;
		for(uint i = 0; i < trackers.size(); i++)
			std::cout<<"["<<__FILE__<<":"<< __LINE__<<"] Initial pos of tracker "<<trackers[i].getId()<<" is "<<trackers[i].initial()<<std::endl;
	}
}

void Controller::processTrackers(Mat& im_gray, std::vector<Rect>& logos, CMT* cmt) {
	for(std::map<int,Tracker>::iterator it = trackers.begin(); it != trackers.end(); ++it) {
		trackers[it->first].processCMT(im_gray);
	}
	for(uint i = 0; i < logos.size(); i++) {
		Point2f a(logos[i].x,logos[i].y);
		bool match = true;
		for(std::map<int,Tracker>::iterator it = trackers.begin(); it != trackers.end(); ++it) {
			Point2f b(trackers[it->first].current());
			double dist = cv::norm(cv::Mat(a),cv::Mat(b));
			double diameter = sqrt(logos[i].width*logos[i].width + logos[i].height*logos[i].height);
			if(dist < diameter) {
				trackers[it->first].setLifetime(lifetime);
				match = false;
				break;
			}
		}
		if(match){
			std::cout<<"New tracker of id "<<getNextIdTracker()<<std::endl;
			cmt->initialize(im_gray, logos[i]);
			Tracker t(cmt, getNextIdTracker(), lifetime);
			addTracker(t);
		}
	}
}

void Controller::deleteUselessTrackers(std::vector<Rect>& logos) {
	for(auto it = trackers.begin(); it != trackers.end(); ) {
		Tracker* t = &trackers[it->first];
		Point2f a(t->current());

		//update lifetime (number of frame where we keep the tracker)
		for(uint j = 0; j < logos.size(); j++) {
			Point2f b(logos[j].x,logos[j].y);
			RotatedRect r = t->getCMT()->bb_rot;
			double dist = cv::norm(cv::Mat(a),cv::Mat(b));
			double diameter = sqrt(r.size.width * r.size.width + r.size.height * r.size.height);
			if(dist < diameter){
				t->setLifetime(t->getLifetime()+1);
				break;
			}
		}
		t->setLifetime(t->getLifetime() - 1);

		//delete tracker if lifetime ended
		if(t->getLifetime() <= 0){
			//remove tracker from the counter it is linked to
			int oldcounterid = trackers[t->getId()].getCounterId();
			if(oldcounterid != -1)
				counters[oldcounterid].removeTracker(*t);

			//remove tracker from the tracker list
			trackers.erase(it++);
		} else {
			++it;
		}

		//delete redundancy trackers (might not be a good idea if tracker are close that would lose infos)
		for(auto it2 = trackers.begin(); it2 != trackers.end(); ) {
			Tracker* cur = &trackers[it2->first];
			if(t != cur) {
				Point2f b(cur->current());
				RotatedRect r = cur->getCMT()->bb_rot;
//				printf("%d et %d\n", t->getId(), cur->getId());
				double dist = cv::norm(cv::Mat(a),cv::Mat(b));
				double diameter = sqrt(r.size.width * r.size.width + r.size.height * r.size.height);
//				printf("diameter=%f\tdist=%f\n", diameter,dist);
				if(dist < diameter/4){
					//remove tracker from the counter it is linked to
					int oldcounterid = trackers[cur->getId()].getCounterId();
					if(oldcounterid != -1)
						counters[oldcounterid].removeTracker(*cur);

					//remove tracker from the tracker list
					trackers.erase(it2++);
					break;
				} else {
					++it2;
				}
			} else
				++it2;
		}
	}
}

void Controller::displaySituation(Mat& frame) {
	if(getLines().size()) {
		updateCountersSituation();
		int i = 0;
		for(auto it = lines.begin(); it != lines.end(); ++it) {
			int entered = getEntered(0);
			int left = getLeft(0);
			putText(frame, std::to_string(entered) + " " + std::to_string(left), Point(5,15+15*i++), CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,0));
		}
	}
}
