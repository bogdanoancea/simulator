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
 * SimulationConfiguration.h
 *
 *  Created on: Apr 29, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_SIMULATIONCONFIGURATION_H_
#define INCLUDE_PARSERS_SIMULATIONCONFIGURATION_H_

#include <Distribution.h>
#include <MovementType.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/HomeWorkScenario.h>
#include <parsers/RandomWalkDriftScenario.h>
#include <parsers/LevyFlightScenario.h>
#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <events/EventType.h>
#include <MovementType.h>

/**
 * This class encapsulates the parameters of a simulation read from the simulation configuration file, paased with "-s" option
 * in the command line.
 */
class SimulationConfiguration {
public:

	/**
	 * Default constructor.
	 */
	SimulationConfiguration();

	/**
	 * Default destructor.
	 */
	virtual ~SimulationConfiguration();

	/**
	 * Returns the name of the .csv file where all the technical details of the antennas are saved.
	 * @return the name of the .csv file where all the technical details of the antennas are saved.
	 */
	const string& getAntennasFilename() const;

	/**
	 * Sets the name of the .csv file where all the technical details of the antennas are saved. An excerpt from such a file is presented below:
	 * \code
	 * t,Antenna ID,x,y,MNO ID,mno_name,maxconnection,power,attentuationfactor,type,Smin,Qmin,Smid,SSteep,tilt,azim_dB_back,elev_dB_back,beam_h,beam_v,direction,z,Tile ID
	 * 0,11,8500.000000,3000.000000,0,MNO1,56,10,3.9,omnidirectional,-85,0.3,-91,3,,,,,,,0,473
	 * 0,10,8000.000000,4000.000000,0,MNO1,56,10,4,directional,-85,0.3,-83,1,15,-30,-30,120,75,240,5,631
	 *
	 * \endcode
	 * @param antennasFilename the name of the .csv file where all the technical details of the antennas are saved.
	 */
	void setAntennasFilename(const string& antennasFilename);

	/**
	 * Returns the threshold value of the signal strength or quality under which no connection between a mobile device
	 * and an antenna is possible.
	 * @return the threshold value of the signal strength or quality under which no connection between a mobile device
	 * and an antenna is possible.
	 */
	double getConnThreshold() const;

	/**
	 * Sets the threshold value of the signal strength or quality under which no connection between a mobile device
	 * and an antenna is possible.
	 * @param connThreshold the threshold value of the signal strength or quality under which no connection between a mobile device
	 * and an antenna is possible.
	 */
	void setConnThreshold(double connThreshold);

	/**
	 * Returns the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * strength or dominance. Possible values are: HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE, HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
	 * @return the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * strength or dominance. Possible values are: HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE, HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
	 */
	HoldableAgent::CONNECTION_TYPE getConnType() const;

	/**
	 * Sets the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * strength or dominance. Possible values are: HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE, HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
	 * @param connType the connection type between a mobile device and an antenna. The connection mechanism can be based on the received signal
	 * strength or dominance. Possible values are: HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE, HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
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
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the timing advance variable). The timing advance value
	 * depends on the antenna type (_3G, _4G).
	 * @return the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the timing advance variable).
	 */
	EventType getEventType() const;

	/**
	 * Sets the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the timing advance variable).
	 * @param eventType the type of the network events generated by the interaction between the mobile device and an antenna.
	 * Currently the network events that can be generated are:  CELLID (the id of the antenna where the mobile device is connected) and
	 * CELLIDTA (the id of the antenna where the mobile device is connected and the timing advance variable).
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
	 * Returns the name of the .csv file where the description of the rectangular grid overlapped on the map is saved.
	 * And example file is given below:
	 * \code
	 * Origin X,Origin Y,X Tile Dim,Y Tile Dim,No Tiles X,No Tiles Y
	 * 0.000000,0.000000,250.000000,250.000000,40,40
	 * \endcode
	 * @return the name of the .csv file where the description of the rectangular grid overlapped on the map is saved.
	 */
	const string& getGridFilename() const;

	/**
	 * Sets the name of the .csv file where the description of the rectangular grid overlapped on the map is saved.
	 * @param gridFilename the name of the .csv file where the description of the rectangular grid overlapped on the map is saved.
	 */
	void setGridFilename(const string& gridFilename);

	/**
	 * Returns the probability distribution of the time between two consecutive stops of a person. Some of the mobility patterns (random_walk, random_walk_with_drift,
	 * levy_flight) consist in a sequence of periods of time when the person move and and periods of time when the person stays in the same position.
	 * The actual time intervals between two consecutive stops are generated from a probability distribution returned by this  function as a pointer to a Distribution object.
	 * The type of the distribution and its parameters are read from the simulation configuration file.
	 * @return the probability distribution of the time between two consecutive stops of a person. Currently only the Exponential distribution is accepted.
	 */
	shared_ptr<Distribution> getIntevalBetweenStays() const;

	/**
	 * Sets the probability distribution of the time between two consecutive stops of a person. Some of the mobility patterns (random_walk, random_walk_with_drift,
	 * levy_flight) consist in a sequence of periods of time when the person move and and periods of time when the person stays in the same position.
	 * The actual time intervals between two consecutive stops are generated from a probability distribution returned by this  function as a pointer to a Distribution object.
	 * The type of the distribution and its parameters are read from the simulation configuration file.
	 * * @param intevalBetweenStays the probability distribution of the time between two consecutive stops of a person. Currently only the Exponential distribution is accepted.
	 */
	void setIntevalBetweenStays(shared_ptr<Distribution> intevalBetweenStays);

	/**
	 * Returns a vector with pointers to MobileOperator objects. The number and the name of each MobileOperator are given in the simulation configuration file.
	 * @return a vector with pointers to MobileOperator objects.
	 */
	const vector<MobileOperator*>& getMnos() const;

	/**
	 * Sets the MobileOperator objects. The number and the name of each MobileOperator are given in the simulation configuration file.
	 * @param mnos a vector with pointers to the MobileOperator objects.
	 */
	void setMnos(const vector<MobileOperator*>& mnos);

	/**
	 * Returns the movement type of a person involved in a simulation, which can take one of the following values given by the enumeration MovementType:
	 *  MovementType::RANDOM_WALK_CLOSED_MAP, MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT, MovementType::LEVY_FLIGHT}, MovementType::HOME_WORK. In the first three
	 *  cases, all persons in a simulation have the same movement pattern while in the last  one( MovementType::HOME_WORK) there is a small percent having a random type of
	 *  movement (MovementType::RANDOM_WALK_CLOSED_MAP, MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT, MovementType::LEVY_FLIGHT) while the rest
	 *  follows a home-work pattern.
	 * @return the movement type of a person.
	 */
	MovementType getMvType() const;

	/**
	 * Sets the movement type of a person involved in a simulation, which can take one of the following values given by the enumeration MovementType:
	 *  MovementType::RANDOM_WALK_CLOSED_MAP, MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT, MovementType::LEVY_FLIGHT}, MovementType::HOME_WORK. In the first three
	 *  cases, all persons in a simulation have the same movement pattern while in the last  one( MovementType::HOME_WORK) there is a small percent having a random type of
	 *  movement (MovementType::RANDOM_WALK_CLOSED_MAP, MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT, MovementType::LEVY_FLIGHT) while the rest
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
	 * Returns the name of the file where the details about all persons involved in a simulation are saved. The first few rows of such a file are presented below:
	 * \code
	 * t,Person ID,x,y,Tile ID,Mobile Phone(s) ID
	 *	0,627,1114.405501,6554.977894,1044
	 *	0,626,7472.134000,4195.835899,669
	 *	0,625,9625.145461,2383.061919,398
	 *	...
	 * \endcode
	 * For each time instant, the following parameters are recorded: the ID of the person, the position on map (x,y), the tile ID of the person's location and the
	 * mobile device ID(s) if the person has mobile device(s).
	 * @return the name of the file where the details about all persons involved in a simulation are saved.
	 */
	const string& getPersonsFilename() const;

	/**
	 * Sets the name of the file where the details about all persons involved in a simulation are saved. The first few rows of such a file are presented below:
	 * \code
	 * t,Person ID,x,y,Tile ID,Mobile Phone(s) ID
	 *	0,627,1114.405501,6554.977894,1044
	 *	0,626,7472.134000,4195.835899,669
	 *	0,625,9625.145461,2383.061919,398
	 *	...
	 * \endcode
	 * For each time instant, the following parameters are recorded: the ID of the person, the position on map (x,y), the tile ID of the person's location and the
	 * mobile device ID(s) if the person has mobile device(s).
	 * @param personsFilename the name of the file where the details about all persons involved in a simulation are saved.
	 */
	void setPersonsFilename(const string& personsFilename);

	/**
	 * Returns a person's probability of have 2 mobile devices.
	 * @return a person's probability of have 2 mobile devices.
	 */
	double getProbSecMobilePhone() const;

	/**
	 * Sets a person's probability of have 2 mobile devices.
	 * @param probSecMobilePhone a person's probability of have 2 mobile devices.
	 */
	void setProbSecMobilePhone(double probSecMobilePhone);


	/**
	 * Returns the seed used to initialize the random number generator.
	 * @return the seed used to initialize the random number generator.
	 */
	unsigned getSeed() const;

	/**
	 * Sets the seed used to initialize the random number generator.
	 * @param seed the seed used to initialize the random number generator.
	 */
	void setSeed(unsigned seed);

	/**
	 * Returns the initial time instant of a simulation measured by the simulation clock.
	 * @returns the initial time instant of a simulation measured by the simulation clock.
	 */
	unsigned long getStartTime() const;


	void setStartTime(unsigned long startTime);


	shared_ptr<Distribution> getStay() const;


	void setStay(shared_ptr<Distribution> stay);


	unsigned long getTimeIncrement() const;


	void setTimeIncrement(unsigned long timeIncrement);


	Clock* getClock();


	void setClock();


	Map* getMap();


	void setMap(Map* map);


	bool isHomeWorkScenario() const;


	unsigned int getNumHomeLocations() const;


	unsigned int getNumWorkLocations() const;


	unsigned int getNumAnchorLocations() const;


	HomeWorkLocation getHomeLocation(unsigned int i) const;


	HomeWorkLocation getWorkLocation(unsigned int i) const;


	HomeWorkScenario* getHomeWorkScenario();


	void setHomeWorkScenario(HomeWorkScenario* hws);


	RandomWalkDriftScenario* getRandomWalkDriftScenario() const;


	void setRandomWalkDriftScenario(RandomWalkDriftScenario *randomWalkDriftScenario);


	LevyFlightScenario* getLevyFlightScenario() const;


	void setLevyFlightScenario(LevyFlightScenario *levyFlightScenario);

private:
	string m_outputDir;
	unsigned long m_startTime;
	unsigned long m_endTime;
	unsigned long m_timeIncrement;
	shared_ptr<Distribution> m_stay;
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
	Clock *m_clock;
	Map *m_map;

	HomeWorkScenario *m_homeWorkScenario;
	RandomWalkDriftScenario* m_randomWalkDriftScenario;
	LevyFlightScenario* m_levyFlightScenario;
};

#endif /* INCLUDE_PARSERS_SIMULATIONCONFIGURATION_H_ */
