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


MovableAgent::MovableAgent(Map* m, long id, Point* initialPosition, double initialSpeed) :
		LocatableAgent(m, id, initialPosition), m_speed { initialSpeed } {
	// TODO Auto-generated constructor stub
}

MovableAgent::~MovableAgent() {
	// TODO Auto-generated destructor stub
}

double MovableAgent::getSpeed() const {
	return (m_speed);
}

void MovableAgent::setSpeed(double val) {
	m_speed = val;
}

string MovableAgent::toString() {
	ostringstream ss;
	ss << LocatableAgent::toString() << left << setw(15) << m_speed;
	return (ss.str());
}

