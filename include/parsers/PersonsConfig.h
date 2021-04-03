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
#include <AgeDistribution.h>
#include <RandomNumberGenerator.h>
#include <parsers/Config.h>
#include <parsers/SimConfig.h>
#include <memory>
#include <string>
#include <vector>


class PersonsConfig: public Config {
public:
	PersonsConfig(const string& fileName, SimConfig* sc, AgentsCollection* ag);
	virtual ~PersonsConfig();
	const vector<Person*>& getPersons() const;

private:
	void parse();
	vector<Person*> m_persons;
	vector<Person*> generatePopulation(const unsigned long numPersons, shared_ptr<AgeDistribution> age_distribution,
				double male_share, double speed_walk, double speed_car, double percentHome);
	void setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng );
	int* generateAges(int n, shared_ptr<AgeDistribution> distr, RandomNumberGenerator* rng);
	void addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag);
	void setPersonDisplacementPattern(Person* p);
	Point* generateWorkLocation(unsigned int index);
	SimConfig* m_simConfig;
	AgentsCollection* m_agents;


};

#endif /* INCLUDE_PERSONSCONFIG_H_ */
