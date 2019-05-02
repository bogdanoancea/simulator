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
#include <EMField.h>
#include <ctime>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <AntennaType.h>
#include <tinyxml2.h>

using namespace std;
using namespace utils;
using namespace tinyxml2;

//ctor
World::World(Map* map, int numPersons, int numAntennas, int numMobilePhones) :
		m_map { map } {
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock();
	vector<Person*> persons = generatePopulation(numPersons);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = generateAntennas(numAntennas);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	vector<MobilePhone*> phones = generateMobilePhones(numMobilePhones);
	for (unsigned long i = 0; i < phones.size(); i++) {
		m_agentsCollection->addAgent(phones[i]);
	}
}

World::World(Map* map, int numPersons, char* configAntennasFile, int numMobilePhones) :
		m_map { map } {

	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock();
	vector<Person*> persons = generatePopulation(numPersons);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	vector<MobilePhone*> phones = generateMobilePhones(numMobilePhones);
	for (unsigned long i = 0; i < phones.size(); i++) {
		m_agentsCollection->addAgent(phones[i]);
	}
}

//dtor
World::~World() {
	delete m_clock;
	delete m_agentsCollection;

	cout << "End of simulation!" << endl;
}

void World::runSimulation(string personsFile, string antennasFile) noexcept(false) {
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
	m_clock->setFinalTime(Constants::SIMULATION_TIME);
	auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());

	for (unsigned t = m_clock->getInitialTime(); t < m_clock->getFinalTime(); t = m_clock->tick()) {
		//iterate over all persons and call move()
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = dynamic_cast<Person*>(it->second);
			pFile << p->dumpLocation(m_clock) << p->dumpDevices() << endl;
			p->move();

		}
	}

	//test EMField
//	Coordinate c = Coordinate(0.533584, 10);
//	Point* p = getMap()->getGlobalFactory()->createPoint(c);
//	pair<Antenna*, double> x = EMField::instance()->computeMaxPower(p);
//	int id = x.first->getId();
//	cout << "most powerful antenna in " << p->toText() << " is: " << id << " with power : " << x.second << endl;

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
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons);
	// temporary
	double* speeds = RandomNumberGenerator::instance()->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = RandomNumberGenerator::instance()->generateInt(1, 100, numPersons);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		Person* p = new Person(getMap(), id, positions[i], m_clock, speeds[i], ages[i]);
		result.push_back(p);
	}
	delete[] speeds;
	delete[] ages;

	return (result);
}

vector<Antenna*> World::generateAntennas(int numAntennas) {
	vector<Antenna*> result;

	unsigned id;
	double power = Constants::ANTENNA_POWER;
	double attFactor = Constants::ATT_FACTOR;
	int maxConnections = Constants::MAX_CONNECTIONS;

	vector<Point*> positions = utils::generatePoints(getMap(), numAntennas);
	for (unsigned long i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		Antenna* p = new Antenna(getMap(), id, positions[i], m_clock, attFactor, power, maxConnections, AntennaType::OMNIDIRECTIONAL);
		result.push_back(p);
	}
	return (result);
}

vector<MobilePhone*> World::generateMobilePhones(int numMobilePhones) {
	vector<MobilePhone*> result;
	unsigned id;
	for (auto i = 0; i < numMobilePhones; i++) {
		id = IDGenerator::instance()->next();
		MobilePhone* p = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::POWER_THRESHOLD);
		result.push_back(p);
	}
	return (result);
}

vector<Antenna*> World::parseAntennas(char* configAntennasFile) noexcept(false) {
	vector<Antenna*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(configAntennasFile);
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for antennas ");

	XMLElement* antennasNode = doc.FirstChildElement("antennas");
	if (!antennasNode)
		throw std::runtime_error("Syntax error in the configuration file for antennas ");
	else {
		XMLElement* antennaEl = getFirstChildElement(antennasNode, "antenna");
		for (; antennaEl; antennaEl = antennaEl->NextSiblingElement()) {
			if (antennaEl && strcmp(antennaEl->Name(), "antenna")) {
				cout << "unknown element: " << antennaEl->Name() << " ignoring it" << endl;
				continue;
			}
			Antenna* a = buildAntenna(antennaEl);
			result.push_back(a);
		}
	}
	return (result);
}

XMLNode* World::getNode(XMLElement* el, const char* name) {
	XMLNode* n = el->FirstChildElement(name)->FirstChild();
	if (!n)
		throw std::runtime_error("Syntax error in the configuration file for antennas ");
	return (n);
}

XMLElement* World::getFirstChildElement(XMLElement* el, const char* name) {
	XMLElement* n = el->FirstChildElement(name);
	if (!n)
		throw std::runtime_error("Syntax error in the configuration file for antennas ");
	return (n);
}

Antenna* World::buildAntenna(XMLElement* antennaEl) noexcept(false) {
	Antenna* a = nullptr;
	unsigned id;
	XMLNode* n = getNode(antennaEl, "maxconnections");
	int maxConn = atoi(n->ToText()->Value());
	n = getNode(antennaEl, "power");
	double power = atof(n->ToText()->Value());
	n = getNode(antennaEl, "attenuationfactor");
	double attentuationFactor = atof(n->ToText()->Value());
	n = getNode(antennaEl, "type");
	const char* t = n->ToText()->Value();
	AntennaType type = AntennaType::OMNIDIRECTIONAL;
	if (strcmp(t, "directional"))
		type = AntennaType::DIRECTIONAL;
	n = getNode(antennaEl, "x");
	double x = atof(n->ToText()->Value());
	n = getNode(antennaEl, "y");
	double y = atof(n->ToText()->Value());
	Coordinate c = Coordinate(x, y);
	Point* p = getMap()->getGlobalFactory()->createPoint(c);
	id = IDGenerator::instance()->next();

	a = new Antenna(getMap(), id, p, m_clock, attentuationFactor, power, maxConn, type);

	return (a);
}
