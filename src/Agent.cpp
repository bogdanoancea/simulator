/*
 * Data simulator for mobile phone network events
 *
 * Agent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "Agent.h"
#include "World.h"

Agent::Agent(World& w, long id) :
		m_world { w }, m_id { id } {
}

Agent::~Agent() {
}


bool Agent::operator==(const Agent& a) {
	return true;
}

World& Agent::getWorld() const {
	return m_world;
}

void Agent::setWorld(World& world) {
	m_world = world;
}
