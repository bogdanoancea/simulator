/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "LocatableAgent.h"
#include "Agent.h"
#include <geos/geom/Point.h>
#include <string>
#include <sstream>
#include <iomanip>

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

Point& LocatableAgent::getLocation() const {
	return *m_location;
}

void LocatableAgent::setLocation(Point& location) {
	const Coordinate *c = location.getCoordinate();
	m_location = this->getMap()->getGlobalFactory()->createPoint(*c);
}

string LocatableAgent::toString() {
	ostringstream ss;
	if (m_location != nullptr)
		ss << left << setw(15) << getId() << setw(15) << getLocation().getCoordinate()->x << setw(15) << getLocation().getCoordinate()->y;
	else
		ss << left << setw(15) << getId() << setw(15) << "null" << setw(15) << "null";
	return (ss.str());

}

void LocatableAgent::dumpLocation() {
	cout << left << getId() << "," << getLocation().getCoordinate()->x << "," << getLocation().getCoordinate()->y << endl;
}
