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
#include <sstream>
#include <AntennaType.h>
#include <tinyxml2.h>
#include <cstring>

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

World::World(Map* map, int numPersons, const string& configAntennasFile, int numMobilePhones) :
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

World::World(Map* map, const string& personsFileName, const string& configAntennasFile, int numMobilePhones) :
		m_map { map } {

	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock();
	vector<Person*> persons = parsePersons(personsFileName);
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

void World::runSimulation(string personsFile, string& antennasFile) noexcept(false) {
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
		aFile << a->dumpLocation() << endl;
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
			pFile << p->dumpLocation() << p->dumpDevices() << endl;
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

vector<Person*> World::generatePopulation(unsigned long numPersons) {
	vector<Person*> result;

	unsigned id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons);
	// temporary
	double* speeds = RandomNumberGenerator::instance()->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = RandomNumberGenerator::instance()->generateUniformInt(1, 100, numPersons);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		Person* p = new Person(getMap(), id, positions[i], m_clock, speeds[i], ages[i], Person::Gender::MALE);
		result.push_back(p);
	}
	delete[] speeds;
	delete[] ages;

	return (result);
}

vector<Antenna*> World::generateAntennas(unsigned long numAntennas) {
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

vector<Antenna*> World::parseAntennas(const string& configAntennasFile) noexcept(false) {
	vector<Antenna*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(configAntennasFile.c_str());
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

vector<Person*> World::parsePersons(const string& personsFileName) noexcept(false) {
	vector<Person*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(personsFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for persons ");

	XMLElement* personsEl = doc.FirstChildElement("persons");
	if (!personsEl)
		throw std::runtime_error("Syntax error in the configuration file for persons ");
	else {
		XMLNode* numNode = getNode(personsEl, "num_persons");
		int numPersons = atoi(numNode->ToText()->Value());
		XMLNode* minAgeNode = getNode(personsEl, "min_age");
		unsigned int min_age = atoi(minAgeNode->ToText()->Value());
		XMLNode* maxAgeNode = getNode(personsEl, "max_age");
		unsigned int max_age = atoi(maxAgeNode->ToText()->Value());

		XMLElement* ageDistribEl = getFirstChildElement(personsEl, "age_distribution");
		XMLNode* distribTypeNode = getNode(ageDistribEl, "type");
		const char* distrib = distribTypeNode->ToText()->Value();

		if (strcmp(distrib, "normal") && strcmp(distrib, "uniform"))
			throw std::runtime_error("Unknown age distribution for population!");

		Person::AgeDistributions d;
		vector<double> params;
		if (!strcmp(distrib, "normal")) {
			XMLNode* meanNode = getNode(ageDistribEl, "mean");
			double mean_age = atof(meanNode->ToText()->Value());
			XMLNode* sdNode = getNode(ageDistribEl, "sd");
			double sd = atof(sdNode->ToText()->Value());
			d = Person::AgeDistributions::NORMAL;
			params.push_back(mean_age);
			params.push_back(sd);
			params.push_back(min_age);
			params.push_back(max_age);
		}
		else if (!strcmp(distrib, "uniform")) {
			d = Person::AgeDistributions::UNIFORM;
			params.push_back(min_age);
			params.push_back(max_age);
		}
		XMLNode* maleShareNode = getNode(personsEl, "male_share");
		double male_share = atof(maleShareNode->ToText()->Value());

		XMLNode* prob_mobilePhoneNode = getNode(personsEl, "prob_mobile_phone");
		double probMobilePhone = atof(prob_mobilePhoneNode->ToText()->Value());

		XMLNode* speed_walkNode = getNode(personsEl, "speed_walk");
		double speed_walk = atof(speed_walkNode->ToText()->Value());

		XMLNode* speed_carNode = getNode(personsEl, "speed_car");
		double speed_car = atof(speed_carNode->ToText()->Value());

		result = generatePopulation(numPersons, params, d, male_share, speed_walk, speed_car);
	}
	return (result);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution,
		double male_share, double speed_walk, double speed_car) {

	vector<Person*> result;

	unsigned id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons);

	int* walk_car = RandomNumberGenerator::instance()->generateBinomialInt(1, 0.5, numPersons);
	int sum = 0;
	for (unsigned long i = 0; i < numPersons; i++)
		sum += walk_car[i];

	int* gender = RandomNumberGenerator::instance()->generateBinomialInt(1, male_share, numPersons);

	double* speeds_walk = RandomNumberGenerator::instance()->generateNormalDouble(speed_walk, 0.1 * speed_walk, numPersons - sum);
	double* speeds_car = RandomNumberGenerator::instance()->generateNormalDouble(speed_car, 0.1 * speed_car, sum);

	double* ages;
	if (age_distribution == Person::AgeDistributions::UNIFORM)
		ages = RandomNumberGenerator::instance()->generateUniformDouble(params[0], params[1], numPersons);
	else if (age_distribution == Person::AgeDistributions::NORMAL) {
		ages = RandomNumberGenerator::instance()->generateTruncatedNormalDouble(params[2], params[3], params[0], params[1], numPersons);
	}

	unsigned long cars = 0;
	unsigned long walks = 0;
	Person* p;
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		if (walk_car[i])
			p = new Person(getMap(), id, positions[i], m_clock, speeds_car[cars++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);
		else
			p = new Person(getMap(), id, positions[i], m_clock, speeds_walk[walks++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);
		result.push_back(p);
	}
	delete[] speeds_walk;
	delete[] speeds_car;
	delete[] ages;

	return (result);
}
