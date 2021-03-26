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
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/HoldableAgent.h>
#include <agent/Person.h>
#include <Clock.h>
#include <Constants.h>
#include <geos/geom/Point.h>
#include <map/Map.h>
#include <RandomNumberGenerator.h>
#include <iomanip>
#include <sstream>
#include <utility>

using namespace geos;
using namespace geos::geom;

Person::Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gen, unsigned long timeStay,
		unsigned long intervalBetweenStays) :
		MovableAgent(m, id, initPosition, clock, initSpeed), m_age { age }, m_gender { gen }, m_avgTimeStay { timeStay }, m_avgIntervalBetweenStays { intervalBetweenStays } {
	m_displacementMethod = nullptr;
	m_homeLocation = nullptr;
	m_workLocation = nullptr;
	m_workLocationIndex = -1;
	m_isHomePerson = false;
	m_nextStay = getClock()->getCurrentTime() + intervalBetweenStays;
	while (m_nextStay % getClock()->getIncrement() != 0)
		m_nextStay++;

	m_timeStay = timeStay;
	while (m_timeStay % getClock()->getIncrement() != 0)
		m_timeStay++;

}

Person::~Person() {
	if(m_homeLocation) {
		getMap()->getGlobalFactory()->destroyGeometry(m_homeLocation);
	}
	if(m_workLocation) {
		getMap()->getGlobalFactory()->destroyGeometry(m_workLocation);
	}

}


void Person::setAge(int age) {
	m_age = age;
}

const string Person::toString() const {
	ostringstream ss;
	ss << MovableAgent::toString() << left << setw(15) << m_age << setw(15) << (m_gender == Person::Gender::MALE ? "Male" : "Female");

	if (m_idDevices.size() > 0) {
		for (pair<string, Agent*> i : m_idDevices) {
			ss << setw(15) << (i.second->getId());
		}
	}

	return (ss.str());
}

Point* Person::move() {
	unsigned long currentTime = getClock()->getCurrentTime();
	Point* currentLocation = getLocation();
	if (currentTime >= m_nextStay && currentTime <= m_nextStay + m_timeStay) {
		setLocation(currentLocation);
		if (currentTime == m_nextStay + m_timeStay) {
			unsigned long nextInterval = (unsigned long) RandomNumberGenerator::instance()->generateExponentialDouble(1.0 / this->m_avgIntervalBetweenStays);
			while (nextInterval % getClock()->getIncrement() != 0)
				nextInterval++;

			m_nextStay = currentTime + nextInterval;
			m_timeStay = (unsigned long) RandomNumberGenerator::instance()->generateNormalDouble(m_avgTimeStay, 0.2 * m_avgTimeStay);
			while (m_timeStay % getClock()->getIncrement() != 0)
				m_timeStay++;

		}
	} else {
		Point* pt = m_displacementMethod->generateNewLocation(currentLocation);
		if (pt != currentLocation) {
			this->getMap()->getGlobalFactory()->destroyGeometry(currentLocation);
		}
		setLocation(pt);
	}

	return (getLocation());
}


bool Person::hasDevices() {
	return (m_idDevices.size() > 0);
}

void Person::setLocation(Point* location) {
	LocatableAgent::setLocation(location); //
	if (hasDevices()) {
		unordered_multimap<string, Agent*>::iterator it;
		for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
			Agent* a = it->second;
			HoldableAgent* device = dynamic_cast<HoldableAgent*>(a);
			if (device != nullptr) {
				device->setLocation(location);
			}
		}
	}
}


string Person::dumpDevices() {
	stringstream ss;
	char sep = Constants::sep;
	unordered_multimap<string, Agent*>::iterator it;
	for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
		ss << sep << it->second->getId();
	}
	return (ss.str());
}

void Person::addDevice(string type, Agent* agent) {
	m_idDevices.insert(std::pair<string, Agent*>(type, agent));
}

void Person::setDisplacementMethod(const std::shared_ptr<Displace>& displace) {
	m_displacementMethod = displace;
}

const string Person::getHeader()  {
	ostringstream result;
	result << left << std::setw(15) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15) << " Age" << setw(15) << "Gender" << setw(15)
			<< "Phone(s) ID" << endl;
	return result.str();

}

void Person::setHomeLocation(Point* hl) {
	m_homeLocation = hl;
}

Point* Person::getHomeLocation() const{
	return m_homeLocation;
}


void Person::setWorkLocation(Point* wl) {
	m_workLocation = wl;
}

void Person::setWorkLocationIndex(unsigned index) {
	m_workLocationIndex = index;
}

unsigned int Person::getWorkLocationIndex() const {
	return m_workLocationIndex;
}

void Person::setHomePerson(bool home) {
	m_isHomePerson = home;
}
bool Person::isHomePerson() const {
	return m_isHomePerson;
}
