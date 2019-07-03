/*
 * Data simulator for mobile phone network events
 *
 * World.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef WORLD_H
#define WORLD_H

#include <Antenna.h>
#include <Person.h>
#include <random>
#include <vector>
#include <AgentsCollection.h>
#include <Clock.h>
#include <MobilePhone.h>
#include <MovementType.h>
#include <TinyXML2.h>
#include <PriorType.h>
#include <MobileOperator.h>

using namespace std;
using namespace tinyxml2;

/**
 * This is the class where all the simulation process takes place. A World object has a map, a clock, a set of agents
 * than can be persons, mobile phones, antennas etc. After generating all the required objects for simulation the runSimulation()
 * method is called to perform the actual simulation. The output of the simulation process is written in several files.
 * Each antenna objects output their registered events in a csv file and after the simulation ends, these files are merged
 * in a single file that is used to computed the posterior localization probabilities for each mobile device. These probabilities
 * are computed using a grid that is overlapped on the map, i.e. we compute the probability of a mobile phone to be in a tile
 * of the grid. A finer the grid mean more accurate localization.
 */
class World {
public:

	/**
	 * Builds a new World object, randomly generating a set of persons, antenna and mobile devices with the default parameters
	 * @param map a pointer to a Map object where the simulation takes place
	 * @param numPersons the number of persons to be generated
	 * @param numAntennas the number of antennas to be generated
	 * @param numMobilePhones the number of mobile phones to be generated. These phones are randomly given to persons.
	 */
	World(Map* map, int numPersons, int numAntennas, int numMobilePhones);

	/**
	 * Builds a new World object, reading the parameters for the Persons, Antennas and Mobile phone from configuration files.
	 * The configuration files are in XML format and they should be provided as command line parameters. The general parameters
	 * of the simulation (duration, how people move around the map, how mobile phone try to connect to antennas etc are also
	 * read from a configuration file
	 * @param map a pointer to a Map object where the simulation takes place
	 * @param configPersonsFileName the configuration file name for the persons object
	 * @param configAntennasFileName the configuration file name for antenna objects
	 * @param configSimulationFileName the general configuration file for simulation
	 */
	World(Map* map, const string& configPersonsFileName, const string& configAntennasFileName, const string& configSimulationFileName)
			noexcept(false);

	/**
	 * Destructor
	 * It releases the memory allocated for the agents collection and the the Clock object.
	 */
	virtual ~World();

	/**
	 * This method is called to perform the actual simulation. During the simulation it outputs the exact positions
	 * of all persons in a csv file and the positions of antennas at the starting time of the simulation. A simulation
	 * means a number of time steps, at each step every person move to another position and after arriving at their new
	 * position the mobile phones that they carry try to connect to one of the available antennas.
	 */
	void runSimulation() noexcept(false);

	/**
	 * Returns the AgentsCollection used in simulation
	 * @return a pointer to AgentsCollection object
	 */
	AgentsCollection* getAgents() const;

	/**
	 * Sets the AgentsCollection to be used for simulation
	 * @param agents a pointer to AgentsCollection object
	 */
	void setAgents(AgentsCollection* agents);

	/**
	 * Returns a pointer to a Clock object used for simulation
	 * @return a pointer to a Clock object used for simulation
	 */
	Clock* getClock() const;

	/**
	 * Sets the Clock of the simulation
	 * @param clock a pointer to a Clock object used for simulation
	 */
	void setClock(Clock* clock);

	/**
	 * Returns a pointer to a Map object where the simulation takes place
	 * @return a pointer to a Map object where the simulation takes place
	 */
	Map* getMap() const;

	/**
	 * Sets the map where the simulation takes place
	 * @param map a pointer to a Map object where the simulation takes place
	 */
	void setMap(Map* map);

	/**
	 * Returns the file name where the grid parameters are saved. They are needed for the visualization software.
	 * @return the file name where the grid parameters are saved.
	 */
	const string& getGridFilename() const;

	/**
	 * Returns the name of the file where the probabilities of mobile phones location are saved
	 * @return the name of the file where the probabilities of mobile phones location are saved
	 */
	const string& getProbFilename() const {
		return m_probFilename;
	}

	/**
	 * Returns the name of the file where the antennas exact locations are saved for later analysis
	 * @return the name of the file where the antennas exact locations are saved for later analysis
	 */
	const string& getAntennasFilename() const;

	/**
	 * Returns the name of the file where the persons exact locations are saved for later analysis
	 * @return the name of the file where the persons exact locations are saved for later analysis
	 */
	const string& getPersonsFilename() const;

	/**
	 * Returns the dimension of tiles on OX, this number is read from simulation.xml config file
	 * @return the dimension of tiles on OX, this number is read from simulation.xml config file
	 */
	unsigned long getGridDimTileX() const;

	/**
	 * Returns the dimension of tiles on OY, this number is read from simulation.xml config file
	 * @return the dimension of tiles on OY, this number is read from simulation.xml config file
	 */
	unsigned long getGridDimTileY() const;

	/**
	 * Returns the type of the prior probability used to compute the posterior localization probability
	 * @return the type of the prior probability used to compute the posterior localization probability
	 */
	PriorType getPrior() const;
	unsigned int getNumMno() const;
	void setNumMno(unsigned int numMno);
	void addMNO(string name);


private:

	Map* m_map;

	AgentsCollection* m_agentsCollection;
	Clock* m_clock;
	unsigned long m_startTime;
	unsigned long m_endTime;
	unsigned long m_timeIncrement;
	unsigned long m_GridDimTileX;
	unsigned long m_GridDimTileY;
	PriorType m_prior;
	unsigned int m_numMNO;
	vector<string> m_mnoNames;

	HoldableAgent::CONNECTION_TYPE m_connType;
	MovementType m_mvType;
	string m_gridFilename;
	string m_probFilename;
	string m_personsFilename;
	string m_antennasFilename;


	vector<Person*> generatePopulation(unsigned long numPersons);

	vector<Person*> generatePopulation(unsigned long numPersons, vector<double> params, Person::AgeDistributions age_distribution, double male_share, vector<pair<string, double>> probMobilePhone,
			double probSecMobilePhone, double probSecMobilePhoneSameMNO, double speed_walk, double speed_car);

	vector<Antenna*> generateAntennas(unsigned long numAntennas);
	vector<Antenna*> parseAntennas(const string& configAntennasFile) noexcept(false);
	vector<Person*> parsePersons(const string& personsFileName) noexcept(false);
	vector<MobilePhone*> generateMobilePhones(int numMobilePhones, HoldableAgent::CONNECTION_TYPE connType);
	vector<MobileOperator*> parseSimulationFile(const string& configSimulationFileName) noexcept(false);

};

#endif // WORLD_H
