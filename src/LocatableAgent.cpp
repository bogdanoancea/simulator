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
 * LocatableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <LocatableAgent.h>
#include <Map.h>
#include <Constants.h>
#include <iomanip>
#include <sstream>

using namespace geos;
using namespace geos::geom;

LocatableAgent::LocatableAgent(const Map* m, const unsigned long id, Point* initLocation, const Clock* clock) :
		Agent(m, id, clock) {
	m_location = initLocation;

}

LocatableAgent::~LocatableAgent() {
}

Point* LocatableAgent::getLocation() const {
	return (m_location);
}

void LocatableAgent::setLocation(Point* location) {
	m_location = location;
}

const string LocatableAgent::toString() const {
	ostringstream ss;
	if (m_location != nullptr)
		ss << left << setw(15) << getId() << setw(15) << getLocation()->getCoordinate()->x << setw(15) << getLocation()->getCoordinate()->y;
	else
		ss << left << setw(15) << getId() << setw(15) << "null" << setw(15) << "null";
	return (ss.str());

}

const string LocatableAgent::dumpLocation() {
	ostringstream ss;
	char sep = Constants::sep;
	if (getClock() != nullptr)
		ss << getClock()->getCurrentTime() << sep;
	ss << fixed << getId() << sep << getLocation()->getX() << sep << getLocation()->getY();
	return (ss.str());
}

const string LocatableAgent::getName() const {
	return ("LocatableAgent");
}
