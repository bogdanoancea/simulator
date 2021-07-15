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
 * World.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef WORLD_H
#define WORLD_H

#include <agent/Antenna.h>
#include <AntennaInfo.h>
#include <events/EventFactory.h>
#include <parsers/AntennaConfigParser.h>
#include <parsers/PersonsConfigParser.h>
#include <parsers/ProbabilitiesConfigParser.h>
#include <parsers/SimulationConfiguration.h>
#include <parsers/SimulationConfigurationParser.h>
#include <RandomNumberGenerator.h>
#include <PostLocProb.h>
#include <PriorType.h>
#include <map>
#include <memory>
#include <string>
#include <vector>




using namespace std;
using namespace tinyxml2;

/**
 * This is the class where the simulation process takes place. A World object has a Map, a Clock, a set of Agents
 * than can be persons, mobile phones, antennas, mobile operators etc. After generating all the required objects
 * for a simulation by reading the parameters from the input configuration files, the runSimulation() method is
 * called to perform the actual simulation. The output of the simulation process is written in several files.
 * Antenna objects output their registered events in a .csv file and after the simulation ends, these files are merged
 * into a single file that is used to compute the posterior localization probabilities for each mobile device. These probabilities
 * are computed using a Grid that is overlapped on the Map, i.e. we compute the probability of a mobile phone to be in a tile
 * of the grid. A finer the grid means more accurate localization but this come with an important computational cost.
 */
class World {
public:

	/**
	 * Builds a new World object, reading the parameters for the Persons, Antennas and Mobile Phones from configuration files.
	 * The configuration files are in XML format and they should be provided as command line parameters. The general parameters
	 * of the simulation (duration, how people move around the map, how mobile phones try to connect to antennas, etc. are also
	 * read from a configuration file:
	 * 	\li the persons configuration file is provided through the -p parameter in the command line.
	 * 	\li the antennas configuration file is provided through the -a parameter in the command line.
	 * 	\li the simulation configuration file is provided through the -s parameter in the command line.
	 * 	\li the posterior probabilities configuration file is provided through the -pb parameter in the command line.
	 *
	 * @param map a pointer to a Map object where the simulation takes place.
	 * @param configPersonsFileName the configuration file name for the persons objects.
	 * @param configAntennasFileName the configuration file name for antenna objects.
	 * @param configSimulationFileName the general configuration file for a simulation.
	 * @param probabilitiesFileName the configuration file for the posterior location probabilities.
	 */
	World(Map* map, const string& configPersonsFileName, const string& configAntennasFileName, const string& configSimulationFileName,
			const string& probabilitiesFileName) noexcept(false);

	/**
	 * Destructor. It releases the memory allocated for the agents collection, the event factory and the SimulationConfiguration object.
	 */
	virtual ~World();

	/**
	 * This method is called to perform the actual simulation. During the simulation it outputs the exact positions
	 * of all persons in a .csv file and the positions of antennas at the starting time of the simulation. A simulation
	 * means a number of time steps, at each step every person move to another position and after arriving at their new
	 * positions the mobile phones that they carry try to connect to one of the available antennas. The antennas record
	 * these events and output them in a file.
	 */
	void runSimulation() noexcept(false);

	/**
	 * Returns the AgentsCollection used in simulation.
	 * @return a pointer to AgentsCollection object.
	 */
	AgentsCollection* getAgents() const;

	/**
	 * Returns a pointer to a Map object where the simulation takes place.
	 * @return a pointer to a Map object where the simulation takes place.
	 */
	const Map* getMap() const;

	/**
	 * Returns the file name where the grid parameters are saved. They are needed for the visualization software.
	 * @return the file name where the grid parameters are saved.
	 */
	const string& getGridFilename() const;

	/**
	 * Returns a pointer to the Clock object of the simulation.
	 * @return a pointer to the Clock object of the simulation.
	 */
	Clock* getClock();

	/**
	 * Computes the posterior location probabilities for each device.
	 * @param data a vector with all the network events recorded during a simulation.
	 */
	void computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data);


	/**
	 * Returns the name of the output folder. If this folder does not exists, it will be created.
	 * @return the name of the output folder.
	 */
	const string& getOutputDir() const;

	/**
	 * At the end of a simulation this method merges all the events saved by individual antennas into a single data structure.
	 * @param computeProb if TRUE returns a list of all events needed to compute the location probabilities.
	 * @return a map of <MNO_ID, vector<AntennInfo>> where each MNO identified by its ID has a
	 * vector of all events saved by all antennas belonging to that MNO. This map is needed for computation of location probabilities.
	 */
	std::map<unsigned long, vector<AntennaInfo>> getEvents(bool computeProb);

private:
	AgentsCollection* m_agentsCollection;
	SimulationConfiguration* m_sp;
	shared_ptr<PostLocProb> m_postMethod;
	EventFactory* m_eventFactory;

	int whichMNO(vector<pair<string, double>> probs, vector<MobileOperator*> mnos);
	string parseProbabilities(const string& probabilitiesFileName);
	void writeSignalAndCells(ostream& antennaFile);
	HoldableAgent::CONNECTION_TYPE getConnectionType() const;
};

#endif // WORLD_H
