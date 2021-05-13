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
 * LocatableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef LOCATABLEAGENT_H_
#define LOCATABLEAGENT_H_

#include <agent/Agent.h>


using namespace geos;
using namespace geos::geom;
using namespace std;

/**
 * This class extends the \code{Agent} class and adds location on a map. It has methods to set and get the location on the map
 * and also to build a string with the coordinates of the location.
 */
class LocatableAgent: public Agent {
public:

	/**
	 * Constructor of the class. Builds an object that has a location on the map of the simulation.
	 * @param m a pointer to a \code{Map} object used in this simulation.
	 * @param id the id of the object.
	 * @param initLocation a pointer to a \code{Point} object representing the initial location of the object.
	 * @param clock a pointer to a \code{Clock} object used in this simulation.
	 */
	explicit LocatableAgent(const Map* m, const unsigned long id, Point* initLocation, const Clock* clock);

	/**
	 * Destructor of the class.
	 */
	virtual ~LocatableAgent();


	/**
	 * Builds a \code{string} with the most relevant or all information of the class. It is useful to output the description of concrete agents
	 * to the console or to a file. Depending on the value of the \param{detailed} parameter, the string can contain only the values of the
	 * the most important members (\code{detailed = false}) or all members of the class.
	 * @param detailed if true, the string will contain the values of all members/parameters of the agent, otherwise only the most
	 * important ones are written to the output string. Each derived class will decide which are the most important members.
	 * @return a \code{string} object containing the most important or all parameters/members of the agent.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Returns the location on the map.
	 * @return the location on the map as a pointer to a \code{Point} object.
	 */
	virtual Point* getLocation() const;

	/**
	 * Sets the location of the agent on the map.
	 * @param location the location of the agent on the map passed as a pointer to a \code{Point} object.
	 */
	virtual void setLocation(Point* location);

	/**
	 * Builds a \code{string} representation of the location
	 * @return a \code{string} representation of the location, containing the values of the coordinates of the location.
	 */
	const string dumpLocation() const;

private:

	Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
