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

#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <events/EventType.h>
#include <MovementType.h>
#include <parsers/Config.h>
#include <parsers/HomeWorkScenario.h>
#include <agent/AgentsCollection.h>
#include <TinyXML2.h>
#include <string>
#include <vector>
#include <map>
#include <DistributionType.h>


using namespace tinyxml2;


/**
 * This class parses the simulation configuration file, given in the command line with the prefix -s.
 * It provides getters for all fields parsed from the xml file.
 */
class SimConfig: public Config {
public:
	/**
	 * The constructor of the class. It takes the simulation file name as a parameter and it passes it to the constructor of its superclass.
	 * It also takes as parameters a pointer to the map of the simulation and a pointer to the agents' collection. It stores locally the pointer
	 * to the map object, initialize the \code{m_homeWorkScenario} member with \code{nullptr} and after parsing the xml
	 * configuration file it builds the clock of the simulation and adds the MNOs objects to the agents' collection.
	 * @param fileName the name of the simulation configuration file, given in the command line with the prefix -s.
	 * @param agents a pointer to the collection of all agents involved in a simulation.After parsing the configuration file, it adds
	 * the MNOs objects to this collection.
	 * @param map a pointer to the map of the simulation. Even the map file is parsed elsewhere, this pointer is also stored here
	 * to be passed to other objects that need it.
	 */
	SimConfig(const string& fileName, AgentsCollection* agents, Map* map);

	/**
	 * Destructor of the class. It frees the memory allocated for the \code{m_clock} member (the \code{Clock}
	 * object of the simulation} and for the
	 * \code{m_homeWorkScenario} member in case this simulation is a home-work type simulation of.
	 */
	virtual ~SimConfig();

	const string& getAntennasFilename() const;
	void setAntennasFilename(const string& antennasFilename);
	double getConnThreshold() const;
	void setConnThreshold(double connThreshold);
	HoldableAgent::CONNECTION_TYPE getConnType() const;
	void setConnType(HoldableAgent::CONNECTION_TYPE connType);
	unsigned long getEndTime() const;
	void setEndTime(unsigned long endTime);
	EventType getEventType() const;
	void setEventType(EventType eventType);
	double getGridDimTileX() const;
	void setGridDimTileX(double gridDimTileX);
	double getGridDimTileY() const;
	void setGridDimTileY(double gridDimTileY);
	const string& getGridFilename() const;
	void setGridFilename(const string& gridFilename);
	unsigned getIntevalBetweenStays() const;
	void setIntevalBetweenStays(unsigned intevalBetweenStays);
	const vector<MobileOperator*>& getMnos() const;
	void setMnos(const vector<MobileOperator*>& mnos);
	MovementType getMvType() const;
	void setMvType(MovementType mvType);
	const string& getOutputDir() const;
	void setOutputDir(const string& outputDir);
	const string& getPersonsFilename() const;
	void setPersonsFilename(const string& personsFilename);
	double getProbSecMobilePhone() const;
	void setProbSecMobilePhone(double probSecMobilePhone);
	unsigned getSeed() const;
	void setSeed(unsigned seed);
	unsigned long getStartTime() const;
	void setStartTime(unsigned long startTime);
	unsigned long getStay() const;
	void setStay(unsigned long stay);
	unsigned long getTimeIncrement() const;
	void setTimeIncrement(unsigned long timeIncrement);
	Clock* getClock();
	Map* getMap();
	bool isHomeWorkScenario() const;
	unsigned int getNumHomeLocations() const;
	unsigned int getNumWorkLocations() const;
	HomeWorkLocation getHomeLocation(unsigned int i) const;
	HomeWorkLocation getWorkLocation(unsigned int i) const;
	HomeWorkScenario* getHomeWorkScenario();

private:
	void parse() override;


	string m_outputDir;
	unsigned long m_startTime;
	unsigned long m_endTime;
	unsigned long m_timeIncrement;
	unsigned long m_stay;
	unsigned m_intevalBetweenStays;
	vector<MobileOperator*> m_mnos;
	double m_probSecMobilePhone; //SimConfig
	MovementType m_mvType; //SimConfig
	HoldableAgent::CONNECTION_TYPE m_connType; //SimConfig
	double m_connThreshold; //SimConfig
	string m_gridFilename; //SimConfig
	string m_personsFilename; //SimConfig
	string m_antennasFilename; //SimConfig
	double m_GridDimTileX; //SimConfig
	double m_GridDimTileY; //simConfig
	unsigned m_seed; //simConfig
	EventType m_eventType; // SimConfig
	Clock* m_clock;
	Map* m_map;
	HomeWorkScenario* m_homeWorkScenario;

	vector<MobileOperator*> parseMNOs(XMLElement* el);
	MovementType parseMovement(XMLElement* el);
	HoldableAgent::CONNECTION_TYPE parseConnectionType(XMLElement* el);
	double getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType);
	void parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws);
	Distribution* parseDirectionAngleDistribution(XMLElement* homeWorkElement);
	void parseLaplaceParams(XMLElement* distribution, vector<pair<const char*, double>>& distrPar);
	void parseNormalParams(XMLElement* distribution, vector<pair<const char*, double>>& distrPar);
	void parseUniformParams(XMLElement* distribution, vector<pair<const char*, double>>& distrPar);
};

#endif /* INCLUDE_SIMCONFIG_H_ */
