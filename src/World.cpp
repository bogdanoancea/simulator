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
#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <agent/Person.h>
#include <Clock.h>
#include <Constants.h>
#include <events/EventType.h>
#include <geos/geom/Point.h>
#include <map/Map.h>
#include <NetPriorPostLocProb.h>
#include <parsers/ProbabilitiesConfiguration.h>
#include <UnifPriorPostLocProb.h>
#include <World.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <typeinfo>
#include <unordered_map>
#include <utility>

using namespace std;
using namespace tinyxml2;

class Compare
{
public:
    //Ascending order sort
    bool operator() (pair<AntennaInfo, int> pair1, pair<AntennaInfo, int> pair2)
    {
        return pair1.first > pair2.first;
    }
};

World::World(Map* mmap, const string& configPersonsFileName, const string& configAntennasFileName, const string& configSimulationFileName,
		const string& probabilitiesFileName) {

	m_agentsCollection = new AgentsCollection();
	m_eventFactory = new EventFactory();
	SimulationConfigurationParser sConfigParser(configSimulationFileName, m_agentsCollection, mmap);
	sConfigParser.parse();
	m_sp = sConfigParser.getSimulationConfiguration();
	mmap->addGrid(m_sp->getGridDimTileX(), m_sp->getGridDimTileY());
	time_t tt = m_sp->getClock()->realTime();

	cout << "Generating objects started at " << ctime(&tt) << endl;

	std::unique_ptr<ConfigParser> antennaConfig = make_unique<AntennaConfigParser>(configAntennasFileName, m_sp, m_agentsCollection, m_eventFactory);
	antennaConfig->parse();

	std::unique_ptr<ConfigParser> persConfig = make_unique<PersonsConfigParser>(configPersonsFileName, m_sp, m_agentsCollection);
	persConfig->parse();

	if (!probabilitiesFileName.empty()) {
		std::unique_ptr<ProbabilitiesConfigParser> probabilitiesConfig = make_unique<ProbabilitiesConfigParser>(probabilitiesFileName);
		probabilitiesConfig->parse();
		map<const unsigned long, const string> probFileNames;
		for (unsigned long i = 0; i < m_sp->getMnos().size(); i++)
			probFileNames.insert(pair<const unsigned long, string>(m_sp->getMnos()[i]->getId(), m_sp->getOutputDir() + "/" + probabilitiesConfig->getConfiguration().getProbsFilenamePrefix() + "_" + m_sp->getMnos()[i]->getMNOName() + ".csv"));

		if (probabilitiesConfig->getConfiguration().getPriorType() == PriorType::UNIFORM) {
			m_postMethod = std::make_shared<UnifPriorPostLocProb>(m_sp->getMap(), m_sp->getClock(), m_agentsCollection, probFileNames);
		} else if (probabilitiesConfig->getConfiguration().getPriorType() == PriorType::NETWORK) {
			m_postMethod = std::make_shared<NetPriorPostLocProb>(m_sp->getMap(), m_sp->getClock(), m_agentsCollection, probFileNames);
		}
	}

	tt = m_sp->getClock()->realTime();
	cout << "Generating objects ended at " << ctime(&tt) << endl;
}

World::~World() {
	delete m_agentsCollection;
	delete m_eventFactory;
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
			Point* pt = p->move();
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

const string& World::getOutputDir() const {
	return m_sp->getOutputDir();
}

HoldableAgent::CONNECTION_TYPE World::getConnectionType() const {
	return (m_sp->getConnType());
}

void World::computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data) {
	m_postMethod->computeProbabilities(data);
}

std::map<unsigned long, vector<AntennaInfo>> World::getEvents(bool computeProb) {
	std::map<unsigned long, vector<AntennaInfo>> data;
	auto itr_mno = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	auto itra = m_agentsCollection->getAgentListByType(typeid(Antenna).name());
	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
		vector<AntennaInfo> tmp;
		std::priority_queue<pair<AntennaInfo, int>, std::vector<pair<AntennaInfo, int>>, Compare > minHeap;
		vector<ifstream> handles;
		int i = 0;

		for (auto it = itra.first; it != itra.second; it++) {
			Antenna* a = static_cast<Antenna*>(it->second);
			if (a->getMNO()->getId() == mo->getId()) {
				string fileName = mo->getOutputDir() + "/" + a->getAntennaOutputFileName();
//				cout << "introduc 1: " << fileName << endl;
				handles.push_back(ifstream(fileName.c_str()));
				string firstValue, headerValue;
				handles[i] >> headerValue;
				handles[i] >> firstValue;
//				cout << "header: " << headerValue << endl;
//				cout << "first: " << firstValue << endl;
				if(!firstValue.empty()) {
//					cout << "introduc 2: " << fileName << endl;
					AntennaInfo a(firstValue);
					minHeap.push(pair<AntennaInfo, int>(a, i));
					if(computeProb)
						tmp.push_back(a);

				}
				i++;
			}
		}
		ofstream antennaInfoFile;
		string name = mo->getOutputDir() + "/" + string("AntennaInfo_MNO_" + mo->getMNOName() + ".csv");
		antennaInfoFile.open(name, ios::out);
		antennaInfoFile << Antenna::getEventHeader(m_sp->getEventType()) <<  endl;
	    while (minHeap.size() > 0) {
	        pair<AntennaInfo, int> minPair = minHeap.top();
	        minHeap.pop();
	        antennaInfoFile << minPair.first.toString() << '\n';
	        if(computeProb)
	        	tmp.push_back(minPair.first);
	        flush(antennaInfoFile);
	        string nextValue;
	        if (handles[minPair.second] >> nextValue) {
	        	AntennaInfo b(nextValue);
	            minHeap.push(pair <AntennaInfo, int>(b, minPair.second));
	        }
	    }
		antennaInfoFile.close();
		if(computeProb)
			data.insert(std::pair<unsigned long, vector<AntennaInfo>>(mo->getId(),tmp) );
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
	antennaFile << Antenna::getHeader(true);
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = static_cast<Antenna*>(it->second);
		antennaFile << a->toString(true);
		ofstream& f = a->getMNO()->getAntennaCellsFile();
		f << a->getId() << sep << a->dumpCell();
		a->dumpSignal();
	}
}

