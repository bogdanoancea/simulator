/*
 * AntennaConfig.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_ANTENNACONFIG_H_
#define INCLUDE_ANTENNACONFIG_H_

#include <agent/Antenna.h>
#include <agent/AgentsCollection.h>
#include <events/EventFactory.h>
#include <parsers/ConfigParser.h>
#include <parsers/SimulationConfiguration.h>
#include <vector>


/**
 * Parses the antenna.xml configuration file and builds the antenna objects
 */

class AntennaConfigParser: public ConfigParser {
public:
	AntennaConfigParser(const string& fileName, SimulationConfiguration* sc, AgentsCollection* ag, EventFactory* evFactory);
	virtual ~AntennaConfigParser();
	const vector<Antenna*>& getAntennas() const;

private:
	void parse() override;

	vector<Antenna*> m_antennas;
	SimulationConfiguration* m_simConfig;
	EventFactory* m_eventFactory;
};

#endif /* INCLUDE_ANTENNACONFIG_H_ */
