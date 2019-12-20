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
 * Agent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef AGENT_H
#define AGENT_H

#include <map/Map.h>
#include <Clock.h>

/**
 * This is an abstract class, the base class for all agents involved in a simulation.
 */
class Agent {
public:
	Agent();
	/**
	 * Constructor of the class. Agent is the base class for all agents used in the simulator: persons, antennas, devices, mnos. Agent is an abstract class,
	 * users should build specific subclasses.
	 * @param m - a pointer to the Map object where the simulation take place.
	 * @param id - the id of this agent, it uniquely identifies the agent.
	 * @param clock - a pointer to a Clock object used by the simulator, the Clock is the same for all agents.
	 */
	Agent(const Map* m, const unsigned long id, const Clock* clock);

	/**
	 * Default destructor of the class.
	 */
	virtual ~Agent();

	/**
	 * The equal operator for agents.
	 * @param a the object with which we test the equality.
	 * @return true if this object is the equal to a, false otherwise. Two objects are considered to be equal if they have the same id.
	 */
	bool operator==(const Agent& a);


	/**
	 * Getter that returns a pointer to the Map object passed to the constructor when an object was build.
	 * @return a pointer to the Map object that was passed to the constructor. All agents use the same map for a simulation.
	 */
	virtual const Map* getMap() const;


	/**
	 * Returns a pointer to the Clock object used for simulation. All Agents use the same Clock object for a simulation.
	 * @return
	 */
	virtual const Clock* getClock() const;

	/**
	 * Returns the id of the object.
	 * @return the id of the object.
	 */
	virtual const unsigned long getId() const;

	virtual const string getName() const = 0;

	/**
	 * Builds a string with of the relevant information of the class. It is useful to output on the console or in a file
	 * the description of concrete agents.
	 * @return a string representation of the class content. The values of the members are written in this string.
	 */
	virtual const string toString() const = 0;


private:
	const Map* m_map;
	const unsigned long m_id;
	const Clock* m_clock;
};

#endif // AGENT_H
