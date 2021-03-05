/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * World.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/AgentsCollection.h>
#include <AntennaType.h>
#include <crtdefs.h>
#include <Clock.h>
#include <Constants.h>
#include <CSVparser.hpp>
#include <events/EventType.h>
#include <EMField.h>
#include <geos/geom/Point.h>
#include <IDGenerator.h>
#include <map/Map.h>
#include <NetPriorPostLocProb.h>
#include <parsers/PersonsConfig.h>
#include <TinyXML2.h>
#include <UnifPriorPostLocProb.h>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <map>
#include <typeinfo>


using namespace std;
using namespace utils;
using namespace tinyxml2;


World::World(Map* mmap, const string& configPersonsFileName, const string& configAntennasFile, const string& configSimulationFileName,
		const string& probabilitiesFileName) {

	m_sp = new SimConfig(configSimulationFileName, mmap) ;
	vector<MobileOperator*> mnos = m_sp->getMnos();
	m_agentsCollection = new AgentsCollection();
	m_persConfig = new PersonsConfig(configPersonsFileName, m_sp, m_agentsCollection);

	time_t tt = m_sp->getClock()->realTime();
	cout << "Generating objects started at " << ctime(&tt) << endl;

	string probsPrefix;
	if (!probabilitiesFileName.empty())
		probsPrefix = parseProbabilities(probabilitiesFileName);

	for (unsigned long i = 0; i < mnos.size(); i++) {
		m_agentsCollection->addAgent(mnos[i]);
		if (!probabilitiesFileName.empty())
			m_probFilenames.insert(pair<const unsigned long, string>(mnos[i]->getId(), m_sp->getOutputDir() + "/" + probsPrefix + "_" + mnos[i]->getMNOName() + ".csv"));
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile, mnos);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		antennas[i]->setHandoverMechanism(getConnectionType());
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	mmap->addGrid(m_sp->getGridDimTileX(), m_sp->getGridDimTileY());
	vector<Person*> persons = m_persConfig->getPersons();
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	if (m_prior == PriorType::UNIFORM) {
		auto postProb = std::make_shared<UnifPriorPostLocProb>(m_sp->getMap(), m_sp->getClock(), m_agentsCollection, m_probFilenames);
		setPostProbMethod(postProb);
	} else if (m_prior == PriorType::NETWORK) {
		auto postProb = std::make_shared<NetPriorPostLocProb>(m_sp->getMap(), m_sp->getClock(), m_agentsCollection, m_probFilenames);
		setPostProbMethod(postProb);
	}
	tt = m_sp->getClock()->realTime();
	cout << "Generating objects ended at " << ctime(&tt) << endl;
}

World::~World() {
	delete m_agentsCollection;
	delete m_persConfig;
	delete m_sp;
	cout << "End of simulation!" << endl;
}

void World::runSimulation() noexcept(false) {
	ofstream personsFile, antennaFile;
	char sep = Constants::sep;

	try {
		personsFile.open(m_sp->getOutputDir() + "/" + m_sp->getPersonsFilename(), ios::out);
		antennaFile.open(m_sp->getOutputDir() + "/" + m_sp->getAntennasFilename(), ios::out);
		writeSignalAndCells(antennaFile);
		antennaFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		throw e;
	}
	time_t tt = m_sp->getClock()->realTime();

	personsFile << "t" << sep << "Person ID" << sep << "x" << sep << "y" << sep << "Tile ID" << sep << "Mobile Phone(s) ID" << endl;
	//initial time
	unsigned long t = m_sp->getClock()->getInitialTime();
	tt = m_sp->getClock()->realTime();
	cout << "Simulation started at " << ctime(&tt) << endl;
	cout << "Current simulation step: " << m_sp->getClock()->getCurrentTime() << ":" << ctime(&tt) << endl;
	auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = static_cast<Person*>(it->second);
		Point* loc = p->getLocation();
		personsFile << p->dumpLocation() << sep << m_sp->getMap()->getTileNo(loc->getX(), loc->getY()) << p->dumpDevices() << endl;
	}
	//iterate over all persons and call move()
	t = m_sp->getClock()->tick();
	for (; t < m_sp->getClock()->getFinalTime(); t = m_sp->getClock()->tick()) {
		tt = m_sp->getClock()->realTime();
		cout << "Current simulation step: " << m_sp->getClock()->getCurrentTime() << ":" << ctime(&tt) << endl;
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = static_cast<Person*>(it->second);
			p->move();
			Point* loc = p->getLocation();
			personsFile << p->dumpLocation() << sep << m_sp->getMap()->getTileNo(loc->getX(), loc->getY()) << p->dumpDevices() << endl;
		}
	}
	tt = m_sp->getClock()->realTime();
	cout << "Simulation ended at " << ctime(&tt) << endl;

	try {
		personsFile.close();
	} catch (std::ofstream::failure& e) {
		cerr << "Error closing output files!" << endl;
		throw e;
	}
}

AgentsCollection* World::getAgents() const {
	return (m_agentsCollection);
}

const Map* World::getMap() const {
	return (m_sp->getMap());
}

const string& World::getGridFilename() const {
	return (m_sp->getGridFilename());
}

vector<Person*> World::generatePopulation(unsigned long numPersons, double percentHome) {
	vector<Person*> result;
	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, percentHome, m_sp->getSeed());
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_sp->getSeed());
	double* speeds = random_generator->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = random_generator->generateUniformInt(1, 100, numPersons);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		Person* p = new Person(getMap(), id, positions[i], m_sp->getClock(), speeds[i], ages[i], Person::Gender::MALE, Constants::SIM_STAY_TIME,
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

	vector<Point*> positions = utils::generateFixedPoints(getMap(), numAntennas, m_sp->getSeed());
	for (unsigned long i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		string outDir = Constants::OUTPUT_DIR;
		Antenna* p = new Antenna(getMap(), id, positions[i], m_sp->getClock(), attFactor, power, maxConnections, smid, ssteep,
				AntennaType::OMNIDIRECTIONAL, outDir, m_eventFactory, m_sp->getEventType());
		result.push_back(p);
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
			Antenna* a = new Antenna(getMap(), m_sp->getClock(), id, antennaEl, mnos, m_sp->getOutputDir(), m_eventFactory, m_sp->getEventType());
			result.push_back(a);
		}
	}
	return (result);
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
		probsFileNamePrefix = getValue(probEl, "prob_file_name_prefix", Constants::PROB_FILE_NAME_PREFIX);
	}
	return probsFileNamePrefix;
}

map<const unsigned long, const string> World::getProbFilenames() const {
	return m_probFilenames;
}

const string& World::getOutputDir() const {
	return m_sp->getOutputDir();
}

HoldableAgent::CONNECTION_TYPE World::getConnectionType() const {
	return (m_sp->getConnType());
}

void World::setPostProbMethod(const std::shared_ptr<PostLocProb>& post) {
	m_postMethod = post;
}

void World::computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data) {
	m_postMethod->computeProbabilities(data);
}

std::map<unsigned long, vector<AntennaInfo>> World::getEvents() {
	//char sep = Constants::sep;
	std::map<unsigned long, vector<AntennaInfo>> data;
	auto itr_mno = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	auto itra = m_agentsCollection->getAgentListByType(typeid(Antenna).name());

	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
		vector<AntennaInfo> tmp;
		for (auto it = itra.first; it != itra.second; it++) {
			Antenna* a = static_cast<Antenna*>(it->second);
			if (a->getMNO()->getId() == mo->getId()) {
				string fileName = mo->getOutputDir() + "/" + a->getAntennaOutputFileName();
				CSVParser file = CSVParser(fileName, DataType::eFILE, ',', true);
				for (unsigned long i = 0; i < file.rowCount(); i++) {
					Row s = file[i];
					AntennaInfo a(m_sp->getEventType(), s);
					tmp.push_back(a);
				}
			}
			sort(tmp.begin(), tmp.end());
			ofstream antennaInfoFile;
			string name = mo->getOutputDir() + "/" + string("AntennaInfo_MNO_" + mo->getMNOName() + ".csv");
			antennaInfoFile.open(name, ios::out);
			antennaInfoFile << Antenna::getEventHeader(m_sp->getEventType()) <<  endl;
			for (AntennaInfo& ai : tmp) {
				antennaInfoFile << ai.toString() <<  endl;
			}
			antennaInfoFile.close();
		}
		 data.insert ( std::pair<unsigned long, vector<AntennaInfo>>(mo->getId(),tmp) );
	}
	return (data);
}


void World::writeSignalAndCells(ostream& antennaFile) {
	char sep = Constants::sep;
	auto itr0 = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	for (auto it = itr0.first; it != itr0.second; it++) {
		MobileOperator* mo = static_cast<MobileOperator*>(it->second);
		mo->writeSignalFileHeader();
	}
	auto itr2 = m_agentsCollection->getAgentListByType(typeid(Antenna).name());
	antennaFile << "t" << sep << "Antenna ID" << sep << "x" << sep << "y" << sep << "MNO ID" << sep << "mno_name" << sep << "maxconnection"<< sep<< "power" <<sep <<"attentuationfactor"<< sep << "type" << sep << "Smin" << sep << "Qmin" << sep << "Smid" << sep << "SSteep" << sep << "tilt" <<sep <<  "azim_dB_back" << sep << "elev_dB_back" << sep << "beam_h" << sep << "beam_v" << sep << "direction" << sep << "z" << sep <<"Tile ID" << endl;
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = static_cast<Antenna*>(it->second);
		if(a->getType() == AntennaType::DIRECTIONAL) {
			antennaFile << a->dumpLocation() << sep << a->getMNO()->getId() << sep << a->getMNO()->getMNOName() << sep << a->getMaxConnections() << sep << a->getPower() << sep << a->getAttenuationFactor() << sep << a->getTypeName() << sep << a->getSmin() << sep << a->getQmin() << sep << a->getSmid() << sep << a->getSSteep() << sep << a->getTilt() << sep << a->getAzimDBBack() << sep << a->getElevDBBack() << sep << a->getBeamH() << sep << a->getBeamV() << sep << a->getDirection() << sep << a->getHeight() << sep << m_sp->getMap()->getTileNo(a->getLocation()) << endl;
		}
		else {
			antennaFile << a->dumpLocation() << sep << a->getMNO()->getId() << sep << a->getMNO()->getMNOName() << sep << a->getMaxConnections() << sep << a->getPower() << sep << a->getAttenuationFactor() << sep << a->getTypeName() << sep << a->getSmin() << sep << a->getQmin() << sep << a->getSmid() << sep << a->getSSteep() << sep << "" << sep << "" << sep << "" << sep << "" << sep << "" << sep << "" << sep << a->getHeight() << sep << m_sp->getMap()->getTileNo(a->getLocation()) << endl;
		}
		ofstream& f = a->getMNO()->getAntennaCellsFile();
		f << a->getId() << sep << a->dumpCell();
		a->dumpSignal();
	}
}

