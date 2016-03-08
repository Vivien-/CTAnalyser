/*
 * Tracker.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#ifndef TRACKER_H_
#define TRACKER_H_
#include "CppMT/CMT.h"
#include <opencv.hpp>

using namespace cv;
using cmt::CMT;

class Tracker {
public:
	Tracker();
	virtual ~Tracker();
	Tracker(cmt::CMT* cmt_, int id_, int lf);
	Point current() const;
	Point initial() const;
	void setInitial(Point& p);
	void setCurrent(Point& p);
	int getId() const;
	int getLifetime() const;
	void setLifetime(int l);
	void setCounterId(int id);
	int getCounterId() const;
	void display(Mat& frame);
	void processCMT(Mat& gray_frame);
	CMT* getCMT();

private:
	int id;
	int lifetime;
	Point c;
	Point i;
	CMT* cmt;
	int counterId;
};

inline bool operator==(const Tracker& t1, const Tracker& t2){ return t1.getId() == t2.getId(); }
inline bool operator!=(const Tracker& t1, const Tracker& t2){ return !operator==(t1,t2); }

#endif /* TRACKER_H_ */
