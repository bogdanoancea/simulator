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
 * ImmovableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef IMMOVABLEAGENT_H_
#define IMMOVABLEAGENT_H_

#include <agent/LocatableAgent.h>
#include <string>

using namespace geos;
using namespace geos::geom;

/**
 * This is a class that represents an agent that can have a location on map but it cannot move.
 * The only subclass of it is Antenna.
 */
class ImmovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class. Build an ImmovableAgent object with the parameters provided by the user.
	 * @param m a pointer to a Map object used in this simulation.
	 * @param id the id of this object.
	 * @param initialPosition the initial location on map.
	 * @param clock a pointer to a Clock object used in this simulation.
	 */
	explicit ImmovableAgent(const Map* m, const unsigned long id, Point* initialPosition,
			const Clock* clock);

	/**
	 * Default destructor.
	 */
	virtual ~ImmovableAgent();

	/**
	 * Builds a string representation of this class.
	 * @return a string representation of this class. It is used to write details of the ImmovableAgent objects in a file or on console.
	 */
	const string toString() const override;

};

#endif /* IMMOVABLEAGENT_H_ */
