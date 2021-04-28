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
#include <parsers/HomeWorkScenario.h>
#include <agent/AgentsCollection.h>
#include <TinyXML2.h>
#include <string>
#include <vector>
#include <map>
#include <DistributionType.h>
#include <parsers/ConfigParser.h>


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

	/**
	 * Returns the name of the \code{csv} file where all the details of the antennas are saved.
	 * @return the name of the \code{csv} file where all the details of the antennas are saved.
	 */
	const string& getAntennasFilename() const;

	/**
	 * Sets the name of the \code{csv} file where all the details of the antennas are saved.
	 * @param antennasFilename the name of the \code{csv} file where all the details of the antennas are saved.
	 */
	void setAntennasFilename(const string& antennasFilename);

	/**
	 * Returns the threshold value of the signal (strength or quality) under which no connection between a mobile device
	 * and an antenna is possible.
	 * @return the threshold value of the signal (strength or quality) under which no connection between a mobile device
	 * and an antenna is possible.
	 */
	double getConnThreshold() const;

	/**
	 * Sets the threshold value of the signal (strength or quality) under which no connection between a mobile device
	 * and an antenna is possible.
	 * @param connThreshold the threshold value of the signal (strength or quality) under which no connection between a mobile device
	 * and an antenna is possible.
	 */
	void setConnThreshold(double connThreshold);

	/**
	 * Returns the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * power, strength or quality. Possible values are: USING_POWER, USING_SIGNAL_QUALITY, USING_SIGNAL_STRENGTH.
	 * @return the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * power, strength or quality. Possible values are: USING_POWER, USING_SIGNAL_QUALITY, USING_SIGNAL_STRENGTH.
	 */
	HoldableAgent::CONNECTION_TYPE getConnType() const;

	/**
	 * Sets the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * power, strength or quality. Possible values are: USING_POWER, USING_SIGNAL_QUALITY, USING_SIGNAL_STRENGTH.
	 * @param connType the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * power, strength or quality. Possible values are: USING_POWER, USING_SIGNAL_QUALITY, USING_SIGNAL_STRENGTH.
	 */
	void setConnType(HoldableAgent::CONNECTION_TYPE connType);

	/**
	 * Returns the final time instant of the simulation.
	 *  @return the final time instant of the simulation.
	 */
	unsigned long getEndTime() const;

	/**
	 * Sets the final time instant of the  simulation.
	 * @param endTime the final time instant of the  simulation.
	 */
	void setEndTime(unsigned long endTime);

	/**
	 * Returns the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the time advancing variable). The time advancing value
	 * depends on the antenna type (_3G, _4G).
	 * @return the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the time advancing variable).
	 */
	EventType getEventType() const;

	/**
	 * Sets the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the time advancing variable).
	 * @param eventType the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the time advancing variable).
	 */
	void setEventType(EventType eventType);

	/**
	 * Returns the tile dimension on OX axis. A rectangular grid of tile is overlapped over the map and the location probabilities are computed
	 * for every tile in the grid.
	 * @return the tile dimension on OX axis.
	 */
	double getGridDimTileX() const;

	/**
	 * Sets the tile dimension on OX axis. A rectangular grid of tile is overlapped over the map and the location probabilities are computed
	 * for every tile in the grid.
	 * @param gridDimTileX the tile dimension on OX axis.
	 */
	void setGridDimTileX(double gridDimTileX);

	/**
	 * Returns the tile dimension on OY axis. A rectangular grid of tile is overlapped over the map and the location probabilities are computed
	 * for every tile in the grid.
	 * @return the tile dimension on OY axis.
	 */
	double getGridDimTileY() const;

	/**
	 * Sets the tile dimension on OY axis. A rectangular grid of tile is overlapped over the map and the location probabilities are computed
	 * for every tile in the grid.
	 * @param gridDimTileY the tile dimension on OY axis.
	 */
	void setGridDimTileY(double gridDimTileY);
	/**
	 * Returns the name of the \code{csv} file where the description of the rectangular grid overlapped on the map is saved.
	 * @return the name of the \code{csv} file where the description of the rectangular grid overlapped on the map is saved.
	 */
	const string& getGridFilename() const;

	/**
	 * Sets the name of the \code{csv} file where the description of the rectangular grid overlapped on the map is saved.
	 * @param gridFilename the name of the \code{csv} file where the description of the rectangular grid overlapped on the map is saved.
	 */
	void setGridFilename(const string& gridFilename);

	/**
	 * Returns the mean time between two stops of a person. Some of the movement patterns (random_walk, random_walk_with_drift, levy_flight) consist
	 * in a sequence of periods of time when the person move and and periods of time when the person stays in the same position. The actual time intervals
	 * between two consecutive stops are generated from an exponential distribution with the mean given by the value returned by this function.
	 * @return the mean time between two stops of a person.
	 */
	shared_ptr<Distribution> getIntevalBetweenStays() const;

	/**
	 * Sets the mean time between two stops of a person. Some of the movement patterns (random_walk, random_walk_with_drift, levy_flight) consist
	 * in a sequence of periods of time when the person move and and periods of time when the person stays in the same position. The actual time intervals
	 * between two consecutive stops are generated from an exponential distribution with the mean given by the value set by this function.
	 * @param intevalBetweenStays the mean time between two stops of a person.
	 */
	void setIntevalBetweenStays(shared_ptr<Distribution> intevalBetweenStays);

	/**
	 * Returns a vector with pointers to MNO objects. The number and the name of each MNO are given in the simulation configuration file.
	 * @return a vector with pointers to MNO objects.
	 */
	const vector<MobileOperator*>& getMnos() const;

	/**
	 * Sets the MNO objects. The number and the name of each MNO are given in the simulation configuration file.
	 * @param mnos a vector with pointers to the MNO objects.
	 */
	void setMnos(const vector<MobileOperator*>& mnos);

	/**
	 * Returns the movement type of a person involved in a simulation, which can take one of the following values given by the enumeration \code{MovementType}:
	 *  \code{RANDOM_WALK_CLOSED_MAP}, \code{RANDOM_WALK_CLOSED_MAP_WITH_DRIFT}, \code{LEVY_FLIGHT}, \code{HOME_WORK}. In the first three cases, all persons in a
	 *  simulation have the same movement pattern while in the latest (\code{HOME_WORK}) there is a small percent having a random type of
	 *  movement (\code{RANDOM_WALK_CLOSED_MAP}, \code{RANDOM_WALK_CLOSED_MAP_WITH_DRIFT}, \code{LEVY_FLIGHT}) while the rest
	 *  follows a home-work pattern.
	 * @return the movement type of a person.
	 */
	MovementType getMvType() const;

	/**
	 * Sets the movement type of a person involved in a simulation, which can take one of the following values given by the enumeration \code{MovementType}:
	 *  \code{RANDOM_WALK_CLOSED_MAP}, \code{RANDOM_WALK_CLOSED_MAP_WITH_DRIFT}, \code{LEVY_FLIGHT}, \code{HOME_WORK}. In the first three cases, all persons in a
	 *  simulation have the same movement pattern while in the latest (\code{HOME_WORK}) there is a small percent having a random type of
	 *  movement (\code{RANDOM_WALK_CLOSED_MAP}, \code{RANDOM_WALK_CLOSED_MAP_WITH_DRIFT}, \code{LEVY_FLIGHT}) while the rest
	 *  follows a home-work pattern.
	 * @param mvType the movement type of a person involved in a simulation.
	 */
	void setMvType(MovementType mvType);

	/**
	 * Returns the name of the directory where the output files are saved. If this directory
	 * doesn't exists, it is created by the simulation software.
	 * @return the name of the directory where the output files are saved.
	 */
	const string& getOutputDir() const;

	/**
	 * Set the name of the directory where the output files are saved. If this directory
	 * doesn't exists, it is created by the simulation software.
	 * @param outputDir the name of the directory where the output files are saved.
	 */
	void setOutputDir(const string& outputDir);

	/**
	 *
	 * @return
	 */
	const string& getPersonsFilename() const;

	void setPersonsFilename(const string& personsFilename);

	double getProbSecMobilePhone() const;
	void setProbSecMobilePhone(double probSecMobilePhone);
	unsigned getSeed() const;
	void setSeed(unsigned seed);
	unsigned long getStartTime() const;
	void setStartTime(unsigned long startTime);
	shared_ptr<Distribution> getStay() const;
	void setStay(shared_ptr<Distribution> stay);
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
	//unsigned long m_stay;
	shared_ptr<Distribution> m_stay;
	//unsigned m_intevalBetweenStays;
	shared_ptr<Distribution> m_intevalBetweenStays;
	vector<MobileOperator*> m_mnos;
	double m_probSecMobilePhone;
	MovementType m_mvType;
	HoldableAgent::CONNECTION_TYPE m_connType;
	double m_connThreshold;
	string m_gridFilename;
	string m_personsFilename;
	string m_antennasFilename;
	double m_GridDimTileX;
	double m_GridDimTileY;
	unsigned m_seed;
	EventType m_eventType;
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
	shared_ptr<Distribution> parseStayTimeDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseIntervalBetweenStaysDistribution(XMLElement* parent);

};

#endif /* INCLUDE_SIMCONFIG_H_ */
