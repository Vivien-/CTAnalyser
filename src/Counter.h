/*
 * Counter.h
 *
 *  Created on: 11 february. 2016
 *      Author: vivien & Mohammed
 *
 *  Counter is a class that counts the number of people entering and leaving each train car.
 *  The entrance of a train car is represented by a line. Whenever a tracker crosses a line,
 *  the number of people inside or outside the car is incremented. To each counter we associate
 *  a single line and a set of trackers. These trackers are generally nearby the line.
 */

#ifndef COUNTER_H_
#define COUNTER_H_
#include "Line.h"
#include "Tracker.h"

class Counter {
public:
	Counter();
	virtual ~Counter();
	Counter(Line& l, int id_);
	void addTracker(Tracker& tracker);
	int removeTracker(Tracker& tracker);
	int getIn() const;
	int getOut() const;
	void updateSituation();
	int getId() const;
	void setLine(Line& l);
	std::vector<Tracker> trackers;
private:
	int id;
	Line line;
	float a;
	float b;
	int in, out, stay;
	bool isInside(const cv::Point& c) const;
};

#endif /* COUNTER_H_ */
