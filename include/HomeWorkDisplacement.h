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
	HomeWorkDisplacement(SimConfig* simConfig, double speed, Point* homeLocation, unsigned int workLocationIndex);
	virtual ~HomeWorkDisplacement();
	virtual Point* generateNewLocation(Point* p) override;
	//Point* getWorkLocation();
private:
	//virtual Point* computeNewLocation(Point* initLocation, double theta) override;
	HomeWorkState stateTransition(Point* position);
	unsigned long initDeltaTStayHome();
	unsigned long initDeltaTStayWork();
	bool posAtHome(Point* position) const;
	bool posAtWork(Point* position);
	bool arrivedAtWork(Point* position);
	bool arrivedAtHome(Point* position);
	Point* makeRandomStepAtWork(Point* initLocation);
	Point* toWork(Point* initLocation);
	Point* toHome(Point* initLocation);
	double computeTheta(Point* p1, Point* p2) const;
	//void setPosAtWork(Point* pt);
	Point* generateWorkLocation();



	unsigned long m_deltaTStayHome;
	unsigned long m_deltaTStayWork;
	HomeWorkState m_state;
	Point* m_homeLocation;
	Point* m_workLocation;
	unsigned int m_workLocationIndex;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
