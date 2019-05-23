/*
 * Data simulator for mobile phone network events
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
#include <iomanip>
#include <sstream>

using namespace geos;
using namespace geos::geom;

LocatableAgent::LocatableAgent(Map* m, long id, Point* initLocation, Clock* clock) :
		Agent(m, id, clock) {
	m_location = initLocation;

}

LocatableAgent::~LocatableAgent() {
//		if (m_location != nullptr)
//			this->getMap()->getGlobalFactory()->destroyGeometry(m_location);
}

Point* LocatableAgent::getLocation() const {
	return (m_location);
}

void LocatableAgent::setLocation(Point* location) {
	m_location = location;
}

string LocatableAgent::toString() const {
	ostringstream ss;
	if (m_location != nullptr)
		ss << left << setw(15) << getId() << setw(15) << getLocation()->getCoordinate()->x << setw(15) << getLocation()->getCoordinate()->y;
	else
		ss << left << setw(15) << getId() << setw(15) << "null" << setw(15) << "null";
	return (ss.str());

}

const string LocatableAgent::dumpLocation() {
	ostringstream ss;
	char sep = ',';
	if (getClock() != nullptr)
		ss << left << getClock()->getCurrentTime() << sep;
	ss << getId() << sep << getLocation()->getCoordinate()->x << sep << getLocation()->getCoordinate()->y;
	return (ss.str());
}
