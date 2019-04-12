/*
 * Data simulator for mobile phone network events
 *
 * World.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */


#include <geos/geom/Point.h>
#include <IDGenerator.h>
#include <Map.h>
#include <Utils.h>
#include <World.h>
#include <iostream>


using namespace std;

//ctor
World::World(Map* map, int numPersons) :
		m_map { map } {
	m_agentsCollection = new AgentsCollection();

	std::random_device device;
	std::mt19937 m_generator(device());

	vector<Person*> persons = generatePopulation(numPersons);
	for (int i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}
}


//dtor
World::~World() {
	cout << "End of simulation!" << endl;
}

void World::runSimulation() {
	cout << "The show begins in few seconds ..." << endl;

}

unsigned int World::getCurrentTime() {
	return m_clock->getCurrentTime();
}

AgentsCollection* World::getAgents() const {
	return m_agentsCollection;
}

void World::setAgents(AgentsCollection* agents) {
	m_agentsCollection = agents;
}

Clock* World::getClock() const {
	return m_clock;
}

void World::setClock(Clock* clock) {
	m_clock = clock;
}

Map* World::getMap() const {
	return m_map;
}

void World::setMap(Map* map) {
	m_map = map;
}

vector<Person*> World::generatePopulation(int numPersons) {
	vector<Person*> result;

	unsigned id;
	uniform_int_distribution<int> int_distribution(1, 100);
	vector<Point*> positions = generatePoints(getMap(), m_generator, numPersons);
	for (auto i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();

		Person* p = new Person(getMap(), id, positions[i], int_distribution(m_generator));
		result.push_back(p);
	}
	return (result);
}
