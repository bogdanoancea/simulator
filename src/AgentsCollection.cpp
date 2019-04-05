/*
 * Data simulator for mobile phone network events
 *
 * AgentsCollection.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "AgentsCollection.h"
#include <algorithm>

AgentsCollection::AgentsCollection() :
		m_agents { } {
}

AgentsCollection::~AgentsCollection() {
}

void AgentsCollection::addAgent(Agent a) {
	m_agents.push_back(a);
}

void AgentsCollection::deleteAgent(Agent a) {

	vector<Agent>::iterator position = std::find(m_agents.begin(), m_agents.end(),
			a);
	if (position != m_agents.end()) // == agents.end() means the element was not found
		m_agents.erase(position);
}

const vector<Agent>& AgentsCollection::getAgents() const {
	return m_agents;
}

void AgentsCollection::setAgents(const vector<Agent>& agents) {
	m_agents = agents;
}


const World* AgentsCollection::getWorld() const {
	return m_world;
}

void AgentsCollection::setWorld(World* world) {
	m_world = world;
}
