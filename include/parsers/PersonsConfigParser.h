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
 * PersonsConfig.h
 *
 *  Created on: Mar 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PERSONSCONFIG_H_
#define INCLUDE_PERSONSCONFIG_H_

#include <agent/MobileOperator.h>
#include <agent/Person.h>
#include <Distribution.h>
#include <parsers/ConfigParser.h>
#include <parsers/SimulationConfiguration.h>
#include <RandomNumberGenerator.h>
#include <memory>
#include <string>
#include <vector>

/**
 * Parses the persons configuration file specified by "-p" option in the command line and builds the Person objects.
 * This file is named persons.xml in all example configuration folders.
 * After it parses the configuration file, it builds Person objects and adds them to the AgentsCollection container.
 */
class PersonsConfigParser: public ConfigParser {
public:
	/**
	 * Constructor of the class. It store internally the value of the parameters.
	 * @param fileName the name of the persons configuration file, passed with the "-p" option in the command line.
	 * @param sc  a pointer to a SimulationConfiguration object. This object is needed for different parameters :the map and the
	 *  clock of the simulation, the MobileOperator objects, a person's probability of having 2 mobile devices.
	 * @param ag a pointer to the AgentsCollection container class created by the constructor of the World class.
	 */
	PersonsConfigParser(const string& fileName, SimulationConfiguration* sc, AgentsCollection* ag);

	/**
	 *  Default destructor.
	 */
	virtual ~PersonsConfigParser();

	/**
	 * A method to parse the persons configuration file. After it parses this file, it builds a PersonConfiguration object
	 * and passes it to the constructor of the Person class. All Person objects are added to the
	 * AgentsCollection container class.
	 * It reads the total number of persons, the probability distribution for the speed and age of persons, the male share and
	 * the percent of the people that will have the same initial location on map at each simulation (home persons). Then it generates the Person objects
	 * by generating an initial position on map, a speed according to the speed distribution , an age, it assigns 1 or 2 mobile devices and sets the displacement pettern.
	 */
	void parse() override;
private:


	vector<Person*> generatePopulation(const unsigned long numPersons, shared_ptr<Distribution> age_distribution,
				double male_share, shared_ptr<Distribution> speed_walk, shared_ptr<Distribution> speed_car, double percentHome);
	void setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng );
	int* generateAges(int n, shared_ptr<Distribution> distr, RandomNumberGenerator* rng);
	void addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag);
	void setPersonDisplacementPattern(Person* p);
	Point* generateLocation(unsigned int index, vector<HomeWorkLocation> locations);
	shared_ptr<Distribution> parseAgeDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseSpeedWalkDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseSpeedCarDistribution(XMLElement* parent);
	void setHomePersonHWLocations(Person* p, Point* pt);
	SimulationConfiguration* m_simConfig;
	AgentsCollection* m_agents;


};

#endif /* INCLUDE_PERSONSCONFIG_H_ */
