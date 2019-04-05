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

#include "Agent.h"
#include <vector>

using namespace std;

class AgentsCollection {
	public:
		/** Default constructor */
		AgentsCollection();

		/** Default destructor */
		virtual ~AgentsCollection();
		void addAgent(Agent);
		void deleteAgent(Agent);
		void performAction();

	private:
		vector<Agent> agents;
};

#endif // AGENTSCOLLECTION_H
