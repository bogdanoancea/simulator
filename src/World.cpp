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
	m_probFilename = Constants::PROB_FILE_NAME;

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

World::World(Map* map, const string& configPersonsFileName, const string& configAntennasFile, const string& configSimulationFileName) :
		m_map { map } {

	vector<MobileOperator*> mnos = parseSimulationFile(configSimulationFileName);
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock(m_startTime, m_endTime, m_timeIncrement);

	for (unsigned long i = 0; i < mnos.size(); i++) {
		m_agentsCollection->addAgent(mnos[i]);
	}

	vector<Person*> persons = parsePersons(configPersonsFileName, mnos);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
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
	} catch (std::ofstream::failure& e) {
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

	unsigned long id;
	double power = Constants::ANTENNA_POWER;
	double attFactor = Constants::ATT_FACTOR;
	int maxConnections = Constants::MAX_CONNECTIONS;
	double smid = Constants::S_MID;
	double ssteep = Constants::S_STEEP;

	vector<Point*> positions = utils::generatePoints(getMap(), numAntennas);
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
		XMLElement* antennaEl = utils::getFirstChildElement(antennasNode, "antenna");
		for (; antennaEl; antennaEl = antennaEl->NextSiblingElement()) {
			if (antennaEl && strcmp(antennaEl->Name(), "antenna")) {
				cout << "unknown element: " << antennaEl->Name() << " ignoring it" << endl;
				continue;
			}
			unsigned long id = IDGenerator::instance()->next();
			Antenna* a = new Antenna(getMap(), m_clock, id, antennaEl);
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

		vector<pair<string, double>> probMobilePhones;
		XMLElement* probMobilePhoneEl = utils::getFirstChildElement(personsEl, "prob_mobile_phone");
		for (; probMobilePhoneEl; probMobilePhoneEl = probMobilePhoneEl->NextSiblingElement("prob_mobile_phone")) {
			//unsigned long id = IDGenerator::instance()->next();
			XMLNode* n = getNode(probMobilePhoneEl, "mno_name");
			string name = n->ToText()->Value();
			n = getNode(probMobilePhoneEl, "prob");
			double prob = atof(n->ToText()->Value());
			probMobilePhones.push_back(make_pair(name, prob));
		}

		double probSecMobilePhone = Constants::PROB_SECOND_MOBILE_PHONE;
		XMLNode* prob_sec_mobilePhoneNode = getNode(personsEl, "prob_sec_mobile_phone");
		if (prob_sec_mobilePhoneNode)
			probSecMobilePhone = atof(prob_sec_mobilePhoneNode->ToText()->Value());

		double probSecMobilePhoneSameMNO = Constants::PROB_SECOND_MOBILE_PHONE;
		XMLNode* prob_sec_mobilePhoneSameMNONode = getNode(personsEl, "prob_sec_mobile_phone_same_mno");
		if (prob_sec_mobilePhoneSameMNONode)
			probSecMobilePhoneSameMNO = atof(prob_sec_mobilePhoneSameMNONode->ToText()->Value());

		XMLNode* speed_walkNode = getNode(personsEl, "speed_walk");
		double speed_walk = atof(speed_walkNode->ToText()->Value());

		XMLNode* speed_carNode = getNode(personsEl, "speed_car");
		double speed_car = atof(speed_carNode->ToText()->Value());

		result = generatePopulation(numPersons, params, d, male_share, probMobilePhones, probSecMobilePhone, probSecMobilePhoneSameMNO, mnos, speed_walk, speed_car);
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

unsigned int World::getNumMno() const {
	return m_numMNO;
}

void World::setNumMno(unsigned int numMno) {
	m_numMNO = numMno;
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

		XMLNode* mnoNode = getNode(simEl, "num_mnos");
		if (mnoNode)
			m_numMNO = atol(mnoNode->ToText()->Value());
		else
			m_numMNO = Constants::NUM_MNO;

		if (m_numMNO > 2)
			throw std::runtime_error("Maximum 2 MNOs are supported now");

		if (m_numMNO == 1) {
			XMLNode* mnoNameNode = getNode(simEl, "mno_name");
			if (mnoNameNode) {
				addMNO(mnoNameNode->ToText()->Value());
				cout << mnoNameNode->ToText()->Value() << endl;
			} else
				addMNO(Constants::DEFAULT_MNO_NAME);

		} else {

			XMLElement* mnoNameEl = utils::getFirstChildElement(simEl, "mno_name");
			unsigned long id = IDGenerator::instance()->next();
			MobileOperator* mo = new MobileOperator(getMap(), id, m_clock, mnoNameEl->FirstChild()->ToText()->Value());
			result.push_back(mo);
			for (unsigned int i = 1; i < m_numMNO; i++) {
				//cout << mnoNameEl->FirstChild()->ToText()->Value() << endl;
				mnoNameEl = mnoNameEl->NextSiblingElement("mno_name");
				if (mnoNameEl && strcmp(mnoNameEl->Name(), "mno_name")) {
					cout << "unknown element: " << mnoNameEl->Name() << " ignoring it" << endl;
					continue;
				}
				unsigned long id = IDGenerator::instance()->next();
				MobileOperator* mo = new MobileOperator(getMap(), id, m_clock, mnoNameEl->FirstChild()->ToText()->Value());
				result.push_back(mo);
				//cout << mnoNameEl->FirstChild()->ToText()->Value() << endl;
			}
		}

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

		XMLNode* probNode = getNode(simEl, "prob_file");
		if (probNode)
			m_probFilename = probNode->ToText()->Value();
		else
			m_probFilename = Constants::PROB_FILE_NAME;

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

		XMLNode* priorNode = getNode(simEl, "prior");
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
	}
	return (result);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution, double male_share,
		vector<pair<string, double>> probMobilePhones, double probSecMobilePhone, double probSecMobilePhoneSameMNO, vector<MobileOperator*> mnos, double speed_walk, double speed_car) {

	vector<Person*> result;

	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons);

	double probMobilePhone = 0.0;
	for (auto& n : probMobilePhones)
		probMobilePhone += n.second;

	int* walk_car = RandomNumberGenerator::instance()->generateBernoulliInt(0.5, numPersons);
	int* phone = RandomNumberGenerator::instance()->generateBernoulliInt(probMobilePhone, numPersons);
	int* phone2 = RandomNumberGenerator::instance()->generateBernoulliInt(probSecMobilePhone, numPersons);
	cout << "sec mob phone:" << probSecMobilePhone << endl;
	int sum = 0;
	unsigned long numPhones = 0;
	for (unsigned long i = 0; i < numPersons; i++) {
		sum += walk_car[i];
		numPhones += (phone[i] + phone2[i]);
		//cout << phone[i] << ":" << phone2[i] << endl;
	}
//cout << "numphones: " << numPhones << endl;



	int* gender = RandomNumberGenerator::instance()->generateBinomialInt(1, male_share, numPersons);
	double* speeds_walk = RandomNumberGenerator::instance()->generateNormalDouble(speed_walk, 0.1 * speed_walk, numPersons - sum);
	double* speeds_car = RandomNumberGenerator::instance()->generateNormalDouble(speed_car, 0.1 * speed_car, sum);

	double* ages;
	if (age_distribution == Person::AgeDistributions::UNIFORM)
		ages = RandomNumberGenerator::instance()->generateUniformDouble(params[0], params[1], numPersons);
	else if (age_distribution == Person::AgeDistributions::NORMAL) {
		ages = RandomNumberGenerator::instance()->generateTruncatedNormalDouble(params[2], params[3], params[0], params[1], numPersons);
	} else {
		ages = RandomNumberGenerator::instance()->generateUniformDouble(params[0], params[1], numPersons);
	}

	unsigned long cars = 0;
	unsigned long walks = 0;
	vector<MobilePhone*> mobiles = generateMobilePhones(numPhones, m_connType);
	unsigned long m_index = 0;
	Person* p;
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		if (walk_car[i])
			p = new Person(getMap(), id, positions[i], m_clock, speeds_car[cars++], ages[i], gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);
		else
			p = new Person(getMap(), id, positions[i], m_clock, speeds_walk[walks++], ages[i], gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE);

//		cout << "which MNO " << whichMNO(probMobilePhones,  mnos) << endl;
//		double pmno1 = RandomNumberGenerator::instance()->generateBernoulliInt(0.25);
//		cout << "prob la PRIMUL OPERATOR " << pmno1 << endl;
//		if(pmno1 == 1) {
//			double psec = RandomNumberGenerator::instance()->generateBernoulliInt(0.1);
//			cout << "prob al doilea telefon " << psec << endl;
//		}

		if (phone[i]) {
			mobiles[m_index]->setHolder(p);
			p->addDevice(typeid(MobilePhone).name(), mobiles[m_index]);
			m_index++;
		}
		if (phone2[i]) {
			mobiles[m_index]->setHolder(p);
			p->addDevice(typeid(MobilePhone).name(), mobiles[m_index]);
			m_index++;
		}
		result.push_back(p);
	}
	delete[] walk_car;
	delete[] phone;
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

void World::addMNO(string mnoName) {
	m_mnoNames.push_back(mnoName);
}

int World::whichMNO(vector<pair<string, double>> probs, vector<MobileOperator*> mnos) {
	int result = -1;
	int n = probs.size();
	int j;
	for(j = 0; j < n; j++) {
		cout << "mno " << j << "prob " << probs.at(j).second << endl;
		 result = RandomNumberGenerator::instance()->generateBinomialInt(1, probs.at(j).second);
		 if(result == 1)
			 break;
	}
	if(result == 0)
		return -1;
	else
		return j;
}
