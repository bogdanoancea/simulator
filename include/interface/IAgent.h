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
 * IAgent.h
 *
 *  Created on: Nov 27, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_INTERFACE_IAGENT_H_
#define INCLUDE_INTERFACE_IAGENT_H_

#include <string>
#include "../Map.h"
#include "../Clock.h"


class IAgent {

public:


	virtual const string getName() const = 0;

	/**
	 * Builds a string with of the relevant information of the class. It is useful to output on the console or in a file
	 * the description of concrete agents.
	 * @return a string representation of the class content. The values of the members are written in this string.
	 */
	virtual const string toString() const = 0;

	/**
	 * Getter that returns a pointer to the Map object passed to the constructor when an object was build.
	 * @return a pointer to the Map object that was passed to the constructor. All agents use the same map for a simulation.
	 */
	virtual const Map* getMap() const = 0;

	/**
	* Returns a pointer to the Clock object used for simulation. All Agents use the same Clock object for a simulation.
	* @return
	*/
	virtual const Clock* getClock() const = 0;

	/**
	* Returns the id of the object.
	* @return the id of the object.
	*/
	virtual const unsigned long getId() const = 0;


};

#endif /* INCLUDE_INTERFACE_IAGENT_H_ */
