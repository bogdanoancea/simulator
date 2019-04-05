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
#include <World.h>
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

		const vector<Agent>& getAgents() const;
		void setAgents(const vector<Agent>& agents);

		const World* getWorld() const;
		void setWorld(World* world);

	private:
		vector<Agent> m_agents;
		World* m_world;
};

#endif // AGENTSCOLLECTION_H
