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
	/**
	 * This method generate a new position, starting from initLocation toward destination. It is used to move the person from home to work location, from work
	 * location to an anchor point location and from an anchor point location to home location. It computes the direction angle from the
	 * current location to the destination, adds a small random noise to this direction (generated using the distribution given by the
	 * m_angleDistribution member) and then make a step (using the speed and the time step) in this direction. If the final position falls outside the map
	 * the person, it tries to generate another location by changing the direction, generating it with a uniform distribution with the
	 * limits 0 and 2 PI. If after 10 times the final location still falls outside the map, the person stays in the same initial location.
	 * Before returning the new position, it checks it this new position is close enough to the destination and if it is sets the returned value
	 * to the destination location. Checking is done by the <tt>arrivedAtDestination</tt> method.
	 * @param initLocation the initial (current) location.
	 * @param destination the destination location.
	 * @return a new location on the map, toward the destination location.
	 */
	virtual Point* toDestination(Point*  initLocation, Point* destination);

	/**
	 * Computes the direction angle (on the trigonometric circle) of the line segment defined by the p1 and p2 parameters. The value of this angle is between 0 and 2 PI.
	 * @param p1 the first point of the line segment.
	 * @param p2 the second point of the line segment.
	 * @return the direction angle (on the trigonometric circle) of the line segment defined by the p1 and p2 parameters. The value of this angle is between 0 and 2 PI.
	 */
	double computeTheta(Point* p1, Point* p2) const;

	/**
	 * Checks if a person arrived at a destination point. If the distance between the current position (given by the parameter position) and the destination
	 * location is less than a step (computed as the product between the speed and the time step) the person is considered to have reached his/her destination
	 * and the returned value is true.
	 * @param position the current position.
	 * @param destination the destination position.
	 * @return true if the position parameter is close enough to the destination point, false otherwise.
	 */
	virtual const bool arrivedAtDestination(Point* position, Point* destination) const;

	/**
	 * While at work position, this method generates a random location starting from the initLocation to implement a random walk
	 * closely to the work location.
	 * @param initLocation the initial location.
	 * @return a random location starting from the initLocation
	 */
	virtual Point* makeRandomStepAtWork(Point* initLocation);

	/**
	 * This probability distribution is used to generate a noise to be added to the direction angle. The direction angle is the angle of the line
	 * segment between the current position and the destination position.
	 */
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
