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
	m_agents2.insert(std::pair<string, Agent*>(typeid(*a).name(), a));
}


//TODO equals between two agents to be implmented
Agent* AgentsCollection::deleteAgent(Agent* a) {
	Agent* result = nullptr;
	std::pair<um_iterator, um_iterator> iterpair = m_agents2.equal_range(typeid(*a).name());
	um_iterator it = iterpair.first;
	for (; it != iterpair.second; ++it) {
	    if (it->second == a) {
			m_agents2.erase(it);
	        break;
	    }
	}
	return (result);
}


std::pair<um_iterator, um_iterator> AgentsCollection::getAgentListByType(const string& agentType) {
	return (m_agents2.equal_range(agentType));
}

Agent* AgentsCollection::getAgent(int id) {
	Agent* result = nullptr;
	for(auto& a : m_agents2){
		if(a.second->getId() == id)
			result = a.second;
	}
	return (result);
}


