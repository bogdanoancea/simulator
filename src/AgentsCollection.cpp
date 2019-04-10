/*
 * Data simulator for mobile phone network events
 *
 * AgentsCollection.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <AgentsCollection.h>
#include <algorithm>
#include <iterator>

AgentsCollection::AgentsCollection() {
}

AgentsCollection::~AgentsCollection() {
}

void AgentsCollection::addAgent(Agent* a) {
	m_agents.push_back(a);
}

Agent* AgentsCollection::deleteAgent(Agent* a) {
	Agent* result = nullptr;
	vector<Agent*>::iterator position = std::find(m_agents.begin(),
			m_agents.end(), a);
	if (position != m_agents.end()) { // == agents.end() means the element was not found
		result = *position;
		m_agents.erase(position);
	}
	return result;
}

//const vector<Agent*>& AgentsCollection::getAgents() const {
//	return m_agents;
//}
//
//void AgentsCollection::setAgents(vector<Agent*> agents) {
//	m_agents = std::move(agents);
//}


