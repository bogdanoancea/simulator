/*
 * AntennaConfig.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_ANTENNACONFIG_H_
#define INCLUDE_ANTENNACONFIG_H_

#include <agent/Antenna.h>
#include <events/EventFactory.h>
#include <parsers/Config.h>
#include <parsers/SimConfig.h>
#include <string>
#include <vector>


/**
 * Parses the antenna.xml configuration file and builds the antenna objects
 */

class AntennaConfig: public Config {
public:
	AntennaConfig(const string& fileName, SimConfig* sc, AgentsCollection* ag, EventFactory* evFactory);
	virtual ~AntennaConfig();
	const vector<Antenna*>& getAntennas() const;

private:
	void parse();

	vector<Antenna*> m_antennas;
	SimConfig* m_simConfig;
	EventFactory* m_eventFactory;
};

#endif /* INCLUDE_ANTENNACONFIG_H_ */
