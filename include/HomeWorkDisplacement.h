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
	HomeWorkDisplacement(SimConfig* simConfig, double speed, Point* homeLocation, Point* workLocation);
	virtual ~HomeWorkDisplacement();
	virtual Point* generateNewLocation(Point* p) override;
	//Point* getWorkLocation();
private:
	HomeWorkState stateTransition(Point* position);
	unsigned long initDeltaTStayHome();
	unsigned long initDeltaTStayWork();
	bool posAtDestination(Point* position, Point* destination) const;
	bool arrivedAtDestination(Point* position, Point* destination);
	Point* makeRandomStepAtWork(Point* initLocation);
	Point* toDestination(Point*  initLocation, Point* destination);
	double computeTheta(Point* p1, Point* p2) const;
	//Point* generateWorkLocation();



	unsigned long m_deltaTStayHome;
	unsigned long m_deltaTStayWork;
	HomeWorkState m_state;
	Point* m_homeLocation;
	Point* m_workLocation;
	//unsigned int m_workLocationIndex;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
