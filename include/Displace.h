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
 * Displace.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_DISPLACE_H_
#define INCLUDE_DISPLACE_H_

#include <geos/geom/Point.h>
#include <Clock.h>
#include <map/Map.h>
#include <parsers/SimulationConfiguration.h>


using namespace geos;
using namespace geos::geom;


/**
 * This class implements a Strategy design pattern. It acts as an interface for different displacement algorithms.
 */
class Displace {
public:

	/**
	 * Constructor of the class. It initializes members of the class
	 * @param simconfig a pointer to  a SimulationConfiguration containing the parameters of the simulation read from the simulation configuration file.
	 * @param speed the speed of displacement.
	 */
	Displace(SimulationConfiguration* simconfig, double speed);

	/**
	 * Default destructor.
	 */
	virtual ~Displace();

	/**
	 * This pure virtual method is implemented by each of the concrete subclasses derived from Displace.
	 * @return the new location where the object subject to the displacement operation has to move.
	 */
	virtual Point* generateNewLocation(Point * initLocation) = 0;

	/**
	 * Returns the speed of movement. Normally it is the speed set in the constructor of the Person object but in case of the HomeWorkDisplacement
	 * or HomeWorkMahattanDisplacement mobility patterns the speed is computed using the "time to travel" parameter of the corresponding simulation
	 * scenario object and the distance from the home location to the work location or from the anchor point to the home location in case the person
	 * visits an anchor point. The maximum from the two values is chosen as the movement speed.
	 * @return the speed of movement.
	 */
	double getSpeed();


protected:
	SimulationConfiguration* m_simConfig;
	double m_speed;
	virtual Point* computeNewLocation(Point* initLocation, double theta);


};

#endif /* INCLUDE_DISPLACE_H_ */
