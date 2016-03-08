/*
 * Counter.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
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
