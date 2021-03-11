/*
 * AntennaConfig.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#include <agent/Antenna.h>
#include <agent/AgentsCollection.h>
#include <IDGenerator.h>
#include <parsers/AntennaConfig.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <EMField.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


using namespace tinyxml2;
using namespace utils;


AntennaConfig::AntennaConfig(const string& fileName, SimConfig* sc, AgentsCollection* agents, EventFactory* evFactory): Config(fileName) {
	m_simConfig = sc;
	m_eventFactory = evFactory;
	m_agents = agents;
	parse();
	for (unsigned long i = 0; i < m_antennas.size(); i++) {
		m_antennas[i]->setHandoverMechanism(m_simConfig->getConnType());
		m_agents->addAgent(m_antennas[i]);
		EMField::instance()->addAntenna(m_antennas[i]);
	}

}

AntennaConfig::~AntennaConfig(){
	// TODO Auto-generated destructor stub
}

void AntennaConfig::parse() {
	XMLDocument doc;
	XMLError err = doc.LoadFile(getFileName().c_str());
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
			Antenna* a = new Antenna(m_simConfig->getMap(), m_simConfig->getClock(), id, antennaEl, m_simConfig->getMnos(), m_simConfig->getOutputDir(), *m_eventFactory, m_simConfig->getEventType());
			m_antennas.push_back(a);
		}
	}
}


const vector<Antenna*>& AntennaConfig::getAntennas() const {
	return m_antennas;
}
