/*
 * HomeWorkDisplacement.h
 *
 *  Created on: Mar 22, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKDISPLACEMENT_H_
#define INCLUDE_HOMEWORKDISPLACEMENT_H_

#include <Displace.h>
#include <HomeWorkState.h>

class HomeWorkDisplacement: public Displace {
public:
	HomeWorkDisplacement(SimConfig* simConfig, double speed, Point* homeLocation, Point* workLocation, unsigned int workLocationIndex);
	virtual ~HomeWorkDisplacement();
	virtual Point* generateNewLocation(Point* p) override;


private:
	virtual Point* computeNewLocation(Point* initLocation, double theta) override;
	HomeWorkState stateTransition(Point* position);
	unsigned long initDeltaTStayHome();
	unsigned long initDeltaTStayWork();
	bool posAtHome(Point* position) const;
	bool posAtWork(Point* position);

	unsigned long m_deltaTStayHome;
	unsigned long m_deltaTStayWork;
	HomeWorkState m_state;
	Point* m_homeLocation;
	Point* m_workLocation;
	unsigned int m_workLocationIndex;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
