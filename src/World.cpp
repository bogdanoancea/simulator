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
#include <iomanip>
#include <SimException.h>

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
	m_probSecMobilePhone = 0.0;
	m_seed = -1;

	vector<Person*> persons = generatePopulation(numPersons, 0.8);
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

World::World(Map* mmap, const string& configPersonsFileName, const string& configAntennasFile, const string& configSimulationFileName,
		const string& probabilitiesFileName) :
		m_map { mmap } {

	m_probSecMobilePhone = 0.0;
	vector<MobileOperator*> mnos = parseSimulationFile(configSimulationFileName);

	m_agentsCollection = new AgentsCollection();

	m_clock = new Clock(m_startTime, m_endTime, m_timeIncrement);
	time_t tt = getClock()->realTime();
	cout << "Generating objects started at " << ctime(&tt) << endl;

	string probsPrefix = parseProbabilities(probabilitiesFileName);
	for (unsigned long i = 0; i < mnos.size(); i++) {
		m_agentsCollection->addAgent(mnos[i]);
		m_probFilenames.insert(pair<const unsigned long, string>(mnos[i]->getId(), probsPrefix + "_" + mnos[i]->getMNOName() + ".csv"));
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile, mnos);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	mmap->addGrid(getGridDimTileX(), getGridDimTileY());
	vector<Person*> persons = parsePersons(configPersonsFileName, mnos);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}
	tt = getClock()->realTime();
	cout << "Generating objects ended at " << ctime(&tt) << endl;
}

World::~World() {
	delete m_clock;
	delete m_agentsCollection;
	cout << "End of simulation!" << endl;
}

void World::runSimulation() noexcept(false) {
	ofstream pFile, aFile;
	char sep = Constants::sep;
	try {
		pFile.open(m_personsFilename, ios::out);
		aFile.open(m_antennasFilename, ios::out);
	} catch (ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		throw e;
	}

	auto itr0 = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	for (auto it = itr0.first; it != itr0.second; it++) {
		MobileOperator* mo = static_cast<MobileOperator*>(it->second);
		ofstream& f = mo->getSignalQualityFile();
		f << "Antenna ID" << sep;
		unsigned long noTiles = getMap()->getGrid()->getNoTiles();
		for (unsigned long i = 0; i < noTiles - 1; i++) {
			f << "Tile" << i << sep;
		}
		f << "Tile" << noTiles - 1 << endl;
	}

	auto itr2 = m_agentsCollection->getAgentListByType(typeid(Antenna).name());
	aFile << "t" << sep << "Antenna ID" << sep << "x" << sep << "y" << sep << "MNO ID" << sep << "Tile ID" << endl;
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = static_cast<Antenna*>(it->second);
		aFile << a->dumpLocation() << sep << a->getMNO()->getId() << sep << a->getMap()->getGrid()->getTileNo(a->getLocation()) << endl;
		ofstream& f = a->getMNO()->getAntennaCellsFile();
		f << a->getId() << sep << a->dumpCell();
	}

	time_t tt = getClock()->realTime();
	cout << "Simulation started at " << ctime(&tt) << endl;

	auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());

	RandomNumberGenerator* r = RandomNumberGenerator::instance();
	r->setSeed(time(0));
	const Grid* g = getMap()->getGrid();

	pFile << "t" << sep << "Person ID" << sep << "x" << sep << "y" << sep << "Tile ID" << sep << "Mobile Phone(s) ID" << endl;
	//initial time
	unsigned long t = m_clock->getInitialTime();
	tt = getClock()->realTime();
	cout << "Current simulation step: " << m_clock->getCurrentTime() << ":" << ctime(&tt) << endl;
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = static_cast<Person*>(it->second);
		Point* loc = p->getLocation();
		int n = g->getTileNo(loc->getX(), loc->getY());
		pFile << p->dumpLocation() << sep << n << p->dumpDevices() << endl;
	}

	//iterate over all persons and call move()
	t = m_clock->tick();
	for (; t < m_clock->getFinalTime(); t = m_clock->tick()) {
		tt = getClock()->realTime();
		cout << "Current simulation step: " << m_clock->getCurrentTime() << ":" << ctime(&tt) << endl;
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = static_cast<Person*>(it->second);
			p->move(m_mvType);
			Point* loc = p->getLocation();
			int n = g->getTileNo(loc->getX(), loc->getY());
			pFile << p->dumpLocation() << sep << n << p->dumpDevices() << endl;
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
	return (m_agentsCollection);
}

void World::setAgents(AgentsCollection* agents) {
	m_agentsCollection = agents;
}

Clock* World::getClock() const {
	return (m_clock);
}

void World::setClock(Clock* clock) {
	m_clock = clock;
}

const Map* World::getMap() const {
	return (m_map);
}

void World::setMap(Map* map) {
	m_map = map;
}

const string& World::getGridFilename() const {
	return (m_gridFilename);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, double percentHome) {
	vector<Person*> result;
	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, percentHome, m_seed);
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_seed);
	double* speeds = random_generator->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = random_generator->generateUniformInt(1, 100, numPersons);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		Person* p = new Person(getMap(), id, positions[i], m_clock, speeds[i], ages[i], Person::Gender::MALE, Constants::SIM_STAY_TIME,
				Constants::SIM_INTERVAL_BETWEEN_STAYS);
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
	unsigned long maxConnections = Constants::ANTENNA_MAX_CONNECTIONS;
	double smid = Constants::ANTENNA_S_MID;
	double ssteep = Constants::ANTENNA_S_STEEP;

	vector<Point*> positions = utils::generateFixedPoints(getMap(), numAntennas, m_seed);
	for (unsigned long i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		Antenna* p = new Antenna(getMap(), id, positions[i], m_clock, attFactor, power, maxConnections, smid, ssteep,
				AntennaType::OMNIDIRECTIONAL);
		result.push_back(p);
	}
	return (result);
}

vector<MobilePhone*> World::generateMobilePhones(int numMobilePhones, HoldableAgent::CONNECTION_TYPE connType) {
	vector<MobilePhone*> result;
	unsigned long id;
	for (auto i = 0; i < numMobilePhones; i++) {
		id = IDGenerator::instance()->next();
		MobilePhone* p = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::PHONE_CONNECTION_THRESHOLD, connType);
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

		unsigned long numPersons = getValue(personsEl, "num_persons", Constants::SIM_NO_PERSONS);
		unsigned long min_age = getValue(personsEl, "min_age");
		unsigned long max_age = getValue(personsEl, "max_age");

		XMLElement* ageDistribEl = getFirstChildElement(personsEl, "age_distribution");
		const char* distrib = getValue(ageDistribEl, "type", "UNKNOWN");
		if (strcmp(distrib, "normal") && strcmp(distrib, "uniform"))
			throw std::runtime_error("Unknown age distribution for population!");

		Person::AgeDistributions d;
		vector<double> params;
		if (!strcmp(distrib, "normal")) {
			double mean_age = getValue(ageDistribEl, "mean");
			double sd = getValue(ageDistribEl, "sd");
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

		double male_share = getValue(personsEl, "male_share");
		double speed_walk = getValue(personsEl, "speed_walk");
		double speed_car = getValue(personsEl, "speed_car");
		double percentHome = getValue(personsEl, "percent_home");
		result = generatePopulation(numPersons, params, d, male_share, mnos, speed_walk, speed_car, percentHome);
	}
	return (result);
}

double World::getGridDimTileX() const {
	return (m_GridDimTileX);
}

double World::getGridDimTileY() const {
	return (m_GridDimTileY);
}

PriorType World::getPrior() const {
	return (m_prior);
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
		m_startTime = getValue(simEl, "start_time", Constants::SIM_START_TIME);
		m_endTime = getValue(simEl, "end_time", Constants::SIM_END_TIME);
		m_timeIncrement = getValue(simEl, "time_increment", Constants::SIM_INCREMENT_TIME);
		m_stay = getValue(simEl, "time_stay", Constants::SIM_STAY_TIME);
		m_intevalBetweenStays = getValue(simEl, "interval_between_stays", Constants::SIM_INTERVAL_BETWEEN_STAYS);

		unsigned numMNO = 0;
		XMLElement* mnoEl = utils::getFirstChildElement(simEl, "mno");
		if (mnoEl) {
			for (; mnoEl; mnoEl = mnoEl->NextSiblingElement("mno")) {
				numMNO++;
				const char* name = getValue(mnoEl, "mno_name", "UNKNOWN");
				const double prob = getValue(mnoEl, "prob_mobile_phone");
				unsigned long id = IDGenerator::instance()->next();
				MobileOperator* mo = new MobileOperator(getMap(), id, m_clock, name, prob);
				result.push_back(mo);
			}
		}
		if (numMNO > 2)
			throw std::runtime_error("Maximum 2 MNOs are supported!");

		m_probSecMobilePhone = getValue(simEl, "prob_sec_mobile_phone", Constants::SIM_PROB_SECOND_MOBILE_PHONE);

		const char* mvType = getValue(simEl, "movement_type", "UNKNOWN");
		if (!strcmp(mvType, "random_walk_closed_map"))
			m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP;
		else if (!strcmp(mvType, "random_walk_closed_map_drift")) {
			m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT;
		} else
			m_mvType = MovementType::UNKNOWN;

		const char* connType = getValue(simEl, "connection_type", "UNKNOWN");
		if (!strcmp(connType, "power"))
			m_connType = HoldableAgent::CONNECTION_TYPE::USING_POWER;
		else if (!strcmp(connType, "quality"))
			m_connType = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY;
		else if (!strcmp(connType, "strength"))
			m_connType = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH;
		else
			throw runtime_error("Unknown connection mechanism! Available values: power, quality, strength");


		m_connThreshold = getValue(simEl, "conn_threshold", getDefaultConnectionThreshold(m_connType));
		m_gridFilename = getValue(simEl, "grid_file", Constants::GRID_FILE_NAME);
		m_personsFilename = getValue(simEl, "persons_file", Constants::PERSONS_FILE_NAME);
		m_antennasFilename = getValue(simEl, "antennas_file", Constants::ANTENNAS_FILE_NAME);
		m_GridDimTileX = getValue(simEl, "grid_dim_tile_x", Constants::GRID_DIM_TILE_X);
		m_GridDimTileY = getValue(simEl, "grid_dim_tile_y", Constants::GRID_DIM_TILE_Y);
		m_seed = getValue(simEl, "random_seed", Constants::RANDOM_SEED);
	}
	return (result);
}

double World::getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType) {
	double result = -1;
	if (connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		result = Constants::PHONE_POWER_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
		result = Constants::PHONE_QUALITY_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		result = Constants::PHONE_STRENGTH_THRESHOLD;
	return (result);

}
vector<Person*> World::generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution,
		double male_share, vector<MobileOperator*> mnos, double speed_walk, double speed_car, double percentHome) {

	vector<Person*> result;
	unsigned long id;
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

	int* phone1 = nullptr;
	int* phone2 = nullptr;
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
		for (unsigned long i = 0; i < numPersons; i++) {
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

	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, percentHome, m_seed);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		unsigned long stay = (unsigned long) random_generator->generateNormalDouble(m_stay, 0.2 * m_stay);
		unsigned long interval = (unsigned long) random_generator->generateExponentialDouble(1.0 / m_intevalBetweenStays);
		if (walk_car[i]) {
			p = new Person(getMap(), id, positions[i], m_clock, speeds_car[cars++], (int) ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		} else {
			p = new Person(getMap(), id, positions[i], m_clock, speeds_walk[walks++], (int) ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		}
		int np1 = phone1[i];
		while (np1) {
			id = IDGenerator::instance()->next();
			MobilePhone* mp = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, m_connThreshold, m_connType);
			mp->setMobileOperator(mnos[0]);
			mp->setHolder(p);
			m_agentsCollection->addAgent(mp);
			p->addDevice(typeid(MobilePhone).name(), mp);
			np1--;
		}
		if (numMno == 2) {
			int np2 = phone2[i];
			while (np2) {
				id = IDGenerator::instance()->next();
				MobilePhone* mp = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, m_connThreshold, m_connType);
				mp->setMobileOperator(mnos[1]);
				mp->setHolder(p);
				m_agentsCollection->addAgent(mp);
				p->addDevice(typeid(MobilePhone).name(), mp);
				np2--;
			}
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
	return (m_antennasFilename);
}

const string& World::getPersonsFilename() const {
	return (m_personsFilename);
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
		const char* prior = getValue(probEl, "prior", "UNKNOWN");
		if (!strcmp(prior, "network"))
			m_prior = PriorType::NETWORK;
		else if (!strcmp(prior, "uniform"))
			m_prior = PriorType::UNIFORM;
		else if (!strcmp(prior, "register"))
			m_prior = PriorType::REGISTER;
		else
			m_prior = Constants::PRIOR_PROBABILITY;
		;
		probsFileNamePrefix = getValue(probEl, "prob_file_name_prefix", Constants::PROB_FILE_NAME_PREFIX);
	}
	return probsFileNamePrefix;
}

