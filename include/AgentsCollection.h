/*
 * Data simulator for mobile phone network events
 *
 * AgentsCollection.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef AGENTSCOLLECTION_H
#define AGENTSCOLLECTION_H

#include <Agent.h>
#include <typeinfo>
#include <unordered_map>
#include <vector>

using namespace std;

typedef unordered_multimap<string, Agent*>::iterator um_iterator;

/**
 * This is actually a container for all the agents used for simulation. An agent could be an object of one the the derived classes of Agent.
 * The Agents are kept in an unordered_multimap as pairs <string, Agent*> where the first element of the pair is
 * the name of the concrete agent (a person, a mobile device, an antenna etc)
 * and the second element is a pointer to the actual object (agent).
 */
class AgentsCollection {
public:

	/**
	 * The default constructor of the class.
	 */
	AgentsCollection();

	/**
	 * Default destructor: it iterates through the collection of agents and frees the memory allocated for each agent in the collection.
	 */
	virtual ~AgentsCollection();

	/**
	 * Adds a new Agent to the collection. For performance reasons the AgentsCollection class keep only pointer to actual agents (objects).
	 * @param a a pointer to the object (one of the derived classes of the Agent) to be added to the collection.
	 */
	void addAgent(Agent* a);

	/**
	 * Removes an object from the collection.
	 * @param a a pointer to the object to be removed from the collection.
	 * @return
	 */
	Agent* deleteAgent(Agent* a);

	/**
	 * Returns a pointer to an agent from the collection. The agent/object is identified by its id.
	 * @param id of the agent to be returned.
	 * @return a pointer to the agent with the id equal to the @param id. If there is no agent with the provided id,
	 * this method returns nullptr.
	 */
	Agent* getAgent(unsigned long id) const;

	/**
	 * This method is used to get a subset with a certain type of agents: persons, mobile phone etc.
	 *
	 * @param agentType is the name of the class of agents that the user wants to retrieve from the collection of all agents.
	 * @return a std::pair of iterators of type unordered_multimap<string, Agent*>::iterator
	 * that can be used to iterate through to subset of the agents.
	 */
	pair<um_iterator, um_iterator> getAgentListByType(const string& agentType) const;

	/**
	 * Iterator to the past-the-end of the collection. It does not point to any agent, and thus shall not be dereferenced.
	 * @return an iterator referring to the past-the-end element in the agents container. If the container
	 * is empty, this function returns the same as AgentsColletion::begin().
	 */
	um_iterator end();

	/**
	 * Iterator to the first agent of the container.
	 * @return a random access iterator pointing to the first element (agent) in the container. If the container is empty, the returned
	 * iterator value shall not be dereferenced.
	 */
	um_iterator begin();

	/**
	 * @return the number of elements in the container. This is the number of actual objects held in the container,
	 * which is not necessarily equal to its storage capacity.
	 */
	unsigned long size();

private:
	unordered_multimap<string, Agent*> m_agents;
};

#endif // AGENTSCOLLECTION_H
