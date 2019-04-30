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

class AgentsCollection {
	public:
		/** Default constructor */
		AgentsCollection();

		/** Default destructor */
		virtual ~AgentsCollection();

		void addAgent(Agent* a);

		Agent* deleteAgent(Agent* a);

		void performAction();

		Agent* getAgent(int id);

		std::pair<um_iterator, um_iterator> getAgentListByType(const string& agentType);

		um_iterator end() {
			return (m_agents2.end());
		}

		um_iterator begin() {
			return (m_agents2.begin());
		}

		unsigned long size() {
			return (m_agents2.size());
		}

	private:
		unordered_multimap<string, Agent*> m_agents2;
};

#endif // AGENTSCOLLECTION_H
