/*
 * LineControllerBasic.h
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#ifndef LINECONTROLLERBASIC_H_
#define LINECONTROLLERBASIC_H_
#include "LineController.h"


class LineControllerBasic : public LineController{
public:
	LineControllerBasic();
	virtual ~LineControllerBasic();
	void mouseCallback(int event, int _x, int _y, int , void* );
	void buttonCallback(int , void *id);
};

#endif /* LINECONTROLLERBASIC_H_ */
