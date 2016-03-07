/*
 * LineControllerBasic.cpp
 *
 *  Created on: 11 févr. 2016
 *      Author: vivien
 */

#include "LineControllerBasic.h"

LineControllerBasic::LineControllerBasic() {
	// TODO Auto-generated constructor stub

}

LineControllerBasic::~LineControllerBasic() {
	// TODO Auto-generated destructor stub
}

void LineControllerBasic::mouseCallback(int event, int _x, int _y, int , void*){
	// if(event == EVENT_LBUTTONDOWN ){
	// 	if(click_nb%2 == 0) {
	// 		X.x = _x;
	// 		X.y = _y;
	// 	} else {
	// 		Y.y = _y;
	// 		Y.x = _x;
	// 		Line l = addLine(X,Y);
	// 		counters.push_back(l);
	// 		controller.draw();
	// 		imshow(window_name, controller.frame());
	// 		int this_id = controller.getNextId() - 1;
	// 		buttons_id[new_button] = this_id;
	// 		createButton(patch::to_string(this_id), on_change, &buttons_id[new_button++], CV_CHECKBOX);
	// 	}
	// 	click_nb++;
	// }
}


void LineControllerBasic::buttonCallback(int , void *id){
	// int position = controller.getLinePositionById(*(int*)id);
	// if(position < 0 || position >= (int)counters.size())
	// 	asm("int3");
	// controller.removeLine(*(int*)id);
	// counters.erase(counters.begin()+position);
	// destroy_window = true;

}

