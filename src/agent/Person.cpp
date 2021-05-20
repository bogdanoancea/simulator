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
#include <Distribution.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <map/Map.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <utility>
#include <HomeWorkDisplacement.h>
using namespace geos;
using namespace geos::geom;

Person::Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gen, shared_ptr<Distribution> timeStayDistribution,
		shared_ptr<Distribution> intervalBetweenStaysDistribution) :
		MovableAgent(m, id, initPosition, clock, initSpeed), m_age { age }, m_gender { gen }, m_timeStayDistribution { timeStayDistribution }, m_intervalBetweenStaysDistribution { intervalBetweenStaysDistribution } {
	m_displacementMethod = nullptr;
	m_homeLocation = nullptr;
	m_workLocation = nullptr;
	m_anchorLocation = nullptr;
	unsigned long interval = 0;
	if(m_intervalBetweenStaysDistribution)
		interval = (unsigned long )abs(RandomNumberGenerator::instance()->generateDouble(m_intervalBetweenStaysDistribution.get()));
	m_nextStay = getClock()->getCurrentTime() + interval;

	while (m_nextStay % getClock()->getIncrement() != 0)
		m_nextStay++;

	m_timeStay = 0;
	if(m_timeStayDistribution)
		m_timeStay = (unsigned long) abs(RandomNumberGenerator::instance()->generateInt(m_timeStayDistribution.get()));
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


const string Person::toString(bool detailed) const {
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
			unsigned long nextInterval = 0;
			if(m_intervalBetweenStaysDistribution)
				nextInterval = (unsigned long) abs( RandomNumberGenerator::instance()->generateDouble(m_intervalBetweenStaysDistribution.get()) );
			while (nextInterval % getClock()->getIncrement() != 0)
				nextInterval++;
			m_nextStay = currentTime + nextInterval;
			if(m_timeStayDistribution)
				m_timeStay = (unsigned long) abs(RandomNumberGenerator::instance()->generateDouble(m_timeStayDistribution.get()));
			else
				m_timeStay = 0;
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
//	if(getId() == 426) {
//		shared_ptr<Displace> dpl = m_displacementMethod;
//		cout << "locatia:" << getLocation()->toString() << "state " << static_cast<int>(((HomeWorkDisplacement*)dpl.get())->getState()) << "delta stay anchor: " << ((HomeWorkDisplacement*)dpl.get())->getDeltaTStayAnchor() <<  endl;
//	}
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

void Person::setAnchorLocation(Point* al) {
	m_anchorLocation = al;
}

Point* Person::getAnchorLocation() const{
	return m_anchorLocation;
}


Point* Person::getWorkLocation() const{
	return m_workLocation;
}


bool Person::isHomePerson() const {
	return m_homeLocation != nullptr;
}

const shared_ptr<Distribution>& Person::getIntervalBetweenStaysDistribution() const {
	return m_intervalBetweenStaysDistribution;
}

void Person::setIntervalBetweenStaysDistribution(const shared_ptr<Distribution> &intervalBetweenStaysDistribution) {
	m_intervalBetweenStaysDistribution = intervalBetweenStaysDistribution;
	if(!m_intervalBetweenStaysDistribution)  {
		m_nextStay = 0;
	}
}

const shared_ptr<Distribution>& Person::getTimeStayDistribution() const {
	return m_timeStayDistribution;
}

void Person::setTimeStayDistribution(const shared_ptr<Distribution> &timeStayDistribution) {
	m_timeStayDistribution = timeStayDistribution;
	if(!m_timeStayDistribution)
		m_timeStay = 0;
}
