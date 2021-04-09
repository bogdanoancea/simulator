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

/**
 * This class is part of the Strategy design pattern used to implement the displacement of persons on the map. It implements
 * the home-work scenario, overriding the generateNewLocation() method from its superclass, Displace.
 */
class HomeWorkDisplacement: public Displace {
public:

	/**
	 * This is the constructor of the class.
	 * @param simConfig the SimConfig object containing the simulation configuration options read from the simulation configuration file.
	 * @param speed the speed of the person. This value is read from persons configuration file.
	 * @param homeLocation the home location. This value is set at the creation of the Person object and it is a point whose coordinates are
	 * generated from a normal distribution with the parameters read from the simulation configuration file. The persons involved in a simulation
	 * are equally distributed among the home locations.
	 * @param workLocation the work location the work location. This value is set at the creation of the Person object and it is a point whose coordinates are
	 * generated from a normal distribution with the parameters read from the simulation configuration file. If there are several work locations in this file,
	 * a work location is assigned for each person using a uniform distribution.
	 */
	HomeWorkDisplacement(SimConfig* simConfig, double speed, Point* homeLocation, Point* workLocation);

	/**
	 * This is the destructor of the class. It does notihng.
	 */
	virtual ~HomeWorkDisplacement();

	/**
	 * Implements the home - work behavior. It takes a pointer to the current location, and generates a new location according to the current status of the person.
	 * @param p a pointer to the current location.
	 * @return the new location.
	 */
	virtual Point* generateNewLocation(Point* p) override;

private:
	HomeWorkState stateTransition(Point* position);
	unsigned long initDeltaTStayHome() const;
	unsigned long initDeltaTStayWork() const;
	const bool posAtDestination(Point* position, Point* destination) const;
	const bool arrivedAtDestination(Point* position, Point* destination) const;
	Point* makeRandomStepAtWork(Point* initLocation);
	Point* toDestination(Point*  initLocation, Point* destination);
	double computeTheta(Point* p1, Point* p2) const;
	unsigned long m_deltaTStayHome;
	unsigned long m_deltaTStayWork;
	HomeWorkState m_state;
	Point* m_homeLocation;
	Point* m_workLocation;
	Distribution* m_angleDistribution;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
