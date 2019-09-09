/*
 * Data simulator for mobile phone network events
 *
 * MovableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <MovableAgent.h>
#include <iomanip>
#include <sstream>

MovableAgent::MovableAgent(const Map* m, const unsigned long id, Point* initialPosition, const Clock* clock, double initialSpeed) :
		LocatableAgent(m, id, initialPosition, clock), m_speed { initialSpeed } {
}

MovableAgent::~MovableAgent() {
}

double MovableAgent::getSpeed() const {
	return (m_speed);
}

void MovableAgent::setSpeed(double val) {
	m_speed = val;
}

const string MovableAgent::toString() const {
	ostringstream ss;
	ss << LocatableAgent::toString() << left << setw(15) << m_speed;
	return (ss.str());
}

const string MovableAgent::getName() const {
	return ("MovableAgent");
}
