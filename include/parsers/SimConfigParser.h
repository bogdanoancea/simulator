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
 * SimConfig.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_SIMCONFIG_H_
#define INCLUDE_SIMCONFIG_H_



#include <parsers/HomeWorkScenario.h>
#include <agent/AgentsCollection.h>
#include <TinyXML2.h>
#include <string>
#include <vector>
#include <map>
#include <DistributionType.h>
#include <parsers/ConfigParser.h>
#include <parsers/SimulationConfiguration.h>


using namespace tinyxml2;


/**
 * This class parses the simulation configuration file, given in the command line with the prefix -s.
 * It provides getters for all fields parsed from the xml file.
 */
class SimConfigParser: public ConfigParser {
public:
	/**
	 * The constructor of the class. It takes the simulation file name as a parameter and it passes it to the constructor of its superclass.
	 * It also takes as parameters a pointer to the map of the simulation and a pointer to the agents' collection. It stores locally the pointer
	 * to the map object, initialize the \code{m_homeWorkScenario} member with \code{nullptr},and after parsing the xml
	 * configuration file it builds the clock of the simulation and adds the MNOs objects to the agents' collection.
	 * @param fileName the name of the simulation configuration file, given in the command line with the prefix -s.
	 * @param agents a pointer to the collection of all agents involved in a simulation.After parsing the configuration file, it adds
	 * the MNOs objects to this collection.
	 * @param map a pointer to the map of the simulation. Even the map file is parsed elsewhere, this pointer is also stored here
	 * to be passed to other objects that need it.
	 */
	SimConfigParser(const string& fileName, AgentsCollection* agents, Map* map);

	/**
	 * Destructor of the class. It frees the memory allocated for the \code{m_clock} member, (the \code{Clock}
	 * object of the simulation}, and for the
	 * \code{m_homeWorkScenario} member in case this simulation is a home-work type of simulation.
	 */
	virtual ~SimConfigParser();

	SimulationConfiguration* getSimulationConfiguration();

	void parse() override;
private:



	SimulationConfiguration* m_simConfig;
	AgentsCollection* m_agents;
	vector<MobileOperator*> parseMNOs(XMLElement* el);
	MovementType parseMovement(XMLElement* el);
	HoldableAgent::CONNECTION_TYPE parseConnectionType(XMLElement* el);
	double getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType);
	void parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws);
	Distribution* parseDirectionAngleDistribution(XMLElement* homeWorkElement);
	shared_ptr<Distribution> parseStayTimeDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseIntervalBetweenStaysDistribution(XMLElement* parent);

};

#endif /* INCLUDE_SIMCONFIG_H_ */
