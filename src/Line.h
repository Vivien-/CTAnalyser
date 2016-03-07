/*
 * Line.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#ifndef LINE_H_
#define LINE_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <opencv.hpp>



class Line {
public:
	Line();
	virtual ~Line();
	Line(cv::Point a, cv::Point b, int id_);
	cv::Point getX(void) const;
	cv::Point getY(void) const;
	int getId(void) const;
	void setX(int _x, int _y);
	void setY(int _x, int _y);
private:
	cv::Point X;
	cv::Point Y;
	int id;
};

#endif /* LINE_H_ */
