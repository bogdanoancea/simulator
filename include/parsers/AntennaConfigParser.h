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
 * Parses the antenna configuration file specified by "-a" option in the command line and builds the antenna objects.
 * This file is named antenna.xml in all example configuration folders.
 * While it parses the parameters of each antenna, it builds Antenna objects and adds them to the AgentsCollection container.
 */
class AntennaConfigParser: public ConfigParser {
public:
	/**
	 * Constructor of the class. It takes the filename of the antenna configuration files, a pointer to the SimulationConfiguration object,
	 * a pointer to the AgentsCollection container class and a pointer to the EventFactory.
	 * The AgentsCollection and EventFactory object are created outside, in the caller of this constructor, in this case the constructor of the World object.
	 * The SimulationConfiguration object is also created by the constructor of the World object by parsing the simulation configuration file.
	 * While this constructor parses the antenna configuration file, an AntennaConfiguration object is created for each antenna.
	 * This object encapsulates all required parameters to build an Antenna object and it is passed to the constructor of the Antenna class.
	 * @param fileName the name of the antenna configuration file, given in the command line of the software with "-a" option.
	 * @param sc a pointer to the SimulationConfiguration object created by the SimulationConfigurationParser. It contains the parameters
	 * of the simulation read from the simulation configuration file, given in the command line of the software with "-s" option.
	 * @param ag a pointer to the AgentsCollection container class created by the constructor of the World class.
	 * @param evFactory a pointer to the EventFactory class created by the constructor of the World class. This factory is used by each Antenna object
	 * the generate network events.
	 */
	AntennaConfigParser(const string& fileName, SimulationConfiguration* sc, AgentsCollection* ag, EventFactory* evFactory);

	/**
	 * Default destructor.
	 */
	virtual ~AntennaConfigParser();

	/**
	 * A method to parse the antenna configuration file. While it parses this file, it builds an AntennaConfiguration object
	 * for each antenna and passes it to the constructor of the Antenna class. All Antenna objects are added to the
	 * AgentsCollection container class.
	 */
	void parse() override;

private:
	SimulationConfiguration* m_simConfig;
	EventFactory* m_eventFactory;
	AgentsCollection* m_agents;
};

#endif /* INCLUDE_ANTENNACONFIG_H_ */
