/*
 * Data simulator for mobile phone network events
 *
 * AgentsCollection.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <AgentsCollection.h>
#include <initializer_list>
#include <iostream>
#include <utility>

AgentsCollection::AgentsCollection() {
}

AgentsCollection::~AgentsCollection() {
}

void AgentsCollection::addAgent(Agent* a) {
	m_agents2.insert( { typeid(*a).name(), a });
}

Agent* AgentsCollection::deleteAgent(Agent* a) {
	Agent* result = nullptr;



	std::pair<umit, umit> iterpair = m_agents2.equal_range(typeid(*a).name());
	umit it = iterpair.first;
	for (; it != iterpair.second; ++it) {
	    if (it->second == a) {
	        m_agents2.erase(it);
	        break;
	    }
	}


	return result;
}

umit AgentsCollection::getAgentListByType(string agentType) {
	return m_agents2.find(agentType);
}

//const vector<Agent*>& AgentsCollection::getAgents() const {
//	return m_agents;
//}
//
//void AgentsCollection::setAgents(vector<Agent*> agents) {
//	m_agents = std::move(agents);
//}


