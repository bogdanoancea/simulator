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
 * HomeWorkManhattanDisplacement.h
 *
 *  Created on: Jun 4, 2021
 *      Author: Bogdan Oancea
 *      Email:  bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_
#define INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_

#include <HomeWorkDisplacement.h>
#include <ManhattanDisplacement.h>
#include <Directions.h>


/**
 * This class is part of the Strategy design pattern used to implement the displacement of persons on the map. It implements
 * the home-work scenario with Manhattan mobility pattern, overriding the generateNewLocation() method from its superclass, HomeWorkDisplacement.
 * For an explanation on how this mobility pattern works see HomeWorkManhattanScenario class.
 * This class overrides the <tt>toDestination()</tt>, <tt>arrivedAtDestination()</tt> method inherited from
 * the HomeWorkDisplacement class to implement the Manhattan mobility pattern between home, work and anchor points.
 */
class HomeWorkManhattanDisplacement: public HomeWorkDisplacement {
public:
	/**
	 * This is the constructor of the class.
	 * @param simConfig the SimConfig object containing the simulation configuration options read from the simulation configuration file.
	 * @param speed the speed of the person. This value is initially read from persons configuration file.
	 * @param homeLocation the home location. This value is set at the creation of the Person object and it is a point whose coordinates are
	 * generated from a normal distribution with the parameters read from the simulation configuration file. The persons involved in a simulation
	 * are equally distributed among the home locations.
	 * @param workLocation the work location . This value is set at the creation of the Person object and it is a point whose coordinates are uniformly
	 * generated inside a disk with the parameters read from the simulation configuration file. If there are several work locations in this file,
	 * a work location is assigned for each person using a uniform distribution.
	 * @param anchorLocation the anchor point location. This value is set at the creation of the Person object and it is a point whose coordinates are uniformly
	 * generated inside a disk with the parameters read from the simulation configuration file. If there are several anchor point locations in this file,
	 * a work location is assigned for each person using a uniform distribution. An anchor point is optional, only some persons will have an anchor point.
	 * The persons who have an anchor point are selected randomly according to a probability specified in the simulation configuration file.
	 */
	HomeWorkManhattanDisplacement(SimulationConfiguration *simConfig, double speed, Point *homeLocation, Point *workLocation, Point *anchorLocation);

	/**
	 * Default destructor.
	 */
	virtual ~HomeWorkManhattanDisplacement();

	/**
	 *
	 * @param initLocation
	 * @return
	 */
	virtual Point* generateNewLocation(Point * initLocation) override;

private:
	virtual Point* toDestination(Point*  initLocation, Point* destination) override;
	virtual const bool arrivedAtDestination(Point* position, Point* destination) const override;
	virtual Point* makeRandomStepAtWork(Point* initLocation) override;
	ManhattanDisplacement m_manhattanDisplacement;
};

#endif /* INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_ */
