/*
 * LineController.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien & mohammed
 */

#ifndef LINECONTROLLER_H_
#define LINECONTROLLER_H_

#include <string>
#include <opencv2/opencv.hpp>
class Line;
class Counter;
class Controller;

class LineController {
public:
	LineController();
	~LineController();
	//create and store a new line. return the created line
	Line addLine();
	void removeLine(int id);
	//as lines is sorted we can do a log search TODO
	int getLinePositionById(int id);
	//draw all the lines created by the user on the background
	void draw();
	//set the background as the current video frame
	void setBackground(const cv::Mat & m);
	std::vector<Line>& getLines(void);
	//get the background
	cv::Mat& frame();
	//this class is the only one that can create line, so it manages line id
	int getNextId();
	//bind the callback functions to the background. Allow user to create and delete lines
	void process(std::string windowName, cv::Mat& frame);
	void setController(Controller *controller);
	Controller *controller;
	int click_nb;
	bool destroy_window;
	//this class is the only one that can create button, so it manages button id
	int new_button;
	std::map<int, int> buttons_id;
	//endpoints of the next line to create
	cv::Point X;
	cv::Point Y;

private:
	//this class is the only one that can create line, so it manages line id
	int next_id;
	//[TODO use something else than opencv for the ui] because we have to recreate a window when deleting a line
	bool destroyed;
	std::vector<Line> lines;
	cv::Mat currentBackground;
	std::string window_name;
	void drawLine(const Line &l, cv::Scalar color);
};

#endif /* LINECONTROLLER_H_ */
