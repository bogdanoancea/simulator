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
 * This is a class that represents an agent which has a fixed location on the map.
 * The only subclass of it is Antenna.
 */
class ImmovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class. It builds an ImmovableAgent object with the parameters provided by the user.
	 * @param m a pointer to a Map object used for a simulation.
	 * @param id the id of this object.
	 * @param initialPosition the initial location on map.
	 * @param clock a pointer to a Clock object used for a simulation.
	 */
	explicit ImmovableAgent(const Map* m, const unsigned long id, Point* initialPosition, const Clock* clock);

	/**
	 * Default destructor.
	 */
	virtual ~ImmovableAgent();



	/**
	 * Builds a string with the most relevant or all information of the class. It is useful to output the description of concrete agents
	 * to the console or to a file. Depending on the value of the <tt>detailed</tt> parameter, the string can contain only the values of the
	 * the most important members (<tt>detailed = false</tt>) or all members of the class.
	 * @param detailed if true, the string will contain the values of all members/parameters of the agent, otherwise only the most
	 * important ones are written to the output string. Each derived class will decide which are the most important members.
	 * @return a string object containing the most important or all parameters/members of the agent.
	 */
	const string toString(bool detailed = false) const override;

};

#endif /* IMMOVABLEAGENT_H_ */
