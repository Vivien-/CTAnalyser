/*
 * Controller.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include <map>
#include "Tracker.h"
#include "Line.h"
#include "Counter.h"


class Controller {
public:
	Controller(int lf);
	virtual ~Controller();
	void addLine(Line& l);
	void removeLine(int id);
	void addTracker(Tracker& t);
	void removeTracker(int id);
	void addCounter(Counter& c);
	void removeCounter(int id);
	void addTrackerToCounter(int idTracker, int idCounter);
	void removeTrackerFromCounter(int idTracker, int idCounter);
	std::map<int, Line>& getLines();
	Line& getLine(int id);
	std::map<int, Counter>& getCounters();
	Counter& getCounter(int id);
	std::map<int, Tracker>& getTrackers();
	Tracker& getTracker(int id);
	int getNextIdTracker();
	int getNextIdLine();
	int getNextIdCounter();
	void displayTrackers(Mat& frame);
	void processTrackers(Mat& im_gray, std::vector<Rect>& logos, CMT* cmt);
	void deleteUselessTrackers(std::vector<Rect>& logos);

private:
	std::map<int,Line> lines;
	std::map<int,Counter> counters;
	std::map<int,Tracker> trackers;
	int next_id_tracker;
	int lifetime;
	int next_id_line;
	int next_id_counter;
};

#endif /* CONTROLLER_H_ */
