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
#include <parsers/AntennaConfiguration.h>
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
	parse();
	for (unsigned long i = 0; i < m_antennas.size(); i++) {
		//m_antennas[i]->setCell(m_simConfig->getConnType());
		agents->addAgent(m_antennas[i]);
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
			AntennaConfiguration configuration;
			unsigned long id = IDGenerator::instance()->next();
			XMLNode* n = getNode(antennaEl, "mno_name");
			if (n) {
				const char* mno_name = n->ToText()->Value();
				configuration.setMno(nullptr);
				for (unsigned int i = 0; i < m_simConfig->getMnos().size(); i++) {
					if (m_simConfig->getMnos().at(i)->getMNOName().compare(mno_name) == 0) {
						configuration.setMno(m_simConfig->getMnos().at(i));
						break;
					}
				}
				if (configuration.getMno() == nullptr)
					throw runtime_error("Unknown MNO " + string(mno_name));
			}
			else {
				configuration.setMno(m_simConfig->getMnos().at(0));
			}

			configuration.setMaxConnections(getValue(antennaEl, "maxconnections", Constants::ANTENNA_MAX_CONNECTIONS));
			configuration.setPower(getValue(antennaEl, "power", Constants::ANTENNA_POWER));
			configuration.setPle(getValue(antennaEl, "attenuationfactor", Constants::ATT_FACTOR));
			//cout << m_ple << " citit" << endl;
			const char* t = getValue(antennaEl, "type", "omnidirectional");
			if (!strcmp(t, "directional"))
				configuration.setType(AntennaType::DIRECTIONAL);

			configuration.setSmin(getValue(antennaEl, "Smin", Constants::ANTENNA_SMIN));
			configuration.setQmin(getValue(antennaEl, "Qmin", Constants::ANTENNA_QMIN));
			configuration.setSmid(getValue(antennaEl, "Smid", Constants::ANTENNA_S_MID));
			configuration.setSSteep(getValue(antennaEl, "SSteep", Constants::ANTENNA_S_STEEP));
			double x = getValue(antennaEl, "x");
			double y = getValue(antennaEl, "y");
			configuration.setHeight(getValue(antennaEl, "z", Constants::ANTENNA_HEIGHT));
			configuration.setNetworkType(getValue(antennaEl, "network_type", Constants::NETWORK_TYPE));

		//TODO get elevation from Grid
			Coordinate c = Coordinate(x, y, configuration.getHeight());
			configuration.setLocation(m_simConfig->getMap()->getGlobalFactory()->createPoint(c));

			if (configuration.getType() == AntennaType::DIRECTIONAL) {
				configuration.setTilt(getValue(antennaEl, "tilt", Constants::ANTENNA_TILT));
				configuration.setAzimDBBack(getValue(antennaEl, "azim_dB_back", Constants::ANTENNA_AZIM_DB_BACK));
				configuration.setElevDBBack(getValue(antennaEl, "elev_dB_back", Constants::ANTENNA_ELEV_DB_BACK));
				configuration.setBeamH(getValue(antennaEl, "beam_h", Constants::ANTENNA_BEAM_H));
				configuration.setBeamV(getValue(antennaEl, "beam_v", Constants::ANTENNA_BEAM_V));
				configuration.setDirection(getValue(antennaEl, "direction", Constants::ANTENNA_DIRECTION));
			}
			else {
				configuration.setAzimDBBack(0);
				configuration.setElevDBBack(0);
				configuration.setBeamH(0);
				configuration.setBeamV(0);
				configuration.setDirection(0);
			}
			Antenna* a = new Antenna(id, m_simConfig, configuration, m_eventFactory);
			m_antennas.push_back(a);
		}
	}
}

const vector<Antenna*>& AntennaConfig::getAntennas() const {
	return m_antennas;
}
