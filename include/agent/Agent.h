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
 * A simulation involves different types of agents implemented by specific subclasses:
 * \li
 * 	\item \code{Antenna}
 * 	\item \code{MobilePhone}
 * 	\item \code{MobileOperator}
 * 	\item \code{Person}
 * \li
 * All agents are stored in a container: an \code{AgentsCollection} object.
 * Some of the agents have a fixed location and they are derived from the \code{LocatableAgent}
 * class that adds a location of the agent on the map. There are two type of \code{LocatableAgent}s:
 * some of them have a fixed location on the map and during the simulation and this type of behavior is
 * defined by the \code{ImmovableAgent} class and some of them can change their location during the
 * simulation and they are subclasses of the \code{MovableAgent} class.
 * \code{MovableAgent} objects can be either a \code{Person} instance or a \code{HoldableAgent}
 * subclass which means a device: a \code{MobilePhone} object or a \code{Tablet} object.
 * \code{LocatableAgent}, \code{ImmovableAgent}, \code{HoldableAgent} are abstract classes. The only classes
 * that can be instantiated are \code{Antenna}, \code{MobilePhone}, \code{MobileOperator}, \code{Person}
 * \code{Tablet}.
 * An agent object has a unique id in the whole set of agents.
 */
class Agent {
public:
	Agent();
	/**
	 * Constructor of the class. \code{Agent} is the base class for all agents used in the simulator: persons,
	 * antennas, mobile devices, mobile network operators. \code{Agent} is an abstract class,
	 * users should build specific subclasses. An \code{Agent} keeps a pointer to the map of the simulation,
	 * and pointer to the simulation clock.
	 * @param m - a pointer to the \code{Map} object where the simulation take place.
	 * @param id - the id of this agent, it uniquely identifies the agent.
	 * @param clock - a pointer to a \code{Clock} object used by the simulator, the \code{Clock} object is the same for all agents.
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
	 * Getter function that returns a pointer to the \code{Map} object passed to the constructor when an \code{Agent} object was build.
	 * @return a pointer to the \code{Map} object that was passed to the constructor. All agents use the same map for a simulation.
	 */
	const virtual Map* getMap() const;

	/**
	 * Returns a pointer to the \code{Clock} object used for simulation. All agents use the same \code{Clock} object for a simulation.
	 * @return a pointer to the \code{Clock} object used for simulation.
	 */
	const virtual Clock* getClock() const;

	/**
	 * Returns the id of the object.
	 * @return the id of the object.
	 */
	const virtual unsigned long getId() const;

	/**
	 * Builds a \code{string} with the most relevant or all information of the class. It is useful to output to the console or to a file
	 * the description of concrete agents. Depending on the value of the \param{detailed} parameter, the string can contain only the values of the
	 * the most important members (\code{detailed = false} or all members of the class.
	 * @param detailed if true, the string will contain the values of all members/parameters of the agent, otherwise only the most important ones are outputed.
	 * @return a \code{string} representation of the class content containing the most important or all parameters of the agent.
	 * The values of some members are written in this string.
	 */
	const virtual string toString(bool detailed = false) const = 0;


private:
	const Map* m_map;
	const unsigned long m_id;
	const Clock* m_clock;
};

#endif // AGENT_H
