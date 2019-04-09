/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "LocatableAgent.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;


LocatableAgent::LocatableAgent(Point initLocation) {
	const Coordinate *c = initLocation.getCoordinate();
	m_location = getWorld()->getMap()->getGlobalFactory()->createPoint(*c);
}

LocatableAgent::~LocatableAgent() {
	// TODO Auto-generated destructor stub
}

Point* LocatableAgent::getLocation() const {
	return m_location;
}

void LocatableAgent::setLocation(Point* location) {
	m_location = location;
}
