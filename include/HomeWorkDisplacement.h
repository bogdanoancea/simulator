/*
 * HomeWorkDisplacement.h
 *
 *  Created on: Mar 22, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKDISPLACEMENT_H_
#define INCLUDE_HOMEWORKDISPLACEMENT_H_

#include <Displace.h>

class HomeWorkDisplacement: public Displace {
public:
	HomeWorkDisplacement(SimConfig* simConfig, double speed);
	virtual ~HomeWorkDisplacement();

	virtual Point* generateNewLocation(Point* p) override;

private:
	virtual Point* computeNewLocation(Point* initLocation, double theta) override;
	unsigned long m_deltaTStayHome;
	unsigned long m_deltaTStayWork;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
