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


using namespace tinyxml2;



class SimConfig: public Config {
public:
	SimConfig(const string& fileName, AgentsCollection* agents, Map* map);
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

private:
	void parse();


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
	HomeWorkScenario m_homeWork;

	vector<MobileOperator*> parseMNOs(XMLElement* el);
	MovementType parseMovement(XMLElement* el);
	HoldableAgent::CONNECTION_TYPE parseConnectionType(XMLElement* el);
	double getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType);
	void parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario& hws);
};

#endif /* INCLUDE_SIMCONFIG_H_ */
