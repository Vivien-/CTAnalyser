/*
 * Controller.h
 *
 *  Created on: 11 february. 2016
 *      Author: Vivien & Mohammed
 *
 *  The class Controller manages the diferrent parts of the software : lines, counters and detectors
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
	void setTrackerToCounter(int idTracker, int idCounter);
	std::map<int, Line>& getLines();
	Line& getLine(int id);
	std::map<int, Counter>& getCounters();
	Counter& getCounter(int id);
	std::map<int, Tracker>& getTrackers();
	Tracker& getTracker(int id);
	int getNextIdTracker();
	int getNextIdLine();
	int getNextIdCounter();
	//Draw the rectangles representing the trackers positions on the screen
	void displayTrackers(Mat& frame);
	//Look for new trackers in the image
	void processTrackers(Mat& im_gray, std::vector<Rect>& logos, CMT* cmt);
	//Delete old trackers
	void deleteUselessTrackers(std::vector<Rect>& logos);
	//Get the number of people that entered a coach (line is the entrance of a train car)
	int getEntered(int lineId);
	//Get the number of people that left a coach (line is the entrance of a train car)
	int getLeft(int lineId);
	//Update the counters asociated to each line
	void updateCountersSituation();
	void displaySituation(Mat &frame);

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
