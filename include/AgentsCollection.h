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
#include <vector>

using namespace std;

class AgentsCollection {
	public:
		/** Default constructor */
		AgentsCollection();

		/** Default destructor */
		virtual ~AgentsCollection();

		void addAgent(Agent* a);
		Agent* deleteAgent(Agent* a);
		void performAction();
		Agent* getAgent(int i) {
			return (m_agents[i]);
		}


	private:
		vector<Agent*> m_agents;
};

#endif // AGENTSCOLLECTION_H
