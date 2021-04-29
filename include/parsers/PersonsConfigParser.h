/*
 * PersonsConfig.h
 *
 *  Created on: Mar 5, 2021
 *      Author: bogdan
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


class PersonsConfigParser: public ConfigParser {
public:
	PersonsConfigParser(const string& fileName, SimulationConfiguration* sc, AgentsCollection* ag);
	virtual ~PersonsConfigParser();
	const vector<Person*>& getPersons() const;

private:
	void parse() override;
	vector<Person*> m_persons;
	vector<Person*> generatePopulation(const unsigned long numPersons, shared_ptr<Distribution> age_distribution,
				double male_share, shared_ptr<Distribution> speed_walk, shared_ptr<Distribution> speed_car, double percentHome);
	void setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng );
	int* generateAges(int n, shared_ptr<Distribution> distr, RandomNumberGenerator* rng);
	void addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag);
	void setPersonDisplacementPattern(Person* p);
	Point* generateWorkLocation(unsigned int index);
	shared_ptr<Distribution> parseAgeDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseSpeedWalkDistribution(XMLElement* parent);
	shared_ptr<Distribution> parseSpeedCarDistribution(XMLElement* parent);
	SimulationConfiguration* m_simConfig;
	AgentsCollection* m_agents;


};

#endif /* INCLUDE_PERSONSCONFIG_H_ */