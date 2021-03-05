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
#include <parsers/Config.h>
#include <memory>
#include <string>
#include <vector>

class AgentsCollection;

class RandomNumberGenerator;

class SimConfig;

class PersonsConfig: public Config {
public:
	PersonsConfig(const string& fileName, SimConfig* sc, AgentsCollection* ag);
	virtual ~PersonsConfig();
	double getMaleShare() const;
	void setMaleShare(double maleShare);
	unsigned getMaxAge() const;
	void setMaxAge(unsigned maxAge);
	unsigned getMinAge() const;
	void setMinAge(unsigned minAge);
	unsigned getNumPersons() const;
	void setNumPersons(unsigned numPersons);
	double getPercentHome() const;
	void setPercentHome(double percentHome);
	const vector<Person*>& getPersons() const;
	void setPersons(const vector<Person*> &persons);
	double getSpeedCar() const;
	void setSpeedCar(double speedCar);
	double getSpeedWalk() const;
	void setSpeedWalk(double speedWalk);

private:
	void parse();
	vector<Person*> m_persons;
	//vector<MobileOperator*> m_mnos;
	vector<Person*> generatePopulation(const unsigned long numPersons, shared_ptr<AgeDistribution> age_distribution,
				double male_share, double speed_walk, double speed_car, double percentHome);
	void setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng );
	int* generateAges(int n, shared_ptr<AgeDistribution> distr, RandomNumberGenerator* rng);
	void addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag);
	void setPersonDisplacementPattern(Person* p);
	SimConfig* m_simConfig;
	AgentsCollection* m_agents;

	unsigned m_numPersons;
	unsigned m_minAge;
	unsigned m_maxAge;
	double m_maleShare;
	double m_speedWalk;
	double m_speedCar;
	double m_percentHome;
};

#endif /* INCLUDE_PERSONSCONFIG_H_ */
