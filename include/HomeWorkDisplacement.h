/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * HomeWorkDisplacement.h
 *
 *  Created on: Mar 22, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_HOMEWORKDISPLACEMENT_H_
#define INCLUDE_HOMEWORKDISPLACEMENT_H_

#include <Displace.h>
#include <HomeWorkState.h>

/**
 * This class is part of the Strategy design pattern used to implement the displacement of persons on the map. It implements
 * the home-work scenario, overriding the generateNewLocation() method from its superclass, Displace. For an explanation on how this
 * mobility pattern works see HomeWorkScenario class.
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
	HomeWorkDisplacement(SimulationConfiguration* simConfig, double speed, Point* homeLocation, Point* workLocation, Point* anchorLocation);

	/**
	 * This is the destructor of the class. It does nothing.
	 */
	virtual ~HomeWorkDisplacement();

	/**
	 * Implements the home - work behavior. It takes a pointer to the current location, and generates a new location according to the current status of the person.
	 * @param p a pointer to the current location.
	 * @return the new location.
	 */
	virtual Point* generateNewLocation(Point* p) override;
protected:
	virtual Point* toDestination(Point*  initLocation, Point* destination);
	double computeTheta(Point* p1, Point* p2) const;
	virtual const bool arrivedAtDestination(Point* position, Point* destination) const;
	virtual Point* makeRandomStepAtWork(Point* initLocation);
	Distribution* m_angleDistribution;


private:
	HomeWorkState stateTransition(Point* position);
	long initDeltaTStayHome() const;
	long initDeltaTStayWork() const;
	long initDeltaTStayAnchor() const;
	const bool posAtDestination(Point* position, Point* destination) const;
	HomeWorkState getState() const;


	long m_deltaTStayHome;
	long m_deltaTStayWork;
	long m_deltaTStayAnchor;
	HomeWorkState m_state;
	Point* m_homeLocation;
	Point* m_workLocation;
	Point* m_anchorLocation;
	double m_stepLength;
	shared_ptr<Distribution> m_defaultAngleDistribution;
};

#endif /* INCLUDE_HOMEWORKDISPLACEMENT_H_ */
