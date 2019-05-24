/*
 * Data simulator for mobile phone network events
 *
 * Agent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <Agent.h>

Agent::Agent(const Map* m, unsigned long id, Clock* clock) :
		m_id { id } {
	m_map = m;
	m_clock = clock;
}

Agent::~Agent() {
}

bool Agent::operator==(const Agent& a) {
	return this->m_id == a.getId();
}

const Map* Agent::getMap() const {
	return (m_map);
}

void Agent::setMap(const Map* map) {
	m_map = map;
}

Clock* Agent::getClock() const {
	return m_clock;
}

unsigned long Agent::getId() const {
	return m_id;
}
