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

#include <Antenna.h>
#include <Person.h>
#include <random>
#include <vector>
#include <AgentsCollection.h>
#include <Clock.h>
#include <MobilePhone.h>



using namespace std;

class World {
	public:
		/** Default constructor */
		World(Map* map, int numPersons, int numAntennas, int numMobilePhones);

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
		std::mt19937 m_generator;
		vector<Person*> generatePopulation(int numPersons);
		vector<Antenna*> generateAntennas(int numAntennas);
		vector<MobilePhone*> generateMobilePhones(int numMobilePhones);

};

#endif // WORLD_H
