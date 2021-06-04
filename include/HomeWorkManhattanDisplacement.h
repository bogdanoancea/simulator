/*
 * HomeWorkManhattanDisplacement.h
 *
 *  Created on: Jun 4, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_
#define INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_

#include <HomeWorkDisplacement.h>

class HomeWorkManhattanDisplacement: public HomeWorkDisplacement {
public:
	HomeWorkManhattanDisplacement(SimulationConfiguration* simConfig, double speed, Point* homeLocation, Point* workLocation, Point* anchorLocation);
	virtual ~HomeWorkManhattanDisplacement();
};

#endif /* INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_ */
