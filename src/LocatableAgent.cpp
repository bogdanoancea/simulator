/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "LocatableAgent.h"

LocatableAgent::LocatableAgent(Point initLocation) :
		m_location { initLocation } {
	// TODO Auto-generated constructor stub

}

LocatableAgent::~LocatableAgent() {
	// TODO Auto-generated destructor stub
}

const Point& LocatableAgent::getLocation() const {
	return m_location;
}

void LocatableAgent::setLocation(const Point& location) {
	m_location = location;
}
