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
	for (auto& a : m_agents) {
		delete a.second;
	}
}

void AgentsCollection::addAgent(Agent* a) {
	m_agents.insert(std::pair<string, Agent*>(typeid(*a).name(), a));
}


Agent* AgentsCollection::deleteAgent(Agent* a) {
	Agent* result = nullptr;
	std::pair<um_iterator, um_iterator> iterpair = m_agents.equal_range(typeid(*a).name());
	um_iterator it = iterpair.first;
	for (; it != iterpair.second; ++it) {
		if (it->second == a) {
			m_agents.erase(it);
			break;
		}
	}
	return (result);
}

std::pair<um_iterator, um_iterator> AgentsCollection::getAgentListByType(const string& agentType)  {
	return (m_agents.equal_range(agentType));
}

Agent* AgentsCollection::getAgent(unsigned long id) const {
	Agent* result = nullptr;
	for (auto& a : m_agents) {
		if (a.second->getId() == id)
			result = a.second;
	}
	return (result);
}

um_iterator AgentsCollection::begin() {
	return (m_agents.begin());
}

um_iterator AgentsCollection::end() {
	return (m_agents.end());
}

unsigned long AgentsCollection::size() {
	return (m_agents.size());
}
