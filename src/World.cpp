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
#include <numeric>
#include <map>
#include <ctime>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <AntennaType.h>
#include <cstring>
#include <HoldableAgent.h>
#include <MovementType.h>
#include <TinyXML2.h>
#include <stdlib.h>

using namespace std;
using namespace utils;
using namespace tinyxml2;

//ctor
World::World(Map* map, int numPersons, int numAntennas, int numMobilePhones) :
		m_map { map } {
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock();
	m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP;
	m_connType = HoldableAgent::CONNECTION_TYPE::USING_POWER;
	m_gridFilename = Constants::GRID_FILE_NAME;
	m_personsFilename = Constants::PERSONS_FILE_NAME;
	m_antennasFilename = Constants::ANTENNAS_FILE_NAME;
	//m_probFilename = Constants::PROB_FILE_NAME_PREFIX;
	//m_numMNO = 0;
	m_probSecMobilePhone = 0.0;
	m_seed = -1;
	vector<Person*> persons = generatePopulation(numPersons);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = generateAntennas(numAntennas);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	vector<MobilePhone*> phones = generateMobilePhones(numMobilePhones, m_connType);
	for (unsigned long i = 0; i < phones.size(); i++) {
		m_agentsCollection->addAgent(phones[i]);
	}
}

World::World(Map* mmap, const string& configPersonsFileName, const string& configAntennasFile, const string& configSimulationFileName, const string& probabilitiesFileName) :
		m_map { mmap } {

	//m_numMNO = 0;
	m_probSecMobilePhone = 0.0;
	m_seed = -1;
	vector<MobileOperator*> mnos = parseSimulationFile(configSimulationFileName);
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock(m_startTime, m_endTime, m_timeIncrement);
	string probsPrefix = parseProbabilities(probabilitiesFileName);

	for (unsigned long i = 0; i < mnos.size(); i++) {
		m_agentsCollection->addAgent(mnos[i]);
		m_probFilenames.insert(pair<const unsigned long, string>(mnos[i]->getId(), probsPrefix + "_" + mnos[i]->getMNOName() + ".csv"));
	}

	vector<Person*> persons = parsePersons(configPersonsFileName, mnos);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile, mnos);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
		const unsigned long id  = antennas[i]->getMNO()->getId();
		Agent* ag = m_agentsCollection->getAgent(id);
		MobileOperator* mo = dynamic_cast<MobileOperator*>(ag);
		ofstream& f = mo->getAntennaCellsFile();
		f << antennas[i]->getId() << "," << antennas[i]->dumpCell();
	}
}

//dtor
World::~World() {
	delete m_clock;
	delete m_agentsCollection;

	cout << "End of simulation!" << endl;
}

void World::runSimulation() noexcept(false) {
	ofstream pFile, aFile;
	try {
		pFile.open(m_personsFilename, ios::out);
		aFile.open(m_antennasFilename, ios::out);
	} catch (ofstream::failure& e) {
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
	cout << "Simulation started at " << ctime(&tt) << endl;

	auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());

	RandomNumberGenerator* r = RandomNumberGenerator::instance();
	r->setSeed(time(0));
	for (unsigned long t = m_clock->getInitialTime(); t < m_clock->getFinalTime(); t = m_clock->tick()) {
		//iterate over all persons and call move()
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = dynamic_cast<Person*>(it->second);
			pFile << p->dumpLocation() << p->dumpDevices() << endl;
			p->move(m_mvType);
		}
	}

	tt = getClock()->realTime();
	cout << "Simulation ended at " << ctime(&tt) << endl;

	try {
		pFile.close();
		aFile.close();
	} catch (std::ofstream::failure& e) {
		cerr << "Error closing output files!" << endl;
		throw e;
	}
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

const string& World::getGridFilename() const {
	return m_gridFilename;
}

vector<Person*> World::generatePopulation(unsigned long numPersons) {
	vector<Person*> result;

	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, m_seed);
// temporary
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_seed);
	double* speeds = random_generator->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = random_generator->generateUniformInt(1, 100, numPersons);
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

	unsigned long id;
	double power = Constants::ANTENNA_POWER;
	double attFactor = Constants::ATT_FACTOR;
	int maxConnections = Constants::MAX_CONNECTIONS;
	double smid = Constants::S_MID;
	double ssteep = Constants::S_STEEP;

	vector<Point*> positions = utils::generatePoints(getMap(), numAntennas, m_seed);
	for (unsigned long i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		Antenna* p = new Antenna(getMap(), id, positions[i], m_clock, attFactor, power, maxConnections, smid, ssteep, AntennaType::OMNIDIRECTIONAL);
		result.push_back(p);
	}
	return (result);
}

vector<MobilePhone*> World::generateMobilePhones(int numMobilePhones, HoldableAgent::CONNECTION_TYPE connType) {
	vector<MobilePhone*> result;
	unsigned long id;
	for (auto i = 0; i < numMobilePhones; i++) {
		id = IDGenerator::instance()->next();
		MobilePhone* p = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::POWER_THRESHOLD, Constants::QUALITY_THRESHOLD, connType);
		result.push_back(p);
		m_agentsCollection->addAgent(p);
	}
	return (result);
}

vector<Antenna*> World::parseAntennas(const string& configAntennasFile, vector<MobileOperator*> mnos) noexcept(false) {
	vector<Antenna*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(configAntennasFile.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for antennas ");

	XMLElement* antennasNode = doc.FirstChildElement("antennas");
	if (!antennasNode)
		throw std::runtime_error("Syntax error in the configuration file for antennas ");
	else {
		XMLElement* antennaEl = utils::getFirstChildElement(antennasNode, "antenna");
		for (; antennaEl; antennaEl = antennaEl->NextSiblingElement()) {
			if (antennaEl && strcmp(antennaEl->Name(), "antenna")) {
				cout << "unknown element: " << antennaEl->Name() << " ignoring it" << endl;
				continue;
			}
			unsigned long id = IDGenerator::instance()->next();
			Antenna* a = new Antenna(getMap(), m_clock, id, antennaEl, mnos);
			result.push_back(a);
		}
	}
	return (result);
}

vector<Person*> World::parsePersons(const string& personsFileName, vector<MobileOperator*> mnos) noexcept(false) {
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
		} else if (!strcmp(distrib, "uniform")) {
			d = Person::AgeDistributions::UNIFORM;
			params.push_back(min_age);
			params.push_back(max_age);
		} else {
			d = Person::AgeDistributions::UNIFORM;
			params.push_back(min_age);
			params.push_back(max_age);
		}

		XMLNode* maleShareNode = getNode(personsEl, "male_share");
		double male_share = atof(maleShareNode->ToText()->Value());

		XMLNode* speed_walkNode = getNode(personsEl, "speed_walk");
		double speed_walk = atof(speed_walkNode->ToText()->Value());

		XMLNode* speed_carNode = getNode(personsEl, "speed_car");
		double speed_car = atof(speed_carNode->ToText()->Value());

		result = generatePopulation(numPersons, params, d, male_share, mnos, speed_walk, speed_car);
	}
	return (result);
}

unsigned long World::getGridDimTileX() const {
	return m_GridDimTileX;
}

unsigned long World::getGridDimTileY() const {
	return m_GridDimTileY;
}

PriorType World::getPrior() const {
	return m_prior;
}

//unsigned int World::getNumMno() const {
//	return m_numMNO;
//}
//
//void World::setNumMno(unsigned int numMno) {
//	m_numMNO = numMno;
//}

unsigned World::getSeed() const {
	return m_seed;
}

void World::setSeed(unsigned seed) {
	m_seed = seed;
}

vector<MobileOperator*> World::parseSimulationFile(const string& configSimulationFileName) noexcept(false) {
	XMLDocument doc;
	vector<MobileOperator*> result;
	XMLError err = doc.LoadFile(configSimulationFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for simulation ");

	XMLElement* simEl = doc.FirstChildElement("simulation");
	if (!simEl)
		throw std::runtime_error("Syntax error in the configuration file for simulation ");
	else {
		XMLNode* sTNode = getNode(simEl, "start_time");
		if (sTNode)
			m_startTime = atol(sTNode->ToText()->Value());
		else
			m_startTime = Constants::START_TIME;

		XMLNode* eTNode = getNode(simEl, "end_time");
		if (eTNode)
			m_endTime = atol(eTNode->ToText()->Value());
		else
			m_endTime = Constants::END_TIME;

		XMLNode* iTNode = getNode(simEl, "time_increment");
		if (iTNode)
			m_timeIncrement = atol(iTNode->ToText()->Value());
		else
			m_timeIncrement = Constants::INCREMENT_TIME;

		unsigned numMNO = 0;
		XMLElement* mnoEl = utils::getFirstChildElement(simEl, "mno");
		if (mnoEl) {
			for (; mnoEl; mnoEl = mnoEl->NextSiblingElement("mno")) {
				numMNO++;
				XMLNode* n = getNode(mnoEl, "mno_name");
				const char* name = n->ToText()->Value();
				n = getNode(mnoEl, "prob_mobile_phone");
				const double prob = atof(n->ToText()->Value());
				unsigned long id = IDGenerator::instance()->next();
				MobileOperator* mo = new MobileOperator(getMap(), id, m_clock, name, prob);
				result.push_back(mo);
			}
		}
		if (numMNO > 2)
			throw std::runtime_error("Maximum 2 MNOs are supported!");

		m_probSecMobilePhone = Constants::PROB_SECOND_MOBILE_PHONE;
		XMLNode* prob_sec_mobilePhoneNode = getNode(simEl, "prob_sec_mobile_phone");
		if (prob_sec_mobilePhoneNode)
			m_probSecMobilePhone = atof(prob_sec_mobilePhoneNode->ToText()->Value());

		XMLNode* mvNode = getNode(simEl, "movement_type");
		if (mvNode) {
			if (!strcmp(mvNode->ToText()->Value(), "random_walk_closed_map"))
				m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP;
			else if (!strcmp(mvNode->ToText()->Value(), "random_walk_closed_map_drift")) {
				m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT;
			} else
				m_mvType = MovementType::UNKNOWN;
		} else {
			m_mvType = MovementType::UNKNOWN;
		}

		XMLNode* connNode = getNode(simEl, "connection_type");
		if (connNode) {
			if (!strcmp(connNode->ToText()->Value(), "power"))
				m_connType = HoldableAgent::CONNECTION_TYPE::USING_POWER;
			else if (!strcmp(connNode->ToText()->Value(), "quality"))
				m_connType = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY;
			else
				m_connType = HoldableAgent::CONNECTION_TYPE::UNKNOWN;
		} else
			m_connType = HoldableAgent::CONNECTION_TYPE::UNKNOWN;

		XMLNode* gridNode = getNode(simEl, "grid_file");
		if (gridNode)
			m_gridFilename = gridNode->ToText()->Value();
		else
			m_gridFilename = Constants::GRID_FILE_NAME;

		XMLNode* persNode = getNode(simEl, "persons_file");
		if (persNode)
			m_personsFilename = persNode->ToText()->Value();
		else
			m_personsFilename = Constants::PERSONS_FILE_NAME;

		XMLNode* antennasNode = getNode(simEl, "antennas_file");
		if (antennasNode)
			m_antennasFilename = antennasNode->ToText()->Value();
		else
			m_antennasFilename = Constants::ANTENNAS_FILE_NAME;

		XMLNode* xTilesNode = getNode(simEl, "grid_dim_tile_x");
		if (xTilesNode)
			m_GridDimTileX = atol(xTilesNode->ToText()->Value());
		else
			m_GridDimTileX = Constants::GRID_DIM_TILE_X;

		XMLNode* yTilesNode = getNode(simEl, "grid_dim_tile_y");
		if (yTilesNode)
			m_GridDimTileY = atol(yTilesNode->ToText()->Value());
		else
			m_GridDimTileY = Constants::GRID_DIM_TILE_Y;

		XMLNode* randomSeedNode = getNode(simEl, "random_seed");
		if (randomSeedNode)
			m_seed = atoi(randomSeedNode->ToText()->Value());

	}
	return (result);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution, double male_share,
		vector<MobileOperator*> mnos, double speed_walk, double speed_car) {

	vector<Person*> result;
	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, m_seed);

	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_seed);

	double probMobilePhone = 0.0;
	double probIntersection = 1.0;
	unsigned int numMno = mnos.size();
	for (auto& n : mnos) {
		probMobilePhone += n->getProbMobilePhone();
		probIntersection *= n->getProbMobilePhone();
	}
	if (numMno > 1 && probIntersection > m_probSecMobilePhone)
		throw std::runtime_error(
				"Error specifying probabilities of having mobile phones: the probability of having a second mobile phone should be greater than or equal to the product of the probabilities of having mobile phones at each MNO");

	int* walk_car = random_generator->generateBernoulliInt(0.5, numPersons);

	int* phone1 = nullptr; //RandomNumberGenerator::instance()->generateBernoulliInt(probMobilePhones[0].second, numPersons);
	int* phone2 = nullptr; //new int[numPersons] { 0 }; //RandomNumberGenerator::instance()->generateBernoulliInt(probSecMobilePhone, numPersons);
	if (numMno == 1) {
		phone1 = random_generator->generateBernoulliInt(mnos[0]->getProbMobilePhone(), numPersons);
		for (unsigned long j = 0; j < numPersons; j++) {
			if (phone1[j] == 1)
				phone1[j] += random_generator->generateBernoulliInt(m_probSecMobilePhone);
		}
	} else if (numMno == 2) {
		double pSecPhoneDiffMNO = probIntersection;
		double pSecPhoneMNO1 = (m_probSecMobilePhone - pSecPhoneDiffMNO) / 2.0;
		double pSecPhoneMNO2 = pSecPhoneMNO1;

		double pOnePhoneMNO1 = mnos[0]->getProbMobilePhone() - pSecPhoneMNO1 - pSecPhoneDiffMNO;
		double pOnePhoneMNO2 = mnos[1]->getProbMobilePhone() - pSecPhoneMNO2 - pSecPhoneDiffMNO;

		phone1 = random_generator->generateBernoulliInt(pOnePhoneMNO1, numPersons);
		phone2 = random_generator->generateBernoulliInt(pOnePhoneMNO2, numPersons);
		for (unsigned int i = 1; i < numPersons; i++) {
			if (phone1[i] == 1) {
				phone1[i] = phone1[i] + random_generator->generateBernoulliInt(pSecPhoneMNO1);
			}
			if (phone2[i] == 1) {
				phone2[i] = phone2[i] + random_generator->generateBernoulliInt(pSecPhoneMNO2);
			}
		}
	} else {
		throw std::runtime_error("Number of MNOs supported should be 1 or 2!");
	}

	int sum = 0;
	unsigned long numPhones = 0;
	for (unsigned long i = 0; i < numPersons; i++) {
		sum += walk_car[i];
		if (phone1 && phone2)
			numPhones += (phone1[i] + phone2[i]);
		else
			numPhones += phone1[i];
	}

	int* gender = random_generator->generateBinomialInt(1, male_share, numPersons);
	double* speeds_walk = random_generator->generateNormalDouble(speed_walk, 0.1 * speed_walk, numPersons - sum);
	double* speeds_car = random_generator->generateNormalDouble(speed_car, 0.1 * speed_car, sum);

	double* ages;
	if (age_distribution == Person::AgeDistributions::UNIFORM)
		ages = random_generator->generateUniformDouble(params[0], params[1], numPersons);
	else if (age_distribution == Person::AgeDistributions::NORMAL) {
		ages = random_generator->generateTruncatedNormalDouble(params[2], params[3], params[0], params[1], numPersons);
	} else {
		ages = random_generator->generateUniformDouble(params[0], params[1], numPersons);
	}

	unsigned long cars = 0;
	unsigned long walks = 0;
	Person* p;
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		if (walk_car[i])
			p = new Person(getMap(), id, positions[i], m_clock, speeds_car[cars++], ages[i], gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);
		else
			p = new Person(getMap(), id, positions[i], m_clock, speeds_walk[walks++], ages[i], gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);

		int np1 = phone1[i];
		int np2 = phone2[i];
		while (np1) {
			id = IDGenerator::instance()->next();
			MobilePhone* mp = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::POWER_THRESHOLD, Constants::QUALITY_THRESHOLD, m_connType);
			mp->setMobileOperator(mnos[0]);
			mp->setHolder(p);
			m_agentsCollection->addAgent(mp);
			p->addDevice(typeid(MobilePhone).name(), mp);
			np1--;
		}
		while (np2) {
			id = IDGenerator::instance()->next();
			MobilePhone* mp = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::POWER_THRESHOLD, Constants::QUALITY_THRESHOLD, m_connType);
			mp->setMobileOperator(mnos[1]);
			mp->setHolder(p);
			m_agentsCollection->addAgent(mp);
			p->addDevice(typeid(MobilePhone).name(), mp);
			np2--;
		}
		result.push_back(p);
	}
	delete[] walk_car;
	delete[] phone1;
	delete[] phone2;
	delete[] speeds_walk;
	delete[] speeds_car;
	delete[] ages;
	delete[] gender;
	return (result);
}

const string& World::getAntennasFilename() const {
	return m_antennasFilename;
}

const string& World::getPersonsFilename() const {
	return m_personsFilename;
}

string World::parseProbabilities(const string& probabilitiesFileName) {
	XMLDocument doc;
	string probsFileNamePrefix;
	XMLError err = doc.LoadFile(probabilitiesFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for probabilities ");

	XMLElement* probEl = doc.FirstChildElement("probabilities");
	if (!probEl)
		throw std::runtime_error("Syntax error in the configuration file for probabilities ");
	else {
		XMLNode* priorNode = getNode(probEl, "prior");
		if (priorNode) {
			if (!strcmp(priorNode->ToText()->Value(), "network"))
				m_prior = PriorType::NETWORK;
			else if (!strcmp(priorNode->ToText()->Value(), "uniform"))
				m_prior = PriorType::UNIFORM;
			else if (!strcmp(priorNode->ToText()->Value(), "register"))
				m_prior = PriorType::REGISTER;
			else
				m_prior = PriorType::UNIFORM;
		} else
			m_prior = Constants::PRIOR_PROBABILITY;
		XMLNode* probsNode = getNode(probEl, "prob_file_name_prefix");
		if (probsNode)
			probsFileNamePrefix = probsNode->ToText()->Value();
		else
			probsFileNamePrefix = Constants::PROB_FILE_NAME_PREFIX;
	}
	return probsFileNamePrefix;
}

