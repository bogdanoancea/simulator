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
 * AgentsCollection.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef AGENTSCOLLECTION_H
#define AGENTSCOLLECTION_H

#include <agent/Agent.h>
#include <string>
#include <unordered_map>

using namespace std;

typedef unordered_multimap<string, Agent*>::iterator um_iterator;

/**
 * This is actually a container for all the agents used for simulation. An agent could
 * be an object of one the the derived classes of Agent. The Agents are kept
 * in an unordered_multimap as pairs of <string, Agent*> where the first element of the pair is
 * the name of the concrete agent (a person, a mobile device, an antenna, a mobile network operator, etc.)
 * and the second element is a pointer to the actual object (agent).
 */
class AgentsCollection {
public:

	/**
	 * The default constructor of the class.
	 */
	AgentsCollection();

	/**
	 * Default destructor: it iterates through the collection of agents and frees the
	 * memory allocated for each agent in the collection.
	 */
	virtual ~AgentsCollection();

	/**
	 * Adds a new Agent object to the collection. For performance reasons the AgentsCollection class keep
	 * only a pointer to actual agents (objects).
	 * @param ag a pointer to the object (one of the derived classes of the Agent) to be added to the collection.
	 */
	void addAgent(Agent* ag);

	/**
	 * Removes an object from the collection.
	 * @param ag a pointer to the object to be removed from the collection.
	 * @return a pointer to the object removed from the collection
	 */
	Agent* deleteAgent(Agent* ag);


	/**
	 * Returns a pointer to an agent from the collection. The agent/object is identified by its
	 * id which is unique throughout the collection.
	 * @param id the id of the agent to be returned.
	 * @return a pointer to the agent with the id equals to the parameter <tt>id</tt>.
	 * If there is no agent with the provided id, this method returns <tt>nullptr</tt>.
	 */
	Agent* getAgent(const unsigned long id) const;

	/**
	 * This method is used to retrieve a subset of agents with a certain type: persons, mobile phones etc.
	 * @param agentType is the name of the class of agents that the user wants to retrieve from the collection of all agents.
	 * @return a std::pair of iterators of type unordered_multimap<string, Agent*>::iterator
	 * that can be used to iterate through to subset of the agents.
	 */
	pair<um_iterator, um_iterator> getAgentListByType(const string& agentType);

	/**
	 * Iterator to the past-the-end object of the collection. It does not point to any agent,
	 * and thus shall not be dereferenced.
	 * @return an iterator referring to the past-the-end element in the agents container. If the container
	 * is empty, this function returns the same as AgentsCollection::begin().
	 */
	um_iterator end();

	/**
	 * Iterator to the first object of the container.
	 * @return a random access iterator pointing to the first element (agent) in the container.
	 * If the container is empty, the returned iterator value shall not be dereferenced.
	 */
	um_iterator begin();

	/**
	 * Returns the number of elements in the container.
	 * @return the number of elements in the container. This is the number of actual objects
	 * held in the container,which is not necessarily equal to its storage capacity.
	 */
	unsigned long size();

	/**
	 * Prints out all agents in the collection calling toString() method for each agent.
	 */
	void printAgents();

private:
	unordered_multimap<string, Agent*> m_agents;
};

#endif // AGENTSCOLLECTION_H
