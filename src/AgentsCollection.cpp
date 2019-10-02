/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
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

std::pair<um_iterator, um_iterator> AgentsCollection::getAgentListByType(const string& agentType) {
	return (m_agents.equal_range(agentType));
}

Agent* AgentsCollection::getAgent(const unsigned long id) const {
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

void AgentsCollection::printAgents() {
//not yet implemented
}

