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
#include <tinyxml2.h>


using namespace std;
using namespace tinyxml2;

class World {
	public:
		/** Default constructor */
		World(Map* map, int numPersons, int numAntennas, int numMobilePhones);

		World(Map* map, int numPersons, const string& configAntennasFile, int numMobilePhones) noexcept(false);

		World(Map* map, const string& personsFileName, const string& configAntennasFile, int numMobilePhones) noexcept(false);

		/** Default destructor */
		virtual ~World();

		void runSimulation(string personsFile, string& antennasFile) noexcept(false);

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
		vector<Person*> generatePopulation(unsigned long numPersons);
		vector<Person*> generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution,
				double male_share, double speed_walk, double speed_car);
		vector<Antenna*> generateAntennas(unsigned long numAntennas);
		vector<Antenna*> parseAntennas(const string& configAntennasFile) noexcept(false);
		vector<Person*> parsePersons(const string& personsFileName) noexcept(false);

		vector<MobilePhone*> generateMobilePhones(int numMobilePhones);
		XMLNode* getNode(XMLElement* el, const char* name) noexcept(false);
		XMLElement* getFirstChildElement(XMLElement* el, const char* name) noexcept(false);
		Antenna* buildAntenna(XMLElement* amtennaEl) noexcept(false);
};

#endif // WORLD_H
