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

Agent::Agent(World& w) :
		m_world { w } {
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
