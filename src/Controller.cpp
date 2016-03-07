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
void Controller::addTrackerToCounter(int idTracker, int idCounter){
	counters[idCounter].addTracker(trackers[idTracker]);
}

void Controller::removeTrackerFromCounter(int idTracker, int idCounter){
	counters[idCounter].removeTracker(trackers[idTracker]);
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

	return 0;
}

int Controller::getLeft(int lineId) {
	return 0;
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
				printf("%d et %d", t->getId(), cur->getId());
				double dist = cv::norm(cv::Mat(a),cv::Mat(b));
				double diameter = sqrt(r.size.width * r.size.width + r.size.height * r.size.height);
				printf("diameter=%f\tdist=%f\n", diameter,dist);
				if(dist < diameter/4){
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
