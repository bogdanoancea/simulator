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
 * PersonsConfig.cpp
 *
 *  Created on: Mar 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/AgentsCollection.h>
#include <agent/MobilePhone.h>
#include <Clock.h>
#include <Constants.h>
#include <DistributionType.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <HomeWorkDisplacement.h>
#include <IDGenerator.h>
#include <LevyFlightDisplacement.h>
#include <map/Map.h>
#include <MovementType.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/HomeWorkScenario.h>
#include <parsers/PersonsConfigParser.h>
#include <RandomWalkDisplacement.h>
#include <RandomWalkDriftDisplacement.h>
#include <ManhattanDisplacement.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <cmath>
#include <cstring>
#include <numeric>
#include <stdexcept>
#include <typeinfo>

using namespace tinyxml2;
using namespace utils;

PersonsConfigParser::PersonsConfigParser(const string& filename, SimulationConfiguration* sc, AgentsCollection* ag) : ConfigParser(filename) {
	m_simConfig = sc;
	m_agents = ag;
}

PersonsConfigParser::~PersonsConfigParser() {
	// TODO Auto-generated destructor stub
}

void PersonsConfigParser::parse() noexcept(false) {

	XMLDocument doc;
	XMLError err = doc.LoadFile(getFileName().c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for persons ");

	XMLElement* personsEl = doc.FirstChildElement("persons");
	if (!personsEl)
		throw std::runtime_error("Syntax error in the configuration file for persons ");
	else {
		unsigned long numPersons = getValue(personsEl, "num_persons", Constants::SIM_NO_PERSONS);
		shared_ptr<Distribution> ageDistr = parseAgeDistribution(personsEl);
		shared_ptr<Distribution> speedWalkDistribution = parseSpeedWalkDistribution(personsEl);
		shared_ptr<Distribution> speedCarDistribution = parseSpeedCarDistribution(personsEl);
		double male_share = getValue(personsEl, "male_share");
		double percentHome = getValue(personsEl, "percent_home");
		vector<Person* >persons = generatePopulation(numPersons, ageDistr, male_share, speedWalkDistribution, speedCarDistribution, percentHome);
		for (unsigned long i = 0; i < persons.size(); i++) {
				m_agents->addAgent(persons[i]);
		}

	}
}


shared_ptr<Distribution> PersonsConfigParser::parseAgeDistribution(XMLElement* parent) {
	shared_ptr<Distribution> ageDistr;
	XMLElement *distributionEl = parent->FirstChildElement("age");
	if (distributionEl) {
		const XMLAttribute *type = distributionEl->FindAttribute("distributionType");
		DistributionType dType;
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else if (!strcmp(dname, "TruncatedNormal")) {
				dType = DistributionType::TRUNCATED_NORMAL;
			} else {
				throw std::runtime_error("Unknown/unsupported age distribution");
			}
		} else {
			throw std::runtime_error("Age distribution type not specified ");
		}
		ageDistr = make_shared<Distribution>(Distribution(dType, distributionEl));
	} else {
		throw std::runtime_error("Age distribution not specified ");
	}
	return ageDistr;
}

shared_ptr<Distribution> PersonsConfigParser::parseSpeedWalkDistribution(XMLElement* parent) {
	shared_ptr<Distribution> result;
	XMLElement *speedWEl = parent->FirstChildElement("speed_walk");
	if (speedWEl) {
		const XMLAttribute *type = speedWEl->FindAttribute("distributionType");
		DistributionType dType;
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else if (!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			} else {
				throw std::runtime_error("Unknown/unsupported speed distribution");
			}
		} else {
			throw std::runtime_error("Speed distribution type not specified ");
		}
		result = make_shared<Distribution>(Distribution(dType, speedWEl));
	} else {
		throw std::runtime_error("Speed distribution not specified ");
	}
	return result;
}

shared_ptr<Distribution> PersonsConfigParser::parseSpeedCarDistribution(XMLElement *parent) {
	shared_ptr<Distribution> result;
	XMLElement *speedCEl = parent->FirstChildElement("speed_car");
	if (speedCEl) {
		const XMLAttribute *type = speedCEl->FindAttribute("distributionType");
		DistributionType dType;
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else if (!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			} else {
				throw std::runtime_error("Unknown/unsupported speed distribution");
			}
		} else {
			throw std::runtime_error("Speed distribution type not specified ");
		}
		result = make_shared<Distribution>(Distribution(dType, speedCEl));
	} else {
		throw std::runtime_error("Speed distribution not specified ");
	}
	return result;
}


vector<Person*> PersonsConfigParser::generatePopulation(unsigned long numPersons, shared_ptr<Distribution> ageDistribution,
		double male_share, shared_ptr<Distribution> speed_walk, shared_ptr<Distribution> speed_car, double percentHome) {

	vector<Person*> result;
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_simConfig->getSeed());

	int *phone1 = nullptr, *phone2 = nullptr;
	setPhones(phone1, phone2, m_simConfig->getProbSecMobilePhone(), numPersons, random_generator);
	int* walk_car = random_generator->generateBernoulliInt(0.5, numPersons);
	int sum = 0;
	sum = accumulate(walk_car, walk_car + numPersons, sum);
	int* gender = random_generator->generateBinomialInt(1, male_share, numPersons);
	double* speeds_walk = random_generator->generateDouble(numPersons - sum, speed_walk.get());
	double* speeds_car = random_generator->generateDouble(sum, speed_car.get());
	int* ages = generateAges(numPersons, ageDistribution, random_generator );
	unsigned long cars = 0, walks = 0;
	vector<Point*> positions = utils::generatePoints(m_simConfig, numPersons, percentHome);

	for (unsigned long i = 0; i < numPersons; i++) {
		unsigned long id = IDGenerator::instance()->next();
		double speed = walk_car[i] ? speeds_car[cars++] : speeds_walk[walks++];
		Person* p = new Person(m_simConfig->getMap(), id, positions[i], m_simConfig->getClock(), speed, ages[i],
				gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, m_simConfig->getStay(), m_simConfig->getIntevalBetweenStays());
		if(m_simConfig->isHomeWorkScenario() || m_simConfig->isHomeWorkManhattanScenario()) {
			 bool homePerson = RandomNumberGenerator::instance()->generateBernoulliInt(percentHome);
			 if( homePerson) {
				 setHomePersonHWLocations(p, positions[i]);
			 }
		}
		int np1 = phone1[i];
		while (np1) {
			addMobilePhoneToPerson(p, m_simConfig->getMnos()[0], m_agents);
			np1--;
		}
		if ( m_simConfig->getMnos().size() == 2) {
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


void PersonsConfigParser::setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng ) {
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


int* PersonsConfigParser::generateAges(int n, shared_ptr<Distribution> distr, RandomNumberGenerator* rng) {
	int* ages = new int[n];
	ages = rng->generateInt(n, distr.get());
	return (ages);
}

void PersonsConfigParser::addMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag) {
	unsigned long id = IDGenerator::instance()->next();
	MobilePhone* mp = new MobilePhone(m_simConfig->getMap(), id, nullptr, nullptr, m_simConfig->getClock(), m_simConfig->getConnThreshold(), m_simConfig->getConnType());
	mp->setMobileOperator(mno);
	mp->setHolder(p);
	ag->addAgent(mp);
	p->addDevice(typeid(MobilePhone).name(), mp);
}


void PersonsConfigParser::setPersonDisplacementPattern(Person* p) {
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
	} else if (type == MovementType::HOME_WORK) {
		if(p->isHomePerson()) {
			auto displace1 = std::make_shared<HomeWorkDisplacement>(m_simConfig, p->getSpeed(), p->getHomeLocation(), p->getWorkLocation(), p->getAnchorLocation());
			p->setDisplacementMethod(displace1);
		}
		else {
			auto displace2 = std::make_shared<RandomWalkDisplacement>(m_simConfig, p->getSpeed());
			p->setDisplacementMethod(displace2);
		}
	} else if (type == MovementType::MANHATTAN) {
		auto displace = std::make_shared<ManhattanDisplacement>(m_simConfig, p->getSpeed());
		p->setDisplacementMethod(displace);
	} else if (type == MovementType::HOME_WORK_MANHATTAN) {
		if(p->isHomePerson()) {
			auto displace1 = std::make_shared<HomeWorkDisplacement>(m_simConfig, p->getSpeed(), p->getHomeLocation(), p->getWorkLocation(), p->getAnchorLocation());
			p->setDisplacementMethod(displace1);
		}
		else {
			auto displace2 = std::make_shared<ManhattanDisplacement>(m_simConfig, p->getSpeed());
			p->setDisplacementMethod(displace2);
		}
	}
}


Point* PersonsConfigParser::generateLocation(unsigned int index, vector<HomeWorkLocation> locations) {
	Point* result = nullptr;
	double angle = RandomNumberGenerator::instance()->generateUniformDouble(0, 2.0 * utils::PI);
	HomeWorkLocation wl = locations.at(index);
	double L = sqrt( 1.0/(pow(cos(angle),2)/wl.getSdX() + pow(sin(angle),2)/wl.getSdY()) );
	double l = RandomNumberGenerator::instance()->generateUniformDouble(0, L);
	double x = wl.getSdX() + l * cos(angle);
	double y = wl.getSdY() + l * sin(angle);
	Coordinate c1(x, y, wl.getZ());
	result = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);

	return result;
}

void PersonsConfigParser::setHomePersonHWLocations(Person* p, Point* pt) {
	 Point* hl = m_simConfig->getMap()->getGlobalFactory()->createPoint(pt->getCoordinates());
	 p->setHomeLocation(hl);
	 int workLocationIndex = RandomNumberGenerator::instance()->generateUniformInt(0,  m_simConfig->getNumWorkLocations() - 1);
	 vector<HomeWorkLocation> hwl;
	 if(m_simConfig->isHomeWorkScenario()) {
		hwl = m_simConfig->getHomeWorkScenario()->getWorkLocations();
	 }
	 else if(m_simConfig->isHomeWorkManhattanScenario()) {
		 hwl = m_simConfig->getHomeWorkManhattanScenario()->getWorkLocations();
	 }
	 Point* workLocation = generateLocation(workLocationIndex, hwl);
	 p->setWorkLocation(workLocation);
	if (m_simConfig->getNumAnchorLocations() > 0) {
		double probAnchor = 0.0;
		if(m_simConfig->isHomeWorkScenario())
			probAnchor = m_simConfig->getHomeWorkScenario()->getProbAnchorPoint();
		else if(m_simConfig->isHomeWorkManhattanScenario())
			probAnchor = m_simConfig->getHomeWorkManhattanScenario()->getProbAnchorPoint();
		int goAnchor = RandomNumberGenerator::instance()->generateBernoulliInt(probAnchor);
		if (goAnchor) {
			int anchorLocationIndex = RandomNumberGenerator::instance()->generateUniformInt(0, m_simConfig->getNumAnchorLocations() - 1);
			vector<HomeWorkLocation> apl;
			if(m_simConfig->isHomeWorkScenario())
				apl = m_simConfig->getHomeWorkScenario()->getAnchorLocations();
			else if(m_simConfig->isHomeWorkManhattanScenario())
				apl = m_simConfig->getHomeWorkManhattanScenario()->getAnchorLocations();
			Point *anchorLocation = generateLocation(anchorLocationIndex, apl);
			p->setAnchorLocation(anchorLocation);
		}
	}
	 p->setIntervalBetweenStaysDistribution(nullptr);
	 p->setTimeStayDistribution(nullptr);
}
