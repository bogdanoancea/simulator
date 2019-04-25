/*
 * Data simulator for mobile phone network events
 *
 * Agent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <Agent.h>


Agent::Agent(Map* m, long id, Clock* clock) :
		m_id { id } {
	m_map = m;
	m_clock = clock;
}

Agent::~Agent() {
}


bool Agent::operator==(const Agent& a) {
	return this->m_id == a.getId();
}

Map* Agent::getMap() const {
	return m_map;
}
void Agent::setMap(Map* map) {
	m_map = map;
}

Clock* Agent::getClock() const {
	return m_clock;
}
