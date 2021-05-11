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
 * AntennaConfig.h
 *
 *  Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
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
	//const vector<Antenna*>& getAntennas() const;
	void parse() override;
private:

	//vector<Antenna*> m_antennas;
	SimulationConfiguration* m_simConfig;
	EventFactory* m_eventFactory;
	AgentsCollection* m_agents;
};

#endif /* INCLUDE_ANTENNACONFIG_H_ */
