/*
 * PersonsConfig.cpp
 *
 *  Created on: Mar 5, 2021
 *      Author: bogdan
 */

#include <agent/AgentsCollection.h>
#include <agent/MobilePhone.h>
#include <Clock.h>
#include <Constants.h>
#include <geos/geom/Point.h>
#include <IDGenerator.h>
#include <LevyFlightDisplacement.h>
#include <map/Map.h>
#include <MovementType.h>
#include <parsers/PersonsConfig.h>
#include <parsers/SimConfig.h>
#include <RandomNumberGenerator.h>
#include <RandomWalkDisplacement.h>
#include <RandomWalkDriftDisplacement.h>
#include <HomeWorkDisplacement.h>
#include <TinyXML2.h>
#include <TruncatedNormalAgeDistribution.h>
#include <UniformAgeDistribution.h>
#include <Utils.h>
#include <cstring>
#include <numeric>
#include <stdexcept>
#include <typeinfo>

using namespace tinyxml2;
using namespace utils;

PersonsConfig::PersonsConfig(const string& filename, SimConfig* sc, AgentsCollection* ag) : Config(filename) {
	//m_mnos = mnos;
	m_simConfig = sc;
	m_agents = ag;
	parse();
	for (unsigned long i = 0; i < m_persons.size(); i++) {
			m_agents->addAgent(m_persons[i]);
	}
}

PersonsConfig::~PersonsConfig() {
	// TODO Auto-generated destructor stub
}

void PersonsConfig::parse() noexcept(false) {

	XMLDocument doc;
	XMLError err = doc.LoadFile(getFileName().c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for persons ");

	XMLElement* personsEl = doc.FirstChildElement("persons");
	if (!personsEl)
		throw std::runtime_error("Syntax error in the configuration file for persons ");
	else {
		unsigned long numPersons = getValue(personsEl, "num_persons", Constants::SIM_NO_PERSONS);
		unsigned long min_age = getValue(personsEl, "min_age");
		unsigned long max_age = getValue(personsEl, "max_age");

		XMLElement* ageDistribEl = getFirstChildElement(personsEl, "age_distribution");
		const char* distrib = getValue(ageDistribEl, "type", "UNKNOWN");
		if (strcmp(distrib, "normal") && strcmp(distrib, "uniform"))
			throw std::runtime_error("Unknown age distribution for population!");

		shared_ptr<AgeDistribution> ageDistr;
		if (!strcmp(distrib, "normal")) {
			double mean_age = getValue(ageDistribEl, "mean");
			double sd = getValue(ageDistribEl, "sd");
			ageDistr = make_shared<TruncatedNormalAgeDistribution>(TruncatedNormalAgeDistribution(min_age, max_age, mean_age, sd));
		} else if (!strcmp(distrib, "uniform")) {
			ageDistr = make_shared<UniformAgeDistribution>(UniformAgeDistribution(min_age, max_age));
		} else {
			ageDistr = make_shared<UniformAgeDistribution>(UniformAgeDistribution(min_age, max_age));
		}

		double male_share = getValue(personsEl, "male_share");
		double speed_walk = getValue(personsEl, "speed_walk");
		double speed_car = getValue(personsEl, "speed_car");
		double percentHome = getValue(personsEl, "percent_home");
		m_persons = generatePopulation(numPersons, ageDistr, male_share, speed_walk, speed_car, percentHome);
	}
}


vector<Person*> PersonsConfig::generatePopulation(unsigned long numPersons, shared_ptr<AgeDistribution> ageDistribution,
		double male_share, double speed_walk, double speed_car, double percentHome) {

	vector<Person*> result;
	unsigned long id;
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_simConfig->getSeed());
	int *phone1 = nullptr, *phone2 = nullptr;
	setPhones(phone1, phone2, m_simConfig->getProbSecMobilePhone(), numPersons, random_generator);

	int* walk_car = random_generator->generateBernoulliInt(0.5, numPersons);
	int sum = 0;
	sum = accumulate(walk_car, walk_car + numPersons, sum);
	int* gender = random_generator->generateBinomialInt(1, male_share, numPersons);
	double* speeds_walk = random_generator->generateNormalDouble(speed_walk, 0.1 * speed_walk, numPersons - sum);
	double* speeds_car = random_generator->generateNormalDouble(speed_car, 0.1 * speed_car, sum);
	int* ages = generateAges(numPersons, ageDistribution, random_generator );
	unsigned long cars = 0, walks = 0;
	Person* p;
	vector<Point*> positions = utils::generatePoints(m_simConfig, numPersons, percentHome);
	unsigned int numMno =  m_simConfig->getMnos().size();
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		unsigned long stay = (unsigned long) random_generator->generateNormalDouble(m_simConfig->getStay(), 0.2 * m_simConfig->getStay());
		unsigned long interval = (unsigned long) random_generator->generateExponentialDouble(1.0 / m_simConfig->getIntevalBetweenStays());
		if (walk_car[i]) {
			p = new Person(m_simConfig->getMap(), id, positions[i], m_simConfig->getClock(), speeds_car[cars++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		} else {
			p = new Person(m_simConfig->getMap(), id, positions[i], m_simConfig->getClock(), speeds_walk[walks++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		}
		int np1 = phone1[i];
		while (np1) {
			addMobilePhoneToPerson(p, m_simConfig->getMnos()[0], m_agents);
			np1--;
		}
		if (numMno == 2) {
			int np2 = phone2[i];
			while (np2) {
				addMobilePhoneToPerson(p, m_simConfig->getMnos()[1], m_agents);
				np2--;
			}
		}
		setPersonDisplacementPattern(p);
		result.push_back(p);
	}
	delete[] walk_car;
	delete[] phone1;
	delete[] phone2;
	delete[] speeds_walk;
	delete[] speeds_car;
	delete[] ages;
	delete[] gender;
	return (result);
}


void PersonsConfig::setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng ) {
	int numMno = m_simConfig->getMnos().size();
	double probIntersection = 1;
	for (auto& n : m_simConfig->getMnos()) {
		probIntersection *= n->getProbMobilePhone();
	}
	if (numMno > 1 && probIntersection > probSecMobilePhone)
		throw std::runtime_error(
				"Error specifying probabilities of having mobile phones: the probability of having a second mobile phone should be greater than or equal to the product of the probabilities of having mobile phones at each MNO");

	if (numMno == 1) {
		ph1 = rng->generateBernoulliInt(m_simConfig->getMnos()[0]->getProbMobilePhone(), numPersons);
		for (unsigned long j = 0; j < numPersons; j++) {
			if (ph1[j] == 1)
				ph1[j] += rng->generateBernoulliInt(probSecMobilePhone);
		}
	} else if (numMno == 2) {
		double pSecPhoneMNO1 = (probSecMobilePhone - probIntersection) / 2.0;
		double pSecPhoneMNO2 = pSecPhoneMNO1;

		double pOnePhoneMNO1 = m_simConfig->getMnos()[0]->getProbMobilePhone() - pSecPhoneMNO1;
		double pOnePhoneMNO2 = m_simConfig->getMnos()[1]->getProbMobilePhone() - pSecPhoneMNO2;

		ph1 = rng->generateBernoulliInt(pOnePhoneMNO1, numPersons);
		ph2 = rng->generateBernoulliInt(pOnePhoneMNO2, numPersons);
		for (unsigned long i = 0; i < numPersons; i++) {
			if(ph1[i] == 1 && ph2[i] == 1)
				continue;
			if(ph1[i] == 1)
				ph1[i] += rng->generateBernoulliInt(pSecPhoneMNO1/pOnePhoneMNO1);
			if(ph2[i] == 1)
				ph2[i] += rng->generateBernoulliInt(pSecPhoneMNO2/pOnePhoneMNO2);
		}
	} else {
		throw std::runtime_error("Number of MNOs supported should be 1 or 2!");
	}
}


int* PersonsConfig::generateAges(int n, shared_ptr<AgeDistribution> distr, RandomNumberGenerator* rng) {
	int* ages = new int[n];

	if(shared_ptr<UniformAgeDistribution> ageDistr = dynamic_pointer_cast<UniformAgeDistribution>(distr)){
		ages = rng->generateUniformInt(ageDistr->getMinAge(),ageDistr->getMaxAge(), n);
	}
	else if (shared_ptr<TruncatedNormalAgeDistribution> ageDistr = dynamic_pointer_cast<TruncatedNormalAgeDistribution>(distr)) {
		ages = rng->generateTruncatedNormalInt(ageDistr->getMinAge(),ageDistr->getMaxAge(), ageDistr->getMean(),ageDistr->getSd(), n);
	} else {
		ages = rng->generateUniformInt(distr->getMinAge(),distr->getMaxAge(), n);
	}
	return (ages);
}

void PersonsConfig::addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag) {
	unsigned long id = IDGenerator::instance()->next();
	MobilePhone* mp = new MobilePhone(m_simConfig->getMap(), id, nullptr, nullptr, m_simConfig->getClock(), m_simConfig->getConnThreshold(), m_simConfig->getConnType());
	mp->setMobileOperator(mno);
	mp->setHolder(p);
	ag->addAgent(mp);
	p->addDevice(typeid(MobilePhone).name(), mp);
}


void PersonsConfig::setPersonDisplacementPattern(Person* p) {
	MovementType type = m_simConfig->getMvType();
	if (type == MovementType::RANDOM_WALK_CLOSED_MAP) {
		auto displace = std::make_shared<RandomWalkDisplacement>(m_simConfig, p->getSpeed());
		p->setDisplacementMethod(displace);
	} else if (type == MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT) {
		auto displace = std::make_shared<RandomWalkDriftDisplacement>(m_simConfig, p->getSpeed());
		p->setDisplacementMethod(displace);
	} else if (type == MovementType::LEVY_FLIGHT) {
		auto displace = std::make_shared<LevyFlightDisplacement>(m_simConfig, p->getSpeed());
		p->setDisplacementMethod(displace);
	}
	else if (type == MovementType::HOME_WORK) {
			auto displace = std::make_shared<HomeWorkDisplacement>(m_simConfig, p->getSpeed());
			p->setDisplacementMethod(displace);
	}
}




const vector<Person*>& PersonsConfig::getPersons() const {
	return m_persons;
}


