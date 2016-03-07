/*
 * LineController.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#ifndef LINECONTROLLER_H_
#define LINECONTROLLER_H_
#include "Line.h"
#include "Counter.h"
#include "Controller.h"

class LineController {
public:
	LineController();
	virtual ~LineController();
	Line addLine();
	void removeLine(int id);
	//as lines is sorted we can do a log search TODO
	int getLinePositionById(int id);
	void draw();
	void setBackground(const cv::Mat & m);
	std::vector<Line>& getLines(void);
	cv::Mat& frame();
	int getNextId();
	void process(std::string windowName, cv::Mat& frame);
	//	void mouseCallback(int event, int _x, int _y, int , void* );
	//	void buttonCallback(int , void *id);
	void setController(Controller *controller);

	Controller *controller;
	int next_id;
	int click_nb;
	bool destroy_window;
	bool destroyed;
	int new_button = 0;
	std::vector<Line> lines;
	std::map<int, int> buttons_id;
	cv::Mat currentBackground;
	cv::Point X;
	cv::Point Y;


private:
	void drawLine(const Line &l, cv::Scalar color);

};

#endif /* LINECONTROLLER_H_ */
