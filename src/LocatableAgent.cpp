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

LocatableAgent::LocatableAgent(Map* m, long id, Point* initLocation) :
		Agent(m, id) {
	if (initLocation != nullptr) {
		const Coordinate *c = initLocation->getCoordinate();
		m_location = this->getMap()->getGlobalFactory()->createPoint(*c);
	}
	else
		m_location = nullptr;

}

LocatableAgent::~LocatableAgent() {
}

Point* LocatableAgent::getLocation() const {
	return m_location;
}

void LocatableAgent::setLocation(Point* location) {
	//const Coordinate *c = location.getCoordinate();
	m_location = location;//this->getMap()->getGlobalFactory()->createPoint(*c);
}

string LocatableAgent::toString() {
	ostringstream ss;
	if (m_location != nullptr)
		ss << left << setw(15) << getId() << setw(15) << getLocation()->getCoordinate()->x << setw(15) << getLocation()->getCoordinate()->y;
	else
		ss << left << setw(15) << getId() << setw(15) << "null" << setw(15) << "null";
	return (ss.str());

}

string LocatableAgent::dumpLocation(Clock* clock) {
	ostringstream ss;
	char sep = ',';
	if (clock != nullptr)
		ss << left << clock->getCurrentTime() << sep;
	ss << getId() << sep << getLocation()->getCoordinate()->x << sep << getLocation()->getCoordinate()->y;
	return (ss.str());

}
