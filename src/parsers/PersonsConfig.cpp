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
#include <RandomNumberGenerator.h>
#include <RandomWalkDisplacement.h>
#include <RandomWalkDriftDisplacement.h>
#include <HomeWorkDisplacement.h>
#include <parsers/PersonsConfigParser.h>
#include <parsers/SimConfigParser.h>
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

PersonsConfig::PersonsConfig(const string& filename, SimConfigParser* sc, AgentsCollection* ag) : ConfigParser(filename) {
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

		shared_ptr<Distribution> ageDistr;
		if (!strcmp(distrib, "normal")) {
			double mean_age = getValue(ageDistribEl, "mean");
			double sd = getValue(ageDistribEl, "sd");
			vector<pair<const char *, double>> ageDistrParams;
			std::pair<const char*, double> p1 = std::make_pair("min", min_age);
			std::pair<const char*, double> p2 = std::make_pair("max", max_age);
			std::pair<const char*, double> p3 = std::make_pair("mean", mean_age);
			std::pair<const char*, double> p4 = std::make_pair("sd", sd);
			ageDistrParams.push_back(p1);
			ageDistrParams.push_back(p2);
			ageDistrParams.push_back(p3);
			ageDistrParams.push_back(p4);
			DistributionType type = DistributionType::TRUNCATED_NORMAL;
			ageDistr = make_shared<Distribution>(Distribution(type, ageDistrParams));
		} else if (!strcmp(distrib, "uniform")) {
			DistributionType type = DistributionType::UNIFORM;
			vector<pair<const char *, double>> ageDistrParams;
			std::pair<const char*, double> p1 = std::make_pair("min", min_age);
			std::pair<const char*, double> p2 = std::make_pair("max", min_age);
			ageDistrParams.push_back(p1);
			ageDistrParams.push_back(p2);
			ageDistr = make_shared<Distribution>(Distribution(type, ageDistrParams));
		} else { // default distribution
			DistributionType type = DistributionType::UNIFORM;
			vector<pair<const char *, double>> ageDistrParams;
			std::pair<const char*, double> p1 = std::make_pair("min", min_age);
			std::pair<const char*, double> p2 = std::make_pair("max", min_age);
			ageDistrParams.push_back(p1);
			ageDistrParams.push_back(p2);
			ageDistr = make_shared<Distribution>(Distribution(type, ageDistrParams));
		}
		double male_share = getValue(personsEl, "male_share");
		double speed_walk = getValue(personsEl, "speed_walk");
		double speed_car = getValue(personsEl, "speed_car");
		double percentHome = getValue(personsEl, "percent_home");
		m_persons = generatePopulation(numPersons, ageDistr, male_share, speed_walk, speed_car, percentHome);
	}
}


vector<Person*> PersonsConfig::generatePopulation(unsigned long numPersons, shared_ptr<Distribution> ageDistribution,
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
		//cout << "home positions" << positions[i]->toString() << endl;
		id = IDGenerator::instance()->next();
		unsigned long stay = (unsigned long) random_generator->generateNormalDouble(m_simConfig->getStay(), 0.2 * m_simConfig->getStay());
		unsigned long interval = (unsigned long) random_generator->generateExponentialDouble(1.0 / m_simConfig->getIntevalBetweenStays());
		if (walk_car[i]) {
			p = new Person(m_simConfig->getMap(), id, positions[i], m_simConfig->getClock(), speeds_car[cars++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
			if(m_simConfig->isHomeWorkScenario()) {
				 bool homePerson = RandomNumberGenerator::instance()->generateBernoulliInt(percentHome);
				 if( homePerson) {
					 Point* hl = m_simConfig->getMap()->getGlobalFactory()->createPoint(positions[i]->getCoordinates());
					 p->setHomeLocation(hl);
					 int workLocationIndex = RandomNumberGenerator::instance()->generateUniformInt(0,  m_simConfig->getNumWorkLocations() - 1);
					 Point* workLocation = generateWorkLocation(workLocationIndex);
					 p->setWorkLocation(workLocation);
				 }
			}
		} else {
			p = new Person(m_simConfig->getMap(), id, positions[i], m_simConfig->getClock(), speeds_walk[walks++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
			if(m_simConfig->isHomeWorkScenario()) {
				 bool homePerson = RandomNumberGenerator::instance()->generateBernoulliInt(percentHome);
				 if( homePerson) {
					 Point* hl = m_simConfig->getMap()->getGlobalFactory()->createPoint(positions[i]->getCoordinates());
					 p->setHomeLocation(hl);
					 int workLocationIndex = RandomNumberGenerator::instance()->generateUniformInt(0,  m_simConfig->getNumWorkLocations() - 1);
					 Point* workLocation = generateWorkLocation(workLocationIndex);
					 p->setWorkLocation(workLocation);
				 }
			}
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


int* PersonsConfig::generateAges(int n, shared_ptr<Distribution> distr, RandomNumberGenerator* rng) {
	int* ages = new int[n];
	ages = rng->generateInt(n, *distr.get());
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
		if(p->isHomePerson()) {
			auto displace1 = std::make_shared<HomeWorkDisplacement>(m_simConfig, p->getSpeed(), p->getHomeLocation(), p->getWorkLocation());
			p->setDisplacementMethod(displace1);
		}
		else {
			auto displace2 = std::make_shared<RandomWalkDisplacement>(m_simConfig, p->getSpeed());
			p->setDisplacementMethod(displace2);
		}

	}
}


const vector<Person*>& PersonsConfig::getPersons() const {
	return m_persons;
}

Point* PersonsConfig::generateWorkLocation(unsigned int index) {
	Point* result = nullptr;
	double angle = RandomNumberGenerator::instance()->generateUniformDouble(0, 2.0 * utils::PI);
	HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(index);
	double L = sqrt( 1.0/(pow(cos(angle),2)/wl.m_sdx + pow(sin(angle),2)/wl.m_sdy) );
	double l = RandomNumberGenerator::instance()->generateUniformDouble(0, L);
	double x = wl.m_x + l * cos(angle);
	double y = wl.m_y + l * sin(angle);
	Coordinate c1(x, y, wl.m_z);
	result = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);

	return result;
}


