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
 * MovableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef MOVABLEAGENT_H_
#define MOVABLEAGENT_H_

#include <agent/LocatableAgent.h>
#include <string>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents an Agent that can move inside the map. This is an abstract class. It is used as a base class for all
 * agents that can move. Currently Person and MobilePhone classes inherits it and are already implemented.
 */
class MovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class.
	 * @param m a pointer to a Map object used by the simulation.
	 * @param id the id of the object.
	 * @param initPosition the initial location on map.
	 * @param clock a pointer to the Clock object used by this simulation.
	 * @param initSpeed the initial speed of the agent. Depending on the derived classes, this value could be read
	 * from a configuration file. For example, for the Person class, this value is specified in the persons.xml configuration file.
	 */
	explicit MovableAgent(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed);

	/**
	 * The default destructor.
	 */
	virtual ~MovableAgent();


	/**
	 * Builds and returns a human readable string representation of the agent.
	 * @return a human readable string representation of the agent.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * A pure virtual method used to move the agent to a new location on the map. All the classes that inherit MovableAgent implement
	 * this function. The actual implementation is based on a Strategy design pattern. Displace defines the displacement strategy
	 * interface and the classes that inherits it implements the interface defining concrete displacement methods.
	 * @return the final location after displacement.
	 */
	virtual Point* move() = 0;

	/**
	 * Returns the speed of this agent.
	 * @return the speed of this agent.
	 */
	double getSpeed() const;

	/**
	 * Sets the speed of this agent.
	 * @param speed the speed of this agent.
	 */
	void setSpeed(double speed);

private:
	double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
