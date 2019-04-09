/*
 * Data simulator for mobile phone network events
 *
 * World.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */


#ifndef WORLD_H
#define WORLD_H

//#include "AgentsCollection.h"
#include "Map.h"
#include "Clock.h"

using namespace std;

class World {
	public:
		/** Default constructor */
		World();

		/** Default destructor */
		virtual ~World();

		void runSimulation();

		void dumpState();

		unsigned int getCurrentTime();

//		AgentsCollection* getAgents() const;
//		void setAgents(AgentsCollection* agents);

		Clock* getClock() const;
		void setClock(Clock* clock);

		Map* getMap() const;
		void setMap(Map* map);

	private:
		//AgentsCollection* m_agents;
		Map* m_map;
		Clock* m_clock;

};

#endif // WORLD_H
