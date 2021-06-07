/*
 * HomeWorkManhattanDisplacement.h
 *
 *  Created on: Jun 4, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_
#define INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_

#include <HomeWorkDisplacement.h>
#include <ManhattanDisplacement.h>

class HomeWorkManhattanDisplacement: public HomeWorkDisplacement {
public:
	HomeWorkManhattanDisplacement(SimulationConfiguration* simConfig, double speed, Point* homeLocation, Point* workLocation, Point* anchorLocation);
	virtual ~HomeWorkManhattanDisplacement();
	virtual Point* generateNewLocation(Point * initLocation) override;

private:
	virtual Point* toDestination(Point*  initLocation, Point* destination) override;
	ManhattanDisplacement m_manhattanDisplacement;
};

#endif /* INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_ */
