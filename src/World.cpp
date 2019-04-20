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
#include <RandomNumberGenerator.h>
#include <sys/_types/_time_t.h>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <utility>


using namespace std;
using namespace utils;

//ctor
World::World(Map* map, int numPersons, int numAntennas, int numMobilePhones) :
		m_map { map } {
	m_agentsCollection = new AgentsCollection();


//	std::random_device device;
//	m_generator.seed(device());

	vector<Person*> persons = generatePopulation(numPersons);
	for (int i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = generateAntennas(numAntennas);
	for (int i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
	}

	vector<MobilePhone*> phones = generateMobilePhones(numMobilePhones);
	for (int i = 0; i < phones.size(); i++) {
		m_agentsCollection->addAgent(phones[i]);
	}
	m_clock = new Clock();
}

//dtor
World::~World() {
	cout << "End of simulation!" << endl;
}

void World::runSimulation(string personsFile, string antennasFile) {
	ofstream pFile, aFile;
	try {
		pFile.open(personsFile, ios::out);
		aFile.open(antennasFile, ios::out);
	}
	catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		throw e;
	}

	//dumping antennas positions
	auto itr2 = m_agentsCollection->getAgentListByType(typeid(Antenna).name());
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = dynamic_cast<Antenna*>(it->second);
		aFile << a->dumpLocation(nullptr) << endl;
	}

	time_t tt = getClock()->realTime();
	pFile << "Simulation started at " << ctime(&tt) << endl;
	m_clock->setInitialTime(0);
	m_clock->setIncrement(1);
	m_clock->setFinalTime(100);

	for (unsigned t = m_clock->getInitialTime(); t < m_clock->getFinalTime(); t = m_clock->tick()) {

		//iterate over all persons and call move()
		auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = dynamic_cast<Person*>(it->second);
			pFile << p->dumpLocation(m_clock) << p->dumpDevices() << endl;
			p->move(RandomNumberGenerator::instance()->getGenerator());
		}
	}

	try {
		pFile.close();
		aFile.close();
	}
	catch (std::ofstream::failure& e) {
		cerr << "Error closing output files!" << endl;
		throw e;
	}
}


unsigned int World::getCurrentTime() {
	return (m_clock->getCurrentTime());
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
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons);
	// temporary
	double* speeds = RandomNumberGenerator::instance()->generateNormal2Double(0.5, 0.1, 2.5, 0.1, numPersons);

	for (auto i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();

		Person* p = new Person(getMap(), id, positions[i], speeds[i], int_distribution(RandomNumberGenerator::instance()->getGenerator()));
		result.push_back(p);
	}
	return (result);
}

vector<Antenna*> World::generateAntennas(int numAntennas) {
	vector<Antenna*> result;

	unsigned id;
	double power = 100;
	double attFactor = 2;
	int maxConnections = 100;

	vector<Point*> positions = utils::generatePoints(getMap(), numAntennas);
	for (auto i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		Antenna* p = new Antenna(getMap(), id, positions[i], attFactor, power, maxConnections);
		result.push_back(p);
	}
	return (result);
}

vector<MobilePhone*> World::generateMobilePhones(int numMobilePhones) {
	vector<MobilePhone*> result;
	unsigned id;
	for (auto i = 0; i < numMobilePhones; i++) {
		id = IDGenerator::instance()->next();
		MobilePhone* p = new MobilePhone(getMap(), id, nullptr, nullptr);
		result.push_back(p);
	}
	return (result);
}
