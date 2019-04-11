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

#include <Person.h>
#include <random>
#include <vector>
#include <AgentsCollection.h>
#include <Clock.h>;



using namespace std;

class World {
	public:
		/** Default constructor */
		World(Map* map, int numPersons);

		/** Default destructor */
		virtual ~World();

		void runSimulation();

		void dumpState();

		unsigned int getCurrentTime();

		AgentsCollection* getAgents() const;
		void setAgents(AgentsCollection* agents);

		Clock* getClock() const;
		void setClock(Clock* clock);

		Map* getMap() const;
		void setMap(Map* map);

	private:

		Map* m_map;

		AgentsCollection* m_agentsCollection;
		Clock* m_clock;
		default_random_engine m_generator;
		vector<Person*> generatePopulation(int numPersons);

};

#endif // WORLD_H
