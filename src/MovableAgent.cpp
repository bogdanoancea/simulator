/*
 * Data simulator for mobile phone network events
 *
 * MovableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */


#include "MovableAgent.h"

MovableAgent::MovableAgent(Map* m, long id, Point* initialPosition) :
		LocatableAgent(m, id, initialPosition), m_speed { 0.0 } {
	// TODO Auto-generated constructor stub
}

MovableAgent::~MovableAgent() {
	// TODO Auto-generated destructor stub
}

double MovableAgent::getSpeed() const {
	return m_speed;
}

void MovableAgent::setSpeed(double val) {
	m_speed = val;
}

